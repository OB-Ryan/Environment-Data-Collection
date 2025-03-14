// Ryan Root
// Establish and handle a wireless connection

#include "wireless.h"

static int curr_reconnect_num = 0;
EventGroupHandle_t wifi_event_group;
static const char* TAG = "WIRELESS";

static void Wifi_IP_Event_Handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    // Wifi Event
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
                break;
            case WIFI_EVENT_STA_CONNECTED:
                ESP_LOGI(TAG, "WiFi connected");
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                if (curr_reconnect_num < MAX_RECONNECT_ATTEMPS) {
                    ESP_LOGW(TAG, "WiFi disconnected. Attempting to reconnect");
                    esp_wifi_connect();
                    curr_reconnect_num++;
                } else {
                    xEventGroupSetBits(wifi_event_group, WIFI_FAIL_BIT);
                    ESP_LOGE(TAG, "Connection failed.");
                }
                break;
            case WIFI_EVENT_STA_STOP:
                ESP_LOGI(TAG, "WiFi stopped");
                break;
            default:
                ESP_LOGW(TAG, "Unhandled event. ID: %ld", event_id);
                break;
        }
    }

    // IP Event
    if (event_base == IP_EVENT) {
        switch (event_id) {
            case IP_EVENT_STA_GOT_IP:
                ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
                ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
                curr_reconnect_num = 0;
                xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
            default:
                ESP_LOGW(TAG, "Unhandled event. ID: %ld", event_id);
                break;
        }
    }
}

void LwIP_WiFi_Init() {
    wifi_event_group = xEventGroupCreate();

    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_t* netif = esp_netif_create_default_wifi_sta();

    wifi_init_config_t init = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&init));

    // Handle WiFi events
    esp_event_handler_instance_t instance_any_id;
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(
            WIFI_EVENT, 
            ESP_EVENT_ANY_ID,
            &Wifi_IP_Event_Handler,
            NULL,
            &instance_any_id
        )
    );

    // Handle IP events
    esp_event_handler_instance_t instance_got_ip;
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(
            IP_EVENT,
            IP_EVENT_STA_GOT_IP,
            &Wifi_IP_Event_Handler,
            NULL,
            &instance_got_ip
        )
    );

    // Configure connection
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = SSID,
            .password = PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    // Use Station Mode
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG, "Initialization of wireless finished");

    // Wait until connection is established, or failed maximum number of allowed attempts
    EventBits_t event_bits = xEventGroupWaitBits(wifi_event_group,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY
    );

    // Check the status of our EventGroup
    if (event_bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "Connected to %s", SSID);
    } else if (event_bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to %s", SSID);
    } else {
        ESP_LOGE(TAG, "Error with event!");
    }
}

void WiFi_Start() {
    ESP_ERROR_CHECK(esp_wifi_start());
}

void WiFi_Deinit() {
    esp_wifi_disconnect();
    esp_wifi_stop();
    esp_wifi_deinit();
}

void print_connection_info() {
    wifi_ap_record_t ap_info;
    esp_netif_ip_info_t ip_info;

    // Access Point information
    if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
        ESP_LOGI(TAG, "Connected to SSID: %s", ap_info.ssid);
    } else {
        ESP_LOGW(TAG, "Failed to get AP info");
    }

    // IP information
    if (esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &ip_info) == ESP_OK) {
        char ip[100];
        char netmask[100];
        char gateway[100];
        esp_ip4addr_ntoa(&ip_info.ip, ip, 100);
        esp_ip4addr_ntoa(&ip_info.netmask, netmask, 100);
        esp_ip4addr_ntoa(&ip_info.gw, gateway, 100);
        ESP_LOGI(TAG, "IP Address: %s", ip);
        ESP_LOGI(TAG, "Subnet Mask: %s", netmask);
        ESP_LOGI(TAG, "Gateway: %s", gateway);
    } else {
        ESP_LOGW(TAG, "Failed to get IP information");
    }
}
