#include "Helios.h"

#ifdef VORTEX_EMBEDDED
int main(int argc, char *argv[])
{
  Helios::init();
  while (Helios::keepGoing()) {
    Helios::tick();
  }
  return 0;
}
#endif
