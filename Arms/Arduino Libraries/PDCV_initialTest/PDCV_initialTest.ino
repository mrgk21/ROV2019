#include "pdcv.h"

pdcv p = pdcv(2,3,false);
void setup() {
  p.pdcv_setup();
  pinMode(13, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available())
  {
    char a = Serial.read();
    switch (a)
    {
      case 'w':
        p.pdcv_forward();
        digitalWrite(13, HIGH);
        Serial.println("forward");
        break;
      case 's':
        p.pdcv_backward();
        digitalWrite(13, LOW);
        Serial.println("backward");
        break;
      case 'a':
        p.pdcv_static();
        Serial.println("static");
        break;
    }
  }
}
