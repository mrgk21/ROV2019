#include "pdcv.h"
#define ARRAY_SIZE(A) sizeof(A)/sizeof(A[0])

String data = "";
String prevData = "";
float _4dof[4];
float _6dof[6];
char* ArmFormat = "%f, %f, %f, %f, %f, %f, %f, %f, %f, %f";

void ArmMove();
unsigned char GetArmSpeed(float joyRead);


pdcv p_6dof[] = {pdcv(8, 9, false), pdcv(4, 5, false), pdcv(12, 13 , false), pdcv(2, 3, false), pdcv(6, 7, false)};


void setup() {
  for (int i = 0; i < ARRAY_SIZE(p_6dof); i++)
  {
    p_6dof[i].pdcv_setup();
  }
  pdcv::setDelay(500);
  Serial1.begin(9600);
  Serial1.setTimeout(20);
  Serial.begin(9600);
  Serial.setTimeout(100);
}

void loop() {
  if (Serial1.available()) {
    data = Serial1.readString();
    Serial1.flush();
    sscanf(&data[0], ArmFormat, &_4dof[0], &_4dof[1], &_4dof[2], &_4dof[3], &_6dof[0], &_6dof[1], &_6dof[2], &_6dof[3], &_6dof[4], &_6dof[5]);
  }
  ArmMove();
  prevData = data;
}

void ArmMove() {
  if (!prevData.equals(data)) {
    Serial.println(data);
    Serial.flush();
    for (int i = 0; i < 6; i++)
    {
      p_6dof[i].pdcv_setSpeed(GetArmSpeed(_6dof[i]));
      if (_6dof > 0)
      {
        p_6dof[i].pdcv_forward(true);
      }
      else
      {
        p_6dof[i].pdcv_backward(true);
      }
    }

    Serial1.println("hello from arm");
    Serial1.flush();
  }
}

unsigned char GetArmSpeed(float joyRead)
{
  return floor(map(abs(joyRead), 0.0, 1.0, 0, 255));
}
