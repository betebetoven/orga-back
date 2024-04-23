#include <Arduino.h>

String inputData; // This will hold the incoming partial data
String items[9]; // Array to store separated items, adjust size as necessary
int itemCount = 0; // Counter for the number of items stored

void setup() {
  Serial.begin(9600); // Initialize serial communication at 9600 bits per second.
}

void loop() {
  while (Serial.available() > 0) {
    char received = Serial.read();
    // Check if the incoming character is the newline, which indicates the end of the message
    if (received == '\n') {
      // Process the last item if inputData is not empty
      if (inputData != "") {
        items[itemCount++] = inputData;
        Serial.print("Received item: ");
        Serial.println(inputData);
        inputData = ""; // Clear inputData for safety
      }
      
      // Output all received data for debugging or further processing
      Serial.println("All received items:");
      for (int i = 0; i < itemCount; i++) {
        Serial.println(items[i]);
      }
      
      itemCount = 0; // Reset the item count for the next message
    }
    else if (received == '5') {
      // When '5' is found, save the current inputData to the array
      if (inputData != "") {
        items[itemCount++] = inputData;
        Serial.print("Received item: ");
        Serial.println(inputData);
      }
      inputData = ""; // Clear inputData for the next part of the message
    } else {
      inputData += received; // Add characters to inputData until '5' or '\n' is found
    }
  }
}
