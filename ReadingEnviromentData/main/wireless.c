// Ryan Root
// Establish a wireless connection

#include "wireless.h"
#include "esp_mac.h"

void connect () {
    wifi_init_config_t init = WIFI_INIT_CONFIG_DEFAULT();
    if (esp_wifi_init(&init) != ESP_OK) {
        printf("Error initializing wifi\n");
        exit(-1);
    }   
}