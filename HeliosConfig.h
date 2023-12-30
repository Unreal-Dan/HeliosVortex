#ifndef HELIOS_CONFIG_H
#define HELIOS_CONFIG_H

// Short Click Threshold
//
// The length of time in milliseconds for a click to
// be considered either a short or long click
#define SHORT_CLICK_THRESHOLD 400

// Selection Flash Duration
//
// How long the led flashes when selecting something
#define SELECTION_FLASH_DURATION 250

// Max Color Slots
//
// The number of slots in a colorset
#define NUM_COLOR_SLOTS 6

// Mode Slots
//
// The number of modes on the device
#define NUM_MODE_SLOTS 9

// Default Brightness
//
// The default brightness of the led
#define DEFAULT_BRIGHTNESS 255

// Tickrate
//
// The number of engine ticks per second
#define TICKRATE 1000

// Menu Hold Time
//
// How long the button must be held for the menus to open and cycle
// note this is a measure of ticks, but if the tickrate is 1000 then
// it is a measure of milliseconds
#define MENU_HOLD_TIME 1000

// Force Sleep Time
//
// The duration in ms/ticks to hold the button to force the chip to
// sleep at any location in the menus
#define FORCE_SLEEP_TIME 5000

// #define HELIOS_ARDUINO 1

#endif
