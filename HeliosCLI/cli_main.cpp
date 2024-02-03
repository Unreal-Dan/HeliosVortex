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

#include "Helios.h"
#include "TimeControl.h"
#include "Storage.h"
#include "Colortypes.h"
#include "Button.h"
#include "Led.h"

std::vector<RGBColor> colorBuffer;

// the output types of the tool
enum OutputType {
  OUTPUT_TYPE_NONE,
  OUTPUT_TYPE_HEX,
  OUTPUT_TYPE_COLOR,
};

// various globals for the tool
OutputType output_type = OUTPUT_TYPE_COLOR;
std::string bmp_filename = "pattern.bmp";
bool in_place = false;
bool lockstep = false;
bool storage = false;
bool timestep = true;
bool eeprom = false;
bool generate_bmp = false;
// Define the scaling factor (e.g., 1 = no scaling up 30 = scale up 100%)
float scaleFactor = 1.0f;

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
  // if the user requested a bmp file to be written
  if (generate_bmp) {
    // if they didn't record anything give them a message indicating they need to record
    if (!colorBuffer.size()) {
      std::cout << "Cannot generate BMP! Color buffer is empty, did you specify a section to record with 's' and 'e' input commands?" << std::endl;
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
    {"bmp", optional_argument, nullptr, 'b'},
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
  while ((opt = getopt_long(argc, argv, "xcqltisb::Eh", long_options, &option_index)) != -1) {
    switch (opt) {
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
      // TODO: catch the 's' and the 'e' here and handle them at the CLI level
      //       OR implement recording in the engine itself. But don't have the recording bool
      //       owned by the engine, but the actual recording data (array of colors) owned by the CLI
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
  std::string out;
  if (in_place) {
    // this resets the cursor back to the beginning of the line
    out += "\r";
  }
  // Get the current color and scale its brightness up
  RGBColor currentColor = {Led::get().red, Led::get().green, Led::get().blue};
  RGBColor scaledColor = currentColor.scaleBrightness(scaleFactor);
  if (output_type == OUTPUT_TYPE_COLOR) {
    out += "\x1B[0m["; // opening |
    out += "\x1B[48;2;"; // colorcode start
    out += std::to_string(scaledColor.red) + ";"; // col red
    out += std::to_string(scaledColor.green) + ";"; // col green
    out += std::to_string(scaledColor.blue) + "m"; // col blue
    out += "  "; // colored space
    out += "\x1B[0m]"; // ending |
  } else if (output_type == OUTPUT_TYPE_HEX) {
    // otherwise this just prints out the raw hex code if not in color mode
    for (uint32_t i = 0; i < output_type; ++i) {
      char buf[128] = { 0 };
      snprintf(buf, sizeof(buf), "%02X%02X%02X", scaledColor.red, scaledColor.green, scaledColor.blue);
      out += buf;
    }
  }
  // if the engine
  if (Helios::is_recording()) {
    // Add scaled color to buffer
    colorBuffer.push_back(scaledColor);
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
  // structure of the bmp and dib headers to write out
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
  // some basic calculations for the bmp dimensions
  int32_t width = colors.size();
  int32_t height = 1;
  uint32_t rowSize = width * 3 + (width % 4 ? 4 - (width * 3 % 4) : 0);
  // create local instances of the BMP and DIB header to write out
  BMPHeader bmpHeader = {{'B', 'M'}, 54 + rowSize * height, 0, 54};
  DIBHeader dibHeader = {40, width, height, 1, 24, 0, 0, 2835, 2835, 0, 0};
  // open the output bmp file
  std::ofstream file(filename, std::ios::binary);
  if (!file) {
    std::cerr << "Failed to open file: " << filename << " (" << strerror(errno) << ")" << std::endl;
    return false;
  }
  // write out the bmp header and dib header
  file.write((char*)&bmpHeader, sizeof(bmpHeader));
  file.write((char*)&dibHeader, sizeof(dibHeader));
  // write out the array of colors
  for (const auto& color : colors) {
    file.write((char*)&color, 3);
  }
  // fill the rest of the space with 0s so it's aligned
  for (uint32_t i = 0; i < (rowSize - (width * 3)); ++i) {
    file.put(0);
  }
  // check the file was written properly
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
  fprintf(stderr, "\n");
  fprintf(stderr, "Engine Control Flags (optional):\n");
  fprintf(stderr, "  -l, --lockstep           Only step once each time an input is received\n");
  fprintf(stderr, "  -t, --no-timestep        Run as fast as possible without managing timestep\n");
  fprintf(stderr, "  -i, --in-place           Print the output in-place (interactive mode)\n");
  fprintf(stderr, "  -s, --no-storage         Disable persistent storage to file (FlashStorage.flash)\n");
  fprintf(stderr, "\n");
  fprintf(stderr, "Other Options:\n");
  fprintf(stderr, "  -b, --bmp [file]         Specify a bitmap file to generate (default: output.bmp)\n");
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
    "\n   s         start recording BMP output",
    "\n   e         end recording BMP output",
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
