#include "pdcv.h"



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


void pdcv::pdcv_setup()
{
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
}

void pdcv::pdcv_forward()
{
  if (reverseDir)
  {
    digitalWrite(pin2, HIGH);
    digitalWrite(pin1, LOW);
  }
  else
  {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
  }
  delay(pdcv::delay_ms);
  this->pdcv_static();
  
}

void pdcv::pdcv_backward()
{
  if (reverseDir)
  {
    digitalWrite(pin1, HIGH);
    digitalWrite(pin2, LOW);
  }
  else
  {
    digitalWrite(pin2, HIGH);
    digitalWrite(pin1, LOW);
  }
  delay(pdcv::delay_ms);
  this->pdcv_static();
}

void pdcv::pdcv_static()
{
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
}
