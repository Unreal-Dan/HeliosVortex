#include <stdint.h>

#include "ColorConstants.h"
#include "BasicPattern.h"
#include "TimeControl.h"
#include "Button.h"
#include "Led.h"

void handle_button();
void handle_state();
void tick_clock();

Pattern *pat;

#ifndef CLI_MODE
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
#ifndef CLI_MODE
  //// init all the hardware
  //pinMode(PIN_R, OUTPUT);
  //pinMode(PIN_G, OUTPUT);
  //pinMode(PIN_B, OUTPUT);
  //pinMode(PIN_BUTTON, INPUT);

  //// Enable Pin Change Interrupt on the BUTTON pin
  //GIMSK |= _BV(PCIE);
  //PCMSK |= _BV(PIN_BUTTON);
  //sei();  // Enable interrupts

  //// Set CPU Frequency to 8 MHz
  //CLKPR = 0x80;  // Enable change
  //CLKPR = 0x00;  // Change clock division factor to 1 (8 MHz / 1 = 8 MHz)
#endif

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
}

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

enum helios_state {
  STATE_MODES


};

helios_state cur_state = STATE_MODES;
void handle_state_modes();

void handle_state()
{
  switch (cur_state) {
    case STATE_MODES:
      handle_state_modes();
      break;

  }
}

void next_mode()
{
  if (pat) {
    // destruct global pattern instance
    delete pat;
  }
  // read mode from storage at cur mode index
  // instantiate new pattern
  //pat = Storage::load_pattern(cur_mode++);
}

void handle_hold(uint32_t magnitude)
{
  RGBColor cols[3] = { RGB_CYAN, RGB_YELLOW, RGB_MAGENTA };
  Led::set(cols[magnitude]);
}

void handle_release(uint32_t magnitude)
{
  switch (magnitude) {
    case 0:  // 0
    case 1:  // 100
             // cyan
             // switch to this menu state
    case 2:  // 200
             // magenta
             // switch to this menu state
    case 3:  // 300
             // yellow
             // switch to this menu state
      break;
  }
}

void handle_state_modes()
{
  if (button.onShortClick()) {
    next_mode();
    return;
  }
  uint32_t holdDur = button.holdDuration();
  // show a color based on the hold duration past 200
  // the magnitude will be some value from 0-3 corresponding
  // to the holdDurations of 200 to 500
  uint16_t magnitude = (holdDur / 100) - 2;
  // if the button is held for at least 1 second
  if (button.isPressed()) {
    handle_hold(magnitude);
  }
  if (button.onRelease()) {
    handle_release(magnitude);
  }
  // just play the current mode
  if (pat) {
    pat->play();
  }
}
