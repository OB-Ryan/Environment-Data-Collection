// Ryan Root

#include <stdio.h>
#include "ReadPhotoResistor.h"

void app_main(void) {
    config_adc();
    printf("Starting Data Read:\n");
    endless_read();
}