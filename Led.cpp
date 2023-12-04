#include <math.h>

#include "Led.h"

#include "TimeControl.h"

#include "HeliosConfig.h"

#ifdef HELIOS_EMBEDDED
#include <avr/sleep.h>
#include <avr/io.h>
#ifdef HELIOS_ARDUINO
#include <arduino.h>
#endif
#endif

#define LED_DATA_PIN  7

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
  DDRB |= (1 << DDB0) | (1 << DDB1) | (1 << DDB4);
  // Timer/Counter0 in Fast PWM mode
  TCCR0A |= (1 << WGM01) | (1 << WGM00);
  // Clear OC0A and OC0B on compare match, set at BOTTOM (non-inverting mode)
  TCCR0A |= (1 << COM0A1) | (1 << COM0B1);
  // Use clk/8 prescaler (adjust as needed)
  TCCR0B |= (1 << CS01);
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

#define NOT_ON_TIMER 0
#define TIMER0A 1
#define TIMER0B 2
#define TIMER1A 3
#define TIMER1B 4
#define TIMER1C 5
#define TIMER2  6
#define TIMER2A 7
#define TIMER2B 8

#define TIMER3A 9

#define LOW 0
#define HIGH 1
#define TIMER3B 10
#define TIMER3C 11
#define TIMER4A 12
#define TIMER4B 13
#define TIMER4C 14
#define TIMER4D 15
#define TIMER5A 16
#define TIMER5B 17
#define TIMER5C 18

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//#define LOW 0
//#define HIGH 1
//
//const uint8_t PROGMEM digital_pin_to_timer_PGM[] = {
//  TIMER0A, /* OC0A */
//  TIMER0B, /* OC0B */
//  NOT_ON_TIMER,
//  NOT_ON_TIMER,
//  TIMER1B, /*OC1B*/
//  NOT_ON_TIMER,
//};

void Led::update()
{
#ifdef HELIOS_EMBEDDED
  // write out the rgb values to analog pins
#ifdef HELIOS_ARDUINO
  analogWrite(0, m_ledColor.red);
  analogWrite(1, m_ledColor.green);
  analogWrite(4, m_ledColor.blue);
#else
  sbi(TCCR0A, COM0A1);    
  sbi(TCCR0A, COM0B1);    
  OCR0A = m_ledColor.red; // set pwm duty
  OCR0B = m_ledColor.green; // set pwm duty
  //for (int i = 0; i < 255; i++) {
  //  if (i < m_ledColor.blue) {
  //    PORTB |= (1 << PWM_PIN_B); // Turn on blue
  //  } else {
  //    PORTB &= ~(1 << PWM_PIN_B); // Turn off blue
  //  }
  //  asm("nop");
  //  asm("nop");
  //  asm("nop");
  //  asm("nop");
  //  asm("nop");
  //  asm("nop");
  //  asm("nop");
  //  asm("nop");
  //}
#endif
#endif
}
