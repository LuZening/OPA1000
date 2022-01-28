/*
 * Sensor_task.h
 *
 *  Created on: Mar 2, 2020
 *      Author: Zening
 */

#ifndef SENSOR_TASK_H_
#define SENSOR_TASK_H_

#include "cmsis_os.h"
#include "RadioTypes.h"
#include "peak_detector.h"
#include "NTC.h"

#define ADC1_N_CHANNELS 8
#define ADC2_N_CHANNELS 0
#define ADC3_N_CHANNELS 5
#define N_SENSORS (ADC1_N_CHANNELS + ADC2_N_CHANNELS + ADC3_N_CHANNELS)

#define MAX_ADC_VOLTAGE 3.3f
#define N_BANDCODE_VOLTAGES 7
#define BANDCODE_VOLTAGE_TOLERANCE .2f
#define VMAIN_RATIO 48.0f
//#define LOG_DETECTOR # uncomment to use logarithm power detector


/*** ADC DMA Buffer ***/
extern uint16_t uhADC1ConvertedValues[ADC1_N_CHANNELS];
extern bool ADC1DataReady;
//uint16_t uhADC2ConvertedValues[ADC2_N_OVERSAMPLING][ADC2_N_CHANNELS];
//bool ADC2DataReady = false;
extern uint16_t uhADC3ConvertedValues[ADC3_N_CHANNELS];
extern bool ADC3DataReady;

extern const float ICOM_BANDCODE_VOLTAGES[N_BANDCODE_VOLTAGES];



extern const char* BANDNAME_160M;
extern const char* BANDNAME_80M;
extern const char* BANDNAME_40M_30M;
extern const char* BANDNAME_20M_17M;
extern const char* BANDNAME_15M_10M;
const char* convert_Band_to_Name(band_t band);

//extern osEventFlagsId_t SensorDataReadyEvents_handle;

/* Use peak detectors to slow down number refresh */
extern PeakDetector PD_FWD2;
extern PeakDetector PD_REV2;
extern PeakDetector PD_SWR2;
// Temperature Sensing NTC
extern NTC_t NTC1, NTC2;

// Sensor Task Thread handler
extern osThreadId_t SensorTaskHandle;

void init_sensors();
void StartSensorTask();

#endif /* SENSOR_TASK_H_ */
