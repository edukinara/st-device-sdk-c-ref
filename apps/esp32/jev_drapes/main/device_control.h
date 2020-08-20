/* ***************************************************************************
 *
 * Copyright 2019 Samsung Electronics All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
 * either express or implied. See the License for the specific
 * language governing permissions and limitations under the License.
 *
 ****************************************************************************/

#include "stepper.h"

#define GPIO_OUTPUT_MAINLED 2

#define DIR_IO 16
#define STEP_IO 17
#define ENABLE_IO 18
#define FULL_STEPS 200
#define STEPS 1000
#define RPM 60

enum drapes_openclosed_state {
    DRAPES_CLOSED = 0,
    DRAPES_OPEN,
    DRAPES_OPENING,
    DRAPES_CLOSING,
    DRAPES_PARTIALLY_OPEN,
};

enum main_led_gpio_state {
    MAINLED_GPIO_ON = 1,
    MAINLED_GPIO_OFF = 0,
};

enum led_animation_mode_list {
    LED_ANIMATION_MODE_IDLE = 0,
    LED_ANIMATION_MODE_FAST,
    LED_ANIMATION_MODE_SLOW,
};

int change_windowShade_state(int windowShade_state, int current_windowShade_state);
void change_led_mode(int noti_led_mode);
void change_led_state(int led_state);
void iot_gpio_init(void);
void stepper_test(int steps, int rpm, int full_steps);
