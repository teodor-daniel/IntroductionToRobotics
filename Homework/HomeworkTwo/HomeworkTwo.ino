// Constants for pin numbers and floor count
const int ledPins[] = {9, 10, 11};
const int buttonPins[] = {2, 4, 7};
const int buzzerPin = 6;
const int operationalLEDPin = 13;
const int floorCount = 3;

// Arrays to store LED and button states
int ledState[] = {LOW, LOW, LOW};
int buttonState[] = {LOW, LOW, LOW};
int lastButtonState[] = {LOW, LOW, LOW};

// Variables for debouncing button presses
unsigned long lastDebounceTime[] = {0, 0, 0};
unsigned long debounceDelay = 50;

// Variables to track elevator state
int currentFloor = 0;
bool isMoving = false;

// Variables to control the buzzer
unsigned long buzzerStartTime = 0;
unsigned long buzzerDuration = 0;
bool buzzerActive = false;

// Setup function to initialize pin modes
void setup() {
  pinMode(operationalLEDPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  for (int i = 0; i < floorCount; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
  updateLEDs();
}

// Main loop function
void loop() {
  checkButtons();
}

// Function to check button presses and debounce
void checkButtons() {
  for (int i = 0; i < floorCount; i++) {
    int reading = digitalRead(buttonPins[i]);

    if (reading != lastButtonState[i]) {
      lastDebounceTime[i] = millis();
    }

    if ((millis() - lastDebounceTime[i]) > debounceDelay) {
      if (reading != buttonState[i]) {
        buttonState[i] = reading;

        if (buttonState[i] == HIGH) {
          if (!isMoving) {
            moveElevator(i);
          }
        }
      }
    }

    lastButtonState[i] = reading;
  }
}

// Function to move the elevator to the target floor
void moveElevator(int targetFloor) {
  if (!isMoving && currentFloor != targetFloor) {
    isMoving = true;
    unsigned long previousMillis = millis();
    const unsigned long floorDelay = 2500;
    bool destinationReached = false;
    bool addedDelay = false;  // Flag to track if the delay has been added

    while (!destinationReached) {
      pulseOperationalLED(); 

      unsigned long currentMillis = millis();

      if (currentMillis - previousMillis >= floorDelay) {
        previousMillis = currentMillis;

        if (currentFloor < targetFloor || currentFloor > targetFloor) {
          currentFloor += (targetFloor > currentFloor) ? 1 : -1; //if the floor is higher go up else go down
          activateBuzzer();//play the going up sound
        } else {
          noTone(buzzerPin);
          destinationReached = true;
          digitalWrite(operationalLEDPin, HIGH);//the led must remain high after it has reached the floor.
          activateBuzzerFinale();//play open door sound
        }
        updateLEDs();
      }

      if (!addedDelay && (currentMillis - previousMillis) >= 500) {
        addedDelay = true;
      }
    }
    if (addedDelay) {
      isMoving = false;
      noTone(buzzerPin);//stop the open door sound;
    }
  }
}

// Function to activate the buzzer with a tone while the elevetor is not on the current floor
void activateBuzzer() {
  tone(buzzerPin, 200);
}

// Function to activate the buzzer for a finale sound of the doors opening
void activateBuzzerFinale() {
  if (!buzzerActive) {
    buzzerStartTime = millis();
    buzzerDuration = 300;
    buzzerActive = true;
    tone(buzzerPin, 400);
  }

  if (millis() - buzzerStartTime >= buzzerDuration) {
    buzzerActive = false;
  }
}

// Function to pulse the operational LED
void pulseOperationalLED() {
  unsigned long startTime = millis();
  int pulseDuration = 600;
  int pulseInterval = 300;
  //duration of the operationalLEDPin pulsing
  while (millis() - startTime < pulseDuration) {
    if (millis() - startTime < pulseInterval) {
      digitalWrite(operationalLEDPin, HIGH);
    } else {
      digitalWrite(operationalLEDPin, LOW);
    }
  }

  digitalWrite(operationalLEDPin, LOW);
}

// Function to update the LED display based on the current floor
void updateLEDs() {
  for (int i = 0; i < floorCount; i++) {
    digitalWrite(ledPins[i], (i == currentFloor) ? HIGH : LOW);
  }
}
