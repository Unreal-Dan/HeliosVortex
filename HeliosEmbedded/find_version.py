import sys

def find_version(hex_content):
    try:
        # Convert hex file content to string if it's bytes
        if isinstance(hex_content, bytes):
            hex_content = hex_content.decode('ascii')

        # Look for the specific hex pattern: 312E332E3238 (1.3.28)
        # First convert the entire content to a continuous string of hex values
        hex_string = ''.join(line.strip()[9:-2] for line in hex_content.splitlines() if line.startswith(':'))

        # Look for our version pattern
        import re
        # Pattern for version number in hex (31 2E 33 2E 32 38 = "1.3.28")
        version_pattern = r'312E[0-9A-F]{2}2E[0-9A-F]{2,4}'
        match = re.search(version_pattern, hex_string)

        if match:
            # Convert hex to ASCII
            hex_version = match.group(0)
            version_bytes = bytes.fromhex(hex_version)
            version_str = version_bytes.decode('ascii')
            return version_str

        return None
    except Exception as e:
        print(f"Error processing hex file: {str(e)}")
        return None

if __name__ == "__main__":
    try:
        # Check if filename is provided as argument
        if len(sys.argv) > 1:
            filename = sys.argv[1]
            with open(filename, 'rb') as f:  # Open in binary mode
                content = f.read()
        # Otherwise read from stdin if input is piped
        elif not sys.stdin.isatty():
            content = sys.stdin.buffer.read()  # Read binary from stdin
        # Finally, try to read default filename
        else:
            with open('helios_firmware.hex', 'rb') as f:  # Open in binary mode
                content = f.read()

        version = find_version(content)
        if version:
            print(f"Found version: {version}")
        else:
            print("Version signature not found.")
    except FileNotFoundError:
        print("Error: Hex file not found")
    except Exception as e:
        print(f"Error: {str(e)}")