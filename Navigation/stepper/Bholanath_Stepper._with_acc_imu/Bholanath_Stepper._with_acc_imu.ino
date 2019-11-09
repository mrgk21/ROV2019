
  #include <Wire.h>
  #include <MPU6050.h>
  
int PUL=4; //define Pulse pin
int DIR=3; //define Direction pin
int ENA=2; //define Enable Pin
int flag =0;
void forward();
void reverse();
char val;
int pitch;
int roll;
//  TCCR2B = TCCR2B&B11111000|B00000001;//for frequency of 31372.55 Hz
MPU6050 mpu;

int set_angle = -20;

void setup()
{
  Serial.begin(115200);

  Serial.println("Initialize MPU6050");

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }
    pinMode (PUL, OUTPUT);
    pinMode (DIR, OUTPUT);
    pinMode (ENA, OUTPUT);
   
}

void loop() 
{

 roll_pitch();
 if(pitch>(set_angle+2))
 reverse();
 else if(pitch<(set_angle-2))
 forward();
 else ;
}

void forward()
{
    for (int i=0; i<6400; i++)    //i=6400 for 200 steps
  {
    digitalWrite(DIR,LOW ); //Away from motor
    digitalWrite(ENA,HIGH);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(50);
    digitalWrite(PUL,LOW);
    delayMicroseconds(50);
    
  }
 
  
}

void reverse()
{
   for (int i=0; i<6400; i++)   
  {
    digitalWrite(DIR,HIGH);//Towards motor
    digitalWrite(ENA,HIGH);
    digitalWrite(PUL,HIGH);
    delayMicroseconds(50);
    digitalWrite(PUL,LOW);
    delayMicroseconds(50);
  }
}
void roll_pitch()
{
   // Read normalized values 
  Vector normAccel = mpu.readNormalizeAccel();

  // Calculate Pitch & Roll
   pitch = -(atan2(normAccel.XAxis, sqrt(normAccel.YAxis*normAccel.YAxis + normAccel.ZAxis*normAccel.ZAxis))*180.0)/M_PI;
   roll = (atan2(normAccel.YAxis, normAccel.ZAxis)*180.0)/M_PI;

  // Output
  Serial.print(" Pitch = ");
  Serial.print(pitch);
  Serial.print(" Roll = ");
  Serial.print(roll);
  
  Serial.println();
  
  delay(10);
}


