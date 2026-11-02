<div align="center">

# Keypad Mapper

**Find the row and column pins of an unknown matrix keypad.**

An Arduino utility that detects electrical connections between keypad wires and prints ready-to-copy row/column arrays for the Arduino Keypad library.

![Platform](https://img.shields.io/badge/Platform-ESP32%20%2F%20Arduino-blue)
![License](https://img.shields.io/badge/License-MIT-green)

</div>

---

## 🔍 What problem does it solve?

When a matrix keypad has unlabeled wires, identifying the row and column order can be tedious. Keypad Mapper scans a configurable set of GPIOs, records the pin pairs connected by each key, and derives a row/column configuration when the expected matrix is complete.

It identifies **electrical connections**, not the printed key labels (1, 2, A, etc.).

## ✨ Supported matrix sizes

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

A typical 1×4 keypad with four independent button wires is **not** a matrix keypad and is not supported by this version.

## ⚡ Quick start

1. Open `Keypad_Mapper.ino` in Arduino IDE.
2. Set the matrix dimensions.
3. Connect the keypad wires to the selected GPIO candidates.
4. Select your board and upload.
5. Open Serial Monitor at **115200 baud**.
6. Press every key once.
7. Copy the generated row and column arrays into your keypad project.

## ⚙️ Configuration

Set the matrix size near the top of the sketch:

```cpp
constexpr uint8_t KEYPAD_ROWS = 4;
constexpr uint8_t KEYPAD_COLS = 4;
```

For a 2×4 keypad:

```cpp
constexpr uint8_t KEYPAD_ROWS = 2;
constexpr uint8_t KEYPAD_COLS = 4;
```

The number of GPIOs required is always:

```text
ROWS + COLS
```

Examples: 2×4 needs 6 GPIOs, 3×4 needs 7, and 4×4 needs 8.

## 🔌 Wiring

The default GPIO candidates are:

```text
13, 12, 27, 26, 25, 33, 32, 23
```

Connect the keypad wires to these GPIOs in any physical order. For smaller matrices, the sketch uses the first `ROWS + COLS` entries.

**Do not connect the keypad wires to 5V.** The sketch uses the board's internal pull-ups and directly scans GPIO connections. Check the logic-level and pin restrictions of your exact board before wiring.

## 🖨️ Example output

After all expected keys are detected, the sketch prints the discovered connections and, if a complete matrix can be determined, an Arduino Keypad configuration:

```cpp
byte rowPins[ROWS] = {
  // detected GPIOs
};

byte colPins[COLS] = {
  // detected GPIOs
};
```

The actual pin values depend on your keypad and wiring.

## ⚠️ Important notes

- This version supports matrix keypads only.
- Press every key once so every row/column intersection can be detected.
- GPIO candidates must support the required input and output modes.
- Avoid input-only, reserved, or board-specific restricted pins.
- If the mapper cannot determine a complete matrix, check the dimensions, wiring, and whether every key was pressed.

## 📄 License

MIT — see [LICENSE](LICENSE).
