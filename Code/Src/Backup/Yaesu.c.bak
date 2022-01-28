/*
 * Yaesu.c
 *
 *  Created on: Mar 25, 2021
 *      Author: Zening
 */


#include "Yaesu.h"

char YAESU_CMD_GET_FREQ_VFO_A[4] = "FA";

Yaesu_CAT_t Yaesu_CAT = {.received = false, .overflow = false, .idxCMD = 0, .idxData = 0};


void Yaesu_CAT_init(Yaesu_CAT_t* p)
{
	p->received = false;
	p->overflow = false;
	p->idxCMD = 0;
	p->idxData = 0;
}

bool Yaesu_CAT_transfer_state(Yaesu_CAT_t* p, char d)
{
	bool r = false;
	if(d == ';') // End of transmission
	{
		r = !p->overflow; // return true if buffer has not been overflowed
		p->received = r;
		if(p->overflow)
			Yaesu_CAT_init(p);
	}
	else if(d >= '0' && d <= '9') // data
	{
		if(p->idxData < LEN_YAESU_DATA)
			p->data[p->idxData++] = (d - '0');
		else
			p->overflow = true;
	}
	else // command
	{
		if(p->idxCMD < LEN_YAESU_CMD)
			p->CMD[p->idxCMD++] = d;
		else
			p->overflow = true;
	}

	return r;
}

uint32_t Yaesu_parse_freq(Yaesu_CAT_t* p)
{
	uint32_t freq = 0;
	if(p->idxData == 8)
	{
		for(int8_t i = 0; i < 8; ++i)
		{
			freq = 10 * freq + p->data[i];
		}
	}
	freq *= 1000; // kHz to Hz
	return freq;
}
