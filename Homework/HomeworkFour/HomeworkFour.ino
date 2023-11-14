const int latchPinNumber = 11;
const int clockPinNumber = 10;
const int dataPinNumber = 12;

int displayDigitPins[] = { 4, 5, 6, 7 };
const int numberOfDisplays = 4;
const int numberOfEncodings = 10;

byte digitEncodings[numberOfEncodings][2] = {
  { B11111100, B11111101 },  // 0
  { B01100000, B01100001 },  // 1
  { B11011010, B11011011 },  // 2
  { B11110010, B11110011 },  // 3
  { B01100110, B01100111 },  // 4
  { B10110110, B10110111 },  // 5
  { B10111110, B10111111 },  // 6
  { B11100000, B11100001 },  // 7
  { B11111110, B11111111 },  // 8
  { B11110110, B11110111 }   // 9
};
unsigned long lastIncrement = 0;
unsigned long delayCount = 100;
unsigned long displayedNumber = 0;
const int buttonOne = 2;
const int buttonTwo = 3;
const int buttonThree = 9;
int debounceDelay = 300;

byte startStopButtonState;
byte startStopButtonLastState = HIGH;
byte startStopButtonActive = LOW;
long startStopButtonLastDebounceTime = 0;

byte resetButtonState;
byte resetButtonLastState = HIGH;
byte resetButtonActive = LOW;
long resetButtonLastDebounceTime = 0;

byte flagButtonState;
byte flagButtonLastState = HIGH;
byte flagButtonActive = LOW;
long flagButtonLastDebounceTime = 0;

const int lapArraySize = 4;
int lapArray[lapArraySize];
int currentLapIndex = -1;

void setup() {
  pinMode(latchPinNumber, OUTPUT);
  pinMode(clockPinNumber, OUTPUT);
  pinMode(dataPinNumber, OUTPUT);

  for (int i = 0; i < numberOfDisplays; i++) {
    pinMode(displayDigitPins[i], OUTPUT);
    digitalWrite(displayDigitPins[i], LOW);
  }

  pinMode(buttonOne, INPUT_PULLUP);
  pinMode(buttonTwo, INPUT_PULLUP);
  pinMode(buttonThree, INPUT_PULLUP);

  Serial.begin(9600);
}

void loop() {
  startStop();
  resetButton();
  Lap();
  writeNumber(displayedNumber);
}

void addLap(int value) {
  currentLapIndex = (currentLapIndex + 1) % lapArraySize;
  lapArray[currentLapIndex] = value;
}

void printLapArray() {
  if (currentLapIndex >= 0) {
    displayedNumber = lapArray[currentLapIndex];
    currentLapIndex = (currentLapIndex - 1 + lapArraySize) % lapArraySize;
  } else {
    currentLapIndex = lapArraySize - 1;
    printLapArray();
  }
}

void writeDigit(int digit) {
  digitalWrite(latchPinNumber, LOW);
  shiftOut(dataPinNumber, clockPinNumber, MSBFIRST, digit);
  digitalWrite(latchPinNumber, HIGH);
}

void writeNumber(int number) {
  if (number == 0) {
    for (int i = 0; i < numberOfDisplays; i++) {
      activateDisplay(i);
      writeDigit(digitEncodings[0][i == 2 ? 1 : 0]);
      delay(0);
      writeDigit(B00000000);
    }
  } else {
    int displayDigit = 3;
    while (number != 0) {
      int lastDigit = number % 10;
      activateDisplay(displayDigit);

      if (displayDigit == 2) {
        writeDigit(digitEncodings[lastDigit][1]);
      } else {
        writeDigit(digitEncodings[lastDigit][0]);
      }

      delay(0);
      writeDigit(B00000000);
      displayDigit--;
      number /= 10;
    }
    initDisplay(displayDigit);
  }
}

void startStop() {
  startStopButtonState = digitalRead(buttonOne);

  if (millis() - startStopButtonLastDebounceTime > debounceDelay && startStopButtonState == LOW) {
    if (startStopButtonState != startStopButtonLastState) {
      startStopButtonActive = !startStopButtonActive;
      startStopButtonLastDebounceTime = millis();
    }
  }
  startStopButtonLastState = startStopButtonState;

  if (millis() - lastIncrement > delayCount && startStopButtonActive == 1) {
    displayedNumber++;
    displayedNumber %= 10000;
    lastIncrement = millis();
  }
}

void resetButton() {
  resetButtonState = digitalRead(buttonTwo);

  if (millis() - resetButtonLastDebounceTime > debounceDelay && resetButtonState == LOW) {
    if (startStopButtonActive == LOW) {
      displayedNumber = 0;
      resetButtonActive = !resetButtonActive;
      Serial.println("Active");
    }

    resetButtonLastDebounceTime = millis();
    resetButtonLastState = resetButtonState;
  }
}

void Lap() {
  flagButtonState = digitalRead(buttonThree);

  if (millis() - flagButtonLastDebounceTime > debounceDelay && flagButtonState == LOW) {
    if (resetButtonActive == LOW) {
      addLap(displayedNumber);
    } else {
      printLapArray();
    }

    flagButtonLastDebounceTime = millis();
    flagButtonLastState = flagButtonState;
  }
}

void initDisplay(int phase) {
  for (int i = phase; i >= 0; i--) {
    activateDisplay(i);
    writeDigit(B11111100);
    delay(0);
    writeDigit(B00000000);
  }
}

void activateDisplay(int displayNumber) {
  for (int i = 0; i < numberOfDisplays; i++) {
    if (i == displayNumber) {
      digitalWrite(displayDigitPins[i], LOW);
    } else {
      digitalWrite(displayDigitPins[i], HIGH);
    }
  }
}
