#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <sdkconfig.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_types.h> 
#include <esp_rmaker_standard_params.h> 
#include <driver/gpio.h>
#include "app_reset.h"
#include "app_priv.h"
#include "dht.h"  
#include "esp_log.h"
#define LED_GPIO GPIO_NUM_10  // LED control GPIO
#define BUTTON_GPIO 5  // GPIO for reset button
#define SWITCH_GPIO GPIO_NUM_4  // GPIO for switch control
#define DHT_PIN GPIO_NUM_2  
#define DHT_TYPE DHT_TYPE_DHT11  
#define WIFI_RESET_BUTTON_TIMEOUT 3
#define FACTORY_RESET_BUTTON_TIMEOUT 11

static TimerHandle_t sensor_timer;
static float g_temperature = DEFAULT_TEMPERATURE;
static float g_humidity = DEFAULT_HUMIDITY;

static void app_sensor_update(TimerHandle_t handle)
{
    if (dht_read_float_data(DHT_TYPE, DHT_PIN, &g_humidity, &g_temperature) == ESP_OK) {
        ESP_LOGI("DHT", "Temperature: %.1f°C, Humidity: %.1f%%", g_temperature, g_humidity);
        
        /* Update Temperature */
        esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_type(temp_sensor_device, ESP_RMAKER_PARAM_TEMPERATURE),
            esp_rmaker_float(g_temperature));

        /* Update Humidity */
        esp_rmaker_param_t *humidity_param = esp_rmaker_device_get_param_by_name(humidity_sensor_device, "Humidity");
        if (humidity_param) {
            esp_rmaker_param_update_and_report(humidity_param, esp_rmaker_float(g_humidity));
        } else {
            ESP_LOGE("RainMaker", "Failed to get Humidity parameter!");
        }

        /* 🔥 Raise Alert if Temperature > 30°C */
        if (g_temperature > 30) {
            char alert_msg[ESP_RMAKER_MAX_ALERT_LEN];
            snprintf(alert_msg, sizeof(alert_msg), "🔥 High Temperature Alert: %.1f°C!", g_temperature);
            ESP_LOGW("ALERT", "%s", alert_msg);
            esp_rmaker_raise_alert(alert_msg);
        }
    } else {
        ESP_LOGE("DHT", "Failed to read from DHT sensor!");
    }
}


float app_get_current_temperature() { return g_temperature; }
float app_get_current_humidity() { return g_humidity; }

esp_err_t app_sensor_init(void)
{
    sensor_timer = xTimerCreate("app_sensor_update_tm", pdMS_TO_TICKS(REPORTING_PERIOD * 1000),
                                pdTRUE, NULL, app_sensor_update);
    if (sensor_timer) {
        xTimerStart(sensor_timer, 0);
        return ESP_OK;
    }
    return ESP_FAIL;
}

void app_driver_init()
{
    /* Initialize LED GPIO */
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_GPIO, 0);  // Start with LED off

    app_sensor_init();
    app_reset_button_register(app_reset_button_create(BUTTON_GPIO, BUTTON_ACTIVE_LOW),
                              WIFI_RESET_BUTTON_TIMEOUT, FACTORY_RESET_BUTTON_TIMEOUT);
}