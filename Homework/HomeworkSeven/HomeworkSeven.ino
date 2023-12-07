#include <LiquidCrystal.h>

const byte rsPin = 9;
const byte enPin = 8;
const byte d4Pin = 7;
const byte d5Pin = 6;
const byte d6Pin = 5;
const byte d7Pin = 4;

const int xPin = A0;
const int swPin = 2;
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

int currentMenu = menuStart;
int subMenu = 1;
bool exitMenu = false;

// Function prototypes
void updateMenu();
void executeAction();
void startGame();
void handleGameOptions();
void handleMatrixOptions();
void handleLcdOptions();
void showCredits();
void executeSubMenuAction();
void displayGameOptions();

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  pinMode(swPin, INPUT_PULLUP);
  pinMode(joyStickBtn, INPUT_PULLUP); 
  lcd.print("Boomber Man");
  lcd.setCursor(0, 1);
  lcd.print("By Teodor");
  delay(1000);
  lcd.clear();
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

  if (!digitalRead(swPin)) {
    executeAction();
    delay(debounceDelay);
    updateMenu();
    delay(debounceDelay);
    while (!digitalRead(swPin));
  }
}

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

void startGame() {
  lcd.clear();
  lcd.print("START:");
  delay(1000);
}

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

    if (!digitalRead(swPin)) {
      executeSubMenuAction();
      delay(debounceDelay);
      displayGameOptions();
      delay(debounceDelay);
      while (!digitalRead(swPin));
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

void executeSubMenuAction() {
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
  int selectedDifficulty = 1; 
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

    }
  


  delay(500); 
}



void handleMatrixOptions() {
  lcd.clear();
  lcd.print("Matrix:");
  delay(1000);
}

void handleLcdOptions() {
  lcd.clear();
  lcd.print("Lcd:");
  delay(1000);
}

void showCredits() {
  lcd.clear();
  lcd.print("Credits:");
  delay(1000);
}
