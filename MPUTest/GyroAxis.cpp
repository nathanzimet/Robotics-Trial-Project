#include "Arduino.h"
#include "GyroAxis.h"

GyroAxis::GyroAxis() {}

void GyroAxis::update_samples() {
  sum -= samples[index];      // remove oldest from sum
  scaled_sum -= (float)samples[index] * SCALE_FACTOR;
  samples[index] = raw;       // update arr with newest
  sum += samples[index];      // add newest to sum
  scaled_sum += (float)samples[index] * SCALE_FACTOR;
  index++;                    // increment index_z
  if (index == SAMPLE_SIZE)
    index = 0;
  mean = sum / SAMPLE_SIZE;   // recalc mean
  scaled_mean = scaled_sum / (float)SAMPLE_SIZE;
}

void GyroAxis::print_info() {

}