#!/bin/bash

#######################################################################################################
#    Usage: ./helios [options] < input commands
#    Output Selection (at least one required):
#      -x, --hex                Print hex values to represent led colors instead of color codes
#      -c, --color              Print console color codes to represent led colors
#      -q, --quiet              Do not print anything, silently perform an operation
#
#    Engine Control Flags (optional):
#      -l, --lockstep           Only step once each time an input is received
#      -t, --no-timestep        Run as fast as possible without managing timestep
#      -i, --in-place           Print the output in-place (interactive mode)
#      -s, --no-storage         Disable persistent storage to file (FlashStorage.flash)
#      -y, --cycle [N]          Run N cycles of the first mode, default 1 (to gen pattern images)
#      -a, --brightness-scale   Set the brightness scale of the output colors (2.0 is 100% brighter)
#
#    Initial Pattern and Colorset (optional):
#      -C, --colorset           Set the colorset of the first mode, ex: red,green,0x0000ff
#      -P, --pattern            Set the pattern of the first mode, ex: 1 or blend
#      -I, --mode-index         Set the initial mode index, ex 4
#
#    Other Options:
#      -b, --bmp [file]         Specify a bitmap file to generate (default: pattern.bmp)
#      -E, --eeprom             Generate an eeprom file for flashing
#      -h, --help               Display this help message
#
#    Input Commands (pass to stdin):
#       c         standard short click
#       l         standard long click
#       p         press the button and hold
#       r         release the button from hold
#       t         toggle button press state
#       w         wait 1 tick
#       q         quit
#
#    Example Usage:
#       ./helios
#       ./helios -ci
#       ./helios -cl <<< 300wcw300wcp1500wr300wq


HELIOS=./helios
PATTERN_DIR=./default_patterns
BMP_DIR=./bmp_patterns

# Default values
CYCLE_COUNT=1
INPUT_COMMANDS="410wq"
NUM_PATTERNS=20
NUM_DEFAULT_PATTERNS=6

# Ensure bmp_patterns directory exists
mkdir -p "$BMP_DIR"

make helios
if [ $? -ne 0 ]; then
    echo "Failed to build helios"
    exit 1
fi

if [ ! -x "$HELIOS" ]; then
    echo "Cannot find helios program: $HELIOS"
    exit 1
fi

# Parse command-line options
while getopts ":c" opt; do
  case ${opt} in
    c )
      INPUT_COMMANDS=""
      ;;
    \? )
      echo "Invalid option: $OPTARG" 1>&2
      exit 1
      ;;
  esac
done

# Iterate over .pattern files
for pattern_file in "$PATTERN_DIR"/*.pattern; do
    filename=$(basename -- "$pattern_file")
    filename="${filename%.*}"
    COLOR_SET=$(grep "COLOR_SET=" "$pattern_file" | cut -d= -f2)
    PATTERN_ID=$(grep "PATTERN_ID=" "$pattern_file" | cut -d= -f2)
    BRIGHTNESS_SCALE=$(grep "BRIGHTNESS_SCALE=" "$pattern_file" | cut -d= -f2)

    # Use extracted parameters to generate the pattern
    if [ -z "$INPUT_COMMANDS" ]; then
        $HELIOS \
            --quiet \
            --no-timestep \
            --brightness-scale "$BRIGHTNESS_SCALE" \
            --colorset "$COLOR_SET" \
            --pattern "$PATTERN_ID" \
            --bmp "$BMP_DIR/${filename}.bmp" \
            --cycle "$CYCLE_COUNT"
    else
        $HELIOS \
            --quiet \
            --no-timestep \
            --brightness-scale "$BRIGHTNESS_SCALE" \
            --colorset "$COLOR_SET" \
            --pattern "$PATTERN_ID" \
            --bmp "$BMP_DIR/${filename}.bmp" \
            <<< "$INPUT_COMMANDS"
    fi

    if [ $? -ne 0 ]; then
        echo "Helios command failed for $filename with COLOR_SET=$COLOR_SET, PATTERN_ID=$PATTERN_ID"
    fi
done

# Generate patterns 0 to 19
for ((i = 0; i <= NUM_PATTERNS - 1; i++)); do
    echo "Generating pattern $i..."
    if [ -z "$INPUT_COMMANDS" ]; then
        $HELIOS \
            --quiet \
            --no-timestep \
            --brightness-scale "2.0" \
            --colorset "red,orange,yellow,turquoise,blue,pink" \
            --pattern "$i" \
            --bmp "$BMP_DIR/$(printf "%03d_Pattern.bmp" $((i + NUM_DEFAULT_PATTERNS + 1)))" \
            --cycle "$CYCLE_COUNT"
    else
        $HELIOS \
            --quiet \
            --no-timestep \
            --brightness-scale "2.0" \
            --colorset "red,orange,yellow,turquoise,blue,pink" \
            --pattern "$i" \
            --bmp "$BMP_DIR/$(printf "%03d_Pattern.bmp" $((i + NUM_DEFAULT_PATTERNS + 1)))" \
            <<< "$INPUT_COMMANDS"
    fi
done