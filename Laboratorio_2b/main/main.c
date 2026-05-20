#include "nvs_flash.h"
#include "esp_err.h"

#include "rgb_led.h"
#include "wifi_manager.h"
#include "web_server.h"
#include "touch.h"
#include "delay.h"

#define WIFI_MODE_AP_SELECTED   1
#define WIFI_MODE_STA_SELECTED  2
#define WIFI_MODE_APSTA_SELECTED 3

// defino red sta
#define STA_SSID "caliope"
#define STA_PASS "sinlugar"

#define WIFI_SELECTED_MODE WIFI_MODE_APSTA_SELECTED

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

    wifi_init_sta(STA_SSID, STA_PASS);
    web_server_start();

#elif WIFI_SELECTED_MODE == WIFI_MODE_APSTA_SELECTED

    wifi_init_apsta();
    web_server_start();

#endif

    while (1)
    {    
        touchpad_update_led();
        delay_ms(10);
    }
}
