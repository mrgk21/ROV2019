#include <AccelStepper.h>

AccelStepper stepper(1, 6, 7); // (driver, pulse, direction)

void setup() {
  stepper.setEnablePin(5);
  stepper.setMaxSpeed(1000);
  stepper.setSpeed(200); //measured in steps/sec
}

void loop() {
  stepper.runSpeed(); //execute this as frequently as possible
}
