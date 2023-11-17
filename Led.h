#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <inttypes.h>

#include "ColorTypes.h"

class Led
{
  // private unimplemented constructor
  Led();

public:
  // opting for static class here because there should only ever be one
  // Led control object and I don't like singletons
  static bool init();
  static void cleanup();

  // control individual LED, these are appropriate to use in internal pattern logic
  static void set(RGBColor col);

  // Turn off individual LEDs, these are appropriate to use in internal pattern logic
  static void clear() { set(RGB_OFF); }

  // Dim individual LEDs, these are appropriate to use in internal pattern logic
  static void adjustBrightness(uint8_t fadeBy);

  // strobe between two colors with a simple on/off timing
  static void strobe(uint8_t on_time, uint8_t off_time, RGBColor col1, RGBColor col2);

  // Blink an led to blank or a color
  //
  // These APIs work by checking if the current time is within the
  // 'on duration' then they apply the color to the target, otherwise they do
  // nothing. The goal of this behaviour is to allow them to be used in any
  // situation to 'blink' an led to either off or some color.
  //
  // However since these APIs modulate current time to check if within the 'on'
  // threshold that makes them unsuitable for internal pattern usage because it
  // is unpredictable whether they will blink on or off first
  static void blinkOffset(uint32_t time, uint16_t offMs = 250, uint16_t onMs = 500, RGBColor col = RGB_OFF);
  static void blink(uint16_t offMs = 250, uint16_t onMs = 500, RGBColor col = RGB_OFF);

  // breath the hue on an index
  // warning: these use hsv to rgb in realtime!
  static void breath(uint8_t hue, uint32_t variance,
    uint32_t magnitude = 15, uint8_t sat = 255, uint8_t val = 210);
  static void breathSat(uint8_t hue, uint32_t variance,
    uint32_t magnitude = 15, uint8_t sat = 255, uint8_t val = 210);
  static void breathVal(uint8_t hue, uint32_t variance,
    uint32_t magnitude = 15, uint8_t sat = 255, uint8_t val = 210);

  // a very specialized api to hold all leds on a color for 250ms
  static void hold(RGBColor col);

  // get the RGBColor of an Led index
  static RGBColor get() { return m_ledColor; }

  // global brightness
  static uint8_t getBrightness() { return m_brightness; }
  static void setBrightness(uint8_t brightness) { m_brightness = brightness; }

  // actually update the LEDs and show the changes
  static void update();

private:
  // the global brightness
  static uint8_t m_brightness;
  // led color
  static RGBColor m_ledColor;
};

#endif
