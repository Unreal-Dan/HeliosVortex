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

// Slot size is the size of the colorset + 7 params + pat flags
#define SLOT_SIZE ((sizeof(Colorset) + 8))

bool Storage::init()
{
#ifdef HELIOS_CLI
  unlink(STORAGE_FILENAME);
#endif
  return true;
}

bool Storage::read_pattern(uint8_t slot, Pattern &pat)
{
#ifdef HELIOS_EMBEDDED
  for (uint8_t i = 0; i < SLOT_SIZE; ++i) {
    ((uint8_t *)&pat)[i] = read_byte((slot * SLOT_SIZE) + i);
  }
#elif defined(HELIOS_CLI)
  FILE *f = fopen(STORAGE_FILENAME, "r");
  if (!f) {
    return false;
  }
  long offset = (slot * SLOT_SIZE);
  fseek(f, offset, SEEK_SET);
  if (!fread((void *)&pat, sizeof(char), SLOT_SIZE, f)) {
    return false;
  }
  fclose(f);
#endif
  return true;
}

bool Storage::write_pattern(uint8_t slot, const Pattern &pat)
{
#ifdef HELIOS_EMBEDDED
  for (uint8_t i = 0; i < SLOT_SIZE; ++i) {
    uint8_t val = ((uint8_t *)&pat)[i];
    uint8_t target = (slot * SLOT_SIZE) + i;
    if (val != read_byte(target)) {
      write_byte(target, val);
    }
  }
#elif defined(HELIOS_CLI)
  FILE *f = fopen(STORAGE_FILENAME, "a");
  if (!f) {
    return false;
  }
  long offset = (slot * SLOT_SIZE);
  fseek(f, offset, SEEK_SET);
  if (!fwrite((void *)&pat, sizeof(char), SLOT_SIZE, f)) {
    return false;
  }
  fclose(f);
#endif
  return true;
}


bool Storage::read_config(uint8_t index, uint8_t &val)
{
#ifdef HELIOS_EMBEDDED
  val = read_byte(255 - index);
#elif defined(HELIOS_CLI)
  FILE *f = fopen(STORAGE_FILENAME, "r");
  if (!f) {
    return false;
  }
  long offset = 255 - index;
  fseek(f, offset, SEEK_SET);
  if (!fread((void *)&val, sizeof(char), 1, f)) {
    return false;
  }
  fclose(f);
#endif
  return true;
}

bool Storage::write_config(uint8_t index, uint8_t val)
{
#ifdef HELIOS_EMBEDDED
  write_byte(255 - index, val);
#elif defined(HELIOS_CLI)
  FILE *f = fopen(STORAGE_FILENAME, "a");
  if (!f) {
    return false;
  }
  long offset = 255 - index;
  fseek(f, offset, SEEK_SET);
  if (!fwrite((void *)&val, sizeof(char), 1, f)) {
    return false;
  }
  fclose(f);
#endif
  return true;
}

#ifdef HELIOS_EMBEDDED
void Storage::write_byte(uint8_t address, uint8_t data)
{
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
}

uint8_t Storage::read_byte(uint8_t address)
{
  /* Wait for completion of previous write */
  while(EECR & (1<<EEPE))
    ;
  /* Set up address register */
  EEAR = address;
  /* Start eeprom read by writing EERE */
  EECR |= (1<<EERE);
  /* Return data from data register */
  return EEDR;
}
#endif
