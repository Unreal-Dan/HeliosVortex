#include "Mode.h"

//#include "PatternBuilder.h"
#include "Pattern.h"
#include "TimeControl.h"
#include "Colorset.h"
#include "Led.h"

//Mode::Mode(const DefaultModeEntry &entry) :
//  Mode()
//{
//  for (LedPos pos = LED_FIRST; pos < LED_COUNT; ++pos) {
//    Colorset set(entry.leds[pos].numColors, entry.leds[pos].cols);
//    setPattern(entry.leds[pos].patternID, pos, nullptr, &set);
//  }
//}

Mode::Mode() :
  m_pattern(nullptr)
{
}

Mode::Mode(PatternID id, const Colorset &set) :
  Mode()
{
  setPattern(id, nullptr, &set);
}

Mode::Mode(PatternID id, const PatternArgs &args, const Colorset &set) :
  Mode()
{
  setPattern(id, &args, &set);
}

Mode::Mode(PatternID id, const PatternArgs *args, const Colorset *set) :
  Mode()
{
  setPattern(id, args, set);
}

Mode::Mode(const Mode *other) :
  Mode()
{
  if (!other) {
    return;
  }
  *this = *other;
}

Mode::~Mode()
{
  clearPattern();
}

// copy and assignment operators
Mode::Mode(const Mode &other) :
  Mode()
{
  *this = other;
}

void Mode::operator=(const Mode &other)
{
  clearPattern();
  if (other.m_pattern) {
    //m_pattern = PatternBuilder::dupe(other.m_pattern);
  }
}

// equality operators
bool Mode::operator==(const Mode &other) const
{
  return equals(&other);
}

bool Mode::operator!=(const Mode &other) const
{
  return !equals(&other);
}

void Mode::init()
{
  if (m_pattern) {
    m_pattern->init();
  }
}

void Mode::play()
{
  if (m_pattern) {
    m_pattern->play();
  }
}

// this is a hairy function, but a bit of a necessary complexity
bool Mode::equals(const Mode *other) const
{
  if (!other) {
    return false;
  }
  if (m_pattern) {
    return m_pattern->equals(other->m_pattern);
  }
  // current is null, check if other is valid
  if (other->m_pattern) {
    return false;
  }
  // both are null
  return true;
}

const Pattern *Mode::getPattern() const
{
  return ((Mode *)this)->getPattern();
}

Pattern *Mode::getPattern()
{
  return m_pattern;
}

const Colorset Mode::getColorset() const
{
  return ((Mode *)this)->getColorset();
}

Colorset Mode::getColorset()
{
  Pattern *pat = getPattern();
  if (!pat) {
    return Colorset();
  }
  return pat->getColorset();
}

PatternID Mode::getPatternID() const
{
  const Pattern *pat = getPattern();
  if (!pat) {
    return PATTERN_NONE;
  }
  return pat->getPatternID();
}

bool Mode::setPattern(PatternID pat, const PatternArgs *args, const Colorset *set)
{
  // Use provided colorset, or colorset from pos if valid, otherwise use effective colorset
  Colorset newSet = set ? *set : getColorset();
  if (m_pattern) {
    delete m_pattern;
  }
  //m_pattern = PatternBuilder::makeSingle(pat, args);
  // they could set PATTERN_NONE to clear
  if (m_pattern) {
    m_pattern->setColorset(newSet);
  }
  return true;
}

// set colorset at a specific position
bool Mode::setColorset(const Colorset &set)
{
  if (m_pattern) {
    m_pattern->setColorset(set);
  }
  // actually break here
  return true;
}

void Mode::clearPattern()
{
  setPattern(PATTERN_NONE);
}

void Mode::clearColorset()
{
  Colorset empty;
  setColorset(empty);
}

void Mode::setArg(uint8_t param, uint8_t value)
{
  if (!m_pattern) {
    return;
  }
  m_pattern->setArg(param, value);
}

uint8_t Mode::getArg(uint8_t index)
{
  if (!m_pattern) {
    return 0;
  }
  return m_pattern->getArg(index);
}

bool Mode::isEmpty() const
{
  // if any patterns are non-null and not PATTERN_NONE
  if (m_pattern && m_pattern->getPatternID() != PATTERN_NONE) {
    // then this mode is not empty
    return false;
  }
  return true;
}
