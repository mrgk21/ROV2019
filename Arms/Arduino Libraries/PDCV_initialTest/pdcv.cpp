#include "pdcv.h"
#include "PID_v1.h"

int pdcv::delay_ms = 0;

void pdcv::setDelay(int delay)
{
  pdcv::delay_ms = delay;
}


pdcv::pdcv(int p1, int p2, bool reverse)
{
  pin1 = p1;
  pin2 = p2;
  reverseDir = reverse;
}

pdcv::pdcv(int p1, int p2, bool reverse, int _analogPin, double _Kp_pos, double _Kd_pos)
{
  myPID_pos = new PID(&Input, &Output, &Setpoint, _Kp_pos, 0, _Kd_pos, DIRECT);
  analogPin = _analogPin;
  pin1 = p1;
  pin2 = p2;
  reverseDir = reverse;
}

pdcv::pdcv(int p1, int p2, bool reverse, int _analogPin, double _Kp_pos, double _Kd_pos, double _Kp_neg, double _Kd_neg)
{
  myPID_pos = new PID(&Input, &Output, &Setpoint, _Kp_pos, 0, _Kd_pos, DIRECT);
  myPID_neg = new PID(&Input, &Output, &Setpoint, _Kp_neg, 0, _Kd_neg, DIRECT);
  myPID_pos->SetOutputLimits(-255, 255);
  myPID_neg->SetOutputLimits(-255, 255);

  analogPin = _analogPin;
  pin1 = p1;
  pin2 = p2;
  reverseDir = reverse;
}

void pdcv::pdcv_setup()
{
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  myPID_pos->SetMode(AUTOMATIC);
  myPID_neg->SetMode(AUTOMATIC);
}

void pdcv::pdcv_forward(bool backToStatic)
{
  if (reverseDir)
  {
    analogWrite(pin2, OutputNegative ? abs(Output) : Output);
    analogWrite(pin1, 0);
  }
  else
  {
    analogWrite(pin1, OutputNegative ? abs(Output) : Output);
    analogWrite(pin2, 0);
  }

  if (backToStatic)
  {
    delay(pdcv::delay_ms);
    this->pdcv_static();
  }
}

void pdcv::pdcv_backward(bool backToStatic)
{
  if (reverseDir)
  {
    analogWrite(pin1, OutputNegative ? abs(Output) : Output);
    analogWrite(pin2, 0);
  }
  else
  {
    analogWrite(pin2, OutputNegative ? abs(Output) : Output);
    analogWrite(pin1, 0);
  }
  if (backToStatic)
  {
    delay(pdcv::delay_ms);
    this->pdcv_static();
  }
}

void pdcv::pdcv_static()
{
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
}

void pdcv::pdcv_setPoint(double setPoint)
{
  Setpoint = setPoint;
}

void pdcv::pdcv_setSpeed(double speed)
{
  Output = speed;
}
void pdcv::pdcv_PID_dual()
{
  Input = analogRead(analogPin);
  Serial.print("Analog read: ");
  Serial.println(Input);
  if (abs(Setpoint - Input) > 10) 
  {
    if ((Setpoint - Input) > 0)
    {
      myPID_pos->Compute();
      if (Output < OUTPUT_LIMIT_MIN_POS)
      {
        Output = OUTPUT_LIMIT_MIN_POS;
      }
    }
    else if ((Setpoint - Input) < 0)
    {
      myPID_neg->Compute();
      if (Output > -OUTPUT_LIMIT_MIN_NEG)
      {
        Output = -OUTPUT_LIMIT_MIN_NEG;
      }
    }
  }
  else
  {
    Output = 0;
  }
  Serial.print("Output: ");
  Serial.println(Output);
  if (Output > 0)
  {
    OutputNegative = false;
    if (reverseDir)
      pdcv_backward(false);
    else
      pdcv_forward(false);
  }
  else
  {
    OutputNegative = true;
    if (reverseDir)
      pdcv_forward(false);
    else
      pdcv_backward(false);
  }
}

void pdcv::pdcv_PID()
{
  Input = analogRead(analogPin);
  Serial.print("Analog read: ");
  Serial.println(Input);
  if (abs(Setpoint - Input) > 10) 
  {
    myPID_pos->Compute();
    if (Output > 0 && Output < OUTPUT_LIMIT_MIN_POS)
    {
      Output = OUTPUT_LIMIT_MIN_POS;
    }
    else if (Output < 0 && Output > -OUTPUT_LIMIT_MIN_NEG)
    {
        Output = -OUTPUT_LIMIT_MIN_NEG;
    }
  }
  else
  {
    Output = 0;
  }

  Serial.print("Output: ");
  Serial.println(Output);
  if (Output > 0)
  {
    OutputNegative = false;
    if (reverseDir)
      pdcv_backward(false);
    else
      pdcv_forward(false);
  }
  else
  {
    OutputNegative = true;
    if (reverseDir)
      pdcv_forward(false);
    else
      pdcv_backward(false);
  }
}