#include "pdcv.h"
#define ARRAY_SIZE(A) sizeof(A)/sizeof(A[0])

String a = "";
int linkIndex;
char motion;

//pdcv p[] = {pdcv(2, 3, true), pdcv(4, 5 , false), pdcv(6, 7 , false)};

pdcv p(2, 3, true, A0,1.7,0.25,1.8,0.3);

//pdcv p(2,3,true);

void setup() {
  pinMode(13, OUTPUT);
  //  for (int i = 0; i < ARRAY_SIZE(p); i++)
  //  {
  //    p[i].pdcv_setup();
  //  }

  p.pdcv_setup();

  //pdcv::setDelay(200);


  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available())
  {
    int out = Serial.readString().toInt();
    p.pdcv_setPoint(out);
  }
  p.pdcv_PID();

/*  
    if(Serial.available())
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

    p.pdcv_setSpeed(temp_num.toInt());
    motion = temp_motion[0];
    Serial.println(temp_num.toInt());
    Serial.println(motion);
    switch (motion)
    {
      case 'f':
        p.pdcv_forward(false);
        break;
      case 'b':
        p.pdcv_backward(false);
        break;
    }

  
}
*/



}
