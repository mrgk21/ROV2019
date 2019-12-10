/************* STRUCTURES************/
struct pdcv
{
  int pin1;
  int pin2;
  int reverseDir;
  int OutputNegative;
  float Output;
  bool canActuate;
  int prevTime;
};
struct pdcv p_6dof[] = {
  {12, 13, false},
  {10, 11, false},
  {8, 9, false},
  {4, 5, false},
  {2, 3, false},
};

struct pdcv p_4dof[] = {
  {6, 7, true},
  {27, 29, false},
  {30, 31, false},
  {39, 41, false}
};
int delay_ms = 500;

inline void pdcv_setup(struct pdcv *temp) {
  pinMode(temp->pin1, OUTPUT);
  pinMode(temp->pin2, OUTPUT);
  temp->canActuate = true;
}

inline void pdcv_setSpeed(struct pdcv *temp, float x) {
  temp->Output = x;
}

inline void pdcv_static(struct pdcv *temp) {
  digitalWrite(temp->pin1, LOW);
  digitalWrite(temp->pin2, LOW);
}

void pdcv_forward(struct pdcv *temp, bool backToStatic) {
  if (temp->canActuate)
  {
    if (temp->reverseDir) {
      analogWrite(temp->pin2, temp->Output<0 ? abs(temp->Output) : temp->Output);
      digitalWrite(temp->pin1, LOW);
    }
    else {
      analogWrite(temp->pin1, temp->Output<0 ? abs(temp->Output) : temp->Output);
      digitalWrite(temp->pin2, LOW);
    }
  }
  if (backToStatic) {
    if (temp->canActuate)
    {
      temp->prevTime = millis();
    }
    if (millis() - temp->prevTime < delay_ms)
    {
      temp->canActuate = false;
    }
    else
    {
      temp->canActuate = true;
      pdcv_static(temp);
    }
  }
}

void pdcv_backward(struct pdcv *temp, bool backToStatic) {
  if (temp->canActuate)
  {
    if (temp->reverseDir) {
      analogWrite(temp->pin1, temp->Output<0? abs(temp->Output) : temp->Output);
      digitalWrite(temp->pin2, LOW);
    }
    else {
      analogWrite(temp->pin2, temp->Output<0? abs(temp->Output) : temp->Output);
      digitalWrite(temp->pin1, LOW);
    }
  }
  if (backToStatic) {
    if (temp->canActuate)
    {
      temp->prevTime = millis();
    }
    if (millis() - temp->prevTime < delay_ms)
    {
      temp->canActuate = false;
    }
    else
    {
      temp->canActuate = true;
      pdcv_static(temp);
    }
  }
}
/********************* VARIABLES ****************************/
String data = "", prevData = "";
int _6dof[6], _6dof_prev[6];
int _4dof[5], _4dof_prev[5];
const char* ArmFormat = "%d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d";

/********************  FUNCTIONS  ******************************/
void ArmMove(int mode) {
  if (!prevData.equals(data)) {
    if (mode == 1) {
      for (int i = 0; i < 5; i++)
      {
        if (_6dof[i] != _6dof_prev[i] && abs(_6dof[i]) > 50)
        {
          pdcv_setSpeed(&p_6dof[i], _6dof[i] > 0 ? 255 : -255); //_6dof[i]);
          if (_6dof[i] > 0)
          {
            pdcv_forward(&p_6dof[i], false);
          }
          else
          {
            pdcv_backward(&p_6dof[i], false);
          }
        }
        else {
          pdcv_static(&p_6dof[i]);
        }
      }
    }
    else
    {
      for (int i = 0; i < 4; i++)
      {
        if (_4dof[i] != _4dof_prev[i] && abs(_4dof[i]) > 50)
        {
          pdcv_setSpeed(&p_4dof[i], _4dof[i]);
          if (_4dof[i] > 0)
          {
            pdcv_forward(&p_4dof[i], false);
          }
          else
          {
            pdcv_backward(&p_4dof[i], false);
          }
        }
        else {
          pdcv_static(&p_4dof[i]);
        }
      }
    }
    Serial1.println(_4dof[3]);
    Serial1.flush();

  }
}


#define sim_size 3
int ind[] = {1, 2, 3};
int speeds[] = {255,127,255};
void Arm_Simultaneous(int mode)
{
  if (!prevData.equals(data)) {
    for (int i = 0; i < sim_size; i++)
    {
      pdcv_setSpeed(&p_6dof[ind[i]], _6dof[0]>0?speeds[i]:-speeds[i]);
      if(abs(_6dof[0])>30)
      {
        if(_6dof[0]>0)
        {
          Serial1.println("motion-f");
          Serial1.flush();
          pdcv_forward(&p_6dof[ind[i]], false);
        }
        else
        {
          Serial1.println("motion-b");
          Serial1.flush();
          pdcv_backward(&p_6dof[ind[i]], false);
        }
      }
      else
      {
        pdcv_static(&p_6dof[ind[i]]);
      }
    }
    //Serial1.println(*mapped_input);
    //Serial1.flush();
  }
}

void setup() {
  for (int i = 0; i < 5; i++) {
    if (i < 4) {
      pdcv_setup(&p_4dof[i]);
    }
    pdcv_setup(&p_6dof[i]);
  }

  Serial1.begin(9600);
  Serial1.setTimeout(10);
}

void loop() {
  if (Serial1.available()) {
    data = Serial1.readString();
    Serial1.flush();
    sscanf(&data[0], ArmFormat, &_4dof[0], &_4dof[1], &_4dof[2], &_4dof[3], &_4dof[4], &_6dof[0], &_6dof[1], &_6dof[2], &_6dof[3], &_6dof[4], &_6dof[5]);
  }
  //ArmMove(1);
  ArmMove(0);
  /*
    if(!prevData.equals(data))
    {
    Serial1.println(_6dof[0]);
    Serial1.flush();
    }
  */
  //Arm_Simultaneous(1);
  prevData = data;
  for (int i = 0; i < 5; i++) {
    if (i < 4) {
      _4dof_prev[i] = _4dof[i];
    }
    _6dof_prev[i] = _6dof[i];
  }
}
