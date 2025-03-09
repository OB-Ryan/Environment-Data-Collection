// Ryan Root
// Establish and handle a wireless connection

#include "stdio.h"
#include "esp_mac.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "nvs_flash.h"
#include "esp_log.h"

#define SSID ""                  // Your SSID
#define PASSWORD ""              // Your password


static void Wifi_Event_Handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
bool LwIP_WiFi_Init();
bool WiFi_Config();
bool WiFi_Start();
void WiFi_Deinit();
void print_connection_info();
