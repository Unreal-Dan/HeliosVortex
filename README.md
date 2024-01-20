# Helios Vortex Project

## Introduction
![Build](https://github.com/Unreal-Dan/HeliosVortex/actions/workflows/build.yml/badge.svg?branch=master)  
Welcome to the Helios Vortex project! This repository contains the firmware for the Helios microlight. Specifically, the firmware is designed for the ATTiny85 chip and is capable of controlling a single LED to produce a variety of patterns and effects.

## Features
- A large list of available flashing patterns
- Customizable colorsets up to 6 slots and over 100 colors
- Ability to shift modes around
- Colorset/Pattern randomizer feature
- Light lock to prevent accidental turn-ons
- Factory reset to restore chip settings
- Conjure mode for one-click on-off
- Efficient code optimized for ATTiny85
- Easy to modify for different flashing patterns

## Installation
To install and run this firmware on your ATTiny85, follow these steps:

1. **Prerequisites**: Ensure you have a working setup for programming an ATTiny85, including an ISP programmer.
2. **Clone the Repository**: Use `git clone` to download this repository to your local machine.
3. **Setup Toolchain**: Make sure you have AVR-GCC and AVRDUDE installed to compile and upload the firmware.
4. **Compile the Firmware**: Navigate to the firmware directory and run `make` to compile the firmware.
5. **Upload to ATTiny85**: Use AVRDUDE with your ISP programmer to upload the compiled firmware to the ATTiny85.

There are some make targets for uploading with avrdude, 'make upload' but you will need to manually configure the makefile to target the correct COM port.
