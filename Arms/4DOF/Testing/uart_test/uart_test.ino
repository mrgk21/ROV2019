String x = "";
int link[4];
void setup() {
  Serial1.begin(9600);
  Serial.begin(115200);
  Serial1.setTimeout(20);
  pinMode(13, OUTPUT);
  Serial.println("size | link");
}

void loop() {
  if (Serial1.available()) {
    x = Serial1.readString();
    int size = sscanf(&x[0], "%d, %d, %d, %d",  &link[0], &link[1], &link[2], &link[3]);
    Serial1.flush();
    if (x.equals("a")) {
      digitalWrite(13, HIGH);
    } else if (x.equals("b")) {
      digitalWrite(13, LOW);
    }
    Serial.print(size);
    Serial.print(" | ");
    Serial.println(link[0] + link[1] + link[2] + link[3]);
  }
}
