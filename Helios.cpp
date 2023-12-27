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

Helios::State Helios::cur_state = STATE_MODES;
Helios::Flags Helios::global_flags = FLAG_NONE;
uint8_t Helios::menu_selection = 0;
uint8_t Helios::cur_mode = 0xFF;
uint8_t Helios::selected_slot = 0;
uint8_t Helios::selected_base_hue = 0;
uint8_t Helios::selected_hue = 0;
uint8_t Helios::selected_sat = 0;
Pattern Helios::pat;
bool Helios::keepgoing = true;
bool Helios::sleeping = false;

bool Helios::init()
{    
#ifdef HELIOS_EMBEDDED
  // Clear the reset flag on the MCU status register
  MCUSR &= ~(1 << WDRF);
  // Disable the Watchdog Timer to prevent unwanted resets
  wdt_disable();
#endif

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
  
  // read the global flags from index 0 config
  Storage::read_config(0, (uint8_t &)global_flags);
  if (has_flag(FLAG_CONJURE)) {
    // set the current mode to the stored mode, which will actually
    // be the target mode minus 1 so that next_mode will iterate to
    // the correct target mode
    Storage::read_config(1, cur_mode);
  }

  if (has_flag(FLAG_LOCKED)) {
    // go back to sleep?
  }

  // iterate to the next mode (or first mode in this case)
  next_mode();

#ifdef HELIOS_EMBEDDED
  // Timer0 setup for 1 kHz interrupt
  TCCR0A |= (1 << WGM01);
  OCR0A = 124; // 1ms at 8MHz clock with prescaler of 64
  TIMSK |= (1 << OCIE0A);
  TCCR0B |= (1 << CS01) | (1 << CS00); // Start timer with prescaler of 64
  // enable interrupts
  sei();
#endif
  return true;
}

#ifdef HELIOS_EMBEDDED
ISR(TIM0_COMPA_vect)
{
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

void Helios::enter_sleep(bool save)
{
  //if (save) {
  //  // update the startup mode when going to sleep
  //  Modes::setStartupMode(Modes::curModeIndex());
  //  // save anything that hasn't been saved
  //  Modes::saveStorage();
  //}
  // clear all the leds
  Led::clear();
  Led::update();
#ifdef HELIOS_EMBEDDED
  // init the output pins to prevent any floating pins
  //clearOutputPins();
  // Enable wake on interrupt for the button
  Button::enableWake();
  // Set sleep mode to POWER DOWN mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  // enable the sleep boo lright before we enter sleep, this will allow
  // the main loop to break and return
  sleeping = true;
  // enter sleep
  sleep_mode();
#else
  cur_state = STATE_SLEEP;
  // enable the sleep bool
  sleeping = true;
#endif
}

void Helios::wakeup()
{
  // re-initialize helios
  init();
}

void Helios::handle_state()
{
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
  case STATE_CONJURE_MODE:
    handle_state_conjure_mode();
    break;
#ifdef HELIOS_CLI
  case STATE_SLEEP:
    // simulate sleep in helios CLI
    if (Button::onPress()) {
      // wakeup
      printf("Wakeup\n");
      sleeping = false;
    }
    break;
#endif
  }
}

void Helios::next_mode()
{
  // increment current mode and wrap around
  cur_mode = (uint8_t)(cur_mode + 1) % 6;
  // read pattern from storage at cur mode index
  if (!Storage::read_pattern(cur_mode, pat)) {
    // and just initialize default if it cannot be read
    Patterns::make_default(cur_mode, pat);
  }
  // then re-initialize the pattern
  pat.init();
}

void Helios::handle_state_modes()
{
  if (Button::onShortClick()) {
    if (has_flag(FLAG_CONJURE)) {
      enter_sleep(false);
    } else {
      next_mode();
    }
    return;
  }
  // just play the current mode
  pat.play();
  
  // check how long the button is held
  uint32_t holdDur = Button::holdDuration();
  // show a color based on the hold duration past 200
  // the magnitude will be some value from 0-3 corresponding
  // to the holdDurations of 200 to 500
  uint16_t magnitude = (holdDur / MENU_HOLD_TIME);
  if (magnitude >= 4) {
    magnitude = 0;
  }
  bool heldPast = (holdDur > SHORT_CLICK_THRESHOLD);
  // if the button is held for at least 1 second
  if (Button::isPressed() && heldPast) {
    const RGBColor menu_cols[4] = { RGB_OFF, RGB_CYAN, RGB_MAGENTA, RGB_YELLOW };
    Led::set(menu_cols[magnitude]);
  }
  //State next_state = STATE_MODES;
  // when released, switch to different state based on hold duration
  if (Button::onRelease()) {
    switch (magnitude) {
    case 0:  // off
      // but only if we held for more than a short click
      if (heldPast) {
        enter_sleep(false);
      }
      break;
    case 1:  // color select
      //cur_state = STATE_COLOR_SELECT_SLOT;
      // use the nice hue to rgb rainbow
      //g_hsv_rgb_alg = HSV_TO_RGB_RAINBOW;
      // reset the menu selection
      //menu_selection = 0;
      break;
    case 2:  // pat select
      //cur_state = STATE_PATTERN_SELECT;
      break;
    case 3:  // conjure mode
      cur_state = STATE_CONJURE_MODE;
      break;
    default: // hold past
      // do nothing
      break;
    }
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
      if (num_cols < MAX_COLOR_SLOTS) {
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

bool Helios::handle_state_col_select_slot()
{
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
    Led::strobe(100, 100, RGB_WHITE4, RGB_OFF);
  } else if (menu_selection == num_cols + 1) {
    // exit
    Led::strobe(60, 40, RGB_RED3, RGB_OFF);
  } else {
    // render current selection
    Led::set(set.get(menu_selection));
  }
  return true;
}

bool Helios::handle_state_col_select_quadrant()
{
  uint8_t hue_quad = (menu_selection - 2);
  uint8_t hue = hue_quad * (255 / 4);
  HSVColor hcol(hue, 255, 255);
  if (Button::onLongClick()) {
    // select hue/sat/val
    switch (menu_selection) {
    case 0: // exit
      cur_state = STATE_MODES;
      return false;
    case 1: // selected blank
      // add blank to set
      pat.colorset().set(selected_slot, RGB_OFF);
      // go to slot selection - 1 because we will increment outside here
      cur_state = STATE_COLOR_SELECT_SLOT;
      return false;
    case 2: // selected white
      // adds white, skip hue/sat to brightness
      selected_hue = 0;
      selected_sat = 0;
      cur_state = STATE_COLOR_SELECT_VAL;
      return false;
    default: // 3-6
      selected_base_hue = hue;
      break;
    }
  }
  // default col1/col2 to off and white for the first two options
  RGBColor col1 = RGB_OFF;
  RGBColor col2;
  switch (menu_selection) {
  case 0: // exit
    col2 = RGB_RED8;
    break;
  case 1: // blank
    col2 = RGB_WHITE1;
    break;
  case 2: // white
    col2 = RGB_WHITE;
    break;
  default: // colors
    col1 = hcol;
    hcol.hue += 32;
    col2 = hcol;
    break;
  }
  // render current selection
  Led::strobe(6, 10, col1, col2);
  return true;
}

bool Helios::handle_state_col_select_hue()
{
  uint8_t hue = selected_base_hue + (menu_selection * 16);
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

bool Helios::handle_state_col_select_val()
{
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

void Helios::handle_state_pat_select()
{
  if (Button::onLongClick()) {
    cur_state = STATE_MODES;
    // save
    return;
  }
  if (Button::onShortClick()) {
    menu_selection = (menu_selection + 1) % PATTERN_COUNT;
    Patterns::make_pattern((PatternID)menu_selection, pat);
    if (!Storage::write_pattern(cur_mode, pat)) {
      // failed to save?
    }
    pat.init();
  }
  pat.play();
}

void Helios::handle_state_conjure_mode()
{
  // toggle the conjure flag
  toggle_flag(FLAG_CONJURE);
  // write out the new global flags and the current mode
  Storage::write_config(0, global_flags);
  Storage::write_config(1, (uint8_t)cur_mode - 1);
  // switch back to modes
  cur_state = STATE_MODES;
}
