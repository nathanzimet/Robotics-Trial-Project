#include <Servo.h>

Servo myservo;
int pos = 0;
int current_time = 0;

void setup() {
  Serial.begin(9600);
  myservo.attach(9);
}

void loop() {
  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    log_info();
    delay(15);
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    log_info();
    delay(15);
  }

}

void log_info() {
  current_time = millis();
  Serial.print("At ");
  Serial.print(current_time);
  Serial.print(" position is: ");
  Serial.println(pos);
}
