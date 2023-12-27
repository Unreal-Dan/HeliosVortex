#ifndef STORAGE_H
#define STORAGE_H

#include <inttypes.h>
#include "HeliosConfig.h"

class Pattern;

class Storage
{
public:

  static bool init();

  static bool read_pattern(uint8_t slot, Pattern &pat);
  static bool write_pattern(uint8_t slot, const Pattern &pat);

  static bool read_config(uint8_t index, uint8_t &val);
  static bool write_config(uint8_t index, uint8_t val);

private:
  static uint8_t crc8(uint8_t pos, uint8_t size);
  static uint8_t crc_pos(uint8_t pos);
  static bool read_crc(uint8_t pos);
  static bool check_crc(uint8_t pos);
  static void write_crc(uint8_t pos);
  static void write_byte(uint8_t address, uint8_t data);
  static uint8_t read_byte(uint8_t address);
};

#endif