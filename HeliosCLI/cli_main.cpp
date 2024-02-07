#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <getopt.h>

#include <string>
#include <vector>
#include <fstream>
#include <cstdint>
#include <sstream>
#include <algorithm>
#include <map>

#include "Helios.h"
#include "TimeControl.h"
#include "Storage.h"
#include "Colortypes.h"
#include "Button.h"
#include "Led.h"
#include "color_map.h"

/*
 * TODO still:
 * 3. look at making a script to record output and create images
 * 5. maybe??? add options to generate circle instead of line?
 * 6. Maybe use imagemagic to generate circle? instead of C++?
 */

// the output types of the tool
enum OutputType {
  OUTPUT_TYPE_NONE,
  OUTPUT_TYPE_HEX,
  OUTPUT_TYPE_COLOR,
};

// the default bmp filename
#define DEFAULT_BMP_FILENAME "pattern.bmp"

// various globals for the tool
OutputType output_type = OUTPUT_TYPE_COLOR;
std::string bmp_filename = DEFAULT_BMP_FILENAME;
bool in_place = false;
bool lockstep = false;
bool storage = false;
bool timestep = true;
bool eeprom = false;
bool generate_bmp = false;
std::vector<RGBColor> colorBuffer;
uint32_t num_cycles = 0;
float brightness_scale = 1.0f;
uint8_t minumum_brightness = 75;
std::string initial_colorset_str = "";
std::string initial_pattern_str = "";
uint32_t initial_mode_index = 0;

// used to switch terminal to non-blocking and back
static struct termios orig_term_attr = {0};

// internal functions
static void parse_options(int argc, char *argv[]);
static bool read_inputs();
static void show();
static void restore_terminal();
static void set_terminal_nonblocking();
static bool writeBMP(const std::string& filename, const std::vector<RGBColor>& colors);
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
  // run the engine initialization
  Helios::init();
  // set the initial mode index
  Helios::set_mode_index(initial_mode_index);
  // Set the initial pattern based on user arguments
  if (initial_pattern_str.length() > 0) {
    // convert the string arg to integer, then treat it as a PatternID
    PatternID id = (PatternID)strtoul(initial_pattern_str.c_str(), NULL, 10);
    // pass the current pattern to make_pattern to update it's internals
    Patterns::make_pattern(id, Helios::cur_pattern());
    // re-initialize the current pattern
    Helios::cur_pattern().init();
  }
  // Set the initial colorset based on user arguments
  if (initial_colorset_str.length() > 0) {
    std::stringstream ss(initial_colorset_str);
    std::string color;
    Colorset set;
    while (getline(ss, color, ',')) {
      // iterate letters and lowercase them
      std::transform(color.begin(), color.end(), color.begin(), [](unsigned char c){ return tolower(c); });
      if (color_map.count(color) > 0) {
        set.addColor(color_map[color]);
      } else {
        set.addColor(strtoul(color.c_str(), nullptr, 16));
      }
    }
    // update the colorset of the current pattern
    Helios::cur_pattern().setColorset(set);
    // re-initialize the current pattern
    Helios::cur_pattern().init();
  }
  // just generate eeprom?
  if (eeprom) {
    return 0;
  }
  // keep track of the number of cycles and the last colorset index each tick
  // so that we can detect when one full cycle of the pattern has passed
  uint32_t cycle_count = 0;
  uint8_t last_index = 0;
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
    // watch for a full cycle if it was requested by the command line
    if (num_cycles > 0) {
      // grab the current index of the colorset, which might be the same for
      // several tick in a row, so we must check whether it just changed this
      // tick by comparing it to the index we saved last tick
      uint8_t cur_index = Helios::cur_pattern().colorset().curIndex();
      if (cur_index == 0 && last_index != 0) {
        // only if the current index is 0 (start of colorset) and the last index was
        // not 0 then the colorset *just* started iterating through it's colors, so
        // count this as +1 cycle in the cycle count
        cycle_count++;
      }
      // then if we run more than the chosen number of cycles just quit
      if (cycle_count >= num_cycles) {
        Helios::terminate();
        break;
      }
      last_index = cur_index;
    }
    // render the output of the main loop
    show();
  }
  // if the user requested a bmp file to be written
  if (generate_bmp) {
    // if they didn't record anything give them a message indicating they need to record
    if (!colorBuffer.size()) {
      std::cout << "Cannot generate BMP! Color buffer is empty" << std::endl;
      return 0;
    }
    std::cout << "Writing " << colorBuffer.size() << " colors to " << bmp_filename << std::endl;
    // try to write out however many colors they recorded to the bmp file
    if (!writeBMP(bmp_filename.c_str(), colorBuffer)) {
      // non-zero exit code means the utility failed it's job
      return 1;
    }
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
    {"quiet", no_argument, nullptr, 'q'},
    {"lockstep", no_argument, nullptr, 'l'},
    {"no-timestep", no_argument, nullptr, 't'},
    {"in-place", no_argument, nullptr, 'i'},
    {"no-storage", no_argument, nullptr, 's'},
    {"cycle", optional_argument, nullptr, 'y'},
    {"brightness-scale", required_argument, nullptr, 'a'},
    {"min-brightness", required_argument, nullptr, 'm'},
    {"colorset", required_argument, nullptr, 'C'},
    {"pattern", required_argument, nullptr, 'P'},
    {"mode-index", required_argument, nullptr, 'I'},
    {"bmp", optional_argument, nullptr, 'b'},
    {"eeprom", no_argument, nullptr, 'E'},
    {"help", no_argument, nullptr, 'h'},
    {nullptr, 0, nullptr, 0}
  };
  while ((opt = getopt_long(argc, argv, "xcqltisyamC:P:I:b::Eh", long_options, &option_index)) != -1) {
    switch (opt) {
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
    case 'y':
      // set the number of cycles to default 1
      num_cycles = 1;
      // allow for a space between the -y and the cycle count
      if (optarg == NULL && optind < argc && argv[optind][0] != '-') {
        optarg = argv[optind++];
      }
      // if an argument was provided for -y then set it as the number of cycles
      if (optarg) {
        num_cycles = strtoul(optarg, NULL, 10);
      }
      break;
    case 'a':
      // allow for a space between the -y and the cycle count
      if (optarg == NULL && optind < argc && argv[optind][0] != '-') {
        optarg = argv[optind++];
      }
      if (optarg) {
        brightness_scale = strtof(optarg, NULL);
      }
      // allow brightness scale 0? probably not because it's most likely a mistake
      // like wrong arguments and the brightness was passed a string, so just reset
      // the brightness to 1.0 if it parsed as 0
      if (!brightness_scale) {
        brightness_scale = 1.0f;
      }
      break;
    case 'm':
      // allow for a space between the -y and the cycle count
      if (optarg == NULL && optind < argc && argv[optind][0] != '-') {
        optarg = argv[optind++];
      }
      if (optarg) {
        minumum_brightness = strtoul(optarg, NULL, 100);
      }
      // allow brightness scale 0? probably not because it's most likely a mistake
      // like wrong arguments and the brightness was passed a string, so just reset
      // the brightness to 1.0 if it parsed as 0
      if (!minumum_brightness) {
        minumum_brightness = 75;
      }
      break;
    case 'C':
      // set the initial colorset from the string
      initial_colorset_str = optarg;
      break;
    case 'P':
      // set the initial pattern from the string
      initial_pattern_str = optarg;
      break;
    case 'I':
      // set the initial mode index
      initial_mode_index = strtoul(optarg, NULL, 10);
      break;
    case 'b':
      // generate a bmp file
      generate_bmp = true;
      // allow for a space between the -b and the filename
      if (optarg == NULL && optind < argc && argv[optind][0] != '-') {
        optarg = argv[optind++];
      }
      // if an argument was provided for -b then set it as the bmp filename
      if (optarg) {
        bmp_filename = optarg;
      }
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
    if (generate_bmp) {
      // still need to generate the BMP by recoring all the output colors
      // even if they have chosen the -q for quiet option
      RGBColor currentColor = {Led::get().red, Led::get().green, Led::get().blue};
      colorBuffer.push_back(currentColor);
    }
    return;
  }
  std::string out;
  if (in_place) {
    // this resets the cursor back to the beginning of the line
    out += "\r";
  }
  // Get the current color and scale its brightness up
  RGBColor currentColor = {Led::get().red, Led::get().green, Led::get().blue};
  if (output_type == OUTPUT_TYPE_COLOR) {
    out += "\x1B[0m["; // opening |
    out += "\x1B[48;2;"; // colorcode start
    out += std::to_string(currentColor.red) + ";"; // col red
    out += std::to_string(currentColor.green) + ";"; // col green
    out += std::to_string(currentColor.blue) + "m"; // col blue
    out += "  "; // colored space
    out += "\x1B[0m]"; // ending |
  } else if (output_type == OUTPUT_TYPE_HEX) {
    // otherwise this just prints out the raw hex code if not in color mode
    for (uint32_t i = 0; i < output_type; ++i) {
      char buf[128] = { 0 };
      snprintf(buf, sizeof(buf), "%02X%02X%02X", currentColor.red, currentColor.green, currentColor.blue);
      out += buf;
    }
  }
  // if the engine
  if (generate_bmp) {
    // Add scaled color to buffer
    colorBuffer.push_back(currentColor);
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

bool writeBMP(const std::string& filename, const std::vector<RGBColor>& colors)
{
  if (colors.empty()) {
    std::cerr << "Invalid image dimensions or empty color array." << std::endl;
    return false;
  }
  // need structure alignment for BMP header structures
#pragma pack(push, 1)
  // just putting these here because they are directly relevant to this function
  // and not used anywhere else in the code, if they get used more then just pull
  // this all out into it's own file based on BMP writing
  struct BMPHeader {
    char signature[2];
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
  };
  struct DIBHeader {
    uint32_t headerSize;
    int32_t width;
    int32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    int32_t xPixelsPerMeter;
    int32_t yPixelsPerMeter;
    uint32_t colorsInColorTable;
    uint32_t importantColorCount;
  };
#pragma pack(pop)
  const int32_t width = colors.size();
  const int32_t height = 1;
  // rows are padded to the nearest multiple of 4 bytes
  const uint32_t rowPaddedSize = (width * 3 + 3) & ~3;
  const uint32_t imageSize = rowPaddedSize * height;
  const uint32_t fileSize = 54 + imageSize;
  // BMP header (14 bytes) + DIB header (40 bytes) + image data
  BMPHeader bmpHeader = {{'B', 'M'}, fileSize, 0, 54};
  DIBHeader dibHeader = {40, width, height, 1, 24, 0, imageSize, 2835, 2835, 0, 0};
  // open the file
  std::ofstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Failed to open file: " << filename << " (" << strerror(errno) << ")" << std::endl;
    return false;
  }
  // write out the headers
  file.write((const char *)&bmpHeader, sizeof(bmpHeader));
  file.write((const char *)&dibHeader, sizeof(dibHeader));
  // write out data
  for (int32_t y = height - 1; y >= 0; --y) {
    for (int32_t x = 0; x < width; ++x) {
      const RGBColor& color = colors[y * width + x];
      // BGR format
      const unsigned char pixel[3] = { color.blue, color.green, color.red };
      file.write((const char *)pixel, 3);
    }
    // write out extra padding bytes at end of row
    file.write("\0\0\0", rowPaddedSize - (width * 3));
  }
  if (!file.good()) {
    std::cerr << "Error writing to file: " << filename << std::endl;
    return false;
  }
  return true;
}

// print out the usage for the tool
static void print_usage(const char* program_name)
{
  fprintf(stderr, "Usage: %s [options] < input commands\n", program_name);
  fprintf(stderr, "Output Selection (at least one required):\n");
  fprintf(stderr, "  -x, --hex                Print hex values to represent led colors instead of color codes\n");
  fprintf(stderr, "  -c, --color              Print console color codes to represent led colors\n");
  fprintf(stderr, "  -q, --quiet              Do not print anything, silently perform an operation\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Engine Control Flags (optional):\n");
  fprintf(stderr, "  -l, --lockstep           Only step once each time an input is received\n");
  fprintf(stderr, "  -t, --no-timestep        Run as fast as possible without managing timestep\n");
  fprintf(stderr, "  -i, --in-place           Print the output in-place (interactive mode)\n");
  fprintf(stderr, "  -s, --no-storage         Disable persistent storage to file (FlashStorage.flash)\n");
  fprintf(stderr, "  -y, --cycle [N]          Run N cycles of the first mode, default 1 (to gen pattern images)\n");
  fprintf(stderr, "  -a, --brightness-scale   Set the brightness scale of the output colors (2.0 is 100%% brighter)\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Initial Pattern and Colorset (optional):\n");
  fprintf(stderr, "  -C, --colorset           Set the colorset of the first mode, ex: red,green,0x0000ff\n");
  fprintf(stderr, "  -P, --pattern            Set the pattern of the first mode, ex: 1 or blend\n");
  fprintf(stderr, "  -I, --mode-index         Set the initial mode index, ex 4\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Other Options:\n");
  fprintf(stderr, "  -b, --bmp [file]         Specify a bitmap file to generate (default: " DEFAULT_BMP_FILENAME ")\n");
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
