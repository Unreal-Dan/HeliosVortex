#include <stdint.h>

#include "Helios.h"

#include "ColorConstants.h"
#include "TimeControl.h"
#include "Storage.h"
#include "Pattern.h"
#include "Random.h"
#include "Button.h"
#include "Led.h"

#ifdef HELIOS_EMBEDDED
#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#endif

#ifdef HELIOS_CLI
#include <stdio.h>
#endif

#include <stdlib.h>

Helios::State Helios::cur_state;
Helios::Flags Helios::global_flags;
uint8_t Helios::menu_selection;
uint8_t Helios::cur_mode;
uint8_t Helios::selected_slot;
uint8_t Helios::selected_base_quad;
uint8_t Helios::selected_hue;
uint8_t Helios::selected_sat;
Pattern Helios::pat;
bool Helios::keepgoing;

#ifdef HELIOS_CLI
bool Helios::sleeping;
#endif

bool Helios::init()
{
  // initialize the time control and led control
  if (!Time::init()) {
    return false;
  }
  if (!Led::init()) {
    return false;
  }
  if (!Storage::init()) {
    return false;
  }
  if (!Button::init()) {
    return false;
  }

  // initialize globals
  cur_state = STATE_MODES;
  menu_selection = 0;
  cur_mode = 0;
  selected_slot = 0;
  selected_base_quad = 0;
  selected_hue = 0;
  selected_sat = 0;
  keepgoing = true;
#ifdef HELIOS_CLI
  sleeping = false;
#endif

  // read the global flags from index 0 config
  global_flags = (Flags)Storage::read_config(0);
  if (has_flag(FLAG_CONJURE)) {
    // if conjure is enabled then load the current mode index from storage
    cur_mode = Storage::read_config(1);
  }

  // load the current mode from store and initialize it
  load_cur_mode();

#ifdef HELIOS_EMBEDDED
  // Timer0 setup for 1 kHz interrupt
  TCCR0A |= (1 << WGM01);
#if F_CPU == 16000000L
  // 1ms at 16MHz clock with prescaler of 64
  OCR0A = 249;
#elif F_CPU == 8000000L
  // 1ms at 8mhz clock with prescaler of 64
  OCR0A = 124;
#endif
  TIMSK |= (1 << OCIE0A);
  // Start timer with prescaler of 64
  TCCR0B |= (1 << CS01) | (1 << CS00);
  // enable interrupts
  sei();
#endif
  return true;
}

#ifdef HELIOS_EMBEDDED
ISR(TIM0_COMPA_vect) {
  // 1 kHz system tick
  Helios::tick();
}
#endif

void Helios::tick()
{
  // sample the button and re-calculate all button globals
  // the button globals should not change anywhere else
  Button::update();

  // handle the current state of the system, ie whatever state
  // we're in we check for the appropriate input events for that
  // state by checking button globals, then run the appropriate logic
  handle_state();

  // NOTE: Do not update the LED here anymore, instead we call Led::update()
  //       in the tight loop inside main() where it can perform software PWM
  //       on the LED pins at a much higher frequency

  // finally tick the clock forward and then sleep till the entire
  // tick duration has been consumed
  Time::tickClock();
}

void Helios::enter_sleep()
{
  // clear the led
  Led::clear();
#ifdef HELIOS_EMBEDDED
  // init the output pins to prevent any floating pins
  clear_output_pins();
  // Enable wake on interrupt for the button
  Button::enableWake();
  // Set sleep mode to POWER DOWN mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  // enter sleep
  sleep_mode();
#else
  cur_state = STATE_SLEEP;
  // enable the sleep bool
  sleeping = true;
#endif
}

#ifdef HELIOS_EMBEDDED
void Helios::clear_output_pins() {
  // TODO: turn off any peripherals and stop floating pins
}
#endif

void Helios::wakeup() {
#ifdef HELIOS_EMBEDDED
  // re-initialize helios
  // TODO: should CLI do this too?
  init();
#else
  sleeping = false;
#endif
}

void Helios::handle_state()
{
  // check for the force sleep button hold regardless of which state we're in
  if (Button::holdDuration() > FORCE_SLEEP_TIME) {
    // when released the device will just sleep
    if (Button::onRelease()) {
      enter_sleep();
    }
    // but as long as it's held past the sleep time it just turns off the led
    if (Button::isPressed()) {
      Led::clear();
      return;
    }
  }
  // otherwise just handle the state like normal
  switch (cur_state) {
    case STATE_MODES:
      handle_state_modes();
      break;
    case STATE_COLOR_SELECT_SLOT:
    case STATE_COLOR_SELECT_QUADRANT:
    case STATE_COLOR_SELECT_HUE:
    case STATE_COLOR_SELECT_SAT:
    case STATE_COLOR_SELECT_VAL:
      handle_state_col_select();
      break;
    case STATE_PATTERN_SELECT:
      handle_state_pat_select();
      break;
    case STATE_TOGGLE_CONJURE:
      handle_state_toggle_flag(FLAG_CONJURE);
      break;
    case STATE_TOGGLE_LOCK:
      handle_state_toggle_flag(FLAG_LOCKED);
      break;
    case STATE_SET_DEFAULTS:
      handle_state_set_defaults();
      break;
    case STATE_SET_GLOBAL_BRIGHTNESS:
      handle_state_set_global_brightness();
      break;
    case STATE_SHIFT_MODE:
      handle_state_shift_mode();
      break;
    case STATE_RANDOMIZE:
      handle_state_randomize();
      break;
#ifdef HELIOS_CLI
    case STATE_SLEEP:
      // simulate sleep in helios CLI
      if (Button::onPress()) {
        wakeup();
      }
      break;
#endif
  }
}

void Helios::load_next_mode()
{
  // increment current mode and wrap around
  cur_mode = (uint8_t)(cur_mode + 1) % NUM_MODE_SLOTS;
  // now load current mode again
  load_cur_mode();
}

void Helios::load_cur_mode()
{
  // read pattern from storage at cur mode index
  if (!Storage::read_pattern(cur_mode, pat)) {
    // and just initialize default if it cannot be read
    Patterns::make_default(cur_mode, pat);
    // try to write it out because storage was corrupt
    Storage::write_pattern(cur_mode, pat);
  }
  // then re-initialize the pattern
  pat.init();
}

void Helios::save_cur_mode()
{
  Storage::write_pattern(cur_mode, pat);
}

void Helios::handle_state_modes()
{
  // whether they have released the button since turning on
  bool hasReleased = (Button::releaseCount() > 0);

  if (Button::releaseCount() > 1 && Button::onShortClick()) {
    if (has_flag(FLAG_CONJURE)) {
      enter_sleep();
    } else {
      load_next_mode();
    }
    return;
  }

  // check for lock and go back to sleep
  if (has_flag(FLAG_LOCKED) && hasReleased && !Button::onRelease()) {
    enter_sleep();
    return;
  }

  if (!has_flag(FLAG_LOCKED) && hasReleased) {
    // just play the current mode
    pat.play();
  }

  // check how long the button is held
  uint16_t holdDur = (uint16_t)Button::holdDuration();
  // calculate a magnitude which corresponds to how many times past the MENU_HOLD_TIME
  // the user has held the button, so 0 means haven't held fully past one yet, etc
  uint8_t magnitude = (uint8_t)(holdDur / MENU_HOLD_TIME);
  // whether the user has held the button longer than a short click
  bool heldPast = (holdDur > SHORT_CLICK_THRESHOLD);
  // if the button is held for at least 1 second
  if (Button::isPressed() && heldPast) {
    // if the button has been released before then show the on menu
    if (hasReleased) {
      switch (magnitude) {
        case 0:
          Led::clear();
          break;
        case 1: // Color Selection
          Led::set(RGB_TURQUOISE_BRI_LOW);
          break;
        case 2: // Pattern Selection
          Led::set(RGB_MAGENTA_BRI_LOW);
          break;
        case 3: // Conjure Mode
          Led::set(RGB_YELLOW_BRI_LOW);
          break;
        case 4: // Shift Mode
          Led::set(RGB_WHITE_BRI_LOW);
          break;
        case 5: // Randomizer
          Led::set(HSVColor(Time::getCurtime(), 255, 180));
          break;
        default:
          Led::clear();
          break;
      }
    } else {
      // Hold for 5 Seconds to Exit Lock
      if (has_flag(FLAG_LOCKED)) {
        switch (magnitude) {
          case 0:
            Led::clear();
            break;
          case 5:  // Exit Lock
            Led::set(RGB_RED_BRI_LOW);
            break;
          default:
            Led::clear();
            break;
        }
      } else {
        switch (magnitude) {
          case 0:
            Led::clear();
            break;
          case 1:  // Enter Glow Lock
            Led::set(RGB_RED_BRI_LOW);
            break;
          case 2:  // Master Reset
            Led::set(RGB_BLUE_BRI_LOW);
            break;
          case 3:  // Global Brightness
            Led::set(RGB_GREEN_BRI_LOW);
            break;
          default:
            Led::clear();
            break;
        }
      }
    }
  }
  // if this isn't a release tick there's nothing more to do
  if (Button::onRelease()) {
    if (heldPast && Button::releaseCount() == 1) {
      handle_off_menu(magnitude, heldPast);
      return;
    }
    // otherwise if we have released it then we are in the 'on' menu
    handle_on_menu(magnitude, heldPast);
  }
}

void Helios::handle_off_menu(uint8_t mag, bool past)
{
  // if we have not released the button yet we're in the 'off' menu
  if (has_flag(FLAG_LOCKED)) {
    switch (mag) {
      case 5:  // red lock
        cur_state = STATE_TOGGLE_LOCK;
        break;
      default:
        // just go back to sleep in hold-paste off menu
        enter_sleep();
        break;
    }
  } else {
    switch (mag) {
      case 1:  // red lock
        cur_state = STATE_TOGGLE_LOCK;
        Led::clear();
        break;
      case 2:  // blue reset defaults
        cur_state = STATE_SET_DEFAULTS;
        break;
      case 3:  // green global brightness
        cur_state = STATE_SET_GLOBAL_BRIGHTNESS;
        break;
      default:
        // just go back to sleep in hold-paste off menu
        enter_sleep();
        break;
    }
  }
}

void Helios::handle_on_menu(uint8_t mag, bool past)
{
  switch (mag) {
    case 0:  // off
      // but only if we held for more than a short click
      if (past) {
        enter_sleep();
      }
      break;
    case 1:  // color select
      cur_state = STATE_COLOR_SELECT_SLOT;
      // reset the menu selection
      menu_selection = 0;
#if ALTERNATIVE_HSV_RGB == 1
      // use the nice hue to rgb rainbow
      g_hsv_rgb_alg = HSV_TO_RGB_RAINBOW;
#endif
      break;
    case 2:  // pat select
      cur_state = STATE_PATTERN_SELECT;
      // reset the menu selection
      menu_selection = 0;
      break;
    case 3:  // conjure mode
      cur_state = STATE_TOGGLE_CONJURE;
      break;
    case 4:  // shift mode down
      cur_state = STATE_SHIFT_MODE;
      break;
    case 5:  // ???
      cur_state = STATE_RANDOMIZE;
      break;
    default:  // hold past
      break;
  }
}

void Helios::handle_state_col_select()
{
  if (Button::onShortClick()) {
    // next hue/sat/val selection
    uint8_t num_menus = 4;
    if (cur_state == STATE_COLOR_SELECT_SLOT) {
      uint8_t num_cols = pat.colorset().numColors();
      // menus = all colors + exit
      num_menus = num_cols + 1;
      // but if the num cols is less than total color slots
      if (num_cols < NUM_COLOR_SLOTS) {
        // then we have another menu: add color
        num_menus++;
      }
    } else if (cur_state == STATE_COLOR_SELECT_QUADRANT) {
      num_menus = 7;
    }
    menu_selection = (menu_selection + 1) % num_menus;
  }
  bool check_longclick = true;
  switch (cur_state) {
    default:
    case STATE_COLOR_SELECT_SLOT:
      // pick the target colorset slot
      check_longclick = handle_state_col_select_slot();
      break;
    case STATE_COLOR_SELECT_QUADRANT:
      // pick the hue quadrant
      check_longclick = handle_state_col_select_quadrant();
      break;
    case STATE_COLOR_SELECT_HUE:
      // target hue for changes
      check_longclick = handle_state_col_select_hue();
      break;
    case STATE_COLOR_SELECT_SAT:
      // target sat for changes
      check_longclick = handle_state_col_select_sat();
      break;
    case STATE_COLOR_SELECT_VAL:
      // target val for changes
      check_longclick = handle_state_col_select_val();
      break;
  }
  if (check_longclick && Button::onLongClick()) {
    if (cur_state == STATE_COLOR_SELECT_VAL) {
      cur_state = STATE_COLOR_SELECT_SLOT;
    } else {
      cur_state = (State)(cur_state + 1);
    }
    menu_selection = 0;
  }
  // get the current color
  RGBColor cur = Led::get();
  cur.red /= 2;
  cur.green /= 2;
  cur.blue /= 2;
  // show selection in all of these menus
  show_selection(cur);
}

bool Helios::handle_state_col_select_slot()
{
  Colorset &set = pat.colorset();
  uint8_t num_cols = set.numColors();

  bool long_click = Button::onLongClick();

  if (num_cols < NUM_COLOR_SLOTS && menu_selection == num_cols) {
    // add color
    Led::strobe(100, 100, RGB_WHITE_BRI_LOW, RGB_OFF);
    if (long_click) {
      selected_slot = menu_selection;
    }
  } else if (menu_selection == num_cols + 1 || (num_cols == NUM_COLOR_SLOTS && menu_selection == num_cols)) {
    // exit
    Led::strobe(60, 40, RGB_RED_BRI_LOW, RGB_OFF);
    if (long_click) {
#if ALTERNATIVE_HSV_RGB == 1
      // restore hsv to rgb algorithm type, done color selection
      g_hsv_rgb_alg = HSV_TO_RGB_GENERIC;
#endif
      save_cur_mode();
      cur_state = STATE_MODES;
      return false;
    }
  } else {
    // render current selection
    RGBColor col = set.get(menu_selection);
    Led::set(col);
    uint16_t hold_dur = (uint16_t)Button::holdDuration();
    bool deleting = ((hold_dur > DELETE_COLOR_TIME) &&
                     ((hold_dur % (DELETE_COLOR_TIME * 2)) > DELETE_COLOR_TIME));
    if (deleting) {
      if (Button::isPressed()) {
        // flash red
        Led::strobe(150, 150, RGB_RED_BRI_LOW, col);
      }
      if (long_click) {
        set.removeColor(menu_selection);
        return false;
      }
    }
  }
  return true;
}

struct QuadMenuData {
  uint8_t hue1;
  uint8_t hue2;
  uint16_t on_dur;
  uint16_t off_dur;
};

QuadMenuData menu_data[4] = {
  // hue1                hue2              on   off
  // ===============================================================
  {HSV_HUE_RED,          HSV_HUE_ORANGE,   60,  40},
  {HSV_HUE_LIME_GREEN,   HSV_HUE_SEAFOAM,  5,   30},
  {HSV_HUE_ICE_BLUE,     HSV_HUE_BLUE,     9,   0},
  {HSV_HUE_PURPLE,       HSV_HUE_HOT_PINK, 500, 500}
};

bool Helios::handle_state_col_select_quadrant()
{
  uint8_t hue_quad = (menu_selection - 3) % 4;
  HSVColor hcol(menu_data[hue_quad].hue1, 255, 255);
  RGBColor color_values[3] = {RGB_RED_BRI_LOW, RGB_WHITE_BRI_LOWEST, RGB_WHITE};

  if (Button::onLongClick()) {
    // select hue/sat/val
    switch (menu_selection) {
      case 0:  // exit
        cur_state = STATE_MODES;
        return false;
      case 1:  // selected blank
        // add blank to set
        pat.colorset().set(selected_slot, RGB_OFF);
        // go to slot selection - 1 because we will increment outside here
        cur_state = STATE_COLOR_SELECT_SLOT;
        return false;
      case 2:  // selected white
        // adds white, skip hue/sat to brightness
        selected_hue = 0;
        selected_sat = 0;
        cur_state = STATE_COLOR_SELECT_VAL;
        return false;
      default:  // 3-6
        selected_base_quad = hue_quad;
        break;
    }
  }

  // default col1/col2 to off and white for the first two options
  RGBColor col1 = RGB_OFF;
  RGBColor col2;

  uint16_t on_dur = menu_data[3].on_dur;
  uint16_t off_dur = menu_data[3].off_dur;

  if (menu_selection < 3) {
    col2 = color_values[menu_selection];
    on_dur = menu_data[menu_selection].on_dur;
    off_dur = menu_data[menu_selection].off_dur;
  } else {
    col1 = HSVColor(menu_data[hue_quad].hue1, 255, 255);
    col2 = HSVColor(menu_data[hue_quad].hue2, 255, 255);
  }

  Led::strobe(on_dur, off_dur, col1, col2);
  return true;
}

struct ColorsMenuData {
  uint8_t hues[4];
};
// array of hues for selection
static const ColorsMenuData color_menu_data[4] = {
  // hue0               hue1                  hue2              hue3
  // ==================================================================================
  { HSV_HUE_RED,        HSV_HUE_CORAL_ORANGE, HSV_HUE_ORANGE,   HSV_HUE_YELLOW },
  { HSV_HUE_LIME_GREEN, HSV_HUE_GREEN,        HSV_HUE_SEAFOAM,  HSV_HUE_TURQUOISE },
  { HSV_HUE_ICE_BLUE,   HSV_HUE_LIGHT_BLUE,   HSV_HUE_BLUE,     HSV_HUE_ROYAL_BLUE },
  { HSV_HUE_PURPLE,     HSV_HUE_PINK,         HSV_HUE_HOT_PINK, HSV_HUE_MAGENTA },
};

bool Helios::handle_state_col_select_hue()
{
  uint8_t hue = color_menu_data[selected_base_quad].hues[menu_selection];
  if (Button::onLongClick()) {
    // select hue/sat/val
    selected_hue = hue;
  }
  // render current selection
  Led::set(HSVColor(hue, 255, 255));
  return true;
}

bool Helios::handle_state_col_select_sat()
{
  if (menu_selection > 3) {
    menu_selection = 3;
  }
  uint8_t saturation_values[4] = {HSV_SAT_HIGH, HSV_SAT_MEDIUM, HSV_SAT_LOW, HSV_SAT_LOWEST};
  uint8_t sat = saturation_values[menu_selection];

  // use the nice hue to rgb rainbow
  if (Button::onLongClick()) {
    // select hue/sat/val
    selected_sat = sat;
  }
  // render current selection
  Led::set(HSVColor(selected_hue, sat, 255));
  return true;
}

bool Helios::handle_state_col_select_val()
{
  if (menu_selection > 3) {
    menu_selection = 3;
  }
  uint8_t brightness_values[4] = {HSV_BRI_HIGH, HSV_BRI_MEDIUM, HSV_BRI_LOW, HSV_BRI_LOWEST};
  uint8_t val = brightness_values[menu_selection];

  RGBColor targetCol = HSVColor(selected_hue, selected_sat, val);
  // use the nice hue to rgb rainbow
  if (Button::onLongClick()) {
    // change the current patterns color
    pat.colorset().set(selected_slot, targetCol);
    pat.init();
    save_cur_mode();
  }
  // render current selection
  Led::set(targetCol);
  return true;
}

void Helios::handle_state_pat_select()
{
  if (Button::onLongClick()) {
    save_cur_mode();
    cur_state = STATE_MODES;
  }
  if (Button::onShortClick()) {
    menu_selection = (menu_selection + 1) % PATTERN_COUNT;
    Patterns::make_pattern((PatternID)menu_selection, pat);
    pat.init();
  }
  pat.play();
  show_selection(RGB_MAGENTA_BRI_LOW);
}

void Helios::handle_state_toggle_flag(Flags flag)
{
  // toggle the conjure flag
  toggle_flag(flag);
  // write out the new global flags and the current mode
  save_global_flags();
  // switch back to modes
  cur_state = STATE_MODES;
}

void Helios::handle_state_set_defaults()
{
  if (Button::onShortClick()) {
    menu_selection = !menu_selection;
  }
  // show low white for exit or red for select
  if (menu_selection) {
    Led::strobe(20, 10, RGB_RED_BRI_LOW, RGB_OFF);
  } else {
    Led::strobe(60, 20, RGB_WHITE_BRI_LOWEST, RGB_OFF);
  }
  // when the user long clicks a selection
  if (Button::onLongClick()) {
    // if the user actually selected 'yes'
    if (menu_selection == 1) {
      for (uint8_t i = 0; i < NUM_MODE_SLOTS; ++i) {
        Patterns::make_default(i, pat);
        Storage::write_pattern(i, pat);
      }
      // reset global flags
      global_flags = FLAG_NONE;
      cur_mode = 0;
      // save global flags
      save_global_flags();
      // re-load current mode
      load_cur_mode();
    }
    cur_state = STATE_MODES;
  }
  show_selection(RGB_WHITE_BRI_LOW);
}

void Helios::handle_state_set_global_brightness()
{
  if (Button::onShortClick()) {
    menu_selection = (menu_selection + 1) % 3;
  }
  // show different levels of green for each selection
  switch (menu_selection) {
    case 0:
      Led::set(RGB_GREEN);
      break;
    case 1:
      Led::set(RGB_GREEN_BRI_MEDIUM);
      break;
    case 2:
      Led::set(RGB_GREEN_BRI_LOW);
      break;
  }
  // when the user long clicks a selection
  if (Button::onLongClick()) {
    // set the brightness based on the selection
    switch (menu_selection) {
      case 0:
        Led::setBrightness(255);
        break;
      case 1:
        Led::setBrightness(170);
        break;
      case 2:
        Led::setBrightness(85);
        break;
    }
    cur_state = STATE_MODES;
  }
  show_selection(RGB_WHITE_BRI_LOW);
}

inline uint32_t crc32(const uint8_t *data, uint8_t size)
{
  uint32_t hash = 5381;
  for (uint8_t i = 0; i < size; ++i) {
    hash = ((hash << 5) + hash) + data[i];
  }
  return hash;
}

void Helios::handle_state_shift_mode()
{
  uint8_t new_mode = cur_mode ? (uint8_t)(cur_mode - 1) : (uint8_t)(NUM_MODE_SLOTS - 1);
  Storage::swap_pattern(cur_mode, new_mode);
  cur_mode = new_mode;
  cur_state = STATE_MODES;
}

void Helios::handle_state_randomize()
{
  if (Button::onShortClick()) {
    uint32_t seed = crc32((const uint8_t *)&pat, PATTERN_SIZE);
    Random ctx(seed);
    Colorset &cur_set = pat.colorset();
    uint8_t num_cols = (ctx.next8() + 1) % NUM_COLOR_SLOTS;
    cur_set.randomizeColors(ctx, num_cols, Colorset::THEORY);
    Patterns::make_pattern((PatternID)(ctx.next8() % PATTERN_COUNT), pat);
    pat.init();
  }
  if (Button::onLongClick()) {
    save_cur_mode();
    cur_state = STATE_MODES;
  }
  pat.play();
  show_selection(RGB_WHITE_BRI_LOW);
}

void Helios::save_global_flags()
{
  Storage::write_config(0, global_flags);
  Storage::write_config(1, cur_mode);
}

void Helios::show_selection(RGBColor color)
{
  // only show seletion while pressing the button
  if (!Button::isPressed()) {
    return;
  }
  uint32_t holdDur = Button::holdDuration();
  // if the hold duration is outside the flashing range do nothing
  if (holdDur < SHORT_CLICK_THRESHOLD ||
      holdDur > (SHORT_CLICK_THRESHOLD + SELECTION_FLASH_DURATION)) {
    return;
  }
  // set the selection color
  Led::set(color);
}
