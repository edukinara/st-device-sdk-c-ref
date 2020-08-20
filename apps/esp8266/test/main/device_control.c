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

xTaskHandle myTaskHandle;

void change_led_state(int led_state)
{
    if (led_state == MAINLED_GPIO_OFF) {
        gpio_set_level(GPIO_OUTPUT_MAINLED, MAINLED_GPIO_OFF);
    } else {
        gpio_set_level(GPIO_OUTPUT_MAINLED, MAINLED_GPIO_ON);
    }
}

void myTask(void *args) {
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
            printf("Done\n");
            astepper_disableOutputs(m);
            running = 0;
            change_led_state(MAINLED_GPIO_OFF);
            vTaskDelete(NULL);
        }
    }
}

void trigger() {
    xTaskCreate(myTask, "myTask", 2048, NULL, 5, myTaskHandle);
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
    steps = _steps;
    trigger();
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

}
