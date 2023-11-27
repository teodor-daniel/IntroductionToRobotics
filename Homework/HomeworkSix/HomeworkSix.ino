#include "LedControl.h"
#include <EEPROM.h>

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
//controller
const int xPin = A0;
const int yPin = A1;
const int pinSW = 7;

boolean lastButtonState = false;

LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
int selectedMap = 0;
byte matrixBrightness = 2;

const int numMaps = 5;  // Number of matrix maps to do more and select random 
byte matrixMap[numMaps][8][8] = {
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 1, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0}
  },
  {
    {0, 0, 0, 1, 0, 1, 0, 1},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {0, 1, 0, 0, 0, 0, 1, 0},
    {1, 1, 1, 1, 1, 1, 1, 1},
    {1, 0, 0, 0, 0, 0, 0, 1},
    {0, 1, 1, 1, 1, 1, 1, 0}
  },
  {
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 1, 1, 0, 0, 0, 0},
    {0, 0, 1, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 1},
    {0, 0, 0, 0, 1, 1, 0, 1},
    {0, 0, 0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 1, 1, 0, 1}
  },
{
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0},
    {0, 1, 0, 1, 1, 0, 1, 0},
    {0, 0, 0, 1, 1, 0, 1, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 1, 1, 0, 0, 0}
},
{
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 0, 1, 1, 1, 0, 0},
    {0, 0, 1, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1, 1, 0},
    {1, 0, 1, 0, 0, 0, 1, 0},
    {1, 0, 1, 0, 0, 0, 1, 0},
    {1, 0, 1, 1, 1, 1, 1, 0}
}
};
byte matrixSize = 8;
byte xPos = 0;
byte yPos = 0;
byte xLastPos = 0;
byte yLastPos = 0;
const int buzzerPin = 9; 
const int minThreshold = 200;
const int maxThreshold = 600;
const int debounceDelay = 50; 
unsigned long lastButtonPressTime = 0;

const byte moveInterval = 100;
unsigned long long lastMoved = 0;
bool matrixChanged = true;


int xLast;
int yLast;

  boolean buttonState = false;
  boolean exist = false;
  int xBlink = -1;
  int yBlink = -1;
 int lastPositionSetTime;

void setup() {

  Serial.begin(9600);
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);
  pinMode(pinSW, INPUT_PULLUP);
  selectedMap = EEPROM.read(0);
  selectedMap = (selectedMap + 1) % numMaps;
  EEPROM.write(0, selectedMap); 
//generateRandomMap(matrixMap[selectedMap]);
  Serial.println(selectedMap);

  pinMode(buzzerPin, OUTPUT);
  matrixMap[selectedMap][xPos][yPos] = 1;

  updateMatrix();
}


void loop() {
  boolean currentButtonState = digitalRead(pinSW);

  if (currentButtonState != lastButtonState) {
    lastButtonPressTime = millis();
  }
  if ((millis() - lastButtonPressTime) > debounceDelay) {
    if (currentButtonState == LOW) {
      buttonState = !buttonState;
    }
  }
  lastButtonState = currentButtonState;

  if (millis() - lastMoved >= moveInterval) {
    xLast = xPos;
    yLast = yPos;
    updatePositions();

    if ((xLast != xPos || yLast != yPos) && buttonState == true) {
      //debuging
      // Serial.print("X: ");
      // Serial.println(xLast);
      // Serial.print("Y: ");
      // Serial.println(yLast);
      matrixMap[selectedMap][xLast][yLast] = 1;
      lc.setLed(1, xLast, yLast, matrixMap[selectedMap][xLast][yLast]);

      lastPositionSetTime = millis();

      exist = true;
      xBlink = xLast;
      yBlink = yLast;
      buttonState = false;
    }
    lastMoved = millis();
  }
  //bomb starts blinking if it exists
  if (exist) {
    blinkFast(xBlink, yBlink);
  }
  
  if (exist && millis() - lastPositionSetTime >= 3000) {
    matrixMap[selectedMap][xBlink][yBlink] = 0;
    lc.setLed(1, xBlink, yBlink, matrixMap[selectedMap][xBlink][yBlink]);

    if (xBlink > 0) {
      //destroy the walls
      matrixMap[selectedMap][xBlink - 1][yBlink] = 0;
      lc.setLed(1, xBlink - 1, yBlink, matrixMap[selectedMap][xBlink - 1][yBlink]);
    //you die if you stand too close to the bomb and you go back to spawn;
      if (xPos == xBlink - 1 && yBlink == yPos) {
        yPos = 0;
        xPos = 0;
      }
    }

    if (xBlink < (8 - 1)) {
      matrixMap[selectedMap][xBlink + 1][yBlink] = 0;
      lc.setLed(1, xBlink + 1, yBlink, matrixMap[selectedMap][xBlink + 1][yBlink]);

      if (xPos == xBlink + 1 && yBlink == yPos) {
        yPos = 0;
        xPos = 0;
      }
    }

    if (yBlink > 0) {
      matrixMap[selectedMap][xBlink][yBlink - 1] = 0;
      lc.setLed(1, xBlink, yBlink - 1, matrixMap[selectedMap][xBlink][yBlink - 1]);

      if (xPos == xBlink && yBlink - 1 == yPos) {
        yPos = 0;
        xPos = 0;
      }
    }

    if (yBlink < (8 - 1)) {
      matrixMap[selectedMap][xBlink][yBlink + 1] = 0;
      lc.setLed(1, xBlink, yBlink + 1, matrixMap[selectedMap][xBlink][yBlink + 1]);

      if (xPos == xBlink && yBlink + 1 == yPos) {
        yPos = 0;
        xPos = 0;
      }
    }

    exist = false;
    playBuzzer();
    updateMatrix();
  }

  if (matrixChanged) {
    updateMatrix();
    matrixChanged = false;
  }

  blink(xPos, yPos);

}

void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrixMap[selectedMap][row][col]);
    }
  }
}

void playBuzzer() {
  tone(buzzerPin, 2000, 500);  
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
    newYPos++;
    hasMoved = true;

  } else if (yValue > maxThreshold && newYPos > 0 && hasMoved == false) {
    newYPos--;
    hasMoved = true;

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

void blink(byte x, byte y ) {
  static unsigned long lastBlinkTime = 0;
  static bool isOn = true;
  static unsigned blinkInterval = 400;
  if (millis() - lastBlinkTime >= blinkInterval) {
    isOn = !isOn;
    lc.setLed(0, x, y, isOn);
    
    lastBlinkTime = millis();
  }
}
//i know i should use blink with parameers but it was easier for me to change it here i also wanted to add the logic here but i let it in loop
void blinkFast(byte x, byte y) {
  static unsigned long lastBlinkTime = 0;
  static bool isOn = true;
  static unsigned blinkInterval = 200;
  if (millis() - lastBlinkTime >= blinkInterval) {
    isOn = !isOn;
    lc.setLed(0, x, y, isOn);
    
    lastBlinkTime = millis();
  }
}
//here i could generate a random matrix but it seems more fun to have levels
void generateRandomMap(byte matrix[8][8]) {
  for (byte i = 0; i < matrixSize; ++i) {
    for (byte j = 0; j < matrixSize; ++j) {
      matrix[i][j] = random(2);  
    }
  }
  matrix[0][0] = 0;
  matrix[1][0] = 0; //to be able to escape any random generated levels you need at least 3 spaces
  matrix[2][0] = 0;
}


