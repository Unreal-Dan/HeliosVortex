#include "Button.h"
#include "TimeControl.h"

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
  return true;
}

// directly poll the pin for whether it's pressed right now
bool Button::check()
{
  return false; //digitalRead(1) == HIGH;
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

// global button
Button button;
