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
  pinMode(PWM_PIN_R, OUTPUT);
  pinMode(PWM_PIN_G, OUTPUT);
  pinMode(PWM_PIN_B, OUTPUT);
#else
  // Set pins as outputs
  DDRB |= (1 << PWM_PIN_R) | (1 << PWM_PIN_G) | (1 << PWM_PIN_B);

   // Set to Fast PWM mode, non-inverting
  // WGM01:0 = 11 for Fast PWM; COM0x1:0 = 10 for non-inverting
  TCCR0A |= (1 << WGM01) | (1 << WGM00) | (1 << COM0A1) | (1 << COM0B1);
   // Clock select, no prescaling
  TCCR0B |= (1 << CS00);
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
  // write out the rgb values to analog pins
#ifdef HELIOS_ARDUINO
  analogWrite(PWM_PIN_R, m_realColor.red);
  analogWrite(PWM_PIN_G, m_realColor.green);
  analogWrite(PWM_PIN_G, m_realColor.blue);
#else
  // Set hardware PWM duty cycle for red and green
  OCR0A = m_realColor.red;
  OCR0B = m_realColor.green;

  // Software PWM for blue
  static uint8_t counter = 0;
  counter++;
  if (m_realColor.blue == 0) {
    PORTB &= ~(1 << PWM_PIN_B);
  } else if (counter < m_realColor.blue) {
    PORTB |= (1 << PWM_PIN_B);
  } else {
    PORTB &= ~(1 << PWM_PIN_B);
  }
#endif
#endif
}

void Led::updateSoftwarePWM()
{
#ifdef HELIOS_EMBEDDED
  // write out the rgb values to analog pins
#ifdef HELIOS_ARDUINO
  // software PWM for blue
  analogWrite(PWM_PIN_G, m_realColor.blue);
#else
  // Software PWM for blue
  static uint8_t counter = 0;
  counter++;
  if (counter < m_realColor.blue) {
  PORTB |= (1 << PWM_PIN_B);
  } else {
    PORTB &= ~(1 << PWM_PIN_B);
  }
  // if (m_realColor.blue == 0) {
  //   PORTB &= ~(1 << PWM_PIN_B);
  // } else if (counter < m_realColor.blue) {
  //   PORTB |= (1 << PWM_PIN_B);
  // } else {
  //   PORTB &= ~(1 << PWM_PIN_B);
  // }
#endif
#endif
}

void Led::updateHardwarePWM()
{
#ifdef HELIOS_EMBEDDED
  // write out the rgb values to analog pins
#ifdef HELIOS_ARDUINO
  analogWrite(PWM_PIN_R, m_realColor.red);
  analogWrite(PWM_PIN_G, m_realColor.green);
#else
  // Set hardware PWM duty cycle for red and green
  OCR0A = m_realColor.red;
  OCR0B = m_realColor.green;
#endif
#endif
}