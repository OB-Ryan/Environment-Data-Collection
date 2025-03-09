// Ryan Root
// Establish and handle a wireless connection

#include "wireless.h"

bool EXPECTED_DISCONNECT = false;

static void Wifi_Event_Handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:
                esp_wifi_connect();
                break;
            case WIFI_EVENT_STA_CONNECTED:
                printf("Connected\n");
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                // TODO: if this event is set by esp_wifi_disconnect() -> Do not call connect
                if (!EXPECTED_DISCONNECT) {
                    esp_wifi_connect();
                    EXPECTED_DISCONNECT = false;
                }
                printf("Disconnect expected... Handling nothing...\n");
                break;
            case WIFI_EVENT_STA_STOP:
                break;
            default:
                break;
        }
    }
}

bool LwIP_WiFi_Init() {
    // Initialize the flash
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    if (esp_netif_init() != ESP_OK) {
        return false;
    }

    if (esp_event_loop_create_default() != ESP_OK) {
        return false;
    }

    esp_netif_t* netif = esp_netif_create_default_wifi_sta();

    wifi_init_config_t init = WIFI_INIT_CONFIG_DEFAULT();
    if (esp_wifi_init(&init) != ESP_OK) {
        return false;
    }

    // Hand WiFi events
    esp_event_handler_instance_t any_id;
    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(
                WIFI_EVENT, 
                ESP_EVENT_ANY_ID,
                &Wifi_Event_Handler,
                NULL,
                &any_id
            )
    );
    return true;
}

bool WiFi_Config() {
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = SSID,
            .password = PASSWORD,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };

    // Use Station Mode
    if (esp_wifi_set_mode(WIFI_MODE_STA) != ESP_OK) {
        return false;
    }

    if (esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) != ESP_OK) {
        return false;
    }

    return true;
}

bool WiFi_Start() {
    if (esp_wifi_start() != ESP_OK) {
        return false;
    }

    return true;
}


void WiFi_Deinit() {
    EXPECTED_DISCONNECT = true;
    esp_wifi_disconnect();
    esp_wifi_stop();
    esp_wifi_deinit();
}

void print_connection_info() {
    wifi_ap_record_t ap_info;
    esp_netif_ip_info_t ip_info;

    // Access Point information
    if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
        printf("Connected to SSID: %s\n", ap_info.ssid);
    } else {
        printf("Failed to get AP info\n");
    }

    // IP information
    if (esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_STA_DEF"), &ip_info) == ESP_OK) {
        char ip[100];
        char netmask[100];
        char gateway[100];
        esp_ip4addr_ntoa(&ip_info.ip, ip, 100);
        esp_ip4addr_ntoa(&ip_info.netmask, netmask, 100);
        esp_ip4addr_ntoa(&ip_info.gw, gateway, 100);
        printf("IP Address: %s\n", ip);
        printf("Subnet Mask: %s\n", netmask);
        printf("Gateway: %s\n", gateway);
    } else {
        printf("Failed to get IP info\n");
    }
}