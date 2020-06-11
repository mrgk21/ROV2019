// Make this a local dependency --+
#include <PID_v1.h>               |
//--------------------------------+

void PDCV_PWM(int );
#define M1    30
#define M2    31
#define lowcons 200
#define highcons 550

// modify for optimal performance
double kp = 0.15 , ki = 0 , kd = 0;             
double input = 0, output = 0, setpoint = 0;

long temp;
// if the arm runs only at full speed try 'REVERSE' instead of 'DIRECT'
PID myPID(&input, &output, &setpoint, kp, ki, kd, DIRECT); 


void setup()
{
  // set 31KHz PWM to prevent motor noise
  TCCR1B = TCCR1B & 0b11111000 | 1;
  
  //Initialise UART for monitoring
  Serial.begin (9600);                              
}

void loop()
{
  int error;
  if (Serial.available())
  {
    setpoint = Serial.readString().toInt();
    Serial.println(setpoint);
  }
  input = analogRead(A14);
  Serial.println(input);
  //delay(10);
  
  //Calculate error
  error = setpoint - input;
  
  //If the input is within the bounds, compute the PID response
  if (input >= lowcons && input <= highcons)
  {
    Serial.print("Input: ");
    Serial.println(input);
    if (error > 0)
    {
      myPID.Compute();
      Serial.print("Output: ");
      Serial.println(output);
      PDCV_PWM(output);
    }
    else
    {
      input = -input;
      setpoint = -setpoint;
      myPID.Compute();
      Serial.print("Output: ");
      Serial.println(-output);
      PDCV_PWM(-output);
      input = -input;
      setpoint = -setpoint;
    }
  }
  else
  {
    PDCV_PWM(0);//Stop PDCV
  }
}


void PDCV_PWM(int out)
{
  if (out > 0) {
    analogWrite(M1, out);                             // CW
    analogWrite(M2, 0);
  }
  else if (out < 0)
  {
    analogWrite(M1, 0);
    analogWrite(M2, abs(out));                        // CCW
  }
  else
  {
    analogWrite(M1, 0);
    analogWrite(M2, 0);                        // Zero
  }
}
