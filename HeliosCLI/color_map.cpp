#include <map>
#include <string>

#include "ColorConstants.h"
#include "color_map.h"

// map of colors for the -C, --colorset parsing
std::map<std::string, uint32_t> color_map = {
  {"off", RGB_OFF},
  {"white", RGB_WHITE},
  {"red", RGB_RED},
  {"coralorange", RGB_CORAL_ORANGE},
  {"orange", RGB_ORANGE},
  {"yellow", RGB_YELLOW},
  {"limegreen", RGB_LIME_GREEN},
  {"green", RGB_GREEN},
  {"seafoam", RGB_SEAFOAM},
  {"turquoise", RGB_TURQUOISE},
  {"iceblue", RGB_ICE_BLUE},
  {"lightblue", RGB_LIGHT_BLUE},
  {"blue", RGB_BLUE},
  {"royalblue", RGB_ROYAL_BLUE},
  {"purple", RGB_PURPLE},
  {"pink", RGB_PINK},
  {"hotpink", RGB_HOT_PINK},
  {"magenta", RGB_MAGENTA},
  {"whitebrimedium", RGB_WHITE_BRI_MEDIUM},
  {"redbrimedium", RGB_RED_BRI_MEDIUM},
  {"coralorangebrimedium", RGB_CORAL_ORANGE_BRI_MEDIUM},
  {"orangebrimedium", RGB_ORANGE_BRI_MEDIUM},
  {"yellowbrimedium", RGB_YELLOW_BRI_MEDIUM},
  {"limegreenbrimedium", RGB_LIME_GREEN_BRI_MEDIUM},
  {"greenbrimedium", RGB_GREEN_BRI_MEDIUM},
  {"seafoambrimedium", RGB_SEAFOAM_BRI_MEDIUM},
  {"turquoisebrimedium", RGB_TURQUOISE_BRI_MEDIUM},
  {"icebluebrimedium", RGB_ICE_BLUE_BRI_MEDIUM},
  {"lightbluebrimedium", RGB_LIGHT_BLUE_BRI_MEDIUM},
  {"bluebrimedium", RGB_BLUE_BRI_MEDIUM},
  {"royalbluebrimedium", RGB_ROYAL_BLUE_BRI_MEDIUM},
  {"purplebrimedium", RGB_PURPLE_BRI_MEDIUM},
  {"pinkbrimedium", RGB_PINK_BRI_MEDIUM},
  {"hotpinkbrimedium", RGB_HOT_PINK_BRI_MEDIUM},
  {"magentabrimedium", RGB_MAGENTA_BRI_MEDIUM},
  {"whitebrilow", RGB_WHITE_BRI_LOW},
  {"redbrilow", RGB_RED_BRI_LOW},
  {"coralorangebrilow", RGB_CORAL_ORANGE_BRI_LOW},
  {"orangebrilow", RGB_ORANGE_BRI_LOW},
  {"yellowbrilow", RGB_YELLOW_BRI_LOW},
  {"limegreenbrilow", RGB_LIME_GREEN_BRI_LOW},
  {"greenbrilow", RGB_GREEN_BRI_LOW},
  {"seafoambrilow", RGB_SEAFOAM_BRI_LOW},
  {"turquoisebrilow", RGB_TURQUOISE_BRI_LOW},
  {"icebluebrilow", RGB_ICE_BLUE_BRI_LOW},
  {"lightbluebrilow", RGB_LIGHT_BLUE_BRI_LOW},
  {"bluebrilow", RGB_BLUE_BRI_LOW},
  {"royalbluebrilow", RGB_ROYAL_BLUE_BRI_LOW},
  {"purplebrilow", RGB_PURPLE_BRI_LOW},
  {"pinkbrilow", RGB_PINK_BRI_LOW},
  {"hotpinkbrilow", RGB_HOT_PINK_BRI_LOW},
  {"magentabrilow", RGB_MAGENTA_BRI_LOW},
  {"whitebrilowest", RGB_WHITE_BRI_LOWEST},
  {"redbrilowest", RGB_RED_BRI_LOWEST},
  {"coralorangebrilowest", RGB_CORAL_ORANGE_BRI_LOWEST},
  {"orangebrilowest", RGB_ORANGE_BRI_LOWEST},
  {"yellowbrilowest", RGB_YELLOW_BRI_LOWEST},
  {"limegreenbrilowest", RGB_LIME_GREEN_BRI_LOWEST},
  {"greenbrilowest", RGB_GREEN_BRI_LOWEST},
  {"seafoambrilowest", RGB_SEAFOAM_BRI_LOWEST},
  {"turquoisebrilowest", RGB_TURQUOISE_BRI_LOWEST},
  {"icebluebrilowest", RGB_ICE_BLUE_BRI_LOWEST},
  {"lightbluebrilowest", RGB_LIGHT_BLUE_BRI_LOWEST},
  {"bluebrilowest", RGB_BLUE_BRI_LOWEST},
  {"royalbluebrilowest", RGB_ROYAL_BLUE_BRI_LOWEST},
  {"purplebrilowest", RGB_PURPLE_BRI_LOWEST},
  {"pinkbrilowest", RGB_PINK_BRI_LOWEST},
  {"hotpinkbrilowest", RGB_HOT_PINK_BRI_LOWEST},
  {"magentabrilowest", RGB_MAGENTA_BRI_LOWEST},
  {"redsatmedium", RGB_RED_SAT_MEDIUM},
  {"coralorangesatmedium", RGB_CORAL_ORANGE_SAT_MEDIUM},
  {"orangesatmedium", RGB_ORANGE_SAT_MEDIUM},
  {"yellowsatmedium", RGB_YELLOW_SAT_MEDIUM},
  {"limegreensatmedium", RGB_LIME_GREEN_SAT_MEDIUM},
  {"greensatmedium", RGB_GREEN_SAT_MEDIUM},
  {"seafoamsatmedium", RGB_SEAFOAM_SAT_MEDIUM},
  {"turquoisesatmedium", RGB_TURQUOISE_SAT_MEDIUM},
  {"icebluesatmedium", RGB_ICE_BLUE_SAT_MEDIUM},
  {"lightbluesatmedium", RGB_LIGHT_BLUE_SAT_MEDIUM},
  {"bluesatmedium", RGB_BLUE_SAT_MEDIUM},
  {"royalbluesatmedium", RGB_ROYAL_BLUE_SAT_MEDIUM},
  {"purplesatmedium", RGB_PURPLE_SAT_MEDIUM},
  {"pinksatmedium", RGB_PINK_SAT_MEDIUM},
  {"hotpinksatmedium", RGB_HOT_PINK_SAT_MEDIUM},
  {"magentasatmedium", RGB_MAGENTA_SAT_MEDIUM},
  {"redsatlow", RGB_RED_SAT_LOW},
  {"coralorangesatlow", RGB_CORAL_ORANGE_SAT_LOW},
  {"orangesatlow", RGB_ORANGE_SAT_LOW},
  {"yellowsatlow", RGB_YELLOW_SAT_LOW},
  {"limegreensatlow", RGB_LIME_GREEN_SAT_LOW},
  {"greensatlow", RGB_GREEN_SAT_LOW},
  {"seafoamsatlow", RGB_SEAFOAM_SAT_LOW},
  {"turquoisesatlow", RGB_TURQUOISE_SAT_LOW},
  {"icebluesatlow", RGB_ICE_BLUE_SAT_LOW},
  {"lightbluesatlow", RGB_LIGHT_BLUE_SAT_LOW},
  {"bluesatlow", RGB_BLUE_SAT_LOW},
  {"royalbluesatlow", RGB_ROYAL_BLUE_SAT_LOW},
  {"purplesatlow", RGB_PURPLE_SAT_LOW},
  {"pinksatlow", RGB_PINK_SAT_LOW},
  {"hotpinksatlow", RGB_HOT_PINK_SAT_LOW},
  {"magentasatlow", RGB_MAGENTA_SAT_LOW},
  {"redsatlowest", RGB_RED_SAT_LOWEST},
  {"coralorangesatlowest", RGB_CORAL_ORANGE_SAT_LOWEST},
  {"orangesatlowest", RGB_ORANGE_SAT_LOWEST},
  {"yellowsatlowest", RGB_YELLOW_SAT_LOWEST},
  {"limegreensatlowest", RGB_LIME_GREEN_SAT_LOWEST},
  {"greensatlowest", RGB_GREEN_SAT_LOWEST},
  {"seafoamsatlowest", RGB_SEAFOAM_SAT_LOWEST},
  {"turquoisesatlowest", RGB_TURQUOISE_SAT_LOWEST},
  {"icebluesatlowest", RGB_ICE_BLUE_SAT_LOWEST},
  {"lightbluesatlowest", RGB_LIGHT_BLUE_SAT_LOWEST},
  {"bluesatlowest", RGB_BLUE_SAT_LOWEST},
  {"royalbluesatlowest", RGB_ROYAL_BLUE_SAT_LOWEST},
  {"purplesatlowest", RGB_PURPLE_SAT_LOWEST},
  {"pinksatlowest", RGB_PINK_SAT_LOWEST},
  {"hotpinksatlowest", RGB_HOT_PINK_SAT_LOWEST},
  {"magentasatlowest", RGB_MAGENTA_SAT_LOWEST}
};