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
#endif

#ifdef HELIOS_CLI
#include <stdio.h>
#endif

Helios::State Helios::cur_state = STATE_MODES;
uint8_t Helios::menu_selection = 0;
uint8_t Helios::cur_mode = 0xFF;
uint8_t Helios::selected_base_hue = 0;
uint8_t Helios::selected_hue = 0;
uint8_t Helios::selected_sat = 0;
PatternArgs Helios::default_args[6] = {
  {1, 0, 0, 0, 0, 0, 0},
  {3, 4, 0, 0, 0, 0, 0},
  {15, 5, 4, 0, 0, 0, 0},
  {15, 5, 4, 3, 0, 0, 0},
  {15, 5, 4, 3, 2, 0, 0},
  {15, 5, 4, 3, 2, 1, 0},
};
Colorset Helios::default_colorsets[6] = {
  { RGB_RED, RGB_GREEN, RGB_BLUE },
  { RGB_RED, RGB_BLUE },
  { RGB_RED, RGB_BLUE },
  { RGB_RED, RGB_BLUE },
  { RGB_RED, RGB_BLUE },
  { RGB_RED, RGB_BLUE },
};
Pattern Helios::pat;
bool Helios::keepgoing = true;
bool Helios::sleeping = false;

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

  // init the button
  if (!Button::init()) {
    return false;
  }

  // iterate to the next mode (or first mode in this case)
  next_mode();

#ifdef HELIOS_EMBEDDED
  // Set CTC (Clear Timer on Compare Match) mode
  TCCR1 = (1 << CTC1);
  // Set prescaler to 8 (CS12 = 1, CS11 = 0, CS10 = 0)
  TCCR1 |= (1 << CS11);
  // Set compare match value for 1000 Hz
  OCR1C = 124;
  // Enable Timer/Counter 1 Output Compare A Match interrupt
  TIMSK |= (1 << OCIE1A);
  // Setup sleep mode for standby
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  // Enable interrupts
  sei();
  // Standby indefinitely while the ISR runs ticks
  while (!sleeping) {
    sleep_mode();
  }
#endif
  return true;
}

#ifdef HELIOS_EMBEDDED
// ISR to call tick 1000 times a second
ISR(TIM1_COMPA_vect) {
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

  // render the current led color by sending the data to the leds, this
  // function is basically just set_color()
  Led::update();

  // finally tick the clock forward and then sleep till the entire
  // tick duration has been consumed
  Time::tickClock();
}

void Helios::enterSleep(bool save)
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
  // delay for a bit to let the mosfet close and leds turn off
  Time::delayMicroseconds(250);
  // this is an ISR that runs in the timecontrol system to handle
  // micros, it will wake the device up periodically
  TIMSK &= ~(1 << OCIE1A);
  // Enable wake on interrupt for the button
  //g_pButton->enableWake();
  // Set sleep mode to POWER DOWN mode
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  // enable the sleep boo lright before we enter sleep, this will allow
  // the main loop to break and return
  sleeping = true;
  // enter sleep
  sleep_mode();
#else
  // enable the sleep bool
  sleeping = true;
#endif
}

void Helios::handle_state()
{
  switch (cur_state) {
  case STATE_MODES:
    handle_state_modes();
    break;
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
  case STATE_SLEEP:
    break;
  }
}

void Helios::next_mode()
{
  // increment current mode and wrap around
  cur_mode = (uint8_t)(cur_mode + 1) % 6;
  // read pattern from storage at cur mode index
  if (!Storage::read(cur_mode, pat)) {
    // and just initialize default if it cannot be read
    set_default(cur_mode);
  }
  // then re-initialize the pattern
  pat.init();
}

void Helios::set_default(uint8_t default_num)
{
  pat = Pattern(default_args[default_num]);
  pat.setColorset(default_colorsets[default_num]);
  // don't forget to init the pattern!
}

void Helios::handle_state_modes()
{
  if (Button::onShortClick()) {
    next_mode();
    return;
  }
  // just play the current mode
  pat.play();
  // check how long the button is held
  uint32_t holdDur = Button::holdDuration();
  // show a color based on the hold duration past 200
  // the magnitude will be some value from 0-3 corresponding
  // to the holdDurations of 200 to 500
  uint16_t magnitude = (holdDur / 1000);
  if (magnitude >= 4) {
    magnitude = 0;
  }
  // if the button is held for at least 1 second
  if (Button::isPressed()) {
    const RGBColor menu_cols[4] = { RGB_OFF, RGB_CYAN, RGB_YELLOW, RGB_MAGENTA };
    Led::set(menu_cols[magnitude]);
  }
  //State next_state = STATE_MODES;
  // when released, switch to different state based on hold duration
  if (Button::onRelease()) {
    switch (magnitude) {
    case 0:  // off 0-100 ms
      cur_state = STATE_SLEEP;
      break;
    case 1:  // color select 100-200 ms
      cur_state = STATE_COLOR_SELECT_QUADRANT;
      // use the nice hue to rgb rainbow
      g_hsv_rgb_alg = HSV_TO_RGB_RAINBOW;
      break;
    case 2:  // pat select 200-300 ms
      cur_state = STATE_PATTERN_SELECT;
      break;
    case 3:  // conjure mode 300-400 ms
      cur_state = STATE_CONJURE_MODE;
      break;
    default: // hold past 400+ ms
      // do nothing
      break;
    }
  }
}

void Helios::handle_state_col_select_quadrant()
{
  uint8_t hue_quad = (menu_selection - 2);
  uint8_t hue = hue_quad * (255 / 4);
  HSVColor hcol(hue, 255, 255);
  if (Button::onLongClick()) {
    // select hue/sat/val
    if (menu_selection == 0 || menu_selection == 1) {
      // selected 0 or 1
      // return
    } else {
      selected_base_hue = hue;
      menu_selection = 0;
    }
  }
  // default col1/col2 to off and white for the first two options
  RGBColor col1 = RGB_OFF;
  RGBColor col2 = (menu_selection == 0) ? RGB_WHITE1 : RGB_WHITE;
  // if it's more than the first or second option then generate hues
  if (menu_selection > 1) {
    col1 = hcol;
    hcol.hue += 32;
    col2 = hcol;
  }
  // render current selection
  Led::strobe(6, 10, col1, col2);
}

void Helios::handle_state_col_select_hue()
{
  uint8_t hue = selected_base_hue + (menu_selection * 16);
  if (Button::onLongClick()) {
    // select hue/sat/val
    selected_hue = hue;
  }
  // render current selection
  Led::set(HSVColor(hue, 255, 255));
}

void Helios::handle_state_col_select_sat()
{
  uint8_t sat = 255 - (menu_selection * 60);
  // use the nice hue to rgb rainbow
  if (Button::onLongClick()) {
    // select hue/sat/val
    selected_sat = sat;
  }
  // render current selection
  Led::set(HSVColor(selected_hue, sat, 255));
}

void Helios::handle_state_col_select_val()
{
  uint8_t val = 255 - (menu_selection * 50);
  RGBColor targetCol = HSVColor(selected_hue, selected_sat, val);
  // use the nice hue to rgb rainbow
  if (Button::onLongClick()) {
    // change the current patterns color
    pat.colorset().set(0, targetCol);
    // restore hsv to rgb algorithm type, done color selection
    g_hsv_rgb_alg = HSV_TO_RGB_GENERIC;
  }
  // render current selection
  Led::set(targetCol);
}

void Helios::handle_state_col_select()
{
  if (Button::onShortClick()) {
    // next hue/sat/val selection
    uint8_t num_menus = 4;
    if (cur_state == STATE_COLOR_SELECT_QUADRANT) {
      num_menus = 6;
    }
    menu_selection = (menu_selection + 1) % num_menus;
  }
  switch (cur_state) {
  default:
  case STATE_COLOR_SELECT_QUADRANT:
    // pick the hue quadrant
    handle_state_col_select_quadrant();
    break;
  case STATE_COLOR_SELECT_HUE:
    // target hue for changes
    handle_state_col_select_hue();
    break;
  case STATE_COLOR_SELECT_SAT:
    // target sat for changes
    handle_state_col_select_sat();
    break;
  case STATE_COLOR_SELECT_VAL:
    // target val for changes
    handle_state_col_select_val();
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
    if (!Storage::write(cur_mode, pat)) {
      // failed to save?
    }
    pat.init();
  }
  pat.play();
}

void Helios::handle_state_conjure_mode()
{
}
