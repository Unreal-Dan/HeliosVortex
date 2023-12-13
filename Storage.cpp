#include "Storage.h"

#include "Colorset.h"

#ifdef HELIOS_EMBEDDED
#include <avr/io.h>
#endif

#ifdef HELIOS_CLI
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#define STORAGE_FILENAME "Helios.storage"
#endif

// the colorset is just an array of colors but it also has a num colors val
#define COLORSET_SIZE ((sizeof(RGBColor) * MAX_COLOR_SLOTS) + 1)
// The actual pattern storage size is the size of the colorset + 7 params + 1 pat flags
#define PATTERN_SIZE (COLORSET_SIZE + 7 + 1)
// the slot stores the pattern + 1 byte CRC
#define SLOT_SIZE (PATTERN_SIZE + 1)
// Some math to calculate storage sizes:
// 3 * 6 = 18 for the colorset
// 1 + 7 + 1 + 1 = 10 for the rest
//  = 28 bytes total for a pattern including CRC
//    -> 8 slots = 8 * 28 = 224
//      = 31 bytes left
//    -> 9 slots = 9 * 28 = 252
//      = 3 bytes left


bool Storage::init()
{
#ifdef HELIOS_CLI
  unlink(STORAGE_FILENAME);
#endif
  return true;
}

bool Storage::read_pattern(uint8_t slot, Pattern &pat)
{
  uint8_t pos = slot * SLOT_SIZE;
  if (!check_crc(pos)) {
    return false;
  }
  for (uint8_t i = 0; i < PATTERN_SIZE; ++i) {
    ((uint8_t *)&pat)[i] = read_byte(pos + i);
  }
  return true;
}

bool Storage::write_pattern(uint8_t slot, const Pattern &pat)
{
  uint8_t pos = slot * SLOT_SIZE;
  for (uint8_t i = 0; i < PATTERN_SIZE; ++i) {
    uint8_t val = ((uint8_t *)&pat)[i];
    uint8_t target = pos + i;
    if (val != read_byte(target)) {
      write_byte(target, val);
    }
  }
  write_crc(pos);
  return true;
}

bool Storage::read_config(uint8_t index, uint8_t &val)
{
  val = read_byte(255 - index);
  return true;
}

bool Storage::write_config(uint8_t index, uint8_t val)
{
  write_byte(255 - index, val);
  return true;
}

uint8_t Storage::crc8(uint8_t pos, uint8_t size)
{
  uint8_t hash = 33;  // A non-zero initial value
  for (uint8_t i = 0; i < size; ++i) {
    hash = ((hash << 5) + hash) + read_byte(pos);
  }
  return hash;
}

uint8_t Storage::crc_pos(uint8_t pos)
{
  // crc the entire slot except last byte
  return crc8(pos, PATTERN_SIZE);
}

bool Storage::read_crc(uint8_t pos)
{
  // read the last byte of the slot
  return read_byte(pos + PATTERN_SIZE);
}

bool Storage::check_crc(uint8_t pos)
{
  // compare the last byte to the calculated crc
  return read_crc(pos) == crc_pos(pos);
}

void Storage::write_crc(uint8_t pos)
{
  // compare the last byte to the calculated crc
  write_byte(pos + PATTERN_SIZE, crc_pos(pos));
}

void Storage::write_byte(uint8_t address, uint8_t data)
{
#ifdef HELIOS_EMBEDDED
  /* Wait for completion of previous write */
  while(EECR & (1<<EEPE))
    ;
  /* Set Programming mode */
  EECR = (0<<EEPM1)|(0<<EEPM0);
  /* Set up address and data registers */
  EEAR = address;
  EEDR = data;
  /* Write logical one to EEMPE */
  EECR |= (1<<EEMPE);
  /* Start eeprom write by setting EEPE */
  EECR |= (1<<EEPE);
#else // HELIOS_CLI
  FILE *f = fopen(STORAGE_FILENAME, "r+b"); // Open file for reading and writing in binary mode
  if (!f) {
    perror("Error opening file");
    return;
  }
  fseek(f, address, SEEK_SET); // Seek to the specified address
  fwrite(&data, sizeof(uint8_t), 1, f); // Write the byte of data
  fclose(f); // Close the file
#endif
}

uint8_t Storage::read_byte(uint8_t address)
{
#ifdef HELIOS_EMBEDDED
  /* Wait for completion of previous write */
  while(EECR & (1<<EEPE))
    ;
  /* Set up address register */
  EEAR = address;
  /* Start eeprom read by writing EERE */
  EECR |= (1<<EERE);
  /* Return data from data register */
  return EEDR;
#else
  uint8_t val = 0;
  FILE *f = fopen(STORAGE_FILENAME, "rb"); // Open file for reading in binary mode
  if (!f) {
    perror("Error opening file");
    return val;
  }
  fseek(f, address, SEEK_SET); // Seek to the specified address
  fread(&val, sizeof(uint8_t), 1, f); // Read a byte of data
  fclose(f); // Close the file
  return val;
#endif
}
