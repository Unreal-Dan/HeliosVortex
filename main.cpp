#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>
#include <avr/io.h>
#include <avr/interrupt.h>  // Include this for sei()

volatile bool sleepEnabled = false; // Flag to control sleep mode

// Interrupt Service Routine for Pin Change Interrupt
ISR(PCINT0_vect) {

}

int main() {
  // Button setup
  DDRB &= ~(1 << PB3); // Set PB3 as input
  PORTB |= (1 << PB3); // Enable pull-up resistor on PB3

  // Configure PB0 as output for the LED
  DDRB |= (1 << PB0);  // Set PB0 as output

  // Enable Pin Change Interrupt for PB3
  GIMSK |= (1 << PCIE);  // Enable Pin Change Interrupts
  PCMSK |= (1 << PCINT3); // Enable interrupt for PB3

  sei(); // Enable global interrupts

   // Turn off LED
    PORTB &= ~(1 << PB0);
    // Disable the Watchdog Timer
    MCUSR &= ~(1 << WDRF);
    wdt_disable();
    // Configure all I/O pins as input with pull-up enabled
    DDRB = 0x00;  // Set all pins on Port B as input
    // PORTB = 0xFF; // Enable pull-up resistors on all Port B pins

    // Disable digital input buffers
    DIDR0 = 0xFF;

    // Set the sleep mode to Power-down
    set_sleep_mode(SLEEP_MODE_PWR_DOWN);

    // // Disable ADC
    // ADCSRA &= ~(1 << ADEN);

    // Activate PRR (Power Reduction Register)
    PRR |= (1 << PRTIM1) | (1 << PRTIM0) | (1 << PRUSI) | (1 << PRADC);

    // Disable the BOD
    sleep_bod_disable();

    // Enable global interrupts
    sei();

    // Put the device to sleep
    sleep_mode();

    // The program will continue from here after wakeup

    // Disable sleep mode
    sleep_disable();

  while (1) {
   
    // Re-enable Pin Change Interrupt for PB3
    GIMSK |= (1 << PCIE);  // Enable Pin Change Interrupts
    PCMSK |= (1 << PCINT3); // Enable interrupt for PB3

    // Reconfigure PB0 as output for the LED
    DDRB |= (1 << PB0);  // Set PB0 as output
    // Set PB0 (LED) to high when not sleeping
    PORTB |= (1 << PB0);
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
