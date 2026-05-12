#include "web_server.h"

#include <stdio.h>

#include "esp_http_server.h"
#include "esp_log.h"

#include "rgb_led.h"

static const char *TAG = "webserver";

extern const uint8_t index_html_start[] asm("_binary_index_html_start");
extern const uint8_t index_html_end[]   asm("_binary_index_html_end");

extern const uint8_t style_css_start[] asm("_binary_style_css_start");
extern const uint8_t style_css_end[]   asm("_binary_style_css_end");

extern const uint8_t app_js_start[] asm("_binary_app_js_start");
extern const uint8_t app_js_end[]   asm("_binary_app_js_end");

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

static esp_err_t led_on_handler_rojo(httpd_req_t *req)
{
    ESP_LOGI(TAG, "LED ON");

    rgb_led_rojo();

    httpd_resp_send(req,
                    "ROJO",
                    HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t led_on_handler_verde(httpd_req_t *req)
{
    ESP_LOGI(TAG, "LED ON");

    rgb_led_verde();

    httpd_resp_send(req,
                    "VERDE",
                    HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t led_on_handler_azul(httpd_req_t *req)
{
    ESP_LOGI(TAG, "LED ON");

    rgb_led_azul();

    httpd_resp_send(req,
                    "AZUL",
                    HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t led_on_handler_amarillo(httpd_req_t *req)
{
    ESP_LOGI(TAG, "LED ON");

    rgb_led_amarillo();

    httpd_resp_send(req,
                    "AMARILLO",
                    HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t led_on_handler_cyan(httpd_req_t *req)
{
    ESP_LOGI(TAG, "LED ON");

    rgb_led_cyan();

    httpd_resp_send(req,
                    "CYAN",
                    HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t led_on_handler_magenta(httpd_req_t *req)
{
    ESP_LOGI(TAG, "LED ON");

    rgb_led_magenta();

    httpd_resp_send(req,
                    "MAGENTA",
                    HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t led_on_handler_blanco(httpd_req_t *req)
{
    ESP_LOGI(TAG, "LED ON");

    rgb_led_blanco();

    httpd_resp_send(req,
                    "BLANCO",
                    HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}


static esp_err_t led_off_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "LED OFF");

    rgb_led_off();

    httpd_resp_send(req,
                    "LED OFF",
                    HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

static esp_err_t led_status_handler(httpd_req_t *req)
{
    char json[64];

    snprintf(json,
             sizeof(json),
             "{\"on\":%s,\"color\":\"%s\"}",
             rgb_led_is_on() ? "true" : "false",
             rgb_led_get_color());

    httpd_resp_set_type(req, "application/json");

    httpd_resp_send(req,
                    json,
                    HTTPD_RESP_USE_STRLEN);

    return ESP_OK;
}

void web_server_start(void)
{
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();

    config.max_uri_handlers = 15;

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

    httpd_uri_t led_rojo_uri = {
        .uri = "/led/rojo",
        .method = HTTP_GET,
        .handler = led_on_handler_rojo,
        .user_ctx = NULL
    };

    httpd_uri_t led_verde_uri = {
        .uri = "/led/verde",
        .method = HTTP_GET,
        .handler = led_on_handler_verde,
        .user_ctx = NULL
    };

    httpd_uri_t led_azul_uri = {
        .uri = "/led/azul",
        .method = HTTP_GET,
        .handler = led_on_handler_azul,
        .user_ctx = NULL
    };

    httpd_uri_t led_amarillo_uri = {
        .uri = "/led/amarillo",
        .method = HTTP_GET,
        .handler = led_on_handler_amarillo,
        .user_ctx = NULL
    };

    httpd_uri_t led_cyan_uri = {
        .uri = "/led/cyan",
        .method = HTTP_GET,
        .handler = led_on_handler_cyan,
        .user_ctx = NULL
    };

    httpd_uri_t led_magenta_uri = {
        .uri = "/led/magenta",
        .method = HTTP_GET,
        .handler = led_on_handler_magenta,
        .user_ctx = NULL
    };

    httpd_uri_t led_blanco_uri = {
        .uri = "/led/blanco",
        .method = HTTP_GET,
        .handler = led_on_handler_blanco,
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

    if (httpd_start(&server, &config) == ESP_OK)
    {
        httpd_register_uri_handler(server, &index_uri);
        httpd_register_uri_handler(server, &css_uri);
        httpd_register_uri_handler(server, &js_uri);

        httpd_register_uri_handler(server, &led_rojo_uri);
        httpd_register_uri_handler(server, &led_verde_uri);
        httpd_register_uri_handler(server, &led_azul_uri);
        httpd_register_uri_handler(server, &led_amarillo_uri);
        httpd_register_uri_handler(server, &led_cyan_uri);
        httpd_register_uri_handler(server, &led_magenta_uri);
        httpd_register_uri_handler(server, &led_blanco_uri);
        httpd_register_uri_handler(server, &led_off_uri);
        httpd_register_uri_handler(server, &led_status_uri);

        ESP_LOGI(TAG, "Servidor web iniciado");
    }
}
