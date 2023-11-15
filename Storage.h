#ifndef STORAGE_H
#define STORAGE_H

#include <inttypes.h>
#include "HeliosConfig.h"

class Pattern;

class Storage
{
public:

  static bool init();

  static bool read(uint8_t slot, Pattern &pat);
  static bool write(uint8_t slot, const Pattern &pat);

private:
#ifdef HELIOS_EMBEDDED
  static void write_byte(uint8_t address, uint8_t data);
  static uint8_t read_byte(uint8_t address);
#endif

};

#endif
