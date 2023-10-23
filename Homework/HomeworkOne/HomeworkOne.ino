// Defined the pins for controlling the led (RGB)
const int redPin = 9;    
const int greenPin = 10;  
const int bluePin = 11;   

// Defined the analog input pins for the potentiometers
const int redPotPin = A0;     
const int greenPotPin = A1;  
const int bluePotPin = A2;    

// Variables to store the values for the led colors
int redValue = 0;           
int greenValue = 0;         
int blueValue = 0;          
// Constants for mapping values to avoid magic numbers
const int analogMin = 0;
const int analogMax = 1023;
const int pwmMin = 0;
const int pwmMax = 255;

void setup() {
  // Led control pins as output
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
}

void loop() {
// read the value from the potentiometer
  int redPotValue = analogRead(redPotPin);
  int greenPotValue = analogRead(greenPotPin);
  int bluePotValue = analogRead(bluePotPin);

// map the values,
  redValue = map(redPotValue, analogMin, analogMax, pwmMin, pwmMax);
  greenValue = map(greenPotValue, analogMin, analogMax, pwmMin, pwmMax);
  blueValue = map(bluePotValue, analogMin, analogMax, pwmMin, pwmMax);

// display the values to the led
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}
