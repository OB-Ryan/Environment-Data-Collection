// Ryan Root
// Establish and handle a wireless connection

#include "wireless.h"

static int curr_reconnect_num = 0;

static void Wifi_IP_Event_Handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    // Wifi Event
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
                if (curr_reconnect_num < MAX_RECONNECT_ATTEMPS) {
                    esp_wifi_connect();
                    curr_reconnect_num++;
                } else {
                    xEventGroupSetBits(wifi_event_group, WIFI_FAIL_BIT);
                }
                break;
            case WIFI_EVENT_STA_STOP:
                break;
            default:
                break;
        }
    }

    // IP Event
    if (event_base == IP_EVENT) {
        switch (event_id) {
            case IP_EVENT_STA_GOT_IP:
                ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
                // TODO: Add to log
                curr_reconnect_num = 0;
                xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
            default:
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

    // Hand WiFi events
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
    printf("init finished\n");

    // Wait until connection is established, or failed maximum number of allowed attempts
    EventBits_t event_bits = xEventGroupWaitBits(wifi_event_group,
        WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
        pdFALSE,
        pdFALSE,
        portMAX_DELAY
    );

    // Check the status of our EventGroup
    if (event_bits & WIFI_CONNECTED_BIT) {
        printf("Connected to: %s\n", SSID);
        // TODO: Add to log
    } else if (event_bits & WIFI_FAIL_BIT) {
        printf("Failed to connect to %s\n", SSID);
        // TODO: Add to log
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