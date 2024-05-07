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
  // Set pins as outputs
  //DDRB |= (1 << 0) | (1 << 1) | (1 << 4);
  // Timer/Counter0 in Fast PWM mode
  //TCCR0A |= (1 << WGM01) | (1 << WGM00);
  // Clear OC0A and OC0B on compare match, set at BOTTOM (non-inverting mode)
  //TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
  // Use clk/8 prescaler (adjust as needed)
  //TCCR0B |= (1 << CS01);

  //TCCR1 |= (1 << PWM1A) | (1 << COM1A1) | (1 << PWM1B) | (1 << COM1B1);
  //GTCCR |= (1 << PWM1B);


  //// Set the DDR to enable output on the corresponding pins
  //DDRB |= (1 << DDB0);  // PB0 as output (OC0A)
  //DDRB |= (1 << DDB1);  // PB1 as output (OC0B)
  //DDRB |= (1 << DDB4);  // PB4 as output (OC1B)

  //// Timer0 Configuration for Fast PWM
  //TCCR0A = (1 << WGM01) | (1 << WGM00);  // Fast PWM
  //TCCR0A |= (1 << COM0A1) | (1 << COM0B1);  // Non-inverting mode
  //TCCR0B = (1 << CS00);  // No prescaling

  //// Timer1 Configuration for Fast PWM
  //TCCR1 = (1 << PWM1A) | (1 << COM1A1) | (1 << CS10);  // Fast PWM on OC1A, No prescaling
  //GTCCR = (1 << PWM1B) | (1 << COM1B1);  // Enable PWM for OC1B
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
  analogWrite(PWM_PIN_B, m_realColor.blue);
#else
  // backup SREG and turn off interrupts
  uint8_t oldSREG = SREG;
  cli();

  // the commented out stuff here is the full complete solution
  // that is equivalent to analogWrite -- first it checks if the value
  // is either 0 or 255 and just passes through to digitalWrite, then
  // if the value is not 0 or 255 it enables the analog PWM output.
  //
  // However in practice I can only seem to get the analog PWM output
  // working and the 'digitalWrite' part for 0 and 255 doesn't seem to
  // be working. This is likely the source of the 'glow' that is seen when
  // the led is supposed to be off, it's because the pin is not dropping
  // entirely to 0 as it would normally from digitalWrite(0)

  if (m_realColor.red <= 0) {
    // this is digitalWrite(0, LOW);
    TCCR0A &= ~(1 << COM0A1);
    PORTB &= ~(1 << PWM_PIN_R);
  } else {
    // this is analogWrite(0, realColor.red);
    TCCR0A |= (1 << COM0A1);
    OCR0A = m_realColor.red;
  }

  // TODO: uncommenting this section would be the correct way to
  //       disable the green LED when the output value g = 0 however
  //       it seems to break everything when I do this, I've tried all
  //       kinds of things but nothing seems to make it work
  //
  //if (m_realColor.green <= 0) {
  //  // this is digitalWrite(1, LOW);
  //  TCCR0A &= ~(1 << COM0B1);
  //  PORTB &= ~(1 << PWM_PIN_G);
  //} else {
      // this is analogWrite(1, realColor.green);
      TCCR0A |= (1 << COM0B1);
      OCR0B = m_realColor.green;
  //}

  if (m_realColor.blue <= 0) {
    // this is digitalWrite(4, LOW);
    GTCCR &= ~(1 << COM1B1);
    PORTB &= ~(1 << PWM_PIN_B);
  } else {
    // this is analogWrite(4, realColor.blue);
    GTCCR |= (1 << COM1B1);
    OCR1B = m_realColor.blue;
  }

  // turn interrupts back on
  SREG = oldSREG;
#endif
#endif
}
