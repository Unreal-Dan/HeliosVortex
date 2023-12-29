#include "Patterns.h"

#include "Storage.h"
#include "Pattern.h"

// the parameters of the 9 defaults
static const PatternArgs default_args[NUM_MODE_SLOTS] = {
  { 2, 13, 0, 0, 0, 1, 0 },   // 0
  { 6, 50, 0, 0, 0, 0, 0 },   // 1
  { 25, 13, 0, 0, 0, 1, 0 },  // 2
  { 6, 13, 0, 0, 0, 1, 1 },   // 3
  { 5, 5, 200, 0, 4, 0, 0 },  // 4
  { 1, 15, 0, 50, 0, 0, 0 },  // 5
  { 20, 20, 0, 0, 0, 0, 0 },  // 6
  { 4, 5, 150, 0, 8, 0, 0 },  // 7
  { 12, 10, 0, 0, 3, 0, 0 },  // 8
};

// define arrays of colors, you can reuse these if you have multiple
// modes that use the same colorset -- these demonstrate the max amount
// of colors in each set but you can absolutely list a lesser amount
static const uint32_t color_codes0[] = { RGB_RED, RGB_GREEN, RGB_BLUE, RGB_YELLOW, RGB_PURPLE, RGB_CYAN };
static const uint32_t color_codes1[] = { RGB_ORANGE, RGB_PINK, RGB_WHITE, RGB_BLUE, RGB_YELLOW, RGB_RED };
static const uint32_t color_codes2[] = { RGB_GREEN, RGB_CYAN, RGB_PURPLE, RGB_RED3, RGB_BLUE3, RGB_GREEN3 };
static const uint32_t color_codes3[] = { RGB_YELLOW3, RGB_PURPLE3, RGB_CYAN3, RGB_ORANGE, RGB_WHITE, RGB_OFF };
static const uint32_t color_codes4[] = { RGB_WHITE4, RGB_BLUE4, RGB_YELLOW4, RGB_RED4, RGB_GREEN4, RGB_CYAN4 };
static const uint32_t color_codes5[] = { RGB_PURPLE4, RGB_ORANGE4, RGB_PINK, RGB_MAGENTA, RGB_CYAN, RGB_PINK };
static const uint32_t color_codes6[] = { RGB_WHITE6, RGB_BLUE6, RGB_YELLOW6, RGB_RED6, RGB_GREEN6, RGB_CYAN6 };
static const uint32_t color_codes7[] = { RGB_PURPLE6, RGB_ORANGE6, RGB_GREEN, RGB_CYAN, RGB_BLUE, RGB_PURPLE };
static const uint32_t color_codes8[] = { RGB_WHITE9, RGB_BLUE9, RGB_YELLOW9, RGB_RED9, RGB_GREEN9, RGB_CYAN9 };

// Define Colorset configurations for each slot
struct default_colorset {
  uint8_t num_cols;
  const uint32_t *cols;
};

// the array of colorset entries, make sure the number on the left reflects
// the number of colors in the array on the right
static const default_colorset default_colorsets[] = {
  { 6, color_codes0 }, // 0
  { 6, color_codes1 }, // 1
  { 6, color_codes2 }, // 2
  { 6, color_codes3 }, // 3
  { 6, color_codes4 }, // 4
  { 6, color_codes5 }, // 5
  { 6, color_codes6 }, // 6
  { 6, color_codes7 }, // 7
  { 6, color_codes8 }  // 8
};

void Patterns::make_default(uint8_t index, Pattern &pat)
{
  if (index > 8) {
    return;
  }
  // assign default args
  pat.setArgs(default_args[index]);
  // assign default colorset
  pat.setColorset(Colorset(default_colorsets[index].num_cols, default_colorsets[index].cols));
}

void Patterns::make_pattern(PatternID id, Pattern &pat)
{
  PatternArgs args = { 0, 0, 0, 0, 0, 0, 0 };
  switch (id) {
  default:
  case PATTERN_STROBEGAP:
    args.gap_dur = 25;
  case PATTERN_STROBE:
    args.on_dur = 6;
    args.off_dur = 6;
    break;
  case PATTERN_HYPERGAP:
    args.gap_dur = 218;
  case PATTERN_HYPERSTROBE:
    args.on_dur = 16;
    args.off_dur = 20;
    break;
  case PATTERN_PICOGAP:
    args.gap_dur = 90;
  case PATTERN_PICOSTROBE:
    args.on_dur = 6;
    args.off_dur = 40;
    break;
  case PATTERN_STROBIEGAP:
    args.gap_dur = 100;
  case PATTERN_STROBIE:
    args.on_dur = 2;
    args.off_dur = 28;
    break;
  case PATTERN_DOPSGAP:
    args.gap_dur = 40;
  case PATTERN_DOPS:
    args.on_dur = 1;
    args.off_dur = 10;
    break;
  case PATTERN_ULTRAGAP:
    args.gap_dur = 55;
  case PATTERN_ULTRADOPS:
    args.on_dur = 1;
    args.off_dur = 3;
    break;
  case PATTERN_BLINKIE:
    args.on_dur = 3;
    args.off_dur = 1;
    args.gap_dur = 65;
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
    args.on_dur = 6;
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
  case PATTERN_BLENDSTROBE:
    args.on_dur = 6;
    args.off_dur = 6;
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
