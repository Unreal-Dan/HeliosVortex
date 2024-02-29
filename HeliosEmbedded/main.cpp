#include "Helios.h"
#include "Led.h"

#if !defined(HELIOS_CLI) && !defined(HELIOS_ARDUINO)
// this is the main thread for non-arduino embedded builds
int main(int argc, char *argv[])
{
  Helios::init();
  while (Helios::keep_going()) {
    // continuously update the led at 8mhz cpu speed while the
    // engine ticks 1000khz in the interrupt handler
    Led::updateSoftwarePWM();
  }
  return 0;
}
#endif

// #include <avr/io.h>
// #include <avr/interrupt.h>
// #include <util/delay.h>

// volatile uint8_t softwarePWM = 0;
// // Setup Timer0 for hardware PWM
// void setupTimer0PWM() {
//     // Set PB0 (OC0A) and PB1 (OC0B) as output
//     DDRB |= (1 << PB0) | (1 << PB1);

//     // Set to Fast PWM mode, non-inverting
//     // WGM01:0 = 11 for Fast PWM; COM0x1:0 = 10 for non-inverting
//     TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);

//     // Clock select, no prescaling
//     TCCR0B |= (1 << CS00);
// }

// // Function to change PWM duty cycle
// void setPWM(uint8_t red, uint8_t green) {
//     OCR0A = red; // Set duty cycle for PB0 (Red)
//     OCR0B = green; // Set duty cycle for PB1 (Green)
// }


// // Initialize Timer1 for 1 kHz interrupts
// void setupTimer1Interrupt() {
//     // Set CTC mode (Clear Timer on Compare Match) (WGM12:0 = 010)
//     TCCR1 |= (1 << CTC1);

//     // Set OCR1C for a 1 kHz rate assuming 8 MHz internal clock with prescaler 8
//     OCR1C = 99; // OCR1C = (F_CPU / Prescaler / DesiredFrequency) - 1

//     // Enable Timer1 compare match interrupt
//     TIMSK |= (1 << OCIE1A);

//     // Start Timer1 with prescaler 8
//     TCCR1 |= (1 << CS12) | (1 << CS10);
// }

// // Timer1 Compare Match A Interrupt Service Routine
// ISR(TIM1_COMPA_vect) {
//     // Set hardware PWM duty cycle for red and green
//     OCR0A = m_realColor.red;
//     OCR0B = m_realColor.green;
//     // Toggle PB4 (assuming PB4 is used for software PWM)
//     if (softwarePWM) {
//         PORTB &= ~(1 << PB4); // Turn off
//         softwarePWM = 0;
//     } else {
//         PORTB |= (1 << PB4); // Turn on
//         softwarePWM = 1;
//     }
// }



// int main(void) {
//     // Initialize PWM and interrupt
//     setupTimer0PWM();
//     setupTimer1Interrupt();
//     DDRB |= (1 << PB4); // Set PB4 as output for software PWM

//     sei(); // Enable global interrupts

//     uint8_t red = 0, green = 0, blue = 0;
//     while (1) {
//         // Cycle through the full spectrum of colors
//         for (int i = 0; i < 765; i++) {
//             if (i < 255) {
//                 // From black to blue to cyan
//                 blue = i;
//             } else if (i < 510) {
//                 // From cyan to green
//                 blue = 255 - (i - 255);
//                 green = i - 255;
//             } else {
//                 // From green to red
//                 red = i - 510;
//                 green = 255 - (i - 510);
//             }

//             // Set hardware PWM for red and green
//             setPWM(red, green);

//             // Adjust software PWM value for blue
//             // This is a simplistic approach; you might need to implement
//             // a more sophisticated software PWM handler for smoother transitions.
//             softwarePWM = blue / 255.0 * 10; // Scale and convert to a count for toggling

//             _delay_ms(1);
//         }
//     }
// }
