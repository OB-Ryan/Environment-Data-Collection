// Ryan Root

#include "stdio.h"
#include "nvs_flash.h"
#include "ReadPhotoResistor.h"
#include "client.h"
#include "wireless.h"

void init_NVS() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void app_main(void) {
    init_NVS();

    // Attempt connection
    LwIP_WiFi_Init();

    // Delay 5 seconds
    vTaskDelay(pdMS_TO_TICKS(5000));
    print_connection_info();

    // Read analog data 25 times
    config_adc();
    endless_read();

    // Stop and flush our connection
    WiFi_Deinit();
}