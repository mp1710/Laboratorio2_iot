#ifndef TOUCH_H
#define TOUCH_H

#include <stdbool.h>
#include <stdint.h>

void touchpad_init(void);
bool touchpad_is_pressed(uint8_t i);

#endif
