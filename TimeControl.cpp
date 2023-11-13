#include "TimeControl.h"

#include <math.h>

#include "Timings.h"

#include "Led.h"

#ifdef VORTEX_EMBEDDED
#include <avr/sleep.h>
#include <avr/interrupt.h>
#endif

#ifdef HELIOS_CLI
#include <unistd.h>
#include <time.h>
uint64_t start = 0;
// convert seconds and nanoseconds to microseconds
#define SEC_TO_US(sec) ((sec)*1000000)
#define NS_TO_US(ns) ((ns)/1000)
#endif

// static members
uint32_t Time::m_curTick = 0;
// the last frame timestamp
uint32_t Time::m_prevTime = 0;

bool Time::init()
{
  m_prevTime = microseconds();
  m_curTick = 0;
  return true;
}

void Time::cleanup()
{
}

void Time::tickClock()
{
  // tick clock forward
  m_curTick++;

  // the rest of this only runs inside vortexlib because on the duo the tick runs in the
  // tcb timer callback instead of in a busy loop constantly checking microseconds()
  // perform timestep
  uint32_t elapsed_us;
  uint32_t us;
  do {
    us = microseconds();
    // detect rollover of microsecond counter
    if (us < m_prevTime) {
      // calculate wrapped around difference
      elapsed_us = (uint32_t)((UINT32_MAX - m_prevTime) + us);
    } else {
      // otherwise calculate regular difference
      elapsed_us = (uint32_t)(us - m_prevTime);
    }
    // if building anywhere except visual studio then we can run alternate sleep code
    // because in visual studio + windows it's better to just spin and check the high
    // resolution clock instead of trying to sleep for microseconds.
    // 1000us per ms, divided by tickrate gives
    // the number of microseconds per tick
  } while (elapsed_us < (1000000 / TICKRATE));

  // store current time
  m_prevTime = microseconds();
}

uint32_t Time::microseconds()
{
#ifdef HELIOS_CLI
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  uint64_t us = SEC_TO_US((uint64_t)ts.tv_sec) + NS_TO_US((uint64_t)ts.tv_nsec);
  return (unsigned long)us;
#else
  // TODO: microseconds on attiny85
  return 0;
#endif
}

#ifdef VORTEX_EMBEDDED
__attribute__ ((noinline))
#endif
void Time::delayMicroseconds(uint32_t us)
{
#ifdef VORTEX_EMBEDDED
#if F_CPU >= 20000000L
  // for a one-microsecond delay, burn 4 clocks and then return
  __asm__ __volatile__ (
    "rjmp .+0" "\n\t"     // 2 cycles
    "nop" );              // 1 cycle
                          // wait 3 cycles with 2 words
  if (us <= 1) return; //  = 3 cycles, (4 when true)
  // the loop takes a 1/2 of a microsecond (10 cycles) per iteration
  // so execute it twice for each microsecond of delay requested.
  us = us << 1; // x2 us, = 2 cycles
  // we just burned 21 (23) cycles above, remove 2
  // us is at least 4 so we can subtract 2.
  us -= 2; // 2 cycles
#elif F_CPU >= 10000000L
  // for a 1 microsecond delay, simply return.  the overhead
  // of the function call takes 14 (16) cycles, which is 1.5us
  if (us <= 2) return; //  = 3 cycles, (4 when true)
  // we just burned 20 (22) cycles above, remove 4, (5*4=20)
  // us is at least 6 so we can subtract 4
  us -= 4; // 2 cycles
#endif
  __asm__ __volatile__(
    "1: sbiw %0, 1" "\n\t"            // 2 cycles
    "rjmp .+0"      "\n\t"            // 2 cycles
    "rjmp .+0"      "\n\t"            // 2 cycles
    "rjmp .+0"      "\n\t"            // 2 cycles
    "brne 1b" : "=w" (us) : "0" (us)  // 2 cycles
  );
  // return = 4 cycles
#else
  uint32_t newtime = microseconds() + us;
  while (microseconds() < newtime) {
    // busy loop
  }
#endif
}

void Time::delayMilliseconds(uint32_t ms)
{
#ifdef HELIOS_CLI
  usleep(ms * 1000);
#else
  // not very accurate
  for (uint16_t i = 0; i < ms; ++i) {
    delayMicroseconds(1000);
  }
#endif
}
