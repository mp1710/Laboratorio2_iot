#include "nvs_flash.h"
#include "esp_err.h"

#include "rgb_led.h"
#include "wifi_manager.h"
#include "web_server.h"
#include "touch.h"

// función principal de la aplicación
void app_main(void)
{
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    // verificar que la inicialización de NVS fue exitosa
    ESP_ERROR_CHECK(ret);
    // inicializar componentes
    rgb_led_init();
    touchpad_init();
    wifi_init_softap();
    web_server_start();

    // loop infinito para mantener la aplicación corriendo
    while (1)
    {
        touchpad_update_led();
    }
}
