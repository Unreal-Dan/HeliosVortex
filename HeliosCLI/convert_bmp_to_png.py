from PIL import Image, ImageDraw
import numpy as np
import argparse
import os

def map_pattern_to_circle(draw, pattern, center, radius, thickness):
    circumference = int(2 * np.pi * radius)
    num_repeats = circumference // pattern.width
    print(f"-- Mapping pattern to circle with circumference {circumference} and {num_repeats} repeats")

    for n in range(num_repeats):
        for i in range(pattern.width):
            angle = (2 * np.pi * (i + n * pattern.width)) / circumference
            for t in range(thickness):
                dx = (radius - t) * np.cos(angle)
                dy = (radius - t) * np.sin(angle)
                x, y = center[0] + dx, center[1] + dy
                color = pattern.getpixel((i, 0))
                if color != (0, 0, 0):
                    draw.point((x, y), fill=color)

def create_circular_pattern(path_to_bmp):
    print(f"-- Creating circular pattern for {path_to_bmp}")
    pattern_strip = Image.open(path_to_bmp)
    width, _ = pattern_strip.size

    canvas_size = int(width * 6.28)
    canvas = Image.new('RGB', (canvas_size, canvas_size), 'black')
    draw = ImageDraw.Draw(canvas)

    center = (canvas_size // 2, canvas_size // 2)
    outer_radius = canvas_size // 3
    thickness = 10

    print(f"-- Canvas prepared with size {canvas_size}x{canvas_size}, center at {center}, outer radius {outer_radius}, and thickness {thickness}")
    map_pattern_to_circle(draw, pattern_strip, center, outer_radius, thickness)

    print("-- Pattern mapped successfully")
    return canvas

def main():
    parser = argparse.ArgumentParser(description="Generate a circular pattern from a BMP file.")
    parser.add_argument("bmp_file", type=str, help="Path to the BMP file")
    parser.add_argument("output_file", type=str, help="Path to the output PNG file")
    args = parser.parse_args()

    circular_pattern = create_circular_pattern(args.bmp_file)
    circular_pattern.save(args.output_file)
    print(f"** Saved circular pattern to {args.output_file}")

if __name__ == "__main__":
    main()
