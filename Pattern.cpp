#include "Pattern.h"

//#include "../Patterns/PatternBuilder.h"
#include "TimeControl.h"
#include "Colorset.h"

#include "HeliosConfig.h"

Pattern::Pattern() :
  m_patternID(PATTERN_FIRST),
  m_patternFlags(0),
  m_colorset(),
  m_numArgs(0),
  m_argList()
{
}

Pattern::Pattern(const PatternArgs &args) :
  Pattern()
{
}

Pattern::~Pattern()
{
}

void Pattern::init()
{
  m_colorset.resetIndex();
}

void Pattern::setArg(uint8_t index, uint8_t value)
{
  if (index >= m_numArgs) {
    return;
  }
  *((uint8_t *)this + m_argList[index]) = value;
}

uint8_t Pattern::getArg(uint8_t index) const
{
  if (index >= m_numArgs) {
    return 0;
  }
  return *((uint8_t *)this + m_argList[index]);
}

uint8_t &Pattern::argRef(uint8_t index)
{
  if (index >= m_numArgs) {
    index = 0;
  }
  return *((uint8_t *)this + m_argList[index]);
}

void Pattern::setArgs(const PatternArgs &args)
{
  for (uint32_t i = 0; i < m_numArgs; ++i) {
    *((uint8_t *)this + m_argList[i]) = args.args[i];
  }
}

void Pattern::getArgs(PatternArgs &args) const
{
  args.init();
  for (uint32_t i = 0; i < m_numArgs; ++i) {
    args.addArgs(*((uint8_t *)this + m_argList[i]));
  }
}

bool Pattern::equals(const Pattern *other)
{
  if (!other) {
    return false;
  }
  // compare pattern id
  if (m_patternID != other->m_patternID) {
    return false;
  }
  // then colorset
  if (!m_colorset.equals(&other->m_colorset)) {
    return false;
  }
  // number of args
  if (m_numArgs != other->m_numArgs) {
    return false;
  }
  // compare each arg
  for (uint8_t i = 0; i < m_numArgs; ++i) {
    if (getArg(i) != other->getArg(i)) {
      return false;
    }
  }
  return true;
}

// change the colorset
void Pattern::setColorset(const Colorset &set)
{
  m_colorset = set;
}

void Pattern::clearColorset()
{
  m_colorset.clear();
}

void Pattern::registerArg(uint8_t argOffset)
{
  if (m_numArgs >= MAX_PATTERN_ARGS) {
    //ERROR_LOG("too many args");
    return;
  }
  m_argList[m_numArgs++] = argOffset;
}
