# Set the directory for the AVR toolchain binaries
BINDIR=/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/

# Compiler and other tools
CC = ${BINDIR}avr-g++
LD = ${BINDIR}avr-g++
OBJCOPY = ${BINDIR}avr-objcopy
AVRDUDE = ${BINDIR}avrdude

# AVRDUDE configurations
AVRDUDE_CONF = avrdude.conf
AVRDUDE_PROGRAMMER = stk500v1
AVRDUDE_BAUDRATE = 19200
AVRDUDE_CHIP = attiny85
AVRDUDE_PORT = /dev/tty.usbmodem1423201

AVRDUDE_FLAGS = -C$(AVRDUDE_CONF) \
	-p$(AVRDUDE_CHIP) \
	-c$(AVRDUDE_PROGRAMMER) \
	-P$(AVRDUDE_PORT) \
	-b$(AVRDUDE_BAUDRATE) \
	-v

CPU_SPEED = 16000000L

# Compiler flags
CFLAGS = -Os \
	-Wall \
	-flto \
	-std=gnu++17 \
	-mmcu=$(AVRDUDE_CHIP) \
	-DF_CPU=$(CPU_SPEED)

LDFLAGS = -Os \
	-Wall \
	-flto \
	-Wl,--gc-sections \
	-lm \
	-mmcu=$(AVRDUDE_CHIP)

# Source files
SRCS = main.cpp
OBJS = $(SRCS:.cpp=.o)

# Target name
TARGET = helios

all: $(TARGET).hex

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

upload: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -Uflash:w:$(TARGET).hex:i

set_16mhz_fuses:
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m -U efuse:w:0xff:m

set_default_fuses:
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).hex $(TARGET).bin $(TARGET).eep

# Include dependency files to ensure partial rebuilds work correctly
-include $(OBJS:.o=.d)
