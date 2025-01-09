#include <Arduino.h>
#include "audio/audio.h"

#define TEST_BUFFER 32

void setup() {
   Serial.begin(115200);
   init_adc();
   init_i2s();
}

void loop() {
   
   int16_t pcm_buffer[TEST_BUFFER];
   read(pcm_buffer, TEST_BUFFER);
   preview_pcm(pcm_buffer, TEST_BUFFER, 10);
   delay(1000);
   //write(pcm_buffer,TEST_BUFFER);

}

