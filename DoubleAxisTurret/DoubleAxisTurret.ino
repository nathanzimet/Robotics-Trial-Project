/*  Double Axis Laser Turret for Upper div Robotics Trial Project
    Only use Servo and Wire libraries

    Main sources:
    https://docs.arduino.cc/learn/electronics/servo-motors/ 
    https://docs.arduino.cc/language-reference/en/functions/communication/wire/
    https://controllerstech.com/mpu6050-arduino-tutorial/
    MPU6000/6050 Register Map and Descriptions, Revision 4.0
*/

#include <Servo.h>
#include <Wire.h>
#include "GyroAxis.h"

#define TICK_RATE 2
#define SAMPLE_RATE 6
int printcounter = 0;

// Servo variables
Servo servo1;   // pan servo
Servo servo2;   // tilt servo
int pos_s1 = 90;
int pos_s2 = 90;
int hidden_pos_s1 = 90; // continue tracking when out of bounds
int hidden_pos_s2 = 90;

// IMU variables
int MPU_addr = 0x68;

GyroAxis gyr_x;
GyroAxis gyr_y;
GyroAxis gyr_z;

// Other variables (temp stuff)
int temp_pos_counter = 0;
int increment = 1;

//-------------------------- Setup --------------------------
void setup() {
  Serial.begin(9600);
  Wire.begin(MPU_addr);

  // Attach servos to pins
  servo1.attach(3);   // pin D3
  servo2.attach(4);   // pin D4

  // set servo initial positions to 90
  servo1.write(pos_s1);
  servo2.write(pos_s2);

  // Wake up MPU
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);   //PWR_MGMT_1
  Wire.write(0x00);   //wake up from sleep
  Wire.endTransmission(true);

  // Set gyro to output
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x1B);   //GYR_CONFIG
  Wire.write(0x00);   //+=250 degrees per second
  Wire.endTransmission(true);

  // get first 128 samples for initial sum and mean
  // also waits for servos to reach initial position
  calibrate();

}

//-------------------------- Loop ---------------------------
void loop() {
  printcounter++;

  // Gyroscope reading
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x43);   // reg [43:48] 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 6, true);
  
  read_high_low(gyr_x.raw);
  read_high_low(gyr_y.raw);
  read_high_low(gyr_z.raw);

  if (printcounter % (TICK_RATE * 5 * SAMPLE_RATE) == 0) print_gyr_raw();
  // skipped std_dev

  gyr_x.update_samples();
  gyr_y.update_samples();
  gyr_z.update_samples();
  
  // Update servo position
  if (printcounter % (TICK_RATE * SAMPLE_RATE) == 0) {
    get_s1_pos();  // probably incorrect method of calculating angles
    get_s2_pos();
  }

  //if (printcounter % (TICK_RATE * SAMPLE_RATE) == 0) print_gyr();

  servo1.write(pos_s1);
  servo2.write(pos_s2);

  delay(TICK_RATE);

}
//------------------------- Methods --------------------------
/*  calibrate
    Spends ~0.5 seconds filling the 3 axis with initial data
    used to make offset to ignore background noise, mean, and sum
*/
void calibrate() {
  for (int i = 0; i < SAMPLE_SIZE; i++) {
    // Normal register read
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x43); 
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 6, true);
  
    read_high_low(gyr_x.raw);
    read_high_low(gyr_y.raw);
    read_high_low(gyr_z.raw);

    // Populate sample tables, create sum
    gyr_x.samples[i] = gyr_x.raw;
    gyr_y.samples[i] = gyr_y.raw;
    gyr_z.samples[i] = gyr_z.raw;
    gyr_x.sum += gyr_x.raw;
    gyr_y.sum += gyr_y.raw;
    gyr_z.sum += gyr_z.raw;
    
    delay(TICK_RATE);
  }
  gyr_x.mean = gyr_x.offset = gyr_x.sum / SAMPLE_SIZE;
  gyr_y.mean = gyr_y.offset = gyr_y.sum / SAMPLE_SIZE;
  gyr_z.mean = gyr_z.offset = gyr_z.sum / SAMPLE_SIZE;
}

/*  read_high_low
    Takes reference to gyro raw axis variable,
    performs 2 Wire reads to fill it

    Inputs: Reference to int16_t
    Returns: None
*/
void read_high_low(int16_t &var) {
  uint16_t highbits = 0;
  uint16_t lowbits = 0;

  highbits = (uint16_t)Wire.read() << 8;
  lowbits = (uint16_t)Wire.read();
  var = highbits + lowbits;
}

/*  get_s1_pos()
    Calculate servo position from gyro data and old position
    gyro gives angular velocity in degrees per second
    use xf = xi + vt

    Inputs: None
    Output: New servo position bound to [0, 180]
    Precondition: Servos resting state is at 90 degrees
*/
void get_s1_pos() {
  float temp = gyr_z.scaled_mean * ((float)TICK_RATE * SAMPLE_RATE) / 100;
  hidden_pos_s1 = hidden_pos_s1 + (int)temp;
  if (hidden_pos_s1 < 0) pos_s1 = 0;
  else if (hidden_pos_s1 > 180) pos_s1 = 180;
  else pos_s1 = hidden_pos_s1;
}

/*  get_s2_pos()
    Same as servo 1 but uses x axis of rotation
*/
void get_s2_pos() {
  float temp = gyr_x.scaled_mean * ((float)TICK_RATE * SAMPLE_RATE) / 100;
  hidden_pos_s2 = hidden_pos_s2 + (int)temp;
  if (hidden_pos_s2 < 0) pos_s2 = 0;
  else if (hidden_pos_s2 > 180) pos_s2 = 180;
  else pos_s2 = hidden_pos_s2;
}

/*  print_gyr_raw
    print (raw - offset) for 3 axis as required by spec doc
*/
void print_gyr_raw() {
  Serial.print("Raw sample rolling mean: (x=");
  Serial.print(gyr_x.mean - gyr_x.offset);
  Serial.print(", y=");
  Serial.print(gyr_y.mean - gyr_y.offset);
  Serial.print(", z=");
  Serial.print(gyr_z.mean - gyr_z.offset);
  Serial.println(")");
}

/*  print_gyr
    print whatever gyr axis obj says is its info 
*/
void print_gyr() {
  Serial.print("(x=");
  gyr_x.print_info();
  Serial.print(", y=");
  gyr_y.print_info();
  Serial.print(", z=");
  gyr_z.print_info();
  Serial.println(")");
}

void print_servo_pos() {
  Serial.print("pos_s1: ");
  Serial.print(pos_s1);
  Serial.print(", pos_s2: ");
  Serial.println(pos_s2);
}
