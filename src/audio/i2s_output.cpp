#include "i2s_output.h"

i2s_config_t i2s_config_output = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_TX),  
    .sample_rate = SAMPLE_RATE,            
    .bits_per_sample = BITS_PER_SAMPLE,  
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,  
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,  
    .intr_alloc_flags = 0,               
    .dma_buf_count = BUF_COUNT,                    
    .dma_buf_len = BUF_LEN,    
};


i2s_pin_config_t i2s_pin_config = {
    .bck_io_num = BCLK_PIN,  
    .ws_io_num = WS_PIN,   
    .data_out_num = I2S_PIN_NO_CHANGE, 
    .data_in_num = DIN_PIN
};

void write(int16_t *pcm_data, size_t data_len) {
    size_t bytes_written;
    // Scrivi i dati PCM nel buffer I2S per la trasmissione
    i2s_write(I2S_NUM, pcm_data, data_len * sizeof(int16_t), &bytes_written, portMAX_DELAY);
}

// Inizializzazione dell'I2S
void init_i2s() {
    esp_err_t result;
    // Inizializza l'I2S driver per trasmettere
    ESP_ERROR_CHECK(result = i2s_driver_install(I2S_NUM, &i2s_config_output, 0, NULL));
    ESP_ERROR_CHECK(result = i2s_set_pin(I2S_NUM, &i2s_pin_config));  // Configura i pin
    
    if(result != -1){
       Serial.println("I2S configurato correttamente!");
    } else {
       Serial.println("Errore durante la configurazione di I2S OUTPUT");
    }
}
