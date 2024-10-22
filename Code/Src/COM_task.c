/*
 * COM_task.c
 *
 *  Created on: Apr 20, 2022
 *      Author: cpholzn
 */


#include "main.h"
#include "COM_task.h"
#include "SoftUART.h"
#include "LVGL_GUI.h"
const char CAT_CMD_MONITORING[3] = "MN";
// Working status of the Amp
const char* MoniItemsNames[] = {
		"IMAIN_mA","VMAIN_mV","FWDPWR_mW","REVPWR_mW",
		"SWR_deci","TEMP1_deci", "TEMP2_deci", "FREQ_Hz"
};
int32_t MoniItemsIntValues[N_MONI_ITEMS] = {
		0
};
extern UART_HandleTypeDef huart1;
size_t utoaz(uint32_t v, char* buf, int8_t nDigits)
{
	size_t i = 0;
	while(v || (nDigits > 0))
	{
		buf[i++] = (v % 10) + '0';
		v /= 10;
		nDigits--;
	}
	uint8_t j = 0;
	char c;
	for(j=0; j < i / 2; ++j)
	{
		c = buf[j];
		buf[j] = buf[i - j - 1];
		buf[i - j - 1] = c;
	}
	buf[i] = 0;
	return i;
}

size_t itoaz(int32_t v, char* buf, int8_t nDigits)
{
	size_t i = 0;
	bool isNeg =false;
	if(v < 0)
	{
	    v = -v;
	    isNeg = true;
	}
	while((v != 0) || (nDigits > 0))
	{
		buf[i++] = (((uint32_t)v) % 10) + '0';
		v /= 10;
		nDigits--;
	}
    if(isNeg) buf[i++] = '-';
	uint8_t j = 0;
	char c;
	for(j=0; j < i / 2; ++j)
	{
		c = buf[j];
		buf[j] = buf[i - j - 1];
		buf[i - j - 1] = c;
	}
	buf[i] = 0;
	return i;
}

uint32_t my_atou(const char* buf, int8_t nMaxLen)
{
	const char* p = buf;
	uint32_t v = 0;
	while(*p && (nMaxLen > 0))
	{
		v = v * 10 + (*p - '0');
		p++;
		nMaxLen--;
	}
	return v;
}

int32_t my_atoi(const char* buf, int8_t nMaxLen)
{
	const char* p = buf;
	int32_t v = 0;
	bool isNeg;
	if(*p == '-')
	{
		isNeg = true;
		p++;
	}
	while(*p && (nMaxLen > 0))
	{
		v = v * 10 + (*p - '0');
		p++;
		nMaxLen--;
	}
	return (isNeg ? (-v) : v);
}

band_t BAND_decoder = BAND_15M_10M;

uint8_t command_buffer[COM_BUFFER_LEN] = {0};
uint8_t idx_command = 0;
Command_t command;
bool is_moni_start = false;



/*************************
 * Command Processors
 * ***********************/
static Command_t Command;
void Command_init(Command_t * p)
{
	p->idxBuf = 0;
	p->argc = 0;
	*(p->op) = 0;
}
// query current band or set band
// format: band\r
// format: band 7000\r // which is 7 meters, the unit is mm
bool command_band(uint8_t argc, const uint8_t** argv)
{
	if(argc == 1) // return band info if no parameters
	{
		const char* band_name;
		osMutexAcquire(mtxBandDataHandle, 0);
		switch(cfg.Band_source)
		{
		case BAND_FROM_SWITCH:
			band_name = convert_Band_to_Name(BAND_switch);
			break;
		case BAND_FROM_REMOTE:
			band_name = convert_Band_to_Name(cfg.Band_soft);
			break;
		default:
			band_name = convert_Band_to_Name(BAND_decoder);
		}
		COM_send_message(&COM1, band_name, strlen(band_name));
		osMutexRelease(mtxBandDataHandle);
		return true;
	}
	else if(argc == 2) // set band if with 1 parameter
	{
		const char* param = argv[1]; // param is band name
		while(*param)
		{
			if((*param < '0') || (*param > '9'))
			{
				return false;
			}
			++param;
		}
		uint32_t n_band_mm = my_atoi(param, 7);
		band_t band_new;
		if(n_band_mm >= 10000 && n_band_mm <= 15000)
			band_new = BAND_15M_10M;
		else if(n_band_mm >= 17000 && n_band_mm <= 20000)
			band_new = BAND_20M_17M;
		else if(n_band_mm >= 30000 && n_band_mm <= 40000)
			band_new = BAND_40M_30M;
		else if(n_band_mm == 80000)
			band_new = BAND_80M;
		else if(n_band_mm >= 160000)
			band_new = BAND_160M;
		else
			return false;
		// change Band settings
		osMutexAcquire(mtxBandDataHandle, 0);
		cfg.Band_source = BAND_FROM_REMOTE;
		cfg.Band_soft = band_new;
		osMutexRelease(mtxBandDataHandle);
		return true;
	}
	else
		return false;
}

// Turn on or turn off monitoring
// format: moni\r // return moni state, "on\r" or "off\r"
// format: moni on\r // which is 7 meters, the unit is mm
bool command_moni(uint8_t argc, uint8_t** argv)
{
	if(argc < 2) return false;
	const uint8_t* param = argv[1];
	if(strcasecmp(param, "on") == 0)
	{
		is_moni_start = true;
	}
	else if(strcasecmp(param, "off") == 0)
	{
		is_moni_start = false;
	}
	else
		return false;
	return true;
}

/*
 * IMAIN_mA=31212;VMAIN_mV=51000;
 */
size_t OPA_get_moni_info(char* buf)
{
	uint8_t i, j;
	char* p = buf;
	for(i=0; i<N_MONI_ITEMS; ++i)
	{
		const char* sName = MoniItemsNames[i];
		uint8_t lenName = strlen(sName);
		strcpy(p, sName);
		p += lenName;
		*(p++) = ' ';
		p += itoaz(MoniItemsIntValues[i], p, 1);
		*(p++) = COM_END_OF_COMMAND;
	}
	*p = 0;
	return (p-buf);
}

// parse_command alters the command string s
void CAT_parse_command(char* s, Command_t* command, size_t lenbuf)
{
	char* p = s;
	char* send = s + lenbuf - 1;
	// first leading letters form command operator
	command->argc = 0;
	command->argv[0] = s;
	// find first non letter, which sperates command
	while(*p >= 'A' && *p <= 'Z' && *p && (p < send))
	{
		p++;
	}
	uint8_t lenOp = p -s;
	strncpy(command->op, s, lenOp);
	command->op[lenOp] = '\0';
	if(p < send)
	{
		char* tok = strtok(p, " ");
		if(tok)
		{
			command->argc = 1;
			command->argv[command->argc++] = tok;
		}
		while((tok = strtok(NULL, " ")))
		{
			command->argv[command->argc++] = tok;
		}
	}

}

bool CAT_execute_command(const Command_t* command)
{
	if(command->argc < 1) return false;
	const char* cmd = command->argv[0];
	if(strcasecmp(cmd, CAT_CMD_FREQ) == 0)
	{
		if(!command_band(command->argc, command->argv))
			goto BAD_COMMAND;
	}
	// start/stop monitoring the sensor values
	else if(strcasecmp(cmd, CAT_CMD_MONITORING) == 0)
	{
		if(!command_moni(command->argc, command->argv))
			goto BAD_COMMAND;
	}
	else
	{
		goto BAD_COMMAND;
	}
BAD_COMMAND:
	return false;
}


// TODO: define band decoder mode infer
BandMode_t infer_band_mode(uint8_t* buf, uint16_t n)
{
	BandMode_t r = BAND_MODE_AUTO; // if infer failed, return BAND_MODE_AUTO

	return r;
}

void StartCOMRecvTask()
{
	// TODO: COM Task
//	uint32_t tick = osKernelGetTickCount();
	const uint8_t lenBuf = SoftUartRxBufferSize;
	uint8_t buf[lenBuf];
	/* Init software UART */
	uint16_t baudBand_old = 0;
	uint16_t baudCAT_old = 0;
	BandMode_t bandMode_old = 0;
	/* Initialized in main.c
	  // Software UART - for band decoder
	  SoftUartInit(0, BAND_TX_GPIO_Port, BAND_TX_Pin, BAND_RX_GPIO_Port, BAND_RX_Pin, NULL);
	  SoftUartStart();
	  SoftUartEnableRx(0);
	*/
	if(cfg.baudCAT > 19200)
		cfg.baudCAT = 19200;
	if(cfg.baudBand > 19200)
		cfg.baudBand = 19200;
	// single byte read buf
	char c;
	Command_init(&Command);
	for(;;)
	{
		/* Check baudrate changes */
		if(cfg.baudBand != baudBand_old || cfg.baudCAT != baudCAT_old || cfg.bandMode != bandMode_old)
		{
			bandMode_old = cfg.bandMode;
			osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
			GUI_set_band_decoder_info(255); // clear band decoder info display
			osMutexRelease(mtxGUIWidgetsHandle);
		}
		if(cfg.baudBand != baudBand_old)
		{
			baudBand_old = cfg.baudBand;
			SoftUartChangeBaud(cfg.baudBand);
		}
		if(cfg.baudCAT != baudCAT_old)
		{
			baudCAT_old = cfg.baudCAT;
			HAL_UART_DeInit(&huart1);
			huart1.Instance->BRR = cfg.baudBand;
			HAL_UART_Init(&huart1);
		}
		/* Send frequency request */
//		if(cfg.bandMode == BAND_MODE_YAESU || cfg.bandMode == BAND_MODE_KENWOOD)
//		{
//			bool needBaudResume = false;
//			if(cfg.baudCAT != cfg.baudBand)
//			{
//				needBaudResume = true;
//				HAL_GPIO_WritePin(BAND_TX_GPIO_Port, BAND_TX_Pin, GPIO_PIN_SET);
//				// temporarily Switch to USART1 baudrate to CIV baudrate
//				HAL_UART_DeInit(&huart1);
//				huart1.Instance->BRR = cfg.baudBand;
//				HAL_UART_Init(&huart1);
//			}
//			if(cfg.bandMode == BAND_MODE_YAESU)
//			{
//				HAL_UART_Transmit(&huart1, "FA;", 3, 10);
//			}
//			else if(cfg.bandMode == BAND_MODE_KENWOOD)
//			{
//				HAL_UART_Transmit(&huart1, "FA;", 3, 10);
//			}
//			// resume CAT baudrate
//			if(needBaudResume)
//			{
//				HAL_UART_DeInit(&huart1);
//				huart1.Instance->BRR = cfg.baudCAT;
//				HAL_UART_Init(&huart1);
//			}
//		}
		/* Check CIV or received band response */
		// Delay 300ms to wait for responses
		// osDelay(300);
		bool isFreqRspnReceived = false;
		uint8_t lenRecved = SoftUartReadRxBuffer(0, buf, lenBuf);
		// Guess band mode from received buffer to check if config is correct
		BandMode_t bandModeInfer = BAND_MODE_AUTO;
		bandModeInfer = infer_band_mode(buf, lenRecved);
		if (bandModeInfer != BAND_MODE_AUTO) // infer succeeded
			cfg.bandMode = bandModeInfer;

		switch(cfg.bandMode)
		{
		case BAND_MODE_ICOM:
			for(uint8_t i=0; i<lenRecved; ++i)
			{
				if(ICOM_CIV_transfer(&ICOM_CIV, buf[i]))
				{
					if(ICOM_CIV.cmd == ICOM_CIV_CMD_FREQ)
					{
						// switch band
						uint32_t freqHz = ICOM_CIV_parse_band(&ICOM_CIV);
						if(freqHz > 1000000) // NG below 1MHz
						{
							osMutexAcquire(mtxBandDataHandle, osWaitForever);
							cfg.Band_source = BAND_FROM_DECODER;
							BAND_decoder = freq_to_band(freqHz);
							osMutexRelease(mtxBandDataHandle);
							isFreqRspnReceived = true;
						}
						else
						{
							osMutexAcquire(mtxBandDataHandle, osWaitForever);
							cfg.Band_source = BAND_FROM_DECODER;
							BAND_decoder = freq_to_band(freqHz);
							osMutexRelease(mtxBandDataHandle);
							isFreqRspnReceived = true;
						}
					}
				}
			}
			break;
		case BAND_MODE_YAESU:
			for(uint8_t i=0; i<lenRecved; ++i)
			{
				if(Yaesu_CAT_transfer_state(&Yaesu_CAT, buf[i]))
				{
					if(strncmp(Yaesu_CAT.CMD, YAESU_CMD_GET_FREQ_VFO_A, Yaesu_CAT.idxCMD) == 0)
					{
						uint32_t freqHz = Yaesu_parse_freq(&Yaesu_CAT);
						if(freqHz)
						{
							osMutexAcquire(mtxBandDataHandle, osWaitForever);
							cfg.Band_source = BAND_FROM_DECODER;
							BAND_decoder = freq_to_band(freqHz);
							osMutexRelease(mtxBandDataHandle);
							isFreqRspnReceived = true;
						}
					}
					Yaesu_CAT_init(&Yaesu_CAT);
				}
			}
			break;
		case BAND_MODE_KENWOOD:
			for(uint8_t i=0; i<lenRecved; ++i)
			{
				if(Kenwood_CAT_transfer_state(&Kenwood_CAT, buf[i]))
				{
					if(strncmp(Kenwood_CAT.CMD, KENWOOD_CMD_GET_FREQ_VFO_A, Kenwood_CAT.idxCMD) == 0)
					{
						uint32_t freqHz = Kenwood_CAT_parse_freq(&Kenwood_CAT);
						if(freqHz)
						{
							osMutexAcquire(mtxBandDataHandle, osWaitForever);
							cfg.Band_source = BAND_FROM_DECODER;

							band_t newband = freq_to_band(freqHz);
							if(newband != BAND_FAULT)
								BAND_decoder = newband;
							osMutexRelease(mtxBandDataHandle);
							isFreqRspnReceived = true;
						}
					}
					Kenwood_CAT_init(&Kenwood_CAT);
				}
			}
			break;
		}
		// Show Auto band sign on screen
		if(isFreqRspnReceived)
		{
			osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
			GUI_set_band_decoder_info(cfg.bandMode);
			osMutexRelease(mtxGUIWidgetsHandle);
		}

		/*TODO: Check Hardware UART CAT control from PC */
		// single byte each time

		while(COM_read_message(&COM1, &c, 1))
		{
			//if()
			Command.buf[Command.idxBuf++] = c;

		}
		osDelay(100);
	}
}


