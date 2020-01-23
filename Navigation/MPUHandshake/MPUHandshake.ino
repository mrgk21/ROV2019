String data = "";
int count;
bool MPU_init = false;
bool once = false;
void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial1.setTimeout(20);
  Serial.begin(9600);
  count = 0;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available())
  {
    data = Serial1.readString();
    Serial1.flush();
    Serial.println(data);
    if (data == "is MPU init?")
    {
      if (count <= 5 && !MPU_init)
      {
        count ++;
        Serial.println(count);
      }
      else
      {
        MPU_init = true;
      }
    }
  }

  if(MPU_init && !once)
  {
    Serial1.println("HANDSHAKE SUCCESSFUL");
    Serial1.flush();
    once = true;
  }
}
