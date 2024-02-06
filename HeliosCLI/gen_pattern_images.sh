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
BRIGHTNESS_SCALE=10.0

make helios
if [ $? -ne 0 ]; then
	echo "Failed to build helios"
	exit 1
fi

if [ ! -x "$HELIOS" ]; then
	echo "Cannot find helios program: $HELIOS"
	exit 1
fi

for i in {0..5}; do
	echo "Generating pattern $i..."
  $HELIOS \
		--quiet \
		--no-timestep \
		--brightness-scale $BRIGHTNESS_SCALE \
		--mode-index $i \
		--cycle 1 \
		--bmp "pattern${i}.bmp"
done
