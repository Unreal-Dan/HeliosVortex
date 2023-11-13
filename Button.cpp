#include "Button.h"
#include "TimeControl.h"

#ifdef HELIOS_EMBEDDED
#include <avr/io.h>
#define BUTTON_PIN 3
#endif

#ifdef HELIOS_CLI
#include "Helios.h"
#endif

// initialize a new button object with a pin number
bool Button::init()
{
  m_pressTime = 0;
  m_releaseTime = 0;
  m_holdDuration = 0;
  m_releaseDuration = 0;
  m_newPress = false;
  m_newRelease = false;
  m_shortClick = false;
  m_longClick = false;
  // this is weird, when I did m_releaseCount = !m_buttonState the
  // compiler generated a huge amount of assembly, but not !check()
  m_buttonState = check();
  m_releaseCount = !check();
  m_isPressed = m_buttonState;
#ifdef HELIOS_CLI
  m_pinState = false;
#endif
#ifdef HELIOS_EMBEDDED
  // Set pin3 (Port B4) as input
  DDRB &= ~(1 << PB4);

  // Enable Pin Change Interrupt on the BUTTON pin
  //GIMSK |= _BV(PCIE);
  //PCMSK |= _BV(PIN_BUTTON);
  //sei();  // Enable interrupts
#endif
  return true;
}

// directly poll the pin for whether it's pressed right now
bool Button::check()
{
#ifdef HELIOS_EMBEDDED
  return ((PINB & (1 << PB4)) != 0);
#elif defined(HELIOS_CLI)
  // in CLI mode we directly manipulate the button state
  // so this api is kinda pointless, because it's used in
  // update() to do newState = check()
  return m_pinState;
#endif
}

// poll the button pin and update the state of the button object
void Button::update()
{
  m_newPress = false;
  m_newRelease = false;
  bool newButtonState = check();
  if (newButtonState != m_buttonState) {
    m_buttonState = newButtonState;
    m_isPressed = m_buttonState;
    if (m_isPressed) {
      m_pressTime = Time::getCurtime();
      m_newPress = true;
    } else {
      if (m_releaseCount > 0) {
        m_releaseTime = Time::getCurtime();
        m_newRelease = true;
      }
      m_releaseCount++;
    }
  }
  if (m_isPressed) {
    m_holdDuration = (Time::getCurtime() >= m_pressTime) ? (uint32_t)(Time::getCurtime() - m_pressTime) : 0;
  } else {
    m_releaseDuration = (Time::getCurtime() >= m_releaseTime) ? (uint32_t)(Time::getCurtime() - m_releaseTime) : 0;
  }
  m_shortClick = (m_newRelease && (m_holdDuration <= SHORT_CLICK_THRESHOLD));
  m_longClick = (m_newRelease && (m_holdDuration > SHORT_CLICK_THRESHOLD));
}

#ifdef HELIOS_CLI
#include <stdio.h>
void Button::processInput()
{
  // process input queue from the command line
  if (m_inputQueue.size() > 0) {
    char command = m_inputQueue.front();
    m_inputQueue.pop();
    switch (command) {
    case 'c': // click button
      button.doShortClick();
      break;
    case 'l': // long click button
      button.doLongClick();
      break;
    case 'p': // press
      button.doPress();
      break;
    case 'r': // release
      button.doRelease();
      break;
    case 't': // toggle
      button.doToggle();
      break;
    case 'q': // quit
      Helios::terminate();
      break;
    case 'w': // wait
    default:
      break;
    }
  }
}

void Button::doShortClick()
{
  m_shortClick = true;
}

void Button::doLongClick()
{
  m_longClick = true;
}

// this will actually press down the button, it's your responsibility to wait
// for the appropriate number of ticks and then release the button
void Button::doPress()
{
  m_pinState = true;
  update();
}

void Button::doRelease()
{
  m_pinState = false;
  update();
}

void Button::doToggle()
{
  m_pinState = !m_pinState;
  update();
}

// queue up an input event for the button
void Button::queueInput(char input)
{
  m_inputQueue.push(input);
}

uint32_t Button::inputQueueSize() const
{
  return m_inputQueue.size();
}
#endif

// global button
Button button;
