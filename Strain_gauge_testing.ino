//Priyanka

#include <Servo.h>
int signal;
byte servoPin = 4;
Servo servo;
void setup() {
  Serial.begin(9600);
  servo.attach(servoPin);

  servo.writeMicroseconds(1500); // send "stop" signal to ESC. Stays ON; 0 rpm

  delay(7000); // delay to allow the ESC to recognize the stopped signal
}

void loop() {
  int signal; //  Signal value should be between 1100 and 1900
  String a;
  a=Serial.readString();
  signal=a.toInt();
  Serial.println(signal);
  if(signal>1100 && signal<1900)
  {
    while(!(Serial.available()))
  {
    servo.writeMicroseconds(signal); // Send signal to ESC.
  }
}
}
