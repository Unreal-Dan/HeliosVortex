#include <stdint.h>

#include "BasicPattern.h"
#include "TimeControl.h"
#include "Button.h"
#include "Led.h"

void handle_button();
void handle_state();
void tick_clock();

Pattern *pat;

void setup()
{
#ifndef CLI_MODE
  // init all the hardware
  pinMode(PIN_R, OUTPUT);
  pinMode(PIN_G, OUTPUT);
  pinMode(PIN_B, OUTPUT);
  pinMode(PIN_BUTTON, INPUT);

  // Enable Pin Change Interrupt on the BUTTON pin
  GIMSK |= _BV(PCIE);
  PCMSK |= _BV(PIN_BUTTON);
  sei();  // Enable interrupts

  // Set CPU Frequency to 8 MHz
  CLKPR = 0x80;  // Enable change
  CLKPR = 0x00;  // Change clock division factor to 1 (8 MHz / 1 = 8 MHz)
#endif

  // init the button
  button.init();

	PatternArgs args(6, 10);
	pat = new BasicPattern(args);
	Colorset set(RGB_RED, RGB_GREEN, RGB_BLUE);
	pat->setColorset(set);

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

void handle_state()
{
	if (pat) {
		pat->play();
	}
}

//void next_mode()
//{
//  // destruct global mode instant
//  // iterate cur mode index
//  // read mode from storage at cur mode index
//}

//void handle_state_modes()
//{
//  if (button.onShortClick()) {
//    next_mode();
//    return;
//  }
//  // if the button is held for at least 1 second
//  if (button.isPressed()) {
//    uint32_t holdDur = button.holdDuration();
//    switch (holdDur / 100) {
//      case 0: // 0
//      case 1: // 100
//        // cyan
//      case 2: // 200
//        // magenta
//      case 3: // 300
//        // yellow
//        break;
//    }
//    if (button.onRelease()) {
//      switch (holdDur / 100) {
//        case 0:  // 0
//        case 1:  // 100
//                 // cyan
//                 // switch to this menu state
//        case 2:  // 200
//                 // magenta
//                 // switch to this menu state
//        case 3:  // 300
//                 // yellow
//                 // switch to this menu state
//          break;
//      }
//    }
//    return;
//  }
//  // just play the current mode
//  //mode.play();
//}
