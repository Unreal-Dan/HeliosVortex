#ifndef PATTERNS_H
#define PATTERNS_H

#include <inttypes.h>

// List of patterns that can be built, both single and multi-led patterns are found in this list.
// Within both single and multi LED pattern lists there are 'core' patterns which are associated
// with a class, and there are 'shell' patterns which are simply wrapperns around another pattern
// with different parameters passed to the constructor.  There is no way to know which patterns
// are 'core' patterns, except by looking at PatternBuilder::generate to see which classes exist
enum PatternID : int8_t {
  // no pattern at all, use this sparingly and default to
  // PATTERN_FIRST when possible
  PATTERN_NONE = (PatternID)-1,

  // first pattern of all
  PATTERN_FIRST = 0,
  // =====================================

  // all 'single led' patterns below

  PATTERN_RIBBON = PATTERN_FIRST,
  PATTERN_ULTRA_DOPS,
  PATTERN_DOPS,
  PATTERN_STROBE,
  PATTERN_STROBIE,
  PATTERN_HYPERSTROBE,
  PATTERN_HYPNOSTROBE,
  PATTERN_FLARE,
  PATTERN_GLOW,
  PATTERN_FLICKER,
  PATTERN_BLINKIE,
  INOVA_BLINK,
  PATTERN_BLEND,
  PATTERN_BLEND_STROBE,
  PATTERN_BLEND_STROBIE,
  PATTERN_DASHCYCLE,
  PATTERN_ULTRADASH,
  PATTERN_DASHDOPS,
  PATTERN_TRACER,
  PATTERN_MINIRIBBON,

  PATTERN_BLENDSTROBEGAP,

  PATTERN_STROBEGAP,
  PATTERN_HYPERGAP,
  PATTERN_STROBIEGAP,
  PATTERN_DOPSGAP,
  PATTERN_GHOSTCRUSH,
  PATTERN_DOUBLEDOPS,
  PATTERN_CHOPPER,
  PATTERN_DASHGAP,

  PATTERN_DASHCRUSH,

  PATTERN_GAPCYCLE,

  PATTERN_COMPLEMENTARY_BLEND,
  PATTERN_COMPLEMENTARY_BLENDSTROBE,
  PATTERN_COMPLEMENTARY_BLENDSTROBEGAP,
  PATTERN_SOLID,

  // ADD NEW PATTERNS HERE

  // Meta pattern constants
  INTERNAL_PATTERNS_END,
  PATTERN_LAST = (INTERNAL_PATTERNS_END - 1),
  PATTERN_COUNT = (PATTERN_LAST - PATTERN_FIRST) + 1,  // total number of patterns
};

class Pattern;

class Patterns {
 public:
  static void make_default(uint8_t index, Pattern &pat);
  static void make_pattern(PatternID id, Pattern &pat);
};

#endif
