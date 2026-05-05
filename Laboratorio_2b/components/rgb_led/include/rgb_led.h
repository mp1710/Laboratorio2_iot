#ifndef RGB_LED_H
#define RGB_LED_H

#include "esp_err.h"

// Se declararon las funciones que seran utilizadas en rgb_led.c, 
//cada una de ellas retorna un valor de tipo esp_err_t (de la libreria 'esp_err.h') 
// que indica si la operación fue exitosa o no.
esp_err_t rgb_led_init(void);

esp_err_t rgb_led_rojo(void);
esp_err_t rgb_led_verde(void);
esp_err_t rgb_led_azul(void);

esp_err_t rgb_led_off(void);

esp_err_t rgb_led_set_color(uint8_t r, uint8_t g, uint8_t b);

#endif
