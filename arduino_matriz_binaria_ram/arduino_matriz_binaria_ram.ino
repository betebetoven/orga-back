#include <Arduino.h>

String inputData; // This will hold the incoming data
bool processingSecondPart = false; // Flag to switch processing modes

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 bits per second.
  // Set pin modes for the data pins
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
