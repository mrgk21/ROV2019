int data;

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  if(Serial.available()) {
    while(Serial.available()) {
      data = Serial.read();
      if(char(data) == '1') {
        digitalWrite(13, HIGH)	;
      } else {
        digitalWrite(13, LOW);
      }
    }
  }
}
