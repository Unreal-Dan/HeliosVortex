#ifndef PATTERNS_H
#define PATTERNS_H

#include <inttypes.h>

// List of patterns that can be built, both single and multi-led patterns are found in this list.
// Within both single and multi LED pattern lists there are 'core' patterns which are associated
// with a class, and there are 'shell' patterns which are simply wrapperns around another pattern
// with different parameters passed to the constructor.  There is no way to know which patterns
// are 'core' patterns, except by looking at PatternBuilder::generate to see which classes exist
enum PatternID : int8_t
{
  // no pattern at all, use this sparingly and default to
  // PATTERN_FIRST when possible
  PATTERN_NONE = (PatternID)-1,

  // first pattern of all
  PATTERN_FIRST = 0,
  // =====================================

  // all 'single led' patterns below

  PATTERN_STROBE = PATTERN_FIRST,
  PATTERN_HYPERSTROBE,
  PATTERN_PICOSTROBE,
  PATTERN_STROBIE,
  PATTERN_DOPS,
  PATTERN_ULTRADOPS,
  PATTERN_STROBEGAP,
  PATTERN_HYPERGAP,
  PATTERN_PICOGAP,
  PATTERN_STROBIEGAP,
  PATTERN_DOPSGAP,
  PATTERN_ULTRAGAP,
  PATTERN_BLINKIE,
  PATTERN_GHOSTCRUSH,
  PATTERN_DOUBLEDOPS,
  PATTERN_CHOPPER,
  PATTERN_DASHGAP,
  PATTERN_DASHDOPS,
  PATTERN_DASHCRUSH,
  PATTERN_ULTRADASH,
  PATTERN_GAPCYCLE,
  PATTERN_DASHCYCLE,
  PATTERN_TRACER,
  PATTERN_RIBBON,
  PATTERN_MINIRIBBON,
  PATTERN_BLEND,
  PATTERN_BLENDSTROBE,
  PATTERN_BLENDSTROBEGAP,
  PATTERN_COMPLEMENTARY_BLEND,
  PATTERN_COMPLEMENTARY_BLENDSTROBE,
  PATTERN_COMPLEMENTARY_BLENDSTROBEGAP,
  PATTERN_SOLID,

  // ADD NEW PATTERNS HERE

  // Meta pattern constants
  INTERNAL_PATTERNS_END,
  PATTERN_LAST = (INTERNAL_PATTERNS_END - 1),
  PATTERN_COUNT = (PATTERN_LAST - PATTERN_FIRST) + 1, // total number of patterns
};

class Pattern;

class Patterns
{
public:

  static void make_pattern(PatternID id, Pattern &pat);
};

// PatternID operators
inline PatternID &operator++(PatternID &c)
{
  c = PatternID(((uint16_t)c) + 1);
  return c;
}
inline PatternID operator++(PatternID &c, int)
{
  PatternID temp = c;
  ++c;
  return temp;
}
inline PatternID operator+(PatternID &c, int b)
{
  return (PatternID)((uint16_t)c + b);
}
inline PatternID operator-(PatternID &c, int b)
{
  return (PatternID)((uint16_t)c - b);
}

#endif
