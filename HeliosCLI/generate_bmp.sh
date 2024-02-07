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

# Iterate over .pattern files
for pattern_file in "$PATTERN_DIR"/*.pattern; do
    filename=$(basename -- "$pattern_file")
    filename="${filename%.*}"
    COLOR_SET=$(grep "COLOR_SET=" "$pattern_file" | cut -d= -f2)
    PATTERN_ID=$(grep "PATTERN_ID=" "$pattern_file" | cut -d= -f2)

    # Use extracted parameters to generate the pattern
    $HELIOS \
        --quiet \
        --no-timestep \
        --colorset "$COLOR_SET" \
        --pattern "$PATTERN_ID" \
        --cycle 1 \
        --bmp "$BMP_DIR/${filename}.bmp"

    if [ $? -ne 0 ]; then
        echo "Helios command failed for $filename with COLOR_SET=$COLOR_SET, PATTERN_ID=$PATTERN_ID"
    fi

for i in {0..19}; do
	echo "Generating pattern $i..."
  $HELIOS \
		--quiet \
		--no-timestep \
		--colorset "red,orange,yellow,turquoise,blue,pink" \
        --pattern "$i" \
		--cycle 1 \
		--bmp "$BMP_DIR/$(printf "%03d_Pattern.bmp" $((i + 7)))"
done

done
