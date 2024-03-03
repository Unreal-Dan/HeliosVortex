#include "Helios.h"
#include "Led.h"

#if !defined(HELIOS_CLI) && !defined(HELIOS_ARDUINO)
// this is the main thread for non-arduino embedded builds
int main(int argc, char *argv[])
{
  Helios::init();
  return 0;
}
#endif
