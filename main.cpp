#include "Helios.h"

#ifndef HELIOS_ARDUINO
int main(int argc, char *argv[]) {
  Helios::init();
  while (Helios::keep_going()) {
    Helios::tick();
  }
  return 0;
}
#endif
