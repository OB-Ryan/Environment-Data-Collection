// Ryan Root
// MQTT Client Functions

#include "client.h"

void start_mqtt () {
    /*
    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = BROKER_ADR
    };
    esp_mqtt_client_handle_t client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, client);
    esp_mqtt_client_start(client);
    */
}

void start_mqtt();
void send_message();
void end_mqtt();