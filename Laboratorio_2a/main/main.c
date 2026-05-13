#include "touch.h"
#include "rgb_led.h"
#include "delay.h"

void app_main(void)
{
    touchpad_init();
    rgb_led_init();

    while (1)
    {
        if (touchpad_is_pressed(0)) rgb_led_rojo();
        else if (touchpad_is_pressed(1)) rgb_led_verde();
        else if (touchpad_is_pressed(2)) rgb_led_azul();
        else if (touchpad_is_pressed(3)) rgb_led_set_color(255,255,0);
        else if (touchpad_is_pressed(4)) rgb_led_set_color(0,255,255);
        else if (touchpad_is_pressed(5)) rgb_led_set_color(255,0,255);
        else rgb_led_off();

        delay_ms(100);
    }
}
