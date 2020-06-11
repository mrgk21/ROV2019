#include <Wire.h>
#include <MPU6050.h>
#include <AccelStepper.h>

void forward();
void reverse();

int pitch, roll, flag;
int offset = 0, tolerance = 10, speed = 0;

//Objects of respective classes
MPU6050 mpu;
AccelStepper stepper(1, 6, 7); // (driver, pulse, direction)


void setup() {
  pinMode(13, OUTPUT);
  stepper.setEnablePin(5);
  stepper.setMaxSpeed(1000);
  //Wait for MPU to initialise properly
  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G));
}

void loop() {
  //Calculate Roll,Pitch
  roll_pitch();

  //If the error is not within the acceptable limits,
  // correct the error by rotating in opposite direction
  if (pitch > (offset + tolerance))
    reverse();
  else if (pitch < (offset - tolerance))
    forward();
  else
    speed = 0;
  stepper.setSpeed(speed);
  stepper.runSpeed();
}

void forward() {
  speed = 400;
  digitalWrite(13, HIGH);
}

void reverse()  {
  speed = -400;
  digitalWrite(13, LOW);
}

void roll_pitch() {
  // Read normalized values
  Vector normAccel = mpu.readNormalizeAccel();
  
  // Calculate Pitch & Roll
  pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis * normAccel.YAxis + normAccel.ZAxis * normAccel.ZAxis)) * 180.0) / M_PI;
  roll = (atan2(normAccel.YAxis, normAccel.ZAxis) * 180.0) / M_PI;
}
