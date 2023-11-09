#ifndef TIME_CONTROL_H
#define TIME_CONTROL_H

#include <inttypes.h>

#include "HeliosConfig.h"

#define TICKRATE 1000

#define MS_TO_TICKS(ms) (uint32_t)(((uint32_t)(ms) * TICKRATE) / 1000)
#define SEC_TO_TICKS(s) (uint32_t)((uint32_t)(s) * TICKRATE)

class Time
{
  // private unimplemented constructor
  Time();

public:
  // opting for static class here because there should only ever be one
  // Settings control object and I don't like singletons
  static bool init();
  static void cleanup();

  // tick the clock forward to millis()
  static void tickClock();

  // get the current tick, offset by any active simulation (simulation only exists in vortexlib)
  // Exposing this in the header seems to save on space a non negligible amount, it is used a lot
  // and exposing in the header probably allows the compiler to optimize away repititive calls
  static uint32_t getCurtime() { return m_curTick; }

  // this ignore simulation time, it's used by timers to make simulations work
  static uint32_t getRealCurtime();

  // Set tickrate in Ticks Per Second (TPS)
  // The valid range for this is 1 <= x <= 1000000
  //
  // Setting a value of 0 will restore the default
  //
  // NOTE: Patterns will not change when this changes, you
  //       must re-create the pattern for the change to take
  //       effect. This is done by design to allow the test
  //       framework to control the speed of patterns.
  //       See PatternBuilder.cpp for more info.
  static void setTickrate(uint32_t tickrate = 0);

  // The current tickrate
  static uint32_t getTickrate();

  // Current microseconds since startup, only use this for things like measuring rapid data transfer timings.
  // If you just need to perform regular time checks for a pattern or some logic then use getCurtime() and measure 
  // time in ticks, use the SEC_TO_TICKS() or MS_TO_TICKS() macros to convert timings to measures of ticks for
  // purpose of comparing against getCurtime()
  static uint32_t microseconds();

  // delay for some number of microseconds or milliseconds, these are bad
  static void delayMicroseconds(uint32_t us);
  static void delayMilliseconds(uint32_t ms);

private:
  static void initMCUTime();

  // global tick counter
  static uint32_t m_curTick;
  // the last frame timestamp
  static uint32_t m_prevTime;
};

#endif

