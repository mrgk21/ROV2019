#include <stdio.h>

void setup() {
  Serial.begin(9600);
  char* data = "1 1 1 0 0 0 0 0 0 0 0 0 0 0 0 0 0";
  char* dataFormat = "%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d";
  int axis[3];
  int buttons[12];
  int hats[2];
  sscanf(data, dataFormat, &axis[0], &axis[1], &axis[2], &buttons[0], &buttons[1], &buttons[2], &buttons[3], &buttons[4], &buttons[5], &buttons[6], &buttons[7], &buttons[8], &buttons[9], &buttons[10], &buttons[11], &buttons[12], &hats[0], &hats[1]
        );
  String notData = (String) axis[0] + axis[1] + axis[2] + buttons[0] + buttons[1] + buttons[2] + buttons[3] + buttons[4]+ buttons[5] + buttons[6] + buttons[7] + buttons[8] + buttons[9] + buttons[10] + buttons[11] + buttons[12] + hats[0] + hats[1];
  Serial.println(notData);
}

void loop() {
    // put your main code here, to run repeatedly:

}
