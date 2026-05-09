#include "touch.h"
#include "driver/touch_pad.h"
#include "rgb_led.h"
#include <stdio.h>

#define NUM_TOUCH 6

static touch_pad_t pads[NUM_TOUCH] = {
    TOUCH_PAD_NUM11,
    TOUCH_PAD_NUM5,
    TOUCH_PAD_NUM2,
    TOUCH_PAD_NUM6,
    TOUCH_PAD_NUM1,
    TOUCH_PAD_NUM3
};

static uint32_t base[NUM_TOUCH];
static uint32_t threshold[NUM_TOUCH];

#define TOUCH_MARGIN 0.10f   // 10%

void touchpad_init(void)
{
    touch_pad_init();

    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    touch_pad_set_measurement_interval(0x1000);

    for (int i = 0; i < NUM_TOUCH; i++) {
        touch_pad_config(pads[i]);
    }

    touch_pad_fsm_start();

    // estabilizar FSM 
    for (volatile int d = 0; d < 4000000; d++);

    for (int i = 0; i < NUM_TOUCH; i++) {

        uint32_t sum = 0;
        uint32_t val = 0;

        for (int k = 0; k < 10; k++) {
            touch_pad_read_raw_data(pads[i], &val);
            sum += val;
        }

        base[i] = sum / 10;

        // porcentaje sobre baseline
        threshold[i] = (uint32_t)(base[i] * (1.0f + TOUCH_MARGIN));

        printf("PAD %d base=%lu threshold=%lu\n",
               i, base[i], threshold[i]);
    }
}

bool touchpad_is_pressed(uint8_t i)
{
    uint32_t val = 0;

    touch_pad_read_raw_data(pads[i], &val);

    //printf("PAD %d val=%lu thr=%lu\n",
           //i, val, threshold[i]);

    return val > threshold[i];
}

void touchpad_update_led(void)
{
    if (touchpad_is_pressed(0)) {
        rgb_led_rojo();
    }
    else if (touchpad_is_pressed(1)) {
        rgb_led_verde();
    }
    else if (touchpad_is_pressed(2)) {
        rgb_led_azul();
    }
    else if (touchpad_is_pressed(3)) {
        rgb_led_off();
    }
}
