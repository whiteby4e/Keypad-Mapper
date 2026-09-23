/*
 * Keypad Mapper
 * Automatically detects the row/column pin pairs of a 4x4 matrix keypad.
 *
 * Default wiring candidates:
 * 13, 12, 27, 26, 25, 33, 32, 23
 *
 * Open Serial Monitor at 115200 baud.
 * Press every key once. The detected matrix connections are printed
 * and, after all 16 keys are found, an Arduino Keypad configuration
 * snippet is generated.
 */

#include <Arduino.h>

constexpr uint8_t PIN_COUNT = 8;
constexpr uint8_t KEY_COUNT = 16;
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

void printGeneratedCode() {
  uint8_t rows[4];
  uint8_t cols[4];
  uint8_t rowCount = 0;
  uint8_t colCount = 0;

  /*
   * A valid 4x4 matrix is a complete bipartite graph:
   * every row is connected to all four columns.
   * Use the first detected pin as one side of the matrix,
   * then use its four connected pins as the opposite side.
   */
  const uint8_t referencePin = PINS[0];

  for (uint8_t i = 0; i < foundCount; ++i) {
    if (found[i].first == referencePin) {
      if (colCount < 4) cols[colCount++] = found[i].second;
    } else if (found[i].second == referencePin) {
      if (colCount < 4) cols[colCount++] = found[i].first;
    }
  }

  if (colCount == 4) {
    rows[rowCount++] = referencePin;

    for (uint8_t p = 0; p < PIN_COUNT && rowCount < 4; ++p) {
      const uint8_t candidate = PINS[p];
      bool isColumn = false;

      for (uint8_t c = 0; c < colCount; ++c) {
        if (candidate == cols[c]) {
          isColumn = true;
          break;
        }
      }

      if (!isColumn) {
        rows[rowCount++] = candidate;
      }
    }
  }

  Serial.println();
  Serial.println("=== Finished ===");
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

  if (rowCount == 4 && colCount == 4) {
    Serial.println("byte rowPins[ROWS] = {");
    Serial.print("  ");
    for (uint8_t i = 0; i < 4; ++i) {
      Serial.print(rows[i]);
      if (i < 3) Serial.print(", ");
    }
    Serial.println();
    Serial.println("};");

    Serial.println("byte colPins[COLS] = {");
    Serial.print("  ");
    for (uint8_t i = 0; i < 4; ++i) {
      Serial.print(cols[i]);
      if (i < 3) Serial.print(", ");
    }
    Serial.println();
    Serial.println("};");
  } else {
    Serial.println("Could not determine a valid 4x4 matrix.");
    Serial.println("Check the keypad wiring and GPIO list.");
  }

  Serial.println();
  Serial.println("Mapper stopped.");
}

void setup() {
  Serial.begin(115200);
  delay(500);

  Serial.println();
  Serial.println("=== Keypad Mapper ===");
  Serial.println("Target: 4x4 matrix keypad");
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
