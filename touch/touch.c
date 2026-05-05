#include "touch.h"
#include "driver/touch_pad.h"
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

// Umbral fijo 
static const uint32_t threshold = 20000;

void touchpad_init(void)
{
    touch_pad_init();

    touch_pad_set_fsm_mode(TOUCH_FSM_MODE_TIMER);
    touch_pad_set_measurement_interval(0x1000);

    for (int i = 0; i < NUM_TOUCH; i++) {
        touch_pad_config(pads[i]);
    }

    touch_pad_fsm_start();

    // espera para estabilizar FSM
    for (volatile int d = 0; d < 3000000; d++);

}

bool touchpad_is_pressed(uint8_t i)
{
    uint32_t val = 0;

    touch_pad_read_raw_data(pads[i], &val);

    printf("PAD %d val=%lu\n", i, val);

    return val > threshold;
}
