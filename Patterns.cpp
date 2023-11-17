#include "Patterns.h"

#include "Storage.h"
#include "Pattern.h"

void Patterns::make_default(uint8_t index, Pattern &pat)
{
  if (index > 5) {
    return;
  }
  //Storage::read_pattern(index + 6, pat);
  const PatternArgs default_args[6] = {
    {1},
    {3, 4},
    {15, 5, 4},
    {15, 5, 4, 3},
    {15, 5, 4, 3, 2},
    {15, 5, 4, 3, 2, 1},
  };
  Colorset default_colorset(RGB_RED, RGB_GREEN, RGB_BLUE);
  pat.setArgs(default_args[index]);
  pat.setColorset(default_colorset);
}

void Patterns::make_pattern(PatternID id, Pattern &pat)
{
  PatternArgs args;
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
