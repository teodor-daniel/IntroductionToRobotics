#include <LiquidCrystal.h>
#include <EEPROM.h>
#include "LedControl.h"

const int dinPin = 12;
const int clockPin = 11;
const int loadPin = 10;
boolean lastButtonState = false;
LedControl lc = LedControl(dinPin, clockPin, loadPin, 1);
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

byte customChar[8] = {
	0b01110,
	0b01110,
	0b00100,
	0b11111,
	0b00100,
	0b01110,
	0b01010,
	0b01010
};

byte matrixSize = 8;
byte xPos = 0;
byte yPos = 0;
byte xLastPos = 0;
byte yLastPos = 0;
const int buzzerPin = 13; 
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

int selectedMap = 0;
const byte rsPin = 9;
const byte enPin = 8;
const byte d4Pin = 7;
const byte d5Pin = 6;
const byte d6Pin = 5;
const byte d7Pin = 4;
const byte Apin = 3;
const int xPin = A0;
const int yPin = A1;
const int pinSW = 2;
const int joyStickBtn = A2;
const int minThreshold = 300;
const int maxThreshold = 600;
const int debounceDelay = 50;

const int menuStart = 1;
const int menuGameOptions = 2;
const int menuMatrixOptions = 3;
const int menuLcdOptions = 4;
const int menuCredits = 5;
LiquidCrystal lcd(rsPin, enPin, d4Pin, d5Pin, d6Pin, d7Pin);
int selectedDifficulty; 
int matrixBrightness;
int lcdBrightness;
int currentMenu = 1;
int subMenu = 1;
int lives = 3;
bool exitMenu = false;

bool exitGame = false;

// Function prototypes
void updateMenu();
void executeAction();
void startGame();
void handleGameOptions();
void handleMatrixOptions();
void handleLcdOptions();
void showCredits();
void executeGameMenuAction();
void displayGameOptions();

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(Apin, OUTPUT); 
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(joyStickBtn, INPUT_PULLUP); 
  pinMode(buzzerPin, OUTPUT);

  lcdBrightness = EEPROM.read(2);
  analogWrite(Apin, lcdBrightness);  

  lcd.print("Boomber Man");
  lcd.setCursor(0, 1);
  lcd.print("By Teodor");
  delay(1000);
  lcd.clear();

  selectedDifficulty = EEPROM.read(0);
  matrixBrightness = EEPROM.read(1);
  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);

//Matrix
  matrixMap[selectedMap][xPos][yPos] = 1;
  selectedMap = EEPROM.read(3);
  selectedMap = (selectedMap + 1) % numMaps;
  EEPROM.write(3, selectedMap); 
  updateMenu();
}

void loop() {
  int joyValue = analogRead(xPin);

  if (joyValue < minThreshold) {
    currentMenu++;
    updateMenu();
    delay(debounceDelay);
    while (analogRead(xPin) < minThreshold);
  }

  if (joyValue > maxThreshold) {
    currentMenu--;
    updateMenu();
    delay(debounceDelay);
    while (analogRead(xPin) > maxThreshold);
  }

  if (!digitalRead(pinSW)) {
    executeAction();
    delay(debounceDelay);
    updateMenu();
    delay(debounceDelay);
    while (!digitalRead(pinSW));
  }


}

//Update display based on the joystick inpus
void updateMenu() {
  if (currentMenu < menuStart) currentMenu = menuCredits;
  if (currentMenu > menuCredits) currentMenu = menuStart;

  lcd.clear();

  switch (currentMenu) {
    case menuStart:
      lcd.print(">Start");
      lcd.setCursor(0, 1);
      lcd.print(" Game options");
      break;
    case menuGameOptions:
      lcd.print(" Start");
      lcd.setCursor(0, 1);
      lcd.print(">Game options");
      break;
    case menuMatrixOptions:
      lcd.print(">Matrix options");
      lcd.setCursor(0, 1);
      lcd.print(" LCD options");
      break;
    case menuLcdOptions:
      lcd.print(" Matrix options");
      lcd.setCursor(0, 1);
      lcd.print(">LCD options");
      break;
    case menuCredits:
      lcd.print(">CREDITS");
      break;
  }
}

//Switch for the actions
void executeAction() {
  switch (currentMenu) {
    case menuStart:
      startGame();
      break;
    case menuGameOptions:
      handleGameOptions();
      break;
    case menuMatrixOptions:
      handleMatrixOptions();
      break;
    case menuLcdOptions:
      handleLcdOptions();
      break;
    case menuCredits:
      showCredits();
      break;
  }
}


//Start game
void startGame() {
selectedMap = (selectedMap + 1) % numMaps;
lives = selectedDifficulty;
lcd.clear();
lcd.print("Loading...");
delay(400);
lcd.clear();
lcd.createChar(0, customChar);
exitGame = false;
while(exitGame == false){

  for (int i = 0; i < lives; i++) {
    lcd.setCursor(i, 0);  
    lcd.write((uint8_t) 0);  
  }
  boolean currentButtonState = digitalRead(pinSW);
  if(!digitalRead(joyStickBtn)) {
      exitGame = true;
      }

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
          lives--;
          xLast = 1;
          yLast = 1;
          xBlink = 3;
          xBlink = 3;
          exist = false;
        }

      }

      if (xBlink < (8 - 1)) {
        matrixMap[selectedMap][xBlink + 1][yBlink] = 0;
        lc.setLed(1, xBlink + 1, yBlink, matrixMap[selectedMap][xBlink + 1][yBlink]);

        if (xPos == xBlink + 1 && yBlink == yPos) {
          yPos = 0;
          xPos = 0;
          lives--;
          xLast = 1;
          yLast = 1;
          xBlink = 3;
          xBlink = 3;
          exist = false;
          lcd.clear();

        }
      }

      if (yBlink > 0) {
        matrixMap[selectedMap][xBlink][yBlink - 1] = 0;
        lc.setLed(1, xBlink, yBlink - 1, matrixMap[selectedMap][xBlink][yBlink - 1]);

        if (xPos == xBlink && yBlink - 1 == yPos) {
          yPos = 0;
          xPos = 0;
          lives--;
          xLast = 1;
          yLast = 1;
          xBlink = 3;
          xBlink = 3;
          exist = false;
          lcd.clear();
        }
      }

      if (yBlink < (8 - 1)) {
        matrixMap[selectedMap][xBlink][yBlink + 1] = 0;
        lc.setLed(1, xBlink, yBlink + 1, matrixMap[selectedMap][xBlink][yBlink + 1]);

        if (xPos == xBlink && yBlink + 1 == yPos) {
          yPos = 0;
          xPos = 0;
          lives--;
          xLast = 1;
          yLast = 1;
          xBlink = 3;
          xBlink = 3;
          exist = false;
          lcd.clear();

        }
      }

      exist = false;
      // playBuzzer();
      updateMatrix();
      lastPositionSetTime = 0;

    }

    if (matrixChanged) {
      updateMatrix();
      matrixChanged = false;
    }
    blink(xPos, yPos);
    if(lives == 0){
      exitGame = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("YOU DIED");
        delay(2000);           
      }
  if (areAllLedsOff(matrixMap[selectedMap], xPos, yPos)) {
        exitGame = true;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("YOU Win");
        delay(2000);  

      }
  
  }

  // EEPROM.write(3, selectedMap); 
  lc.clearDisplay(0);

  
}


//Game Options
void handleGameOptions() {
  lcd.clear();
  subMenu = 4;
  exitMenu = false;
  displayGameOptions();
  
  while (!exitMenu) {
    int joyValue = analogRead(xPin);

    if (joyValue < minThreshold) {
      subMenu++;
      displayGameOptions();
      delay(debounceDelay);
      while (analogRead(xPin) < minThreshold);
    }

    if (joyValue > maxThreshold) {
      subMenu--;
      displayGameOptions();
      delay(debounceDelay);
      while (analogRead(xPin) > maxThreshold);
    }

    if (!digitalRead(pinSW)) {
      executeGameMenuAction();
      delay(debounceDelay);
      displayGameOptions();
      delay(debounceDelay);
      while (!digitalRead(pinSW));
    }
  }
}

void displayGameOptions() {
  if (subMenu < 1) subMenu = 2;
  if (subMenu > 2 && subMenu != 4) {
    subMenu = 1;
  }
  if(subMenu > 4) subMenu = 1;
  
  lcd.clear();

  switch (subMenu) {
    case 1:
      lcd.print(">Difficulty");
      lcd.setCursor(0, 1);
      lcd.print(" Exit");
      break;
    case 2:
      lcd.print(" Difficulty");
      lcd.setCursor(0, 1);
      lcd.print(">Exit");
      break;
  }
}

void executeGameMenuAction() {
  switch (subMenu) {
    case 1:
      chooseDifficulty();
      break;
    case 2:
      exitMenu = true;
      break;
    case 4:
      lcd.clear();
      lcd.print("Loading...");
      delay(400);
      subMenu =1;
      break;
  }
}

void chooseDifficulty() {
  lcd.clear();
  lcd.print("Difficulty:");
  boolean exitThis = false;
  while (exitThis == false) { 
    int joyValue = analogRead(xPin);
     if(!digitalRead(joyStickBtn)) {
      exitThis = true;
      }
    if (joyValue < minThreshold) {
      selectedDifficulty++;
      if (selectedDifficulty > 10) {
        selectedDifficulty = 10;
      }
      lcd.setCursor(0, 1);
      lcd.setCursor(0, 1);
      lcd.print(selectedDifficulty);
      delay(debounceDelay);
      while (analogRead(xPin) < minThreshold);
    }

    if (joyValue > maxThreshold) {
      selectedDifficulty--;
      if (selectedDifficulty < 1) {
        selectedDifficulty = 1;
      }
      lcd.setCursor(0, 1);
      lcd.setCursor(0, 1);
      
      lcd.print(selectedDifficulty);
      delay(debounceDelay);
      while (analogRead(xPin) > maxThreshold);

    }
    EEPROM.write(0, selectedDifficulty);

    }
  delay(500); 
}

//Matrix Options
void handleMatrixOptions() {
  lcd.clear();
  subMenu = 4;
  exitMenu = false;
  displayMatrixOptions();
  
  while (!exitMenu) {
    int joyValue = analogRead(xPin);

    if (joyValue < minThreshold) {
      subMenu++;
      displayMatrixOptions();
      delay(debounceDelay);
      while (analogRead(xPin) < minThreshold);
    }

    if (joyValue > maxThreshold) {
      subMenu--;
      displayMatrixOptions();
      delay(debounceDelay);
      while (analogRead(xPin) > maxThreshold);
    }

    if (!digitalRead(pinSW)) {
      executeMatrixMenuAction();
      delay(debounceDelay);
      displayMatrixOptions();
      delay(debounceDelay);
      while (!digitalRead(pinSW));
    }
  }
}

void displayMatrixOptions() {
  if (subMenu < 1) subMenu = 2;
  if (subMenu > 2 && subMenu != 4) {
    subMenu = 1;
  }
  if(subMenu > 4) subMenu = 1;
  
  lcd.clear();

  switch (subMenu) {
    case 1:
      lcd.print(">Brightness");
      lcd.setCursor(0, 1);
      lcd.print(" Exit");
      break;
    case 2:
      lcd.print(" Brightness");
      lcd.setCursor(0, 1);
      lcd.print(">Exit");
      break;
  }
}

void executeMatrixMenuAction(){
    switch (subMenu) {
    case 1:
      chooseLightLevelMatrix();
      break;
    case 2:
      exitMenu = true;
      break;
    case 4:
      lcd.clear();
      lcd.print("Loading...");
      delay(400);
      subMenu =1;
      break;
  }
}

void chooseLightLevelMatrix(){
  lcd.clear();
  lcd.print("Brightness:");
  boolean exitThis = false;
  while (exitThis == false) { 
    int joyValue = analogRead(xPin);
     if(!digitalRead(joyStickBtn)) {
      exitThis = true;
      }
    if (joyValue < minThreshold) {
      matrixBrightness++;
      if (matrixBrightness > 10) {
        matrixBrightness = 10;
      }
      lcd.setCursor(0, 1);
      lcd.setCursor(0, 1);
      lcd.print(matrixBrightness);
      delay(debounceDelay);
      while (analogRead(xPin) < minThreshold);
    }

    if (joyValue > maxThreshold) {
      matrixBrightness--;
      if (matrixBrightness < 1) {
        matrixBrightness = 1;
      }
      lcd.setCursor(0, 1);
      lcd.setCursor(0, 1);
      
      lcd.print(matrixBrightness);
      delay(debounceDelay);
      while (analogRead(xPin) > maxThreshold);

    }
    EEPROM.write(1, matrixBrightness);

    }
  lc.setIntensity(0, matrixBrightness);
  matrixLight();
  delay(500); 
  lc.clearDisplay(0);
}


void matrixLight() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, 1);
    }
  }

  delay(2000);  

  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, 0); 
    }
  }
}


//Lcd options
void handleLcdOptions() {
  lcd.clear();
  subMenu = 4;
  exitMenu = false;
  displayLcdOptions();
  
  while (!exitMenu) {
    int joyValue = analogRead(xPin);

    if (joyValue < minThreshold) {
      subMenu++;
      displayLcdOptions();
      delay(debounceDelay);
      while (analogRead(xPin) < minThreshold);
    }

    if (joyValue > maxThreshold) {
      subMenu--;
      displayLcdOptions();
      delay(debounceDelay);
      while (analogRead(xPin) > maxThreshold);
    }

    if (!digitalRead(pinSW)) {
      executeLcdMenuAction();
      delay(debounceDelay);
      displayLcdOptions();
      delay(debounceDelay);
      while (!digitalRead(pinSW));
    }
  }

}
void displayLcdOptions(){
  if (subMenu < 1) subMenu = 2;
  if (subMenu > 2 && subMenu != 4) {
    subMenu = 1;
  }
  if(subMenu > 4) subMenu = 1;
  
  lcd.clear();

  switch (subMenu) {
    case 1:
      lcd.print(">LCD Brightness");
      lcd.setCursor(0, 1);
      lcd.print(" Exit");
      break;
    case 2:
      lcd.print(" LCD Brightness");
      lcd.setCursor(0, 1);
      lcd.print(">Exit");
      break;
  }
}

void executeLcdMenuAction(){
    switch (subMenu) {
    case 1:
      chooseLightLevelLcd();
      break;
    case 2:
      exitMenu = true;
      break;
    case 4:
      lcd.clear();
      lcd.print("Loading...");
      delay(400);
      subMenu =1;
      break;
  }
}

void chooseLightLevelLcd(){
  lcd.clear();
  lcd.print("Brightness:");
  boolean exitThis = false;
  while (exitThis == false) { 
    int joyValue = analogRead(xPin);
     if(!digitalRead(joyStickBtn)) {
      exitThis = true;
      }
    if (joyValue < minThreshold) {
      lcdBrightness += 100;
      if (lcdBrightness > 1000) {
        lcdBrightness = 1000;
      }
      lcd.setCursor(0, 1);
      lcd.setCursor(0, 1);
      lcd.print(lcdBrightness);
      delay(debounceDelay);
      while (analogRead(xPin) < minThreshold);
    }

    if (joyValue > maxThreshold) {
      lcdBrightness -= 100;
      if (lcdBrightness < 100) {
        lcdBrightness = 100;
      }
      lcd.setCursor(0, 1);
      lcd.setCursor(0, 1);
      
      lcd.print(lcdBrightness);
      delay(debounceDelay);
      while (analogRead(xPin) > maxThreshold);

    }
    EEPROM.write(2, lcdBrightness);
    }
  analogWrite(Apin, lcdBrightness);  
 
  delay(500); 
}

//Credits
void showCredits() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game made by Balan Teodor, BomberMan");
  delay(2000); 

  for (int i = 0; i < 33; ++i) {
    lcd.scrollDisplayLeft();
    delay(250);  
  }
  delay(450);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Introduction to Robotics on GitHub.");
  delay(2000);  

  for (int i = 0; i < 36; ++i) {
    lcd.scrollDisplayLeft();
    delay(250);  
  }

  delay(450);  // Delay after displaying the credits
}


// MATRIX GAME FUNCTIONS : 

void updateMatrix() {
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      lc.setLed(0, row, col, matrixMap[selectedMap][row][col]);
    }
  }
}

void playBuzzer() {
  digitalWrite(buzzerPin, HIGH);
  delayMicroseconds(1000); 
  digitalWrite(buzzerPin, LOW);
  delayMicroseconds(1000); 
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


bool areAllLedsOff(byte matrix[8][8], byte ignoreX, byte ignoreY) {
  for (int row = 0; row < 8; row++) {
    for (int col = 0; col < 8; col++) {
      if ((row != ignoreX || col != ignoreY) && matrix[row][col] != 0) {
        return false;
      }
    }
  }
  return true;
}






