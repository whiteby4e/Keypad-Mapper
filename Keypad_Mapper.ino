/*
 * Keypad Mapper
 * Detects electrical connections of common matrix keypads.
 *
 * Supported matrix sizes:
 *   2x2, 2x3, 2x4
 *   3x2, 3x3, 3x4
 *   4x2, 4x3, 4x4
 *
 * Set KEYPAD_ROWS and KEYPAD_COLS below before uploading.
 * The total number of GPIOs must be ROWS + COLS.
 *
 * Default GPIO candidates:
 *   13, 12, 27, 26, 25, 33, 32, 23
 *
 * Open Serial Monitor at 115200 baud and press every key once.
 */

#include <Arduino.h>

constexpr uint8_t KEYPAD_ROWS = 4;
constexpr uint8_t KEYPAD_COLS = 4;

constexpr uint8_t PIN_COUNT = KEYPAD_ROWS + KEYPAD_COLS;
constexpr uint8_t KEY_COUNT = KEYPAD_ROWS * KEYPAD_COLS;
constexpr unsigned long DEBOUNCE_MS = 50;

const uint8_t PINS[PIN_COUNT] = {
  13, 12, 27, 26, 25, 33, 32, 23
};

struct KeyConnection {
  uint8_t first;
  uint8_t second;
};

KeyConnection found[KEY_COUNT];
uint8_t foundCount = 0;

bool connectionExists(uint8_t first, uint8_t second) {
  for (uint8_t i = 0; i < foundCount; ++i) {
    if ((found[i].first == first && found[i].second == second) ||
        (found[i].first == second && found[i].second == first)) {
      return true;
    }
  }
  return false;
}

bool keyIsPressed(uint8_t firstIndex, uint8_t secondIndex) {
  for (uint8_t i = 0; i < PIN_COUNT; ++i) {
    pinMode(PINS[i], INPUT_PULLUP);
  }

  pinMode(PINS[firstIndex], OUTPUT);
  digitalWrite(PINS[firstIndex], LOW);

  return digitalRead(PINS[secondIndex]) == LOW;
}

void addConnection(uint8_t first, uint8_t second) {
  if (foundCount >= KEY_COUNT || connectionExists(first, second)) {
    return;
  }

  found[foundCount] = {first, second};

  Serial.print("Key ");
  Serial.print(foundCount + 1);
  Serial.print(": ");
  Serial.print(first);
  Serial.print(" <-> ");
  Serial.println(second);

  ++foundCount;
}

bool containsPin(const uint8_t pins[], uint8_t count, uint8_t value) {
  for (uint8_t i = 0; i < count; ++i) {
    if (pins[i] == value) return true;
  }
  return false;
}

void printArray(const char* name, const uint8_t pins[], uint8_t count) {
  Serial.print(name);
  Serial.println(" = {");

  Serial.print("  ");
  for (uint8_t i = 0; i < count; ++i) {
    Serial.print(pins[i]);
    if (i + 1 < count) Serial.print(", ");
  }
  Serial.println();
  Serial.println("};");
}

void printGeneratedCode() {
  uint8_t rows[KEYPAD_ROWS];
  uint8_t cols[KEYPAD_COLS];
  uint8_t rowCount = 0;
  uint8_t colCount = 0;

  /*
   * A complete matrix is a bipartite graph:
   * each row connects to every column.
   *
   * Use the first configured GPIO as a reference row.
   * Its detected connections are the columns.
   */
  const uint8_t referencePin = PINS[0];

  for (uint8_t i = 0; i < foundCount; ++i) {
    if (found[i].first == referencePin) {
      if (!containsPin(cols, colCount, found[i].second) &&
          colCount < KEYPAD_COLS) {
        cols[colCount++] = found[i].second;
      }
    } else if (found[i].second == referencePin) {
      if (!containsPin(cols, colCount, found[i].first) &&
          colCount < KEYPAD_COLS) {
        cols[colCount++] = found[i].first;
      }
    }
  }

  if (colCount == KEYPAD_COLS) {
    rows[rowCount++] = referencePin;

    for (uint8_t p = 0; p < PIN_COUNT && rowCount < KEYPAD_ROWS; ++p) {
      const uint8_t candidate = PINS[p];

      if (!containsPin(cols, colCount, candidate)) {
        rows[rowCount++] = candidate;
      }
    }
  }

  Serial.println();
  Serial.println("=== Finished ===");
  Serial.print("Keypad size: ");
  Serial.print(KEYPAD_ROWS);
  Serial.print("x");
  Serial.println(KEYPAD_COLS);

  Serial.print("Detected connections: ");
  Serial.println(foundCount);

  Serial.println();
  Serial.println("=== Detected Connections ===");
  for (uint8_t i = 0; i < foundCount; ++i) {
    Serial.print(found[i].first);
    Serial.print(" <-> ");
    Serial.println(found[i].second);
  }

  Serial.println();
  Serial.println("=== Arduino Keypad Configuration ===");

  if (rowCount == KEYPAD_ROWS && colCount == KEYPAD_COLS) {
    printArray("byte rowPins[ROWS]", rows, KEYPAD_ROWS);
    printArray("byte colPins[COLS]", cols, KEYPAD_COLS);

    Serial.println();
    Serial.println("Use these with the Arduino Keypad library.");
  } else {
    Serial.println("Could not determine a valid matrix.");
    Serial.println("Check the keypad wiring, GPIO list,");
    Serial.println("and KEYPAD_ROWS / KEYPAD_COLS settings.");
  }

  Serial.println();
  Serial.println("Mapper stopped.");
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("=== Keypad Mapper ===");
  Serial.print("Target: ");
  Serial.print(KEYPAD_ROWS);
  Serial.print("x");
  Serial.println(KEYPAD_COLS);

  Serial.print("Expected keys: ");
  Serial.println(KEY_COUNT);

  Serial.println("Press every key once.");
  Serial.println();

  for (uint8_t i = 0; i < PIN_COUNT; ++i) {
    pinMode(PINS[i], INPUT_PULLUP);
  }
}

void loop() {
  if (foundCount >= KEY_COUNT) {
    printGeneratedCode();

    while (true) {
      delay(1000);
    }
  }

  for (uint8_t a = 0; a < PIN_COUNT; ++a) {
    for (uint8_t b = 0; b < PIN_COUNT; ++b) {
      if (a == b || foundCount >= KEY_COUNT) {
        continue;
      }

      if (keyIsPressed(a, b)) {
        delay(DEBOUNCE_MS);

        if (keyIsPressed(a, b)) {
          addConnection(PINS[a], PINS[b]);

          while (keyIsPressed(a, b)) {
            delay(10);
          }
        }
      }
    }
  }
}
