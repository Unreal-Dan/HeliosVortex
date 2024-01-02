BINDIR=/Applications/Arduino.app/Contents/Java/hardware/tools/avr/bin/
AVRDUDEDIR=/usr/local/bin/

CC = ${BINDIR}/avr-g++
LD = ${BINDIR}/avr-g++
OBJCOPY = ${BINDIR}/avr-objcopy -v
AR = ${BINDIR}/avr-gcc-ar
SIZE = ${BINDIR}/avr-size
OBJDUMP = ${BINDIR}/avr-objdump
NM = ${BINDIR}/avr-nm
AVRDUDE = ${AVRDUDEDIR}/avrdude

AVRDUDE_CONF = avrdude.conf
AVRDUDE_PORT = /dev/tty.usbmodem1423201
AVRDUDE_PROGRAMMER = stk500v1
AVRDUDE_BAUDRATE = 19200
AVRDUDE_CHIP = attiny85

AVRDUDE_FLAGS = -C$(AVRDUDE_CONF) \
		-p$(AVRDUDE_CHIP) \
		-c$(AVRDUDE_PROGRAMMER) \
		-P$(AVRDUDE_PORT) \
		-b$(AVRDUDE_BAUDRATE) \
		-v

CPU_SPEED = 16000000L

# the port for serial upload
SERIAL_PORT = COM11

CFLAGS = -g \
	 -Os \
	 -MMD \
	 -Wall \
	 -flto \
	 -mrelax \
	 -std=gnu++17 \
	 -fshort-enums \
	 -fpack-struct \
	 -fno-exceptions \
	 -fdata-sections \
	 -funsigned-char \
	 -ffunction-sections\
	 -funsigned-bitfields \
	 -fno-threadsafe-statics \
	 -D__AVR_ATtiny85__ \
	 -mmcu=$(AVRDUDE_CHIP) \
	 -DF_CPU=$(CPU_SPEED) \
	 -D HELIOS_EMBEDDED

LDFLAGS = -g \
	  -Wall \
	  -Os \
	  -flto \
	  -fuse-linker-plugin \
	  -Wl,--gc-sections \
	  -mrelax \
	  -lm \
	  -mmcu=$(AVRDUDE_CHIP) \

INCLUDES=\
	-I $(INCLUDE_DIR) \
	-I ./

CFLAGS+=$(INCLUDES)

# Source files
ifeq ($(OS),Windows_NT) # Windows
SRCS = \
       $(shell find . -maxdepth 1 -type f -name '\*.cpp')
else # linux
SRCS = \
       $(shell find . -maxdepth 1 -type f -name \*.cpp)
endif

OBJS = $(SRCS:.cpp=.o)

DFILES = $(SRCS:.cpp=.d)

# Target name
TARGET = helios

all: $(TARGET).hex
	$(OBJDUMP) --disassemble --source --line-numbers --demangle --section=.text $(TARGET).elf > $(TARGET).lst
	$(NM) --numeric-sort --line-numbers --demangle --print-size --format=s $(TARGET).elf > $(TARGET).map
	chmod +x avrsize.sh
	./avrsize.sh $(TARGET).elf

$(TARGET).hex: $(TARGET).elf
	$(OBJCOPY) -O binary -R .eeprom $(TARGET).elf $(TARGET).bin
	$(OBJCOPY) -O ihex -j .eeprom --set-section-flags=.eeprom=alloc,load --no-change-warnings --change-section-lma .eeprom=0 $(TARGET).elf $(TARGET).eep
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(TARGET).elf: $(OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

%.o: %.S
	$(CC) $(ASMFLAGS) -c $< -o $@

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

upload: $(TARGET).hex
	$(AVRDUDE) $(AVRDUDE_FLAGS) -Uflash:w:$(TARGET).hex:i

upload_eeprom: $(TARGET).eep
	$(AVRDUDE) $(AVRDUDE_FLAGS) -Ueeprom:w:$(TARGET).eep:i

ifneq ($(OS),Windows_NT) # Linux
build: all
INSTALL_DIR=~/atmel_setup
# Name of the toolchain tarball
TOOLCHAIN_TAR=avr8-gnu-toolchain-3.7.0.1796-linux.any.x86_64.tar.gz
# Name of the ATtiny DFP zip
ATTINY_ZIP=Atmel.ATtiny_DFP.2.0.368.atpack
install:
	@echo "Setting up in directory $(INSTALL_DIR)"
	@mkdir -p $(INSTALL_DIR)
	@cd $(INSTALL_DIR) && \
	echo "Downloading and installing AVR 8-bit Toolchain..." && \
	wget -q https://ww1.microchip.com/downloads/aemDocuments/documents/DEV/ProductDocuments/SoftwareTools/$(TOOLCHAIN_TAR) && \
	tar -xf $(TOOLCHAIN_TAR) && \
	echo "Downloading and installing ATtiny DFP..." && \
	wget -q http://packs.download.atmel.com/$(ATTINY_ZIP) && \
	unzip $(ATTINY_ZIP)
	@echo "Download and extraction complete. You'll find the toolchain and pack files in $(INSTALL_DIR)"
endif

set_16mhz_fuses:
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U lfuse:w:0xe1:m -U hfuse:w:0xdd:m -U efuse:w:0xff:m

set_default_fuses:
	$(AVRDUDE) $(AVRDUDE_FLAGS) -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m

clean:
	rm -f $(OBJS) $(TARGET).elf $(TARGET).hex $(DFILES) $(TARGET).bin $(TARGET).eep $(TARGET).lst $(TARGET).map

# include dependency files to ensure partial rebuilds work correctly
-include $(DFILES)
