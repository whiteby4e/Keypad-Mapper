# Keypad Mapper

A small Arduino/ESP32 utility for identifying the pin pairs of an unknown **4x4 matrix keypad**.

It scans a set of eight GPIO pins, detects which two pins are connected by each key, records all 16 connections, and prints an Arduino Keypad configuration to the Serial Monitor.

## Features

- Designed for 4x4 matrix keypads
- Automatically detects the 16 key connections
- Works with configurable GPIO candidates
- Uses internal pull-up resistors
- Includes simple debounce handling
- Prevents duplicate connections
- Prints a ready-to-copy Arduino Keypad configuration
- No display or extra hardware required

## Hardware

- ESP32 or another Arduino-compatible board
- 4x4 matrix keypad
- USB cable

### Default GPIO candidates

The sketch currently tests:

```text
13, 12, 27, 26, 25, 33, 32, 23
```

Change the `PINS` array in `Keypad_Mapper.ino` if you want to use different GPIOs.

## Wiring

A typical 4x4 matrix keypad has eight connections: four rows and four columns.

Connect the eight keypad wires to the eight GPIOs listed in `PINS[]`. The physical order of the wires does not need to be known beforehand.

**Do not connect the keypad to 5V.** The mapper uses the GPIO internal pull-ups and reads the matrix directly.

## Usage

1. Upload `Keypad_Mapper.ino` to the board.
2. Open Arduino IDE Serial Monitor.
3. Set the baud rate to **115200**.
4. Press every keypad key once.
5. The mapper prints each detected connection.
6. After all 16 keys are detected, it prints the detected row/column arrays.
7. Copy those arrays into your Arduino Keypad project.

Example output:

```text
=== Keypad Mapper ===
Target: 4x4 matrix keypad
Press every key once.

Key 1: 13 <-> 27
Key 2: 13 <-> 26
...
Key 16: 23 <-> 25

=== Finished ===
Detected connections: 16

=== Arduino Keypad Configuration ===
byte rowPins[ROWS] = {
  ...
};

byte colPins[COLS] = {
  ...
};
```

## Notes

- This project is intentionally focused on **4x4 matrix keypads**.
- The eight candidate GPIOs must be suitable for digital input/output on your board.
- Avoid GPIOs that are unavailable, input-only, or reserved by your board.
- On ESP32 boards, check the specific board's GPIO restrictions before changing `PINS[]`.
- The mapper identifies the electrical matrix connections; it does not identify the printed key labels such as 1, 2, 3, A, etc.

## License

MIT License. See [LICENSE](LICENSE).
