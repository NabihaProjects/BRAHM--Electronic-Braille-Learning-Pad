// Include library for serial communication using digital pins
#include <SoftwareSerial.h>

// Include library to control the DFPlayer Mini audio module
#include <DFRobotDFPlayerMini.h>

// Create software serial connection
// A4 = RX (receive), A3 = TX (transmit)
SoftwareSerial mySerial(A4, A3);

// Create DFPlayer object
DFRobotDFPlayerMini mp3;

// Define row pins for the button matrix
// These pins will activate each row one by one
int rowPins[3] = {2, 3, 4};

// Define column pins for the button matrix
// These pins detect which button is pressed
int colPins[3] = {5, 6, 7};

// Labels corresponding to each button position
// This is used to print which letter was pressed
char labels[3][3] = {
  {'C','B','A'},
  {'F','E','D'},
  {'I','H','G'}
};

// Audio track numbers stored in the SD card
// Each button corresponds to a specific audio file
int tracks[3][3] = {
  {3, 2, 1},
  {6, 5, 4},
  {9, 8, 7}
};

void setup() {

  // Start serial communication for debugging
  Serial.begin(9600);

  // Configure row pins as OUTPUT
  // Initially keep them HIGH (inactive)
  for (int r = 0; r < 3; r++) {
    pinMode(rowPins[r], OUTPUT);
    digitalWrite(rowPins[r], HIGH);
  }

  // Configure column pins as INPUT with internal pull-up resistors
  // This helps detect button presses
  for (int c = 0; c < 3; c++) {
    pinMode(colPins[c], INPUT_PULLUP);
  }

  // Start communication with DFPlayer module
  mySerial.begin(9600);

  // Give time for module to initialize
  delay(2000);

  // Check if DFPlayer is connected properly
  if (!mp3.begin(mySerial)) {
    Serial.println("dfplayer not found");
    while (true); // Stop program if module not detected
  }

  // Set audio volume (0–30)
  mp3.volume(20);

  Serial.println("ready");
}

void loop() {

  // Scan each row of the button matrix
  for (int r = 0; r < 3; r++) {

    // Activate the current row
    digitalWrite(rowPins[r], LOW);

    // Check each column in the active row
    for (int c = 0; c < 3; c++) {

      // If button is pressed (LOW because of pull-up)
      if (digitalRead(colPins[c]) == LOW) {

        // Print the letter corresponding to the button
        Serial.println(labels[r][c]);

        // Play the associated audio track from folder 1
        mp3.playFolder(1, tracks[r][c]);

        // Wait until the button is released
        while (digitalRead(colPins[c]) == LOW) delay(10);

        // Small delay to avoid multiple detections
        delay(100);
      }
    }

    // Deactivate the row after scanning
    digitalWrite(rowPins[r], HIGH);
  }
}
