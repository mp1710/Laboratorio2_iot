#include "nvs_flash.h"
#include "esp_err.h"

#include "rgb_led.h"
#include "wifi_manager.h"
#include "web_server.h"
#include "touch.h"

#define WIFI_MODE_AP_SELECTED   1
#define WIFI_MODE_STA_SELECTED  2

// Cambiar acá:
// AP  -> WIFI_MODE_AP_SELECTED
// STA -> WIFI_MODE_STA_SELECTED

#define WIFI_SELECTED_MODE WIFI_MODE_AP_SELECTED

void app_main(void)
{
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    rgb_led_init();
    touchpad_init();

#if WIFI_SELECTED_MODE == WIFI_MODE_AP_SELECTED

    wifi_init_softap();
    web_server_start();

#elif WIFI_SELECTED_MODE == WIFI_MODE_STA_SELECTED

    wifi_init_sta();
    web_server_start();

#endif

    while (1)
    {
        touchpad_update_led();
    }
}