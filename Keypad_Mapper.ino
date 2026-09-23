const byte pins[8] = {13,12,27,26,25,33,32,23};
int found[16][2];
int count = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("=== Auto Keypad Mapper ===");
  Serial.println("Press every key once...");
}

void loop() {

  for (int a = 0; a < 8; a++) {

    for (int i = 0; i < 8; i++) {
      pinMode(pins[i], INPUT_PULLUP);
    }

    pinMode(pins[a], OUTPUT);
    digitalWrite(pins[a], LOW);

    for (int b = 0; b < 8; b++) {

      if (a == b) continue;

      if (digitalRead(pins[b]) == LOW) {

        bool exists = false;

        for (int x = 0; x < count; x++) {
          if ((found[x][0] == pins[a] && found[x][1] == pins[b]) ||
              (found[x][0] == pins[b] && found[x][1] == pins[a])) {
            exists = true;
          }
        }

        if (!exists && count < 16) {
          found[count][0] = pins[a];
          found[count][1] = pins[b];

          Serial.print("Key connection: ");
          Serial.print(pins[a]);
          Serial.print(" <-> ");
          Serial.println(pins[b]);

          count++;
        }

        delay(300);
      }
    }
  }

  if (count >= 16) {
    Serial.println("\n=== Finished ===");
    while(1);
  }
}
