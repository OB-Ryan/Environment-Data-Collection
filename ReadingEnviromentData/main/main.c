// Ryan Root

#include <stdio.h>
#include "nvs_flash.h"
#include "esp_log.h"
#include "ReadPhotoResistor.h"
#include "client.h"
#include "wireless.h"

static const char* TAG = "MAIN";

void init_NVS() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void app_main(void) {
    ESP_LOGI(TAG, "Initializing NVS\n");
    init_NVS();

    // Attempt wifi connection
    ESP_LOGI(TAG, "Initializing and Starting WiFi\n");
    LwIP_WiFi_Init();

    // Delay 5 seconds
    vTaskDelay(pdMS_TO_TICKS(5000));
    print_connection_info();

    // Initialize our MQTT
    ESP_LOGI(TAG, "Initializing and Starting connection to MQTT Broker\n");
    init_mqtt();

    // Delay 5 seconds
    vTaskDelay(pdMS_TO_TICKS(5000));

    // Read analog data 25 times, publish each reading
    ESP_LOGI(TAG, "Starting Analog Data Collection\n");
    config_adc();
    char analog_string_buf[20];
    for (int i = 0; i < 25; i++) {
        int analog_read = read_analog();
        sprintf(analog_string_buf, "%d", analog_read);
        publish_message_mqtt(analog_string_buf);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
