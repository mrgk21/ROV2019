const int S = A0;
int value;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.print(" analog values for water level up to 4cm");
}

void loop() {
  // put your main code here, to run repeatedly:
   value = analogRead(S);
   //Serial.println(value);
   if(value == 0)
   {
    Serial.println("fully dry");
   }
   else if(value <= 114 )
   {
    Serial.println("not to worry, less than 1 cm");
   }
   else if(value >=325 && value <= 345  )
   {
    Serial.println("water till 1 cm");
   }
   else if(value >=360 && value < 380 )
   {
    Serial.println("water till 2 cm");
   }
    else if(value > 380)
   {
    Serial.println("there is a problem now,more than 3 cm");
   }
   
}
