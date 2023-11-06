//7Segment
const int pinA = 12;
const int pinB = 10;
const int pinC = 9;
const int pinD = 8;
const int pinE = 7;
const int pinF = 6;
const int pinG = 5;
const int pinDP = 4;
// Controller
const int pinSW = A2;
const int pinX = A0;
const int pinY = A1;

const int segSize = 8;
bool commonAnode = false;
byte state = HIGH;
byte dpState = LOW;
byte swState = LOW;
byte lastSwState = LOW;
bool displayOn = LOW;

bool joyMoved = false;
bool joyReturned = true;



int segments[segSize] = {
  // 0      1     2     3     4     5     6   7
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
int stateSegments[segSize] = { 1, 1, 1, 1, 1, 1, 1, 1 };


int xValue;
int yValue;

int minThreshold = 200;
int maxThreshold = 700;
int startValue = pinDP;
int wasPressed = 0;

unsigned long previousMillis = 0;
const unsigned long interval = 100;
unsigned long previousMillis2 = 0;
const unsigned long interval2 = 400;

void handleButtonInterrupt() {

  swState = digitalRead(pinSW);
  if (swState == LOW && lastSwState == HIGH) {
    displayOn = !displayOn;
  }
  lastSwState = swState;
}

void handleXJoystickInterrupt() {
  xValue = analogRead(pinX);
}

void handleYJoystickInterrupt() {
  yValue = analogRead(pinY);
}
void setup() {
  // Initialize all the pins
  for (int i = 0; i < segSize; i++) {
    pinMode(segments[i], OUTPUT);
  }

  pinMode(pinSW, INPUT_PULLUP);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);

  if (commonAnode == true) {
    state = !state;
  }
  attachInterrupt(digitalPinToInterrupt(pinSW), handleButtonInterrupt, FALLING);

  attachInterrupt(digitalPinToInterrupt(pinX), handleXJoystickInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(pinY), handleYJoystickInterrupt, CHANGE);
}
boolean segmentState = false;  // Initialize segment state as OFF

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis;

    swState = digitalRead(pinSW);
    if (swState == LOW && lastSwState == HIGH) {
      displayOn = !displayOn;
      wasPressed++;
    }
    lastSwState = swState;
    segmentState = !segmentState;

    // Update the segment based on the segmentState
    digitalWrite(startValue, segmentState ? HIGH : LOW);


    xValue = analogRead(pinX);
    yValue = analogRead(pinY);

    if (xValue < minThreshold && joyMoved == false && joyReturned) {
      handleJoystickBot(startValue, displayOn);
      joyMoved = true;
      joyReturned = false;
    }

    if (xValue > maxThreshold && joyMoved == false && joyReturned) {
      handleJoystickTop(startValue, displayOn);
      joyMoved = true;
      joyReturned = false;
    }

    if (yValue < minThreshold && joyMoved == false && joyReturned) {
      handleJoystickLeft(startValue, displayOn, stateSegments, wasPressed);
      joyMoved = true;
      joyReturned = false;
    }
    if (yValue > maxThreshold && joyMoved == false && joyReturned) {
      handleJoystickYRight(startValue, displayOn, wasPressed);
      joyMoved = true;
      joyReturned = false;
    }

    if (xValue >= minThreshold && xValue <= maxThreshold && yValue >= minThreshold && yValue <= maxThreshold) {
      joyMoved = false;
      joyReturned = true;
      pause();
    }
    wasPressed = 0;
  }
}
void pause() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
  }
}
void pause300() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis2 >= interval2) {
    previousMillis2 = currentMillis;
  }
}
void handleJoystickTop(int &startValue, bool &displayOn) {
  if (xValue > maxThreshold && joyMoved == false) {
    switch (startValue) {
      case pinC:
      case pinE:
        digitalWrite(startValue, displayOn);
        displayOn = LOW;
        startValue = pinG;
        break;
      case pinD:
        digitalWrite(startValue, displayOn);
        displayOn = LOW;
        startValue = pinG;
        break;
      case pinG:
        digitalWrite(startValue, displayOn);
        displayOn = LOW;
        startValue = pinA;
        break;
      case pinF:
      case pinB:
        digitalWrite(startValue, displayOn);
        displayOn = LOW;
        startValue = pinA;
        break;
    }
  }
}

void handleJoystickBot(int &startValue, bool &displayOn) {
  if (xValue < minThreshold && joyMoved == false) {
    switch (startValue) {
      case pinG:
        digitalWrite(startValue, displayOn);
        displayOn = LOW;
        startValue = pinD;
        break;
      case pinC:
      case pinE:
        digitalWrite(startValue, displayOn);
        displayOn = LOW;
        startValue = pinD;
        break;
      case pinB:
      case pinF:
        digitalWrite(startValue, displayOn);
        displayOn = LOW;
        startValue = pinG;
        break;
      case pinA:
        digitalWrite(startValue, displayOn);
        displayOn = LOW;
        startValue = pinG;
        break;
    }
  }
}
//Stanga
void handleJoystickLeft(int &startValue, bool &displayOn, int stateSegments[], int wasPressed) {
  if (yValue < minThreshold && joyMoved == false) {
    switch (startValue) {
      case pinDP:
        digitalWrite(startValue, displayOn);
        displayOn = LOW;
        startValue = pinC;
        break;
        // Add other cases for additional segments here as needed

      case pinC:
        digitalWrite(startValue, displayOn);
        displayOn = LOW;
        startValue = pinE;
        break;
      case pinD:
        digitalWrite(startValue, displayOn);
        displayOn = LOW;
        startValue = pinE;
        break;
      case pinA:
        digitalWrite(startValue, displayOn);
        displayOn = LOW;
        startValue = pinF;
        break;
      case pinB:
        digitalWrite(startValue, displayOn);
        displayOn = LOW;
        startValue = pinF;
        break;
    }
  }
}

void handleJoystickYRight(int &startValue, bool &displayOn, int wasPressed) {
  if (yValue > maxThreshold && joyMoved == false) {
    switch (startValue) {
      case pinE:
        digitalWrite(startValue, LOW);
        displayOn = LOW;
        startValue = pinC;
        break;
      case pinC:
        digitalWrite(startValue, LOW);
        displayOn = LOW;
        startValue = pinDP;
        break;
      case pinD:
        digitalWrite(startValue, LOW);
        displayOn = LOW;

        startValue = pinC;
        break;
      case pinA:
        digitalWrite(startValue, LOW);
        displayOn = LOW;

        startValue = pinB;
        break;
      case pinF:
        digitalWrite(startValue, LOW);
        displayOn = LOW;

        startValue = pinB;
        break;
    }
  }
}
