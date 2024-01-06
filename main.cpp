#include <avr/sleep.h>
#include <avr/power.h>

int main()
{
  // Disable ADC
  ADCSRA &= ~(1 << ADEN);

  // Configure all I/O pins as input with pull-up enabled
  DDRB = 0;  // Set all pins on Port B as input
  PORTB = 0; // Disable pull-up resistors on all Port B pins
  // Disable digital input buffers
  DIDR0 = 0xFF;

  // Set the sleep mode to Power-down
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  // Disable unnecessary features
  power_adc_disable();
  power_usi_disable();
  power_timer0_disable();
  power_timer1_disable();
  while (1)
  {
    // Enable sleep mode
    sleep_enable();

    // Put the device to sleep
    sleep_mode();

    // The program will continue from here after wakeup

    // Disable sleep mode
    sleep_disable();
  }
}
