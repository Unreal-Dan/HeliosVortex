#include <math.h>

#include "Led.h"

#include "TimeControl.h"

#include "HeliosConfig.h"

#ifdef VORTEX_EMBEDDED
#include <avr/io.h>
#include <string.h>
#endif

#define LED_DATA_PIN  7

// array of led color values
RGBColor Led::m_ledColor = RGB_OFF;
// global brightness
uint8_t Led::m_brightness = DEFAULT_BRIGHTNESS;

// Output PORT register
volatile uint8_t *Led::m_port = nullptr;
// Output PORT bitmask
uint8_t Led::m_pinMask = 0;

bool Led::init()
{
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
  //Time::delayMilliseconds(250);
}

void Led::update()
{
}

