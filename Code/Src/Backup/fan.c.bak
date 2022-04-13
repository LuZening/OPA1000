/*
 * fan.c
 *
 *  Created on: Mar 29, 2022
 *      Author: cpholzn
 */


#include "fan.h"

void fan_init(Fan_t* p, GPIO_TypeDef* portGPIO, uint16_t pinGPIO)
{
	// 10000us = 10ms period, levels = 10
	soft_pwm_driver_init(&p->PWM, portGPIO, pinGPIO, 1, 100, 10000);
	soft_pwm_driver_set_duty(&p->PWM, 30);
	soft_pwm_driver_enable(&p->PWM);
	p->AutoFanSpeed_NSegments = 0;
	p->AutoFanSpeed_speed = NULL;
}


void fan_init_automode(Fan_t* p, uint8_t AutoFanSpeed_NSegments,
		const int16_t *AutoFanSpeed_tempC, const uint8_t *AutoFanSpeed_speed, uint8_t AutoFanSpeed_hyst)
{
	p->AutoFanSpeed_NSegments = AutoFanSpeed_NSegments;
	p->AutoFanSpeed_tempC = AutoFanSpeed_tempC;
	p->AutoFanSpeed_speed = AutoFanSpeed_speed;
	p->AutoFanSpeed_hyst = AutoFanSpeed_hyst;
}

uint8_t fan_get_speed(Fan_t* p)
{
	return p->PWM.duty;
}

uint8_t fan_speed_transfer_func(Fan_t* p, int16_t tempCur)
{
	uint8_t i = 0, speedOld = fan_get_speed(p);
	uint8_t speedNew;

	// find current temperature level
	if(p->AutoFanSpeed_NSegments == 0 || p->AutoFanSpeed_speed == NULL || p->AutoFanSpeed_tempC == NULL)
		return FAN_MAX_SPEED;

	// invalid values, max fanspeed
	if(tempCur < FAN_INVALID_TEMP_C)
		return FAN_MAX_SPEED;

	for(i = 0; i < p->AutoFanSpeed_NSegments; ++i)
	{
		if(tempCur < p->AutoFanSpeed_tempC[i])
			break;
	}

	// theoretically lookup for the candidate new speed
	if(i > 0)
		speedNew = p->AutoFanSpeed_speed[i-1];
	else
		speedNew = 0; // No speed

	// if the candidate new speed goes down, use hystersis mechanism to avoid fan speed waggling around boundaries
	if(speedNew < speedOld
			&& i < p->AutoFanSpeed_NSegments
			&& !(tempCur + p->AutoFanSpeed_hyst < p->AutoFanSpeed_tempC[i]))
	{
		speedNew = speedOld;
	}

	if(speedNew != speedOld) fan_set_speed(p, speedNew);

	return speedNew;

}


bool fan_enabled(Fan_t* p)
{
	return p->PWM.enabled;
}

void fan_set_speed(Fan_t* p, uint8_t speedCur)
{
	soft_pwm_driver_set_duty(p, speedCur);
}


void fan_tick(Fan_t* p, uint16_t tick_us, bool isAuto, int16_t tempCur)
{
	if(fan_enabled(p))
	{
		if (isAuto)
		{
			uint8_t speedOld = fan_get_speed(p);
			uint8_t speedNew = fan_speed_transfer_func(p, tempCur);
			if (speedNew != speedOld)
				fan_set_speed(p, speedNew);
		}
		soft_pwm_driver_tick(&(p->PWM), tick_us);
	}
}


