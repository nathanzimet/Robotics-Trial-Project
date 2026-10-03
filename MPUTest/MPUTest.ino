#include <Wire.h>

int MPU_addr = 0x68;

int16_t acc_x;
int16_t acc_y;
int16_t acc_z;

void setup() {
  Serial.begin(9600);
  Wire.begin(MPU_addr);
  
  // Wake up MPU
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B);   //PWR_MGMT_1
  Wire.write(0x00);   //wake up from sleep
  Wire.endTransmission(true);

  // Congif: sample rate to 1/8th output rate
  // Wire.beginTransmission(MPU_addr);
  // Wire.write(0x19);   //SMPLRT_DIV
  // Wire.write(0x07);   //set divisor to 7+1 = 8
  // Wire.endTransmission(true);

  // Congif: something filtering not sure what
  // Wire.beginTransmission(MPU_addr);
  // Wire.write(0x1A);   //SMPLRT_DIV
  // Wire.write(0x03);   //not sure what this does either
  // Wire.endTransmission(true);

  // Accelerometer congif: set error range 
  // Wire.beginTransmission(MPU_addr);
  // Wire.write(0x1C);   //ACCEL_CONFIG
  // Wire.write(0x00);   //set to += 2
  // Wire.endTransmission(true);

}

void loop() {
  // 
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x3B);   // [3B:40] 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 6, true);
  
  uint16_t highbits = 0;
  uint16_t lowbits = 0;

  highbits = (uint16_t)Wire.read() << 8;
  lowbits = (uint16_t)Wire.read();
  acc_x = highbits + lowbits;

  highbits = (uint16_t)Wire.read() << 8;
  lowbits = (uint16_t)Wire.read();
  acc_y = highbits + lowbits;

  highbits = (uint16_t)Wire.read() << 8;
  lowbits = (uint16_t)Wire.read();
  acc_z = highbits + lowbits;

  Serial.print("ACC (x, y, z): ");
  Serial.print(acc_x);
  Serial.print(",  ");
  Serial.print(acc_y);
  Serial.print(", ");
  Serial.println(acc_z);

  delay(1000);

}
