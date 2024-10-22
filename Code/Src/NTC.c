/*
 * NTC.c
 *
 *  Created on: Jan 30, 2021
 *      Author: Zening
 */


#include "NTC.h"


void NTC_init(NTC_t *p, NTCResistorDividerTopology_t topology, uint16_t mV_divider, uint32_t R_divider, uint32_t R_25, uint16_t B_const, uint8_t nAnchors, const int16_t* CAnchors, const uint32_t* RAnchors)
{
	p->topology = topology;
	p->mV_divider = mV_divider;
	p->B_const = B_const;
	p->R_25 = R_25;
	p->R_divider = R_divider;
	p->nAnchors = nAnchors;
	p->CAnchors = CAnchors;
	p->RAnchors = RAnchors;
}

int16_t NTC_get_C(NTC_t* p, uint16_t vADC)
{
	// calc R
	uint32_t R =0;
	int16_t C = 0;
	uint32_t mV = (uint32_t)(vADC) * NTC_ADC_mVREF / FULL_SCALE_ADC_VALUE;
	switch(p->topology)
	{
	case NTC_AT_LOWER:
		if(vADC > FULL_SCALE_ADC_VALUE * 0.95)
			R = p->RAnchors[0];
		else
			R = (mV * p->R_divider) / (p->mV_divider - mV); // mV / (mVRefDivider - mV) = R_NTC / R_divider
		break;
	case NTC_AT_UPPER:
		if(vADC < FULL_SCALE_ADC_VALUE * 0.1)
			R = p->RAnchors[0];
		else
			R = (((uint32_t)p->mV_divider - mV) * p->R_divider) / mV ; // (mVRefDivider - mV) / mV= R_NTC / R_divider
		break;
	}
	int8_t i;
	for(i = p->nAnchors - 1; i >= 0; --i)
	{
		if(R < p->RAnchors[i]) break;
	}
	if(i >= p->nAnchors - 1) // temperature too high
		C = p->CAnchors[p->nAnchors - 1];
	else if(i < 0) // temperature too low
		C = p->CAnchors[0];
	else
	{
		C = p->CAnchors[i] + (uint32_t)(p->RAnchors[i] - R) * (p->CAnchors[i+1] - p->CAnchors[i])  / (p->RAnchors[i] - p->RAnchors[i+1]);
	}
	return C;
}
