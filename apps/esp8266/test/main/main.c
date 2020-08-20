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

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "device_control.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "iot_uart_cli.h"
#include "iot_cli_cmd.h"

void stepper_test_event(int steps)
{
    stepper_test(steps);
}

static void app_main_task(void *arg)
{
    for (;;) {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    iot_gpio_init();
    register_iot_cli_cmd();
    uart_cli_main();
    xTaskCreate(app_main_task, "app_main_task", 4096, NULL, 10, NULL);
}
