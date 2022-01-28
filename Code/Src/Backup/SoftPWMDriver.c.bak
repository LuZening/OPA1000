/*
 * SoftPWMDriver.c
 *
 *  Created on: 2021年1月27日
 *      Author: Zening
 */

#include "SoftPWMDriver.h"
#include "main.h"


void soft_pwm_driver_init(SoftPWMDriver_t* p, GPIO_TypeDef* portGPIO, uint16_t pinGPIO, bool polarity, uint8_t levels, uint32_t periodUS)
{
	p->portGPIO = portGPIO;
	p->pinGPIO = pinGPIO;
	p->polarity = polarity;
	p->enabled = false;
	p->levels = levels;
	p->duty = 0;
	p->cntUS = 0;
	p->thresholdUS = 0;
	p->periodUS = periodUS;
}

void soft_pwm_driver_tick(SoftPWMDriver_t* p, uint32_t tickUS)
{

	uint32_t newCntUS = (p->cntUS + tickUS);
	if(p->thresholdUS == 0)
		HAL_GPIO_WritePin(p->portGPIO, p->pinGPIO, !(p->polarity));
	if(newCntUS >= p->periodUS && p->cntUS < p->periodUS)
	{
		HAL_GPIO_WritePin(p->portGPIO, p->pinGPIO, (p->polarity));
	}
	else if (newCntUS >= p->thresholdUS && p->cntUS < p->thresholdUS) // inactivate
	{
		HAL_GPIO_WritePin(p->portGPIO, p->pinGPIO, !(p->polarity));
	}
	p->cntUS = newCntUS % (p->periodUS);
}

void soft_pwm_driver_set_duty(SoftPWMDriver_t* p, uint8_t duty)
{
	p->duty = duty;
	p->thresholdUS = p->periodUS * duty / p->levels;
}

void soft_pwm_driver_disable(SoftPWMDriver_t* p)
{
	p->enabled = false;
	HAL_GPIO_WritePin(p->portGPIO, p->pinGPIO, !(p->polarity));
}

void soft_pwm_driver_enable(SoftPWMDriver_t* p)
{
	p->enabled = true;
	p->cntUS = 0;
	HAL_GPIO_WritePin(p->portGPIO, p->pinGPIO, p->polarity);
}
