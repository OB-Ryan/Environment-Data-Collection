// Ryan Root
// MQTT Client Functions

#include "client.h"

static const char* TAG = "MQTT";
static esp_mqtt_client_handle_t client;
static int curr_reconnect_num = 0;
EventGroupHandle_t mqtt_event_group;

static void mqtt_event_handler(void* args, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t)event_data;

    switch (event_id) {
        case MQTT_EVENT_CONNECTED:
            ESP_LOGI(TAG, "Client has connected\n");
            // QoS 0; Fire and Forget
            esp_mqtt_client_subscribe(client, TOPIC, 0);
            ESP_LOGI(TAG, "Cient has subscribed to topic %s\n", TOPIC);
            break;
        case MQTT_EVENT_DISCONNECTED:
            if (curr_reconnect_num < MAX_RECONNECT_ATTEMPTS) {
                ESP_LOGW(TAG, "MQTT disconnected. Attempting to reconnect\n");
                esp_mqtt_client_subscribe(client, TOPIC, 0);
                curr_reconnect_num++;
            } else {
                xEventGroupSetBits(mqtt_event_group, MQTT_FAIL_BIT);
                ESP_LOGE(TAG, "MQTT connection failed.\n");
            }
            break;
        case MQTT_EVENT_SUBSCRIBED:
            ESP_LOGI(TAG, "Client has subscribed to a topic\n");
            publish_message_mqtt();
            break;
        case MQTT_EVENT_UNSUBSCRIBED:
            ESP_LOGI(TAG, "Client has unsubscribed from a topic\n");
            break;
        case MQTT_EVENT_PUBLISHED:
            ESP_LOGI(TAG, "Client published a message.\n");
            break;
        case MQTT_EVENT_ERROR:
            ESP_LOGE(TAG, "MQTT EVENT ERROR!\n");
            break;
        case MQTT_EVENT_DATA:
            ESP_LOGI(TAG, "Received event data\n");
            break;
        default:
            ESP_LOGW(TAG, "MQTT Event %ld left undandled...!\n", event_id);
            break;
    }
}

void init_mqtt () {
    mqtt_event_group = xEventGroupCreate();

    const esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = BROKER_URI
    };

    client = esp_mqtt_client_init(&mqtt_cfg);
    ESP_ERROR_CHECK(
        esp_mqtt_client_register_event(
            client,
            ESP_EVENT_ANY_ID,
            mqtt_event_handler,
            client
        )
    );
    ESP_ERROR_CHECK(esp_mqtt_client_start(client));

    // Wait until mqtt connection is established, or failed maximum number of allowed attempts
    EventBits_t event_bits = xEventGroupWaitBits(mqtt_event_group,
        MQTT_CONNECTED_BIT | MQTT_FAIL_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY
    );

    // Check the status of our EventGroup
    if (event_bits & MQTT_CONNECTED_BIT) {
        ESP_LOGI(TAG, "Connected to %s\n", BROKER_ADR);
    } else if (event_bits & MQTT_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to %s\n", BROKER_ADR);
    } else {
        ESP_LOGE(TAG, "Error with event!");
    }
}

void publish_message_mqtt() {
    /*
    *   PUBLISH TEST:
    *       - Test string
    *       - QoS 0, Fire and Forget
    *       - len: 0 (is calcuated from data string)
    *       - retain 1: Broker retains
    */
    esp_mqtt_client_publish(
        client,
        TOPIC,
        "Hello, message string!\n",
        0,
        0,
        1
    );
}
