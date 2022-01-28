/*
 * ICOM_CIV.c
 *
 *  Created on: 2021年3月13日
 *      Author: Zening
 */

#include "ICOM_CIV.h"

ICOM_CIV_t ICOM_CIV = {.state = ICOM_CIV_waiting_for_pre1, .idxData = 0};

void ICOM_CIV_init(ICOM_CIV_t* p)
{
	p->state = ICOM_CIV_waiting_for_pre1;
	p->idxData = 0;
}

// return True if a command is parsed
// 0xFE 0xFE TO FROM CMD(freq:0) DATA DATA DATA ... 0xFD(end)
bool ICOM_CIV_transfer(ICOM_CIV_t* p, uint8_t data)
{
	bool r = false;
	switch(p->state)
	{
	case ICOM_CIV_waiting_for_pre1:
		if(data == ICOM_CIV_BYTE_START)
		{
			p->state = ICOM_CIV_waiting_for_pre2;
			p->idxData = 0;
		}
		break;
	case ICOM_CIV_waiting_for_pre2:
		if(data == ICOM_CIV_BYTE_START)
			p->state = ICOM_CIV_waiting_for_to;
		else
			p->state = ICOM_CIV_waiting_for_pre1;
		break;
	case ICOM_CIV_waiting_for_to:
		if(data == ICOM_CIV_BYTE_COLLISION)
			p->state = ICOM_CIV_waiting_for_pre1;
		else
		{
			p->state = ICOM_CIV_waiting_for_from;
			p->addrTo = data;
		}
		break;
	case ICOM_CIV_waiting_for_from:
		if(data == ICOM_CIV_BYTE_COLLISION)
			p->state = ICOM_CIV_waiting_for_pre1;
		else // OK
		{
			p->state = ICOM_CIV_waiting_for_cmd;
			p->addrFrom = data;
		}
		break;
	case ICOM_CIV_waiting_for_cmd:
		if(data == ICOM_CIV_BYTE_COLLISION)
			p->state = ICOM_CIV_waiting_for_pre1;
		else // OK
		{
			p->state = ICOM_CIV_receiving_data;
			p->cmd = data;
		}
		break;
	case ICOM_CIV_receiving_data:
		if(data == ICOM_CIV_BYTE_COLLISION)
			p->state = ICOM_CIV_waiting_for_pre1;
		else if(data == ICOM_CIV_BYTE_END) // Transmission finished
		{
			p->state = ICOM_CIV_waiting_for_pre1;
			r = true; // command succesfully received
		}
		else // OK
		{
			if(p->idxData < ICOM_CIV_LEN_DATA_BUFFER)
			{
				p->data[p->idxData++] = data;
			}
			else
			{
				p->state = ICOM_CIV_waiting_for_pre1;
			}
		}
		break;
	}
	return r;
}

uint32_t ICOM_CIV_parse_band(ICOM_CIV_t* p)
{
	// data 5 Bytes
	// 14273.551 kHz -> 0x51 0x35 0x27 0x14 0x00
	// 2413752.551 kHz (2.4GHz) -> 0x51 0x25 0x75 0x13 0x24
	uint32_t freqHz = 0; // at most 4GHz
	for(int8_t i = p->idxData - 1; i >= 0;  --i)
	{
		uint8_t d = p->data[i];
		uint8_t dL = (d & 0x0f);
		uint8_t dH = d >> 4;
		if (dH >= 0x0A || dL >= 0x0A)
			return 0;
		freqHz = freqHz * 100 + dH * 10 + dL;
	}
	return freqHz;
}
