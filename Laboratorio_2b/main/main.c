#include "nvs_flash.h"

#include "rgb_led.h"
#include "wifi_manager.h"
#include "web_server.h"

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

    wifi_init_softap();

    web_server_start();
}
