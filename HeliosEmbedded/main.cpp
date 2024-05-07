#include "Helios.h"
#include "Led.h"

#include <avr/sleep.h>

#if !defined(HELIOS_CLI) && !defined(HELIOS_ARDUINO)
// this is the main thread for non-arduino embedded builds
int main(int argc, char *argv[])
{
  Helios::init();
  // the main thread just initializes Helios then either spins or sleeps while
  // the interrupt handler that Helios::init() registered calls Helios::tick()
  while (Helios::keep_going()) {
    // just sleep here
    // TODO: test whether this is actually necessary or makes any difference
    //       I don't know if the while-loop is actually necessary or not if
    //       we end up sleeping here, and if we do sleep here, I don't know which
    //       mode of sleep would produce the best power draw - or whether or not
    //       that sleep call needs to be inside a while-loop or we can just call
    //       it once? Will need to try various different things here and see if it
    //       effects power draw.
    set_sleep_mode(SLEEP_MODE_IDLE);
    sleep_mode();
  }
  return 0;
}
#endif
