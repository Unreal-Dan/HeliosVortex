#include <stdint.h>

#include "ColorConstants.h"
#include "BasicPattern.h"
#include "TimeControl.h"
#include "Button.h"
#include "Led.h"

#ifdef HELIOS_EMBEDDED
#include <avr/sleep.h>
#include <avr/interrupt.h>
#endif

void enterSleep(bool save);
void handle_state();

bool sleeping = false;
Pattern *pat;

#ifndef HELIOS_CLI
void setup();
void loop();

// continue running
bool keepgoing = true;

int main(int argc, char *argv[])
{
  setup();
  while (keepgoing) {
    loop();
  }
  return 0;
}
#endif

void setup()
{
  if (!Time::init()) {
    return;
  }
  if (!Led::init()) {
    return;
  }

  // init the button
  button.init();

  PatternArgs tempargs(6, 10);
  pat = new BasicPattern(tempargs);
  Colorset set = Colorset(RGB_RED, RGB_GREEN, RGB_BLUE);
  pat->setColorset(set);
  pat->init();

  // if loading the storage fails (corrupted, empty, etc) then load
  // the default values and let them save back to storage naturally later
  // you don't need to wipe the storage at init for any reason really
  //if (!load_modes()) {
  //  load_defaults();
  //}

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
}

#ifdef HELIOS_EMBEDDED
ISR(TIM1_COMPA_vect) {
  // Your ISR code for the 1000 Hz interrupt goes here
  loop();
}
#endif

void loop()
{
  // sample the button and re-calculate all button globals
  // the button globals should not change anywhere else
  button.update();

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

void enterSleep(bool save)
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

enum helios_state {
  STATE_MODES,
  STATE_COLOR_SELECT_HUE,
  STATE_COLOR_SELECT_SAT,
  STATE_COLOR_SELECT_VAL,
  STATE_PATTERN_SELECT,
  STATE_FACTORY_RESET
};

helios_state cur_state = STATE_MODES;
void handle_state_modes();
void handle_state_col_select();
void handle_state_pat_select();
void handle_state_fac_reset();

void handle_state()
{
  switch (cur_state) {
    case STATE_MODES:
      handle_state_modes();
      break;
    case STATE_COLOR_SELECT_HUE:
    case STATE_COLOR_SELECT_SAT:
    case STATE_COLOR_SELECT_VAL:
      handle_state_col_select();
      break;
    case STATE_PATTERN_SELECT:
      handle_state_pat_select();
      break;
    case STATE_FACTORY_RESET:
      handle_state_fac_reset();
      break;
  }
}

void next_mode()
{
  if (pat) {
    // destruct global pattern instance
    delete pat;
    pat = nullptr;
  }
  // read mode from storage at cur mode index
  // instantiate new pattern
  //pat = Storage::load_pattern(cur_mode++);
}

void handle_state_modes()
{
  if (button.onShortClick()) {
    next_mode();
    return;
  }
  // just play the current mode
  if (pat) {
    pat->play();
  } else {
    Led::clear();
  }
  uint32_t holdDur = button.holdDuration();
  // show a color based on the hold duration past 200
  // the magnitude will be some value from 0-3 corresponding
  // to the holdDurations of 200 to 500
  uint16_t magnitude = (holdDur / 100) - 2;
  // if the button is held for at least 1 second
  if (button.isPressed()) {
    RGBColor cols[3] = { RGB_CYAN, RGB_YELLOW, RGB_MAGENTA };
    Led::set(cols[magnitude]);
  }
  // when released, switch to different state based on hold duration
  if (button.onRelease()) {
    switch (magnitude) {
    case 0:  // color select
      cur_state = STATE_COLOR_SELECT_HUE;
      break;
    case 1:  // pat select
      cur_state = STATE_PATTERN_SELECT;
      break;
    case 2:  // fac reset
      cur_state = STATE_FACTORY_RESET;
      break;
    }
  }
}

RGBColor sat_options[6] = {
  RGB_WHITE,
  RGB_WHITE6,
  RGB_WHITE3,
  RGB_WHITE1,
};

uint8_t menu_selection = 0;

void handle_state_col_select()
{
  switch (cur_state) {
  default:
  case STATE_COLOR_SELECT_HUE:
    // target hue for changes
    break;
  case STATE_COLOR_SELECT_SAT:
    // target sat for changes
    break;
  case STATE_COLOR_SELECT_VAL:
    // target val for changes
    break;
  }
  if (button.onShortClick()) {
    // next hue/sat/val selection
    menu_selection++;
  }
  if (button.onLongClick()) {
    // select hue/sat/val
  }
  // render current selection
}

void handle_state_pat_select()
{
}

void handle_state_fac_reset()
{
}
