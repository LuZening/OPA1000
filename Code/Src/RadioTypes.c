/*
 * RadioTypes.c
 *
 *  Created on: Mar 25, 2021
 *      Author: Zening
 */


#include "RadioTypes.h"

band_t freq_to_band(uint32_t freqHz)
{
	band_t band = BAND_15M_10M;
	if(freqHz < 1800000)
		band = BAND_FAULT;
	if(freqHz < 2000000)
		band = BAND_160M;
	else if(freqHz < 3900000)
		band = BAND_80M;
	else if(freqHz < 10150000)
		band = BAND_40M_30M;
	else if(freqHz < 18168000)
		band = BAND_20M_17M;
	else if(freqHz < 29600000)
		band = BAND_15M_10M;
	else
		band = BAND_FAULT;
	return band;
}
