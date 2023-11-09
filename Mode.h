#ifndef MODE_H
#define MODE_H

#include "Patterns.h"
#include "HeliosConfig.h"
//#include "DefaultModes.h"

class PatternArgs;
class Colorset;
class Pattern;

typedef uint8_t ModeFlags;

class Mode
{
public:
  Mode();

  //Mode(const DefaultModeEntry &entry);
  Mode(PatternID id, const Colorset &set);
  Mode(PatternID id, const PatternArgs &args, const Colorset &set);
  Mode(PatternID id, const PatternArgs *args, const Colorset *set);
  Mode(const Mode *other);
  ~Mode();

  // copy and assignment operators
  Mode(const Mode &other);
  void operator=(const Mode &other);

  // equality operators
  bool operator==(const Mode &other) const;
  bool operator!=(const Mode &other) const;

  // initialize the mode to initial state
  void init();

  // Play the mode
  void play();

  // mode comparison
  bool equals(const Mode *other) const;

  // get the pattern at a position
  const Pattern *getPattern() const;
  Pattern *getPattern();

  // get the colorset at a position
  const Colorset getColorset() const;
  Colorset getColorset();

  // get the pattern ID of the given pattern
  PatternID getPatternID() const;

  // change the pattern on a mode (NOTE: you may need to call init() after!)
  bool setPattern(PatternID pat, const PatternArgs *args = nullptr, const Colorset *set = nullptr);

  // set colorset at a specific position
  bool setColorset(const Colorset &set);

  // clear stored patterns in various ways
  void clearPattern();

  // clear colorset in various ways
  void clearColorset();

  // set/get a single argument on various positions
  void setArg(uint8_t index, uint8_t value);
  uint8_t getArg(uint8_t index);

  // whether the mode contains no patterns, or all patterns are PATTERN_NONE
  bool isEmpty() const;

private:
  // list of pointers to Patterns, one for each led
  Pattern *m_pattern;
};

#endif
