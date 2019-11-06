/************* CLASSES ************/
class pdcv
{
    static int delay_ms;
    int pin1;
    int pin2;
    bool reverseDir = false;
    bool OutputNegative;
    float Output;
  public:
    pdcv(int p1, int p2, bool reverse);
    void pdcv_setup();
    void pdcv_forward(bool backToStatic=true);
    void pdcv_backward(bool backToStatic=true);
    void pdcv_static();
    void pdcv_setSpeed(double speed);
    static void setDelay(int);
};
int pdcv::delay_ms = 0;

void pdcv::setDelay(int delay)
{
  pdcv::delay_ms = delay;
}

pdcv::pdcv(int p1, int p2, bool reverse)
{
  this->pin1 = p1;
  this->pin2 = p2;
  this->reverseDir = reverse;
}

void pdcv::pdcv_setup()
{
  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
}

void pdcv::pdcv_forward(bool backToStatic)
{
  if (reverseDir)
  {
    analogWrite(pin2, OutputNegative ? abs(Output) : Output);
    analogWrite(pin1, 50);
  }
  else
  {
    analogWrite(pin1, OutputNegative ? abs(Output) : Output);
    analogWrite(pin2, 50);
  }

  if (backToStatic)
  {
    delay(pdcv::delay_ms);
    this->pdcv_static();
  }
}

void pdcv::pdcv_backward(bool backToStatic)
{
  if (reverseDir)
  {
    analogWrite(pin1, OutputNegative ? abs(Output) : Output);
    analogWrite(pin2, 50);
  }
  else
  {
    analogWrite(pin2, OutputNegative ? abs(Output) : Output);
    analogWrite(pin1, 50);
  }
  if (backToStatic)
  {
    delay(pdcv::delay_ms);
    this->pdcv_static();
  }
}

void pdcv::pdcv_static()
{
  digitalWrite(pin1, LOW);
  digitalWrite(pin2, LOW);
}

void pdcv::pdcv_setSpeed(double speed)
{
  this->Output = speed;
}

/****************** VARIABLES *****************/

String data = "";
String prevData = "";
float _4dof[4];
float _6dof[6];
char* ArmFormat = "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d";

void ArmMove();
unsigned char GetArmSpeed(float joyRead);

pdcv p_6dof[] = {pdcv(8, 9, false), pdcv(4, 5, false), pdcv(12, 13 , false), pdcv(2, 3, false), pdcv(6, 7, false)};
void setup() {
  for (int i = 0; i < 5; i++)
  {
    p_6dof[i].pdcv_setup();
  }
  pdcv::setDelay(500);
  Serial1.begin(9600);
  Serial1.setTimeout(20);
  Serial.begin(9600);
  Serial.setTimeout(100);
}

void loop() {
  if (Serial1.available()) {
    data = Serial1.readString();
    Serial1.flush();
    sscanf(&data[0], ArmFormat, &_4dof[0], &_4dof[1], &_4dof[2], &_4dof[3], &_6dof[0], &_6dof[1], &_6dof[2], &_6dof[3], &_6dof[4], &_6dof[5]);
  }
  ArmMove();
  prevData = data;
}

void ArmMove() {
  if (!prevData.equals(data)) {
    Serial.println(data);
    Serial.flush();
    for (int i = 0; i < 5; i++)
    {
      p_6dof[i].pdcv_setSpeed(_6dof[i]);
      if (_6dof[i] > 0)
      {
        p_6dof[i].pdcv_forward(true);
      }
      else
      {
        p_6dof[i].pdcv_backward(true);
      }
    }

    Serial1.println("hello from arm");
    Serial1.flush();
  }
}
