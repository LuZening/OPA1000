
#ifndef __PERSISTENT_H_
#define __PERSISTENT_H_


#include <stdbool.h>
#include "touch_HR2046.h"
#include "RadioTypes.h"
#include "band.h"

#define THREAD_FLAG_SAVE_CONFIG 0x01
#define VALIDATE_CODE "OPA1000"
/* Pust persistent variables here*/
// Load at power up
// Save when required
typedef struct
{
	// Data Integrity Validator
	char validator[8];
	// Touch Screen Calibration Info
	TouchScreenCalibrationInfo_t TSCalibInfo;
	// Band Source selection last time
	uint8_t Band_source;
	// Software Band before shutdown
	band_t Band_soft;
	// Fan1 speed
	bool Fan1Auto;
	uint8_t Fan1Speed;
	// Fan2 speed
	bool Fan2Auto;
	uint8_t Fan2Speed;
	// CAT Baud
	uint16_t baudCAT;
	// Band Baud
	uint16_t baudBand;
	// Band Mode
	BandMode_t bandMode;
} PersistentVars_t;

void init_config(PersistentVars_t* p);
bool isPersistentVarsValid(const PersistentVars_t* p);

extern PersistentVars_t cfg;
extern bool isCfgValid;

#endif
