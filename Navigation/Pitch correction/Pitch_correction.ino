//towards router pitch positive and lead screw LOW
int PUL=6; //define Pulse pin
int DIR=7; //define Direction pin
int ENA=5; //define Enable Pin
void setup() {
  // put your setup code here, to run once:
 
  pinMode (PUL, OUTPUT);
  pinMode (DIR, OUTPUT);
  pinMode (ENA, OUTPUT);
}

void loop() {
  int reqpitch = 0;
  int currentpitch, newcp, temp;
  error = reqpitch - currentpitch;
  int eps=5;

  while (1)
  {
   if(abs(error)<=eps) //If within limit, do nothing
  {
    for (int i=0; i<6400; i++)    //Forward 5000 steps
      digitalWrite(ENA,LOW);
  }

  else  //error out of eps range
  {
     if (error < 0) //cp positive
     {
      while (!(abs(error)<=eps))
      {
        for (int i=0; i<6400; i++)   //Backward 5000 steps
        {
        digitalWrite(DIR,LOW);
        digitalWrite(ENA,HIGH);
        digitalWrite(PUL,HIGH);
        delayMicroseconds(18);
        digitalWrite(PUL,LOW);
        delayMicroseconds(18);
        }
     }
   }
     if (error > 0) //cp negative
     {
      while (!(abs(error)<=eps))
      {
        for (int i=0; i<6400; i++)   //Backward 5000 steps
        {
        digitalWrite(DIR,HIGH);
        digitalWrite(ENA,HIGH);
        digitalWrite(PUL,HIGH);
        delayMicroseconds(18);
        digitalWrite(PUL,LOW);
        delayMicroseconds(18);
        }
     }
     }
  }
  
  }
}
