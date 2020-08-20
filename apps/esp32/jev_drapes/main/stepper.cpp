

#include "stepper.h"
#include "iot_a4988.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/ledc.h"
#include "driver/pcnt.h"
#include "freertos/timers.h"

struct stepper
{
        void *obj;
};

stepper_t* stepper_create(int step_io, int dir_io, int number_of_steps, int enable_io = 0)
{
	stepper_t *m;
	CA4988Stepper *obj;

	m      = (typeof(m))malloc(sizeof(*m));
	obj    = new CA4988Stepper(step_io, dir_io, number_of_steps, enable_io);
	m->obj = obj;

	return m;   
}

void stepper_destroy(stepper_t *m)
{
	if (m == NULL)
		return;
	delete static_cast<CA4988Stepper *>(m->obj);
	free(m);
}

esp_err_t stepper_step(stepper_t *m, int steps)
{
	CA4988Stepper *obj;

	if (m == NULL)
		return ESP_FAIL;

	obj = static_cast<CA4988Stepper *>(m->obj);
	return obj->step(steps);
}

esp_err_t stepper_run(stepper_t *m, int dir)
{
	CA4988Stepper *obj;

	if (m == NULL)
		return ESP_FAIL;

	obj = static_cast<CA4988Stepper *>(m->obj);
	return obj->run(dir);
}

esp_err_t stepper_stop(stepper_t *m)
{
	CA4988Stepper *obj;

	if (m == NULL)
		return ESP_FAIL;

	obj = static_cast<CA4988Stepper *>(m->obj);
	return obj->stop();
}

esp_err_t stepper_wait(stepper_t *m)
{
	CA4988Stepper *obj;

	if (m == NULL)
		return ESP_FAIL;

	obj = static_cast<CA4988Stepper *>(m->obj);
	return obj->wait();
}

esp_err_t stepper_setSpeedRpm(stepper_t *m, int rpm)
{
	CA4988Stepper *obj;

	if (m == NULL)
		return ESP_FAIL;

	obj = static_cast<CA4988Stepper *>(m->obj);
	return obj->setSpeedRpm(rpm);
}

esp_err_t stepper_setSpeedRpmCur(stepper_t *m, int rpm)
{
	CA4988Stepper *obj;

	if (m == NULL)
		return ESP_FAIL;

	obj = static_cast<CA4988Stepper *>(m->obj);
	return obj->setSpeedRpmCur(rpm);
}

void stepper_enable(stepper_t *m)
{
	CA4988Stepper *obj;

	if (m == NULL)
		return;

	obj = static_cast<CA4988Stepper *>(m->obj);
	obj->enable();
}

void stepper_disable(stepper_t *m)
{
	CA4988Stepper *obj;

	if (m == NULL)
		return;

	obj = static_cast<CA4988Stepper *>(m->obj);
	obj->disable();
}

int stepper_getSpeedRpm(stepper_t *m)
{
	CA4988Stepper *obj;

	if (m == NULL)
		return 0;

	obj = static_cast<CA4988Stepper *>(m->obj);
	return obj->getSpeedRpm();
}

int stepper_getSpeedRpmTarget(stepper_t *m)
{
	CA4988Stepper *obj;

	if (m == NULL)
		return 0;

	obj = static_cast<CA4988Stepper *>(m->obj);
	return obj->getSpeedRpmTarget();
}