#include <Wire.h>
#include "GyroAxis.h"

#define TICK_RATE 10

int printcounter = 0;
int MPU_addr = 0x68;

GyroAxis gyr_x;
GyroAxis gyr_y;
GyroAxis gyr_z;

void setup() {
  Serial.begin(9600);
  Wire.begin(MPU_addr);
  
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
  calibrate();

}

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

  if (printcounter % (TICK_RATE * 5) == 0) print_gyr();
  gyr_x.update_samples();
  gyr_y.update_samples();
  gyr_z.update_samples();

  delay(TICK_RATE);
}

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
    
    delay(4);
  }
  gyr_x.mean = gyr_x.offset = gyr_x.sum / SAMPLE_SIZE;
  gyr_y.mean = gyr_y.offset = gyr_y.sum / SAMPLE_SIZE;
  gyr_z.mean = gyr_z.offset = gyr_z.sum / SAMPLE_SIZE;
}

void read_high_low(int16_t &var) {
  uint16_t highbits = 0;
  uint16_t lowbits = 0;

  highbits = (uint16_t)Wire.read() << 8;
  lowbits = (uint16_t)Wire.read();
  var = highbits + lowbits;
}

void print_gyr() {
  Serial.print("Raw sample rolling mean: (x=");
  Serial.print(gyr_x.mean - gyr_x.offset);
  Serial.print(", y=");
  Serial.print(gyr_y.mean - gyr_y.offset);
  Serial.print(", z=");
  Serial.print(gyr_z.mean - gyr_z.offset);
  Serial.println(")");
}
