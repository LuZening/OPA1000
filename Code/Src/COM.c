/*
 * COM.c
 *
 *  Created on: Mar 13, 2020
 *      Author: Zening
 */


#include "main.h"
#include "COM.h"
#include "Sensor_task.h"
#include "string.h"
#include "stdlib.h"
#include "ICOM_CIV.h"
#include "Yaesu.h"
#include "Kenwood.h"
#include "SoftUART.h"
#include "LVGL_GUI.h"
#define COM_BUFFER_LEN 128

band_t BAND_decoder = BAND_15M_10M;

uint8_t command_buffer[COM_BUFFER_LEN] = {0};
uint8_t idx_command = 0;
Command_t command;
bool is_moni_start = false;

typedef bool *(func_command_t) (uint8_t argc, uint8_t** argv);

/*************************
 * Commands
 * ***********************/
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
		COM_send_message(band_name);
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
		uint8_t n_band_meters = atoi(param);
		band_t band_new;
		if(n_band_meters >= 10 && n_band_meters <= 15)
			band_new = BAND_15M_10M;
		else if(n_band_meters >= 17 && n_band_meters <= 20)
			band_new = BAND_20M_17M;
		else if(n_band_meters >= 30 && n_band_meters <= 40)
			band_new = BAND_40M_30M;
		else if(n_band_meters == 80)
			band_new = BAND_80M;
		else if(n_band_meters == 160)
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

bool command_moni(uint8_t argc, uint8_t** argv)
{
	if(argc < 2) return false;
	const uint8_t* param = argv[1];
	if(strcasecmp(param, "start") == 0)
	{
		is_moni_start = true;
	}
	else if(strcasecmp(param, "stop") == 0)
	{
		is_moni_start = false;
	}
	else
		return false;
	return true;
}

// parse_command alters the command string s
void parse_command(char* s, Command_t* command)
{
	command->argv[command->argc++] = strtok(s, " ");
	char* tok;
	while((tok = strtok(NULL, " ")))
	{
		command->argv[command->argc++] = tok;
	}
}

bool execute_command(const Command_t* command)
{
	if(command->argc < 1) return false;
	const char* cmd = command->argv[0];
	if(strcasecmp(cmd, "band") == 0)
	{
		if(!command_band(command->argc, command->argv))
			goto BAD_COMMAND;
	}
	// start/stop monitoring the sensor values
	else if(strcasecmp(cmd, "moni") == 0)
	{
//		if()
	}
	else if(strcasecmp(cmd, "state") == 0)
	{

	}
	else
	{
		goto BAD_COMMAND;
	}
BAD_COMMAND:
	return false;
}

void COM_send_message(const char* s)
{
	if(*s)
	{
		while(*s)
		{
			osMessageQueuePut(COMBytesToSendQHandle, s, 0, 0);
			++s;
		}
		COM_ENABLE_TXE(huart1);
	}
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
	BandMode_t bandMode_old = BAND_MODE_ICOM;
	SoftUartInit(0, BAND_GPIO_Port, BAND_Pin, BAND_GPIO_Port, BAND_Pin, NULL);
	SoftUartStart();
	SoftUartEnableRx(0);
	if(cfg.baudCAT > 9600)
		cfg.baudCAT = 9600;
	if(cfg.baudBand > 9600)
		cfg.baudBand = 9600;

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
		if(cfg.bandMode == BAND_MODE_YAESU || cfg.bandMode == BAND_MODE_KENWOOD)
		{
			bool needBaudResume = false;
			if(cfg.baudCAT != cfg.baudBand)
			{
				needBaudResume = true;
				HAL_GPIO_WritePin(BAND_GPIO_Port, BAND_Pin, GPIO_PIN_SET);
				// temporarily Switch to USART1 baudrate to CIV baudrate
				HAL_UART_DeInit(&huart1);
				huart1.Instance->BRR = cfg.baudBand;
				HAL_UART_Init(&huart1);
			}
			if(cfg.bandMode == BAND_MODE_YAESU)
			{
				HAL_UART_Transmit(&huart1, "FA;", 3, 10);
			}
			else if(cfg.bandMode == BAND_MODE_KENWOOD)
			{
				HAL_UART_Transmit(&huart1, "FA;", 3, 10);
			}
			// resume CAT baudrate
			if(needBaudResume)
			{
				HAL_UART_DeInit(&huart1);
				huart1.Instance->BRR = cfg.baudCAT;
				HAL_UART_Init(&huart1);
			}
		}
		/* Check CIV or received band response */
		// Delay 300ms to wait for responses
		osDelay(300);
		bool isFreqRspnReceived = false;
		uint8_t lenRecved = SoftUartReadRxBuffer(0, buf, lenBuf);
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
						if(freqHz)
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
							BAND_decoder = freq_to_band(freqHz);
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

		/*TODO: Check CAT */


		osDelay(100);
	}
}


