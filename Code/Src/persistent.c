/*
 * persistent.c
 *
 *  Created on: Jul 1, 2020
 *      Author: Zening
 */

#include "persistent.h"
#include "string.h"


void init_config(PersistentVars_t* p)
{
	init_touch_screen_calib_info(&p->TSCalibInfo);
	strncpy(p->validator, VALIDATE_CODE, sizeof(VALIDATE_CODE));
	p->Band_source = BAND_FROM_REMOTE;
	p->Band_soft = BAND_15M_10M;
	p->Fan1Auto = true;
	p->Fan1Speed = 0;
	p->Fan2Auto = true;
	p->Fan2Speed = 0;
	p->baudCAT = 9600;
	p->baudBand = 1200;
	p->bandMode = BAND_MODE_ICOM;
	p->PTT_delay_ms = 10;
}

bool isPersistentVarsValid(const PersistentVars_t* p)
{
	return strncmp(p->validator, VALIDATE_CODE, sizeof(VALIDATE_CODE)-1) == 0;
}
