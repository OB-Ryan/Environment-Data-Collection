// Ryan Root
// 04/08/2025
// Read Serial Data from Photo Resistor - GPIO 36

#include "ReadPhotoResistor.h"

static const char* TAG = "PHOTO_RESISTOR";

void config_adc () {
    // Set the bit capture width
    ESP_ERROR_CHECK(adc1_config_width(ADC_BITWIDTH_12));

    // Set the attenuation level
    ESP_ERROR_CHECK(adc1_config_channel_atten(CHANNEL, ADC_ATTEN_DB_12));
}

int read_analog () {
    int analog_read = adc1_get_raw(CHANNEL);
    ESP_LOGI(TAG, "%d", analog_read);
    return analog_read;
}

void endless_read() {
    ESP_LOGI(TAG, "Starting Endless PhotoResistor Read:");

    TickType_t delay = TICK_DELAY;
    int i = 0;
    while (i < 25) {
        // Continuously read data - wait 25 ticks between each read
        vTaskDelay(delay);
        i++; 
    }
}
