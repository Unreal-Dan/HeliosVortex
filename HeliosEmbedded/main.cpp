#include "Helios.h"
#include "Led.h"

#include <avr/sleep.h>

#if !defined(HELIOS_CLI) && !defined(HELIOS_ARDUINO)
// this is the main thread for non-arduino embedded builds
int main(int argc, char *argv[])
{
  Helios::initialize_hardware();
  Helios::init();
  // the main thread just initializes Helios then continuously calls tick
  while (Helios::keep_going()) {
    Helios::tick();
  }
  return 0;
}
#endif
