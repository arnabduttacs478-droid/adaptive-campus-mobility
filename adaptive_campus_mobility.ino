//adaptive campus mobility and parking
#include <LedControl.h>

LedControl matrix = LedControl(11, 13, 10, 3);

const int ENTRY_DISPLAY    = 0;
const int GROUND_DISPLAY   = 1;
const int BASEMENT_DISPLAY = 2;

const int ENTRY_TRIG = 2;
const int ENTRY_ECHO = 3;
const int GROUND_TRIG = 4;
const int GROUND_ECHO = 5;
const int BASEMENT_TRIG = 6;
const int BASEMENT_ECHO = A2;

const int GROUND_RED    = 7;
const int GROUND_GREEN  = 8;
const int GROUND_YELLOW = 9;

const int BASEMENT_RED    = 12;
const int BASEMENT_GREEN  = A0;
const int BASEMENT_YELLOW = A1;

const int DETECTION_DISTANCE = 8;

// Number of stable readings required
const int REQUIRED_OCCUPIED_READINGS = 3;
const int REQUIRED_FREE_READINGS = 2;

enum Destination { NONE, GROUND, BASEMENT };
Destination selectedDestination = NONE;

bool waitingForEntryToClear = false;

// Stable occupancy states
bool groundStableOccupied = false;
bool basementStableOccupied = false;

// Counters for stable detection
int groundOccupiedCount = 0;
int groundFreeCount = 0;

int basementOccupiedCount = 0;
int basementFreeCount = 0;


void setup() {

  pinMode(ENTRY_TRIG, OUTPUT);
  pinMode(ENTRY_ECHO, INPUT);

  pinMode(GROUND_TRIG, OUTPUT);
  pinMode(GROUND_ECHO, INPUT);

  pinMode(BASEMENT_TRIG, OUTPUT);
  pinMode(BASEMENT_ECHO, INPUT);

  pinMode(GROUND_RED, OUTPUT);
  pinMode(GROUND_GREEN, OUTPUT);
  pinMode(GROUND_YELLOW, OUTPUT);

  pinMode(BASEMENT_RED, OUTPUT);
  pinMode(BASEMENT_GREEN, OUTPUT);
  pinMode(BASEMENT_YELLOW, OUTPUT);

  allLEDsOff();

  for (int i = 0; i < 3; i++) {
    matrix.shutdown(i, false);
    matrix.setIntensity(i, 5);
    matrix.clearDisplay(i);
  }

  groundFreeLED();
  basementFreeLED();

  showCheck(GROUND_DISPLAY);
  showCheck(BASEMENT_DISPLAY);

  clearDisplay(ENTRY_DISPLAY);
}


void loop() {

  updateSystem();

  delay(100);
}


void updateSystem() {

  // Read the three sensors
  bool groundRaw = isVehicleDetected(GROUND_TRIG, GROUND_ECHO);

  bool basementRaw = isVehicleDetected(BASEMENT_TRIG, BASEMENT_ECHO);

  bool vehicleAtEntry =
    isVehicleDetected(ENTRY_TRIG, ENTRY_ECHO);


  // Convert ground sensor reading into stable occupancy
  groundStableOccupied =
    getStableGroundStatus(groundRaw);


  // Convert basement sensor reading into stable occupancy
  basementStableOccupied =
    getStableBasementStatus(basementRaw);


  // Use stable parking status
  bool groundOccupied = groundStableOccupied;
  bool basementOccupied = basementStableOccupied;


  // If selected destination becomes genuinely occupied,
  // cancel that destination.
  if (selectedDestination == GROUND && groundOccupied)
    selectedDestination = NONE;

  if (selectedDestination == BASEMENT && basementOccupied)
    selectedDestination = NONE;


  // Reset entry waiting state when vehicle leaves entry
  if (!vehicleAtEntry)
    waitingForEntryToClear = false;


  // Select a destination for a new vehicle
  if (vehicleAtEntry &&
      !waitingForEntryToClear &&
      selectedDestination == NONE) {

    waitingForEntryToClear = true;

    if (!groundOccupied)
      selectedDestination = GROUND;

    else if (!basementOccupied)
      selectedDestination = BASEMENT;

    else
      selectedDestination = NONE;
  }


  updateLEDs(groundOccupied, basementOccupied);

  updateDisplays(
    vehicleAtEntry,
    groundOccupied,
    basementOccupied
  );
}


// --------------------------------------------------
// STABLE GROUND SENSOR
// --------------------------------------------------

bool getStableGroundStatus(bool rawReading) {

  if (rawReading) {

    groundOccupiedCount++;
    groundFreeCount = 0;

    if (groundOccupiedCount >= REQUIRED_OCCUPIED_READINGS) {
      groundOccupiedCount = REQUIRED_OCCUPIED_READINGS;
      return true;
    }

  } else {

    groundFreeCount++;
    groundOccupiedCount = 0;

    if (groundFreeCount >= REQUIRED_FREE_READINGS) {
      groundFreeCount = REQUIRED_FREE_READINGS;
      return false;
    }
  }

  // Keep previous state while readings are uncertain
  return groundStableOccupied;
}


// --------------------------------------------------
// STABLE BASEMENT SENSOR
// --------------------------------------------------

bool getStableBasementStatus(bool rawReading) {

  if (rawReading) {

    basementOccupiedCount++;
    basementFreeCount = 0;

    if (basementOccupiedCount >= REQUIRED_OCCUPIED_READINGS) {
      basementOccupiedCount = REQUIRED_OCCUPIED_READINGS;
      return true;
    }

  } else {

    basementFreeCount++;
    basementOccupiedCount = 0;

    if (basementFreeCount >= REQUIRED_FREE_READINGS) {
      basementFreeCount = REQUIRED_FREE_READINGS;
      return false;
    }
  }

  // Keep previous state while readings are uncertain
  return basementStableOccupied;
}


// --------------------------------------------------
// LED CONTROL
// --------------------------------------------------

void updateLEDs(bool groundOccupied, bool basementOccupied) {

  // GROUND

  if (groundOccupied)
    groundOccupiedLED();

  else if (selectedDestination == GROUND)
    groundApproachingLED();

  else
    groundFreeLED();


  // BASEMENT

  if (basementOccupied)
    basementOccupiedLED();

  else if (selectedDestination == BASEMENT)
    basementApproachingLED();

  else
    basementFreeLED();
}


// --------------------------------------------------
// DISPLAY CONTROL
// --------------------------------------------------

void updateDisplays(
  bool vehicleAtEntry,
  bool groundOccupied,
  bool basementOccupied
) {

  // ENTRY DISPLAY

  if (!vehicleAtEntry)
    clearDisplay(ENTRY_DISPLAY);

  else if (selectedDestination == GROUND)
    showG(ENTRY_DISPLAY);

  else if (selectedDestination == BASEMENT)
    showB(ENTRY_DISPLAY);

  else if (groundOccupied && basementOccupied)
    showX(ENTRY_DISPLAY);


  // GROUND DISPLAY

  if (groundOccupied)
    showX(GROUND_DISPLAY);

  else if (selectedDestination == GROUND)
    showRightArrow(GROUND_DISPLAY);

  else
    showCheck(GROUND_DISPLAY);


  // BASEMENT DISPLAY

  if (basementOccupied)
    showX(BASEMENT_DISPLAY);

  else if (selectedDestination == BASEMENT)
    showDownArrow(BASEMENT_DISPLAY);

  else
    showCheck(BASEMENT_DISPLAY);
}


// --------------------------------------------------
// ULTRASONIC SENSOR
// --------------------------------------------------

bool isVehicleDetected(int trigPin, int echoPin) {

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);

  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000);

  if (duration == 0)
    return false;

  int distance = duration * 0.034 / 2;

  if (distance > 0 &&
      distance <= DETECTION_DISTANCE)
    return true;

  return false;
}


// --------------------------------------------------
// GROUND LEDs
// --------------------------------------------------

void groundFreeLED() {

  digitalWrite(GROUND_GREEN, HIGH);
  digitalWrite(GROUND_YELLOW, LOW);
  digitalWrite(GROUND_RED, LOW);
}


void groundApproachingLED() {

  digitalWrite(GROUND_GREEN, LOW);
  digitalWrite(GROUND_YELLOW, HIGH);
  digitalWrite(GROUND_RED, LOW);
}


void groundOccupiedLED() {

  digitalWrite(GROUND_GREEN, LOW);
  digitalWrite(GROUND_YELLOW, LOW);
  digitalWrite(GROUND_RED, HIGH);
}


// --------------------------------------------------
// BASEMENT LEDs
// --------------------------------------------------

void basementFreeLED() {

  digitalWrite(BASEMENT_GREEN, HIGH);
  digitalWrite(BASEMENT_YELLOW, LOW);
  digitalWrite(BASEMENT_RED, LOW);
}


void basementApproachingLED() {

  digitalWrite(BASEMENT_GREEN, LOW);
  digitalWrite(BASEMENT_YELLOW, HIGH);
  digitalWrite(BASEMENT_RED, LOW);
}


void basementOccupiedLED() {

  digitalWrite(BASEMENT_GREEN, LOW);
  digitalWrite(BASEMENT_YELLOW, LOW);
  digitalWrite(BASEMENT_RED, HIGH);
}


// --------------------------------------------------
// ALL LEDs OFF
// --------------------------------------------------

void allLEDsOff() {

  digitalWrite(GROUND_GREEN, LOW);
  digitalWrite(GROUND_YELLOW, LOW);
  digitalWrite(GROUND_RED, LOW);

  digitalWrite(BASEMENT_GREEN, LOW);
  digitalWrite(BASEMENT_YELLOW, LOW);
  digitalWrite(BASEMENT_RED, LOW);
}


// --------------------------------------------------
// G DISPLAY
// --------------------------------------------------

void showG(int device) {

  byte G[8] = {
    B00111100,
    B01100110,
    B11000000,
    B11001110,
    B11000110,
    B01100110,
    B00111100,
    B00000000
  };

  showPattern(device, G);
}


// --------------------------------------------------
// B DISPLAY
// --------------------------------------------------

void showB(int device) {

  byte B[8] = {
    B11111100,
    B11000110,
    B11000110,
    B11111100,
    B11000110,
    B11000110,
    B11111100,
    B00000000
  };

  showPattern(device, B);
}


// --------------------------------------------------
// X DISPLAY
// --------------------------------------------------

void showX(int device) {

  byte X[8] = {
    B11000011,
    B01100110,
    B00111100,
    B00011000,
    B00011000,
    B00111100,
    B01100110,
    B11000011
  };

  showPattern(device, X);
}


// --------------------------------------------------
// CHECK DISPLAY
// --------------------------------------------------

void showCheck(int device) {

  byte check[8] = {
    B00000000,
    B00000001,
    B00000010,
    B00000100,
    B10001000,
    B01010000,
    B00100000,
    B00000000
  };

  showPattern(device, check);
}


// --------------------------------------------------
// DOWN ARROW
// --------------------------------------------------

void showDownArrow(int device) {

  byte arrow[8] = {
    B00011000,
    B00011000,
    B00011000,
    B00011000,
    B00011000,
    B01111110,
    B00111100,
    B00011000
  };

  showPattern(device, arrow);
}


// --------------------------------------------------
// RIGHT ARROW
// --------------------------------------------------

void showRightArrow(int device) {

  byte arrow[8] = {
    B00011000,
    B00001100,
    B00000110,
    B11111111,
    B11111111,
    B00000110,
    B00001100,
    B00011000
  };

  showPattern(device, arrow);
}


// --------------------------------------------------
// DISPLAY PATTERN
// --------------------------------------------------

void showPattern(int device, byte pattern[8]) {

  for (int row = 0; row < 8; row++) {
    matrix.setRow(device, row, pattern[row]);
  }
}


// --------------------------------------------------
// CLEAR DISPLAY
// --------------------------------------------------

void clearDisplay(int device) {

  matrix.clearDisplay(device);
}
