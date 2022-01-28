/*
 * touch_HR2046.h
 *
 *  Created on: Jun 28, 2020
 *      Author: Zening
 */

#ifndef TOUCH_HR2046_H_
#define TOUCH_HR2046_H_

#include "stm32f4xx_hal.h"

#define TOUCHSCREEN_XY_REVERSED

extern SPI_HandleTypeDef hspi1;

#if defined(TOUCHSCREEN_XY_REVERSED)
#define HR2046_CMD_READ_Y 0xD0
#define HR2046_CMD_READ_X 0x90
#else
#define HR2046_CMD_READ_X 0xD0
#define HR2046_CMD_READ_Y 0x90
#endif

#define TOUCHSCREEN_CALIB_START 1U
#define TOUCHSCREEN_CALIB_POINT_CAPTURED 2U
//#define TOUCHSCREEN_CALIB_POINT_UR_CAPTURED 4U
//#define TOUCHSCREEN_CALIB_POINT_LL_CAPTURED 8U
//#define TOUCHSCREEN_CALIB_POINT_LR_CAPTURED 16U
//#define TOUCHSCREEN_CALIB_POINT_LR_CAPTURED 32U
//#define TOUCHSCREEN_CALIB_POINT_LR_CAPTURED 64U

#define TOUCHSCREEN_CALIB_UPPER_MARGIN 0.1
#define TOUCHSCREEN_CALIB_LEFT_MARGIN 0.1


#include "stdbool.h"

typedef struct
{
	bool isCalibrated;
	// UL, UR, LL, LR, Center
	uint16_t real_X[5];
	uint16_t real_Y[5]; // real pixel positions, originate at the upper-left corner
	// UL, UR, LL, LR, Center
	uint16_t ADC_X[5];
	uint16_t ADC_Y[5]; // 5 point calibration info
	float k_X;
	float k_Y;
} TouchScreenCalibrationInfo_t;

void init_touch_screen_calib_info(TouchScreenCalibrationInfo_t *p);

// allocate pTSCalib somewhere in main program
// e.g. load from EEPROM
extern TouchScreenCalibrationInfo_t *pTSCalib;


// set and analyze raw calibration info
// @param real_X, real_Y: the ideal position of 5 points, (UL, UR, LL, LR, C)
// @param ADC_X, ADC_Y: the ADC readings of 5 points when calibratings
// @retval result: a place to store calibration info
void touch_set_calibrate(uint16_t real_X[5], uint16_t real_Y[5],
		uint16_t ADC_X[5], uint16_t ADC_Y[5],
		TouchScreenCalibrationInfo_t *result
		);

void touch_read_XY_ADC(uint16_t *pX_orig, uint16_t *pY_orig);

void touch_read_XY_physical(uint16_t *pX, uint16_t *pY);


#endif /* TOUCH_HR2046_H_ */
