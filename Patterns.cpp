#include "Patterns.h"

#include "Pattern.h"
#include "Storage.h"

// define arrays of colors, you can reuse these if you have multiple
// modes that use the same colorset -- these demonstrate the max amount
// of colors in each set but you can absolutely list a lesser amount
static const uint32_t color_codes0[] = { HELIOS_RGB_RED, HELIOS_RGB_ORANGE, HELIOS_RGB_YELLOW, HELIOS_RGB_TURQUOISE, HELIOS_RGB_BLUE, HELIOS_RGB_PINK };
static const uint32_t color_codes1[] = { HELIOS_RGB_RED, HELIOS_RGB_CORAL_ORANGE_SAT_MEDIUM, HELIOS_RGB_ORANGE, HELIOS_RGB_YELLOW_SAT_LOW };
static const uint32_t color_codes2[] = { HELIOS_RGB_PURPLE_SAT_MEDIUM, HELIOS_RGB_RED_BRI_LOWEST, HELIOS_RGB_MAGENTA_BRI_LOWEST, HELIOS_RGB_BLUE_BRI_LOWEST };
static const uint32_t color_codes3[] = { HELIOS_RGB_MAGENTA, HELIOS_RGB_YELLOW, HELIOS_RGB_TURQUOISE, HELIOS_RGB_PINK_SAT_LOW, HELIOS_RGB_RED, HELIOS_RGB_YELLOW };
static const uint32_t color_codes4[] = { HELIOS_RGB_WHITE_BRI_LOWEST, HELIOS_RGB_ROYAL_BLUE_BRI_LOW, HELIOS_RGB_TURQUOISE, HELIOS_RGB_ROYAL_BLUE_BRI_LOW, HELIOS_RGB_MAGENTA_BRI_LOWEST };
static const uint32_t color_codes5[] = { HELIOS_RGB_RED, HELIOS_RGB_HOT_PINK, HELIOS_RGB_ROYAL_BLUE, HELIOS_RGB_BLUE, HELIOS_RGB_GREEN, HELIOS_RGB_YELLOW };

// Define Colorset configurations for each slot
struct default_colorset {
  uint8_t num_cols;
  const uint32_t *cols;
};

// the array of colorset entries, make sure the number on the left reflects
// the number of colors in the array on the right
static const default_colorset default_colorsets[] = {
    {6, color_codes0},  // 0 Lightside
    {4, color_codes1},  // 1 Sauna
    {4, color_codes2},  // 2 UltraViolet
    {6, color_codes3},  // 3 Space Carnival
    {5, color_codes4},  // 4 Ice Blade
    {6, color_codes5},  // 5 Rainbow Glitter
};

void Patterns::make_default(uint8_t index, Pattern &pat) {
  if (index >= NUM_MODE_SLOTS) {
    return;
  }
  PatternArgs args;
  switch (index) {
    case 0: // Lightside
      args.on_dur = 1;
      args.gap_dur = 40;
      break;
    case 1: // Sauna
      args.on_dur = 1;
      args.off_dur = 9;
      break;
    case 2: // UltraViolet
      args.on_dur = 9;
      break;
    case 3: // Space Carnival
      args.on_dur = 3;
      args.off_dur = 23;
      break;
    case 4: // Ice Blade
      args.on_dur = 3;
      args.off_dur = 1;
      break;
    case 5: // Rainbow Glitter
      args.on_dur = 1;
      args.off_dur = 50;
      break;
  }
  // assign default args
  pat.setArgs(args);
  // build the set out of the defaults
  Colorset set(default_colorsets[index].num_cols, default_colorsets[index].cols);
  // assign default colorset
  pat.setColorset(set);
}

void Patterns::make_pattern(PatternID id, Pattern &pat) {
  PatternArgs args;
  switch (id) {
    default:
    case PATTERN_STROBEGAP:
      args.gap_dur = 25;
    case PATTERN_STROBE:
      args.on_dur = 5;
      args.off_dur = 8;
      break;
    case PATTERN_FLARE:
      args.on_dur = 1;
      args.off_dur = 30;
      break;
    case PATTERN_GLOW:
      args.on_dur = 1;
      args.off_dur = 0;
      args.gap_dur = 40;
      break;
    case PATTERN_FLICKER:
      args.on_dur = 1;
      args.off_dur = 50;
      break;
    case INOVA_BLINK:
      args.on_dur = 10;
      args.off_dur = 250;
      break;
    case PATTERN_HYPERGAP:
      args.gap_dur = 218;
    case PATTERN_HYPERSTROBE:
      args.on_dur = 16;
      args.off_dur = 20;
      break;
    case PATTERN_ULTRA_DOPS:
      args.on_dur = 3;
      args.off_dur = 1;
      break;
    case PATTERN_STROBIEGAP:
      args.gap_dur = 100;
    case PATTERN_STROBIE:
      args.on_dur = 3;
      args.off_dur = 23;
      break;
    case PATTERN_DOPSGAP:
      args.gap_dur = 40;
    case PATTERN_DOPS:
      args.on_dur = 1;
      args.off_dur = 9;
      break;
    case PATTERN_BLINKIE:
      args.on_dur = 3;
      args.off_dur = 1;
      args.gap_dur = 150;
      break;
    case PATTERN_GHOSTCRUSH:
      args.on_dur = 3;
      args.off_dur = 1;
      args.gap_dur = 18;
      break;
    case PATTERN_DOUBLEDOPS:
      args.on_dur = 1;
      args.off_dur = 1;
      args.gap_dur = 10;
      args.group_size = 2;
      break;
    case PATTERN_CHOPPER:
      args.on_dur = 1;
      args.off_dur = 1;
      args.gap_dur = 10;
      args.group_size = 2;
      break;
    case PATTERN_DASHGAP:
      args.on_dur = 1;
      args.off_dur = 1;
      args.gap_dur = 20;
      args.dash_dur = 20;
      break;
    case PATTERN_DASHDOPS:
      args.on_dur = 1;
      args.off_dur = 10;
      args.gap_dur = 10;
      args.dash_dur = 18;
      break;
    case PATTERN_DASHCRUSH:
      args.on_dur = 4;
      args.off_dur = 1;
      args.gap_dur = 10;
      args.dash_dur = 18;
      break;
    case PATTERN_ULTRADASH:
      args.on_dur = 1;
      args.off_dur = 3;
      args.gap_dur = 3;
      args.dash_dur = 14;
      break;
    case PATTERN_GAPCYCLE:
      args.on_dur = 2;
      args.off_dur = 6;
      args.gap_dur = 12;
      args.dash_dur = 25;
      args.group_size = 2;
      break;
    case PATTERN_DASHCYCLE:
      args.on_dur = 1;
      args.off_dur = 3;
      args.gap_dur = 3;
      args.dash_dur = 30;
      args.group_size = 2;
      break;
    case PATTERN_TRACER:
      args.on_dur = 3;
      args.dash_dur = 20;
      args.group_size = 1;
      break;
    case PATTERN_RIBBON:
      args.on_dur = 9;
      break;
    case PATTERN_MINIRIBBON:
      args.on_dur = 1;
      break;
    case PATTERN_COMPLEMENTARY_BLEND:
      args.num_flips = 1;
    case PATTERN_BLEND:
      args.on_dur = 2;
      args.off_dur = 13;
      args.blend_speed = 5;
      break;
    case PATTERN_COMPLEMENTARY_BLENDSTROBE:
      args.num_flips = 1;
    case PATTERN_BLEND_STROBE:
      args.on_dur = 5;
      args.off_dur = 8;
      args.blend_speed = 10;
      break;
    case PATTERN_BLEND_STROBIE:
      args.on_dur = 3;
      args.off_dur = 23;
      args.blend_speed = 10;
      break;
    case PATTERN_COMPLEMENTARY_BLENDSTROBEGAP:
      args.num_flips = 1;
    case PATTERN_BLENDSTROBEGAP:
      args.on_dur = 6;
      args.off_dur = 6;
      args.gap_dur = 25;
      args.blend_speed = 10;
      break;
  }
  pat.setArgs(args);
}
