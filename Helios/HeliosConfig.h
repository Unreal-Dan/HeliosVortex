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
#define TIME_TILL_LONG_CLICK_FLASH 1000

// Long Click Threshold
//
// The maximum length a long click can be
#define LONG_CLICK_THRESHOLD (SHORT_CLICK_THRESHOLD + TIME_TILL_LONG_CLICK_FLASH)
// Max Color Slots
//
// The number of slots in a colorset
#define NUM_COLOR_SLOTS 6

// Mode Slots
//
// The number of modes on the device
#define NUM_MODE_SLOTS 6

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
#define FORCE_SLEEP_TIME 7000

// Delete Color Time
//
// How long to hold button on a color to start the delete color flash
// begins and starts flashes. Also how long the cycling flash is for the
// delete color selection, ie how long the user has to release to delete
// the color before it cycles back
#define DELETE_COLOR_TIME 1500

// Alternative HSV to RGB
//
// This enabled the alternative HSV to RGB algorithm to be used in the
// color selection menu and provide a slightly different range of colors
#define ALTERNATIVE_HSV_RGB 0

// Brightness Options
//
// There are three brightness options, the lowest is equal to this value,
// the middle is 2x this value, and the highest is 3x this value
#define BRIGHTNESS_STEP   85

// Pre-defined saturation values
#define HSV_SAT_HIGH      255
#define HSV_SAT_MEDIUM    220
#define HSV_SAT_LOW       180
#define HSV_SAT_LOWEST    120

// Pre-defined brightness values
#define HSV_VAL_HIGH      255
#define HSV_VAL_MEDIUM    120
#define HSV_VAL_LOW       60
#define HSV_VAL_LOWEST    10

// ============================================================================
//  Storage Constants
//
//  These are various storage sizes of data and some math to
//  help calculate sizes or space requirements

// Colorset Size
//
// the colorset is just an array of colors but it also has a num colors val
#define COLORSET_SIZE ((sizeof(RGBColor) * NUM_COLOR_SLOTS) + 1)

// Pattern Size
//
// The actual pattern storage size is the size of the colorset + 7 params + 1 pat flags
#define PATTERN_SIZE (COLORSET_SIZE + 7 + 1)

// Slot Size
//
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

// forbidden constant:
// #define HELIOS_ARDUINO 1



#endif
