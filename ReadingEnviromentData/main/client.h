// Ryan Root
// MQTT Client Functions

#include <stdio.h>
#include "mqtt_client.h"
#include "esp_log.h"
#include "esp_event.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#define BROKER_ADR              ""  // Broker Address
#define BROKER_URI              ""  // Full Broker URI
#define TOPIC                   "EnviromentData"
#define TOPIC_MESSAGE_HEADER    "ESP32 Subscribed to Topic: "
#define TCP_PORT                1883
#define TLS_PORT                8883
#define MQTT_CONNECTED_BIT      BIT0
#define MQTT_FAIL_BIT           BIT1
#define MAX_RECONNECT_ATTEMPTS  10            

extern EventGroupHandle_t mqtt_event_group;

static void mqtt_event_handler(void* args, esp_event_base_t event_base, int32_t event_id, void* event_data);
void init_mqtt();
void publish_message_mqtt(char* message_string);
