#include <stdint.h>

#include "HeliosConfig.h"
#include "Colorset.h"
#include "Pattern.h"

class Helios
{
public:
  static bool init();
  static void tick();

  static void enter_sleep();
  static void wakeup();

  static bool keep_going() { return keepgoing; }
  static void terminate() { keepgoing = false; }

  static bool is_asleep() { return sleeping; }

private:
  static void clear_output_pins();

  static void handle_state();
  static void handle_state_modes();
  static void handle_state_col_select();
  static bool handle_state_col_select_slot();
  static bool handle_state_col_select_quadrant();
  static bool handle_state_col_select_hue();
  static bool handle_state_col_select_sat();
  static bool handle_state_col_select_val();
  static void handle_state_pat_select();
  static void handle_state_conjure_mode();
  static void next_mode();

  static void set_default(uint8_t default_num);

  enum State : uint8_t {
    STATE_MODES,
    STATE_COLOR_SELECT_SLOT,
    STATE_COLOR_SELECT_QUADRANT,
    STATE_COLOR_SELECT_HUE,
    STATE_COLOR_SELECT_SAT,
    STATE_COLOR_SELECT_VAL,
    STATE_PATTERN_SELECT,
    STATE_CONJURE_MODE,
#ifdef HELIOS_CLI
    STATE_SLEEP,
#endif
  };

  enum Flags : uint8_t {
    FLAG_NONE = 0,
    FLAG_LOCKED = (1 << 0),
    FLAG_CONJURE = (1 << 1),
  };

  // get/set global flags
  static void set_flag(Flags flag) { global_flags = (Flags)(global_flags | flag); }
  static bool has_flag(Flags flag) { return (global_flags & flag) != 0; }
  static void clear_flag(Flags flag) { global_flags = (Flags)(global_flags & ~flag); }
  static void toggle_flag(Flags flag) { global_flags = (Flags)(global_flags ^ flag); }

  // the current state of the system
  static State cur_state;
  // global flags for the entire system
  static Flags global_flags;
  static uint8_t menu_selection;
  static uint8_t cur_mode;
  // the quadrant that was selected in color select
  static uint8_t selected_slot;
  static uint8_t selected_base_hue;
  static uint8_t selected_hue;
  static uint8_t selected_sat;
  static PatternArgs default_args[6];
  static Colorset default_colorsets[6];
  static Pattern pat;
  static bool keepgoing;
  static bool sleeping;
};
