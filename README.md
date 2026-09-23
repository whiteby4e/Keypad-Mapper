# Keypad Mapper

A small Arduino/ESP32 utility for identifying the pin pairs of common **matrix keypads**.

The mapper scans a configurable set of GPIO pins, detects which two pins are connected by each key, records the matrix connections, and prints Arduino Keypad row/column configuration to the Serial Monitor.

## Supported Sizes

| Rows | Columns | Keys |
|---:|---:|---:|
| 2 | 2 | 4 |
| 2 | 3 | 6 |
| 2 | 4 | 8 |
| 3 | 2 | 6 |
| 3 | 3 | 9 |
| 3 | 4 | 12 |
| 4 | 2 | 8 |
| 4 | 3 | 12 |
| 4 | 4 | 16 |

The most common keypad sizes are **2x4, 3x4, and 4x4**.

> Note: a typical 1x4 keypad with four independent button wires is not a matrix keypad and is not handled by this version.

## Features

- Supports multiple matrix keypad sizes
- Automatically detects key-to-pin connections
- Configurable row/column count
- Works with configurable GPIO candidates
- Uses internal pull-up resistors
- Includes debounce handling
- Prevents duplicate connections
- Prints ready-to-copy Arduino Keypad row/column arrays
- No display or extra hardware required

## Configuration

Open \`Keypad_Mapper.ino\` and change:

\`\`\`cpp
constexpr uint8_t KEYPAD_ROWS = 4;
constexpr uint8_t KEYPAD_COLS = 4;
\`\`\`

For a 2x4 keypad:

\`\`\`cpp
constexpr uint8_t KEYPAD_ROWS = 2;
constexpr uint8_t KEYPAD_COLS = 4;
\`\`\`

For a 3x4 keypad:

\`\`\`cpp
constexpr uint8_t KEYPAD_ROWS = 3;
constexpr uint8_t KEYPAD_COLS = 4;
\`\`\`

The number of GPIOs required is always:

\`\`\`text
ROWS + COLS
\`\`\`

Examples:

- 2x4 -> 6 GPIOs
- 3x4 -> 7 GPIOs
- 4x4 -> 8 GPIOs

## Hardware

- ESP32 or another Arduino-compatible board
- Matrix keypad
- USB cable

### Default GPIO candidates

The sketch currently contains eight GPIO candidates:

\`\`\`text
13, 12, 27, 26, 25, 33, 32, 23
\`\`\`

For smaller keypads, only the first \`ROWS + COLS\` entries are used.

Change the \`PINS\` array in \`Keypad_Mapper.ino\` if you want to use different GPIOs.

## Wiring

Connect the keypad wires to the GPIOs listed in \`PINS[]\`. The physical order of the wires does not need to be known beforehand.

For example, a 2x4 keypad needs six GPIOs:

\`\`\`text
Keypad -> ESP32
6 wires -> GPIO 13, 12, 27, 26, 25, 33
\`\`\`

**Do not connect the keypad to 5V.** The mapper uses the GPIO internal pull-ups and reads the matrix directly.

## Usage

1. Set \`KEYPAD_ROWS\` and \`KEYPAD_COLS\`.
2. Make sure \`PINS[]\` contains at least \`ROWS + COLS\` suitable GPIOs.
3. Upload \`Keypad_Mapper.ino\`.
4. Open Arduino IDE Serial Monitor.
5. Set the baud rate to **115200**.
6. Press every keypad key once.
7. The mapper prints each detected connection.
8. After all expected keys are detected, it prints the row/column arrays.
9. Copy those arrays into your Arduino Keypad project.

Example for a 4x4 keypad:

\`\`\`text
=== Keypad Mapper ===
Target: 4x4
Expected keys: 16
Press every key once.

Key 1: 13 <-> 27
Key 2: 13 <-> 26
...
Key 16: 23 <-> 25

=== Finished ===
Keypad size: 4x4
Detected connections: 16

=== Arduino Keypad Configuration ===
byte rowPins[ROWS] = {
  ...
};

byte colPins[COLS] = {
  ...
};
\`\`\`

## Important Notes

- This project handles **matrix keypads**.
- It identifies electrical matrix connections, not printed labels such as \`1\`, \`2\`, \`3\`, \`A\`, etc.
- GPIO candidates must support the required input/output modes.
- Avoid GPIOs that are unavailable, input-only, or reserved by your board.
- On ESP32, check your specific board's GPIO restrictions before changing \`PINS[]\`.
- For smaller matrices, make sure the configured GPIO list contains enough usable pins.

## License

MIT License. See [LICENSE](LICENSE).
