#define PDCV_PIN_0 27
#define PDCV_PIN_1 29
#define PDCV_ANALOG A0

#define TIME_THRESH 1000
int DELAY = 2000;
#define TOLERANCE 20
#define SAFETY 25
#define upLimit 311 + SAFETY
#define lowLimit 606 - SAFETY

//int REACH = 300;
int dir;
int startingPoint = 0;
int limit = (lowLimit - upLimit) / 20;
int currGoal = 0;
int itr = 1;

long prevTime = 0;
int vel = 128;

void ArmMove(bool forward)
{
  pinMode(PDCV_PIN_0, OUTPUT);
  pinMode(PDCV_PIN_1, OUTPUT);
  if (forward)
  {
    analogWrite(PDCV_PIN_0, vel);//<0?abs(armSpeed):armSpeed);
    digitalWrite(PDCV_PIN_1, LOW);
  }
  else
  {
    analogWrite(PDCV_PIN_1, vel);//<0?abs(armSpeed):armSpeed);
    digitalWrite(PDCV_PIN_0, LOW);
  }
}


void ArmStatic()
{
  /*
    digitalWrite(PDCV_PIN_0, LOW);//<0?abs(armSpeed):armSpeed);
    digitalWrite(PDCV_PIN_1, LOW);
  */
  
  pinMode(PDCV_PIN_0, INPUT);
  pinMode(PDCV_PIN_1, INPUT);
  
}

void setup() {
  // put your setup code here, to run once:
  pinMode(PDCV_PIN_0, OUTPUT);
  pinMode(PDCV_PIN_1, OUTPUT);
  Serial.begin(9600);

  pinMode(39,OUTPUT);
  pinMode(41,OUTPUT);
  digitalWrite(39,HIGH);
  digitalWrite(41,LOW);


  
  prevTime = millis();
  startingPoint = analogRead(PDCV_ANALOG);
  dir = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
//
//  if (Serial.available())
//  {
//    String s = Serial.readString();
//    Serial.flush();
//    DELAY = s.toInt();
//    //startingPoint = count;
//    itr = 1;
//  }
  int count = analogRead(PDCV_ANALOG);
  //Serial.println(count);
  //Serial.println(dir);
  if (dir == 1) { //downward direction
    ArmMove(true);
    if (count == startingPoint + itr * limit)
    {
      Serial.println("Detected STatic DOWN");
      ArmStatic();
      //delay(DELAY);
      itr++;
    }
    if(count >= lowLimit) {
      dir = 0; //if arm reaches the lower limit then change direction
      //delay(1000);
      startingPoint = lowLimit;
      itr = 1;
    }
  } else { //upward direction
    ArmMove(false);
   // Serial.print(startingPoint - itr * limit);
    if (count == startingPoint - itr * limit) {
      Serial.println("Detected STatic UP");
      ArmStatic();
      //delay(DELAY);
      itr++;
    }
    if(count <= upLimit) {
      dir = 1; //if arm reaches the lower limit then change direction
      //delay(1000);
      startingPoint = upLimit;
      itr = 1;
    }
  }
  /*
  if(count == 550) 
  {
    Serial.println("Velo zero");
    vel = 0;
  }
  */
//  if (abs(count - REACH) > TOLERANCE)
//  {
//    if (count < REACH)
//    {
//      if (count < startingPoint + itr * limit)
//      {
//        ArmMove(true);
//      }
//      else
//      {
//        ArmStatic();
//      }
//    }
//    else
//    {
//      if (count > startingPoint - itr * limit)
//      {
//        ArmMove(false);
//      }
//      else
//      {
//        ArmStatic();
//      }
//    }
//    delay(DELAY);
//    itr++;
//  }
}
