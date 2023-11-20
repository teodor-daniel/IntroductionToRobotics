int value = 0;
int state = 0;
int samplingInterval = 1000;
const int redPin = 5;
const int greenPin = 6;
const int bluePin = 7;
int stopReading = 0;
const int trigPin = 9;
const int echoPin = 10;
long duration = 0;
int distanceArray[10];
int arrayIndex = 0;
unsigned long previousMillis = 0;
int lastDistance;
int minDistance = 10;  //testvalue
int maxDistance = 40;
int reset;
boolean automatic = true;
int red, green, blue;
int lastLight;
const int photocellPin = A0;
int photocellValue = 0;
int ledValue = 0;

int photocellArray[10];
int currentArrayLightPosition = 0;
int minLight = 50;
int maxLight = 300;
void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  mainMenu();
}


void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= samplingInterval) {
    previousMillis = currentMillis;
    //Sonic
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(100);
    digitalWrite(trigPin, LOW);
    duration = pulseIn(echoPin, HIGH);
    distanceArray[arrayIndex] = duration * 0.034 / 2;
    arrayIndex++;
    if (arrayIndex >= 10) {
      arrayIndex = 0;
    }
    lastDistance = distanceArray[(arrayIndex == 0) ? 9 : arrayIndex - 1];
    //Light
    photocellValue = analogRead(photocellPin);


    photocellArray[currentArrayLightPosition] = photocellValue;
    currentArrayLightPosition++;
    if (currentArrayLightPosition >= 10) {
      currentArrayLightPosition = 0;
    }
    lastLight = photocellArray[(currentArrayLightPosition == 0) ? 9 : currentArrayLightPosition - 1];
    ledValue = map(photocellValue, 0, 1023, 0, 255);
    if (ledValue < 100) {
      ledValue = 150;
    }
    if ((automatic == false) && (lastDistance < minDistance || lastDistance > maxDistance)) {
      Serial.println("Warning distance below the expected levels");
    }
    if ((automatic == false) && (lastLight < minLight || lastLight > maxLight)) {
      Serial.println("Warning light below the expected levels");
    }
    if (automatic == true) {
      if ((lastLight < minLight || lastLight > maxLight || lastDistance < minDistance || lastDistance > maxDistance)) {
        analogWrite(redPin, ledValue);
        analogWrite(greenPin, 0);
        analogWrite(bluePin, 0);
      } else {
        analogWrite(redPin, 0);
        analogWrite(greenPin, ledValue);
        analogWrite(bluePin, 0);
      }
    }
  }
  if (Serial.available() > 0) {
    value = Serial.read() - 48;
    switch (state) {
      case 0:
        {
          switch (value) {
            case 1:
              {
                printFirst();
                state = 1;
                break;
              }
            case 2:
              {
                printSecond();
                state = 2;
                break;
              }
            case 3:
              {
                printThird();
                state = 3;
                break;
              }
            case 4:
              {
                printFourth();
                state = 4;
                break;
              }
          }
          break;
        }

      case 1:
        {

          switch (value) {

            case 1:
              {
                Serial.println("What should be the sampling interval?(between 1 and 10 seconds)");
                clearSerial();
                while (!Serial.available()) {}
                samplingInterval = Serial.read() - 48;
                Serial.print("Sampling interval is: ");
                Serial.println(samplingInterval);
                if (samplingInterval < 1 || samplingInterval > 10) {
                  Serial.println("Value set to default(1) as it must be between 1 and 10");
                  samplingInterval = 1;
                }
                samplingInterval *= 1000;
                Serial.println();
                printFirst();
                break;
              }
            case 2:
              {
                Serial.println("What should be the closest distance until warning?");
                clearSerial();
                while (!Serial.available()) {}
                minDistance = Serial.parseInt();
                Serial.println("Minimum distance must be >: ");
                Serial.println(minDistance);
                Serial.println("What should be the furthest distance until warning?");
                clearSerial();
                while (!Serial.available()) {}
                maxDistance = Serial.parseInt();
                Serial.println("Max distance must be <: ");
                Serial.println(maxDistance);
                if (maxDistance <= minDistance) {
                  Serial.println("MAX DISTANCE MUST BE > MIN DISTANCE REVERTING BACK TO DEFAULT VALUES");
                  minDistance = 10;
                  maxDistance = 40;
                }
                printFirst();
                break;
              }
            case 3:
              {
                Serial.println("What should be the lowest light level until warning?");
                clearSerial();
                while (!Serial.available()) {}
                minLight = Serial.parseInt();
                Serial.println("Light distance must be >: ");
                Serial.println(minLight);
                Serial.println("What should be the highest light level distance until warning?");
                clearSerial();
                while (!Serial.available()) {}
                maxLight = Serial.parseInt();
                Serial.println("Max light must be <: ");
                Serial.println(maxLight);
                if (maxLight <= minLight) {
                  Serial.println("MAX LIGH  MUST BE > MIN LIGHT REVERTING BACK TO DEFAULT VALUES");
                  minLight = 100;
                  maxLight = 500;
                }
                printFirst();
                break;
              }
            case 4:
              {
                state = 0;
                mainMenu();
                break;
              }
          }
          break;
        }

      case 2:
        {
          switch (value) {
            case 1:
              {
                Serial.println("Are you sure you want to reset all the values? if so type in 5");
                clearSerial();
                while (!Serial.available()) {}
                reset = Serial.parseInt();
                if (reset == 5) {
                  //reset all the values to the standard;
                  minDistance = 10;
                  maxDistance = 40;
                  minLight = 100;
                  maxLight = 500;
                  for (int i = 0; i < 10; i++) {
                    distanceArray[i] = 0;
                    photocellArray[i] = 0;
                    Serial.print(" ");
                  }
                  lastDistance = 11;
                  lastLight = 111;
                  samplingInterval = 1000;
                  analogWrite(redPin, 0);
                  analogWrite(greenPin, 0);
                  analogWrite(bluePin, 0);
                }

                state = 0;
                mainMenu();
                break;
              }
            case 2:
              {
                state = 0;
                mainMenu();
                break;
              }
          }
          break;
        }

      case 3:
        {
          switch (value) {
            case 1:
              {
                Serial.println(distanceArray[arrayIndex]);
                Serial.println(photocellArray[currentArrayLightPosition]);
                printThird();
                break;
              }
            case 2:
              {
                Serial.println("The sampling rate is: ");
                Serial.print(samplingInterval / 1000);
                Serial.println("Threshold values for ultrasonic are: ");
                Serial.print("Min Distance: ");
                Serial.print(minDistance);
                Serial.print(", Max Distance: ");
                Serial.println(maxDistance);
                Serial.println("Threshold values for light sensor are: ");
                Serial.print("Min light: ");
                Serial.print(minLight);
                Serial.print(", Max Light: ");
                Serial.println(maxLight);
                Serial.println();

                printThird();
                break;
              }
            case 3:
              {
                Serial.println("Sunt in 3.3");
                Serial.println("Distance Array:");
                for (int i = 0; i < 10; i++) {
                  Serial.print(distanceArray[i]);
                  Serial.print(" ");
                }
                Serial.println(" ");
                Serial.println("Photocell Array:");
                for (int i = 0; i < 10; i++) {
                  Serial.print(photocellArray[i]);
                  Serial.print(" ");
                }
                Serial.println();
                printThird();
                break;
              }
            case 4:
              {
                state = 0;
                mainMenu();
                break;
              }
          }
          break;
        }

      case 4:
        {
          switch (value) {
            case 1:
              {
                if (automatic != true) {
                  clearSerial();
                  Serial.print("Enter the Red value (0-255): ");
                  while (!Serial.available()) {}
                  red = Serial.parseInt();
                  clearSerial();

                  Serial.print("Enter the Green value (0-255): ");
                  while (!Serial.available()) {}
                  green = Serial.parseInt();
                  clearSerial();

                  Serial.print("Enter the Blue value (0-255): ");
                  while (!Serial.available()) {}
                  blue = Serial.parseInt();

                  red = constrain(red, 0, 255);
                  green = constrain(green, 0, 255);
                  blue = constrain(blue, 0, 255);
                  Serial.print("Selected RGB values: ");
                  Serial.print("Red: ");
                  Serial.print(red);
                  Serial.print(", Green: ");
                  Serial.print(green);
                  Serial.print(", Blue: ");
                  Serial.println(blue);

                  analogWrite(redPin, red);
                  analogWrite(greenPin, green);
                  analogWrite(bluePin, blue);
                } else {
                  Serial.println("To set the light you must deactivate the auto system");
                }
                printFourth();

                break;
              }
            case 2:
              {
                Serial.println("State has changed");
                automatic = !automatic;
                Serial.println("Currently the statie is: ");
                Serial.println(automatic);
                printFourth();
                break;
              }
            case 3:
              {
                state = 0;
                mainMenu();
                break;
              }
          }

          break;
        }
      case 5:
        break;
    }
  }
}


void mainMenu() {
  Serial.println("Menu:");
  Serial.println("1. Sensor Settings");
  Serial.println("2. Reset Logger Data");
  Serial.println("3. System Status");
  Serial.println("4. RGB LED Control");
  Serial.println();
}

void clearSerial() {
  char wrk[1];
  delay(100);
  while (Serial.available() != 0) {
    Serial.readBytes(wrk, 1);
  }
  delay(1000);
}


void printFirst() {
  Serial.println("1.1 Sensors Sampling Interval");
  Serial.println("1.2 Ultrasonic Alert Threshold");
  Serial.println("1.3 LDR Alert Threshold");
  Serial.println("1.4 Back");
  Serial.println();
}

void printSecond() {
  Serial.println("2.1 Yes");
  Serial.println("2.2 No");
  Serial.println();
}

void printThird() {
  Serial.println("3.1 Current Sensor Readings");
  Serial.println("3.2 Current Sensor Settings");
  Serial.println("3.3 Display Logged Data");
  Serial.println("3.4 Back");
  Serial.println();
}

void printFourth() {
  Serial.println("4.1 Manual Color Control");
  Serial.println("4.2 LED: Toggle Automatic ON/OFF");
  Serial.println("4.3 Back");
  Serial.println();
}