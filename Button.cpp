#include "Button.h"
#include "TimeControl.h"

#ifdef HELIOS_EMBEDDED
#include <avr/io.h>
#include <arduino.h>
#define BUTTON_PIN 3
#define BUTTON_PORT 2
#endif

#ifdef HELIOS_CLI
#include "Helios.h"
#endif

// static members of Button
uint32_t Button::m_pressTime = 0;
uint32_t Button::m_releaseTime = 0;
uint32_t Button::m_holdDuration = 0;
uint32_t Button::m_releaseDuration = 0;
uint8_t Button::m_releaseCount = 0;
bool Button::m_buttonState = false;
bool Button::m_newPress = false;
bool Button::m_newRelease = false;
bool Button::m_isPressed = false;
bool Button::m_shortClick = false;
bool Button::m_longClick = false;

#ifdef HELIOS_CLI
// an input queue for the button, each tick one even is processed
// out of this queue and used to produce input
std::queue<char> Button::m_inputQueue;
// the virtual pin state
bool Button::m_pinState = false;
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
  pinMode(3, INPUT);
  //DDRB &= ~(1 << 3);
  // Set pin3 (Port B3) as input
  //DDRB &= ~(1 << 3);
  //PORTB &= ~(1 << 3); // Enable internal pull-up resistor for PB4
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
  return digitalRead(3) == HIGH;
  //return (PINB & (1 << 3)) != 0;
#elif defined(HELIOS_CLI)
  // then just return the pin state as-is, the input event may have
  // adjusted this value
  return m_pinState;
#endif
}

// poll the button pin and update the state of the button object
void Button::update()
{
#ifdef HELIOS_CLI
  // process any pre-input events in the queue
  bool processed_pre = processPreInput();
#endif

  bool newButtonState = check();
  m_newPress = false;
  m_newRelease = false;
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

#ifdef HELIOS_CLI
  // if there was no pre-input event this tick, process a post input event
  // to ensure there is only one event per tick processed
  if (!processed_pre) {
    processPostInput();
  }
#endif
}

#ifdef HELIOS_CLI
bool Button::processPreInput()
{
  if (!m_inputQueue.size()) {
    return false;
  }
  char command = m_inputQueue.front();
  switch (command) {
  case 'p': // press
    Button::doPress();
    break;
  case 'r': // release
    Button::doRelease();
    break;
  case 't': // toggle
    Button::doToggle();
    break;
  case 'q': // quit
    Helios::terminate();
    break;
  case 'w': // wait
    // wait is pre input I guess
    break;
  default:
    // return here! do not pop the queue
    // do not process post input events
    return false;
  }
  // now pop whatever pre-input command was processed
  m_inputQueue.pop();
  return true;
}

bool Button::processPostInput()
{
  if (!m_inputQueue.size()) {
    // probably processed the pre-input event already
    return false;
  }
  // process input queue from the command line
  char command = m_inputQueue.front();
  switch (command) {
  case 'c': // click button
    Button::doShortClick();
    break;
  case 'l': // long click button
    Button::doLongClick();
    break;
  default:
    // should never happen
    return false;
  }
  m_inputQueue.pop();
  return true;
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
}

void Button::doRelease()
{
  m_pinState = false;
}

void Button::doToggle()
{
  m_pinState = !m_pinState;
}

// queue up an input event for the button
void Button::queueInput(char input)
{
  m_inputQueue.push(input);
}

uint32_t Button::inputQueueSize()
{
  return m_inputQueue.size();
}
#endif

// global button
Button button;
