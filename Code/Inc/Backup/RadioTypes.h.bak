/*
 * RadioTypes.h
 *
 *  Created on: 2020年6月21日
 *      Author: Zening
 */

#ifndef RADIOTYPES_H_
#define RADIOTYPES_H_

#include "stdint.h"

typedef enum {
	STANDBY = 0,
	RECEIVING,
	TRANSMITTING
} Transmission_State_t;

// Band source
#define BAND_FROM_SWITCH 0
#define BAND_FROM_REMOTE 1
#define BAND_FROM_DECODER 2


typedef enum {
	BAND_160M = 0,
	BAND_80M = 1,
	BAND_40M_30M = 2,
	BAND_20M_17M = 3,
	BAND_15M_10M = 4,
} band_t;
#define N_BANDS 5

band_t freq_to_band(uint32_t freqHz);

#define N_BAND_MODES 3
typedef enum
{
	BAND_MODE_ICOM = 0,
	BAND_MODE_YAESU,
	BAND_MODE_KENWOOD
} BandMode_t;
#endif /* RADIOTYPES_H_ */
