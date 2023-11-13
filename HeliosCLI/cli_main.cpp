#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <getopt.h>

#include <string>

#include "Button.h"
#include "Led.h"

using namespace std;

// the output types of the tool
enum OutputType {
  OUTPUT_TYPE_NONE,
  OUTPUT_TYPE_HEX,
  OUTPUT_TYPE_COLOR,
};

// various globals for the tool
OutputType output_type = OUTPUT_TYPE_COLOR;
bool in_place = false;
bool lockstep = false;
bool storage = false;

// continue running
bool keepgoing = true;

// used to switch terminal to non-blocking and back
static struct termios orig_term_attr = {0};

// from the other main.cpp
void setup();
void loop();

// internal functions
static void parse_options(int argc, char *argv[]);
static bool read_inputs();
static void show();
static void restore_terminal();
static void set_terminal_nonblocking();
static void print_usage(const char* program_name);

int main(int argc, char *argv[])
{
  // parse command line options
  parse_options(argc, argv);
  // set the terminal to instantly receive key presses
  set_terminal_nonblocking();
  // run the arduino setup routine
  setup();
  // loop forever and run the tick routine and other main logic
  uint32_t tick = 0;
  while (keepgoing) {
    // check for any inputs and read the next one
    if (!read_inputs()) {
      // if no inputs were read, then if lockstep is enabled
      if (lockstep) {
        // just keep waiting for an input
        continue;
      }
    }
    // run the main loop
    loop();
    // render the output of the main loop
    show();
    // iterate tickcount
    tick++;
  }
  return 0;
}

// parse the command line options into global flags
static void parse_options(int argc, char *argv[])
{
  int opt = -1;
  int option_index = 0;
  static struct option long_options[] = {
    {"hex", no_argument, nullptr, 'x'},
    {"color", no_argument, nullptr, 'c'},
    {"lockstep", no_argument, nullptr, 'l'},
    {"in-place", no_argument, nullptr, 'i'},
    {"storage", optional_argument, nullptr, 's'},
    {"help", no_argument, nullptr, 'h'},
    {nullptr, 0, nullptr, 0}
  };
  while ((opt = getopt_long(argc, argv, "xctliransP:C:A:h", long_options, &option_index)) != -1) {
    switch (opt) {
    case 'x':
      // if the user wants pretty colors or hex codes
      output_type = OUTPUT_TYPE_HEX;
      break;
    case 'c':
      // if the user wants pretty colors
      output_type = OUTPUT_TYPE_COLOR;
      break;
    case 'l':
      // if the user wants to step in lockstep with the engine
      lockstep = true;
      break;
    case 'i':
      // if the user wants to print in-place (on one line)
      in_place = true;
      break;
    case 's':
      // TODO: enable persistent storage to file?
      storage = true;
      break;
    case 'h':
      // print usage and exit
      print_usage(argv[0]);
      exit(EXIT_SUCCESS);
    default: // '?' for unrecognized options
      printf("Unknown arg: -%c\n", opt);
      exit(EXIT_FAILURE);
    }
  }
}

// read the input from stdin to control the tool
static bool read_inputs()
{
  // keep track of the number of inputs and only process
  // one input per tick
  static uint32_t numInputs = 0;
  // check for any inputs on stdin if theres no inputs left
  if (!numInputs) {
    // this will capture the number of characters on stdin
    ioctl(STDIN_FILENO, FIONREAD, &numInputs);
    // if lockstep mode and there's no new input, don't run a tick
    if (!numInputs) {
      // just wait for input
      return false;
    }
  }
  // otherwise process the next input if there is one
  if (numInputs > 0) {
    numInputs--;
    char command = getchar();
    if (command == 'q') {
      keepgoing = false;
      return true;
    }
    // otherwise just queue up the command
    button.queueInput(command);
  }
  return true;
}

// render the led
static void show()
{
  string out;
  if (in_place) {
    // this resets the cursor back to the beginning of the line and moves it up 1 line
    out += "\33[2K\033[1A\r";
  }
  if (output_type == OUTPUT_TYPE_COLOR) {
    out += "\x1B[0m["; // opening |
    out += "\x1B[48;2;"; // colorcode start
    out += to_string(Led::get().red) + ";"; // col red
    out += to_string(Led::get().green) + ";"; // col green
    out += to_string(Led::get().blue) + "m"; // col blue
    out += "  "; // colored space
    out += "\x1B[0m]"; // ending |
  } else if (output_type == OUTPUT_TYPE_HEX) {
    // otherwise this just prints out the raw hex code if not in color mode
    for (uint32_t i = 0; i < output_type; ++i) {
      char buf[128] = { 0 };
      snprintf(buf, sizeof(buf), "%02X%02X%02X", Led::get().red, Led::get().green, Led::get().blue);
      out += buf;
    }
  } else { // OUTPUT_TYPE_NONE
           // do nothing
  }
  if (!in_place) {
    out += "\n";
  }
  printf("%s", out.c_str());
  fflush(stdout);
}

// installed as an automatic exit handler to restore terminal behaviour
static void restore_terminal()
{
  tcsetattr(STDIN_FILENO, TCSANOW, &orig_term_attr);
}

// switch terminal to non-blocking so input can be captured instantly, and
// automatically install an atexit handler to restore the terminal behaviour
static void set_terminal_nonblocking()
{
  struct termios term_attr = {0};
  // Get the current terminal attributes and store them
  tcgetattr(STDIN_FILENO, &term_attr);
  orig_term_attr = term_attr;
  // Set the terminal in non-canonical mode (raw mode)
  term_attr.c_lflag &= ~(ICANON | ECHO);
  // Set the minimum number of input bytes read at a time to 1
  term_attr.c_cc[VMIN] = 1;
  // Set the timeout for read to 0 (no waiting)
  term_attr.c_cc[VTIME] = 0;
  // Apply the new terminal attributes
  tcsetattr(STDIN_FILENO, TCSANOW, &term_attr);
  // Set the terminal to non-blocking mode
  int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
  fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
  // Register the restore_terminal function to be called at exit
  atexit(restore_terminal);
}

// print out the usage for the tool
static void print_usage(const char* program_name)
{
  fprintf(stderr, "Usage: %s [options] < input commands\n", program_name);
  fprintf(stderr, "Output Selection (at least one required):\n");
  fprintf(stderr, "  -x, --hex                Use hex values to represent led colors\n");
  fprintf(stderr, "  -c, --color              Use console color codes to represent led colors\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Engine Control Flags (optional):\n");
  fprintf(stderr, "  -l, --lockstep           Only step once each time an input is received\n");
  fprintf(stderr, "  -i, --in-place           Print the output in-place (interactive mode)\n");
  fprintf(stderr, "  -s, --storage [file]     Persistent storage to file (default file: FlashStorage.flash)\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Other Options:\n");
  fprintf(stderr, "  -h, --help               Display this help message\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Input Commands (pass to stdin):");
  // the usage for the input strings
  const char *input_usage[] = {
    "\n   c         standard short click",
    "\n   l         standard long click",
    "\n   w         wait 1 tick",
    "\n   q         quit",
  };
  for (uint32_t i = 0; i < (sizeof(input_usage) / sizeof(input_usage[0])); ++i) {
    fprintf(stderr, "%s", input_usage[i]);
  }
  fprintf(stderr, "\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Example Usage:\n");
  fprintf(stderr, "   ./helios\n");
  fprintf(stderr, "   ./helios -ci\n");
  fprintf(stderr, "   ./helios -cl <<< wwwwwwwq\n");
}
