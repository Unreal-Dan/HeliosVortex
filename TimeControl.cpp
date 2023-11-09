#include "TimeControl.h"

#include <math.h>

#include "Timings.h"

#include "Led.h"

#ifdef VORTEX_EMBEDDED
#include <avr/sleep.h>
#include <avr/interrupt.h>
#endif

#if !defined(_WIN32) || defined(WASM)
#include <unistd.h>
#include <time.h>
uint64_t start = 0;
// convert seconds and nanoseconds to microseconds
#define SEC_TO_US(sec) ((sec)*1000000)
#define NS_TO_US(ns) ((ns)/1000)
#else
#include <Windows.h>
static LARGE_INTEGER tps;
static LARGE_INTEGER start;
#endif

// static members
uint32_t Time::m_curTick = 0;
// the last frame timestamp
uint32_t Time::m_prevTime = 0;

bool Time::init()
{
#ifdef VORTEX_EMBEDDED
  // initialize main clock
#if (F_CPU == 20000000)
  // No division on clock
  _PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, 0x00);
#elif (F_CPU == 10000000)
  // 20MHz prescaled by 2, Clock DIV2
  _PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, (CLKCTRL_PEN_bm | CLKCTRL_PDIV_2X_gc));
#else
  #error "F_CPU not supported"
#endif
  // IVSEL = 1 means Interrupt vectors are placed at the start of the boot section of the Flash
  // as opposed to the application section of Flash. See 13.5.1
  _PROTECTED_WRITE(CPUINT_CTRLA, CPUINT_IVSEL_bm);
#endif
  m_prevTime = microseconds();
  m_curTick = 0;
#if VARIABLE_TICKRATE == 1
  m_tickrate = TICKRATE;
#endif
#if !defined(_WIN32) || defined(WASM)
  start = microseconds();
#else
  QueryPerformanceFrequency(&tps);
  QueryPerformanceCounter(&start);
#endif
  return true;
}

void Time::cleanup()
{
}

void Time::tickClock()
{
  // tick clock forward
  m_curTick++;

#if DEBUG_ALLOCATIONS == 1
  if ((m_curTick % MS_TO_TICKS(1000)) == 0) {
    DEBUG_LOGF("Cur Memory: %u (%u)", cur_memory_usage(), cur_memory_usage_background());
  }
#endif

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

// the real current time, bypass simulations, used by timers
uint32_t Time::getRealCurtime()
{
  return m_curTick;
}

uint32_t Time::getTickrate()
{
  return TICKRATE;
}

#if VARIABLE_TICKRATE == 1
uint32_t Time::millisecondsToTicks(uint32_t ms)
{
  // 0ms = 0 ticks
  if (!ms) {
    return 0;
  }
  // but anything > 0 ms must be no less than 1 tick
  // otherwise short durations will disappear at low
  // tickrates
  uint32_t ticks = (ms * TICKRATE) / 1000;
  if (!ticks) {
    return 1;
  }
  return ticks;
}
#endif

uint32_t Time::microseconds()
{
  struct timespec ts;
  clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
  uint64_t us = SEC_TO_US((uint64_t)ts.tv_sec) + NS_TO_US((uint64_t)ts.tv_nsec);
  return (unsigned long)us;
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
#elif defined(_WIN32)
  uint32_t newtime = microseconds() + us;
  while (microseconds() < newtime) {
    // busy loop
  }
#else
  usleep(us);
#endif
}

void Time::delayMilliseconds(uint32_t ms)
{
#ifdef VORTEX_EMBEDDED
  // not very accurate
  for (uint16_t i = 0; i < ms; ++i) {
    delayMicroseconds(1000);
  }
#elif defined(_WIN32)
  Sleep(ms);
#else
  usleep(ms * 1000);
#endif
}
