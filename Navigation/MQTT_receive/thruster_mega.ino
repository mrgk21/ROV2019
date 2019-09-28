
String data = "";
String prevData = "";
int thr[3];
char* thrFormat = "%d, %d, %d";

byte thr1 = 3;
byte thr2 = 4;
byte thr3 = 5;
byte thr4 = 6;
byte thr5 = 7;
byte thr6 = 8;

void pwm(int);

void setup() {
  Serial1.begin(9600);
  Serial1.setTimeout(20);
  Serial.begin(9600);
  Serial.setTimeout(100);
  pinMode(thr1, OUTPUT);
  pinMode(thr2, OUTPUT);
  pinMode(thr3, OUTPUT);
  pinMode(thr4, OUTPUT);
  pinMode(thr5, OUTPUT);
  pinMode(thr6, OUTPUT);

}

void loop() {
  if (Serial1.available()) {
    data = Serial1.readString();
    Serial1.flush();
    sscanf(&data[0], thrFormat, &thr[0], &thr[1], &thr[2]);
  }
//  pwm(1700);
    switch (thr[1]) {
      case 1:
        pwm(1700);
        break;
      case 0:
        pwm(1500);
        break;
      case -1:
        pwm(1300);
        break;
    }
  prevData = data;
}

void pwm(int width) {
  if (!prevData.equals(data)) {
    Serial.println(data);
    Serial.flush();
    Serial1.println("hello from nav");
  }
}
