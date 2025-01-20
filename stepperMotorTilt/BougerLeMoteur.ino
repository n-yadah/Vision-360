
// Include libraries and headers
#include "IRremote.h"
#include "IR.h"

// Pin definitions
#define RESISTANCEPIN_1 A0 // Power
#define RESISTANCEPIN_2 A1 // Right
#define RESISTANCEPIN_3 A2 // Left
#define RESISTANCEPIN_4 A3 // Switch Mode

#define SDG 3 // Step Direction for horizontal movement
#define DDG 2 // Digital Direction for horizontal movement
#define SHB 5 // Step Direction for vertical movement
#define DHB 4 // Digital Direction for vertical movement

#define SEUIL 500 // Threshold value

// Global variables
bool isHorz = true;
bool isPressed = false;
IRrecv irrecv(RECEIVER); 
decode_results results; 

// Function declarations
void initializePins();
void readSensors(int &val1, int &val2, int &val3, int &val4);
void handleHorzMovement(int val2, int val3);
void handleVertMovement(int val2, int val3);
void toggleHorzMode(int val1, int val4);
void irRemote();

void setup() {
  initializePins();
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode");
  irrecv.enableIRIn();
}

void loop() {
  int val1, val2, val3, val4;
  readSensors(val1, val2, val3, val4);
  toggleHorzMode(val1, val4);

  if (val1 < SEUIL) {
    if (isHorz) {
      handleHorzMovement(val2, val3);
    } else {
      handleVertMovement(val2, val3);
    }
  }
  irRemote();  // Call IR Remote Control
}

void initializePins() {
  pinMode(RESISTANCEPIN_1, INPUT);
  pinMode(RESISTANCEPIN_2, INPUT);
  pinMode(RESISTANCEPIN_3, INPUT);
  pinMode(RESISTANCEPIN_4, INPUT);

  pinMode(SDG, OUTPUT);
  pinMode(DDG, OUTPUT);
  pinMode(SHB, OUTPUT);
  pinMode(DHB, OUTPUT);

  
}

void readSensors(int &val1, int &val2, int &val3, int &val4) {
  val1 = analogRead(RESISTANCEPIN_1);
  //Serial.println(val1);
  val2 = analogRead(RESISTANCEPIN_2);
  val3 = analogRead(RESISTANCEPIN_3);
  val4 = analogRead(RESISTANCEPIN_4);
}

void toggleHorzMode(int val1, int val4) {
  if (!isPressed && (val4 > SEUIL)) {
    isHorz = !isHorz;
    //Serial.println("Mode changed");
  }
  isPressed = (val4 > SEUIL);
}

void handleHorzMovement(int val2, int val3) {
  if (val2 > SEUIL) {
    digitalWrite(DDG, HIGH);
    //Serial.println("Moving Right");
    stepMotor(SDG, 5);
  } else if (val3 > SEUIL) {
    digitalWrite(DDG, LOW);
    //Serial.println("Moving Left");
    stepMotor(SDG, 5);
  }
}

void handleVertMovement(int val2, int val3) {
  if (val2 > SEUIL) {
    digitalWrite(DHB, HIGH);
    //Serial.println("Moving Up");
    stepMotor(SHB, 5);
  } else if (val3 > SEUIL) {
    digitalWrite(DHB, LOW);
    //Serial.println("Moving Down");
    stepMotor(SHB, 5);
  }
}

void stepMotor(int pin, int steps) {
  for (int i = 0; i < steps; i++) {
    digitalWrite(pin, HIGH);
    delay(2);
    digitalWrite(pin, LOW);
    delay(2);
  }
}

void irRemote() {
  
    if (irrecv.decode(&results)) { // Check for received IR signal
      
        //Serial.println("IR signal received");
        
        switch (results.value) {
            case 0xFFC23D:
                digitalWrite(DDG, HIGH);
                //Serial.println("Moving Right");
                stepMotor(SDG, 50);
                break;
            case 0xFF22DD:
                digitalWrite(DDG, LOW);
                //Serial.println("Moving Left");
                stepMotor(SDG, 50);
                break;
            case 0xFF906F:
                digitalWrite(DHB, HIGH);
                //Serial.println("Moving Up");
                stepMotor(SHB, 50);
                break;
            case 0xFFE01F:
                digitalWrite(DHB, LOW);
                //Serial.println("Moving Down");
                stepMotor(SHB, 50);
                break;
            default:
                //Serial.println("Unknown key");
                break;
        }
        irrecv.resume(); // Prepare for the next IR signal
    }
    //delay(400); // Prevent repeated signals
}
