#include "adc_sampler.h"
#include <Arduino.h>


i2s_config_t adc_config_i2s = {
        .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
        .sample_rate =  SAMPLE_RATE,
        .bits_per_sample = BITS_PER_SAMPLE_I2S,
        .channel_format = CHANNEL_FORMAT,
	    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
	    .intr_alloc_flags = 0,
	    .dma_buf_count = ADC_BUF_COUNT,
	    .dma_buf_len = ADC_BUF_LEN,
};


void init_adc()
{ 
     esp_err_t result;
	 //install and start i2s driver
	 ESP_ERROR_CHECK(result = i2s_driver_install(I2S_ADC_NUM, &adc_config_i2s, 0, NULL));
	 //init ADC pad
	 ESP_ERROR_CHECK(result = i2s_set_adc_mode(ADC_UNIT, ADC_CHANNEL));
     ESP_ERROR_CHECK(result = i2s_adc_enable(I2S_ADC_NUM));
     //Configure ADC channel
     ESP_ERROR_CHECK(result = adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN));
     ESP_ERROR_CHECK(result = adc1_config_width(BITS_PER_SAMPLE_ADC));

     if(result == -1){
        Serial.println("Qualcosa è andato storto durante l'installazione dell' ADC");
     } else {
        Serial.println("ADC installato correttamente!");
     }

}

void read_raw_from_adc(uint8_t *adc_buffer, size_t adc_buffer_len){
    size_t bytes_read;
    i2s_read(I2S_ADC_NUM, (void*)adc_buffer, adc_buffer_len * sizeof(uint8_t), &bytes_read, portMAX_DELAY);
}

int16_t convert_voltage_to_pcm(float voltage) {
    // Centra la tensione rispetto a VMAX/2 per ottenere valori positivi e negativi
    float centered_voltage = voltage - (VMAX / 2);

    // Mappa il valore centrato a PCM 16 bit (-32768 a 32767)
    int16_t pcm_value = (int16_t)((centered_voltage / (VMAX / 2)) * PCM_MAX_VALUE);

    // Limita ai valori PCM massimo e minimo
    if (pcm_value > PCM_MAX_VALUE) pcm_value = PCM_MAX_VALUE;
    if (pcm_value < PCM_MIN_VALUE) pcm_value = PCM_MIN_VALUE;
    return pcm_value;
}

float convert_raw_to_voltage(uint16_t Dout){
    return ((Dout * VMAX) / DMAX); 
}

void from_raw_to_pcm(uint8_t *adc_buffer, size_t adc_buffer_len, int16_t *pcm_buffer){
    //IMPORTANTE ADC_BUFFER_LEN == PCM_BUFFER_LEN

    // Converti i dati grezzi a 12 bit e riempi il buffer PCM
    for (size_t i = 0; i < adc_buffer_len; i++) {
        // Ricostruzione del campione a 12 bit (2 byte)
        uint16_t Dout = ((adc_buffer[i * 2 + 1] << 8) | adc_buffer[i * 2]) & 0xFFF;

        // Converti il valore grezzo in tensione
        float voltage = convert_raw_to_voltage(Dout);

        // Converti la tensione in valore PCM
        pcm_buffer[i] = convert_voltage_to_pcm(voltage) / VOLUME_DIVIDER;
    }
}

void read(int16_t *pcm_buffer, size_t BUFFER_LEN){
  uint8_t adc_buffer[BUFFER_LEN];
  read_raw_from_adc(adc_buffer, BUFFER_LEN);
  from_raw_to_pcm(adc_buffer, BUFFER_LEN, pcm_buffer);
}

// Funzione per il debug dei dati PCM
void preview_pcm(int16_t *pcm_buffer, size_t buffer_len, size_t first_n_elements) {
    Serial.println("PCM DATA:");
    size_t num_elements = (first_n_elements < buffer_len) ? first_n_elements : buffer_len;
    for (size_t i = 0; i < num_elements; i++) {
        Serial.printf("PCM[%d] = %hd\n", i, pcm_buffer[i]);
    }
    Serial.println("...\n");
}