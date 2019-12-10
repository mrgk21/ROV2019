String data = "", prevData = "";
int _6dof[6], _6dof_prev[6];
int _4dof[5], _4dof_prev[5];
const char* ArmFormat = "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d";

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
  Serial1.setTimeout(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial1.available()) {
    data = Serial1.readString();
    Serial1.flush();
    sscanf(&data[0], ArmFormat, &_4dof[0], &_4dof[1], &_4dof[2], &_4dof[3], &_4dof[4], &_6dof[0], &_6dof[1], &_6dof[2], &_6dof[3], &_6dof[4], &_6dof[5]);
  }

  if(!prevData.equals(data))
  {
    Serial1.println(_4dof[0]);
    Serial1.flush();
  }
  prevData = data;
}
