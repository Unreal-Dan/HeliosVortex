#include "Patterns.h"

#include "Storage.h"
#include "Pattern.h"

// define arrays of colors, you can reuse these if you have multiple
// modes that use the same colorset -- these demonstrate the max amount
// of colors in each set but you can absolutely list a lesser amount
static const uint32_t color_codes0[] = {RGB_RED, RGB_ORANGE, RGB_YELLOW, RGB_TURQUOISE, RGB_BLUE, RGB_PINK};
static const uint32_t color_codes1[] = {RGB_RED, RGB_CORAL_ORANGE_SAT_MEDIUM, RGB_ORANGE, RGB_YELLOW_SAT_LOW};
static const uint32_t color_codes2[] = {RGB_PURPLE_SAT_MEDIUM, RGB_RED_BRI_LOWEST, RGB_MAGENTA_BRI_LOWEST, RGB_BLUE_BRI_LOWEST};
static const uint32_t color_codes3[] = {RGB_BLUE_BRI_LOWEST, RGB_TURQUOISE, RGB_SEAFOAM_BRI_MEDIUM, RGB_ICE_BLUE_BRI_LOW};
static const uint32_t color_codes4[] = {RGB_MAGENTA_BRI_LOWEST, RGB_ROYAL_BLUE_BRI_LOW, RGB_TURQUOISE, RGB_ROYAL_BLUE_BRI_LOW, RGB_MAGENTA_BRI_LOWEST, RGB_OFF};
static const uint32_t color_codes5[] = {RGB_RED, RGB_HOT_PINK, RGB_ROYAL_BLUE, RGB_BLUE, RGB_GREEN, RGB_YELLOW};

// Define Colorset configurations for each slot
struct default_colorset {
  uint8_t num_cols;
  const uint32_t *cols;
};

// the array of colorset entries, make sure the number on the left reflects
// the number of colors in the array on the right
static const default_colorset default_colorsets[] = {
  { 6, color_codes0 },  // 0 Lightside
  { 4, color_codes1 },  // 1 Sauna
  { 4, color_codes2 },  // 2 UltraViolet
  { 4, color_codes3 },  // 3 Freezer Burn
  { 6, color_codes4 },  // 4 Ice Blade
  { 6, color_codes5 },  // 5 Rainbow Glitter
};

void Patterns::make_default(uint8_t index, Pattern &pat) 
{
  if (index >= NUM_MODE_SLOTS) {
    return;
  }
  PatternArgs args;
  switch (index) {
    case 0:  // Lightside
      args.on_dur = 2;
      args.gap_dur = 40;
      break;
    case 1:  // Sauna
      args.on_dur = 1;
      args.off_dur = 9;
      break;
    case 2:  // UltraViolet
      args.on_dur = 9;
      break;
    case 3:  // Freezer Burn
      args.on_dur = 1;
      args.off_dur = 10;
      args.gap_dur = 10;
      args.dash_dur = 18;
      break;
    case 4:  // Ice Blade
      args.on_dur = 3;
      args.off_dur = 1;
      break;
    case 5:  // Rainbow Glitter
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

void Patterns::make_pattern(PatternID id, Pattern &pat) 
{
  PatternArgs args;
  switch (id) {
    default:
    //   case PATTERN_ULTRA_DOPS:
    //     args.on_dur = 1;
    //     args.off_dur = 1;
    //     break;
    // case PATTERN_PARTICLE_DOPS:
    //     args.on_dur = 1;
    //     args.off_dur = 3;
    //     break;
    // case PATTERN_BLIP_DOPS:
    //     args.on_dur = 1;
    //     args.off_dur = 6;
    //     break;
    // case PATTERN_DOPS_DOPS:
    //     args.on_dur = 1;
    //     args.off_dur = 9;
    //     break;
    // case PATTERN_HYPERSTROBE_DOPS:
    //     args.on_dur = 1;
    //     args.off_dur = 17;
    //     break;
    // case PATTERN_STROBIE_DOPS:
    //     args.on_dur = 1;
    //     args.off_dur = 23;
    //     break;
    // case PATTERN_FLARE_DOPS:
    //     args.on_dur = 1;
    //     args.off_dur = 30;
    //     break;
    // case PATTERN_BURST_DOPS:
    //     args.on_dur = 1;
    //     args.off_dur = 40;
    //     break;
    // case PATTERN_FLICKER_DOPS:
    //     args.on_dur = 1;
    //     args.off_dur = 50;
    //     break;
    // case PATTERN_BLINKY_DOPS:
    //     args.on_dur = 1;
    //     args.off_dur = 150;
    //     break;
    // case PATTERN_INOVA_BLINK_DOPS:
    //     args.on_dur = 1;
    //     args.off_dur = 250;
    //     break;



    //   case PATTERN_ULTRA_STROBIE:
    //     args.on_dur = 3;
    //     args.off_dur = 1;
    //     break;
    // case PATTERN_PARTICLE_STROBIE:
    //     args.on_dur = 3;
    //     args.off_dur = 3;
    //     break;
    // case PATTERN_BLIP_STROBIE:
    //     args.on_dur = 3;
    //     args.off_dur = 6;
    //     break;
    // case PATTERN_DOPS_STROBIE:
    //     args.on_dur = 3;
    //     args.off_dur = 9;
    //     break;
    // case PATTERN_HYPERSTROBE_STROBIE:
    //     args.on_dur = 3;
    //     args.off_dur = 17;
    //     break;
    // case PATTERN_STROBIE_STROBIE:
    //     args.on_dur = 3;
    //     args.off_dur = 23;
    //     break;
    // case PATTERN_FLARE_STROBIE:
    //     args.on_dur = 3;
    //     args.off_dur = 30;
    //     break;
    // case PATTERN_BURST_STROBIE:
    //     args.on_dur = 3;
    //     args.off_dur = 40;
    //     break;
    // case PATTERN_FLICKER_STROBIE:
    //     args.on_dur = 3;
    //     args.off_dur = 50;
    //     break;
    // case PATTERN_BLINKY_STROBIE:
    //     args.on_dur = 3;
    //     args.off_dur = 150;
    //     break;
    // case PATTERN_INOVA_BLINK_STROBIE:
    //     args.on_dur = 3;
    //     args.off_dur = 250;
    //     break;



    //   case PATTERN_ULTRA_STROBE:
    //     args.on_dur = 5;
    //     args.off_dur = 1;
    //     break;
    // case PATTERN_PARTICLE_STROBE:
    //     args.on_dur = 5;
    //     args.off_dur = 3;
    //     break;
    // case PATTERN_BLIP_STROBE:
    //     args.on_dur = 5;
    //     args.off_dur = 6;
    //     break;
    // case PATTERN_DOPS_STROBE:
    //     args.on_dur = 5;
    //     args.off_dur = 9;
    //     break;
    // case PATTERN_HYPERSTROBE_STROBE:
    //     args.on_dur = 5;
    //     args.off_dur = 17;
    //     break;
    // case PATTERN_STROBIE_STROBE:
    //     args.on_dur = 5;
    //     args.off_dur = 23;
    //     break;
    // case PATTERN_FLARE_STROBE:
    //     args.on_dur = 5;
    //     args.off_dur = 30;
    //     break;
    // case PATTERN_BURST_STROBE:
    //     args.on_dur = 5;
    //     args.off_dur = 40;
    //     break;
    // case PATTERN_FLICKER_STROBE:
    //     args.on_dur = 5;
    //     args.off_dur = 50;
    //     break;
    // case PATTERN_BLINKY_STROBE:
    //     args.on_dur = 5;
    //     args.off_dur = 150;
    //     break;
    // case PATTERN_INOVA_BLINK_STROBE:
    //     args.on_dur = 5;
    //     args.off_dur = 250;
    //     break;



    //   case PATTERN_ULTRA_HYPNOSTROBE:
    //     args.on_dur = 14;
    //     args.off_dur = 1;
    //     break;
    // case PATTERN_PARTICLE_HYPNOSTROBE:
    //     args.on_dur = 14;
    //     args.off_dur = 3;
    //     break;
    // case PATTERN_BLIP_HYPNOSTROBE:
    //     args.on_dur = 14;
    //     args.off_dur = 6;
    //     break;
    // case PATTERN_DOPS_HYPNOSTROBE:
    //     args.on_dur = 14;
    //     args.off_dur = 9;
    //     break;
    // case PATTERN_HYPERSTROBE_HYPNOSTROBE:
    //     args.on_dur = 14;
    //     args.off_dur = 17;
    //     break;
    // case PATTERN_STROBIE_HYPNOSTROBE:
    //     args.on_dur = 14;
    //     args.off_dur = 23;
    //     break;
    // case PATTERN_FLARE_HYPNOSTROBE:
    //     args.on_dur = 14;
    //     args.off_dur = 30;
    //     break;
    // case PATTERN_BURST_HYPNOSTROBE:
    //     args.on_dur = 14;
    //     args.off_dur = 40;
    //     break;
    // case PATTERN_FLICKER_HYPNOSTROBE:
    //     args.on_dur = 14;
    //     args.off_dur = 50;
    //     break;
    // case PATTERN_BLINKY_HYPNOSTROBE:
    //     args.on_dur = 14;
    //     args.off_dur = 150;
    //     break;
    // case PATTERN_INOVA_BLINK_HYPNOSTROBE:
    //     args.on_dur = 14;
    //     args.off_dur = 250;
    //     break;



    //   case PATTERN_ULTRA_HYPERSTROBE:
    //     args.on_dur = 16;
    //     args.off_dur = 1;
    //     break;
    // case PATTERN_PARTICLE_HYPERSTROBE:
    //     args.on_dur = 16;
    //     args.off_dur = 3;
    //     break;
    // case PATTERN_BLIP_HYPERSTROBE:
    //     args.on_dur = 16;
    //     args.off_dur = 6;
    //     break;
    // case PATTERN_DOPS_HYPERSTROBE:
    //     args.on_dur = 16;
    //     args.off_dur = 9;
    //     break;
    // case PATTERN_HYPERSTROBE_HYPERSTROBE:
    //     args.on_dur = 16;
    //     args.off_dur = 17;
    //     break;
    // case PATTERN_STROBIE_HYPERSTROBE:
    //     args.on_dur = 16;
    //     args.off_dur = 23;
    //     break;
    // case PATTERN_FLARE_HYPERSTROBE:
    //     args.on_dur = 16;
    //     args.off_dur = 30;
    //     break;
    // case PATTERN_BURST_HYPERSTROBE:
    //     args.on_dur = 16;
    //     args.off_dur = 40;
    //     break;
    // case PATTERN_FLICKER_HYPERSTROBE:
    //     args.on_dur = 16;
    //     args.off_dur = 50;
    //     break;
    // case PATTERN_BLINKY_HYPERSTROBE:
    //     args.on_dur = 16;
    //     args.off_dur = 150;
    //     break;
    // case PATTERN_INOVA_BLINK_HYPERSTROBE:
    //     args.on_dur = 16;
    //     args.off_dur = 250;
    //     break;



    //   case PATTERN_RIBBON_DOPS:
    //     args.on_dur = 1;
    //     args.off_dur = 0;
    //     break;
    // case PATTERN_RIBBON_STROBIE:
    //     args.on_dur = 3;
    //     args.off_dur = 0;
    //     break;
    // case PATTERN_RIBBON_STROBE:
    //     args.on_dur = 5;
    //     args.off_dur = 0;
    //     break;
    // case PATTERN_RIBBON_HYPNOSTROBE:
    //     args.on_dur = 14;
    //     args.off_dur = 0;
    //     break;
    // case PATTERN_RIBBON_HYPERSTROBE:
    //     args.on_dur = 16;
    //     args.off_dur = 0;
    //     break;




    //      case PATTERN_ULTRA_DOPS_GAP:
    //     args.on_dur = 1;
    //     args.off_dur = 0;
    //     args.gap_dur = 1;
    //     break;
    // case PATTERN_PARTICLE_DOPS_GAP:
    //     args.on_dur = 1;
    //     args.off_dur = 0;
    //     args.gap_dur = 3;
    //     break;
    // case PATTERN_BLIP_DOPS_GAP:
    //     args.on_dur = 1;
    //     args.off_dur = 0;
    //     args.gap_dur = 6;
    //     break;
    // case PATTERN_DOPS_DOPS_GAP:
    //     args.on_dur = 1;
    //     args.off_dur = 0;
    //     args.gap_dur = 9;
    //     break;
    // case PATTERN_HYPERSTROBE_DOPS_GAP:
    //     args.on_dur = 1;
    //     args.off_dur = 0;
    //     args.gap_dur = 17;
    //     break;
    // case PATTERN_STROBIE_DOPS_GAP:
    //     args.on_dur = 1;
    //     args.off_dur = 0;
    //     args.gap_dur = 23;
    //     break;
    // case PATTERN_FLARE_DOPS_GAP:
    //     args.on_dur = 1;
    //     args.off_dur = 0;
    //     args.gap_dur = 30;
    //     break;
    // case PATTERN_BURST_DOPS_GAP:
    //     args.on_dur = 1;
    //     args.off_dur = 0;
    //     args.gap_dur = 40;
    //     break;
    // case PATTERN_FLICKER_DOPS_GAP:
    //     args.on_dur = 1;
    //     args.off_dur = 0;
    //     args.gap_dur = 50;
    //     break;
    // case PATTERN_BLINKY_DOPS_GAP:
    //     args.on_dur = 1;
    //     args.off_dur = 0;
    //     args.gap_dur = 150;
    //     break;
    // case PATTERN_INOVA_BLINK_DOPS_GAP:
    //     args.on_dur = 1;
    //     args.off_dur = 0;
    //     args.gap_dur = 250;
    //     break;



    //   case PATTERN_ULTRA_STROBIE_GAP:
    //     args.on_dur = 3;
    //     args.off_dur = 0;
    //     args.gap_dur = 1;
    //     break;
    // case PATTERN_PARTICLE_STROBIE_GAP:
    //     args.on_dur = 3;
    //     args.off_dur = 0;
    //     args.gap_dur = 3;
    //     break;
    // case PATTERN_BLIP_STROBIE_GAP:
    //     args.on_dur = 3;
    //     args.off_dur = 0;
    //     args.gap_dur = 6;
    //     break;
    // case PATTERN_DOPS_STROBIE_GAP:
    //     args.on_dur = 3;
    //     args.off_dur = 0;
    //     args.gap_dur = 9;
    //     break;
    // case PATTERN_HYPERSTROBE_STROBIE_GAP:
    //     args.on_dur = 3;
    //     args.off_dur = 0;
    //     args.gap_dur = 17;
    //     break;
    // case PATTERN_STROBIE_STROBIE_GAP:
    //     args.on_dur = 3;
    //     args.off_dur = 0;
    //     args.gap_dur = 23;
    //     break;
    // case PATTERN_FLARE_STROBIE_GAP:
    //     args.on_dur = 3;
    //     args.off_dur = 0;
    //     args.gap_dur = 30;
    //     break;
    // case PATTERN_BURST_STROBIE_GAP:
    //     args.on_dur = 3;
    //     args.off_dur = 0;
    //     args.gap_dur = 40;
    //     break;
    // case PATTERN_FLICKER_STROBIE_GAP:
    //     args.on_dur = 3;
    //     args.off_dur = 0;
    //     args.gap_dur = 50;
    //     break;
    // case PATTERN_BLINKY_STROBIE_GAP:
    //     args.on_dur = 3;
    //     args.off_dur = 0;
    //     args.gap_dur = 150;
    //     break;
    // case PATTERN_INOVA_BLINK_STROBIE_GAP:
    //     args.on_dur = 3;
    //     args.off_dur = 0;
    //     args.gap_dur = 250;
    //     break;



    //   case PATTERN_ULTRA_STROBE_GAP:
    //     args.on_dur = 5;
    //     args.off_dur = 0;
    //     args.gap_dur = 1;
    //     break;
    // case PATTERN_PARTICLE_STROBE_GAP:
    //     args.on_dur = 5;
    //     args.off_dur = 0;
    //     args.gap_dur = 3;
    //     break;
    // case PATTERN_BLIP_STROBE_GAP:
    //     args.on_dur = 5;
    //     args.off_dur = 0;
    //     args.gap_dur = 6;
    //     break;
    // case PATTERN_DOPS_STROBE_GAP:
    //     args.on_dur = 5;
    //     args.off_dur = 0;
    //     args.gap_dur = 9;
    //     break;
    // case PATTERN_HYPERSTROBE_STROBE_GAP:
    //     args.on_dur = 5;
    //     args.off_dur = 0;
    //     args.gap_dur = 17;
    //     break;
    // case PATTERN_STROBIE_STROBE_GAP:
    //     args.on_dur = 5;
    //     args.off_dur = 0;
    //     args.gap_dur = 23;
    //     break;
    // case PATTERN_FLARE_STROBE_GAP:
    //     args.on_dur = 5;
    //     args.off_dur = 0;
    //     args.gap_dur = 30;
    //     break;
    // case PATTERN_BURST_STROBE_GAP:
    //     args.on_dur = 5;
    //     args.off_dur = 0;
    //     args.gap_dur = 40;
    //     break;
    // case PATTERN_FLICKER_STROBE_GAP:
    //     args.on_dur = 5;
    //     args.off_dur = 0;
    //     args.gap_dur = 50;
    //     break;
    // case PATTERN_BLINKY_STROBE_GAP:
    //     args.on_dur = 5;
    //     args.off_dur = 0;
    //     args.gap_dur = 150;
    //     break;
    // case PATTERN_INOVA_BLINK_STROBE_GAP:
    //     args.on_dur = 5;
    //     args.off_dur = 0;
    //     args.gap_dur = 250;
    //     break;



    //   case PATTERN_ULTRA_HYPNOSTROBE_GAP:
    //     args.on_dur = 14;
    //     args.off_dur = 0;
    //     args.gap_dur = 1;
    //     break;
    // case PATTERN_PARTICLE_HYPNOSTROBE_GAP:
    //     args.on_dur = 14;
    //     args.off_dur = 0;
    //     args.gap_dur = 3;
    //     break;
    // case PATTERN_BLIP_HYPNOSTROBE_GAP:
    //     args.on_dur = 14;
    //     args.off_dur = 0;
    //     args.gap_dur = 6;
    //     break;
    // case PATTERN_DOPS_HYPNOSTROBE_GAP:
    //     args.on_dur = 14;
    //     args.off_dur = 0;
    //     args.gap_dur = 9;
    //     break;
    // case PATTERN_HYPERSTROBE_HYPNOSTROBE_GAP:
    //     args.on_dur = 14;
    //     args.off_dur = 0;
    //     args.gap_dur = 17;
    //     break;
    // case PATTERN_STROBIE_HYPNOSTROBE_GAP:
    //     args.on_dur = 14;
    //     args.off_dur = 0;
    //     args.gap_dur = 23;
    //     break;
    // case PATTERN_FLARE_HYPNOSTROBE_GAP:
    //     args.on_dur = 14;
    //     args.off_dur = 0;
    //     args.gap_dur = 30;
    //     break;
    // case PATTERN_BURST_HYPNOSTROBE_GAP:
    //     args.on_dur = 14;
    //     args.off_dur = 0;
    //     args.gap_dur = 40;
    //     break;
    // case PATTERN_FLICKER_HYPNOSTROBE_GAP:
    //     args.on_dur = 14;
    //     args.off_dur = 0;
    //     args.gap_dur = 50;
    //     break;
    // case PATTERN_BLINKY_HYPNOSTROBE_GAP:
    //     args.on_dur = 14;
    //     args.off_dur = 0;
    //     args.gap_dur = 150;
    //     break;
    // case PATTERN_INOVA_BLINK_HYPNOSTROBE_GAP:
    //     args.on_dur = 14;
    //     args.off_dur = 0;
    //     args.gap_dur = 250;
    //     break;



    //   case PATTERN_ULTRA_HYPERSTROBE_GAP:
    //     args.on_dur = 16;
    //     args.off_dur = 0;
    //     args.gap_dur = 1;
    //     break;
    // case PATTERN_PARTICLE_HYPERSTROBE_GAP:
    //     args.on_dur = 16;
    //     args.off_dur = 0;
    //     args.gap_dur = 3;
    //     break;
    // case PATTERN_BLIP_HYPERSTROBE_GAP:
    //     args.on_dur = 16;
    //     args.off_dur = 0;
    //     args.gap_dur = 6;
    //     break;
    // case PATTERN_DOPS_HYPERSTROBE_GAP:
    //     args.on_dur = 16;
    //     args.off_dur = 0;
    //     args.gap_dur = 9;
    //     break;
    // case PATTERN_HYPERSTROBE_HYPERSTROBE_GAP:
    //     args.on_dur = 16;
    //     args.off_dur = 0;
    //     args.gap_dur = 17;
    //     break;
    // case PATTERN_STROBIE_HYPERSTROBE_GAP:
    //     args.on_dur = 16;
    //     args.off_dur = 0;
    //     args.gap_dur = 23;
    //     break;
    // case PATTERN_FLARE_HYPERSTROBE_GAP:
    //     args.on_dur = 16;
    //     args.off_dur = 0;
    //     args.gap_dur = 30;
    //     break;
    // case PATTERN_BURST_HYPERSTROBE_GAP:
    //     args.on_dur = 16;
    //     args.off_dur = 0;
    //     args.gap_dur = 40;
    //     break;
    // case PATTERN_FLICKER_HYPERSTROBE_GAP:
    //     args.on_dur = 16;
    //     args.off_dur = 0;
    //     args.gap_dur = 50;
    //     break;
    // case PATTERN_BLINKY_HYPERSTROBE_GAP:
    //     args.on_dur = 16;
    //     args.off_dur = 0;
    //     args.gap_dur = 150;
    //     break;
    // case PATTERN_INOVA_BLINK_HYPERSTROBE_GAP:
    //     args.on_dur = 16;
    //     args.off_dur = 0;
    //     args.gap_dur = 250;
    //     break;


    



    //   case PATTERN_ULTRA_DOPS_BLEND:
    //     args.on_dur = 1;
    //     args.off_dur = 1;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_PARTICLE_DOPS_BLEND:
    //     args.on_dur = 1;
    //     args.off_dur = 3;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_BLIP_DOPS_BLEND:
    //     args.on_dur = 1;
    //     args.off_dur = 6;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_DOPS_DOPS_BLEND:
    //     args.on_dur = 1;
    //     args.off_dur = 9;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_HYPERSTROBE_DOPS_BLEND:
    //     args.on_dur = 1;
    //     args.off_dur = 17;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_STROBIE_DOPS_BLEND:
    //     args.on_dur = 1;
    //     args.off_dur = 23;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_FLARE_DOPS_BLEND:
    //     args.on_dur = 1;
    //     args.off_dur = 30;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_BURST_DOPS_BLEND:
    //     args.on_dur = 1;
    //     args.off_dur = 40;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_FLICKER_DOPS_BLEND:
    //     args.on_dur = 1;
    //     args.off_dur = 50;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_BLINKY_DOPS_BLEND:
    //     args.on_dur = 1;
    //     args.off_dur = 150;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_INOVA_BLINK_DOPS_BLEND:
    //     args.on_dur = 1;
    //     args.off_dur = 250;
    //     args.blend_speed = 20;
    //     break;



    //   case PATTERN_ULTRA_STROBIE_BLEND:
    //     args.on_dur = 3;
    //     args.off_dur = 1;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_PARTICLE_STROBIE_BLEND:
    //     args.on_dur = 3;
    //     args.off_dur = 3;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_BLIP_STROBIE_BLEND:
    //     args.on_dur = 3;
    //     args.off_dur = 6;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_DOPS_STROBIE_BLEND:
    //     args.on_dur = 3;
    //     args.off_dur = 9;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_HYPERSTROBE_STROBIE_BLEND:
    //     args.on_dur = 3;
    //     args.off_dur = 17;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_STROBIE_STROBIE_BLEND:
    //     args.on_dur = 3;
    //     args.off_dur = 23;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_FLARE_STROBIE_BLEND:
    //     args.on_dur = 3;
    //     args.off_dur = 30;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_BURST_STROBIE_BLEND:
    //     args.on_dur = 3;
    //     args.off_dur = 40;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_FLICKER_STROBIE_BLEND:
    //     args.on_dur = 3;
    //     args.off_dur = 50;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_BLINKY_STROBIE_BLEND:
    //     args.on_dur = 3;
    //     args.off_dur = 150;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_INOVA_BLINK_STROBIE_BLEND:
    //     args.on_dur = 3;
    //     args.off_dur = 250;
    //     args.blend_speed = 20;
    //     break;



    //   case PATTERN_ULTRA_STROBE_BLEND:
    //     args.on_dur = 5;
    //     args.off_dur = 1;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_PARTICLE_STROBE_BLEND:
    //     args.on_dur = 5;
    //     args.off_dur = 3;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_BLIP_STROBE_BLEND:
    //     args.on_dur = 5;
    //     args.off_dur = 6;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_DOPS_STROBE_BLEND:
    //     args.on_dur = 5;
    //     args.off_dur = 9;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_HYPERSTROBE_STROBE_BLEND:
    //     args.on_dur = 5;
    //     args.off_dur = 17;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_STROBIE_STROBE_BLEND:
    //     args.on_dur = 5;
    //     args.off_dur = 23;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_FLARE_STROBE_BLEND:
    //     args.on_dur = 5;
    //     args.off_dur = 30;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_BURST_STROBE_BLEND:
    //     args.on_dur = 5;
    //     args.off_dur = 40;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_FLICKER_STROBE_BLEND:
    //     args.on_dur = 5;
    //     args.off_dur = 50;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_BLINKY_STROBE_BLEND:
    //     args.on_dur = 5;
    //     args.off_dur = 150;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_INOVA_BLINK_STROBE_BLEND:
    //     args.on_dur = 5;
    //     args.off_dur = 250;
    //     args.blend_speed = 20;
    //     break;



    //   case PATTERN_ULTRA_HYPNOSTROBE_BLEND:
    //     args.on_dur = 14;
    //     args.off_dur = 1;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_PARTICLE_HYPNOSTROBE_BLEND:
    //     args.on_dur = 14;
    //     args.off_dur = 3;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_BLIP_HYPNOSTROBE_BLEND:
    //     args.on_dur = 14;
    //     args.off_dur = 6;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_DOPS_HYPNOSTROBE_BLEND:
    //     args.on_dur = 14;
    //     args.off_dur = 9;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_HYPERSTROBE_HYPNOSTROBE_BLEND:
    //     args.on_dur = 14;
    //     args.off_dur = 17;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_STROBIE_HYPNOSTROBE_BLEND:
    //     args.on_dur = 14;
    //     args.off_dur = 23;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_FLARE_HYPNOSTROBE_BLEND:
    //     args.on_dur = 14;
    //     args.off_dur = 30;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_BURST_HYPNOSTROBE_BLEND:
    //     args.on_dur = 14;
    //     args.off_dur = 40;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_FLICKER_HYPNOSTROBE_BLEND:
    //     args.on_dur = 14;
    //     args.off_dur = 50;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_BLINKY_HYPNOSTROBE_BLEND:
    //     args.on_dur = 14;
    //     args.off_dur = 150;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_INOVA_BLINK_HYPNOSTROBE_BLEND:
    //     args.on_dur = 14;
    //     args.off_dur = 250;
    //     args.blend_speed = 20;
    //     break;



    //   case PATTERN_ULTRA_HYPERSTROBE_BLEND:
    //     args.on_dur = 16;
    //     args.off_dur = 1;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_PARTICLE_HYPERSTROBE_BLEND:
    //     args.on_dur = 16;
    //     args.off_dur = 3;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_BLIP_HYPERSTROBE_BLEND:
    //     args.on_dur = 16;
    //     args.off_dur = 6;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_DOPS_HYPERSTROBE_BLEND:
    //     args.on_dur = 16;
    //     args.off_dur = 9;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_HYPERSTROBE_HYPERSTROBE_BLEND:
    //     args.on_dur = 16;
    //     args.off_dur = 17;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_STROBIE_HYPERSTROBE_BLEND:
    //     args.on_dur = 16;
    //     args.off_dur = 23;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_FLARE_HYPERSTROBE_BLEND:
    //     args.on_dur = 16;
    //     args.off_dur = 30;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_BURST_HYPERSTROBE_BLEND:
    //     args.on_dur = 16;
    //     args.off_dur = 40;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_FLICKER_HYPERSTROBE_BLEND:
    //     args.on_dur = 16;
    //     args.off_dur = 50;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_BLINKY_HYPERSTROBE_BLEND:
    //     args.on_dur = 16;
    //     args.off_dur = 150;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_INOVA_BLINK_HYPERSTROBE_BLEND:
    //     args.on_dur = 16;
    //     args.off_dur = 250;
    //     args.blend_speed = 20;
    //     break;



    //   case PATTERN_RIBBON_DOPS_BLEND:
    //     args.on_dur = 1;
    //     args.off_dur = 0;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_RIBBON_STROBIE_BLEND:
    //     args.on_dur = 3;
    //     args.off_dur = 0;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_RIBBON_STROBE_BLEND:
    //     args.on_dur = 5;
    //     args.off_dur = 0;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_RIBBON_HYPNOSTROBE_BLEND:
    //     args.on_dur = 14;
    //     args.off_dur = 0;
    //     args.blend_speed = 20;
    //     break;
    // case PATTERN_RIBBON_HYPERSTROBE_BLEND:
    //     args.on_dur = 16;
    //     args.off_dur = 0;
    //     args.blend_speed = 20;
    //     break;






    case PATTERN_HYPERSTROBE_DOPS_DASH:
        args.on_dur = 1;
        args.off_dur = 9;
        args.dash_dur = 17;
        break;
    case PATTERN_STROBIE_DOPS_DASH:
        args.on_dur = 1;
        args.off_dur = 9;
        args.dash_dur = 23;
        break;
    case PATTERN_FLARE_DOPS_DASH:
        args.on_dur = 1;
        args.off_dur = 9;
        args.dash_dur = 30;
        break;
    case PATTERN_BURST_DOPS_DASH:
        args.on_dur = 1;
        args.off_dur = 9;
        args.dash_dur = 40;
        break;
    case PATTERN_FLICKER_DOPS_DASH:
        args.on_dur = 1;
        args.off_dur = 9;
        args.dash_dur = 50;
        break;
    case PATTERN_BLINKY_DOPS_DASH:
        args.on_dur = 1;
        args.off_dur = 9;
        args.dash_dur = 150;
        break;




    case PATTERN_HYPERSTROBE_STROBIE_DASH:
        args.on_dur = 3;
        args.off_dur = 23;
        args.dash_dur = 17;
        break;
    case PATTERN_STROBIE_STROBIE_DASH:
        args.on_dur = 3;
        args.off_dur = 23;
        args.dash_dur = 23;
        break;
    case PATTERN_FLARE_STROBIE_DASH:
        args.on_dur = 3;
        args.off_dur = 23;
        args.dash_dur = 30;
        break;
    case PATTERN_BURST_STROBIE_DASH:
        args.on_dur = 3;
        args.off_dur = 23;
        args.dash_dur = 40;
        break;
    case PATTERN_FLICKER_STROBIE_DASH:
        args.on_dur = 3;
        args.off_dur = 23;
        args.dash_dur = 50;
        break;
    case PATTERN_BLINKY_STROBIE_DASH:
        args.on_dur = 3;
        args.off_dur = 23;
        args.dash_dur = 150;
        break;





    case PATTERN_HYPERSTROBE_STROBE_DASH:
        args.on_dur = 5;
        args.off_dur = 8;
        args.dash_dur = 17;
        break;
    case PATTERN_STROBIE_STROBE_DASH:
        args.on_dur = 5;
        args.off_dur = 8;
        args.dash_dur = 23;
        break;
    case PATTERN_FLARE_STROBE_DASH:
        args.on_dur = 5;
        args.off_dur = 8;
        args.dash_dur = 30;
        break;
    case PATTERN_BURST_STROBE_DASH:
        args.on_dur = 5;
        args.off_dur = 8;
        args.dash_dur = 40;
        break;
    case PATTERN_FLICKER_STROBE_DASH:
        args.on_dur = 5;
        args.off_dur = 8;
        args.dash_dur = 50;
        break;
    case PATTERN_BLINKY_STROBE_DASH:
        args.on_dur = 5;
        args.off_dur = 8;
        args.dash_dur = 150;
        break;





    case PATTERN_HYPERSTROBE_HYPNOSTROBE_DASH:
        args.on_dur = 14;
        args.off_dur = 10;
        args.dash_dur = 17;
        break;
    case PATTERN_STROBIE_HYPNOSTROBE_DASH:
        args.on_dur = 14;
        args.off_dur = 10;
        args.dash_dur = 23;
        break;
    case PATTERN_FLARE_HYPNOSTROBE_DASH:
        args.on_dur = 14;
        args.off_dur = 10;
        args.dash_dur = 30;
        break;
    case PATTERN_BURST_HYPNOSTROBE_DASH:
        args.on_dur = 14;
        args.off_dur = 10;
        args.dash_dur = 40;
        break;
    case PATTERN_FLICKER_HYPNOSTROBE_DASH:
        args.on_dur = 14;
        args.off_dur = 10;
        args.dash_dur = 50;
        break;
    case PATTERN_BLINKY_HYPNOSTROBE_DASH:
        args.on_dur = 14;
        args.off_dur = 10;
        args.dash_dur = 150;
        break;





    case PATTERN_HYPERSTROBE_HYPERSTROBE_DASH:
        args.on_dur = 16;
        args.off_dur = 17;
        args.dash_dur = 17;
        break;
    case PATTERN_STROBIE_HYPERSTROBE_DASH:
        args.on_dur = 16;
        args.off_dur = 17;
        args.dash_dur = 23;
        break;
    case PATTERN_FLARE_HYPERSTROBE_DASH:
        args.on_dur = 16;
        args.off_dur = 17;
        args.dash_dur = 30;
        break;
    case PATTERN_BURST_HYPERSTROBE_DASH:
        args.on_dur = 16;
        args.off_dur = 17;
        args.dash_dur = 40;
        break;
    case PATTERN_FLICKER_HYPERSTROBE_DASH:
        args.on_dur = 16;
        args.off_dur = 17;
        args.dash_dur = 50;
        break;
    case PATTERN_BLINKY_HYPERSTROBE_DASH:
        args.on_dur = 16;
        args.off_dur = 17;
        args.dash_dur = 150;
        break;




    //   case PATTERN_RIBBON_DOPS_DASH:
    //     args.on_dur = 1;
    //     args.off_dur = 0;
    //     args.dash_dur = 1;
    //     break;
    // case PATTERN_RIBBON_STROBIE_DASH:
    //     args.on_dur = 3;
    //     args.off_dur = 0;
    //     args.dash_dur = 3;
    //     break;
    // case PATTERN_RIBBON_STROBE_DASH:
    //     args.on_dur = 5;
    //     args.off_dur = 0;
    //     args.dash_dur = 5;
    //     break;
    // case PATTERN_RIBBON_HYPNOSTROBE_DASH:
    //     args.on_dur = 14;
    //     args.off_dur = 0;
    //     args.dash_dur = 14;
    //     break;
    // case PATTERN_RIBBON_HYPERSTROBE_DASH:
    //     args.on_dur = 16;
    //     args.off_dur = 0;
    //     args.dash_dur = 16;
    //     break;

  }
  pat.setArgs(args);
}
