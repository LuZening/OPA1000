/*
 * NTC.h
 *
 *  Created on: Jan 29, 2021
 *      Author: Zening
 */

#ifndef NTC_H_
#define NTC_H_

#include "stdint.h"

#define FULL_SCALE_ADC_VALUE 4095
#define NTC_ADC_mVREF 3300

typedef enum
{
	NTC_AT_LOWER = 0,
	NTC_AT_UPPER
} NTCResistorDividerTopology_t;

typedef struct
{
	NTCResistorDividerTopology_t topology;
	uint16_t mV_divider; // max voltage on divider
	uint32_t R_divider;
	uint32_t R_25;
	uint16_t B_const;
	uint8_t nAnchors;
	const int16_t *CAnchors; // Temperatures in C
	const uint32_t *RAnchors; // ADC values
} NTC_t;

void NTC_init(NTC_t *p, NTCResistorDividerTopology_t topology, uint16_t mV_divider, uint32_t R_divider, uint32_t R_25, uint16_t B_const, uint8_t nAnchors, const int16_t* CAnchors, const uint32_t* RAnchors);

// precision to 1C
int16_t NTC_get_C(NTC_t* p, uint16_t vADC);

#endif /* NTC_H_ */
