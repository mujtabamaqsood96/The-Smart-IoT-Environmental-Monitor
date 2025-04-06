#include <string.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_log.h>
#include <nvs_flash.h>
#include <driver/gpio.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_params.h>
#include <esp_rmaker_standard_devices.h>
#include <esp_rmaker_ota.h>
#include <app_network.h>
#include <app_insights.h>
#include "app_priv.h"
#define LED_GPIO GPIO_NUM_10  // LED control GPIO
static const char *TAG = "app_main";

esp_rmaker_device_t *temp_sensor_device;
esp_rmaker_device_t *humidity_sensor_device;
esp_rmaker_device_t *switch_device;

static esp_err_t app_switch_write_cb(const esp_rmaker_device_t *device,
    const esp_rmaker_param_t *param,
    const esp_rmaker_param_val_t val,
    void *priv,
    esp_rmaker_write_ctx_t *ctx)
{
    const char *param_name = esp_rmaker_param_get_name(param);
    
    if (strcmp(param_name, ESP_RMAKER_DEF_POWER_NAME) == 0) {
        ESP_LOGI(TAG, "Received LED switch value: %s", val.val.b ? "ON" : "OFF");
        gpio_set_level(LED_GPIO, val.val.b ? 1 : 0);
        esp_rmaker_param_update_and_report(param, val);
    }
    return ESP_OK;
}

void app_main()
{
    /* Initialize driver (including LED GPIO) */
    app_driver_init();

    /* Initialize NVS */
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    /* Initialize Wi-Fi */
    app_network_init();

    /* Initialize RainMaker */
    esp_rmaker_config_t rainmaker_cfg = {
        .enable_time_sync = false,
    };
    esp_rmaker_node_t *node = esp_rmaker_node_init(&rainmaker_cfg, "ESP RainMaker Device", "DHT11 Sensor");
    if (!node) {
        ESP_LOGE(TAG, "Could not initialize node. Aborting!!!");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        abort();
    }

    /* Create Temperature Sensor */
    temp_sensor_device = esp_rmaker_temp_sensor_device_create("Temperature Sensor", NULL, app_get_current_temperature());
    esp_rmaker_node_add_device(node, temp_sensor_device);

    /* Create Humidity Sensor */
    humidity_sensor_device = esp_rmaker_device_create("Humidity Sensor", NULL, NULL);
    esp_rmaker_param_t *humidity_param = esp_rmaker_param_create(
        "Humidity", NULL, esp_rmaker_float(app_get_current_humidity()), PROP_FLAG_READ);
    esp_rmaker_device_add_param(humidity_sensor_device, humidity_param);
    esp_rmaker_device_assign_primary_param(humidity_sensor_device, humidity_param);
    esp_rmaker_node_add_device(node, humidity_sensor_device);

    /* Create Switch Device for LED */
    switch_device = esp_rmaker_switch_device_create("LED Switch", NULL, false);
esp_rmaker_device_add_cb(switch_device, app_switch_write_cb, NULL);
esp_rmaker_node_add_device(node, switch_device);
    esp_rmaker_param_t *power_param = esp_rmaker_param_create(
        ESP_RMAKER_DEF_POWER_NAME, NULL, esp_rmaker_bool(false), 
        PROP_FLAG_READ | PROP_FLAG_WRITE);
    esp_rmaker_device_add_param(switch_device, power_param);
    esp_rmaker_device_assign_primary_param(switch_device, power_param);
    esp_rmaker_device_add_cb(switch_device, app_switch_write_cb, NULL);
    esp_rmaker_node_add_device(node, switch_device);
    ESP_LOGI(TAG, "LED Switch device created");

    /* Enable OTA */
    esp_rmaker_ota_enable_default();

    /* Enable Insights */
    app_insights_enable();

    /* Start RainMaker */
    esp_rmaker_start();

    /* Start Wi-Fi */
    err = app_network_start(POP_TYPE_RANDOM);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Could not start Wi-Fi. Aborting!!!");
        vTaskDelay(5000 / portTICK_PERIOD_MS);
        abort();
    }
}