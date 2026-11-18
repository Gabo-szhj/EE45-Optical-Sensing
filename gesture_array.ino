// Transcribed from Adeli and Zhang, Hand Gesture Recognition, 12 June 2025, pp. 6–7.
const int numRows = 7;
const int numCols = 7;
const int numPhotodiodes = numRows * numCols;
const int mosfetPins[numPhotodiodes] = {
  7, 8, 9, 10, 11, 12, 13,
  50, 51, 2, 3, 4, 5, 6,
  20, 19, 18, 17, 16, 15, 14,
  22, 24, 26, 28, 30, 32, 34,
  36, 38, 40, 42, 44, 46, 48,
  23, 25, 27, 29, 31, 33, 35,
  37, 39, 41, 43, 45, 47, 49
};
unsigned long timestamp = millis();
const int tiaPin = A0;
float pixelData[numRows][numCols];

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println("Ready to receive");
  for (int i = 0; i < numPhotodiodes; i++) {
    pinMode(mosfetPins[i], OUTPUT);
    digitalWrite(mosfetPins[i], LOW);
  }
}

void loop() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();
    if (command == "start") {
      Serial.println("Start command received!");
      for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
          int index = row * numCols + col;
          long sum = 0;
          digitalWrite(mosfetPins[index], HIGH);
          delay(17);
          for (int i = 0; i < 166; i++) {
            sum += analogRead(tiaPin);
          }
          float average = sum / 166.0;
          pixelData[row][col] = average * 5.0 / 1023.0;
          digitalWrite(mosfetPins[index], LOW);
          delay(17);
        }
      }
      for (int row = 0; row < numRows; row++) {
        for (int col = 0; col < numCols; col++) {
          Serial.print(pixelData[row][col]);
          if (!(row == numRows - 1 && col == numCols - 1)) {
            Serial.print(',');
          }
        }
      }
      Serial.println();
    }
  }
}
