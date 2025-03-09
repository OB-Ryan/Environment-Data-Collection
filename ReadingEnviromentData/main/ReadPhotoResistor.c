// Ryan Root
// 04/08/2025
// Read Serial Data from Photo Resistor - GPIO 36

#include "ReadPhotoResistor.h"

void config_adc () {
    // Set the bit capture width
    esp_err_t config_width = adc1_config_width(ADC_BITWIDTH_12);
    if (config_width != ESP_OK) {
        printf("FATAL: Configuration of ADC Width has failed.\n");
        exit(-1);
    }

    // Set the attenuation level
    esp_err_t config_channel = adc1_config_channel_atten(CHANNEL, ADC_ATTEN_DB_12);
    if (config_channel != ESP_OK) {
        printf("FATAL: Configuration of ADC Attenuation has failed.\n");
        exit(-1);
    }
}

int read_analog () {
    return adc1_get_raw(CHANNEL);
}

void endless_read() {
    config_adc();
    printf("Starting Data Read:\n");

    TickType_t delay = TICK_DELAY;
    while (1) {
        // Continuously read data - wait 25 ticks between each read
        printf("Reading: %d\n", read_analog());
        vTaskDelay(delay);  
    }
}