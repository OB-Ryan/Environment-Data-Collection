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

    printf("Connecting to WiFi\n");
    for (int i = 1; i <= 3; i++) {
        printf("%d...\n", i);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    print_connection_info();

    // Initialize our MQTT
    ESP_LOGI(TAG, "Initializing and Starting connection to MQTT Broker\n");
    init_mqtt();

    printf("Connecting to Broker\n");
    for (int i = 1; i <= 3; i++) {
        printf("%d...\n", i);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    // Read analog data froever, publish each reading
    config_adc();
    publish_message_mqtt("Beginning Endless (Photo Resistor) Analog Read:");
    ESP_LOGI(TAG, "Starting Analog Data Collection\n");
    char analog_string_buf[20];
    while (1) {
        int analog_read = read_analog();
        snprintf(analog_string_buf, sizeof(analog_string_buf), "%d", analog_read);
        publish_message_mqtt(analog_string_buf);
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
