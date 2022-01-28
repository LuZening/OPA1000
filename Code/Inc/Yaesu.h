/*
 * Yaesu.h
 *
 *  Created on: Mar 25, 2021
 *      Author: Zening
 */

#ifndef YAESU_H_
#define YAESU_H_

#include <stdint.h>
#include <stdbool.h>
#define LEN_YAESU_CMD 4
#define LEN_YAESU_DATA 12

extern char YAESU_CMD_GET_FREQ_VFO_A[4];

typedef struct
{
	uint8_t idxCMD;
	char CMD[LEN_YAESU_CMD];
	uint8_t idxData;
	uint8_t data[LEN_YAESU_DATA]; // 0~9 integer numbers
	bool overflow;
	bool received;
} Yaesu_CAT_t;

extern Yaesu_CAT_t Yaesu_CAT;

void Yaesu_CAT_init(Yaesu_CAT_t* p);
bool Yaesu_CAT_transfer_state(Yaesu_CAT_t* p, char d);
// Yaesu radios report frequency in kHz
uint32_t Yaesu_parse_freq(Yaesu_CAT_t* p);

#endif /* YAESU_H_ */
