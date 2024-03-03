#include <math.h>

#include "Led.h"

#include "TimeControl.h"

#include "HeliosConfig.h"
#include "Helios.h"

#ifdef HELIOS_EMBEDDED
#ifdef HELIOS_ARDUINO
#include <arduino.h>
#else
#include <avr/sleep.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#endif
#endif

#define PWM_PIN_R PB0 // Red channel (pin 5)
#define PWM_PIN_G PB1 // Green channel (pin 6)
#define PWM_PIN_B PB4 // Blue channel (pin 3)

#define SCALE8(i, scale)  (((uint16_t)i * (uint16_t)(scale)) >> 8)

// array of led color values
RGBColor Led::m_ledColor = RGB_OFF;
RGBColor Led::m_realColor = RGB_OFF;
// global brightness
uint8_t Led::m_brightness = DEFAULT_BRIGHTNESS;

bool Led::init()
{
  // clear the led colors
  m_ledColor = RGB_OFF;
  m_realColor = RGB_OFF;
#ifdef HELIOS_EMBEDDED
#ifdef HELIOS_ARDUINO
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(4, OUTPUT);
#else
   // Configure Timer0 for Fast PWM on OC0A and OC0B (PB0 and PB1)
    TCCR0A = (1 << WGM01) | (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);
    TCCR0B = (1 << CS00); // No prescaling

    // Configure Timer1 for PWM on OC1A (PB4) and CTC mode for tick clock
    GTCCR = (1 << PWM1B) | (1 << COM1B1);
    OCR1C = 0xFF; // Max PWM value
    TCCR1 = (1 << CTC1) | (1 << CS12) | (1 << CS10); // CTC mode with prescaler

    // Set OCR1A for tick clock interrupt (1 kHz)
    OCR1A = 124; // Adjust based on prescaler and desired frequency

    // Enable Timer1 Compare Match Interrupt
    TIMSK |= (1 << OCIE1A);

    // Set PWM pins as output
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB4);
  // // Set pins as outputs
  // DDRB |= (1 << 0) | (1 << 1) | (1 << 4);
  // // Setup Timer0 (OC0A and OC0B) for PWM
  // TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
  // TCCR0B = (1 << CS00); // No prescaling
  // // Setup Timer1 (OC1A) for PWM
  // GTCCR = (1 << PWM1B) | (1 << COM1B1);
  // OCR1C = 0xFF; // Set top value for PWM
  // TCCR1 = (1 << CS10); // No prescaling, start Timer1
#endif
#endif
  return true;
}

void Led::cleanup()
{
}

void Led::set(RGBColor col)
{
  m_ledColor = col;
  m_realColor.red = SCALE8(m_ledColor.red, m_brightness);
  m_realColor.green = SCALE8(m_ledColor.green, m_brightness);
  m_realColor.blue = SCALE8(m_ledColor.blue, m_brightness);
}

void Led::set(uint8_t r, uint8_t g, uint8_t b)
{
  set(RGBColor(r, g, b));
}

void Led::adjustBrightness(uint8_t fadeBy)
{
  m_ledColor.adjustBrightness(fadeBy);
}

void Led::strobe(uint16_t on_time, uint16_t off_time, RGBColor off_col, RGBColor on_col)
{
  set(((Time::getCurtime() % (on_time + off_time)) > on_time) ? off_col : on_col);
}

void Led::breath(uint8_t hue, uint32_t duration, uint8_t magnitude, uint8_t sat, uint8_t val)
{
  if (!duration) {
    // don't divide by 0
    return;
  }
  // Determine the phase in the cycle
  uint32_t phase = Time::getCurtime() % (2 * duration);
  // Calculate hue shift
  int32_t hueShift;
  if (phase < duration) {
    // Ascending phase - from hue to hue + magnitude
    hueShift = (phase * magnitude) / duration;
  } else {
    // Descending phase - from hue + magnitude to hue
    hueShift = ((2 * duration - phase) * magnitude) / duration;
  }
  // Apply hue shift - ensure hue stays within valid range
  uint8_t shiftedHue = hue + hueShift;
  // Apply the hsv color as a strobing hue shift
  strobe(2, 13, RGB_OFF, HSVColor(shiftedHue, sat, val));
}

void Led::hold(RGBColor col)
{
  set(col);
  update();
  Time::delayMilliseconds(250);
}

void Led::update()
{
#ifdef HELIOS_EMBEDDED
  // write out the rgb values to PWM registers
  OCR0A = m_realColor.red;   // Set PWM duty cycle for red (OC0A)
  OCR0B = m_realColor.green; // Set PWM duty cycle for green (OC0B)
  OCR1B = m_realColor.blue;  // Set PWM duty cycle for blue (OC1A)
#endif
}