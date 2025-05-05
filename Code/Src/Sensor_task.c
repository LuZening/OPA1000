/*
 * Sensor_task.c
 *
 *  Created on: Mar 2, 2020
 *      Author: Zening
 */

#include "main.h"
#include "FreeRTOS.h"
#include "task.h"
#include "Sensor_task.h"
#include "math.h"
//#include "GUI_task.h"
#include "stdlib.h"
#include "string.h"
#include "LVGL_GUI.h"
#include "src/lv_core/lv_debug.h"
#include "SoftPWMDriver.h"
////extern DMA_HandleTypeDef hdma_adc1;
//extern DMA_HandleTypeDef hdma_adc3;
extern ADC_HandleTypeDef hadc1;
extern ADC_HandleTypeDef hadc3;
extern TIM_HandleTypeDef htim2;

// TODO: Calibration Options
const float ICOM_BANDCODE_VOLTAGES[N_BANDCODE_VOLTAGES] = {
		7.5f, // 160m
		6.6f, // 80m
		6.0f, // 40m
		0.f , // 0+-err 30m
		5.0f, // 20m
		4.3f, // 4.5+-err: 15m
		3.5f, // 3.5+-err: 10m
};


const char* BANDNAME_160M = "160m";
const char* BANDNAME_80M = "80m";
const char* BANDNAME_40M_30M = "40-30m";
const char* BANDNAME_20M_17M = "20-17m";
const char* BANDNAME_15M_10M = "15-10m";

/*** ADC DMA Buffer ***/
uint16_t uhADC1ConvertedValues[ADC1_N_CHANNELS];
bool ADC1DataReady = false;
//uint16_t uhADC2ConvertedValues[ADC2_N_OVERSAMPLING][ADC2_N_CHANNELS];
//bool ADC2DataReady = false;
uint16_t uhADC3ConvertedValues[ADC3_N_CHANNELS];
bool ADC3DataReady = false;


/*** Sensor Processed Raw values	***/
// Directional Coupler Attenuator resistors
#define Z0 50.
#define DC_RUPPER 330.
#define DC_RLOWER 100.
#define DC_TURNS 31
#define DC_ATTEN_RATIO  ((DC_RUPPER + DC_RLOWER) / DC_RLOWER)
const float Vf = 0.3; // drop down voltage of the RF rectifier diode BAT54



// ADC1 channel order:
#define IDX_FWD1 0

#define IDX_FWDpeak1 1

#define IDX_REVpeak1 2

#define IDX_REV1 3

#define IDX_FWD2 4

#define IDX_FWDpeak2 5

#define IDX_REVpeak2 6

#define IDX_REV2 7

//
// ADC1_3: FWD1 forward power 1
uint32_t FWD1_raw;
int FWD1 = 0; // Unit 1 Watt


// ADC1_4: FWDpeak1 forward power 1 peak value
uint32_t FWDpeak1_raw;
int FWDpeak1 = 0; // Unit 1 Watt

// ADC1_5: REVpeak1 reverse power 1 peak value
uint32_t REVpeak1_raw;
int REVpeak1 = 0;

// ADC1_6: REV1 reverse power 1 peak value

uint32_t REV1_raw;
int REV1 = 0;

// ADC1_7:FWD2

uint32_t FWD2_raw;
int FWD2 = 0;

// ADC1_8: FWDpeak2

uint32_t FWDpeak2_raw;
int FWDpeak2 = 0;

// ADC1_9: REVpeak2

uint32_t REVpeak2_raw;
int REVpeak2 = 0;

// ADC1_14: REV2

uint32_t REV2_raw;
int REV2 = 0;


// ADC3 channel order
#define IDX_band 0

#define IDX_Vmain 1

#define IDX_Imain 2

#define IDX_Temp1 3

#define IDX_Temp2 4

// ADC3_1: band
uint16_t Band_decoder_raw;

// ADC3_10: Vmain
uint32_t Vmain_raw;
int Vmain = 0; // unit V

// ADC3_11: Imain
uint32_t Imain_raw;
uint16_t Imain_ADC0 = 0; // Imain center ADC value (ADC value at 0A)
int Imain = 0; // unit mA

// ADC3_12: Temperature1
uint32_t Temp1_raw;
int Temp1 = 0; // core temperature to 0.1C

// ADC3_13: Temperature2
uint32_t Temp2_raw;
int Temp2 = 0; // ambient to 0.1C

PeakDetector PD_Vmain;
PeakDetector PD_Imain;
PeakDetector PD_FWD2;
PeakDetector PD_REV2;
PeakDetector PD_SWR2;
PeakDetector PD_Temp1;
PeakDetector PD_Temp2;

// Temperature sensing NTC
NTC_t NTC1, NTC2;


static void init_NTC()
{
	const static int16_t NTC_CAnchors[] = {-20, 0, 25, 40, 60, 70, 80, 90, 100};
	const static uint32_t NTC_RAnchors[] = {89682, 32116, 10000, 5318, 2466, 1725, 1228, 890, 657};
	const static uint8_t N_NTC_ANCHORS = 9;
	NTC_init(&NTC1, NTC_AT_LOWER, 5000, 3300, 10000, 3950, N_NTC_ANCHORS, NTC_CAnchors, NTC_RAnchors);
	NTC_init(&NTC2, NTC_AT_LOWER, 5000, 3300, 10000, 3950, N_NTC_ANCHORS, NTC_CAnchors, NTC_RAnchors);
}


void init_sensors()
{
	init_NTC();
}

// Convert ADC readings to real Forward Power
int convert_ADC2FwdPower1(uint32_t d)
{
#ifdef LOG_DETECTOR
	float v = (d / 4096.f * MAX_ADC_VOLTAGE);
	// ADC8307 0 dBm = 2.25V
	float dBm = ((v - 2.25) / 0.025f);
	dBm += 53.45; // Compensate for the attenuation
	// dBm to Watt
	return (int)(powf(10.f, dBm / 10.f) / 1000.f);
#else
	float VRF = 0.;
	if (d >= 100)
	{
		// 100W : d = 560
		// 200W : d = 1000
		// 250W : d = 1100
		// 400W : d = 1400
		VRF = (d / 4095.f * MAX_ADC_VOLTAGE + 0.3) * DC_ATTEN_RATIO * DC_TURNS;
	}

	return (int)((VRF * VRF) / Z0 / 2.); // P = U^2/R
#endif
}


int convert_FwdPower12ADC(uint32_t adc)
{
	return 1000;
}



int convert_ADC2FwdPower2(uint32_t d)
{
	return convert_ADC2FwdPower1(d);
}

band_t convert_ADC2Band(uint32_t d)
{
	int i = 0;
	float v = d / 4096.f * MAX_ADC_VOLTAGE;
	v *= 5.7; // attenuated by 1K and 4.7K Voltage divider
	for(i=0; i < N_BANDCODE_VOLTAGES; ++i)
	{
		float err = v - ICOM_BANDCODE_VOLTAGES[i];
		if(err > -BANDCODE_VOLTAGE_TOLERANCE && err < BANDCODE_VOLTAGE_TOLERANCE)
			break;
	}
	switch(i)
	{
	case 0: return BAND_160M;
	case 1: return BAND_80M;
	case 2: return BAND_40M_30M;
	case 3: return BAND_40M_30M;
	case 4: return BAND_20M_17M;
	case 5: return BAND_15M_10M;
	case 6: return BAND_15M_10M;
	default: return BAND_15M_10M;
	}
}

const char* convert_Band_to_Name(band_t band)
{
	switch (band)
	{
	case BAND_160M:
		return BANDNAME_160M; break;
	case BAND_80M:
		return BANDNAME_80M; break;
	case BAND_40M_30M:
		return BANDNAME_40M_30M; break;
	case BAND_20M_17M:
		return BANDNAME_20M_17M; break;
	case BAND_15M_10M:
		return BANDNAME_15M_10M; break;
	default:
		return BANDNAME_15M_10M;
	}
}


int convert_ADC2Vmain(uint32_t d)
{
	float v = (d-60) / 4095.f * MAX_ADC_VOLTAGE; // Volt
	v = v / 2.4f * 49.4f; // v is attenuated by 2.2K and 47K voltage divider
	return (int)(v + 0.5);
}

// convert ADC to Imain mA
// ACS758-100B sensor
// 100A Bidirectional
// 0A = 2.5V centered, 20mV/A ratio
int convert_ADC2Imain(int16_t d)
{
	// 20A: d=1000
	// 30A: d=1500
	// 40A: d=2000
	float v = (float)d / 4095. * MAX_ADC_VOLTAGE;
	int mA = (v * 25 * 1000);
	return (mA >= 500 || mA <= -500) ? mA : 0;
}


int convert_ADC2Temp(uint32_t d)
{
	// NTC converter
	return 300;
	// TODO: implement Temperature converter
}

// return: length of the string (excluding \0)
// convert 100 to "10.0"
int convert_Temp2Str(int t, char* str)
{
	char sgn = 1;
	if(t < 0)
	{
		sgn = -1;
		t = -t;
	}
	int before_decimal = t / 10;
	int	decimal = t % 10;
	char* p = str;
	if(sgn == -1)
	{
		*p = '-';
		p++;
	}
	itoa(before_decimal, p, 10);
	while(*p) p++;
	*(p++) = '.';
	*(p++) = decimal + '0';
	*p = 0;
	return (p - str);
}

float calc_SWR(float forward, float reverse)
{
	if (forward < 10 || reverse < 10)
		return 1.;
	float K = reverse / forward;
	K = sqrtf(K);
	K = (K > 0)?(K):0;
	float swr = ((K + 1.f) / (1.f - K));
	return (swr < 10.) ? (swr) : (10.);
}

static uint8_t fan_autospeed_transfer_function(int16_t Temp1Real, uint8_t FanDutyOld)
{
	uint8_t FanDutyNew = FanDutyOld;
	if(FanDutyOld == 100) // 60C : 100%
	{
		if(Temp1Real < 50)
			FanDutyNew = 60;
		else if(Temp1Real < 40)
			FanDutyNew = 40;
		else if(Temp1Real < 25)
			FanDutyNew = 0;
	}
	else if(FanDutyOld == 60) // 50C : 60%
	{
		if(Temp1Real > 60)
			FanDutyNew = 100;
		else if(Temp1Real < 40)
			FanDutyNew = 40;
		else if(Temp1Real < 25)
			FanDutyNew = 0;
	}
	else if(FanDutyOld == 40)
	{
		if(Temp1Real > 60)
			FanDutyNew = 100;
		else if(Temp1Real > 50)
			FanDutyNew = 60;
		else if(Temp1Real < 25)
			FanDutyNew = 0;
	}
	else if(FanDutyOld == 0)
	{
		if(Temp1Real > 60)
			FanDutyNew = 100;
		else if(Temp1Real > 50)
			FanDutyNew = 60;
		else if(Temp1Real > 40)
			FanDutyNew = 40;
	}
	return FanDutyNew;
}

const int ALERT_ID_IMAIN = 0;
const int ALERT_ID_REVPWR2= 3;

extern void power_off(); // defined in main.c
void trigger_alert(int id)
{
	MainPowerEnabled = 0;
	power_off();
	osMessageQueuePut(qAlerts, &id, 0, 0);

}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) // the callback function when ADC1 DMA transmission Totally Completed
{
	int16_t adc;
	static int adc_smoothed_REV = -1;
	static int adc_smoothed_Imain = -1;
	if(hadc == &hadc1)
	{
//		osEventFlagsSet(SensorDataReadyEvents_handle, 0b001);
		/* important : do protections REALTIME */
		// case1 : REV too high
		adc = uhADC1ConvertedValues[IDX_REVpeak2];
		if(adc_smoothed_REV < 0)
			adc_smoothed_REV = adc;
		else
			adc_smoothed_REV = (adc_smoothed_REV +  adc * 3)  >> 2;
		if(adc_smoothed_REV >= REV_RAW_ALERT_THRESHOLD)
		{
			trigger_alert(ALERT_ID_REVPWR2);
		}
		// case2 : Imain too high
		adc = (int16_t)uhADC3ConvertedValues[IDX_Imain] - (int16_t)Imain_ADC0;
		if(adc_smoothed_Imain < 0)
			adc_smoothed_Imain = adc;
		else
			adc_smoothed_Imain = (adc_smoothed_Imain +  adc * 3)  >> 2;
		if(adc_smoothed_Imain >= IMAIN_RAW_ALERT_THRESHOLD)
		{
			trigger_alert(ALERT_ID_IMAIN);
		}

		osThreadFlagsSet(SensorTaskHandle, 0b001);

	}
	else if(hadc == &hadc3)
	{
//		osEventFlagsSet(SensorDataReadyEvents_handle, 0b100);
		osThreadFlagsSet(SensorTaskHandle, 0b100);
	}
}

void HAL_ADC_ErrorCallback(ADC_HandleTypeDef *hadc)
{
	if (hadc == &hadc1)
	{
		//lv_debug_log_error("Error: HADC1 DMA error\n", 0);
//		printf("Error: HADC1 DMA error\n");
	}
	else if(hadc == &hadc3)
	{
		//lv_debug_log_error("Error: HADC3 DMA error\n", 0);
//		printf("Error: HADC3 DMA error\n");
	}
}

void StartSensorTask()
{
	uint32_t i;
	init_sensors();
	// Init peak detectors to slow down number refresh
	init_PeakDetector(&PD_Imain, 10);
	init_PeakDetector(&PD_Vmain, 10);
	init_PeakDetector(&PD_FWD2, 10);
	init_PeakDetector(&PD_REV2, 10);
	init_PeakDetector(&PD_Temp1, 10);
	init_PeakDetector(&PD_Temp2, 10);
	// Start ADC trigger timer on TIM2 CH2:
	// 1kHz 50% duty cycle
	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
	// Start ADC DMA transmission
	HAL_ADC_Start_DMA(&hadc1, (uint16_t*)uhADC1ConvertedValues, ADC1_N_CHANNELS);
	HAL_ADC_Start_DMA(&hadc3, (uint16_t*)uhADC3ConvertedValues, ADC3_N_CHANNELS);
	// mark Slow-Refresh values to INT16_MAX indicating empty
	Temp1 = INT16_MAX;
	Temp2 = INT16_MAX;
	// Counter
	static uint16_t counter = 0;
	//// Calibrate Sensors ADC values before main task
	for(counter = 0; counter < 32; counter++)
	{
		osThreadFlagsClear(0xffff);
		osThreadFlagsWait(0b101, osFlagsWaitAll, osWaitForever);
		uint16_t* p = (uhADC1ConvertedValues);
		FWD1_raw = *(p++);
		FWDpeak1_raw = *(p++);
		REVpeak1_raw = *(p++);
		REV1_raw = *(p++);
		FWD2_raw = *(p++);
		FWDpeak2_raw = *(p++);
		REVpeak2_raw = *(p++);
		REV2_raw = *(p++);
		p = (uhADC3ConvertedValues);
		Band_decoder_raw = *(p++);
		Vmain_raw = *(p++);
		Imain_raw = *(p++);
		Temp1_raw = *(p++);
		Temp2_raw = *(p++);
		//// Calibrate the zero point of Imain ADC values
		// before power on the Vmain
		if(counter == 0)
			Imain_ADC0 = Imain_raw;
		else
			Imain_ADC0 = (Imain_ADC0 * 3  + Imain_raw) / 4; // EWMA
	}
	counter = 0;
	for(;;)
	{
		// wait for Event Group of sensor data ready
		// TODO: implement ADC DMA ISR
		osThreadFlagsClear(0xffff);
		osThreadFlagsWait(0b101, osFlagsWaitAll, osWaitForever);

//		osEventFlagsWait(SensorDataReadyEvents_handle, 0b101, osFlagsWaitAll, osWaitForever);
		{
			/* ADC 1*/
			uint16_t* p = (uhADC1ConvertedValues);
			FWD1_raw = *(p++);
			FWDpeak1_raw = *(p++);
			REVpeak1_raw = *(p++);
			REV1_raw = *(p++);
			FWD2_raw = *(p++);
			FWDpeak2_raw = *(p++);
			REVpeak2_raw = *(p++);
			REV2_raw = *(p++);
			/* ADC3 */
			p = (uhADC3ConvertedValues);
			Band_decoder_raw = *(p++);
			Vmain_raw = *(p++);
			Imain_raw = *(p++);
			Temp1_raw = *(p++);
			Temp2_raw = *(p++);
			append_PeakDetector(&PD_Imain, Imain_raw);
			append_PeakDetector(&PD_Vmain, Vmain_raw);
			append_PeakDetector(&PD_FWD2, FWDpeak2_raw);
			append_PeakDetector(&PD_REV2, REVpeak2_raw);
			append_PeakDetector(&PD_Temp1, Temp1_raw);
			append_PeakDetector(&PD_Temp2, Temp2_raw);

			FWDpeak1 = convert_ADC2FwdPower1(FWDpeak1_raw);
//			FWDpeak1 = convert_ADC2FwdPower1(FWDpeak1_raw);
//			REVpeak1 = convert_ADC2FwdPower1(REVpeak1_raw);
			REVpeak1 = convert_ADC2FwdPower1(REVpeak1_raw);
			float SWR1 = calc_SWR(FWDpeak1, REVpeak1); // 1.0f - inf
			FWDpeak2 = convert_ADC2FwdPower2(max_PeakDetector(&PD_FWD2));
//			FWDpeak2 = convert_ADC2FwdPower2(FWDpeak2_raw);
//			REVpeak2 = convert_ADC2FwdPower2(REVpeak2_raw);
			REVpeak2 = convert_ADC2FwdPower2(max_PeakDetector(&PD_REV2));
			float SWR2 = calc_SWR(FWDpeak2, REVpeak2);
			band_t Band_decoder = convert_ADC2Band(Band_decoder_raw);
			Vmain =  convert_ADC2Vmain(max_PeakDetector(&PD_Vmain));
			// TODO: Imain peak detector
			Imain = convert_ADC2Imain((int16_t)max_PeakDetector(&PD_Imain) - Imain_ADC0);
			Temp1 = NTC_get_C(&NTC1, avg_top_quantile_PeakDetector(&PD_Temp1, 0.8));
			Temp2 = NTC_get_C(&NTC2, avg_top_quantile_PeakDetector(&PD_Temp2, 0.8));
			/* update GUI Widgets */
			osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever); // acquire the Mutex to manipulate GUI widgets
			{
//				taskENTER_CRITICAL();
				////// Fast Refresh Tasks
				//// TODO: update the values of GUI widgets
				GUI_set_FWD_progbar(convert_ADC2FwdPower2(FWDpeak2_raw));
				//// Use the average value of top 20% populations to slow down number refresh
//				GUI_set_FWD_peak_value(avg_top_quantile_PeakDetector(&PD_FWD1, 0.2));
				GUI_set_FWD_peak_value(FWDpeak2);
				GUI_set_REV_progbar(convert_ADC2FwdPower2(REVpeak2_raw));
				//// Use the average value of top 20% populations to slow down number refresh
//				GUI_set_REV_peak_value(avg_top_quantile_PeakDetector(&PD_REV2, 0.2));
				GUI_set_REV_peak_value(REVpeak2);
				GUI_set_SWR_progbar(SWR2);
				//// Use the average value of top 20% populations to slow down number refresh
//				GUI_set_SWR_peak_value(avg_top_quantile_PeakDetector(&PD_SWR2, 0.2));
				GUI_set_SWR_peak_value(SWR2);
				///// Slow Refresh tasks: 5 times slower
				if(counter % 5 == 0)
				{
					//// Refresh temperature display
					GUI_set_core_temperature(Temp1);
					GUI_set_ambient_temperature(Temp2);
					//// Refresh Vmain Imain
					GUI_set_Vmain(Vmain);
					GUI_set_Imain(Imain);
				}
			// TODO: refresh transmission state indicator
			// TODO: refresh Band info
//				GUI_Set_Text_TextBoxH(&gTextBand, strlen(band_name), band_name);
			// 2: refresh Core Temperature info
//				static char Temp1_str[6] = {0};
//				int len_text = convert_Temp2Str(Temp1, Temp1_str);
//				GUI_Set_Text_TextBoxH(&gTextCoreTemp, len_text, Temp1_str);
			// 3: refresh Vmain info
//				static char Vmain_str[6] = {0};
//				len_text = convert_Temp2Str(Vmain_mV / 100, Vmain_str);
//				GUI_Set_Text_TextBoxH(&gTextVmain, len_text, Vmain_str);
			// 4: refresh Imain info
//				static char Imain_str[6] = {0};
//				len_text = convert_Temp2Str(Imain_mA / 100, Imain_str);
//				GUI_Set_Text_TextBoxH(&gTextImain, len_text, Imain_str);
			// 5: refresh Forward Power Bar
//				GUI_Set_Progress_ProgressBarH(&gProgBarFwdPwr, FWD1_progress, FWDpeak1_progress);
			// 6: refresh Forward Power value
//				static char FwdPwr_str[6] = {0};
//				len_text = strlen(itoa(FWD1, FwdPwr_str, 10));
//				GUI_Set_Text_TextBoxH(&gTextFwdPwr, len_text, FwdPwr_str);
			// 7: refresh Reverse Power Bar
//				GUI_Set_Progress_ProgressBarH(&gProgBarRevPwr, REV1_progress, REVpeak1_progress);
			// 8: refresh Reverse Power value
//				static char RevPwr_str[6] = {0};
//				len_text = strlen(itoa(REV1, RevPwr_str, 10));
//				GUI_Set_Text_TextBoxH(&gTextRevPwr, len_text, RevPwr_str);
			// 9: refresh SWR bar
//				GUI_Set_Progress_ProgressBarH(&gProgBarSWR, SWR1_progress, SWR1_progress);
			// 10: refresh SWR value
//				static char SWR_str[6] = {0};
//				len_text = convert_Temp2Str((int)(SWR1 * 10), SWR_str);
//				GUI_Set_Text_TextBoxH(&gTextSWR, len_text, SWR_str);
//				taskEXIT_CRITICAL();
			}
			osMutexRelease(mtxGUIWidgetsHandle);
//			osSemaphoreRelease(GUIDataUpdatedSemaphore_handle); // signal the GUI redrawing task
			osDelay(30);
		}
		++counter;
	}
}
