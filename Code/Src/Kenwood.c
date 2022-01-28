/*
 * Kenwood.c
 *
 *  Created on: Mar 25, 2021
 *      Author: Zening
 */


#include "Kenwood.h"
//TODO: correct Kenwood protocol
char KENWOOD_CMD_GET_FREQ_VFO_A[4] = "FA";

Kenwood_CAT_t Kenwood_CAT = {.received = false, .overflow = false,.idxCMD = 0, .idxData = 0};


void Kenwood_CAT_init(Kenwood_CAT_t* p)
{
	p->overflow = false;
	p->received = false;
	p->idxCMD = 0;
	p->idxData = 0;
}

bool Kenwood_CAT_transfer_state(Kenwood_CAT_t* p, char d)
{
	bool r = false;
	if(d == ';') // End of transmission
	{
		r = !p->overflow; // return true if buffer has not been overflowed
		p->received = r;
		if(p->overflow)
			Kenwood_CAT_init(p);
	}
	else if(d >= '0' && d <= '9') // data
	{
		if(p->idxData < LEN_KENWOOD_DATA)
			p->data[p->idxData++] = (d - '0');
		else
			p->overflow = true;
	}
	else // command
	{
		if(p->idxCMD < LEN_KENWOOD_CMD)
			p->CMD[p->idxCMD++] = d;
		else
			p->overflow = true;
	}
	return r;
}


uint32_t Kenwood_CAT_parse_freq(Kenwood_CAT_t* p)
{
	uint32_t freq = 0;
	if(p->idxData == 11)
	{
		for(int8_t i = 0; i < 11; ++i)
		{
			freq = 10 * freq + p->data[i];
		}
	}
	return freq;
}
