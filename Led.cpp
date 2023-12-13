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

// array of led color values
RGBColor Led::m_ledColor = RGB_OFF;
// global brightness
uint8_t Led::m_brightness = DEFAULT_BRIGHTNESS;

bool Led::init()
{
#ifdef HELIOS_EMBEDDED
#ifdef HELIOS_ARDUINO
  pinMode(0, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(4, OUTPUT);
#else
  // Set pins as outputs
  DDRB |= (1 << 0) | (1 << 1) | (1 << 4);
  // Timer/Counter0 in Fast PWM mode
  //TCCR0A |= (1 << WGM01) | (1 << WGM00);
  // Clear OC0A and OC0B on compare match, set at BOTTOM (non-inverting mode)
  //TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
  // Use clk/8 prescaler (adjust as needed)
  //TCCR0B |= (1 << CS01);

  //TCCR1 |= (1 << PWM1A) | (1 << COM1A1) | (1 << PWM1B) | (1 << COM1B1);
  //GTCCR |= (1 << PWM1B);
#endif
#endif
  return true;
}

void Led::cleanup()
{
  m_ledColor.clear();
}

void Led::set(RGBColor col)
{
  m_ledColor = col;
}

void Led::adjustBrightness(uint8_t fadeBy)
{
  m_ledColor.adjustBrightness(fadeBy);
}

void Led::strobe(uint8_t on_time, uint8_t off_time, RGBColor off_col, RGBColor on_col)
{
  set(((Time::getCurtime() % (on_time + off_time)) > on_time) ? off_col : on_col);
}

void Led::blinkOffset(uint32_t time, uint16_t offMs, uint16_t onMs, RGBColor col)
{
  if ((time % MS_TO_TICKS(offMs + onMs)) < MS_TO_TICKS(onMs)) {
    set(col);
  }
}

void Led::blink(uint16_t offMs, uint16_t onMs, RGBColor col)
{
  if ((Time::getCurtime() % MS_TO_TICKS(offMs + onMs)) < MS_TO_TICKS(onMs)) {
    set(col);
  }
}

void Led::breath(uint8_t hue, uint32_t variance, uint32_t magnitude, uint8_t sat, uint8_t val)
{
  set(HSVColor((uint8_t)(hue + ((sin(variance * 0.0174533) + 1) * magnitude)), sat, val));
}

void Led::breathSat(uint8_t hue, uint32_t variance, uint32_t magnitude, uint8_t sat, uint8_t val)
{
  set(HSVColor(hue, 255 - (uint8_t)(sat + 128 + ((sin(variance * 0.0174533) + 1) * magnitude)), val));
}

void Led::breathVal(uint8_t hue, uint32_t variance, uint32_t magnitude, uint8_t sat, uint8_t val)
{
  set(HSVColor(hue, sat, 255 - (uint8_t)(val + 128 + ((sin(variance * 0.0174533) + 1) * magnitude))));
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
  analogWrite(PWM_PIN_R, m_ledColor.red);
  analogWrite(PWM_PIN_G, m_ledColor.green);
  analogWrite(PWM_PIN_B, m_ledColor.blue);
#else
  // a counter to keep track of milliseconds for the PWM
  uint8_t counter = Time::getCurtime() % 100;
  // run the software PWM on each pin
  if (counter <= m_ledColor.red) { 
    PORTB |= (1 << PWM_PIN_R);
  } else {
    PORTB &= ~(1 << PWM_PIN_R);
  }
  if (counter <= m_ledColor.green) {
    PORTB |= (1 << PWM_PIN_G);
  } else {
    PORTB &= ~(1 << PWM_PIN_G);
  }
  if (counter <= m_ledColor.blue) {
    PORTB |= (1 << PWM_PIN_B);
  } else {
    PORTB &= ~(1 << PWM_PIN_B);
  }
#endif
#endif
}
