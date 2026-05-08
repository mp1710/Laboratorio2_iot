#include <stdio.h>
#include <string.h>

#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_http_server.h"

#include "led_strip.h"

#define WIFI_SSID      "PAN"
#define WIFI_PASS      "LAFIERA1891"
#define WIFI_CHANNEL   6
#define MAX_STA_CONN   4

/* GPIO DEL RGB */
#define LED_GPIO 18

static bool led_on = false;
static const char *led_color = "off";
static const char *TAG = "wifi_ap";

/* LED RGB */
static led_strip_t *led_strip;

/* Archivos embebidos */
extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[]   asm("_binary_index_html_end");

extern const uint8_t style_css_start[] asm("_binary_style_css_start");
extern const uint8_t style_css_end[]   asm("_binary_style_css_end");

extern const uint8_t app_js_start[] asm("_binary_app_js_start");
extern const uint8_t app_js_end[]   asm("_binary_app_js_end");

//RGB

void led_init(void)
{
    ESP_ERROR_CHECK(led_rgb_init(&led_strip));
    led_strip->clear(led_strip, 100);
}

void led_set_red(void)
{
    led_strip->set_pixel(led_strip, 0, 255, 0, 0);
    led_strip->refresh(led_strip, 100);

    led_on = true;
    led_color = "red";
}

void led_set_off(void)
{
    led_strip->clear(led_strip, 100);

    led_on = false;
    led_color = "off";
}

//WIFI_EVENT_HANDLER

static void wifi_event_handler(void *arg,
                               esp_event_base_t event_base,
                               int32_t event_id,
                               void *event_data)
{
    if (event_base == WIFI_EVENT &&
        event_id == WIFI_EVENT_AP_STACONNECTED) {

        wifi_event_ap_staconnected_t *event =
            (wifi_event_ap_staconnected_t *) event_data;

        ESP_LOGI(TAG,
                 "Dispositivo conectado. AID=%d",
                 event->aid);
    }

    if (event_base == WIFI_EVENT &&
        event_id == WIFI_EVENT_AP_STADISCONNECTED) {

        wifi_event_ap_stadisconnected_t *event =
            (wifi_event_ap_stadisconnected_t *) event_data;

        ESP_LOGI(TAG,
                 "Dispositivo desconectado. AID=%d",
                 event->aid);
    }
}

//HTML

static esp_err_t index_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/html");

    httpd_resp_send(
        req,
        (const char *)index_html_start,
        index_html_end - index_html_start
    );

    return ESP_OK;
}

//CSS

static esp_err_t css_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "text/css");

    httpd_resp_send(
        req,
        (const char *)style_css_start,
        style_css_end - style_css_start
    );

    return ESP_OK;
}

//JS

static esp_err_t js_handler(httpd_req_t *req)
{
    httpd_resp_set_type(req, "application/javascript");

    httpd_resp_send(
        req,
        (const char *)app_js_start,
        app_js_end - app_js_start
    );

    return ESP_OK;
}

//LED_ON

static esp_err_t led_on_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "LED RGB ON");

    led_strip->set_pixel(led_strip, 0, 255, 0, 0);
    led_strip->refresh(led_strip, 100);

    led_on = true;
    led_color = "red";

    httpd_resp_send(req, "LED ON", HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}
//LED_OFF
static esp_err_t led_off_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "LED RGB OFF");

    led_strip->clear(led_strip, 100);

    led_on = false;
    led_color = "off";

    httpd_resp_send(req, "LED OFF", HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t led_status_handler(httpd_req_t *req)
{
    char json[64];

    snprintf(json, sizeof(json),
             "{\"on\":%s,\"color\":\"%s\"}",
             led_on ? "true" : "false",
             led_color);

    httpd_resp_set_type(req, "application/json");
    httpd_resp_send(req, json, HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

//WESERVER

void start_webserver(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    httpd_handle_t server = NULL;

    httpd_uri_t index_uri = {
        .uri = "/",
        .method = HTTP_GET,
        .handler = index_handler,
        .user_ctx = NULL
    };

    httpd_uri_t css_uri = {
        .uri = "/style.css",
        .method = HTTP_GET,
        .handler = css_handler,
        .user_ctx = NULL
    };

    httpd_uri_t js_uri = {
        .uri = "/app.js",
        .method = HTTP_GET,
        .handler = js_handler,
        .user_ctx = NULL
    };

    httpd_uri_t led_on_uri = {
        .uri = "/led/on",
        .method = HTTP_GET,
        .handler = led_on_handler,
        .user_ctx = NULL
    };

    httpd_uri_t led_off_uri = {
        .uri = "/led/off",
        .method = HTTP_GET,
        .handler = led_off_handler,
        .user_ctx = NULL
    };
    httpd_uri_t led_status_uri = {
    .uri = "/led",
    .method = HTTP_GET,
    .handler = led_status_handler,
    .user_ctx = NULL
    };

    if (httpd_start(&server, &config) == ESP_OK) {

        httpd_register_uri_handler(server, &index_uri);
        httpd_register_uri_handler(server, &css_uri);
        httpd_register_uri_handler(server, &js_uri);

        httpd_register_uri_handler(server, &led_on_uri);
        httpd_register_uri_handler(server, &led_off_uri);
        httpd_register_uri_handler(server, &led_status_uri);
        ESP_LOGI(TAG, "Servidor web iniciado");
    }
}


// WIFI_INIT_SOFTAP
void wifi_init_softap(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg =
        WIFI_INIT_CONFIG_DEFAULT();

    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(
        esp_event_handler_instance_register(
            WIFI_EVENT,
            ESP_EVENT_ANY_ID,
            &wifi_event_handler,
            NULL,
            NULL
        )
    );

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = WIFI_SSID,
            .ssid_len = strlen(WIFI_SSID),
            .channel = WIFI_CHANNEL,
            .password = WIFI_PASS,
            .max_connection = MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .required = false,
            },
        },
    };

    if (strlen(WIFI_PASS) == 0) {
        wifi_config.ap.authmode =
            WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(
        esp_wifi_set_mode(WIFI_MODE_AP)
    );

    ESP_ERROR_CHECK(
        esp_wifi_set_config(
            WIFI_IF_AP,
            &wifi_config
        )
    );

    ESP_ERROR_CHECK(
        esp_wifi_start()
    );

    ESP_LOGI(TAG,
             "AP iniciado. SSID:%s password:%s canal:%d",
             WIFI_SSID,
             WIFI_PASS,
             WIFI_CHANNEL);
}


//MAIN
void app_main(void)
{
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES ||
        ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {

        ESP_ERROR_CHECK(nvs_flash_erase());

        ret = nvs_flash_init();
    }

    ESP_ERROR_CHECK(ret);

    led_init();

    wifi_init_softap();

    start_webserver();
}
