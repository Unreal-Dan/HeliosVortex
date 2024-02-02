#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <getopt.h>

#include <string>
#include <vector>
#include <fstream>
#include <cstdint>

#include "Helios.h"
#include "TimeControl.h"
#include "Storage.h"
#include "Button.h"
#include "Led.h"

#define SCALE8(i, scale)  (((uint16_t)i * (uint16_t)(scale)) >> 8)

using namespace std;
std::vector<RGBColor> colorBuffer;

RGBColor scaleUpBrightness(const RGBColor& color, float scaleFactor) {
  RGBColor scaledColor;
  scaledColor.red = std::min(static_cast<int>(color.red * scaleFactor), 255);
  scaledColor.green = std::min(static_cast<int>(color.green * scaleFactor), 255);
  scaledColor.blue = std::min(static_cast<int>(color.blue * scaleFactor), 255);
  return scaledColor;
}

// the output types of the tool
enum OutputType {
  OUTPUT_TYPE_NONE,
  OUTPUT_TYPE_HEX,
  OUTPUT_TYPE_COLOR,
  OUTPUT_TYPE_BMP,
};

// various globals for the tool
OutputType output_type = OUTPUT_TYPE_COLOR;
bool in_place = false;
bool lockstep = false;
bool storage = false;
bool timestep = true;
bool eeprom = false;
bool isRecording = false;

// used to switch terminal to non-blocking and back
static struct termios orig_term_attr = {0};

// internal functions
static void parse_options(int argc, char *argv[]);
static bool read_inputs();
static void show();
static void restore_terminal();
static void set_terminal_nonblocking();
static void writeBMP(const std::string& filename, const std::vector<RGBColor>& colors);
static void print_usage(const char* program_name);

int main(int argc, char *argv[])
{
  // parse command line options
  parse_options(argc, argv);
  // set the terminal to instantly receive key presses
  set_terminal_nonblocking();
  // toggle timestep in the engine based on the cli input
  Time::enableTimestep(timestep);
  // toggle storage in the engine based on cli input
  Storage::enableStorage(storage);
  // run the arduino setup routine
  Helios::init();
  // just generate eeprom?
  if (eeprom) {
    return 0;
  }
  while (Helios::keep_going()) {
    // check for any inputs and read the next one
    read_inputs();
    // if lockstep is enabled, only run logic if the
    // input queue isn't actually empty
    if (lockstep && !Button::inputQueueSize()) {
      // just keep waiting for an input
      continue;
    }
    // run the main loop
    Helios::tick();
    // don't render anything if asleep, but technically it's still running...
    if (Helios::is_asleep()) {
      continue;
    }
    // render the output of the main loop
    show();
  }
  // Write remaining colors in buffer to BMP file before exiting
  if (!colorBuffer.empty()) {
    cout << "Writing " << colorBuffer.size() << " colors to BMP." << endl;
    writeBMP("pattern_output.bmp", colorBuffer);
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
    {"bmp", no_argument, nullptr, 'b'},
    {"color", no_argument, nullptr, 'c'},
    {"quiet", no_argument, nullptr, 'q'},
    {"lockstep", no_argument, nullptr, 'l'},
    {"no-timestep", no_argument, nullptr, 't'},
    {"in-place", no_argument, nullptr, 'i'},
    {"no-storage", no_argument, nullptr, 's'},
    {"eeprom", no_argument, nullptr, 'E'},
    {"help", no_argument, nullptr, 'h'},
    {nullptr, 0, nullptr, 0}
  };
  while ((opt = getopt_long(argc, argv, "xbcqtliransP:C:A:h", long_options, &option_index)) != -1) {
    switch (opt) {
    case 'b':
      // if the user wants pretty colors or hex codes
      output_type = OUTPUT_TYPE_BMP;
      break;
    case 'x':
      // if the user wants pretty colors or hex codes
      output_type = OUTPUT_TYPE_HEX;
      break;
    case 'c':
      // if the user wants pretty colors
      output_type = OUTPUT_TYPE_COLOR;
      break;
    case 'q':
      output_type = OUTPUT_TYPE_NONE;
      break;
    case 'l':
      // if the user wants to step in lockstep with the engine
      lockstep = true;
      break;
    case 't':
      // turn off timestep
      timestep = false;
      break;
    case 'i':
      // if the user wants to print in-place (on one line)
      in_place = true;
      break;
    case 's':
      // TODO: implement storage filename
      storage = false;
      break;
    case 'E':
      eeprom = true;
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
    if (!numInputs) {
      return false;
    }
  }
  // otherwise process the next input if there is one
  if (numInputs > 0) {
    numInputs--;
    char command = getchar();
    uint32_t repeatAmount = 1;
    if (isdigit(command)) {
      repeatAmount = command - '0';
      char newc = 0;
      // read the digits into the repeatAmount
      while (1) {
        numInputs--;
        newc = getchar();
        if (!isdigit(newc)) {
          // stop once we reach a non digit
          break;
        }
        // accumulate the digits into the repeat amount
        repeatAmount = (repeatAmount * 10) + (newc - '0');
      }
      command = newc;
    }
    for (uint32_t i = 0; i < repeatAmount; ++i) {
      // otherwise just queue up the command
      Button::queueInput(command);
    }
  }
  return true;
}

// render the led
static void show()
{
  if (output_type == OUTPUT_TYPE_NONE) {
    return;
  }
  string out;
  if (in_place) {
    // this resets the cursor back to the beginning of the line
    out += "\r";
  }
  // Define the scaling factor (e.g., 1 = no scaling up 30 = scale up 100%)
  float scaleFactor = 10;

  // Get the current color and scale its brightness up
  RGBColor currentColor = {Led::get().red, Led::get().green, Led::get().blue};
  RGBColor scaledColor = scaleUpBrightness(currentColor, scaleFactor);
  if (output_type == OUTPUT_TYPE_COLOR) {
    out += "\x1B[0m["; // opening |
    out += "\x1B[48;2;"; // colorcode start
    out += to_string(scaledColor.red) + ";"; // col red
    out += to_string(scaledColor.green) + ";"; // col green
    out += to_string(scaledColor.blue) + "m"; // col blue
    out += "  "; // colored space
    out += "\x1B[0m]"; // ending |
  } else if (output_type == OUTPUT_TYPE_HEX) {
    // otherwise this just prints out the raw hex code if not in color mode
    for (uint32_t i = 0; i < output_type; ++i) {
      char buf[128] = { 0 };
      snprintf(buf, sizeof(buf), "%02X%02X%02X", scaledColor.red, scaledColor.green, scaledColor.blue);
      out += buf;
    }
  } else if (output_type == OUTPUT_TYPE_BMP) {
    // Output color to console
    out += "\x1B[0m["; // opening |
    out += "\x1B[48;2;"; // colorcode start
    out += to_string(scaledColor.red) + ";"; // col red
    out += to_string(scaledColor.green) + ";"; // col green
    out += to_string(scaledColor.blue) + "m"; // col blue
    out += "  "; // colored space
    out += "\x1B[0m]"; // ending |
    // In the 'show' function, apply scaling before adding to buffer
    if (output_type == OUTPUT_TYPE_BMP && isRecording) {


      // Add scaled color to buffer
      colorBuffer.push_back(scaledColor);
    }
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

static void writeBMP(const std::string& filename, const std::vector<RGBColor>& colors) {
  int32_t width = colors.size();
  int32_t height = 1; // Each color is a pixel in a 1-row image
  uint32_t rowSize = width * 3 + width % 4;
  uint32_t fileSize = 54 + rowSize * height;

  std::ofstream file(filename, std::ios::out | std::ios::binary);

  // BMP Header
  file.put('B').put('M'); // Signature
  file.write(reinterpret_cast<const char*>(&fileSize), 4); // File size
  file.write("\0\0\0\0", 4); // Reserved
  file.write("\x36\0\0\0", 4); // Pixel data offset

  // DIB Header
  file.write("\x28\0\0\0", 4); // DIB Header size
  file.write(reinterpret_cast<const char*>(&width), 4); // Width
  file.write(reinterpret_cast<const char*>(&height), 4); // Height
  file.write("\x01\0", 2); // Planes
  file.write("\x18\0", 2); // Bits per pixel (24 for RGB)
  file.write("\0\0\0\0", 4); // Compression (none)
  file.write("\0\0\0\0", 4); // Image size (can be 0 for uncompressed)
  file.write("\x13\0\0\0", 4); // Horizontal resolution (px/m, just a placeholder value)
  file.write("\x13\0\0\0", 4); // Vertical resolution (px/m, placeholder value)
  file.write("\0\0\0\0", 4); // Colors in color table (none for RGB)
  file.write("\0\0\0\0", 4); // Important color count (all)

  // Pixel Data
  for (const auto& color : colors) {
    file.put(color.blue).put(color.green).put(color.red);
  }
  // Padding for each row (BMP rows are aligned to 4-byte boundaries)
  for (int i = 0; i < width % 4; ++i) {
    file.put(0);
  }

  file.close();
}

// print out the usage for the tool
static void print_usage(const char* program_name)
{
  fprintf(stderr, "Usage: %s [options] < input commands\n", program_name);
  fprintf(stderr, "Output Selection (at least one required):\n");
  fprintf(stderr, "  -b, --bmp                Generates a bmp of the colors\n");
  fprintf(stderr, "  -x, --hex                Use hex values to represent led colors\n");
  fprintf(stderr, "  -c, --color              Use console color codes to represent led colors\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Engine Control Flags (optional):\n");
  fprintf(stderr, "  -l, --lockstep           Only step once each time an input is received\n");
  fprintf(stderr, "  -t, --no-timestep        Run as fast as possible without managing timestep\n");
  fprintf(stderr, "  -i, --in-place           Print the output in-place (interactive mode)\n");
  fprintf(stderr, "  -s, --no-storage         Disable persistent storage to file (FlashStorage.flash)\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Other Options:\n");
  fprintf(stderr, "  -E, --eeprom             Generate an eeprom file for flashing\n");
  fprintf(stderr, "  -h, --help               Display this help message\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Input Commands (pass to stdin):");
  // the usage for the input strings
  const char *input_usage[] = {
    "\n   c         standard short click",
    "\n   l         standard long click",
    "\n   p         press the button and hold",
    "\n   r         release the button from hold",
    "\n   t         toggle button press state",
    "\n   s         start recording",
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
  fprintf(stderr, "   ./helios -cl <<< 300wcw300wcp1500wr300wq\n");
}