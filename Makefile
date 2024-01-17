# Essential variables for macOS
BINDIR=/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/
AVRDUDE_PORT = /dev/tty.usbmodem1423201
AVRDUDE_CONF = avrdude.conf
AVRDUDE_PROGRAMMER = stk500v1
AVRDUDE_BAUDRATE = 19200
AVRDUDE_CHIP = attiny85
CPU_SPEED = 16000000L
TARGET = helios

# Compiler, Linker, and Other Tools
CC = ${BINDIR}avr-g++
LD = ${BINDIR}avr-g++
OBJCOPY = ${BINDIR}avr-objcopy -v
SIZE = ${BINDIR}avr-size
OBJDUMP = ${BINDIR}avr-objdump
NM = ${BINDIR}avr-nm
AVRDUDE = ${BINDIR}avrdude

# Compiler and Linker Flags
CFLAGS = -Os -MMD -Wall -flto -mrelax -std=gnu++17 -mmcu=$(AVRDUDE_CHIP) -DF_CPU=$(CPU_SPEED) -D __AVR_ATtiny85__
LDFLAGS = -Wall -Os -flto -fuse-linker-plugin -Wl,--gc-sections -mrelax -lm -mmcu=$(AVRDUDE_CHIP)
INCLUDES = -I ./
CFLAGS += $(INCLUDES)

# Source files
SRCS = $(shell find . -maxdepth 1 -type f -name 'main.cpp')
OBJS = $(SRCS:.cpp=.o)
DFILES = $(SRCS:.cpp=.d)

# AVRDUDE Flags
AVRDUDE_FLAGS = -C$(AVRDUDE_CONF) -p$(AVRDUDE_CHIP) -c$(AVRDUDE_PROGRAMMER) -P$(AVRDUDE_PORT) -b$(AVRDUDE_BAUDRATE) -v

# Targets
all: $(TARGET).hex
	@echo Detected Operating System: Darwin
	$(OBJDUMP) --disassemble --source --line-numbers --demangle --section=.text $(TARGET).elf > $(TARGET).lst
	$(NM) --numeric-sort --line-numbers --demangle --print-size --format=s $(TARGET).elf > $(TARGET).map
	./avrsize.sh $(TARGET).elf

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

upload: set_16mhz_fuses $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -Uflash:w:$(TARGET).hex:i

set_16mhz_fuses:
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m -U efuse:w:0xff:m

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).hex $(DFILES) $(TARGET).bin $(TARGET).eep $(TARGET).lst $(TARGET).map

-include $(DFILES)
