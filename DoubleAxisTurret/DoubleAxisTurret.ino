#include <Servo.h>

Servo servo1;   // pan servo
Servo servo2;   // tilt servo
int pos_s1 = 0;
int pos_s2 = 0;
int temp_pos_counter = 0;
int increment = 1;

void setup() {
  Serial.begin(9600);
  servo1.attach(3);   // pin D3
  servo2.attach(4);   // pin D4
}

void loop() {
  if (temp_pos_counter <= 180 && temp_pos_counter >= 0) {
    temp_pos_counter += increment;
  }
  else {
    increment *= -1;
    temp_pos_counter += increment;
  }
  
  servo1.write(get_s1_pos());
  servo2.write(get_s2_pos());

  delay(20);

}

int get_s1_pos() {
  return temp_pos_counter;
}

int get_s2_pos() {
  return temp_pos_counter;
}

