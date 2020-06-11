/*
 * This code is used to test whether the joystick data is recieved correctly by Arduino Mega.
*/

String data = "", prevData = "";
int _6dof[6], _6dof_prev[6];
int _4dof[5], _4dof_prev[5];

//Frame Format in which data is sent by NanoPi M4
const char* ArmFormat = "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d";

void setup() {
  // Initialise UART
  Serial1.begin(9600);
  Serial1.setTimeout(10);
}

void loop() {
  // Check if there's any data on the serial port
  if (Serial1.available()) {
    data = Serial1.readString();
    Serial1.flush();
    sscanf(&data[0], ArmFormat, &_4dof[0], &_4dof[1], &_4dof[2], &_4dof[3], &_4dof[4], &_6dof[0], &_6dof[1], &_6dof[2], &_6dof[3], &_6dof[4], &_6dof[5]);
  }
  //if the data has changed from the previous data do the following actions.
  if(!prevData.equals(data))
  {
    Serial1.println(_4dof[0]);
    Serial1.flush();
  }
  prevData = data;
}
