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

#include "st_dev.h"
#include "device_control.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "iot_uart_cli.h"
#include "iot_cli_cmd.h"

#include "caps_windowShade.h"

// onboarding_config_start is null-terminated string
extern const uint8_t onboarding_config_start[]    asm("_binary_onboarding_config_json_start");
extern const uint8_t onboarding_config_end[]    asm("_binary_onboarding_config_json_end");

// device_info_start is null-terminated string
extern const uint8_t device_info_start[]    asm("_binary_device_info_json_start");
extern const uint8_t device_info_end[]        asm("_binary_device_info_json_end");

static iot_status_t g_iot_status = IOT_STATUS_IDLE;
static iot_stat_lv_t g_iot_stat_lv;

IOT_CTX* ctx = NULL;

//#define SET_PIN_NUMBER_CONFRIM

static int noti_led_mode = LED_ANIMATION_MODE_IDLE;

static caps_windowShade_data_t *cap_windowShade_data;

int current_windowShade_state = DRAPES_CLOSED;

static int get_windowShade_state(void)
{
    const char* windowShade_value = cap_windowShade_data->get_windowShade_value(cap_windowShade_data);
    int windowShade_state = DRAPES_CLOSED;

    if (!windowShade_value) {
        return -1;
    }

    if (!strcmp(windowShade_value, caps_helper_windowShade.attr_windowShade.value_closed)) {
        windowShade_state = DRAPES_CLOSED;
    } else if (!strcmp(windowShade_value, caps_helper_windowShade.attr_windowShade.value_open)) {
        windowShade_state = DRAPES_OPEN;
    } else if (!strcmp(windowShade_value, caps_helper_windowShade.attr_windowShade.value_opening)) {
        windowShade_state = DRAPES_OPENING;
    } else if (!strcmp(windowShade_value, caps_helper_windowShade.attr_windowShade.value_closing)) {
        windowShade_state = DRAPES_CLOSING;
    } else if (!strcmp(windowShade_value, caps_helper_windowShade.attr_windowShade.value_partially_open)) {
        windowShade_state = DRAPES_PARTIALLY_OPEN;
    }
    return windowShade_state;
}

static void cap_windowShade_cmd_cb(struct caps_windowShade_data *caps_data)
{
    int windowShade_state = get_windowShade_state();
    printf("New: %d, Current: %d\n", windowShade_state, current_windowShade_state);
    current_windowShade_state = change_windowShade_state(windowShade_state, current_windowShade_state);
}

static void capability_init()
{
    cap_windowShade_data = caps_windowShade_initialize(ctx, "main", NULL, NULL);
    if (cap_windowShade_data) {
        const char *windowShade_init_value = caps_helper_windowShade.attr_windowShade.value_closed;

        cap_windowShade_data->cmd_open_usr_cb = cap_windowShade_cmd_cb;
        cap_windowShade_data->cmd_close_usr_cb = cap_windowShade_cmd_cb;
        cap_windowShade_data->cmd_pause_usr_cb = cap_windowShade_cmd_cb;

        cap_windowShade_data->set_windowShade_value(cap_windowShade_data, windowShade_init_value);
    }
}

static void iot_status_cb(iot_status_t status,
                          iot_stat_lv_t stat_lv, void *usr_data)
{
    g_iot_status = status;
    g_iot_stat_lv = stat_lv;

    printf("status: %d, stat: %d\n", g_iot_status, g_iot_stat_lv);

    switch(status)
    {
        case IOT_STATUS_NEED_INTERACT:
            noti_led_mode = LED_ANIMATION_MODE_FAST;
            break;
        case IOT_STATUS_IDLE:
        case IOT_STATUS_CONNECTING:
            noti_led_mode = LED_ANIMATION_MODE_IDLE;
            // change_led_state(MAINLED_GPIO_OFF);
            current_windowShade_state = change_windowShade_state(get_windowShade_state(), current_windowShade_state);
            break;
        default:
            break;
    }
}

#if defined(SET_PIN_NUMBER_CONFRIM)
void* pin_num_memcpy(void *dest, const void *src, unsigned int count)
{
    unsigned int i;
    for (i = 0; i < count; i++)
        *((char*)dest + i) = *((char*)src + i);
    return dest;
}
#endif

static void connection_start(void)
{
    iot_pin_t *pin_num = NULL;
    int err;

#if defined(SET_PIN_NUMBER_CONFRIM)
    pin_num = (iot_pin_t *) malloc(sizeof(iot_pin_t));
    if(!pin_num)
        printf("failed to malloc for iot_pin_t\n");

    // to decide the pin confirmation number(ex. "12345678"). It will use for easysetup.
    //    pin confirmation number must be 8 digit number.
    pin_num_memcpy(pin_num, "12345678", sizeof(iot_pin_t));
#endif

    // process on-boarding procedure. There is nothing more to do on the app side than call the API.
    err = st_conn_start(ctx, (st_status_cb)&iot_status_cb, IOT_STATUS_ALL, NULL, pin_num);
    if (err) {
        printf("fail to start connection. err:%d\n", err);
    }
    if (pin_num) {
        free(pin_num);
    }
}

static void iot_noti_cb(iot_noti_data_t *noti_data, void *noti_usr_data)
{
    printf("Notification message received\n");

    if (noti_data->type == IOT_NOTI_TYPE_DEV_DELETED) {
        printf("[device deleted]\n");
    } else if (noti_data->type == IOT_NOTI_TYPE_RATE_LIMIT) {
        printf("[rate limit] Remaining time:%d, sequence number:%d\n",
               noti_data->raw.rate_limit.remainingTime, noti_data->raw.rate_limit.sequenceNumber);
    }
}

void stepper_test_event(IOT_CAP_HANDLE *handle, int steps)
{
    stepper_test(steps);
}

static void app_main_task(void *arg)
{
    for (;;) {
        if (noti_led_mode != LED_ANIMATION_MODE_IDLE) {
            change_led_mode(noti_led_mode);
        }

        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void app_main(void)
{
    unsigned char *onboarding_config = (unsigned char *) onboarding_config_start;
    unsigned int onboarding_config_len = onboarding_config_end - onboarding_config_start;
    unsigned char *device_info = (unsigned char *) device_info_start;
    unsigned int device_info_len = device_info_end - device_info_start;

    int iot_err;

    // create a iot context
    ctx = st_conn_init(onboarding_config, onboarding_config_len, device_info, device_info_len);
    if (ctx != NULL) {
        iot_err = st_conn_set_noti_cb(ctx, iot_noti_cb, NULL);
        if (iot_err)
            printf("fail to set notification callback function\n");
    } else {
        printf("fail to create the iot_context\n");
    }

    // create a handle to process capability and initialize capability info
    capability_init();

    iot_gpio_init();
    register_iot_cli_cmd();
    uart_cli_main();
    xTaskCreate(app_main_task, "app_main_task", 4096, NULL, 10, NULL);

    // connect to server
    connection_start();
}
