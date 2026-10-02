/*  Double Axis Laser Turret for Upper div Robotics Trial Project
    Only use Servo and Wire libraries
*/

#include <Servo.h>

// Servo variables
Servo servo1;   // pan servo
Servo servo2;   // tilt servo
int pos_s1 = 0;
int pos_s2 = 0;

// To do: IMU variables
// for example 128 var array for sliding window

// Other variables (temp stuff)
int temp_pos_counter = 0;
int increment = 1;

//-------------------------- Setup --------------------------
void setup() {
  Serial.begin(9600);

  // Attach servos to pins
  servo1.attach(3);   // pin D3
  servo2.attach(4);   // pin D4

  // To do: IMU setup
}
//-------------------------- Loop ---------------------------
void loop() {
  // Temporary generator for temp_pos_counter
  if (temp_pos_counter <= 180 && temp_pos_counter >= 0) {
    temp_pos_counter += increment;
  }
  else {
    increment *= -1;
    temp_pos_counter += increment;
  }

  // To do: Get register data from IMU
  
  // Update servo position
  servo1.write(get_s1_pos());
  servo2.write(get_s2_pos());

  delay(20);

}
//------------------------- Methods --------------------------
/*  Parameters: none yet
    Returns: int pos for servo 1
*/
int get_s1_pos() {
  return temp_pos_counter;
}

/*  Parameters: none yet
    Returns: int pos for servo 2
*/
int get_s2_pos() {
  return temp_pos_counter;
}

