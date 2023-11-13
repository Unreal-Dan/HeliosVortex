#include <math.h>

#include "Led.h"

#include "TimeControl.h"

#include "HeliosConfig.h"

#ifdef VORTEX_EMBEDDED
#include <avr/sleep.h>
#include <avr/io.h>
#endif

#define LED_DATA_PIN  7

// array of led color values
RGBColor Led::m_ledColor = RGB_OFF;
// global brightness
uint8_t Led::m_brightness = DEFAULT_BRIGHTNESS;

bool Led::init()
{
#ifdef HELIOS_EMBEDDED
  //// Set pins as outputs
  //DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB4);

  //// Configure Timer/Counter 0 for PB0 (pin 0) and PB1 (pin 1)
  //TCCR0A |= (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
  //TCCR0B |= (1 << CS00); // No prescaler for timer0

  //// Configure Timer/Counter 1 for PB4 (pin 4)
  //TCCR1 |= (1 << PWM1A) | (1 << COM1A1) | (1 << WGM10);
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
  //// write out the rgb values to analog pins
  //OCR0A = m_ledColor.red;
  //OCROB = m_ledColor.green;
  //OCR1A = m_ledColor.blue;
#endif
}
