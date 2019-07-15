#include "pdcv.h"
#define ARRAY_SIZE(A) sizeof(A)/sizeof(A[0])


String a = "";
int linkIndex;
char motion;

pdcv p[] = {pdcv(2, 3, true), pdcv(4, 5 , false), pdcv(8, 9 , false), pdcv(6, 7, true)};

void setup() {
  pinMode(13, OUTPUT);
  for (int i = 0; i < ARRAY_SIZE(p); i++)
  {
    p[i].pdcv_setup();
  }
  pdcv::setDelay(500);
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*
  if (Serial.available())
  {
    a = Serial.readString();
    String temp_num;
    String temp_motion;
    for (int i = 0; i < a.length(); i++)
    {
      if (isDigit(a[i]))
      {
        temp_num += a[i];
      }
      if (isAlpha(a[i]))
      {
        temp_motion += a[i];
      }
    }
    linkIndex = temp_num.toInt() - 1;
    motion = temp_motion[0];
    Serial.println(linkIndex);
    Serial.println(motion);
    switch (motion)
    {
      case 'f':
        p[linkIndex].pdcv_forward();
        if (linkIndex == 0)
        {
          digitalWrite(13, HIGH);
        }
        break;
      case 'b':
        p[linkIndex].pdcv_backward();
        if (linkIndex == 0)
        {
          digitalWrite(13, LOW);
        }
        break;
      case 's':
        p[linkIndex].pdcv_static();
        break;
    }
  }
  */
  /*
  p[0].pdcv_forward();
  p[0].pdcv_forward();
  p[0].pdcv_forward();
  p[0].pdcv_backward();
  p[0].pdcv_backward();
  p[0].pdcv_backward();
  p[1].pdcv_forward();
  p[1].pdcv_forward();
  p[1].pdcv_forward();
  p[1].pdcv_backward();
  p[1].pdcv_backward();
  p[1].pdcv_backward();
  p[2].pdcv_forward();
  p[2].pdcv_forward();
  p[2].pdcv_forward();
  p[2].pdcv_backward();
  p[2].pdcv_backward();
  p[2].pdcv_backward();
  */
  p[3].pdcv_forward();
  p[3].pdcv_forward();
  p[3].pdcv_forward();
  p[3].pdcv_backward();
  p[3].pdcv_backward();
  p[3].pdcv_backward();
  
}
