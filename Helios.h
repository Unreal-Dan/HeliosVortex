#include <stdint.h>

#include "Pattern.h"

class Helios
{
public:
  static void init();
  static void tick();

  static void enterSleep(bool save);

  static bool keepGoing() { return keepgoing; }
  static void terminate() { keepgoing = false; }
private:
  static void handle_state();
  static void handle_state_modes();
  static void handle_state_col_select();
  static void handle_state_pat_select();
  static void handle_state_fac_reset();
  static void next_mode();

  enum State {
    STATE_MODES,
    STATE_COLOR_SELECT_HUE,
    STATE_COLOR_SELECT_SAT,
    STATE_COLOR_SELECT_VAL,
    STATE_PATTERN_SELECT,
    STATE_FACTORY_RESET
  };

  static State cur_state;
  static uint8_t menu_selection;
  static uint8_t cur_mode;
  static Pattern pat;
  static bool keepgoing;
  static bool sleeping;
};
