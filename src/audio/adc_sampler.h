#include "driver/adc.h"
#include "driver/i2s.h"


#define SAMPLE_RATE 44100
#define BITS_PER_SAMPLE_I2S I2S_BITS_PER_SAMPLE_16BIT
#define BITS_PER_SAMPLE_ADC ADC_WIDTH_BIT_12
#define CHANNEL_FORMAT I2S_CHANNEL_FMT_RIGHT_LEFT
#define I2S_ADC_NUM I2S_NUM_0
#define ADC_CHANNEL ADC1_CHANNEL_7
#define ADC_UNIT ADC_UNIT_1
#define ADC_BUF_COUNT 2
#define ADC_BUF_LEN 1024
#define ADC_ATTEN ADC_ATTEN_DB_12
#define VOLUME_DIVIDER 10

#define VMAX 3.9f         // Tensione massima che l'ADC può misurare (es. 3.9V) --> dipende da ATTEN
#define DMAX 4095          // Valore massimo per un ADC a 12 bit (0-4095) --> dipende da DC_BITWIDTH
#define PCM_MAX_VALUE 32767  // Massimo valore per PCM a 16 bit (signed) --> dipende dal formato i2s in output per il MAX98357
#define PCM_MIN_VALUE -32768 // Minimo valore per PCM a 16 bit (signed) --> dipende dal formato i2s in output per il MAX98357

void init_adc();
void read_raw_from_adc(uint8_t *adc_buffer, size_t adc_buffer_len);
int16_t convert_voltage_to_pcm(float voltage);
float convert_raw_to_voltage(uint16_t Dout);
void from_raw_to_pcm(uint8_t *adc_buffer, size_t adc_buffer_len, int16_t *pcm_buffer);
void read(int16_t *pcm_buffer, size_t BUFFER_LEN);
void preview_pcm(int16_t *pcm_buffer, size_t buffer_len, size_t first_n_elements);