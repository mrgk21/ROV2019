#include <Servo.h>

String str1, str2, str3;

float offset1 = 16;
float offset2 = 123;
float offset3 = 80;
float offset4 = 100;

String rec = "";
int link[4];

Servo myservo1; // create servo object to control a servo
Servo myservo2;
Servo myservo3;
Servo myservo4;

void setup() {
  Serial1.begin(9600);
  Serial.begin(115200); //debug
  Serial1.setTimeout(20);
  myservo1.attach(3);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(5);
  myservo3.attach(9);
  myservo4.attach(10);
}

void loop() {
  if (Serial1.available()) {
    rec = Serial1.readString();
    int size = sscanf(&rec[0], "%d, %d, %d, %d",  &link[0], &link[1], &link[2], &link[3]);
    Serial1.flush();
    myservo1.write(link[0] + offset1);
    myservo2.write(link[1] + offset2);
    myservo3.write(link[2] + offset3);
    myservo4.write(link[3] + offset4);
    Serial.println(link[0] + offset1);
    Serial.println(link[1] + offset2);
    Serial.println(link[2] + offset3);
    Serial.println(link[3] + offset4);
    Serial.println();
  }
}
