#include "Helios.h"
#include "Led.h"

#if !defined(HELIOS_CLI) && !defined(HELIOS_ARDUINO)
// this is the main thread for non-arduino embedded builds
int main(int argc, char *argv[])
{
  Helios::init();
  while (Helios::keep_going()) {
    // continuously update the led at 8mhz cpu speed while the
    // engine ticks 1000khz in the interrupt handler
    // Led::update();
  }
  return 0;
}
#endif
