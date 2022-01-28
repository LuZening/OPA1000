/*
 * Kenwood.h
 *
 *  Created on: Mar 25, 2021
 *      Author: Zening
 */

#ifndef KENWOOD_H_
#define KENWOOD_H_
#include <stdbool.h>
#include <stdint.h>

extern char KENWOOD_CMD_GET_FREQ_VFO_A[4];
#define LEN_KENWOOD_CMD 4
#define LEN_KENWOOD_DATA 12


typedef struct
{
	uint8_t idxCMD;
	char CMD[LEN_KENWOOD_CMD];
	uint8_t idxData;
	uint8_t data[LEN_KENWOOD_DATA]; // 0~9 integer numbers
	bool overflow;
	bool received;
} Kenwood_CAT_t;
extern Kenwood_CAT_t Kenwood_CAT;

void Kenwood_CAT_init(Kenwood_CAT_t* p);
bool Kenwood_CAT_transfer_state(Kenwood_CAT_t* p, char d);
// 11 digits
uint32_t Kenwood_CAT_parse_freq(Kenwood_CAT_t* p);
#endif /* KENWOOD_H_ */
