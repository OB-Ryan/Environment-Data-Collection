# _Reading Enviroment Data_

# ReadPhotoResistor
This file is responsible to reading data from a photo resitor.

# Main
main.c servers as an entry point and controller for all our enviroment reading files. Currently it only calls `endless_read()` which continuosuly reads analog data from a photo resitor with an interval of 25 ticks.

