// ACEELSTEPPER

#include "astepper.h"
#include "accelstepper.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/ledc.h"
#include "freertos/timers.h"

struct astepper
{
        void *obj;
};

astepper_t* astepper_init(int dir_io, int step_io)
{
	astepper_t *m;
	accelstepper *obj;
	uint8_t interface = 1;

	m      = (typeof(m))malloc(sizeof(*m));
	obj    = new accelstepper(interface, (gpio_num_t)dir_io, (gpio_num_t)step_io);
	m->obj = obj;

	return m;   
}

// void astepper_destroy(astepper_t *m)
// {
// 	if (m == NULL)
// 		return;
// 	delete static_cast<accelstepper *>(m->obj);
// 	free(m);
// }

void astepper_moveTo(astepper_t *m, int absolute)
{
	accelstepper *obj;

	if (m == NULL)
		return;

	obj = static_cast<accelstepper *>(m->obj);
	obj->moveTo(absolute);
}

int astepper_runSpeedToPosition(astepper_t *m)
{
	accelstepper *obj;

	if (m == NULL)
		return 0;

	obj = static_cast<accelstepper *>(m->obj);
	return obj->runSpeedToPosition();
}

int astepper_run(astepper_t *m)
{
	accelstepper *obj;

	if (m == NULL)
		return false;

	obj = static_cast<accelstepper *>(m->obj);
	return obj->run();
}

void astepper_stop(astepper_t *m)
{
	accelstepper *obj;

	if (m == NULL)
		return;

	obj = static_cast<accelstepper *>(m->obj);
	obj->stop();
}

void astepper_setSpeed(astepper_t *m, int speed)
{
	accelstepper *obj;

	if (m == NULL)
		return;

	obj = static_cast<accelstepper *>(m->obj);
	obj->setSpeed(speed);
}

void astepper_setMaxSpeed(astepper_t *m, int speed)
{
	accelstepper *obj;

	if (m == NULL)
		return;

	obj = static_cast<accelstepper *>(m->obj);
	obj->setMaxSpeed(speed);
}

int astepper_runSpeed(astepper_t *m)
{
	accelstepper *obj;

	if (m == NULL)
		return false;

	obj = static_cast<accelstepper *>(m->obj);
	return obj->runSpeed();
}

void astepper_setEnablePin(astepper_t *m, int enable_pin)
{
	accelstepper *obj;

	if (m == NULL)
		return;

	obj = static_cast<accelstepper *>(m->obj);
	obj->setEnablePin((gpio_num_t)enable_pin);
}

// Defaults to dir = false, step = false, enable = true)
void astepper_setPinsInverted(astepper_t *m, bool directionInvert = false, bool stepInvert = false, bool enableInvert = true)
{
	accelstepper *obj;

	if (m == NULL)
		return;

	obj = static_cast<accelstepper *>(m->obj);
	obj->setPinsInverted(directionInvert, stepInvert, enableInvert);
}

void astepper_disableOutputs(astepper_t *m)
{
	accelstepper *obj;

	if (m == NULL)
		return;

	obj = static_cast<accelstepper *>(m->obj);
	obj->disableOutputs();
}

void astepper_enableOutputs(astepper_t *m)
{
	accelstepper *obj;

	if (m == NULL)
		return;

	obj = static_cast<accelstepper *>(m->obj);
	obj->enableOutputs();
}

uint16_t astepper_currentPos(astepper_t *m)
{
	accelstepper *obj;

	if (m == NULL)
		return 0;

	obj = static_cast<accelstepper *>(m->obj);
	return obj->currentPosition();
}

void astepper_setMinPulseWidth(astepper_t *m, uint16_t duty)
{
	accelstepper *obj;

	if (m == NULL)
		return;

	obj = static_cast<accelstepper *>(m->obj);
	obj->setMinPulseWidth(duty);
}

int astepper_distanceToGo(astepper_t *m)
{
	accelstepper *obj;

	if (m == NULL)
		return 0;

	obj = static_cast<accelstepper *>(m->obj);
	return obj->distanceToGo();
}

void astepper_setDirection(astepper_t *m, bool direction)
{
	accelstepper *obj;

	if (m == NULL)
		return;

	obj = static_cast<accelstepper *>(m->obj);
	obj->setDirection(direction);
}