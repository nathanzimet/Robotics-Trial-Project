#include "Arduino.h"
#include "GyroAxis.h"

GyroAxis::GyroAxis() {}

void GyroAxis::update_samples() {
  sum -= samples[index];      // remove oldest from sum
  samples[index] = raw;       // update arr with newest
  sum += samples[index];      // add newest to sum
  index++;                    // increment index_z
  if (index == SAMPLE_SIZE)
    index = 0;
  mean = sum / SAMPLE_SIZE;   // recalc mean
  scaled_sum = (float)sum * SCALE_FACTOR;
  scaled_mean = scaled_sum / (float)SAMPLE_SIZE;
  scaled_mean -= (float)offset * SCALE_FACTOR;
}

void GyroAxis::print_info() {
  Serial.print(scaled_mean, 4);
}