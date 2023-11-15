#include <stdint.h>

#include "HeliosConfig.h"
#include "Colorset.h"
#include "Pattern.h"

class Helios
{
public:
  static bool init();
  static void tick();

  static void enterSleep(bool save);

  static bool keepGoing() { return keepgoing; }
  static void terminate() { keepgoing = false; }
private:
  static void handle_state();
  static void handle_state_modes();
  static void handle_state_col_select();
  static void handle_state_pat_select();
  static void handle_state_conjure_mode();
  static void next_mode();

  static void set_default(uint8_t default_num);

  enum State {
    STATE_MODES,
    STATE_COLOR_SELECT_HUE,
    STATE_COLOR_SELECT_SAT,
    STATE_COLOR_SELECT_VAL,
    STATE_PATTERN_SELECT,
    STATE_CONJURE_MODE,
    STATE_SLEEP
  };

  static State cur_state;
  static uint8_t menu_selection;
  static uint8_t cur_mode;
  static PatternArgs default_args[6];
  static Colorset default_colorsets[6];
  static Pattern pat;
  static bool keepgoing;
  static bool sleeping;
};
