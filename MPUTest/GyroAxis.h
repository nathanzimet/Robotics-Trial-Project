#ifndef GYROAXIS_h
#define GYROAXIS_h

#define SAMPLE_SIZE (128)
#define SCALE_FACTOR (250.0/32768.0)

#include "Arduino.h"  // for using Serial class

class GyroAxis {
  private:
    int index;
  public:
    int16_t raw;
    int16_t offset;   // mean of very first sample
    int16_t samples[SAMPLE_SIZE];
    int32_t sum;
    int16_t mean;

    // recalculated from int sum, mean every tick to avoid fp error
    float scaled_sum;   
    float scaled_mean;

    GyroAxis();
    void update_samples();
    void print_info();

};

#endif //GYROAXIS_h