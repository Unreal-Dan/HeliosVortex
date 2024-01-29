#ifndef STORAGE_H
#define STORAGE_H

#include <inttypes.h>
#include "HeliosConfig.h"

// Storage Config Indexes
#define STORAGE_GLOBAL_FLAG_INDEX 0
#define STORAGE_CURRENT_MODE_INDEX 1
#define STORAGE_BRIGHTNESS_INDEX 2

class Pattern;

class Storage
{
public:

  static bool init();

  static bool read_pattern(uint8_t slot, Pattern &pat);
  static void write_pattern(uint8_t slot, const Pattern &pat);

  static void swap_pattern(uint8_t slot1, uint8_t slot2);

  static uint8_t read_config(uint8_t index);
  static void write_config(uint8_t index, uint8_t val);

  static uint8_t read_global_flags() { return read_config(STORAGE_GLOBAL_FLAG_INDEX); }
  static void write_global_flags(uint8_t global_flags) { write_config(STORAGE_GLOBAL_FLAG_INDEX, global_flags); }

  static uint8_t read_current_mode() { return read_config(STORAGE_CURRENT_MODE_INDEX); }
  static void write_current_mode(uint8_t current_mode) { write_config(STORAGE_CURRENT_MODE_INDEX, current_mode); }

  static uint8_t read_brightness() { return read_config(STORAGE_BRIGHTNESS_INDEX); }
  static void write_brightness(uint8_t brightness) { write_config(STORAGE_BRIGHTNESS_INDEX, brightness); }

#ifdef HELIOS_CLI
  // toggle storage on/off
  static void enableStorage(bool enabled) { m_enableStorage = enabled; }
#endif
private:
  static uint8_t crc8(uint8_t pos, uint8_t size);
  static uint8_t crc_pos(uint8_t pos);
  static uint8_t read_crc(uint8_t pos);
  static bool check_crc(uint8_t pos);
  static void write_crc(uint8_t pos);
  static void write_byte(uint8_t address, uint8_t data);
  static uint8_t read_byte(uint8_t address);

#ifdef HELIOS_CLI
  // whether storage is enabled
  static bool m_enableStorage;
#endif
};

#endif
