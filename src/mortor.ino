#include <Stepper.h>
const int steps = 3200  //
Stepper myStepper(steps, 8, 9, 10, 11);

void setup() {
  //Serial.begin(9600);
  //myStepper.setSpeed(10);
}

void loop() {
  myStepper.step(3200);
  digitalWrite(8,  LOW); 
  digitalWrite(9,  LOW); 
  digitalWrite(10, LOW); 
  digitalWrite(11, LOW);
  delay(1000);
}