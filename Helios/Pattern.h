#ifndef PATTERN_H
#define PATTERN_H

#include "Colorset.h"

#include "Timer.h"
#include "Patterns.h"

// for specifying things like default args
struct PatternArgs {
  PatternArgs() : on_dur(0), off_dur(0), gap_dur(0), dash_dur(0),
    group_size(0), blend_speed(0)
  {}
  uint8_t on_dur;
  uint8_t off_dur;
  uint8_t gap_dur;
  uint8_t dash_dur;
  uint8_t group_size;
  uint8_t blend_speed;
};

class Pattern
{
public:
  // try to not set on duration to 0
  Pattern(uint8_t onDur = 1, uint8_t offDur = 0, uint8_t gap = 0,
          uint8_t dash = 0, uint8_t group = 0, uint8_t blend = 0);
  Pattern(const PatternArgs &args);
  ~Pattern();

  // init the pattern to initial state
  void init();

  // pure virtual must override the play function
  void play();

  // set args
  void setArgs(const PatternArgs &args);

  // comparison to other pattern
  // NOTE: That may cause problems because the parameter is still a Pattern *
  //       which means comparison would need to cast the other upwards first
  // NOTE2: Removing virtual because this probably shouldn't be overridden
  bool equals(const Pattern *other);

  // change the colorset
  const Colorset getColorset() const { return m_colorset; }
  Colorset getColorset() { return m_colorset; }
  Colorset &colorset() { return m_colorset; }
  void setColorset(const Colorset &set);
  void clearColorset();

  // set a color in the colorset and re-initialize
  void updateColor(uint8_t index, const RGBColor &col);

  // get the pattern flags
  uint32_t getFlags() const { return m_patternFlags; }
  bool hasFlags(uint32_t flags) const { return (m_patternFlags & flags) != 0; }

protected:
  // ==================================
  //  Pattern Parameters
  PatternArgs m_args;

  // ==================================
  //  Pattern Members

  // any flags the pattern has
  uint8_t m_patternFlags;
  // a copy of the colorset that this pattern is initialized with
  Colorset m_colorset;

  // ==================================
  //  Blink Members
  uint8_t m_groupCounter;

  // apis for blink
  void onBlinkOn();
  void onBlinkOff();
  void beginGap();
  void beginDash();
  void nextState(uint8_t timing);

  // the various different blinking states the pattern can be in
  enum PatternState : uint8_t
  {
    // the led is disabled (there is no on or dash)
    STATE_DISABLED,

    // the pattern is blinking on the next color in the set
    STATE_BLINK_ON,
    STATE_ON,

    // the pattern is blinking off
    STATE_BLINK_OFF,
    STATE_OFF,

    // the pattern is starting a gap after a colorset
    STATE_BEGIN_GAP,
    STATE_IN_GAP,

    // the pattern is beginning a dash after a colorset or gap
    STATE_BEGIN_DASH,
    STATE_IN_DASH,

    // the pattern is starting a gap after a dash
    STATE_BEGIN_GAP2,
    STATE_IN_GAP2,
  };

  // the state of the current pattern
  PatternState m_state;

  // the blink timer used to measure blink timings
  Timer m_blinkTimer;

  // ==================================
  //  Blend Members

  // current color and target blend color
  RGBColor m_cur;
  RGBColor m_next;

  // apis for blend
  void blendBlinkOn();
  void interpolate(uint8_t &current, const uint8_t next);
};

#endif
