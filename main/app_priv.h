#pragma once
#include <stdint.h>
#include <stdbool.h>
#include <esp_rmaker_core.h>

#define DEFAULT_TEMPERATURE 0.0
#define DEFAULT_HUMIDITY    0.0
#define REPORTING_PERIOD    5  /* Seconds */



extern esp_rmaker_device_t *temp_sensor_device;
extern esp_rmaker_device_t *humidity_sensor_device;
extern esp_rmaker_device_t *switch_device;

void app_driver_init(void);
float app_get_current_temperature();
float app_get_current_humidity();