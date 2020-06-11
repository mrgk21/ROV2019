#include <Wire.h>
#include "MS5837.h"
#include "MPU6050.h"
#include "AccelStepper.h"


MS5837 sensor;
void forward();
void reverse();

int pitch, roll, flag;
int offset = 0, tolerance = 10, speed = 0;

MPU6050 mpu;
AccelStepper stepper(1, 11, 12); // (driver, pulse, direction)

void setup() {

  Serial.begin(9600);

  Serial.println("Starting Iniitialization of Pressure");

  Wire.begin();

  // Initialize pressure sensor
  // Returns true if initialization was successful
  // We can't continue with the rest of the program unless we can initialize the sensor
  while (!sensor.init()) {
    Serial.println("Init failed!");
    Serial.println("Are SDA/SCL connected correctly?");
    Serial.println("Blue Robotics Bar30: White=SDA, Green=SCL");
    Serial.println("\n\n\n");
    delay(5000);
  }
  sensor.setModel(MS5837::MS5837_30BA);
  sensor.setFluidDensity(997); // kg/m^3 (freshwater, 1029 for seawater)
  
  Serial.println("Starting Iniitialization of MPU");

  pinMode(13, OUTPUT);
  stepper.setEnablePin(5);
  stepper.setMaxSpeed(1000);

  while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G));
}

void loop() 
{
  roll_pitch();
  if (pitch > (offset + tolerance))
    reverse();
  else if (pitch < (offset - tolerance))
    forward();
  else
    speed = 0;
  stepper.setSpeed(speed);
  stepper.runSpeed();

  sensor.read();
/*
  Serial.print("Pressure: ");
  Serial.print(sensor.pressure());
  Serial.println(" mbar");

  Serial.print("Temperature: ");
  Serial.print(sensor.temperature());
  Serial.println(" deg C");
*/
  Serial.print("Depth: ");
  Serial.print(sensor.depth());
  Serial.println(" m");
/*
  Serial.print("Altitude: ");
  Serial.print(sensor.altitude());
  Serial.println(" m above mean sea level");
*/
  delay(100);
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
  Serial.println(pitch);
  //Serial.println(roll);
}
