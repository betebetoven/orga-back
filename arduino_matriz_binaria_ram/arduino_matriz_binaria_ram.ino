#include <Arduino.h>
#include <Servo.h>
Servo myservo;  // crea el objeto servo
int pos = 0;    // posicion del servo

const int motorXPin1 = 2;    // 28BYJ48 In1
const int motorXPin2 = 3;    // 28BYJ48 In2
const int motorXPin3 = 4;    // 28BYJ48 In3
const int motorXPin4 = 5;    // 28BYJ48 In4

const int motorYPin1 = 6;    // 28BYJ48 In1
const int motorYPin2 = 7;    // 28BYJ48 In2
const int motorYPin3 = 8;    // 28BYJ48 In3
const int motorYPin4 = 9;    // 28BYJ48 In4


const int motorZPin1 = 31;    // 28BYJ48 In1
const int motorZPin2 = 32;    // 28BYJ48 In2
const int motorZPin3 = 33;    // 28BYJ48 In3
const int motorZPin4 = 34;    // 28BYJ48 In4

int motorSpeed = 1500;       // Velocidad del motor
int stepsPerRev = 4076;      // Pasos para una vuelta completa

const int numSteps = 8;
const int stepsLookup[8] = { B1000, B1100, B0100, B0110, B0010, B0011, B0001, B1001 };
String inputData; // This will hold the incoming data
bool processingSecondPart = false; // Flag to switch processing modes

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 bits per second.
  // Set pin modes for the data pins
  myservo.attach(10);

  pinMode(motorXPin1, OUTPUT);
  pinMode(motorXPin2, OUTPUT);
  pinMode(motorXPin3, OUTPUT);
  pinMode(motorXPin4, OUTPUT);

  pinMode(motorYPin1, OUTPUT);
  pinMode(motorYPin2, OUTPUT);
  pinMode(motorYPin3, OUTPUT);
  pinMode(motorYPin4, OUTPUT);

  pinMode(motorZPin1, OUTPUT);
  pinMode(motorZPin2, OUTPUT);
  pinMode(motorZPin3, OUTPUT);
  pinMode(motorZPin4, OUTPUT);
  for (int pin = 22; pin <= 30; pin++) {
    pinMode(pin, OUTPUT);
  }
  pinMode(50, INPUT); // Pin for black/white detector signal (modified)
  pinMode(51, INPUT); // Pin for black/white detector signal (modified)
  pinMode(52, OUTPUT); // Pin for read/write signal
  pinMode(53, OUTPUT); // Pin for reset signal

  digitalWrite(52, LOW); // Start with read mode (0)
  digitalWrite(53, HIGH); // Ensure reset signal is low
}

void loop() {
  //myservo.write(0);
  //delay(50000);
  // Check if there is no signal on pin 50 and 51
  if (digitalRead(50) == LOW && digitalRead(51) == LOW) {
    while (Serial.available() > 0) {
      char received = Serial.read();
      if (received == '\n') {
        if (inputData.length() == 9) { // Ensure the data is exactly 9 bits
          for (int i = 0; i < 9; i++) {
            int pinNumber = 22 + i;
            digitalWrite(pinNumber, inputData[i] == '1' ? HIGH : LOW);
          }
          // Processed all bits, now perform a write pulse
          digitalWrite(52, HIGH); // Set write signal
          delay(1000); // Short pulse
          digitalWrite(52, LOW); // Return to read mode
          Serial.println("Processed binary input: " + inputData);
          processingSecondPart = true; // Start processing the second part
        } else {
          Serial.println("Error: Input data must be exactly 9 bits.");
        }
        inputData = ""; // Clear inputData after processing
      } else if (received == '\t') {
        // End of the second part of the input
        if (processingSecondPart) {
          Serial.println("Processed second part: " + inputData);
          for (int i = 0; i < 9; i++) {
  if (inputData[i] != '0') {
    move_angulo(String(inputData[i]));
    switch (i) {
      case 0:
        moverA1(); // Position 1A
        break;
      case 1:
        moverB1(); // Position 1B
        break;
      case 2:
        moverC1(); // Position 1C
        break;
      case 3:
        moverA2(); // Position 2A
        break;
      case 4:
        moverB2(); // Position 2B
        break;
      case 5:
        moverC2(); // Position 2C
        break;
      case 6:
        moverA3(); // Position 3A
        break;
      case 7:
        moverB3(); // Position 3B
        break;
      case 8:
        Serial.println("mueve C3");
        moverC3(); // Position 3C
        break;
    }
  }
}

          // Add specific handling for the second part here
          // Example: Handle each character or perform specific operations
        }
        inputData = ""; // Clear inputData after processing
        processingSecondPart = false; // Reset flag
      } else if (received == 'q') {
        // Pulse the reset pin when 'q' is received
        Serial.println("reset");
        digitalWrite(53, LOW);
        delay(500); // Short pulse
        digitalWrite(53, HIGH);
      } else {
        inputData += received; // Collect each character until '\n', '\t' or 'q'
      }
    }
  } else {
    // If there is a signal on pin 50 or 51, do not process data
    Serial.println("Signal detected on pin 50 or 51. Processing halted.");
  }
}

void moveStepper(String motor, float revolutions, String direction) {
  int totalSteps = revolutions * stepsPerRev; // Calcula el total de pasos a dar
  bool clockwise = (direction == "CW");

  for (int i = 0; i < totalSteps; i++) {
    if (motor == "X") {
      updateStepper(motorXPin1, motorXPin2, motorXPin3, motorXPin4, clockwise);
    } else if (motor == "Y") {
      updateStepper(motorYPin1, motorYPin2, motorYPin3, motorYPin4, clockwise);
    } else if (motor == "Z") {
      updateStepper(motorZPin1, motorZPin2, motorZPin3, motorZPin4, clockwise);
    }
    delayMicroseconds(motorSpeed);
  }
}

void updateStepper(int pin1, int pin2, int pin3, int pin4, bool clockwise) {
  static int stepCounterX = 0;
  static int stepCounterY = 0;
  int* stepCounter = &stepCounterX; // Por defecto apunta al contador del EJE X

  if (pin1 == motorYPin1) { // Cambia a usar el contador del EJE Y si los pines coinciden
    stepCounter = &stepCounterY;
  }

  if (clockwise) {
    *stepCounter = (*stepCounter + 1) % numSteps;
  } else {
    *stepCounter = (*stepCounter - 1 + numSteps) % numSteps;
  }
  setOutput(pin1, pin2, pin3, pin4, *stepCounter);
}

void setOutput(int pin1, int pin2, int pin3, int pin4, int step) {
  digitalWrite(pin1, bitRead(stepsLookup[step], 0));
  digitalWrite(pin2, bitRead(stepsLookup[step], 1));
  digitalWrite(pin3, bitRead(stepsLookup[step], 2));
  digitalWrite(pin4, bitRead(stepsLookup[step], 3));
}

void moverA1(){
  moveStepper("Y",3, "CW");
  moverZ();
  moveStepper("Y",3, "CCW");
}

void moverA2(){
  moveStepper("Y", 1.5, "CW");
  moverZ();
  moveStepper("Y", 1.5, "CCW");
}

void moverA3(){
  moverZ();
}

void moverB1(){
  moveStepper("X", 1.5, "CCW");
  moveStepper("Y", 3, "CW");
  moverZ();
  moveStepper("X", 1.5, "CW");
  moveStepper("Y", 3, "CCW");
}

void moverB2(){
  moveStepper("X", 1.5, "CCW");
  moveStepper("Y", 1.5, "CW");
  moverZ();
  moveStepper("X", 1.5, "CW");
  moveStepper("Y", 1.5, "CCW");
}

void moverB3(){
  moveStepper("X", 1.5, "CCW");
  moverZ();
  moveStepper("X", 1.5, "CW");
}

void moverC1(){
  moveStepper("X", 3, "CCW");
  moveStepper("Y", 3, "CW");
  moverZ();
  moveStepper("X", 3, "CW");
  moveStepper("Y", 3, "CCW");
}

void moverC2(){
  moveStepper("X", 3, "CCW");
  moveStepper("Y", 1.5, "CW");
  moverZ();
  moveStepper("X", 3, "CW");
  moveStepper("Y", 1.5, "CCW");
}

void moverC3(){
  moveStepper("X", 3, "CCW");
  moverZ();
  moveStepper("X", 3, "CW");
}

void moverZ(){
  moveStepper("Z", 1, "CW");
  
  
}

void move_angulo(String number) {
  int angle;
  
  // Convert String to integer
  int num = number.toInt();

  // Determine the angle based on the input number
  switch (num) {
    case 1:
      angle = 55; // Move to 55 degrees
      break;
    case 2:
      angle = 140; // Move to 140 degrees
      break;
    case 3:
      angle = 175; // Move to 175 degrees
      break;
    case 4:
      angle = 0; // Move to 0 degrees
      break;
    default:
      angle = 0; // Default position, can change if needed
      Serial.println("Invalid input, servo set to 0 degrees.");
      break;
  }

  // Command the servo to move to the specified angle
  myservo.write(angle);
  Serial.print("Servo moved to ");
  Serial.print(angle);
  Serial.println(" degrees.");
}

