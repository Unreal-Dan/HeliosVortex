#pragma once

#include <inttypes.h>

// ====================================================================================================
//  Color Constants
//
// This file defines constants for colors as we best see them for the Vortex Engine

// Some Pre-defined hue values
// TODO: remove HSV_ underscore once FastLED is gone
#define HSV_HUE_RED 0
#define HSV_HUE_ORANGE 32
#define HSV_HUE_YELLOW 64
#define HSV_HUE_GREEN 96
#define HSV_HUE_AQUA 128
#define HSV_HUE_BLUE 160
#define HSV_HUE_PURPLE 192
#define HSV_HUE_PINK 224

// if this bit is present it's an HSV constant
#define HSV_BIT ((uint32_t)1 << 31)

// produce a DWORD HSV constant
#define HSV(h, s, v) (HSV_BIT | ((uint32_t)h << 16) | ((uint32_t)s << 8) | (uint32_t)v)

// Pre defined hex HSV values
#define HSV_WHITE HSV_BIT | (uint32_t)0x00006E   //   0   0 110
#define HSV_BLUE HSV_BIT | (uint32_t)0xA0FF6E    // 160 255 110
#define HSV_YELLOW HSV_BIT | (uint32_t)0x3CFF6E  //  60 255 110
#define HSV_RED HSV_BIT | (uint32_t)0x00FF6E     //   0 255 110
#define HSV_GREEN HSV_BIT | (uint32_t)0x55FF6E   //  85 255 110
#define HSV_CYAN HSV_BIT | (uint32_t)0x78FF6E    // 120 255 110
#define HSV_PURPLE HSV_BIT | (uint32_t)0xD4FF6E  // 212 255 110
#define HSV_ORANGE HSV_BIT | (uint32_t)0x14FF6E  //  20 255 110
#define HSV_OFF HSV_BIT | (uint32_t)0x000000     //   0   0   0

// Pure RGB colors (brightest)
#define RGB_WHITE (uint32_t)0xFFFFFF    // 255 255 255
#define RGB_BLUE (uint32_t)0x0000FF     //   0   0 255
#define RGB_YELLOW (uint32_t)0xFFFF00   // 255 255   0
#define RGB_RED (uint32_t)0xFF0000      // 255   0   0
#define RGB_GREEN (uint32_t)0x00FF00    //   0 255   0
#define RGB_CYAN (uint32_t)0x00FFFF     //   0 255 255
#define RGB_PURPLE (uint32_t)0x9933FF   // 153  51 255
#define RGB_ORANGE (uint32_t)0xFF8300   // 255 131   0
#define RGB_PINK (uint32_t)0xFF0099     // 255   0 153
#define RGB_MAGENTA (uint32_t)0xFF00FF  // 255   0 255
#define RGB_OFF (uint32_t)0x000000      //   0   0   0

// Helios Colors
#define HELIOS_RGB_WHITE (uint32_t)0x000000         // 0, 0, 0
#define HELIOS_RGB_RED (uint32_t)0xFF0000           // 255, 0, 0
#define HELIOS_RGB_CORAL_ORANGE (uint32_t)0xFF1E00  // 255, 30, 0
#define HELIOS_RGB_ORANGE (uint32_t)0xFF3C00        // 255, 60, 0
#define HELIOS_RGB_YELLOW (uint32_t)0xFF7800        // 255, 120, 0
#define HELIOS_RGB_LIME_GREEN (uint32_t)0x59FF00    // 89, 255, 0
#define HELIOS_RGB_GREEN (uint32_t)0x00FF00         // 0, 255, 0
#define HELIOS_RGB_SEAFOAM (uint32_t)0x00FF3C       // 0, 255, 60
#define HELIOS_RGB_TURQUOISE (uint32_t)0x00FFD1     // 0, 255, 209
#define HELIOS_RGB_ICE_BLUE (uint32_t)0x00A7FF      // 0, 167, 255
#define HELIOS_RGB_LIGHT_BLUE (uint32_t)0x0047FF    // 0, 71, 255
#define HELIOS_RGB_BLUE (uint32_t)0x0000FF          // 0, 0, 255
#define HELIOS_RGB_ROYAL_BLUE (uint32_t)0x1D00FF    // 29, 0, 255
#define HELIOS_RGB_PURPLE (uint32_t)0x8300FF        // 131, 0, 255
#define HELIOS_RGB_PINK (uint32_t)0xD200FF          // 210, 0, 255
#define HELIOS_RGB_HOT_PINK (uint32_t)0xFF00B4      // 255, 0, 180
#define HELIOS_RGB_MAGENTA (uint32_t)0xFF003C       // 255, 0, 60

// Helios Medium Brightness Colors
#define HELIOS_RGB_WHITE_BRI_MEDIUM (uint32_t)0x000000         // 0, 0, 0
#define HELIOS_RGB_RED_BRI_MEDIUM (uint32_t)0x780000           // 120, 0, 0
#define HELIOS_RGB_CORAL_ORANGE_BRI_MEDIUM (uint32_t)0x780E00  // 120, 14, 0
#define HELIOS_RGB_ORANGE_BRI_MEDIUM (uint32_t)0x781C00        // 120, 28, 0
#define HELIOS_RGB_YELLOW_BRI_MEDIUM (uint32_t)0x783800        // 120, 56, 0
#define HELIOS_RGB_LIME_GREEN_BRI_MEDIUM (uint32_t)0x297800    // 41, 120, 0
#define HELIOS_RGB_GREEN_BRI_MEDIUM (uint32_t)0x007800         // 0, 120, 0
#define HELIOS_RGB_SEAFOAM_BRI_MEDIUM (uint32_t)0x00781C       // 0, 120, 28
#define HELIOS_RGB_TURQUOISE_BRI_MEDIUM (uint32_t)0x007862     // 0, 120, 98
#define HELIOS_RGB_ICE_BLUE_BRI_MEDIUM (uint32_t)0x004E78      // 0, 78, 120
#define HELIOS_RGB_LIGHT_BLUE_BRI_MEDIUM (uint32_t)0x002178    // 0, 33, 120
#define HELIOS_RGB_BLUE_BRI_MEDIUM (uint32_t)0x000078          // 0, 0, 120
#define HELIOS_RGB_ROYAL_BLUE_BRI_MEDIUM (uint32_t)0x0D0078    // 13, 0, 120
#define HELIOS_RGB_PURPLE_BRI_MEDIUM (uint32_t)0x3D0078        // 61, 0, 120
#define HELIOS_RGB_PINK_BRI_MEDIUM (uint32_t)0x620078          // 98, 0, 120
#define HELIOS_RGB_HOT_PINK_BRI_MEDIUM (uint32_t)0x780054      // 120, 0, 84
#define HELIOS_RGB_MAGENTA_BRI_MEDIUM (uint32_t)0x78001C       // 120, 0, 28

// Helios Low Brightness Colors
#define HELIOS_RGB_WHITE_BRI_LOW (uint32_t)0x000000         // 0, 0, 0
#define HELIOS_RGB_RED_BRI_LOW (uint32_t)0x3C0000           // 60, 0, 0
#define HELIOS_RGB_CORAL_ORANGE_BRI_LOW (uint32_t)0x3C0700  // 60, 7, 0
#define HELIOS_RGB_ORANGE_BRI_LOW (uint32_t)0x3C0E00        // 60, 14, 0
#define HELIOS_RGB_YELLOW_BRI_LOW (uint32_t)0x3C1C00        // 60, 28, 0
#define HELIOS_RGB_LIME_GREEN_BRI_LOW (uint32_t)0x143C00    // 20, 60, 0
#define HELIOS_RGB_GREEN_BRI_LOW (uint32_t)0x003C00         // 0, 60, 0
#define HELIOS_RGB_SEAFOAM_BRI_LOW (uint32_t)0x003C0E       // 0, 60, 14
#define HELIOS_RGB_TURQUOISE_BRI_LOW (uint32_t)0x003C31     // 0, 60, 49
#define HELIOS_RGB_ICE_BLUE_BRI_LOW (uint32_t)0x00273C      // 0, 39, 60
#define HELIOS_RGB_LIGHT_BLUE_BRI_LOW (uint32_t)0x00103C    // 0, 16, 60
#define HELIOS_RGB_BLUE_BRI_LOW (uint32_t)0x00003C          // 0, 0, 60
#define HELIOS_RGB_ROYAL_BLUE_BRI_LOW (uint32_t)0x06003C    // 6, 0, 60
#define HELIOS_RGB_PURPLE_BRI_LOW (uint32_t)0x1E003C        // 30, 0, 60
#define HELIOS_RGB_PINK_BRI_LOW (uint32_t)0x31003C          // 49, 0, 60
#define HELIOS_RGB_HOT_PINK_BRI_LOW (uint32_t)0x3C002A      // 60, 0, 42
#define HELIOS_RGB_MAGENTA_BRI_LOW (uint32_t)0x3C000E       // 60, 0, 14

// Helios Lowest Brightness Colors
#define HELIOS_RGB_WHITE_BRI_LOWEST (uint32_t)0x000000         // 0, 0, 0
#define HELIOS_RGB_RED_BRI_LOWEST (uint32_t)0x0A0000           // 10, 0, 0
#define HELIOS_RGB_CORAL_ORANGE_BRI_LOWEST (uint32_t)0x0A0100  // 10, 1, 0
#define HELIOS_RGB_ORANGE_BRI_LOWEST (uint32_t)0x0A0200        // 10, 2, 0
#define HELIOS_RGB_YELLOW_BRI_LOWEST (uint32_t)0x0A0400        // 10, 4, 0
#define HELIOS_RGB_LIME_GREEN_BRI_LOWEST (uint32_t)0x030A00    // 3, 10, 0
#define HELIOS_RGB_GREEN_BRI_LOWEST (uint32_t)0x000A00         // 0, 10, 0
#define HELIOS_RGB_SEAFOAM_BRI_LOWEST (uint32_t)0x000A02       // 0, 10, 2
#define HELIOS_RGB_TURQUOISE_BRI_LOWEST (uint32_t)0x000A08     // 0, 10, 8
#define HELIOS_RGB_ICE_BLUE_BRI_LOWEST (uint32_t)0x00060A      // 0, 6, 10
#define HELIOS_RGB_LIGHT_BLUE_BRI_LOWEST (uint32_t)0x00020A    // 0, 2, 10
#define HELIOS_RGB_BLUE_BRI_LOWEST (uint32_t)0x00000A          // 0, 0, 10
#define HELIOS_RGB_ROYAL_BLUE_BRI_LOWEST (uint32_t)0x01000A    // 1, 0, 10
#define HELIOS_RGB_PURPLE_BRI_LOWEST (uint32_t)0x05000A        // 5, 0, 10
#define HELIOS_RGB_PINK_BRI_LOWEST (uint32_t)0x08000A          // 8, 0, 10
#define HELIOS_RGB_HOT_PINK_BRI_LOWEST (uint32_t)0x0A0007      // 10, 0, 7
#define HELIOS_RGB_MAGENTA_BRI_LOWEST (uint32_t)0x0A0002       // 10, 0, 2

// Helios Medium Saturation Colors
#define HELIOS_RGB_WHITE_SAT_MEDIUM (uint32_t)0x000000         // 0, 0, 0
#define HELIOS_RGB_RED_SAT_MEDIUM (uint32_t)0xFF2222           // 255, 34, 34
#define HELIOS_RGB_CORAL_ORANGE_SAT_MEDIUM (uint32_t)0xFF3C22  // 255, 60, 34
#define HELIOS_RGB_ORANGE_SAT_MEDIUM (uint32_t)0xFF5622        // 255, 86, 34
#define HELIOS_RGB_YELLOW_SAT_MEDIUM (uint32_t)0xFF8A22        // 255, 138, 34
#define HELIOS_RGB_LIME_GREEN_SAT_MEDIUM (uint32_t)0x6FFF22    // 111, 255, 34
#define HELIOS_RGB_GREEN_SAT_MEDIUM (uint32_t)0x22FF22         // 34, 255, 34
#define HELIOS_RGB_SEAFOAM_SAT_MEDIUM (uint32_t)0x22FF56       // 34, 255, 86
#define HELIOS_RGB_TURQUOISE_SAT_MEDIUM (uint32_t)0x22FFD7     // 34, 255, 215
#define HELIOS_RGB_ICE_BLUE_SAT_MEDIUM (uint32_t)0x22B3FF      // 34, 179, 255
#define HELIOS_RGB_LIGHT_BLUE_SAT_MEDIUM (uint32_t)0x2260FF    // 34, 96, 255
#define HELIOS_RGB_BLUE_SAT_MEDIUM (uint32_t)0x2222FF          // 34, 34, 255
#define HELIOS_RGB_ROYAL_BLUE_SAT_MEDIUM (uint32_t)0x3C22FF    // 60, 34, 255
#define HELIOS_RGB_PURPLE_SAT_MEDIUM (uint32_t)0x9422FF        // 148, 34, 255
#define HELIOS_RGB_PINK_SAT_MEDIUM (uint32_t)0xD822FF          // 216, 34, 255
#define HELIOS_RGB_HOT_PINK_SAT_MEDIUM (uint32_t)0xFF22BE      // 255, 34, 190
#define HELIOS_RGB_MAGENTA_SAT_MEDIUM (uint32_t)0xFF2256       // 255, 34, 86

// Helios Low Saturation Colors
#define HELIOS_RGB_WHITE_SAT_LOW (uint32_t)0x000000         // 0, 0, 0
#define HELIOS_RGB_RED_SAT_LOW (uint32_t)0xFF5555           // 255, 85, 85
#define HELIOS_RGB_CORAL_ORANGE_SAT_LOW (uint32_t)0xFF6955  // 255, 105, 85
#define HELIOS_RGB_ORANGE_SAT_LOW (uint32_t)0xFF7D55        // 255, 125, 85
#define HELIOS_RGB_YELLOW_SAT_LOW (uint32_t)0xFFA555        // 255, 165, 85
#define HELIOS_RGB_LIME_GREEN_SAT_LOW (uint32_t)0x90FF55    // 144, 255, 85
#define HELIOS_RGB_GREEN_SAT_LOW (uint32_t)0x55FF55         // 85, 255, 85
#define HELIOS_RGB_SEAFOAM_SAT_LOW (uint32_t)0x55FF7D       // 85, 255, 125
#define HELIOS_RGB_TURQUOISE_SAT_LOW (uint32_t)0x55FFE0     // 85, 255, 224
#define HELIOS_RGB_ICE_BLUE_SAT_LOW (uint32_t)0x55C4FF      // 85, 196, 255
#define HELIOS_RGB_LIGHT_BLUE_SAT_LOW (uint32_t)0x5584FF    // 85, 132, 255
#define HELIOS_RGB_BLUE_SAT_LOW (uint32_t)0x5555FF          // 85, 85, 255
#define HELIOS_RGB_ROYAL_BLUE_SAT_LOW (uint32_t)0x6855FF    // 104, 85, 255
#define HELIOS_RGB_PURPLE_SAT_LOW (uint32_t)0xAC55FF        // 172, 85, 255
#define HELIOS_RGB_PINK_SAT_LOW (uint32_t)0xE055FF          // 224, 85, 255
#define HELIOS_RGB_HOT_PINK_SAT_LOW (uint32_t)0xFF55CD      // 255, 85, 205
#define HELIOS_RGB_MAGENTA_SAT_LOW (uint32_t)0xFF557D       // 255, 85, 125

// Helios Lowest Saturation Colors
#define HELIOS_RGB_WHITE_SAT_LOWEST (uint32_t)0x000000         // 0, 0, 0
#define HELIOS_RGB_RED_SAT_LOWEST (uint32_t)0xFF7D7D           // 255, 125, 125
#define HELIOS_RGB_CORAL_ORANGE_SAT_LOWEST (uint32_t)0xFF8C7D  // 255, 140, 125
#define HELIOS_RGB_ORANGE_SAT_LOWEST (uint32_t)0xFF9B7D        // 255, 155, 125
#define HELIOS_RGB_YELLOW_SAT_LOWEST (uint32_t)0xFFBA7D        // 255, 186, 125
#define HELIOS_RGB_LIME_GREEN_SAT_LOWEST (uint32_t)0xAAFF7D    // 170, 255, 125
#define HELIOS_RGB_GREEN_SAT_LOWEST (uint32_t)0x7DFF7D         // 125, 255, 125
#define HELIOS_RGB_SEAFOAM_SAT_LOWEST (uint32_t)0x7DFF9B       // 125, 255, 155
#define HELIOS_RGB_TURQUOISE_SAT_LOWEST (uint32_t)0x7DFFE7     // 125, 255, 231
#define HELIOS_RGB_ICE_BLUE_SAT_LOWEST (uint32_t)0x7DD2FF      // 125, 210, 255
#define HELIOS_RGB_LIGHT_BLUE_SAT_LOWEST (uint32_t)0x7DA1FF    // 125, 161, 255
#define HELIOS_RGB_BLUE_SAT_LOWEST (uint32_t)0x7D7DFF          // 125, 125, 255
#define HELIOS_RGB_ROYAL_BLUE_SAT_LOWEST (uint32_t)0x8B7DFF    // 139, 125, 255
#define HELIOS_RGB_PURPLE_SAT_LOWEST (uint32_t)0xBF7DFF        // 191, 125, 255
#define HELIOS_RGB_PINK_SAT_LOWEST (uint32_t)0xE87DFF          // 232, 125, 255
#define HELIOS_RGB_HOT_PINK_SAT_LOWEST (uint32_t)0xFF7DD8      // 255, 125, 216
#define HELIOS_RGB_MAGENTA_SAT_LOWEST (uint32_t)0xFF7D9B       // 255, 125, 155

// ===========================================================
//  Steps of colors in increasing brightness from 0 to 9,
//  Where O is the lowest brightness and 9 is the highest
//  brightness before the pure colors above

#define RGB_WHITE0 (uint32_t)0x101010  //  16  16  16
#define RGB_WHITE1 (uint32_t)0x1C1C1C  //  28  28  28
#define RGB_WHITE2 (uint32_t)0x383838  //  56  56  56
#define RGB_WHITE3 (uint32_t)0x545454  //  84  84  84
#define RGB_WHITE4 (uint32_t)0x707070  // 112 112 112
#define RGB_WHITE5 (uint32_t)0x8C8C8C  // 140 140 140
#define RGB_WHITE6 (uint32_t)0xA8A8A8  // 168 168 168
#define RGB_WHITE7 (uint32_t)0xC4C4C4  // 196 196 196
#define RGB_WHITE8 (uint32_t)0xE0E0E0  // 224 224 224
#define RGB_WHITE9 (uint32_t)0xFCFCFC  // 252 252 252

#define RGB_BLUE0 (uint32_t)0x000010  //   0   0  16
#define RGB_BLUE1 (uint32_t)0x00001C  //   0   0  28
#define RGB_BLUE2 (uint32_t)0x000038  //   0   0  56
#define RGB_BLUE3 (uint32_t)0x000054  //   0   0  84
#define RGB_BLUE4 (uint32_t)0x000070  //   0   0 112
#define RGB_BLUE5 (uint32_t)0x00008C  //   0   0 140
#define RGB_BLUE6 (uint32_t)0x0000A8  //   0   0 168
#define RGB_BLUE7 (uint32_t)0x0000C4  //   0   0 196
#define RGB_BLUE8 (uint32_t)0x0000E0  //   0   0 224
#define RGB_BLUE9 (uint32_t)0x0000FC  //   0   0 252

#define RGB_YELLOW0 (uint32_t)0x101000  //  16  16   0
#define RGB_YELLOW1 (uint32_t)0x1C1C00  //  28  28   0
#define RGB_YELLOW2 (uint32_t)0x383800  //  56  56   0
#define RGB_YELLOW3 (uint32_t)0x545400  //  84  84   0
#define RGB_YELLOW4 (uint32_t)0x707000  // 112 112   0
#define RGB_YELLOW5 (uint32_t)0x8C8C00  // 140 140   0
#define RGB_YELLOW6 (uint32_t)0xA8A800  // 168 168   0
#define RGB_YELLOW7 (uint32_t)0xC4C400  // 196 196   0
#define RGB_YELLOW8 (uint32_t)0xE0E000  // 224 224   0
#define RGB_YELLOW9 (uint32_t)0xFCFC00  // 252 252   0

#define RGB_RED0 (uint32_t)0x100000  //  16   0   0
#define RGB_RED1 (uint32_t)0x1C0000  //  28   0   0
#define RGB_RED2 (uint32_t)0x380000  //  56   0   0
#define RGB_RED3 (uint32_t)0x540000  //  84   0   0
#define RGB_RED4 (uint32_t)0x700000  // 112   0   0
#define RGB_RED5 (uint32_t)0x8C0000  // 140   0   0
#define RGB_RED6 (uint32_t)0xA80000  // 168   0   0
#define RGB_RED7 (uint32_t)0xC40000  // 196   0   0
#define RGB_RED8 (uint32_t)0xE00000  // 224   0   0
#define RGB_RED9 (uint32_t)0xFC0000  // 252   0   0

#define RGB_GREEN0 (uint32_t)0x001000  //   0  16   0
#define RGB_GREEN1 (uint32_t)0x001C00  //   0  28   0
#define RGB_GREEN2 (uint32_t)0x003800  //   0  56   0
#define RGB_GREEN3 (uint32_t)0x005400  //   0  84   0
#define RGB_GREEN4 (uint32_t)0x007000  //   0 112   0
#define RGB_GREEN5 (uint32_t)0x008C00  //   0 140   0
#define RGB_GREEN6 (uint32_t)0x00A800  //   0 168   0
#define RGB_GREEN7 (uint32_t)0x00C400  //   0 196   0
#define RGB_GREEN8 (uint32_t)0x00E000  //   0 224   0
#define RGB_GREEN9 (uint32_t)0x00FC00  //   0 252   0

#define RGB_CYAN0 (uint32_t)0x001010  //   0  16  16
#define RGB_CYAN1 (uint32_t)0x001C1C  //   0  28  28
#define RGB_CYAN2 (uint32_t)0x003838  //   0  56  56
#define RGB_CYAN3 (uint32_t)0x005454  //   0  84  84
#define RGB_CYAN4 (uint32_t)0x007070  //   0 112 112
#define RGB_CYAN5 (uint32_t)0x008C8C  //   0 140 140
#define RGB_CYAN6 (uint32_t)0x00A8A8  //   0 168 168
#define RGB_CYAN7 (uint32_t)0x00C4C4  //   0 196 196
#define RGB_CYAN8 (uint32_t)0x00E0E0  //   0 224 224
#define RGB_CYAN9 (uint32_t)0x00FCFC  //   0 252 252

#define RGB_PURPLE0 (uint32_t)0x090310  //   9   3  16
#define RGB_PURPLE1 (uint32_t)0x1C001C  //  28   0  28
#define RGB_PURPLE2 (uint32_t)0x380038  //  56   0  56
#define RGB_PURPLE3 (uint32_t)0x540054  //  84   0  84
#define RGB_PURPLE4 (uint32_t)0x700070  // 112   0 112
#define RGB_PURPLE5 (uint32_t)0x8C008C  // 140   0 140
#define RGB_PURPLE6 (uint32_t)0xA800A8  // 168   0 168
#define RGB_PURPLE7 (uint32_t)0xC400C4  // 196   0 196
#define RGB_PURPLE8 (uint32_t)0xE000E0  // 224   0 224
#define RGB_PURPLE9 (uint32_t)0xFC00FC  // 252   0 252

#define RGB_ORANGE0 (uint32_t)0x100800  //  16   8   0
#define RGB_ORANGE1 (uint32_t)0x1C0E00  //  28  14   0
#define RGB_ORANGE2 (uint32_t)0x381C00  //  56  28   0
#define RGB_ORANGE3 (uint32_t)0x542B00  //  84  43   0
#define RGB_ORANGE4 (uint32_t)0x703900  // 112  57   0
#define RGB_ORANGE5 (uint32_t)0x8C4800  // 140  72   0
#define RGB_ORANGE6 (uint32_t)0xA85600  // 168  86   0
#define RGB_ORANGE7 (uint32_t)0xC46500  // 196 101   0
#define RGB_ORANGE8 (uint32_t)0xE07300  // 224 115   0
#define RGB_ORANGE9 (uint32_t)0xFC8200  // 252 130   0

// Helios White Variations
#define HELIOS_RGB_WHITE0 (uint32_t)0x101010  //  16  16  16
#define HELIOS_RGB_WHITE1 (uint32_t)0x1C1C1C  //  28  28  28
#define HELIOS_RGB_WHITE2 (uint32_t)0x383838  //  56  56  56
#define HELIOS_RGB_WHITE3 (uint32_t)0x545454  //  84  84  84
#define HELIOS_RGB_WHITE4 (uint32_t)0x707070  // 112 112 112
#define HELIOS_RGB_WHITE5 (uint32_t)0x8C8C8C  // 140 140 140
#define HELIOS_RGB_WHITE6 (uint32_t)0xA8A8A8  // 168 168 168
#define HELIOS_RGB_WHITE7 (uint32_t)0xC4C4C4  // 196 196 196
#define HELIOS_RGB_WHITE8 (uint32_t)0xE0E0E0  // 224 224 224
#define HELIOS_RGB_WHITE9 (uint32_t)0xFCFCFC  // 252 252 252

// Helios Red Variations
#define HELIOS_RGB_RED0 (uint32_t)0x190000  //  25   0   0
#define HELIOS_RGB_RED1 (uint32_t)0x330000  //  51   0   0
#define HELIOS_RGB_RED2 (uint32_t)0x4C0000  //  76   0   0
#define HELIOS_RGB_RED3 (uint32_t)0x660000  // 102   0   0
#define HELIOS_RGB_RED4 (uint32_t)0x7F0000  // 127   0   0
#define HELIOS_RGB_RED5 (uint32_t)0x990000  // 153   0   0
#define HELIOS_RGB_RED6 (uint32_t)0xB30000  // 179   0   0
#define HELIOS_RGB_RED7 (uint32_t)0xCC0000  // 204   0   0
#define HELIOS_RGB_RED8 (uint32_t)0xE60000  // 230   0   0
#define HELIOS_RGB_RED9 (uint32_t)0xFF0000  // 255   0   0

// Helios Blood Orange Variations
#define HELIOS_RGB_CORAL_ORANGE0 (uint32_t)0x190500  //  25   5   0
#define HELIOS_RGB_CORAL_ORANGE1 (uint32_t)0x330B00  //  51  11   0
#define HELIOS_RGB_CORAL_ORANGE2 (uint32_t)0x4C1000  //  76  16   0
#define HELIOS_RGB_CORAL_ORANGE3 (uint32_t)0x661500  // 102  21   0
#define HELIOS_RGB_CORAL_ORANGE4 (uint32_t)0x7F1B00  // 127  27   0
#define HELIOS_RGB_CORAL_ORANGE5 (uint32_t)0x992000  // 153  32   0
#define HELIOS_RGB_CORAL_ORANGE6 (uint32_t)0xB32600  // 179  38   0
#define HELIOS_RGB_CORAL_ORANGE7 (uint32_t)0xCC2B00  // 204  43   0
#define HELIOS_RGB_CORAL_ORANGE8 (uint32_t)0xE63000  // 230  48   0
#define HELIOS_RGB_CORAL_ORANGE9 (uint32_t)0xFF1E00  // 255  30   0

// Helios Orange Variations
#define HELIOS_RGB_ORANGE0 (uint32_t)0x190900  //  25   9   0
#define HELIOS_RGB_ORANGE1 (uint32_t)0x331200  //  51  18   0
#define HELIOS_RGB_ORANGE2 (uint32_t)0x4C1B00  //  76  27   0
#define HELIOS_RGB_ORANGE3 (uint32_t)0x662400  // 102  36   0
#define HELIOS_RGB_ORANGE4 (uint32_t)0x7F2D00  // 127  45   0
#define HELIOS_RGB_ORANGE5 (uint32_t)0x993600  // 153  54   0
#define HELIOS_RGB_ORANGE6 (uint32_t)0xB33F00  // 179  63   0
#define HELIOS_RGB_ORANGE7 (uint32_t)0xCC4800  // 204  72   0
#define HELIOS_RGB_ORANGE8 (uint32_t)0xE65100  // 230  81   0
#define HELIOS_RGB_ORANGE9 (uint32_t)0xFF3C00  // 255  60   0

// Helios Yellow Variations
#define HELIOS_RGB_YELLOW0 (uint32_t)0x191300  //  25  19   0
#define HELIOS_RGB_YELLOW1 (uint32_t)0x332600  //  51  38   0
#define HELIOS_RGB_YELLOW2 (uint32_t)0x4C3900  //  76  57   0
#define HELIOS_RGB_YELLOW3 (uint32_t)0x664C00  // 102  76   0
#define HELIOS_RGB_YELLOW4 (uint32_t)0x7F5F00  // 127  95   0
#define HELIOS_RGB_YELLOW5 (uint32_t)0x997200  // 153 114   0
#define HELIOS_RGB_YELLOW6 (uint32_t)0xB38500  // 179 133   0
#define HELIOS_RGB_YELLOW7 (uint32_t)0xCC9800  // 204 152   0
#define HELIOS_RGB_YELLOW8 (uint32_t)0xE6AB00  // 230 171   0
#define HELIOS_RGB_YELLOW9 (uint32_t)0xFF7800  // 255 120   0

// Helios Lime Green Variations
#define HELIOS_RGB_LIME_GREEN0 (uint32_t)0x0E1900  //  14  25   0
#define HELIOS_RGB_LIME_GREEN1 (uint32_t)0x1C3300  //  28  51   0
#define HELIOS_RGB_LIME_GREEN2 (uint32_t)0x294C00  //  41  76   0
#define HELIOS_RGB_LIME_GREEN3 (uint32_t)0x376600  //  55 102   0
#define HELIOS_RGB_LIME_GREEN4 (uint32_t)0x447F00  //  68 127   0
#define HELIOS_RGB_LIME_GREEN5 (uint32_t)0x529900  //  82 153   0
#define HELIOS_RGB_LIME_GREEN6 (uint32_t)0x5FB300  //  95 179   0
#define HELIOS_RGB_LIME_GREEN7 (uint32_t)0x6DCC00  // 109 204   0
#define HELIOS_RGB_LIME_GREEN8 (uint32_t)0x7AE600  // 122 230   0
#define HELIOS_RGB_LIME_GREEN9 (uint32_t)0x88FF00  // 136 255   0

// Helios Green Variations
#define HELIOS_RGB_GREEN0 (uint32_t)0x001900  //   0  25   0
#define HELIOS_RGB_GREEN1 (uint32_t)0x003300  //   0  51   0
#define HELIOS_RGB_GREEN2 (uint32_t)0x004C00  //   0  76   0
#define HELIOS_RGB_GREEN3 (uint32_t)0x006600  //   0 102   0
#define HELIOS_RGB_GREEN4 (uint32_t)0x007F00  //   0 127   0
#define HELIOS_RGB_GREEN5 (uint32_t)0x009900  //   0 153   0
#define HELIOS_RGB_GREEN6 (uint32_t)0x00B300  //   0 179   0
#define HELIOS_RGB_GREEN7 (uint32_t)0x00CC00  //   0 204   0
#define HELIOS_RGB_GREEN8 (uint32_t)0x00E600  //   0 230   0
#define HELIOS_RGB_GREEN9 (uint32_t)0x00FF00  //   0 255   0

// Helios Seafoam Variations
#define HELIOS_RGB_SEAFOAM0 (uint32_t)0x001919  //   0  25  25
#define HELIOS_RGB_SEAFOAM1 (uint32_t)0x003333  //   0  51  51
#define HELIOS_RGB_SEAFOAM2 (uint32_t)0x004C4C  //   0  76  76
#define HELIOS_RGB_SEAFOAM3 (uint32_t)0x006666  //   0 102 102
#define HELIOS_RGB_SEAFOAM4 (uint32_t)0x007F7F  //   0 127 127
#define HELIOS_RGB_SEAFOAM5 (uint32_t)0x009999  //   0 153 153
#define HELIOS_RGB_SEAFOAM6 (uint32_t)0x00B3B3  //   0 179 179
#define HELIOS_RGB_SEAFOAM7 (uint32_t)0x00CCCC  //   0 204 204
#define HELIOS_RGB_SEAFOAM8 (uint32_t)0x00E6E6  //   0 230 230
#define HELIOS_RGB_SEAFOAM9 (uint32_t)0x00FF3C  //   0 255  60

// Helios Turquoise Variations
#define HELIOS_RGB_TURQUOISE0 (uint32_t)0x001925  //   0  25  37
#define HELIOS_RGB_TURQUOISE1 (uint32_t)0x00334B  //   0  51  75
#define HELIOS_RGB_TURQUOISE2 (uint32_t)0x004C70  //   0  76 112
#define HELIOS_RGB_TURQUOISE3 (uint32_t)0x006696  //   0 102 150
#define HELIOS_RGB_TURQUOISE4 (uint32_t)0x007FBB  //   0 127 187
#define HELIOS_RGB_TURQUOISE5 (uint32_t)0x0099E1  //   0 153 225
#define HELIOS_RGB_TURQUOISE6 (uint32_t)0x00B306  //   0 179  6
#define HELIOS_RGB_TURQUOISE7 (uint32_t)0x00CC2C  //   0 204  44
#define HELIOS_RGB_TURQUOISE8 (uint32_t)0x00E651  //   0 230  81
#define HELIOS_RGB_TURQUOISE9 (uint32_t)0x00FFD1  //   0 255 209

// Helios Ice Blue Variations
#define HELIOS_RGB_ICE_BLUE0 (uint32_t)0x001933  //   0  25  51
#define HELIOS_RGB_ICE_BLUE1 (uint32_t)0x003366  //   0  51 102
#define HELIOS_RGB_ICE_BLUE2 (uint32_t)0x004C99  //   0  76 153
#define HELIOS_RGB_ICE_BLUE3 (uint32_t)0x0066CC  //   0 102 204
#define HELIOS_RGB_ICE_BLUE4 (uint32_t)0x007FFF  //   0 127 255
#define HELIOS_RGB_ICE_BLUE5 (uint32_t)0x0099FF  //   0 153 255
#define HELIOS_RGB_ICE_BLUE6 (uint32_t)0x00B3FF  //   0 179 255
#define HELIOS_RGB_ICE_BLUE7 (uint32_t)0x00CCFF  //   0 204 255
#define HELIOS_RGB_ICE_BLUE8 (uint32_t)0x00E6FF  //   0 230 255
#define HELIOS_RGB_ICE_BLUE9 (uint32_t)0x00A7FF  //   0 167 255

// Helios Light Blue Variations
#define HELIOS_RGB_LIGHT_BLUE0 (uint32_t)0x000919  //   0   9  25
#define HELIOS_RGB_LIGHT_BLUE1 (uint32_t)0x001233  //   0  18  51
#define HELIOS_RGB_LIGHT_BLUE2 (uint32_t)0x001C4C  //   0  28  76
#define HELIOS_RGB_LIGHT_BLUE3 (uint32_t)0x002666  //   0  38 102
#define HELIOS_RGB_LIGHT_BLUE4 (uint32_t)0x00307F  //   0  48 127
#define HELIOS_RGB_LIGHT_BLUE5 (uint32_t)0x003A99  //   0  58 153
#define HELIOS_RGB_LIGHT_BLUE6 (uint32_t)0x0044B3  //   0  68 179
#define HELIOS_RGB_LIGHT_BLUE7 (uint32_t)0x004ECC  //   0  78 204
#define HELIOS_RGB_LIGHT_BLUE8 (uint32_t)0x0058E6  //   0  88 230
#define HELIOS_RGB_LIGHT_BLUE9 (uint32_t)0x0047FF  //   0  71 255

// Helios Blue Variations
#define HELIOS_RGB_BLUE0 (uint32_t)0x000019  //    0   0  25
#define HELIOS_RGB_BLUE1 (uint32_t)0x000033  //    0   0  51
#define HELIOS_RGB_BLUE2 (uint32_t)0x00004C  //    0   0  76
#define HELIOS_RGB_BLUE3 (uint32_t)0x000066  //    0   0 102
#define HELIOS_RGB_BLUE4 (uint32_t)0x00007F  //    0   0 127
#define HELIOS_RGB_BLUE5 (uint32_t)0x000099  //    0   0 153
#define HELIOS_RGB_BLUE6 (uint32_t)0x0000B3  //    0   0 179
#define HELIOS_RGB_BLUE7 (uint32_t)0x0000CC  //    0   0 204
#define HELIOS_RGB_BLUE8 (uint32_t)0x0000E6  //    0   0 230
#define HELIOS_RGB_BLUE9 (uint32_t)0x0000FF  //    0   0 255

// Helios Royal Blue Variations
#define HELIOS_RGB_ROYAL_BLUE0 (uint32_t)0x050019  //    5   0  25
#define HELIOS_RGB_ROYAL_BLUE1 (uint32_t)0x0B0033  //   11   0  51
#define HELIOS_RGB_ROYAL_BLUE2 (uint32_t)0x10004C  //   16   0  76
#define HELIOS_RGB_ROYAL_BLUE3 (uint32_t)0x150066  //   21   0 102
#define HELIOS_RGB_ROYAL_BLUE4 (uint32_t)0x1B007F  //   27   0 127
#define HELIOS_RGB_ROYAL_BLUE5 (uint32_t)0x200099  //   32   0 153
#define HELIOS_RGB_ROYAL_BLUE6 (uint32_t)0x2600B3  //   38   0 179
#define HELIOS_RGB_ROYAL_BLUE7 (uint32_t)0x2B00CC  //   43   0 204
#define HELIOS_RGB_ROYAL_BLUE8 (uint32_t)0x3000E6  //   48   0 230
#define HELIOS_RGB_ROYAL_BLUE9 (uint32_t)0x1D00FF  //   29   0 255

// Helios Purple Variations
#define HELIOS_RGB_PURPLE0 (uint32_t)0x190033  //   25   0  51
#define HELIOS_RGB_PURPLE1 (uint32_t)0x330066  //   51   0 102
#define HELIOS_RGB_PURPLE2 (uint32_t)0x4C0099  //   76   0 153
#define HELIOS_RGB_PURPLE3 (uint32_t)0x6600CC  //  102   0 204
#define HELIOS_RGB_PURPLE4 (uint32_t)0x7F00FF  //  127   0 255
#define HELIOS_RGB_PURPLE5 (uint32_t)0x9900FF  //  153   0 255
#define HELIOS_RGB_PURPLE6 (uint32_t)0xB300FF  //  179   0 255
#define HELIOS_RGB_PURPLE7 (uint32_t)0xCC00FF  //  204   0 255
#define HELIOS_RGB_PURPLE8 (uint32_t)0xE600FF  //  230   0 255
#define HELIOS_RGB_PURPLE9 (uint32_t)0x8300FF  //  131   0 255

// Helios Pink Variations
#define HELIOS_RGB_PINK0 (uint32_t)0x190033  //   25   0  51
#define HELIOS_RGB_PINK1 (uint32_t)0x330066  //   51   0 102
#define HELIOS_RGB_PINK2 (uint32_t)0x4C0099  //   76   0 153
#define HELIOS_RGB_PINK3 (uint32_t)0x6600CC  //  102   0 204
#define HELIOS_RGB_PINK4 (uint32_t)0x7F00FF  //  127   0 255
#define HELIOS_RGB_PINK5 (uint32_t)0x9900FF  //  153   0 255
#define HELIOS_RGB_PINK6 (uint32_t)0xB300FF  //  179   0 255
#define HELIOS_RGB_PINK7 (uint32_t)0xCC00FF  //  204   0 255
#define HELIOS_RGB_PINK8 (uint32_t)0xE600FF  //  230   0 255
#define HELIOS_RGB_PINK9 (uint32_t)0xD200FF  //  210   0 255

// Helios Coral Pink Variations
#define HELIOS_RGB_HOT_PINK0 (uint32_t)0x190025  //   25   0  37
#define HELIOS_RGB_HOT_PINK1 (uint32_t)0x33004B  //   51   0  75
#define HELIOS_RGB_HOT_PINK2 (uint32_t)0x4C0070  //   76   0 112
#define HELIOS_RGB_HOT_PINK3 (uint32_t)0x660096  //  102   0 150
#define HELIOS_RGB_HOT_PINK4 (uint32_t)0x7F00BB  //  127   0 187
#define HELIOS_RGB_HOT_PINK5 (uint32_t)0x9900E1  //  153   0 225
#define HELIOS_RGB_HOT_PINK6 (uint32_t)0xB300FF  //  179   0 255
#define HELIOS_RGB_HOT_PINK7 (uint32_t)0xCC00FF  //  204   0 255
#define HELIOS_RGB_HOT_PINK8 (uint32_t)0xE600FF  //  230   0 255
#define HELIOS_RGB_HOT_PINK9 (uint32_t)0xFF00B4  //  255   0 180

// Helios Magenta Variations
#define HELIOS_RGB_MAGENTA0 (uint32_t)0x19000E  //   25   0  14
#define HELIOS_RGB_MAGENTA1 (uint32_t)0x33001C  //   51   0  28
#define HELIOS_RGB_MAGENTA2 (uint32_t)0x4C0029  //   76   0  41
#define HELIOS_RGB_MAGENTA3 (uint32_t)0x660037  //  102   0  55
#define HELIOS_RGB_MAGENTA4 (uint32_t)0x7F0044  //  127   0  68
#define HELIOS_RGB_MAGENTA5 (uint32_t)0x990052  //  153   0  82
#define HELIOS_RGB_MAGENTA6 (uint32_t)0xB3005F  //  179   0  95
#define HELIOS_RGB_MAGENTA7 (uint32_t)0xCC006D  //  204   0 109
#define HELIOS_RGB_MAGENTA8 (uint32_t)0xE6007A  //  230   0 122
#define HELIOS_RGB_MAGENTA9 (uint32_t)0xFF003C  //  255   0  60
