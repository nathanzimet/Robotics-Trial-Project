#include <Wire.h>

int MPU_addr = 0x68;

int16_t acc_x;
int16_t acc_y;
int16_t acc_z;
int16_t gyr_x;
int16_t gyr_y;
int16_t gyr_z;

void setup() {
  Serial.begin(9600);
  Wire.begin(MPU_addr);
  
  // Wake up MPU
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);   //PWR_MGMT_1
  Wire.write(0x00);   //wake up from sleep
  Wire.endTransmission(true);

}

void loop() {
  // Accelerometer reading
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);   // reg [3B:40] 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 6, true);
  
  read_high_low(acc_x);
  read_high_low(acc_y);
  read_high_low(acc_z);

  // Gyroscope reading
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x43);   // reg [43:48] 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 6, true);
  
  read_high_low(gyr_x);
  read_high_low(gyr_y);
  read_high_low(gyr_z);

  print_acc();
  print_gyr();
  Serial.println("--------");

  delay(1000);

}

void read_high_low(int16_t &var) {
  uint16_t highbits = 0;
  uint16_t lowbits = 0;

  highbits = (uint16_t)Wire.read() << 8;
  lowbits = (uint16_t)Wire.read();
  var = highbits + lowbits;
}

void print_acc() {
  Serial.print("ACC (x, y, z): ");
  Serial.print(acc_x);
  Serial.print(",  ");
  Serial.print(acc_y);
  Serial.print(", ");
  Serial.println(acc_z);
}

void print_gyr() {
  Serial.print("GYR (x, y, z): ");
  Serial.print(gyr_x);
  Serial.print(",  ");
  Serial.print(gyr_y);
  Serial.print(", ");
  Serial.println(gyr_z);
}
