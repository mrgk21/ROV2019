
String data = "";
String prevData = "";
int velocities[3];
char* thrFormat = "%d, %d, %d";

//byte thr1 = 3;
//byte thr2 = 4;
//byte thr3 = 5;
//byte thr4 = 6;
//byte thr5 = 7;
//byte thr6 = 8;

void ThrusterDuty();

//TO DO : Declare Servo objects within an array
byte servopin[] = { 2,3,6,7,8,9 };
Servo servo[6]; 

/* 
 *  2 - top right 
 *  3 - top left
 *  4 - top end
 *  7 - bottom right
 *  8 - bottom left
 *  9 - bottom middle
 */
 
void setup() {
  Serial1.begin(9600);
  Serial1.setTimeout(20);
  Serial.begin(9600);
  Serial.setTimeout(100);

  // TO DO : servo intiialization using a for loop
  for(int i=0;i<6;i++)
  {
    servo[i].attach(servopin[i]);
  }
  for(int i=0;i<6;i++)
  {
    servo[i].writeMicroseconds(1500); // send "stop" signal to ESC. Stays ON; 0 rpm
  }
  
  delay(7000); // delay to allow the ESC to recognize the stopped signal 
  
}

void loop() {
  if (Serial1.available()) {
    data = Serial1.readString();
    Serial1.flush();
    sscanf(&data[0], thrFormat, &velocities[0], &velocities[1], &velocities[2]);
  }
  ThrusterDuty();
  prevData = data;
}

void ThrusterDuty() {
  if (!prevData.equals(data))
  {
    Serial.println(data);
    Serial.flush();

    //TO DO: Write the code to assign correct pwm to each thruster.
    // Consider the cases of velocities.
    
    if(velocities[1]==-1)
    {
      servo[3].writeMicroseconds(1300);
      servo[4].writeMicroseconds(1300);
    }
    else if(velocities[1]==1)
    {
      servo[3].writeMicroseconds(1700);
      servo[4].writeMicroseconds(1700);
    }
    else (velocities[1]==0)
    {
      servo[3].writeMicroseconds(1500);
      servo[4].writeMicroseconds(1500);
    }
    if(velocities[2]==-1)
    {
      servo[0].writeMicroseconds(1300);
      servo[1].writeMicroseconds(1300);
      servo[2].writeMicroseconds(1300);
    }
    else if(velocities[2]==1)
    {
      servo[0].writeMicroseconds(1700);
      servo[1].writeMicroseconds(1700);
      servo[2].writeMicroseconds(1700);
    }
    else (velocities[2]==0)
    {
      servo[0].writeMicroseconds(1500);
      servo[1].writeMicroseconds(1500);
      servo[2].writeMicroseconds(1500);
    }
    
    //TO DO IN FUTURE: write a function for mapping the velocity from ( -1 to 1)->(1300 to 1700) using arduino's map() function;
    
    Serial1.println("hello from nav");
  }
}
