/*
 * The purpose of this code is to simulate the process of MPU6050 being initialised. This has caused problems for thecommunication link.
 * Now it is ensured that the MPU is initialised completely before any data is sent/received over the communication link.
*/
String data = "";
int count;
bool MPU_init = false;
bool once = false;
void setup() {
  //Initialisze the 
  Serial1.begin(9600);
  Serial1.setTimeout(20);
  Serial.begin(9600);
  count = 0;
}

void loop() {
  if (Serial1.available())
  {
    data = Serial1.readString();
    Serial1.flush();
    Serial.println(data);
    //This message is coming from the nanopi. Arduino acknowledges it.
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
  //This must not repeat more than once.
  if(MPU_init && !once)
  {
    Serial1.println("HANDSHAKE SUCCESSFUL");
    Serial1.flush();
    once = true;
  }
}
