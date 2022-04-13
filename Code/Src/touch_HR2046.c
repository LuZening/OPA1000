/*
 * touch_HR2046.c
 *
 *  Created on: Jun 28, 2020
 *      Author: Zening
 */

#include "touch_HR2046.h"
#include "main.h"
#include "string.h"

#define hspi hspi1

TouchScreenCalibrationInfo_t *pTSCalib = NULL;

void init_touch_screen_calib_info(TouchScreenCalibrationInfo_t *p)
{
	p->isCalibrated = false;
}

static uint16_t touch_read_data(uint8_t cmd)
{
	static uint8_t txData = 0xff;
	// enable CS
	HAL_GPIO_WritePin(T_CSX_GPIO_Port, T_CSX_Pin, GPIO_PIN_RESET);
	//HAL_Delay(1);
	// write command
	uint8_t res = HAL_SPI_Transmit(&hspi, &cmd, 1, HAL_MAX_DELAY);
	// wait for the busy signal
//	uint8_t busy;
//	uint32_t attempts = 0;
//	do
//	{
//		busy = T_CS_GPIO_Port->IDR & T_CS_Pin;
//	}while(busy && attempts++ < 1000);
	// read value
	uint8_t h, l;
	res = HAL_SPI_TransmitReceive(&hspi, &txData, &h, 1, HAL_MAX_DELAY);
	res = HAL_SPI_TransmitReceive(&hspi, &txData, &l, 1, HAL_MAX_DELAY);
	uint16_t r = (((uint16_t)h << 8) | l) >> 3;
	// disable CS
	HAL_GPIO_WritePin(T_CSX_GPIO_Port, T_CSX_Pin, GPIO_PIN_SET);
	return r;
}

void touch_set_calibrate(uint16_t real_X[5], uint16_t real_Y[5],
		uint16_t ADC_X[5], uint16_t ADC_Y[5],
		TouchScreenCalibrationInfo_t *result
		)
{
	float num = (float)((int32_t)real_X[1] + (int32_t)real_X[3] -(int32_t)real_X[0] - (int32_t)real_X[2]);
	float div = (float)((int32_t)ADC_X[1] + (int32_t)ADC_X[3] -(int32_t)ADC_X[0] - (int32_t)ADC_X[2]);
	if(div != 0.)
		result->k_X = (num / div);
	num = (float)((int32_t)real_Y[0] + (int32_t)real_Y[1]-(int32_t)real_Y[2] - (int32_t)real_Y[3]);
	div = (float)((int32_t)ADC_Y[0] + (int32_t)ADC_Y[1]-(int32_t)ADC_Y[2] - (int32_t)ADC_Y[3]);
	if (div != 0.)
		result->k_Y = num / div;

	if(ADC_X != result->ADC_X)
		memcpy(result->ADC_X, ADC_X, 5*sizeof(uint16_t));
	if(ADC_Y != result->ADC_Y)
		memcpy(result->ADC_Y, ADC_Y, 5*sizeof(uint16_t));
	if(real_X != result->real_X)
		memcpy(result->real_X, real_X, 5*sizeof(uint16_t));
	if(real_Y != result->real_Y)
		memcpy(result->real_Y, real_Y, 5*sizeof(uint16_t));
	result->isCalibrated = true;
	if(pTSCalib == NULL)
		pTSCalib = result;

}

void touch_read_XY_ADC(uint16_t *pX_orig, uint16_t *pY_orig)
{
	// read X
	*pX_orig = touch_read_data(HR2046_CMD_READ_X);
	// read Y
	*pY_orig = touch_read_data(HR2046_CMD_READ_Y);
}

void touch_read_XY_physical(uint16_t *pX, uint16_t* pY)
{
	uint16_t ADC_X, ADC_Y;
	touch_read_XY_ADC(&ADC_X, &ADC_Y);
	if(pTSCalib != NULL && pTSCalib->isCalibrated)
	{
		// ADC_X[4]: X_Center
		*pX = ((int32_t)ADC_X - pTSCalib->ADC_X[4]) * (pTSCalib->k_X) + pTSCalib->real_X[4];
		// ADC_Y[4]: Y_Center
		*pY = ((int32_t)ADC_Y - pTSCalib->ADC_Y[4]) * (pTSCalib->k_Y) + pTSCalib->real_Y[4];
	}
	else // uncalibrated
	{
		*pX = UINT16_MAX;
		*pY = UINT16_MAX;
	}
}
