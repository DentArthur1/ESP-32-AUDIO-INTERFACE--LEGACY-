#include "driver/i2s.h"
#include <Arduino.h>

#define I2S_NUM I2S_NUM_1
#define SAMPLE_RATE 44100
#define BITS_PER_SAMPLE I2S_BITS_PER_SAMPLE_16BIT
#define BUF_COUNT 2
#define BUF_LEN 1024

#define BCLK_PIN 26
#define WS_PIN 25
#define DIN_PIN 22

void write(int16_t *pcm_data, size_t data_len);
void init_i2s();