#ifndef PATTERN_H
#define PATTERN_H

#include "Colorset.h"

#include "Patterns.h"
#include "PatternArgs.h"

#define MAX_PATTERN_ARGS 8

#define PATTERN_FLAGS_NONE  0

// the pattern is a multi-pattern
#define PATTERN_FLAG_MULTI  (1<<0)

// macro to register args of a pattern
#define REGISTER_ARG(arg) registerArg((uint8_t)(((uintptr_t)&arg - (uintptr_t)this)));

class ByteStream;

class Pattern
{
protected:
  // Pattern is an abstract class
  Pattern();

  Pattern(const PatternArgs &args);

public:
  virtual ~Pattern();

  // init the pattern to initial state
  virtual void init();

  // pure virtual must override the play function
  virtual void play() = 0;

  // get or set a single arg
  void setArg(uint8_t index, uint8_t value);
  uint8_t getArg(uint8_t index) const;

  uint8_t &argRef(uint8_t index);

  // get or set the entire list of pattern args
  void setArgs(const PatternArgs &args);
  void getArgs(PatternArgs &args) const;

  // number of args the pattern has
  uint8_t getNumArgs() const { return m_numArgs; }

  // comparison to other pattern
  // NOTE: That may cause problems because the parameter is still a Pattern *
  //       which means comparison would need to cast the other upwards first
  // NOTE2: Removing virtual because this probably shouldn't be overridden
  bool equals(const Pattern *other);

  // change the colorset
  const Colorset getColorset() const { return m_colorset; }
  Colorset getColorset() { return m_colorset; }
  void setColorset(const Colorset &set);
  void clearColorset();

  // get/set the ID of the pattern (set by mode builder)
  PatternID getPatternID() const { return m_patternID; }

  // get the pattern flags
  uint32_t getFlags() const { return m_patternFlags; }
  bool hasFlags(uint32_t flags) const { return (m_patternFlags & flags) != 0; }

protected:
  // the ID of this pattern (set by pattern builder)
  PatternID m_patternID;
  // any flags the pattern has
  uint8_t m_patternFlags;
  // a copy of the colorset that this pattern is initialized with
  Colorset m_colorset;

  uint8_t m_numArgs;
  uint8_t m_argList[MAX_PATTERN_ARGS];

  void registerArg(uint8_t argOffset);
};

#endif
