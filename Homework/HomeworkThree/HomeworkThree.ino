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
int stateSegments[segSize] = { 0, 0, 0, 0, 0, 0, 0, 0 };


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
      handleJoystickBot(startValue, displayOn, stateSegments);
      joyMoved = true;
      joyReturned = false;
    }

    if (xValue > maxThreshold && joyMoved == false && joyReturned) {
      handleJoystickTop(startValue, displayOn, stateSegments);
      joyMoved = true;
      joyReturned = false;
    }

    if (yValue < minThreshold && joyMoved == false && joyReturned) {
      handleJoystickLeft(startValue, displayOn, stateSegments);
      joyMoved = true;
      joyReturned = false;
    }
    if (yValue > maxThreshold && joyMoved == false && joyReturned) {
      handleJoystickYRight(startValue, displayOn, stateSegments);
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
void handleJoystickTop(int &startValue, bool &displayOn, int stateSegments[]) {
  if (xValue > maxThreshold && joyMoved == false) {
    switch (startValue) {
      case pinC:
        if (displayOn == HIGH) {
          stateSegments[2]++;
        }
        if (stateSegments[2] % 2 != 0) {
          digitalWrite(startValue, HIGH);
        } else if (stateSegments[2] % 2 == 0) {
          digitalWrite(startValue, LOW);
        }
        startValue = pinG;
        displayOn = LOW;
        break;
      case pinE:

        if (displayOn == HIGH) {
          stateSegments[4]++;
        }
        if (stateSegments[4] % 2 != 0) {
          digitalWrite(startValue, HIGH);
        } else if (stateSegments[4] % 2 == 0) {
          digitalWrite(startValue, LOW);
        }
        startValue = pinG;
        displayOn = LOW;

        break;
      case pinD:
        if (displayOn == HIGH) {
          stateSegments[3]++;
        }
        if (stateSegments[3] % 2 != 0) {
          digitalWrite(startValue, HIGH);
        } else if (stateSegments[3] % 2 == 0) {
          digitalWrite(startValue, LOW);
        }
        startValue = pinG;
        displayOn = LOW;
        break;
      case pinG:
        if (displayOn == HIGH) {
          stateSegments[6]++;
        }
        if (stateSegments[6] % 2 != 0) {
          digitalWrite(startValue, HIGH);
        } else if (stateSegments[6] % 2 == 0) {
          digitalWrite(startValue, LOW);
        }
        startValue = pinA;
        break;
      case pinF:
        if (displayOn == HIGH) {
          stateSegments[5]++;
        }
        if (stateSegments[5] % 2 != 0) {
          digitalWrite(startValue, HIGH);
        } else if (stateSegments[5] % 2 == 0) {
          digitalWrite(startValue, LOW);
        }
        startValue = pinA;
        break;
      case pinB:
        if (displayOn == HIGH) {
          stateSegments[1]++;
        }
        if (stateSegments[1] % 2 != 0) {
          digitalWrite(startValue, HIGH);
        } else if (stateSegments[1] % 2 == 0) {
          digitalWrite(startValue, LOW);
        }
        startValue = pinA;
        break;
    }
  }
}

void handleJoystickBot(int &startValue, bool &displayOn, int stateSegments[]) {
  if (xValue < minThreshold && joyMoved == false) {
    switch (startValue) {
      case pinG:
        if (displayOn == HIGH) {
          stateSegments[6]++;
        }
        if (stateSegments[6] % 2 != 0) {
          digitalWrite(startValue, HIGH);
        } else if (stateSegments[6] % 2 == 0) {
          digitalWrite(startValue, LOW);
        }
        startValue = pinD;
        displayOn = LOW;

        break;
      case pinC:
        if (displayOn == HIGH) {
          stateSegments[2]++;
        }
        if (stateSegments[2] % 2 != 0) {
          digitalWrite(startValue, HIGH);
        } else if (stateSegments[2] % 2 == 0) {
          digitalWrite(startValue, LOW);
        }
        startValue = pinD;
        displayOn = LOW;
        break;
      case pinE:
        if (displayOn == HIGH) {
          stateSegments[4]++;
        }
        if (stateSegments[4] % 2 != 0) {
          digitalWrite(startValue, HIGH);
        } else if (stateSegments[4] % 2 == 0) {
          digitalWrite(startValue, LOW);
        }
        startValue = pinD;
        displayOn = LOW;
        break;
      case pinB:
        if (displayOn == HIGH) {
          stateSegments[1]++;
        }
        if (stateSegments[1] % 2 != 0) {
          digitalWrite(startValue, HIGH);
        } else if (stateSegments[1] % 2 == 0) {
          digitalWrite(startValue, LOW);
        }
        startValue = pinG;
        displayOn = LOW;
        break;
      case pinF:
        if (displayOn == HIGH) {
          stateSegments[5]++;
        }
        if (stateSegments[5] % 2 != 0) {
          digitalWrite(startValue, HIGH);
        } else if (stateSegments[5] % 2 == 0) {
          digitalWrite(startValue, LOW);
        }
        startValue = pinG;
        displayOn = LOW;
      case pinA:
        if (displayOn == HIGH) {
          stateSegments[0]++;
        }
        if (stateSegments[0] % 2 != 0) {
          digitalWrite(startValue, HIGH);
        } else if (stateSegments[0] % 2 == 0) {
          digitalWrite(startValue, LOW);
        }
        startValue = pinG;
        displayOn = LOW;

        break;
    }
  }
}
//Stanga
void handleJoystickLeft(int &startValue, bool &displayOn, int stateSegments[]) {
  switch (startValue) {
    case pinDP:
      if (displayOn == HIGH) {
        stateSegments[7]++;
      }
      if (stateSegments[7] % 2 != 0) {
        digitalWrite(startValue, HIGH);
      } else if (stateSegments[7] % 2 == 0) {
        digitalWrite(startValue, LOW);
      }
      startValue = pinC;
      displayOn = LOW;
      break;
      // Add other cases for additional segments here as needed

    case pinC:
      if (displayOn == HIGH) {
        stateSegments[2]++;
      }
      if (stateSegments[2] % 2 != 0) {
        digitalWrite(startValue, HIGH);
      } else if (stateSegments[2] % 2 == 0) {
        digitalWrite(startValue, LOW);
      }
      startValue = pinE;
      displayOn = LOW;
      break;
    case pinD:
      if (displayOn == HIGH) {
        stateSegments[3]++;
      }
      if (stateSegments[3] % 2 != 0) {
        digitalWrite(startValue, HIGH);
      } else if (stateSegments[3] % 2 == 0) {
        digitalWrite(startValue, LOW);
      }
      startValue = pinE;
      displayOn = LOW;
      break;
    case pinA:
      if (displayOn == HIGH) {
        stateSegments[0]++;
      }
      if (stateSegments[0] % 2 != 0) {
        digitalWrite(startValue, HIGH);
      } else if (stateSegments[0] % 2 == 0) {
        digitalWrite(startValue, LOW);
      }
      startValue = pinF;
      displayOn = LOW;
      break;
    case pinB:
      if (displayOn == HIGH) {
        stateSegments[1]++;
      }
      if (stateSegments[1] % 2 != 0) {
        digitalWrite(startValue, HIGH);
      } else if (stateSegments[1] % 2 == 0) {
        digitalWrite(startValue, LOW);
      }
      startValue = pinF;
      displayOn = LOW;

      break;
  }
}


void handleJoystickYRight(int &startValue, bool &displayOn, int stateSegments[]) {
  switch (startValue) {
    case pinC:
      if (displayOn == HIGH) {
        stateSegments[2]++;
      }
      if (stateSegments[2] % 2 != 0) {
        digitalWrite(startValue, HIGH);
      } else if (stateSegments[2] % 2 == 0) {
        digitalWrite(startValue, LOW);
      }
      startValue = pinDP;
      displayOn = LOW;

      break;
    case pinE:
      if (displayOn == HIGH) {
        stateSegments[4]++;
      }
      if (stateSegments[4] % 2 != 0) {
        digitalWrite(startValue, HIGH);
      } else if (stateSegments[4] % 2 == 0) {
        digitalWrite(startValue, LOW);
      }
      startValue = pinC;
      displayOn = LOW;
      break;
    case pinD:
      if (displayOn == HIGH) {
        stateSegments[3]++;
      }
      if (stateSegments[3] % 2 != 0) {
        digitalWrite(startValue, HIGH);
      } else if (stateSegments[3] % 2 == 0) {
        digitalWrite(startValue, LOW);
      }
      startValue = pinC;
      displayOn = LOW;
      break;
    case pinA:
      if (displayOn == HIGH) {
        stateSegments[0]++;
      }
      if (stateSegments[0] % 2 != 0) {
        digitalWrite(startValue, HIGH);
      } else if (stateSegments[0] % 2 == 0) {
        digitalWrite(startValue, LOW);
      }
      startValue = pinB;
      displayOn = LOW;
      // startValue = pinB;
      break;
    case pinF:
      if (displayOn == HIGH) {
        stateSegments[5]++;
      }
      if (stateSegments[5] % 2 != 0) {
        digitalWrite(startValue, HIGH);
      } else if (stateSegments[5] % 2 == 0) {
        digitalWrite(startValue, LOW);
      }
      startValue = pinB;
      displayOn = LOW;
      // startValue = pinB;
      break;
  }
}