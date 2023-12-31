#include "Helios.h"

#include <stdint.h>

#include "Button.h"
#include "ColorConstants.h"
#include "Led.h"
#include "Pattern.h"
#include "Random.h"
#include "Storage.h"
#include "TimeControl.h"

#ifdef HELIOS_EMBEDDED
#include <avr/interrupt.h>
#include <avr/sleep.h>
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
uint8_t Helios::selected_base_hue;
uint8_t Helios::selected_hue;
uint8_t Helios::selected_sat;
Pattern Helios::pat;
bool Helios::keepgoing;

#ifdef HELIOS_CLI
bool Helios::sleeping;
#endif

bool Helios::init() {
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
  selected_base_hue = 0;
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
  // 1ms at 8MHz clock with prescaler of 64
  OCR0A = 124;
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

void Helios::tick() {
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

void Helios::enter_sleep() {
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

void Helios::handle_state() {
  if (Button::holdDuration() > FORCE_SLEEP_TIME) {
    if (Button::onRelease()) {
      enter_sleep();
    }
    if (Button::isPressed()) {
      Led::clear();
      return;
    }
  }
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

void Helios::load_next_mode() {
  // increment current mode and wrap around
  cur_mode = (uint8_t)(cur_mode + 1) % NUM_MODE_SLOTS;
  // now load current mode again
  load_cur_mode();
}

void Helios::load_cur_mode() {
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

void Helios::save_cur_mode() {
  Storage::write_pattern(cur_mode, pat);
}

void Helios::handle_state_modes() {
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
  uint32_t holdDur = Button::holdDuration();
  // show a color based on the hold duration past 200
  // the magnitude will be some value from 0-3 corresponding
  // to the holdDurations of 200 to 500
  uint32_t magnitude = (holdDur / MENU_HOLD_TIME);
  bool heldPast = (holdDur > SHORT_CLICK_THRESHOLD);
  // if the button is held for at least 1 second
  if (Button::isPressed() && heldPast) {
    // if the button has been released before then show the on menu
    if (hasReleased) {
      switch (magnitude) {
        case 0:
          Led::clear();
          break;
        case 1:
          Led::set(HELIOS_RGB_TURQUOISE_BRI_MEDIUM);
          break;
        case 2:
          Led::set(HELIOS_RGB_MAGENTA_BRI_MEDIUM);
          break;
        case 3:
          Led::set(HELIOS_RGB_YELLOW_BRI_MEDIUM);
          break;
        case 4:
          Led::set(RGB_WHITE2);
          break;
        case 5:
          Led::set(HSVColor(Time::getCurtime(), 255, 180));
          break;
        default:
          Led::clear();
          break;
      }
    } else {
      // otherwise show the off menu
      switch (magnitude) {
        case 0:
          Led::clear();
          break;
        case 1:
          Led::set(HELIOS_RGB_RED_BRI_MEDIUM);
          break;
        case 2:
          Led::set(HELIOS_RGB_BLUE_BRI_MEDIUM);
          break;
        default:
          Led::clear();
          break;
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

void Helios::handle_off_menu(uint8_t mag, bool past) {
  // if we have not released the button yet we're in the 'off' menu
  switch (mag) {
    case 0:  // red lock
      cur_state = STATE_TOGGLE_LOCK;
      break;
    case 1:  // blue reset defaults
      cur_state = STATE_SET_DEFAULTS;
      break;
    default:
      break;
  }
}

void Helios::handle_on_menu(uint8_t mag, bool past) {
  switch (mag) {
    case 0:  // off
      // but only if we held for more than a short click
      if (past) {
        enter_sleep();
      }
      break;
    case 1:  // color select
      cur_state = STATE_COLOR_SELECT_SLOT;
      // use the nice hue to rgb rainbow
      // g_hsv_rgb_alg = HSV_TO_RGB_RAINBOW;
      // reset the menu selection
      menu_selection = 0;
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

void Helios::handle_state_col_select() {
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
      num_menus = 6;
    }
    menu_selection = (menu_selection + 1) % num_menus;
  }
  switch (cur_state) {
    default:
    case STATE_COLOR_SELECT_SLOT:
      // pick the target colorset slot
      if (!handle_state_col_select_slot()) {
        return;
      }
      break;
    case STATE_COLOR_SELECT_QUADRANT:
      // pick the hue quadrant
      if (!handle_state_col_select_quadrant()) {
        return;
      }
      break;
    case STATE_COLOR_SELECT_HUE:
      // target hue for changes
      if (!handle_state_col_select_hue()) {
        return;
      }
      break;
    case STATE_COLOR_SELECT_SAT:
      // target sat for changes
      if (!handle_state_col_select_sat()) {
        return;
      }
      break;
    case STATE_COLOR_SELECT_VAL:
      // target val for changes
      if (!handle_state_col_select_val()) {
        return;
      }
      break;
  }
  if (Button::onLongClick()) {
    if (cur_state == STATE_COLOR_SELECT_VAL) {
      cur_state = STATE_MODES;
    } else {
      cur_state = (State)(cur_state + 1);
    }
    menu_selection = 0;
  }
}

bool Helios::handle_state_col_select_slot() {
  Colorset &set = pat.colorset();
  uint8_t num_cols = set.numColors();
  if (Button::onLongClick()) {
    if (menu_selection == (num_cols + 1)) {
      // exit
      cur_state = STATE_MODES;
      return false;
    }
    selected_slot = menu_selection;
  }
  if (menu_selection == num_cols) {
    // add color
    Led::strobe(100, 100, RGB_WHITE2, RGB_OFF);
  } else if (menu_selection == num_cols + 1) {
    // exit
    Led::strobe(60, 40, HELIOS_RGB_RED_BRI_MEDIUM, RGB_OFF);
  } else {
    // render current selection
    Led::set(set.get(menu_selection));
  }
  return true;
}

bool Helios::handle_state_col_select_quadrant() {
  uint8_t hue_quad = (menu_selection - 2);
  uint8_t hue = hue_quad * (255 / 4);
  HSVColor hcol(hue, 255, 255);
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
        selected_base_hue = hue;
        break;
    }
  }
  // default col1/col2 to off and white for the first two options
  RGBColor col1 = RGB_OFF;
  RGBColor col2;
  switch (menu_selection) {
    case 0:  // exit
      col2 = HELIOS_RGB_RED_BRI_MEDIUM;
      break;
    case 1:  // blank
      col2 = RGB_WHITE1;
      break;
    case 2:  // white
      col2 = RGB_WHITE;
      break;
    default:  // colors
      col1 = hcol;
      hcol.hue += 32;
      col2 = hcol;
      break;
  }
  // render current selection
  Led::strobe(6, 10, col1, col2);
  return true;
}

bool Helios::handle_state_col_select_hue() {
  uint8_t hue = selected_base_hue + (menu_selection * 16);
  if (Button::onLongClick()) {
    // select hue/sat/val
    selected_hue = hue;
  }
  // render current selection
  Led::set(HSVColor(hue, 255, 255));
  return true;
}

bool Helios::handle_state_col_select_sat() {
  uint8_t sat = 255 - (menu_selection * 60);
  // use the nice hue to rgb rainbow
  if (Button::onLongClick()) {
    // select hue/sat/val
    selected_sat = sat;
  }
  // render current selection
  Led::set(HSVColor(selected_hue, sat, 255));
  return true;
}

bool Helios::handle_state_col_select_val() {
  uint8_t val = 255 - (menu_selection * 50);
  RGBColor targetCol = HSVColor(selected_hue, selected_sat, val);
  // use the nice hue to rgb rainbow
  if (Button::onLongClick()) {
    // change the current patterns color
    pat.colorset().set(selected_slot, targetCol);
    // restore hsv to rgb algorithm type, done color selection
    g_hsv_rgb_alg = HSV_TO_RGB_GENERIC;
  }
  // render current selection
  Led::set(targetCol);
  return true;
}

void Helios::handle_state_pat_select() {
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
  show_selection();
}

void Helios::handle_state_toggle_flag(Flags flag) {
  // toggle the conjure flag
  toggle_flag(flag);
  // write out the new global flags and the current mode
  save_global_flags();
  // switch back to modes
  cur_state = STATE_MODES;
}

void Helios::handle_state_set_defaults() {
  if (Button::onShortClick()) {
    menu_selection = !menu_selection;
  }
  // show low white for exit or red for select
  if (menu_selection) {
    Led::strobe(20, 10, HELIOS_RGB_RED_BRI_MEDIUM, RGB_OFF);
  } else {
    Led::strobe(60, 20, RGB_WHITE0, RGB_OFF);
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
  show_selection();
}

inline uint32_t crc32(const uint8_t *data, uint8_t size) {
  uint32_t hash = 5381;
  for (uint8_t i = 0; i < size; ++i) {
    hash = ((hash << 5) + hash) + data[i];
  }
  return hash;
}

void Helios::handle_state_shift_mode() {
  uint8_t new_mode = (cur_mode + 1) % NUM_MODE_SLOTS;
  Storage::swap_pattern(cur_mode, new_mode);
  cur_mode = new_mode;
  cur_state = STATE_MODES;
}

void Helios::handle_state_randomize() {
  if (Button::onShortClick()) {
    uint32_t seed = crc32((const uint8_t *)&pat, PATTERN_SIZE);
    Random ctx(seed);
    Colorset &cur_set = pat.colorset();
    uint8_t num_cols = (ctx.next8() + 1) % NUM_COLOR_SLOTS;
    cur_set.randomizeColors(ctx, num_cols, Colorset::THEORY);
    pat.init();
  }
  if (Button::onLongClick()) {
    save_cur_mode();
    cur_state = STATE_MODES;
  }
  pat.play();
  show_selection();
}

void Helios::save_global_flags() {
  Storage::write_config(0, global_flags);
  Storage::write_config(1, cur_mode);
}

void Helios::show_selection() {
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
  // set some sort of dim white
  Led::set(RGB_WHITE5);
}
