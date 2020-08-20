#ifndef __ASTEPPER_H
#define __ASTEPPER_H
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"

#ifdef __cplusplus
extern "C" {
#endif
struct astepper;
typedef struct astepper astepper_t;

astepper_t* astepper_init(int dir_io, int step_io);

// void astepper_destroy(astepper_t *m);

void astepper_moveTo(astepper_t *m, int absolute);

int astepper_runSpeedToPosition(astepper_t *m);

int astepper_run(astepper_t *m);

void astepper_stop(astepper_t *m);

void astepper_setSpeed(astepper_t *m, int speed);

void astepper_setMaxSpeed(astepper_t *m, int speed);

int astepper_runSpeed(astepper_t *m);

void astepper_setEnablePin(astepper_t *m, int enable_pin);

// Defaults to dir = false, step = false, enable = true)
void astepper_setPinsInverted(astepper_t *m, bool directionInvert, bool stepInvert, bool enableInvert);

void astepper_disableOutputs(astepper_t *m);

void astepper_enableOutputs(astepper_t *m);

uint16_t astepper_currentPos(astepper_t *m);

void astepper_setMinPulseWidth(astepper_t *m, uint16_t duty);

int astepper_distanceToGo(astepper_t *m);
void astepper_setDirection(astepper_t *m, bool direction);

#ifdef __cplusplus
}
#endif
#endif /* __ASTEPPER_H__ */