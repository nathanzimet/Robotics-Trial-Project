#include <Wire.h>

int MPU_addr = 0x68;
uint8_t WHO_AM_I = 0;

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
  // 
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x75);
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_addr, 1, true);
  WHO_AM_I = Wire.read();

  Serial.print("Returned address: ");
  Serial.println(WHO_AM_I);

  delay(5000);

}
