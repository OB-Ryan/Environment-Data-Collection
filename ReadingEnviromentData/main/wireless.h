// Ryan Root
// Establish and handle a wireless connection

#include "stdio.h"
#include "esp_mac.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#define SSID                    ""              // Your SSID
#define PASSWORD                ""              // Your password
#define WIFI_CONNECTED_BIT      BIT0
#define WIFI_FAIL_BIT           BIT1
#define MAX_RECONNECT_ATTEMPS   10

static EventGroupHandle_t wifi_event_group;

static void Wifi_Event_Handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
void LwIP_WiFi_Init();
void WiFi_Start();
void WiFi_Deinit();
void print_connection_info();
