#include <Wire.h>

#define SAMPLE_SIZE 128
int printcounter = 0;

int MPU_addr = 0x68;

int16_t gyr_x;
int16_t gyr_y;
int16_t gyr_z;

// Offset is mean from first 128 samples
int16_t off_gyr_x;
int16_t off_gyr_y;
int16_t off_gyr_z;

int index_z = 0;
int16_t samples_z[SAMPLE_SIZE];
int32_t sum_z = 0;
int16_t mean_z;

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
  
  read_high_low(gyr_x);
  read_high_low(gyr_y);
  read_high_low(gyr_z);

  if (printcounter % 100 == 0) print_gyr();
  update_samples_z();

  delay(10);
}

void calibrate() {
  for (int i = 0; i < SAMPLE_SIZE; i++) {
    // Normal register read
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x43); 
    Wire.endTransmission(false);
    Wire.requestFrom(MPU_addr, 6, true);
  
    read_high_low(gyr_x);
    read_high_low(gyr_y);
    read_high_low(gyr_z);

    // Populate sample tables, create sum
    samples_z[i] = gyr_z;
    sum_z += gyr_z;
    
    delay(4);
  }
  mean_z = sum_z / SAMPLE_SIZE;
  off_gyr_z = mean_z;
}

void read_high_low(int16_t &var) {
  uint16_t highbits = 0;
  uint16_t lowbits = 0;

  highbits = (uint16_t)Wire.read() << 8;
  lowbits = (uint16_t)Wire.read();
  var = highbits + lowbits;
}

void update_samples_z() {
  sum_z -= samples_z[index_z];  // remove oldest from sum
  samples_z[index_z] = gyr_z;   // update arr with newest
  sum_z += samples_z[index_z];  // add newest to sum
  index_z++;                    // increment index_z
  if (index_z == SAMPLE_SIZE)
    index_z = 0;
  mean_z = sum_z / SAMPLE_SIZE; // recalc mean
}

void print_gyr() {
  Serial.print("index_z: ");
  Serial.print(index_z);
  Serial.print(", sum: ");
  Serial.print(sum_z);
  Serial.print(", last: ");
  Serial.print(samples_z[index_z]);
  Serial.print(", mean: ");
  Serial.println(mean_z);

}
