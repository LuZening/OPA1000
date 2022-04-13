/*
 * fan.h
 *
 *  Created on: Mar 29, 2022
 *      Author: cpholzn
 */

#ifndef FAN_H_
#define FAN_H_

#include <stddef.h>
#include "SoftPWMDriver.h"
#define FAN_MAX_SPEED 100
#define FAN_INVALID_TEMP_C -200

// t <= 40.0C, FanSpeed = 0%
// t <= 50.0C, FanSpeed = 30%
// t <= 60.0C, FanSpeed = 50%
// t <= 70.0C, FanSpeed = 80%
// t > 70.0C, FanSpeed = 100%
// when temp falling down, slow down the fanspeed with hystersis 5.0C, i.e. t<= 35.0C, Fanspeed = 0



typedef struct {
	SoftPWMDriver_t PWM;
	uint8_t AutoFanSpeed_NSegments;
	const int16_t *AutoFanSpeed_tempC;
	const uint8_t *AutoFanSpeed_speed;
	uint8_t AutoFanSpeed_hyst;
} Fan_t;

void fan_init(Fan_t* p, GPIO_TypeDef* portGPIO, uint16_t pinGPIO);
void fan_init_automode(Fan_t* p, uint8_t AutoFanSpeed_NSegments, const int16_t *AutoFanSpeed_tempC, const uint8_t *AutoFanSpeed_speed, uint8_t AutoFanSpeed_hyst);
uint8_t fan_speed_transfer_func(Fan_t* p, int16_t tempCur);
uint8_t fan_get_speed(Fan_t* p);
void fan_set_speed(Fan_t* p, uint8_t speedCur);
bool fan_enabled(Fan_t* p);
// service handle routine to be called at fixed intervals
// use a ticker to pace this function
void fan_tick(Fan_t* p, uint16_t tick_us, bool isAuto, int16_t tempCur);

#endif /* FAN_H_ */
