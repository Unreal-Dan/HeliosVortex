#include "Helios.h"

#if !defined(HELIOS_CLI) && !defined(HELIOS_ARDUINO)
// this is the main thread for non-arduino embedded builds
int main(int argc, char *argv[])
{
  Helios::init();
  while (Helios::keep_going()) {
    //Helios::tick();
    // todo: sleep here
  }
  return 0;
}
#endif
