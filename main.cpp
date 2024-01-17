#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>  // Include this for sei()


void setupClockPrescaler() {
  // Set the clock prescaler to divide by 8
  CLKPR = (1 << CLKPCE);  // Enable change to CLKPR
  CLKPR = (1 << CLKPS1);  // CLKPS1 set, others unset (division factor = 8)
}

int main()
{
  // Disable ADC
  ADCSRA &= ~(1 << ADEN);

  // Activate PRR (Power Reduction Register)
  PRR |= (1 << PRTIM1) | (1 << PRTIM0) | (1 << PRUSI) | (1 << PRADC);

  // Configure all I/O pins as input with pull-up enabled
  DDRB = 0x00;  // Set all pins on Port B as input
  PORTB = 0xFF; // Enable pull-up resistors on all Port B pins

  // Disable digital input buffers
  DIDR0 = 0xFF;

  // Set the sleep mode to Power-down
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  // Disable unnecessary features
  power_adc_disable();
  power_usi_disable();
  power_timer0_disable();
  power_timer1_disable();

  // Disable the Watchdog Timer
  MCUSR &= ~(1 << WDRF);
  wdt_disable();

  // Set up the clock prescaler
  setupClockPrescaler();

  while (1)
  {
    // Disable the BOD
    sleep_bod_disable();

    // Enable global interrupts
    sei();

    // Put the device to sleep
    sleep_mode();
    sleep_cpu();

    // The program will continue from here after wakeup

    // Disable sleep mode
    sleep_disable();
  }
  return 0;
}


// #include "Helios.h"
// #include "Led.h"

// #if !defined(HELIOS_CLI) && !defined(HELIOS_ARDUINO)
// // this is the main thread for non-arduino embedded builds
// int main(int argc, char *argv[])
// {
//   Helios::init();
//   while (Helios::keep_going()) {
//     // continuously update the led at 8mhz cpu speed while the
//     // engine ticks 1000khz in the interrupt handler
//     Led::update();
//   }
//   return 0;
// }
// #endif
