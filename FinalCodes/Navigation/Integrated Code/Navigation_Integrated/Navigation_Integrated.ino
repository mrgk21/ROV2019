#include "flags.h"
#include <AccelStepper.h>
#include <Servo.h>
#include <Wire.h>
#include "MPU6050.h"

/********** VARIABLES *******/
bool isMPUinit = false;

const char *NavFormat = "%d, %d, %d, %d";
int vel[4] = {0};
String data, prevData;

/**** PUMP SOLENOID *****/
#define PUMP_PIN A6
#define SOL_PIN A7

//Initialisation for the pump and solenoid pins chosen.
void PumpSetup()
{
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(SOL_PIN, OUTPUT);
  digitalWrite(PUMP_PIN, LOW);
  digitalWrite(SOL_PIN, LOW);
}

//This is to change the direction(altitude) of the ROV.
void PumpVel(int z)
{
  if (z != 0)
  {
    if (z < 0)
    {
      digitalWrite(PUMP_PIN, LOW);
      digitalWrite(SOL_PIN, HIGH);
    }
    else
    {
      digitalWrite(PUMP_PIN, HIGH);
      digitalWrite(SOL_PIN, HIGH);
    }
  }
  else
  {
    digitalWrite(PUMP_PIN, LOW);
    digitalWrite(SOL_PIN, LOW);
  }
}

/*********** MPU AND STEPPER ****************/

int pitch = 0, roll, flag, prev_pitch = 0;
int speed = 0;
//Object instances required for the respective libraries to work correctly.
MPU6050 mpu;
AccelStepper stepper(1, STEPPER_PUL, STEPPER_DIR);

void PitchCorrect()
{
  //Read the IMU values.
  Vector normAccel = mpu.readNormalizeAccel();
  delay(1);

  //Required for pitch correction using the lead screw assembly.
  pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis * normAccel.YAxis + normAccel.ZAxis * normAccel.ZAxis)) * 180.0) / M_PI;

  //Use this value for monitoring purposes. Not sent over the communication link yet.
  //roll = (atan2(normAccel.YAxis, normAccel.ZAxis) * 180.0) / M_PI;

  //Pitch correction Logic
  if (PITCH_TOLERANCE - abs(pitch) < 0)
  {
    if (pitch > (PITCH_OFFSET + PITCH_TOLERANCE))
    {
      speed = -STEPPER_SPEED;
    }
    else if (pitch < (PITCH_OFFSET - PITCH_TOLERANCE))
    {
      speed = STEPPER_SPEED;
    }
    else
    {
      speed = 0;
    }
    stepper.setSpeed(speed);
    stepper.runSpeed();
  }
  prev_pitch = pitch;
}

/*******************    THRUSTERS   ***********************/
Servo thrusters[6];
int servoPin[] = {3, 4, 8, 6, 7, 5}; //Current arrangement due to isolation board was damaged.
//int servoPin[] = {3, 4, 5, 6, 7, 8};// Original

/******************************************************************************************************/
//CONVENTIONS FOR THE SEQUENCE OF THRUSTERS DO NOT CHANGE THIS EVER.
/* Z_DIRECTION
   +-----------------------+
   | 0                   1 |
   |                       |
   |                       |
   |                       |
   |          2            |
   +-----------------------+
   Y_DIRECTION
   +-----------------------+
   |                       |
   |                       |
   | 3                   4 |
   |                       |
   |                       |
   +-----------------------+
   X_DIRECTION
   +-----------------------+
   |                       |
   |                       |
   |          5            |
   |                       |
   |                       |
   +-----------------------+
*/
/******************************************************************************************************/

//Thrusters mapped to velocities and signalled accordingly. Needs refactoring when PID control for balancing is implemented.
void MoveThrusters()
{
  if (vel[0] != 0)
  {
    if (vel[0] > 0)
    {
      thrusters[HORZ].writeMicroseconds(T_POS);
    }
    else
    {
      thrusters[HORZ].writeMicroseconds(T_NEG);
    }
  }
  else
  {
    thrusters[HORZ].writeMicroseconds(T_ZERO);
  }

  if (vel[1] != 0)
  {
    if (vel[1] > 0)
    {
      thrusters[FWD_1].writeMicroseconds(T_POS);
      thrusters[FWD_2].writeMicroseconds(T_POS);
    }
    else
    {
      thrusters[FWD_1].writeMicroseconds(T_NEG);
      thrusters[FWD_2].writeMicroseconds(T_NEG);
    }
  }
  else
  {
    thrusters[FWD_1].writeMicroseconds(T_ZERO);
    thrusters[FWD_2].writeMicroseconds(T_ZERO);
  }

  if (vel[2] != 0)
  {
    if (vel[2] > 0)
    {
      thrusters[UP_1].writeMicroseconds(T_POS);
      thrusters[UP_2].writeMicroseconds(T_POS);
      thrusters[UP_3].writeMicroseconds(T_POS);
    }
    else
    {
      thrusters[UP_1].writeMicroseconds(T_NEG);
      thrusters[UP_2].writeMicroseconds(T_NEG);
      thrusters[UP_3].writeMicroseconds(T_NEG);
    }
  }
  else
  {
    thrusters[UP_1].writeMicroseconds(T_ZERO);
    thrusters[UP_2].writeMicroseconds(T_ZERO);
    thrusters[UP_3].writeMicroseconds(T_ZERO);
  }
}

/***************   MAIN CODE  *********************/
void setup()
{
  //Initialisation of UART for communication
  Serial.begin(9600);
  Serial.setTimeout(20);
  Serial2.begin(9600);
  Serial2.setTimeout(20);

  //REFER TO THESE MACROS in flags.h
  debug_pump(PumpSetup();)
  debug_step(
    stepper.setEnablePin(STEPPER_EN);
    stepper.setMaxSpeed(STEPPER_MAXSPEED);
    while (!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G));
    isMPUinit = true; //Required for MPU handshake
  )
  debug_thr(
    for (int i = 0; i < 6; i++) {
      thrusters[i].attach(servoPin[i]);
      thrusters[i].writeMicroseconds(T_ZERO);
      delay(100);
    } 
    delay(7000);
  )
}

void loop()
{
  //Read data from joystick.
  if (Serial2.available())
  {
    data = Serial2.readString();
    Serial2.flush();
    Serial.println(data);
    Serial.flush();

    //Initial Check for MPU Handshake
    if (data.substring(0, 12) == "is MPU init?")
    {
      if (isMPUinit)
      {
        Serial2.println("HANDSHAKE SUCCESSFUL");
      }
    }
    else
    {
      //Put the data into respective fields.
      sscanf(&data[0], NavFormat, &vel[0], &vel[1], &vel[2], &vel[3]);
    }
  }
  //If data has changed from previous data, do the following actions.
  if (prevData != data)
  {
    debug_thr(MoveThrusters();)
    debug_pump(PumpVel(vel[3]);)
  }
  //Pitch correction is ON at all the time.
  debug_step(PitchCorrect();)

  //Send data back to GUI.
  if (isMPUinit)
  {
    Serial2.print(pitch);
    Serial2.print(',');
    Serial2.print(pitch);
    Serial2.print(',');
    Serial2.print(pitch);
    Serial2.flush();
  }
  prevData = data;
}
