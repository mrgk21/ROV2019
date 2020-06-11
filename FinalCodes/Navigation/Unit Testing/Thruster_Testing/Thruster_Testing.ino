#include <Servo.h>
int signal;
byte servoPin = 4;


//Signals to the thurster is given by the standard servo library. An object instance is created for the same purpose.
Servo servo;

void setup() {
  Serial.begin(9600);            //UART is initialised to recieve data from serial monitor
  servo.attach(servoPin);        //Attaching the servo object with the pin configures the thurster to listen to the "writeMicroseconds" function.
  servo.writeMicroseconds(1500); // 1500 delay is the stop signal.
  delay(7000);                   // delay to allow the ESC to recognize the stopped signal
}

void loop() {
  int signal; //  Signal value should be between 1100 and 1900
  //Read the delay from user input and convert it to int.
  String a = Serial.readString();
  signal = a.toInt();

  //Sending the signal to thruster whenever the serial input is changed.
  Serial.println(signal);
  if (signal > 1100 && signal < 1900)
  {
    while (!(Serial.available()))
    {
      servo.writeMicroseconds(signal); // Send signal to ESC.
    }
  }
}
