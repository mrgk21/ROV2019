#include "pdcv.h"
#define ARRAY_SIZE(A) sizeof(A)/sizeof(A[0])

#define FORMAT "%d_%d%c"

String a = "";
int dof;
int linkIndex;
char motion;


int anaRead;
pdcv p_6dof[] = {pdcv(12, 13, false), pdcv(10, 11, false), pdcv(8, 9, true), pdcv(4, 5, false), pdcv(2, 3, false)};
pdcv p_4dof[] = {pdcv(6, 7, true), pdcv(27, 29, false), pdcv(30, 31, false), pdcv(39, 41, false)};


//pdcv p(2, 3, true, A0,1.7,0.25,1.8,0.3);

//pdcv p(8, 9, true);



void setup() {
  pinMode(13, OUTPUT);
  for (int i = 0; i < ARRAY_SIZE(p_6dof); i++)
  {
    p_6dof[i].pdcv_setup();
  }
  for (int i = 0; i < ARRAY_SIZE(p_4dof); i++)
  {
    p_4dof[i].pdcv_setup();
  }
  pdcv::setDelay(1000);

  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if (Serial.available())
  {
    a = Serial.readString();
    Serial.flush();

    sscanf(&a[0], FORMAT, &dof, &linkIndex, &motion);
    linkIndex--;
    Serial.print("4DOF: ");
    Serial.println(dof);
    Serial.print("linkIndex: ");
    Serial.println(linkIndex);
    Serial.print("motion: ");
    Serial.println((motion == 'f' ? "Forward" : "Backward"));
    if(dof==4)
    {
      p_4dof[linkIndex].pdcv_setSpeed(255);
      if(motion=='f')
      {
        p_4dof[linkIndex].pdcv_forward(true);
      }
      else
      {
        
        p_4dof[linkIndex].pdcv_backward(true);
      }
    }
    else if(dof==6)
    {
      p_6dof[linkIndex].pdcv_setSpeed(255);
      if(motion=='f')
      {
        p_6dof[linkIndex].pdcv_forward(true);
      }
      else
      {
        p_6dof[linkIndex].pdcv_backward(true);
      }
    }
  }
}
