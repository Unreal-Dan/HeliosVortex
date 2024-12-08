# Helios Engine CLI

## CLI Tool

The Helios Engine project includes a powerful Command Line Interface (CLI) tool that allows users to interact with and test the firmware without the need for physical hardware. This tool is invaluable for development, testing, and pattern creation.

### Key CLI Features

1. **Pattern Simulation**: Simulate any pattern supported by the firmware.
2. **Color Output**: Display LED colors in the terminal using ANSI color codes or hexadecimal values.
3. **Input Simulation**: Simulate button presses and holds to test firmware behavior.
4. **Timestep Control**: Run simulations in real-time or as fast as possible.
5. **Storage Emulation**: Emulate EEPROM storage for testing persistence features.
6. **BMP Generation**: Generate bitmap images of pattern outputs for documentation or analysis.

### CLI Usage

To use the CLI tool, navigate to the `HeliosCLI` directory and run the `helios` executable with various command-line options. Here are some examples:

```bash
./helios --color --in-place
```

```bash
./helios -cl <<< 300wcw300wcp1500wr300wq
```

```bash
./helios --pattern 1 --colorset "red,green,blue" --bmp output.bmp
```

For a full list of options, run `./helios --help`.

### Input Commands

The CLI tool accepts the following input commands:

- `c`: Simulate a short click
- `l`: Simulate a long click
- `p`: Simulate pressing and holding the button
- `r`: Simulate releasing the button
- `t`: Toggle button press state
- `w`: Wait for one tick
- `q`: Quit the simulation

These commands can be chained together to create complex input sequences for testing.

## Pattern Visualization

The Helios Engine project includes tools for generating visual representations of patterns in both PNG and SVG formats. These visualizations are useful for documentation, analysis, and sharing pattern designs.

### Generating BMPs and PNG

To generate bitmap (BMP) and PNG images of patterns:

1. Navigate to the `HeliosCLI` directory.
2. Run the `make pngs` command:

```bash
make pngs
```

This script will:

- Generate BMP files for all default patterns and generic flashing patterns.
- Save the BMP files in the `bmp_patterns` directory.
- Convert the BMP files to PNG format using the `convert_bmp_to_png.py` script.

The resulting PNG files will be saved in the `circular_patterns_png` directory.

### Generating SVGs

To generate Scalable Vector Graphics (SVG) representations of patterns:

1. Navigate to the `HeliosCLI` directory.
2. Run the `make svgs` command:

```bash
make svgs
```

This script will:

- Process all BMP files in the `bmp_patterns` directory.
- Convert each BMP to an SVG format using the `convert_bmp_to_svg.py` script.
- Save the resulting SVG files in the `circular_patterns_svg` directory.

### Customizing Pattern Visualizations

You can customize the pattern visualization process by modifying the following files:

- `generate_bmps.sh`: Adjust parameters such as brightness scale, color sets, or cycle count.
- `convert_bmp_to_png.py`: Modify the PNG conversion process or output format.
- `convert_bmp_to_svg.py`: Customize the SVG generation, such as changing colors or layout.

These visualizations are particularly useful for:

- Documenting the available patterns in the Helios Engine project.
- Analyzing pattern behavior and timing.
- Sharing pattern designs with the community.
- Creating visual aids for user manuals or promotional materials.