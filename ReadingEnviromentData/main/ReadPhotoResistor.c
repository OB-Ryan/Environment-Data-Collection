// Ryan Root
// 04/08/2025
// Read Serial Data from Photo Resistor - GPIO 36

#include "ReadPhotoResistor.h"

const char* TAG = "PHOTO_RESISTOR";

void config_adc () {
    // Set the bit capture width
    ESP_ERROR_CHECK(adc1_config_width(ADC_BITWIDTH_12));

    // Set the attenuation level
    ESP_ERROR_CHECK(adc1_config_channel_atten(CHANNEL, ADC_ATTEN_DB_12));
}

int read_analog () {
    return adc1_get_raw(CHANNEL);
}

void endless_read() {
    config_adc();
    ESP_LOGI(TAG, "Starting Data Read:\n");

    TickType_t delay = TICK_DELAY;
    int i = 0;
    while (i < 25) {
        // Continuously read data - wait 25 ticks between each read
        printf("Reading: %d\n", read_analog());
        vTaskDelay(delay);
        i++; 
    }
}