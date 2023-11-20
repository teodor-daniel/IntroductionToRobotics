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
    if (lastDistance < minDistance || lastDistance > maxDistance) {
      analogWrite(redPin, 255);
      analogWrite(greenPin, 0);
      analogWrite(bluePin, 0);
    } else {
      analogWrite(redPin, 0);
      analogWrite(greenPin, 255);
      analogWrite(bluePin, 0);
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
                Serial.println("What should be the cloest distance until warning?");
                clearSerial();
                while (!Serial.available()) {}
                minDistance = Serial.parseInt();
                Serial.println("Minimum distance must be >: ");
                Serial.println(minDistance);
                Serial.println("What should be the cloest distance until warning?");
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
                Serial.println("Sunt in 1.3");
                Serial.println();
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
                  for (int i = 0; i < 10; i++) {
                    distanceArray[i] = 0;
                    Serial.print(" ");
                  }
                  lastDistance = 11;
                  samplingInterval = 1000;
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
                Serial.println("Sunt in 4.1");
                Serial.println();
                printFourth();
                break;
              }
            case 2:
              {
                Serial.println("Sunt in 4.2");
                Serial.println();
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