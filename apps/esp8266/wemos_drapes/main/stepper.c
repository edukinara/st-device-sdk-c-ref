/* pwm example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "esp_log.h"
#include "esp_system.h"
#include "esp_err.h"

#include "esp8266/gpio_register.h"
#include "esp8266/pin_mux_register.h"

#include "driver/pwm.h"
#include "device_control.h"
#include "driver/gpio.h"

// PWM period 5000us(200Hz), same as depth
#define FREQ    (5000)

double _freq = FREQ;
double _res = RES_16;

// pwm pin number
const uint32_t pin_num[1] = {
    STEP_IO,
};

// phase table, delay = (phase[x]/360)*PERIOD
int16_t phase[1] = {
    0,
};

void stepper_init(int rpm, int res)
{
    _res = res;
    double _rpm = rpm * _res;

    if (_rpm > 0) {
        _freq = FREQ / (_rpm / 60);
    }

    uint32_t duties[1] = {
        _freq / 2
    };

    uint32_t PWM_PERIOD = _freq;

    pwm_init(PWM_PERIOD, duties, 1, pin_num);
    pwm_set_phases(phase);
}

void step(int steps)
{
    double _steps = steps * _res;

    double delay = (_freq / 1000) * _steps;
    
    gpio_set_level(ENABLE_IO, 0);

    pwm_start();
    vTaskDelay(delay / portTICK_RATE_MS);    
    pwm_stop(0x0);

    gpio_set_level(ENABLE_IO, 1);
}
