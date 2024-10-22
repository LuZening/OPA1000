/*
 * COM_task.h
 *
 *  Created on: Apr 20, 2022
 *      Author: cpholzn
 */

#ifndef COM_TASK_H_
#define COM_TASK_H_

#include "COM.h"
#include "Yaesu.h"
#include "ICOM_CIV.h"
#include "Kenwood.h"
#include "RadioTypes.h"
#include "stdint.h"
#include "stddef.h"
#include <string.h>

extern band_t BAND_decoder;


#define CAT_CMD_FREQ YAESU_CMD_GET_FREQ_VFO_A
#define COM_END_OF_COMMAND YAESU_END_OF_CMD
extern const char CAT_CMD_MONITORING[3];

size_t utoaz(uint32_t v, char* buf, int8_t nDigits);

size_t itoaz(int32_t v, char* buf, int8_t nDigits);

uint32_t my_atou(const char* buf, int8_t nMaxLen);
int32_t my_atoi(const char* buf, int8_t nMaxLen);




void StartCOMRecvTask();




/* * CAT * */

typedef struct {
	uint8_t* buf[256]; // command buffer
	size_t idxBuf; // cursor to current buffer
	uint8_t argc;
	// argv[0] points to the command, but not guaranteed to trailling 0
	uint8_t* argv[4]; // argv[0] = command, argv[1], argv[2], argv[3]
	// same as argv[0] but with static memory assigned
	char op[4];
} Command_t;
void Command_init(Command_t * p);
typedef bool *(func_command_t) (uint8_t argc, uint8_t** argv);
// parse_command alters the command string s
// command format derived from YAESU but little different
// e.g. FA014270000; // set band filter to include 14270kHz
// e.g. MN1; turn on monitor
// e.g. MN0; turn off monitor

void CAT_parse_command(char* s, Command_t* command, size_t lenbuf);

bool CAT_execute_command(const Command_t* command);

// Working status of the Amp
extern const char* MoniItemsNames[];
#define N_MONI_ITEMS (sizeof(MoniItemsNames) / sizeof(char*))

// generate the string to represent the working status of the amp
// bufsize: at least name[10]=val[9]; x 8+1 = 21x8+1+1 = 169
size_t OPA_get_moni_info(char* buf);

/* * End of CAT * */
#endif /* COM_TASK_H_ */
