#include "LedControl.h"
#include <EEPROM.h>

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
//controller
const int xPin = A0;
const int yPin = A1;
const int pinSW = A2;


LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
int selectedMap = 0;
byte matrixBrightness = 2;

const int numMaps = 3;  // Number of matrix maps
byte matrixMap[numMaps][8][8] = {
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 1, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 1, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 1, 0, 0},
    {0, 1, 0, 1, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 1, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 1, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 1, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 1, 0, 1, 0, 0},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  }
};

byte matrixSize = 8;
byte xPos = 0;
byte yPos = 0;
byte xLastPos = 0;
byte yLastPos = 0;

const int minThreshold = 200;
const int maxThreshold = 600;

const byte moveInterval = 100;
unsigned long long lastMoved = 0;
bool matrixChanged = true;

void setup() {
  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);
  pinMode(pinSW, INPUT_PULLUP);
  // Read the selected map from EEPROM and increment by 1
  selectedMap = EEPROM.read(0);
  selectedMap = (selectedMap + 1) % numMaps;
  EEPROM.write(0, selectedMap);  // Save the updated selected map to EEPROM

  Serial.println(selectedMap);

  // Initialize the starting position of the LED
  matrixMap[selectedMap][xPos][yPos] = 1;

  // Update the LED matrix display
  updateMatrix();
}

void loop() {
  if (millis() - lastMoved >= moveInterval) {
    updatePositions();
    lastMoved = millis();
  }
  Serial.print("selectedMap = ");
  Serial.println(selectedMap);

  if (matrixChanged) {
    updateMatrix();
    matrixChanged = false;
  }

  // Call blink function for the current position
  blink(xPos, yPos);
}

void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrixMap[selectedMap][row][col]);
    }
  }
}

void updatePositions() {
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);
  int hasMoved = false;
  byte newXPos = xPos;
  byte newYPos = yPos;

  if (xValue < minThreshold && newXPos > 0 && hasMoved == false) {
    newXPos--;
    hasMoved = true;
  } else if (xValue > maxThreshold && newXPos < matrixSize - 1 && hasMoved == false) {
    newXPos++;
        hasMoved = true;

  }

  if (yValue < minThreshold && newYPos < matrixSize - 1 && hasMoved == false) {
    newYPos++;    hasMoved = true;

  } else if (yValue > maxThreshold && newYPos > 0 && hasMoved == false) {
    newYPos--;    hasMoved = true;

  }

  if (matrixMap[selectedMap][newXPos][newYPos] == 0) {
    matrixChanged = true;
    matrixMap[selectedMap][xPos][yPos] = 0;
    matrixMap[selectedMap][newXPos][newYPos] = 1;
    xLastPos = xPos;
    yLastPos = yPos;
    xPos = newXPos;
    yPos = newYPos;
  }
}

void blink(byte x, byte y) {
  static unsigned long lastBlinkTime = 0;
  static bool isOn = true;
  const unsigned long blinkInterval = 400; // Adjust the blink interval as needed

  if (millis() - lastBlinkTime >= blinkInterval) {
    // Toggle the blink state
    isOn = !isOn;
    lc.setLed(0, x, y, isOn);
    
    // Update the last blink time
    lastBlinkTime = millis();
  }
}

