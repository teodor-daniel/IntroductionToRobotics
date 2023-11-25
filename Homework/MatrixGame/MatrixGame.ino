
#include "LedControl.h" // Include LedControl library for controlling the LED matrix
const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;

const int xPin = A0;
const int yPin = A1;
// Create an LedControl object to manage the LED matrix
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1); // DIN, CLK, LOAD, No. DRIVER
// Variable to set the brightness level of the matrix
byte matrixBrightness = 2;
// Variables to track the current and previous positions of the joystick-controlled LED
byte xPos = 0;
byte yPos = 0;
byte xLastPos = 0;
byte yLastPos = 0;
// Thresholds for detecting joystick movement
const int minThreshold = 200;
const int maxThreshold = 600;

const byte moveInterval = 100; // Timing variable to control the speed of LED movement
unsigned long long lastMoved = 0; // Tracks the last time the LED moved

const byte matrixSize = 8 ;// Size of the LED matrix
bool matrixChanged = true; // Flag to track if the matrix display needs updating
// 2D array representing the state of each LED (on/off) in the matrix
byte matrix[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}  
};
// Array representing each row of the LED matrix as a byte
byte matrixByte[matrixSize] = {
  B00000000,
  B01000100,
  B00101000,
B00010000,
  B00010000,
  B00010000,
  B00000000,
  B00000000
};
byte matrixMap[matrixSize][matrixSize] = {
  {0, 0, 0, 0, 0, 0, 0, 0},
  {1, 0, 0, 1, 0, 1, 0, 0},
  {0, 1, 0, 0, 0, 1, 0, 0},
  {0, 1, 0, 1, 0, 1, 0, 0},
  {0, 1, 0, 0, 0, 1, 0, 0},
  {0, 1, 0, 1, 0, 1, 0, 0},
  {0, 0, 0, 0, 0, 0, 1, 0},
  {0, 0, 0, 0, 0, 0, 0, 0}  
};
void setup() {
  Serial.begin(9600);
  // the zero refers to the MAX7219 number, it is zero for 1 chip
  lc.shutdown(0, false); // turn off power saving, enables display
  lc.setIntensity(0, matrixBrightness); // sets brightness (0~15 possible values)
  lc.clearDisplay(0); // Clear the matrix display
  matrix[xPos][yPos] = 1; // Initialize the starting position of the LED
  //also here initialize the map of the matrix : the map of the matrix is here
    // Initialize the starting position of the LED
  matrix[xPos][yPos] = 1;

  // Update the LED matrix display
  updateMatrix();
}
void loop() {
  // Check if it's time to update the LED position
  if (millis() - lastMoved >= moveInterval) {
    // Update the LED position
    updatePositions();
    
    // Reset the timer
    lastMoved = millis();
  }

  // Check if the matrix display needs to be updated based on LED position change
  if (matrixChanged) {
    updateMatrix();
    matrixChanged = false; // Reset the flag
  }
}


void updateByteMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    lc.setRow(0, row, matrixByte[row]);  // set each ROW (or COL) at the same time
  }
}

void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      // Use logical OR (||) to combine the character's position and the fixed map
      lc.setLed(0, row, col, matrix[row][col] || matrixMap[row][col]);
    }
  }
}

void updatePositions() {
  // Read analog values from the joystick (X and Y axes)
  int xValue = analogRead(xPin);
  int yValue = analogRead(yPin);

  // Calculate the potential new positions
  byte newXPos = xPos;
  byte newYPos = yPos;

  if (xValue < minThreshold && newXPos > 0) {
    newXPos--;
  } else if (xValue > maxThreshold && newXPos < matrixSize - 1) {
    newXPos++;
  }

  if (yValue < minThreshold && newYPos < matrixSize - 1) {
    newYPos++;
  } else if (yValue > maxThreshold && newYPos > 0) {
    newYPos--;
  }

  // Check if the target position is clear
  if (matrix[newXPos][newYPos] == 0 && matrixMap[newXPos][newYPos] == 0) {
    // Update the matrixChanged flag and the matrix array
    matrixChanged = true;
    matrix[xPos][yPos] = 0; // Clear the current position
    matrix[newXPos][newYPos] = 1; // Set the new position

    // Update the last positions
    xLastPos = xPos;
    yLastPos = yPos;

    // Update the current positions
    xPos = newXPos;
    yPos = newYPos;
  }
}

