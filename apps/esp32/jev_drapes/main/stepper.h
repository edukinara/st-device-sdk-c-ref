#ifndef __STEPPER_H
#define __STEPPER_H
#include "esp_err.h"
#include "esp_log.h"
#include "driver/ledc.h"
#include "driver/pcnt.h"
#include "freertos/timers.h"

#ifdef __cplusplus
extern "C" {
#endif
struct stepper;
typedef struct stepper stepper_t;

stepper_t* stepper_create(int step_io, int dir_io, int number_of_steps, int enable_io);

void stepper_destroy(stepper_t *m);

esp_err_t stepper_step(stepper_t *m, int steps);

esp_err_t stepper_run(stepper_t *m, int dir);

esp_err_t stepper_stop(stepper_t *m);

esp_err_t stepper_wait(stepper_t *m);

esp_err_t stepper_setSpeedRpm(stepper_t *m, int rpm);

esp_err_t stepper_setSpeedRpmCur(stepper_t *m, int rpm);

void stepper_enable(stepper_t *m);

void stepper_disable(stepper_t *m);

int stepper_getSpeedRpm(stepper_t *m);

int stepper_getSpeedRpmTarget(stepper_t *m);

#ifdef __cplusplus
}
#endif
#endif /* __STEPPER_H__ */