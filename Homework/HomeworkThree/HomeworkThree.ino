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
bool displayOn = LOW;  // Initially, the display is turned on

bool joyMoved = false;  // This variable is used
// Initialize the segments
int segments[segSize] = {
  pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP
};
int xValue;
int yValue;

int minThreshold = 300;
int maxThreshold = 700;
int startValue = pinDP;
int nextValue = 0;

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
  attachInterrupt(digitalPinToInterrupt(pinSW), switchInterrupt, FALLING);
}

void loop() {
  //De refacut

  swState = digitalRead(pinSW);
  if (swState == LOW && lastSwState == HIGH) {
    // Button was pressed, toggle the display state
    displayOn = !displayOn;
  }

  lastSwState = swState;

  digitalWrite(startValue, HIGH);

  xValue = analogRead(pinX);
  yValue = analogRead(pinY);

  // Turn on the 'startValue' segment for Y direction

  // SUS
  handleJoystickTop(startValue, displayOn, joyMoved);

  //JOS
  handleJoystickBot(startValue, displayOn, joyMoved);

  // Check if the joystick is back to the neutral position in X direction
  if (xValue >= minThreshold && xValue <= maxThreshold) {
    joyMoved = false;
  }

  //IN STANGA
  handleJoystickLeft(startValue, displayOn, joyMoved);

  // IN DREAPTA
  handleJoystickYRight(startValue, displayOn, joyMoved);

  // Check if the joystick is back to the neutral position in Y direction
  if (yValue >= minThreshold && yValue <= maxThreshold) {
    joyMoved = false;
  }
}
void switchInterrupt() {
  // This function is called when the switch is pressed
  swState = LOW;
  // Button was pressed, toggle the display state
  displayOn = !displayOn;
}
void handleJoystickTop(int &startValue, bool &displayOn, bool &joyMoved) {
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
    joyMoved = true;
  }
}

void handleJoystickBot(int &startValue, bool &displayOn, bool &joyMoved) {
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
    joyMoved = true;
  }
}

void handleJoystickLeft(int &startValue, bool &displayOn, bool &joyMoved) {
  if (yValue < minThreshold && joyMoved == false) {
    switch (startValue) {
      case pinDP:
        digitalWrite(startValue, displayOn);
        displayOn = LOW;
        startValue = pinC;
        break;
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
    joyMoved = true;
  }
}

void handleJoystickYRight(int &startValue, bool &displayOn, bool &joyMoved) {
  if (yValue > maxThreshold && joyMoved == false) {
    switch (startValue) {
      case pinE:
        digitalWrite(startValue, LOW);
        startValue = pinC;
        break;
      case pinC:
        digitalWrite(startValue, LOW);
        startValue = pinDP;
        break;
      case pinD:
        digitalWrite(startValue, LOW);
        startValue = pinC;
        break;
      case pinA:
        digitalWrite(startValue, LOW);
        startValue = pinB;
        break;
      case pinF:
        digitalWrite(startValue, LOW);
        startValue = pinB;
        break;
    }
    joyMoved = true;
  }
}
