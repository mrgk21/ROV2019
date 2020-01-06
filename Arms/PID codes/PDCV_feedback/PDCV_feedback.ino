#include <PID_v1.h>


#define PDCV_PIN_0 27
#define PDCV_PIN_1 29
#define PDCV_ANALOG A0

#define TIME_THRESH 500
#define VEL 20



int REACH = 400;
#define TOLERANCE 10

#define MIN_PWM 130
#define MAX_PWM 140


long prevTime = 0;
int prev_count = 0;
int armSpeed = 120;
bool forward = true;
bool once = false;

void ArmMove(bool forward)
{
  if (forward)
  {
    analogWrite(PDCV_PIN_0, armSpeed);//<0?abs(armSpeed):armSpeed);
    digitalWrite(PDCV_PIN_1, LOW);
  }
  else
  {
    analogWrite(PDCV_PIN_1, armSpeed);//<0?abs(armSpeed):armSpeed);
    digitalWrite(PDCV_PIN_0, LOW);
  }
}

void ArmStatic(bool forward)
{
  /*
    armSpeed = armSpeed>10?armSpeed-20:0;
    if (forward)
    {
    if(armSpeed!=0)
      analogWrite(PDCV_PIN_0, armSpeed);//<0?abs(armSpeed):armSpeed);
    else
    {
      digitalWrite(PDCV_PIN_0, LOW);
      once = false;
    }
    digitalWrite(PDCV_PIN_1, LOW);
    }
    else
    {
    if(armSpeed!=0)
      analogWrite(PDCV_PIN_1, armSpeed);//<0?abs(armSpeed):armSpeed);
    else
    {
      digitalWrite(PDCV_PIN_1, LOW);
      once = false;
    }
    digitalWrite(PDCV_PIN_0, LOW);
    }
  */
  digitalWrite(PDCV_PIN_0, LOW);
  digitalWrite(PDCV_PIN_1, LOW);
}


/************ PID VARIABLES *****************/
double Setpoint, Input, Output;
double Kp = 0.3, Ki = 5, Kd = 3;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);


void setup() {
  pinMode(39, OUTPUT);
  pinMode(41, OUTPUT);
  digitalWrite(39, LOW);
  digitalWrite(41, LOW);



  myPID.SetOutputLimits(MIN_PWM, MAX_PWM);
  myPID.SetMode(AUTOMATIC);
  pinMode(PDCV_PIN_0, OUTPUT);
  pinMode(PDCV_PIN_1, OUTPUT);
  pinMode(PDCV_ANALOG, INPUT);
  prevTime = millis();
  prev_count = analogRead(PDCV_ANALOG);
  Setpoint = VEL;
  Serial.begin(9600);
}

void loop() {
  if (Serial.available())
  {
    String s = Serial.readString();
    Serial.flush();
    REACH = s.toInt();
  }
  int count = analogRead(PDCV_ANALOG);
  Serial.println(count);
  if (millis() - prevTime > TIME_THRESH)
  {
    Input = abs(count - prev_count);
    myPID.Compute();
    armSpeed = Output;
    if (abs(count - REACH) > TOLERANCE)
    {
      if (count > REACH)
      {
        ArmMove(false);
      }
      else
      {
        ArmMove(true);
      }
    }
    else
    {
      /*
        if(!once)
        {
        armSpeed = 140;
        once = true;
        }
      */
      /*
        if (count > REACH)
        {
        ArmStatic(false);
        }
        else
        {
        ArmStatic(true);
        }
      */
      ArmStatic(true);
    }

    prevTime = millis();
    prev_count = count;
  }
}
