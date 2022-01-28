/*
 * ICOM_CIV.h
 *
 *  Created on: 2021年3月13日
 *      Author: Zening
 */

#ifndef ICOM_CIV_H_
#define ICOM_CIV_H_

#define ICOM_CIV_LEN_DATA_BUFFER 16
#define ICOM_CIV_BYTE_START 0xFE
#define ICOM_CIV_BYTE_END 0xFD
#define ICOM_CIV_BYTE_COLLISION 0xFC
#define ICOM_CIV_CMD_FREQ 0x00


#include <stdbool.h>
#include <stdint.h>
#include "RadioTypes.h"

typedef enum
{
	ICOM_CIV_waiting_for_pre1,
	ICOM_CIV_waiting_for_pre2,
	ICOM_CIV_waiting_for_to,
	ICOM_CIV_waiting_for_from,
	ICOM_CIV_waiting_for_cmd,
	ICOM_CIV_receiving_data,
} ICOM_CIV_state_t;

typedef struct
{
	ICOM_CIV_state_t state;
	uint8_t addrTo, addrFrom, cmd;
	uint8_t data[ICOM_CIV_LEN_DATA_BUFFER];
	uint8_t idxData;
}	ICOM_CIV_t;

void ICOM_CIV_init(ICOM_CIV_t* p);

// return True if a command is parsed
bool ICOM_CIV_transfer(ICOM_CIV_t* p,  uint8_t data);

uint32_t ICOM_CIV_parse_band(ICOM_CIV_t* p);

extern ICOM_CIV_t ICOM_CIV;

#endif /* ICOM_CIV_H_ */
