// Ryan Root

#include "stdio.h"
#include "ReadPhotoResistor.h"
#include "client.h"
#include "wireless.h"

void app_main(void) {
    // Attempt connection
    if (!LwIP_WiFi_Init()) {
        printf("Failed to init wifi\n");
    } else {
        printf("Sucessfully init wifi\n");
    }

    if (!WiFi_Config()) {
        printf("Failed to configure wifi\n");
    } else {
        printf("Sucessfully configured wifi\n");
    }

    if (!WiFi_Start()) {
        printf("Failed to start wifi\n");
    } else {
        printf("Sucessfully started wifi\n");
    }

    // Delay 5 seconds
    vTaskDelay(pdMS_TO_TICKS(5000));
    print_connection_info();

    config_adc();
    endless_read();

    WiFi_Deinit();
}