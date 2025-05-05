/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdlib.h"

/*
 * COM.h Manages softUART Band Decoder and CAT
 * COM.h: handles OS task for all transceiving thru COM port
 * SoftUART.h: soft UART for Band Deocder
 * Architecture
 * COM.h: OS Task handler
 * 	- SoftUART.h Band Decoder
 * 	- Hardware UART CAT for PC to remote control the PA
 */

#include "COM.h"
#ifdef USE_I2C_EEPROM
#include "I2C_EEPROM.h"
#define EEPROM_WriteBytes I2C_EEPROM_WriteBytes
#define EEPROM_ReadBytes I2C_EEPROM_ReadBytes
#else
#include "Flash_EEPROM.h"
#endif
//#include "GUI.h"
//#include "GUI_task.h"
#include "R61408.h"
#include "Sensor_task.h"
#include "task.h"
#include "lvgl.h"
#include "src/lv_core/lv_debug.h"
#include "LVGL_GUI.h"
#include "persistent.h"
#include "touch_HR2046.h"
#include "fan.h"
#include "SoftUART.h"
#include "MultiLinearValueMapper.h"
#include "RotEnc.h"
#include <time.h>
#include "COM_task.h"
#include "Flash_EEPROM.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc1;
DMA_HandleTypeDef hdma_adc3;

CAN_HandleTypeDef hcan2;

CRC_HandleTypeDef hcrc;

SPI_HandleTypeDef hspi1;

TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;

SRAM_HandleTypeDef hsram1;

/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for timerLVGL */
osTimerId_t timerLVGLHandle;
const osTimerAttr_t timerLVGL_attributes = {
  .name = "timerLVGL"
};
/* USER CODE BEGIN PV */
// Config
PersistentVars_t cfg;
bool isCfgValid = false;
// OS handles
// define FreeRTOS heap at CCM address
uint8_t __attribute__((section(".CCMRAM_heap"))) ucHeap[configTOTAL_HEAP_SIZE];
osMutexId_t mtxGUIWidgetsHandle;
osMutexId_t mtxBandDataHandle;
osThreadId_t AlertTaskHandle;
void StartAlertTask();
//osThreadId_t GUIRefreshTaskHandle;
osThreadId_t SensorTaskHandle; // Implement the extern declaration in Sensor_task.h
osThreadId_t HumanInputTaskHandle;
void StartHumanInputTask();
osThreadId_t COMPortTaskHandle;


/* PTT */
volatile bool allowPTT = false;
volatile int16_t transmit_delay_counter = 0;
volatile int16_t transmit_max_counter = 0;

/* Touch Screen */
volatile bool TSCalibAfterStart = false;
volatile bool isTSCalibrating = false;//indicate if the user is running touch screen calibration
static volatile bool TS_touched = false;
static volatile uint16_t TS_X, TS_Y; // the latest touch screen X, Y position
static volatile uint16_t TS_X_ADC, TS_Y_ADC; // the latest touch screen X,Y ADC values
osThreadId_t TouchScreenTaskHandle;
void StartTouchScreenTask();
osThreadId_t TouchScreenCalibHandle; // Touch screen calibration task, shows the calibration screen and takes 5 points
void StartTouchScreenCalibTask();
/* Touch Screen End */

/* Rotary Encoder */
RotEnc_t RotEnc;
/* Rotary Encoder End */

//osSemaphoreId_t GUIDataUpdatedSemaphore_handle;
//osEventFlagsId_t SensorDataReadyEvents_handle;

// Deivce configurations
// Band selector
band_t BAND_switch = BAND_15M_10M; // the position of Band Rotary Switch
/* *****************     Fan    ********************************************** */

// the transfer function
#define N_SEGS_AUTO_FAN_SPEED 4
const int16_t AutoFanSpeed_tempC1[N_SEGS_AUTO_FAN_SPEED] = {400, 500, 600, 700}; // 40.0C, 50.0C ... 70.0C
const uint8_t AutoFanSpeed_speed1[N_SEGS_AUTO_FAN_SPEED] = {30, 50, 80, 100};
const uint8_t AutoFanSpeed_hyst1 = 50;
const int16_t AutoFanSpeed_tempC2[N_SEGS_AUTO_FAN_SPEED] = {400, 500, 600, 700}; // 40.0C, 50.0C ... 70.0C
const uint8_t AutoFanSpeed_speed2[N_SEGS_AUTO_FAN_SPEED] = {30, 50, 80, 100};
const uint8_t AutoFanSpeed_hyst2 = 50;
Fan_t fan1, fan2;
/* *****************   End of  Fan  ********************************************** */

/* *****************	COM		************************************************** */
COM_t COM1;
uint8_t bufFIFOTX1[COM_BUFFER_LEN];
uint8_t bufFIFORX1[COM_BUFFER_LEN];

/* *****************	End of COM		****************************************** */

// System Status
Transmission_State_t  trans_state = STANDBY; // transmission_state: STANDBY, RECEIVING, TRANSMITTING
// Menu configurations
bool Enable_band_remote = false;
#ifdef OPA1000
static void OPA1000_MX_GPIO_Init(void);
#endif
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_FSMC_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC3_Init(void);
static void MX_CAN2_Init(void);
static void MX_CRC_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
static void MX_TIM3_Init(void);
void StartDefaultTask(void *argument);
void onTimerLVGL(void *argument);

/* USER CODE BEGIN PFP */

void onTimerRandomValue(void *argument) // randomly update widgets
{
//	osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
//	if(lvProgbarFwdPwr)
//	{
//		lv_bar_set_value(lvProgbarFwdPwr, rand() % PROGBAR_MAX_VALUE_FWDPWR , LV_ANIM_ON);
//	}
//	if(lvProgbarRevPwr)
//	{
//		lv_bar_set_value(lvProgbarRevPwr, rand() % PROGBAR_MAX_VALUE_REVPWR, LV_ANIM_ON);
//	}
//	if(lvProgbarSWR)
//	{
//		lv_bar_set_value(lvProgbarSWR, rand() % PROGBAR_MAX_VALUE_SWR, LV_ANIM_ON);
//		//lv_label_set_text_fmt(lvLblSWRValue, "%04d", rand() % 1000);
//	}
//	osMutexRelease(mtxGUIWidgetsHandle);
}



void startTouchScreenCalibTask()
{
	if(TouchScreenCalibHandle != NULL)
	{
		osThreadFlagsSet(TouchScreenCalibHandle, TOUCHSCREEN_CALIB_START);
	}

}

void save_config()
{
//	EEPROM_WriteBytes(&EEPROM, &cfg, sizeof(cfg));
	Flash_EEPROM_WriteBytes(&FlashEEPROM, &cfg, sizeof(cfg));
}


// power_off: turn off Vmain
void power_off()
{
	HAL_GPIO_WritePin(RF_12V_ON_GPIO_Port, RF_12V_ON_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(PWR_on_sig_GPIO_Port, PWR_on_sig_Pin, GPIO_PIN_SET);
}

// power_on: turn on Vmain
bool power_on()
{
	bool r = true;
	// check if power is good first
//	if(HAL_GPIO_ReadPin(VMAIN_HIGH_GPIO_Port, VMAIN_HIGH_Pin) == GPIO_PIN_SET
//			|| HAL_GPIO_ReadPin(IMAIN_HIGHX_GPIO_Port, IMAIN_HIGHX_Pin) == GPIO_PIN_RESET
//			|| HAL_GPIO_ReadPin(OverdriveX_GPIO_Port, OverdriveX_Pin) == GPIO_PIN_RESET )
//		r = false;
//	else
	HAL_GPIO_WritePin(RF_12V_ON_GPIO_Port, RF_12V_ON_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(PWR_on_sig_GPIO_Port, PWR_on_sig_Pin, GPIO_PIN_RESET);
	return r;
}

void clear_fault_hold()
{
#ifdef OPA1000
	HAL_GPIO_WritePin(ClearFault_GPIO_Port, ClearFault_Pin, GPIO_PIN_RESET);
//	HAL_GPIO_WritePin(RF_12V_ON_GPIO_Port, RF_12V_ON_Pin, GPIO_PIN_SET);
#endif

}

void clear_fault_release()
{
#ifdef OPA1000
	HAL_GPIO_WritePin(ClearFault_GPIO_Port, ClearFault_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(RF_12V_ON_GPIO_Port, RF_12V_ON_Pin, GPIO_PIN_RESET);
#endif
}

void clear_fault()
{
	clear_fault_hold();
	HAL_Delay(10);
	clear_fault_release();
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* init EEPROM begin */
  // Load Config from EEPROM
//#if (sizeof(cfg) > 256)
//#error("The size of persistent config content is too big to fit in the EEPROM 256Bytes max")
//#endif
#ifdef USE_I2C_EEPROM
  I2C_EEPROM_init(&EEPROM, AT24C02, EEP_SCL_GPIO_Port, EEP_SCL_Pin, EEP_SDA_GPIO_Port, EEP_SDA_Pin, EEP_WP_GPIO_Port, EEP_WP_Pin, 0x00);
  EEPROM_ReadBytes(&EEPROM, (uint8_t*)&cfg, sizeof(cfg));
#endif
  Flash_EEPROM_ReadBytes(&FlashEEPROM, (uint8_t*)&cfg, sizeof(cfg));
  isCfgValid = isPersistentVarsValid(&cfg); // check if config is valid
  if(!isCfgValid)
  {
	  // Lost config data
	  // step 1: rebuild
	  strcpy(cfg.validator, VALIDATE_CODE);
	  // initialize config parameters
	  init_config(&cfg);
	  // step 2: set a flag to execute Touch Screen Calib task after startup
	  TSCalibAfterStart = TRUE;
  }
  // Saved Config: Load Saved Config for Touch Screen
  pTSCalib = &cfg.TSCalibInfo;
  // init FS
  FS_begin(&FS, (uint32_t*)FS_BASE_ADDR);
  /* init EEPROM end */
  /* init LVGL */


  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
#ifdef OPA1000
  OPA1000_MX_GPIO_Init();
  /* USER CODE END SysInit */
  /* Initialize all configured peripherals */
#else
  MX_GPIO_Init();
#endif
  MX_DMA_Init();
  MX_FSMC_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_ADC3_Init();
  MX_CAN2_Init();
  MX_CRC_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  // Make sure main power is off (Vmain)
  power_off();
  //DEBUG: Invalidate Fault
  clear_fault();
#ifdef OPA1000
  clear_fault_hold();
#endif
  // Saved Config: Load previous Fan settings
  /* **********	Init Fan	********** */
  fan_init(&fan1, FAN1_GPIO_Port, FAN1_Pin);
  fan_init_automode(&fan1, N_SEGS_AUTO_FAN_SPEED, AutoFanSpeed_tempC1, AutoFanSpeed_speed1, AutoFanSpeed_hyst1);
  fan_init(&fan2, FAN2_GPIO_Port, FAN2_Pin);
  fan_init_automode(&fan1, N_SEGS_AUTO_FAN_SPEED, AutoFanSpeed_tempC2, AutoFanSpeed_speed2, AutoFanSpeed_hyst2);
  /* **********	End of Init Fan	********** */
  //  soft_pwm_driver_set_duty(&fan1, cfg.Fan1Speed);
//  soft_pwm_driver_set_duty(&fan2, cfg.Fan2Speed);

  /* **********	Init COM	********** */
  // hardware UART - for CAT
  COM_init(&COM1, &huart1, bufFIFOTX1, COM_BUFFER_LEN, bufFIFORX1, COM_BUFFER_LEN);
  // Software UART - for band decoder
  SoftUartInit(0, NULL, 0, BAND_RX_GPIO_Port, BAND_RX_Pin, NULL);
  SoftUartStart();
  SoftUartEnableRx(0);
  /* **********	End of Init COM	****** */
  //while(1)
  LCD_Init();
  // init Rotary Encoder (Debounce delay 10000us(10ms))
#if defined(OPA2000)
  RotEnc_init(&RotEnc, 10000, ENC_AX_GPIO_Port, ENC_AX_Pin, ENC_BX_GPIO_Port, ENC_BX_Pin, ENC_SWX_GPIO_Port, ENC_SWX_Pin);
#endif
  // RotEnc is ticked in onTimerLVGL, each 5ms
  // init GUI
  init_LVGL_GUI();

  if(!TSCalibAfterStart)
  {

	  // 如果上电时旋转编码器按下，则校准屏幕
//	  if(HAL_GPIO_ReadPin(RotEnc.SW.GPIOport, RotEnc.SW.GPIOpin) == GPIO_PIN_RESET)
	  if(!isCfgValid)
	  {
		  HAL_Delay(20);
		  // 消抖
		  if(HAL_GPIO_ReadPin(RotEnc.SW.GPIOport, RotEnc.SW.GPIOpin) == GPIO_PIN_RESET)
			  TSCalibAfterStart = TRUE;

	  }

  }

  init_main_widgets();
  /* 在内存中为内存堆分配两个内存块.第一个内存块0x10000字节,起始地址为0x80000000,
  第二个内存块0xa0000字节,起始地址为0x90000000.起始地址为0x80000000的内存块的
  起始地址更低,因此放到了数组的第一个位置.*/
  const HeapRegion_t xHeapRegions[] =
  {
      { ucHeap, configTOTAL_HEAP_SIZE },
      { NULL, 0 } /* 数组结尾. */
  };

  vPortDefineHeapRegions(xHeapRegions);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
//  const osMutexAttr_t mtxBandData_attr = {.name = "mtxBandData"};
  mtxBandDataHandle = osMutexNew(NULL);
  qAlerts = osMessageQueueNew(8, sizeof(int), NULL);
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
//  osSemaphoreAttr_t GUIDataUpdatedSemaphore_attr = {
//		  .name = "GUIDataUpdatedSemaphore",
//		  .cb_mem = NULL,
//		  .cb_size = 0,
//  };
//  GUIDataUpdatedSemaphore_handle = osSemaphoreNew(1, 0, &GUIDataUpdatedSemaphore_attr);

//  osEventFlagsAttr_t SensorDataReadyEvents_attr = {
//		  .name = "SensorDataReadyEvents",
//		  .cb_mem = NULL,
//		  .cb_size = 0,
//  };
//  SensorDataReadyEvents_handle = osEventFlagsNew(&SensorDataReadyEvents_attr);

  /* USER CODE END RTOS_SEMAPHORES */

  /* Create the timer(s) */
  /* creation of timerLVGL */
  timerLVGLHandle = osTimerNew(onTimerLVGL, osTimerPeriodic, NULL, &timerLVGL_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  osTimerStart(timerLVGLHandle, 5);
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  // COM to send data queue
//  osMessageQueueAttr_t COMBytesToSendQ_attr = {
//		  .name = "COMBytesToSendQ"
//  };
//  COMBytesToSendQHandle = osMessageQueueNew(128, sizeof(uint8_t), &COMBytesToSendQ_attr);
//  // COM recieved data queue
//  osMessageQueueAttr_t COMBytesRecvedQ_attr= {
//		  .name = "COMBytesRecvedQ"
//  };
//  COMBytesRecvedQHandle = osMessageQueueNew(128, sizeof(uint8_t), &COMBytesRecvedQ_attr);
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */


  // reading binary signals
  const osThreadAttr_t AlertTaskHandle_attr = {
		  .name = "AlertTask",
		  .priority = (osPriority_t)(osPriorityNormal),
		  .stack_size = 2048
  };
  AlertTaskHandle = osThreadNew(StartAlertTask, NULL, &AlertTaskHandle_attr);

  // redraw GUI widgets
//  const osThreadAttr_t GUIRefreshTask_attributes = {
//		  .name = "GUIRefreshTask",
//		  .priority = (osPriority_t) osPriorityAboveNormal,
//		  .stack_size = 4 * 1024 // 16KBytes
//  };
//  GUIRefreshTaskHandle = osThreadNew(StartGUIRefreshTask, NULL, &GUIRefreshTask_attributes);
  // process ADC converted value
  const osThreadAttr_t SensorTask_attributes = {
		  .name = "SensorTask",
		  .priority = (osPriority_t)osPriorityAboveNormal,
		  .stack_size = 2048 // 4KBytes
  };
  SensorTaskHandle = osThreadNew(StartSensorTask, NULL, &SensorTask_attributes);
  // process Human inputs
  const osThreadAttr_t HumanInputTask_attr = {
		  .name = "HumanInputTask",
		  .priority = (osPriority_t)osPriorityBelowNormal,
		  .stack_size = 2048 // 1KBytes
  };
  HumanInputTaskHandle = osThreadNew(StartHumanInputTask, NULL, &HumanInputTask_attr);
  const osThreadAttr_t TouchScreenTask_attr={
		  .name = "TouchScreenTask",
		  .priority = (osPriority_t)osPriorityAboveNormal,
		  .stack_size = 2048
  };
  TouchScreenTaskHandle = osThreadNew(StartTouchScreenTask, NULL, &TouchScreenTask_attr);

  static const osThreadAttr_t TouchScreenCalibTask_attr={
		  .name = "TouchScreenCalibTask",
		  .priority = (osPriority_t)osPriorityBelowNormal,
		  .stack_size = 2048
  };
  TouchScreenCalibHandle = osThreadNew(StartTouchScreenCalibTask, NULL, &TouchScreenCalibTask_attr);

  const osThreadAttr_t COMPortTask_attr = {
		  .name = "COMPortTask",
		  .priority = (osPriority_t)osPriorityNormal,
		  .stack_size = 2048 // 2Kytes
  };
  COMPortTaskHandle =osThreadNew(StartCOMRecvTask, NULL, &COMPortTask_attr);
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */

  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 6;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV4;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
  /** Enables the Clock Security System
  */
  HAL_RCC_EnableCSS();
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ENABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 8;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_4;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_5;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = 6;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_9;
  sConfig.Rank = 7;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_14;
  sConfig.Rank = 8;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */
  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV8;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = ENABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
  hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_TRGO;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 5;
  hadc3.Init.DMAContinuousRequests = ENABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SEQ_CONV;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_28CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_10;
  sConfig.Rank = 2;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_11;
  sConfig.Rank = 3;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_12;
  sConfig.Rank = 4;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_13;
  sConfig.Rank = 5;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

/**
  * @brief CAN2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_CAN2_Init(void)
{

  /* USER CODE BEGIN CAN2_Init 0 */

  /* USER CODE END CAN2_Init 0 */

  /* USER CODE BEGIN CAN2_Init 1 */

  /* USER CODE END CAN2_Init 1 */
  hcan2.Instance = CAN2;
  hcan2.Init.Prescaler = 16;
  hcan2.Init.Mode = CAN_MODE_NORMAL;
  hcan2.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan2.Init.TimeSeg1 = CAN_BS1_1TQ;
  hcan2.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan2.Init.TimeTriggeredMode = DISABLE;
  hcan2.Init.AutoBusOff = DISABLE;
  hcan2.Init.AutoWakeUp = DISABLE;
  hcan2.Init.AutoRetransmission = DISABLE;
  hcan2.Init.ReceiveFifoLocked = DISABLE;
  hcan2.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN2_Init 2 */

  /* USER CODE END CAN2_Init 2 */

}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_HIGH;
  hspi1.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_256;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 83; // CLOCKED AT 84MHz
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 1000; // EACH 1000us = 1kHz
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 8400;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 10;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */

}

/**
  * @brief TIM4 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM4_Init(void)
{

  /* USER CODE BEGIN TIM4_Init 0 */
	// TIM4 for Software UART Baudrate
  /* USER CODE END TIM4_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM4_Init 1 */

  /* USER CODE END TIM4_Init 1 */
  htim4.Instance = TIM4;
  htim4.Init.Prescaler = 4;
  htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim4.Init.Period = 250;
  htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim4.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim4) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim4, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM4_Init 2 */
  /* USER CODE END TIM4_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */
  // disable TX buffer empty interrupt
  huart1.Instance->CR1 &= ~(USART_CR1_TXEIE);

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream1_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream1_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, BAND_SEL6X_Pin|BAND_SEL5X_Pin|BAND_SEL4X_Pin|BAND_SEL3X_Pin
                          |BAND_SEL2X_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, BAND_SEL1X_Pin|LCD_RESX_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, PrtctSig_Pin|EEP_WP_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, MCU_STATE_LEDX_Pin|EEP_SDA_Pin|EEP_SCL_Pin|POT_SCL_Pin
                          |POT_SDA_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RF_12V_ON_Pin|BAND_TX_Pin|LCD_BL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Transmit_GPIO_Port, Transmit_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, FAN1_Pin|FAN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PTT_SUPPRESSX_GPIO_Port, PTT_SUPPRESSX_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(T_CSX_GPIO_Port, T_CSX_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PWR_on_sig_GPIO_Port, PWR_on_sig_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BAND_SEL6X_Pin BAND_SEL5X_Pin BAND_SEL4X_Pin BAND_SEL3X_Pin
                           BAND_SEL2X_Pin */
  GPIO_InitStruct.Pin = BAND_SEL6X_Pin|BAND_SEL5X_Pin|BAND_SEL4X_Pin|BAND_SEL3X_Pin
                          |BAND_SEL2X_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : BAND_SEL1X_Pin LCD_RESX_Pin */
  GPIO_InitStruct.Pin = BAND_SEL1X_Pin|LCD_RESX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins :  PrtctSig_Pin EEP_WP_Pin */
  GPIO_InitStruct.Pin = PrtctSig_Pin|EEP_WP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : MCU_STATE_LEDX_Pin POT_SCL_Pin POT_SDA_Pin */
  GPIO_InitStruct.Pin = MCU_STATE_LEDX_Pin|POT_SCL_Pin|POT_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : EEP_SDA_Pin EEP_SCL_Pin */
  GPIO_InitStruct.Pin = EEP_SDA_Pin|EEP_SCL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : RF_12V_ON_Pin LCD_BL_Pin */
  GPIO_InitStruct.Pin = RF_12V_ON_Pin|LCD_BL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : BAND_TX_Pin */
  GPIO_InitStruct.Pin = BAND_TX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(BAND_TX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BAND_RX_Pin REV1_HIGHX_Pin T_BUSY_Pin */
  GPIO_InitStruct.Pin = BAND_RX_Pin|REV1_HIGHX_Pin|T_BUSY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : T_INTX_Pin */
  GPIO_InitStruct.Pin = T_INTX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(T_INTX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ENC_AX_Pin ENC_BX_Pin */
  GPIO_InitStruct.Pin = ENC_AX_Pin|ENC_BX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : ENC_SWX_Pin SpareSigX_Pin IMAIN_HIGHX_Pin */
  GPIO_InitStruct.Pin = ENC_SWX_Pin|SpareSigX_Pin|IMAIN_HIGHX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PTTX_Pin */
  GPIO_InitStruct.Pin = PTTX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(PTTX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : Transmit_Pin */
  GPIO_InitStruct.Pin = Transmit_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(Transmit_GPIO_Port, &GPIO_InitStruct);


  /*Configure GPIO pin : TEMP2_HIGH_Pin */
  GPIO_InitStruct.Pin = TEMP2_HIGH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(TEMP2_HIGH_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : TEMP1_HIGH_Pin VMAIN_HIGH_Pin */
  GPIO_InitStruct.Pin = TEMP1_HIGH_Pin|VMAIN_HIGH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : FAN1_Pin */
  GPIO_InitStruct.Pin = FAN1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PTT_SUPPRESSX_Pin */
  GPIO_InitStruct.Pin = PTT_SUPPRESSX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PTT_SUPPRESSX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : REV2_HIGH_Pin */
  GPIO_InitStruct.Pin = REV2_HIGH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(REV2_HIGH_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : T_CSX_Pin */
  GPIO_InitStruct.Pin = T_CSX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(T_CSX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PWR_on_sig_Pin */
  GPIO_InitStruct.Pin = PWR_on_sig_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PWR_on_sig_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OverdriveX_Pin */
  GPIO_InitStruct.Pin = OverdriveX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(OverdriveX_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 4, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}

/* FSMC initialization function */
static void MX_FSMC_Init(void)
{

  /* USER CODE BEGIN FSMC_Init 0 */

  /* USER CODE END FSMC_Init 0 */

  FSMC_NORSRAM_TimingTypeDef Timing = {0};

  /* USER CODE BEGIN FSMC_Init 1 */

  /* USER CODE END FSMC_Init 1 */

  /** Perform the SRAM1 memory initialization sequence
  */
  hsram1.Instance = FSMC_NORSRAM_DEVICE;
  hsram1.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;
  /* hsram1.Init */
  hsram1.Init.NSBank = FSMC_NORSRAM_BANK1;
  hsram1.Init.DataAddressMux = FSMC_DATA_ADDRESS_MUX_DISABLE;
  hsram1.Init.MemoryType = FSMC_MEMORY_TYPE_SRAM;
  hsram1.Init.MemoryDataWidth = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
  hsram1.Init.BurstAccessMode = FSMC_BURST_ACCESS_MODE_DISABLE;
  hsram1.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  hsram1.Init.WrapMode = FSMC_WRAP_MODE_DISABLE;
  hsram1.Init.WaitSignalActive = FSMC_WAIT_TIMING_BEFORE_WS;
  hsram1.Init.WriteOperation = FSMC_WRITE_OPERATION_ENABLE;
  hsram1.Init.WaitSignal = FSMC_WAIT_SIGNAL_DISABLE;
  hsram1.Init.ExtendedMode = FSMC_EXTENDED_MODE_DISABLE;
  hsram1.Init.AsynchronousWait = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  hsram1.Init.WriteBurst = FSMC_WRITE_BURST_DISABLE;
  hsram1.Init.PageSize = FSMC_PAGE_SIZE_NONE;
  /* Timing */
  Timing.AddressSetupTime = 4;
  Timing.AddressHoldTime = 15;
  Timing.DataSetupTime = 4;
  Timing.BusTurnAroundDuration = 2;
  Timing.CLKDivision = 16;
  Timing.DataLatency = 17;
  Timing.AccessMode = FSMC_ACCESS_MODE_A;
  /* ExtTiming */

  if (HAL_SRAM_Init(&hsram1, &Timing, NULL) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FSMC_Init 2 */

  /* USER CODE END FSMC_Init 2 */
}

/* USER CODE BEGIN 4 */
#ifdef OPA1000
static void OPA1000_MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, BAND_SEL6X_Pin|BAND_SEL5X_Pin|BAND_SEL4X_Pin|BAND_SEL3X_Pin
                          |BAND_SEL2X_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, BAND_SEL1X_Pin|LCD_RESX_Pin, GPIO_PIN_SET);


  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, MCU_STATE_LEDX_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, RF_12V_ON_Pin|BAND_TX_Pin|LCD_BL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, FAN1_Pin|FAN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PTT_SUPPRESSX_GPIO_Port, PTT_SUPPRESSX_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(T_CSX_GPIO_Port, T_CSX_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PWR_on_sig_GPIO_Port, PWR_on_sig_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ClearFault_GPIO_Port, ClearFault_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : BAND_SEL6X_Pin BAND_SEL5X_Pin BAND_SEL4X_Pin BAND_SEL3X_Pin
                           BAND_SEL2X_Pin */
  GPIO_InitStruct.Pin = BAND_SEL6X_Pin|BAND_SEL5X_Pin|BAND_SEL4X_Pin|BAND_SEL3X_Pin
                          |BAND_SEL2X_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : BAND_SEL1X_Pin LCD_RESX_Pin */
  GPIO_InitStruct.Pin = BAND_SEL1X_Pin|LCD_RESX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


  /*Configure GPIO pins : MCU_STATE_LEDX_Pin POT_SCL_Pin POT_SDA_Pin */
  GPIO_InitStruct.Pin = MCU_STATE_LEDX_Pin | ClearFault_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : RF_12V_ON_Pin LCD_BL_Pin */
  GPIO_InitStruct.Pin = RF_12V_ON_Pin|LCD_BL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);


  /*Configure GPIO pins : BAND_RX_Pin REV1_HIGHX_Pin T_BUSY_Pin */
  GPIO_InitStruct.Pin = BAND_RX_Pin|REV1_HIGHX_Pin|T_BUSY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : T_INTX_Pin */
  GPIO_InitStruct.Pin = T_INTX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(T_INTX_GPIO_Port, &GPIO_InitStruct);


  /*Configure GPIO pins : ENC_SWX_Pin SpareSigX_Pin IMAIN_HIGHX_Pin */
  GPIO_InitStruct.Pin = IMAIN_HIGHX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);


  /*Configure GPIO pin : Transmit_Pin */
  GPIO_InitStruct.Pin = Transmit_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(Transmit_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : TEMP2_HIGH_Pin */
  GPIO_InitStruct.Pin = TEMP2_HIGH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(TEMP2_HIGH_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : TEMP1_HIGH_Pin VMAIN_HIGH_Pin */
  GPIO_InitStruct.Pin = TEMP1_HIGH_Pin|VMAIN_HIGH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : FAN1_Pin FAN2_Pin */
  GPIO_InitStruct.Pin = FAN1_Pin|FAN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);


  /*Configure GPIO pin : REV2_HIGH_Pin */
  GPIO_InitStruct.Pin = REV2_HIGH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(REV2_HIGH_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : T_CSX_Pin */
  GPIO_InitStruct.Pin = T_CSX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(T_CSX_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PWR_on_sig_Pin */
  GPIO_InitStruct.Pin = PWR_on_sig_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PWR_on_sig_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : OverdriveX_Pin */
  GPIO_InitStruct.Pin = OverdriveX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(OverdriveX_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 1, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 4, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

}
#endif

// Alert handling task, where binary inputs are processed
// TODO design Alert window
const GPIO_TypeDef* AlertInputGPIOPorts[] = {
		IMAIN_HIGHX_GPIO_Port,
		VMAIN_HIGH_GPIO_Port,
		REV1_HIGHX_GPIO_Port,
		REV2_HIGH_GPIO_Port,
		TEMP1_HIGH_GPIO_Port,
		TEMP2_HIGH_GPIO_Port,
		OverdriveX_GPIO_Port,
};
const uint16_t AlertInputGPIOPins[] = {
		IMAIN_HIGHX_Pin,
		VMAIN_HIGH_Pin,
		REV1_HIGHX_Pin,
		REV2_HIGH_Pin,
		TEMP1_HIGH_Pin,
		TEMP2_HIGH_Pin,
		OverdriveX_Pin,
};

const uint8_t AlertInputActiveLevels[] = {
		GPIO_PIN_RESET,
		GPIO_PIN_SET,
		GPIO_PIN_RESET,
		GPIO_PIN_SET,
		GPIO_PIN_SET,
		GPIO_PIN_SET,
		GPIO_PIN_RESET,
};
const uint8_t AlertTrigThresholds[] = {
		1,
		1,
		2,
		2,
		5,
		5,
		2,
};
#define N_INPUT_LOGIC_ALERT_SIGS (sizeof(AlertInputActiveLevels) / sizeof(uint8_t))



const char* AlertDebugMessages[] = {
		"Fault: Main current (Imain) too high", // 0
		"Fault: Main Voltage (Vmain) too high", // 1
		"Fault: Reverse Power 1 (Rev1) too high", // 2
		"Fault: Reverse Power 2 (Rev2) too high", // 3
		"Fault: Core Temperature (Temp1) too high",// 4
		"Fault: Ambient Temperature (Temp2) too high", // 5
		"Fault: RF Input Overdrive", // 6
};
const char* AlertGUIMessages[] = {
		"主电流过高",
		"主电压过高",
		"反射功率1过高",
		"反射功率2过高",
		"核心温度过高",
		"周边温度过高",
		"RF输入过载",
};


osMessageQueueId_t qAlerts;
void StartAlertTask()
{
	static uint8_t AlertTrigCounts[N_INPUT_LOGIC_ALERT_SIGS] = {0};
	uint8_t i;
#ifdef USING_MY_GUI
	// open files in advance
	const uint16_t* BMP_standby = (const uint16_t*)FS_open(&FS, FILENAME_BMP_STANDBY).p_content;
	const uint16_t* BMP_receiving = (const uint16_t*)FS_open(&FS, FILENAME_BMP_RECEIVING).p_content;
	const uint16_t* BMP_transmitting = (const uint16_t*)FS_open(&FS, FILENAME_BMP_TRANSMITTING).p_content;
	const uint16_t* BMP_Imain_High = (const uint16_t*)FS_open(&FS, FILENAME_BMP_ALERT_CONTENT_IMAINHIGH).p_content;
	const uint16_t* BMP_Vmain_High = (const uint16_t*)FS_open(&FS, FILENAME_BMP_ALERT_CONTENT_VMAINHIGH).p_content;
	const uint16_t* BMP_SWR1_High = (const uint16_t*)FS_open(&FS, FILENAME_BMP_ALERT_CONTENT_SWR1HIGH).p_content;
	const uint16_t* BMP_SWR2_High = (const uint16_t*)FS_open(&FS, FILENAME_BMP_ALERT_CONTENT_SWR2HIGH).p_content;
	const uint16_t* BMP_Temp1_High = (const uint16_t*)FS_open(&FS, FILENAME_BMP_ALERT_CONTENT_TEMP1HIGH).p_content;
	const uint16_t* BMP_Temp2_High = (const uint16_t*)FS_open(&FS, FILENAME_BMP_ALERT_CONTENT_TEMP2HIGH).p_content;
#endif

	for(;;)
	{
		/* Update Trans state */
		// GUI widgets will be updated in onTimerLVGL
		// to avoid spending too much time in alert processing
		if(HAL_GPIO_ReadPin(Transmit_GPIO_Port, Transmit_Pin) == GPIO_PIN_SET)
		{
			trans_state = TRANSMITTING;
		}
		else
		{
			if(!(PWR_on_sig_GPIO_Port->ODR & PWR_on_sig_Pin)) // if PWR_on_sig is low, the power is cut off. The state should be STANDBY
				trans_state = STANDBY;
			else
				trans_state = RECEIVING;
		}

		/* alert according to ISR mail box*/
		int alert_id;
		osStatus r = osMessageQueueGet(qAlerts, &alert_id, 0, 0);
		if(r == osOK)
		{
			show_msgbox_warning("警告", AlertGUIMessages[alert_id]);
			GUI_change_idle_status_indication(MainPowerEnabled);
		}

		osDelay(pdMS_TO_TICKS(100));
	}
}


void switch_band(band_t band)
{
	// Release all relays
	HAL_GPIO_WritePin(BAND_SEL1X_GPIO_Port, BAND_SEL1X_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL2X_GPIO_Port, BAND_SEL2X_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL3X_GPIO_Port, BAND_SEL3X_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL4X_GPIO_Port, BAND_SEL4X_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL5X_GPIO_Port, BAND_SEL5X_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL6X_GPIO_Port, BAND_SEL6X_Pin, GPIO_PIN_SET);
	switch(band)
	{
	case BAND_160M:
		HAL_GPIO_WritePin(BAND_SEL4X_GPIO_Port, BAND_SEL5X_Pin, GPIO_PIN_RESET);
		break;
	case BAND_80M:
		HAL_GPIO_WritePin(BAND_SEL4X_GPIO_Port, BAND_SEL4X_Pin, GPIO_PIN_RESET);
		break;
	case BAND_40M_30M:
		HAL_GPIO_WritePin(BAND_SEL3X_GPIO_Port, BAND_SEL3X_Pin, GPIO_PIN_RESET);
		break;
	case BAND_20M_17M:
		HAL_GPIO_WritePin(BAND_SEL2X_GPIO_Port, BAND_SEL2X_Pin, GPIO_PIN_RESET);
		break;
	case BAND_15M_10M:
		HAL_GPIO_WritePin(BAND_SEL1X_GPIO_Port, BAND_SEL1X_Pin, GPIO_PIN_RESET);
		break;
	default: // 15M_10M
		HAL_GPIO_WritePin(BAND_SEL1X_GPIO_Port, BAND_SEL1X_Pin, GPIO_PIN_RESET);
		break;
	}
}

// Human Input Task
void StartHumanInputTask()
{
	static band_t band_old = BAND_15M_10M;
	// Setup initial GPIO status
	/*
	 * SEL1 : 15-10M
	 * SEL2 : 20-17M
	 * SEL3 : 40-30M
	 * SEL4 : 80M
	 * SEL5,6 : 160m
	 */
	HAL_GPIO_WritePin(BAND_SEL1X_GPIO_Port, BAND_SEL1X_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL2X_GPIO_Port, BAND_SEL2X_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL3X_GPIO_Port, BAND_SEL3X_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL4X_GPIO_Port, BAND_SEL4X_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL5X_GPIO_Port, BAND_SEL5X_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL6X_GPIO_Port, BAND_SEL6X_Pin, GPIO_PIN_SET);

	switch_band(BAND_MAX);
	osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
	GUI_set_band_info(BAND_MAX);
	osMutexRelease(mtxGUIWidgetsHandle);
	for(;;)
	{
		osMutexAcquire(mtxBandDataHandle, osWaitForever);
		// WARNING: wrong pin assignment
//		band_t BAND_switch_new = BAND_15M_10M;
//		if(!(BAND_SEL1X_GPIO_Port->IDR & BAND_SEL1X_Pin))
//			BAND_switch_new = BAND_160M;
//		else if(!(BAND_SEL2X_GPIO_Port->IDR & BAND_SEL2X_Pin))
//			BAND_switch_new = BAND_80M;
//		else if(!(BAND_SEL3X_GPIO_Port->IDR & BAND_SEL3X_Pin))
//			BAND_switch_new = BAND_40M_30M;
//		else if(!(BAND_SEL4X_GPIO_Port->IDR & BAND_SEL4X_Pin))
//			BAND_switch_new = BAND_20M_17M;
//		else if(!(BAND_SEL5X_GPIO_Port->IDR & BAND_SEL5X_Pin))
//			BAND_switch_new = BAND_15M_10M;
//		else if(!(BAND_SEL6X_GPIO_Port->IDR & BAND_SEL6X_Pin))
//			BAND_switch_new = BAND_15M_10M;
//		else // No effective pin, then AUTO Band
//		{
//			if(Enable_band_remote)
//				cfg.Band_source = BAND_FROM_REMOTE;
//			else
//				cfg.Band_source = BAND_FROM_DECODER;
//		}
		// if the rotary switch changes, force to exit from Auto Band mode and Remote Band mode
		// DEBUG: no switch now
//		if(BAND_switch != BAND_switch_new)
//		{
//			cfg.Band_source = BAND_FROM_SWITCH;
//			BAND_switch = BAND_switch_new;
//		}
		// change band source
		band_t band;

		switch(cfg.Band_source)
		{
		case BAND_FROM_SWITCH:
			band = (BAND_switch);
			break;
		case BAND_FROM_REMOTE:
			band = (cfg.Band_soft);
			break;
		case BAND_FROM_DECODER:
			band = (BAND_decoder);
			break;
		default:
			band = BAND_switch;
		}
		osMutexRelease(mtxBandDataHandle);
		if(band != BAND_FAULT)
		{

			if(band != band_old)
			{
				// TODO: switch band
				switch_band(band);
				band_old = band;
				osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
				GUI_set_band_info(band);
				osMutexRelease(mtxGUIWidgetsHandle);
			}
		}
		else
		{
			if(band_old != BAND_15M_10M)
			{
				switch_band(BAND_15M_10M);
				band_old = BAND_15M_10M;
				osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
				GUI_set_band_info(band);
				osMutexRelease(mtxGUIWidgetsHandle);
			}
		}
		// Refresh GUI widgets
		osDelay(configTICK_RATE_HZ * 100/ 1000); // delay 100ms
	}
}


// Touch screen interrupt
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	switch(GPIO_Pin)
	{
#ifdef OPA2000
	case PTTX_Pin: // 9
		break;
#endif
	case T_INTX_Pin: // 15 or (11 on OPA1000)
		osThreadFlagsSet(TouchScreenTaskHandle, 0x10); // awaken touch screen handle task
		break;
	}
}

void StartTouchScreenTask()
{
	static uint16_t X[5], Y[5];
	static uint32_t avg_X, avg_Y;
	for(;;)
	{
		osDelay(200);
		// wait for Pen interrupt
		osThreadFlagsClear(0xffff);
		osThreadFlagsWait(0x10, osFlagsWaitAll, osWaitForever);
		// obtain touch screen values
		avg_X = 0;
		avg_Y = 0;
		taskENTER_CRITICAL();
		touch_read_XY_ADC(X, Y);
		taskEXIT_CRITICAL();
		if(X[0] > 0 && X[0] < 4095 && Y[0] > 0 && Y[0] < 4095)
		{
#ifdef DEBUG
			//GUI_set_FWD_peak_value(X[0]);
			//GUI_set_REV_peak_value(Y[0]);
#endif
		}
		else
			continue; // drop invalid results
		if(cfg.TSCalibInfo.isCalibrated) // calibration info exists
		{
			// read 5 times and take the avg
			for(int i = 0; i < 5; ++i)
			{
				taskENTER_CRITICAL();
				touch_read_XY_physical(X + i, Y + i);
				taskEXIT_CRITICAL();
				avg_X += X[i];
				avg_Y += Y[i];
			}
			TS_X = avg_X / 5;
			TS_Y = avg_Y / 5;
			TS_touched = true;
		}
		else if(isTSCalibrating) // is running calibrating tasks
		{
			// since no calibration info available
			// it is unable to convert ADC values to physical positions on the screen
			taskENTER_CRITICAL();
			for(int i = 0; i < 5; ++i)
			{
				touch_read_XY_ADC(&X[i], &Y[i]);
				avg_X += X[i];
				avg_Y += Y[i];

			}
			taskEXIT_CRITICAL();
			TS_X_ADC = avg_X / 5;
			TS_Y_ADC = avg_Y / 5;
			osThreadFlagsSet(TouchScreenCalibHandle, TOUCHSCREEN_CALIB_POINT_CAPTURED);
			osDelay(pdMS_TO_TICKS(300)); // delay 500ms
		}
		osDelay(pdMS_TO_TICKS(200));
	}
}

void StartTouchScreenCalibTask()
{
	for(;;)
	{
		static bool isPointConfirmed = false;
		uint8_t i;
		/* wait for a signal to start the task */
		if(!TSCalibAfterStart) // 如果设置在上电时即启动校准界面，则不用等待标志位
			osThreadFlagsWait(TOUCHSCREEN_CALIB_START, osFlagsWaitAll, osWaitForever);
		/* show calibration scene */
		osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
		init_touchscreen_calib_widgets();
		osMutexRelease(mtxGUIWidgetsHandle);
		osDelay((pdMS_TO_TICKS(500))); // delay 500ms
		isTSCalibrating = true;
		/* init calibration config */
		// Upper Left
		cfg.TSCalibInfo.real_X[0] = TOUCHSCREEN_CALIB_LEFT_MARGIN * LV_HOR_RES_MAX;
		cfg.TSCalibInfo.real_Y[0] = TOUCHSCREEN_CALIB_UPPER_MARGIN * LV_VER_RES_MAX;
		// Upper Right
		cfg.TSCalibInfo.real_X[1] = (1. - TOUCHSCREEN_CALIB_LEFT_MARGIN) * LV_HOR_RES_MAX;
		cfg.TSCalibInfo.real_Y[1] = TOUCHSCREEN_CALIB_UPPER_MARGIN * LV_VER_RES_MAX;
		// Lower Left
		cfg.TSCalibInfo.real_X[2] = TOUCHSCREEN_CALIB_LEFT_MARGIN * LV_HOR_RES_MAX;
		cfg.TSCalibInfo.real_Y[2] = (1. - TOUCHSCREEN_CALIB_UPPER_MARGIN) * LV_VER_RES_MAX;
		// Lower Right
		cfg.TSCalibInfo.real_X[3] = (1. - TOUCHSCREEN_CALIB_LEFT_MARGIN) * LV_HOR_RES_MAX;
		cfg.TSCalibInfo.real_Y[3] = (1. - TOUCHSCREEN_CALIB_UPPER_MARGIN) * LV_VER_RES_MAX;
		// Center
		cfg.TSCalibInfo.real_X[4] = 0.5 * LV_HOR_RES_MAX;
		cfg.TSCalibInfo.real_Y[4] = 0.5 * LV_VER_RES_MAX;
		/* Wait for touches */
		osThreadFlagsClear(0xffff); // clear all flags before entering calibration process
		for(i = 0; i < 5; ++i)
		{
			// GUI move touch point to desired position
			osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
			GUI_set_TSCalib_touchpoint_pos(cfg.TSCalibInfo.real_X[i], cfg.TSCalibInfo.real_Y[i]);
			osMutexRelease(mtxGUIWidgetsHandle);
			isPointConfirmed = false;
			while(!isPointConfirmed)
			{
				osThreadFlagsWait(TOUCHSCREEN_CALIB_POINT_CAPTURED, osFlagsWaitAll, osWaitForever);
				isPointConfirmed = true;
			}
			cfg.TSCalibInfo.ADC_X[i] = TS_X_ADC;
			cfg.TSCalibInfo.ADC_Y[i] = TS_Y_ADC;
		}
		touch_set_calibrate(cfg.TSCalibInfo.real_X, cfg.TSCalibInfo.real_Y,
				cfg.TSCalibInfo.ADC_X, cfg.TSCalibInfo.ADC_Y, &cfg.TSCalibInfo);
		isTSCalibrating = false;
		/* Touch screen calibration finished */
		taskENTER_CRITICAL();
		save_config();
		taskEXIT_CRITICAL();
		osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
		dismiss_touchscreen_calib_widgets();
		if(TSCalibAfterStart)
		{
			TSCalibAfterStart = false;
			init_main_widgets();
		}
		osMutexRelease(mtxGUIWidgetsHandle);
	}
}

bool lvGetTouchscreenXY(lv_indev_drv_t* drv, lv_indev_data_t* data)
{
	data->point.x = TS_X;
	data->point.y = TS_Y;
	data->state = (pTSCalib != NULL) && TS_touched;
	TS_touched = false;
	return false;
}

// TODO: implement the callback function for the rotary encoder
bool lvGetRotEnc(lv_indev_drv_t* drv, lv_indev_data_t* data)
{
	// Rotary
	static bool isInit = false;
	static int32_t prevCnt;
	if(!isInit)
		prevCnt = RotEnc.cntRot;
	int16_t encdiff = RotEnc.cntRot - prevCnt;
	if(encdiff > 0)
		data->enc_diff = 1;
	else if(encdiff < 0)
		data->enc_diff = -1;
	else
		data->enc_diff = 0;
	// SW
	if(RotEnc.SW.state == 0) // 0: Pressed (Button is low effective)
		data->state = LV_INDEV_STATE_PR;
	else
		data->state = LV_INDEV_STATE_REL;
	return false;

}

void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    signed char *pcTaskName )
{
	while(1);
}

void vApplicationMallocFailedHook( void )
{
	while(1);
}
/* USER CODE END 4 */

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used 
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN 5 */
  /* Infinite loop */
//  for(;;)
//  {
//    osDelay(100);
//  }
	for(;;)
	{
		LCD_Back_Light_ON;
		osThreadFlagsWait(THREAD_FLAG_SAVE_CONFIG, osFlagsWaitAll, osWaitForever);
		taskENTER_CRITICAL();
//		EEPROM_WriteBytes(&EEPROM, &cfg, sizeof(cfg)); // save config
		Flash_EEPROM_WriteBytes(&FlashEEPROM, &cfg, sizeof(cfg)); // save config
		taskEXIT_CRITICAL();
		osDelay(1000);

	}
  /* USER CODE END 5 */
}

/* onTimerLVGL function */
void onTimerLVGL(void *argument)
{
  /* USER CODE BEGIN onTimerLVGL */
  static uint16_t n = 0;
  static Transmission_State_t trans_state_old = STANDBY;
  lv_tick_inc(5);
  if((++n) % 3 == 0)
  {
	  osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
	  lv_task_handler();
	  osMutexRelease(mtxGUIWidgetsHandle);
//	  HAL_IWDG_Refresh(&hiwdg); // feed dog
  }
  /* Check transmitting state*/
  if(trans_state != trans_state_old) // Transmitting state changed
  {
	  osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
	  if(isMainWidgetsCreated)
	  {
		  if(MainPowerEnabled )
		  {
			  if(trans_state == TRANSMITTING)
			  {

				  lv_obj_set_style_local_bg_color(lvContTopBanner, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_TRANSMITTING);
				  lv_obj_set_style_local_bg_color(lvContBottomBanner, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_TRANSMITTING);
				  lv_obj_set_style_local_bg_color(lvBtnEnableMainPower, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_TRANSMITTING);
				  lv_label_set_text_static(lvLblTransmissionState, strTransmissionStateOnAir);
			  }
			  else
			  {
				  lv_obj_set_style_local_bg_color(lvContTopBanner, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_ACTIVE);
				  lv_obj_set_style_local_bg_color(lvContBottomBanner, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_ACTIVE);
				  lv_obj_set_style_local_bg_color(lvBtnEnableMainPower, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_ACTIVE);
				  lv_label_set_text_static(lvLblTransmissionState, strTransmissionStateActive);
			  }
		  }
		  else
		  {
				  lv_obj_set_style_local_bg_color(lvContTopBanner, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_IDLE);
				  lv_obj_set_style_local_bg_color(lvContBottomBanner, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_IDLE);
				  lv_obj_set_style_local_bg_color(lvBtnEnableMainPower, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_IDLE);
				  lv_label_set_text_static(lvLblTransmissionState, strTransmissionStateIdle);
		  }
	  }
	  osMutexRelease(mtxGUIWidgetsHandle);
	  trans_state_old = trans_state;
  }
  /* Check MAIN main power state each 500ms */
  // Never change power state when transmitting
  if(n % 100 == 0 && !isTransmitting) // check main power state each 1 sec
  {
	  if(MainPowerEnabled)
	  {
		  if(!power_on())
		  {
			  MainPowerEnabled = false;
			  show_msgbox_warning("警告", "主电源错");

		  }
	  }
	  else
	  {
		  power_off();
	  }
  }

  /* Tick rotary encoder each 5ms */
  RotEnc_tick(&RotEnc, 5000);

  /* USER CODE END onTimerLVGL */
}

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM1 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM1) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */
  if(htim->Instance == TIM3)
  {
	  /* tick PTT delay */
  }
  // use TIM4 as 1ms time base
  if (htim->Instance == TIM4)
  {
	  /* tick fan PWM */
	  fan_tick(&fan1, 1000, cfg.Fan1Auto, Temp1);
	  fan_tick(&fan2, 1000, cfg.Fan2Auto, Temp2);
	  /* tick Software Soft UART */
	  SoftUartHandler();
  }
  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
