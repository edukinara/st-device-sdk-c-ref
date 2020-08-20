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


#include "device_control.h"

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "driver/gpio.h"
// #include "stepper.h"
#include "astepper.h"

int steps = 0;
int running = 0;

xTaskHandle stepperTaskHandle;

void change_led_state(int led_state)
{
    if (led_state == MAINLED_GPIO_OFF) {
        gpio_set_level(GPIO_OUTPUT_MAINLED, MAINLED_GPIO_OFF);
    } else {
        gpio_set_level(GPIO_OUTPUT_MAINLED, MAINLED_GPIO_ON);
    }
}

void stepper_task(void *args) {
    astepper_t *m = astepper_init(DIR_IO, STEP_IO);
    astepper_setEnablePin(m, ENABLE_IO);
    astepper_setPinsInverted(m, false, false, true);
    astepper_setSpeed(m, SPEED);
    astepper_setMinPulseWidth(m, DUTY);
    astepper_moveTo(m, steps);
    astepper_setSpeed(m, SPEED);
    astepper_enableOutputs(m);
    int dtg = astepper_distanceToGo(m);
    astepper_setDirection(m, dtg >= 0);
    running = 1;

    while(1) {
        if(astepper_distanceToGo(m) != 0) {
            astepper_runSpeed(m);
        } else if (running) {
            astepper_disableOutputs(m);
            running = 0;
            steps = 0;
            change_led_state(MAINLED_GPIO_OFF);
            vTaskDelete(NULL);
        }
    }
}

void stepper_start() {
    xTaskCreate(stepper_task, "stepper_task", 2048, NULL, 20, stepperTaskHandle);
}

int change_windowShade_state(int windowShade_state, int current_windowShade_state)
{
    int newState = current_windowShade_state;
    if (windowShade_state != current_windowShade_state)
    {
        if (windowShade_state == DRAPES_CLOSED) {
            change_led_state(MAINLED_GPIO_OFF);
            gpio_set_level(DIR_IO, DRAPES_CLOSED);
            // step(STEPS);
            newState = DRAPES_OPEN;
        } else if (windowShade_state == DRAPES_OPEN){
            change_led_state(MAINLED_GPIO_ON);
            gpio_set_level(DIR_IO, DRAPES_OPEN);
            // step(STEPS);
            newState = DRAPES_CLOSED;
        }
    }
    return newState;
}

void stepper_test(int _steps)
{
    change_led_state(MAINLED_GPIO_ON);
    // if (steps < 0) {
    //     gpio_set_level(DIR_IO, DRAPES_CLOSED);
    //     step(-1*steps);
    // } else {
    //     gpio_set_level(DIR_IO, DRAPES_OPEN);
    //     step(steps);
    // }
    // change_led_state(MAINLED_GPIO_OFF);
    steps = _steps;
    stepper_start();
}

void change_led_mode(int noti_led_mode)
{
    static TimeOut_t led_timeout;
    static TickType_t led_tick = -1;
    static int last_led_mode = -1;
    static int led_state = MAINLED_GPIO_OFF;

    if (last_led_mode != noti_led_mode) {
        last_led_mode = noti_led_mode;
        vTaskSetTimeOutState(&led_timeout);
        led_tick = 0;
    }

    switch (noti_led_mode)
    {
        case LED_ANIMATION_MODE_IDLE:
            break;
        case LED_ANIMATION_MODE_SLOW:
            if (xTaskCheckForTimeOut(&led_timeout, &led_tick ) != pdFALSE) {
                led_state = 1 - led_state;
                change_led_state(led_state);
                vTaskSetTimeOutState(&led_timeout);
                if (led_state == MAINLED_GPIO_ON) {
                    led_tick = pdMS_TO_TICKS(200);
                } else {
                    led_tick = pdMS_TO_TICKS(800);
                }
            }
            break;
        case LED_ANIMATION_MODE_FAST:
            if (xTaskCheckForTimeOut(&led_timeout, &led_tick ) != pdFALSE) {
                led_state = 1 - led_state;
                change_led_state(led_state);
                vTaskSetTimeOutState(&led_timeout);
                led_tick = pdMS_TO_TICKS(100);
            }
            break;
        default:
            break;
    }
}

void iot_gpio_init(void)
{
	gpio_config_t io_conf;

	io_conf.intr_type = GPIO_INTR_DISABLE;
	io_conf.mode = GPIO_MODE_OUTPUT;
	io_conf.pin_bit_mask = 1 << GPIO_OUTPUT_MAINLED;
	io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
	io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
	gpio_config(&io_conf);

    io_conf.pin_bit_mask = 1 << DIR_IO;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_conf);
    
    io_conf.pin_bit_mask = 1 << ENABLE_IO;
    gpio_config(&io_conf);

	gpio_set_level(ENABLE_IO, 1);
	gpio_set_level(GPIO_OUTPUT_MAINLED, MAINLED_GPIO_OFF);

    // stepper_init(RPM, RES_16);
}
