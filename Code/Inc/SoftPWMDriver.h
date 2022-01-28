/*
 * SoftPWMDriver.h
 *
 *  Created on: 2021年1月27日
 *      Author: Zening
 */

#ifndef SOFTPWMDRIVER_H_
#define SOFTPWMDRIVER_H_

#include "stdint.h"
#include "stdbool.h"
#include "stm32f407xx.h"


typedef struct
{
	GPIO_TypeDef* portGPIO;
	uint16_t pinGPIO;
	bool enabled;
	bool polarity; // True: HIGH EFFECT False: Low Effect
	uint8_t levels; // number of adjustment levels
	uint8_t duty; // duty cycle (0 ~ levels-1)
	uint32_t periodUS;
	uint32_t thresholdUS; // activate if cntUS < thresholdUS
	volatile uint32_t cntUS; // current counter

} SoftPWMDriver_t;

void soft_pwm_driver_init(SoftPWMDriver_t* p, GPIO_TypeDef* portGPIO, uint16_t pinGPIO, bool polarity, uint8_t levels, uint32_t periodUS);

void soft_pwm_driver_tick(SoftPWMDriver_t* p, uint32_t tickUS);

void soft_pwm_driver_set_duty(SoftPWMDriver_t* p, uint8_t duty);

void soft_pwm_driver_disable(SoftPWMDriver_t* p);

void soft_pwm_driver_enable(SoftPWMDriver_t* p);

#endif /* SOFTPWMDRIVER_H_ */
