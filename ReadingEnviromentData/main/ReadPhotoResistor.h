// Ryan Root
// 04/08/2025
// Read Serial Data from Photo Resistor - GPIO 36

#include <stdio.h>
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define PHOTO_RESISTOR      GPIO_NUM_36
#define CHANNEL             ADC1_CHANNEL_0      // For GPIO 36
#define TICK_DELAY          25

void config_adc ();
int read_analog ();
void endless_read();