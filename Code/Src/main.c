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
#include "COM.h"
#include "Flash_EEPROM.h"
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
#include <time.h>
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
TIM_HandleTypeDef htim4;

UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;

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

volatile bool TSCalibAfterStart = false;
volatile bool isTSCalibrating = false;//indicate if the user is running touch screen calibration
static volatile bool TS_touched = false;
static volatile uint16_t TS_X, TS_Y; // the latest touch screen X, Y position
static volatile uint16_t TS_X_ADC, TS_Y_ADC; // the latest touch screen X,Y ADC values
osThreadId_t TouchScreenTaskHandle;
void StartTouchScreenTask();
osThreadId_t TouchScreenCalibHandle; // Touch screen calibration task, shows the calibration screen and takes 5 points
void StartTouchScreenCalibTask();
osMessageQueueId_t COMBytesToSendQHandle;
osMessageQueueId_t COMBytesRecvedQHandle;
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
// System Status
Transmission_State_t  trans_state = STANDBY; // transmission_state: STANDBY, RECEIVING, TRANSMITTING
// Menu configurations
bool Enable_band_remote = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_FSMC_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_ADC1_Init(void);
static void MX_ADC3_Init(void);
static void MX_CAN2_Init(void);
static void MX_CRC_Init(void);
static void MX_TIM2_Init(void);
static void MX_TIM4_Init(void);
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

bool lvGetTouchscreenXY(lv_indev_drv_t* drv, lv_indev_data_t* data);

void startTouchScreenCalibTask()
{
	if(TouchScreenCalibHandle != NULL)
	{
		osThreadFlagsSet(TouchScreenCalibHandle, TOUCHSCREEN_CALIB_START);
	}

}

void save_config()
{
	EEPROM_WriteBytes(&EEPROM, &cfg, sizeof(cfg));
}

// power_off: turn off Vmain
void power_off()
{
	HAL_GPIO_WritePin(PWR_on_sig_GPIO_Port, PWR_on_sig_Pin, GPIO_PIN_SET);
}

// power_on: turn on Vmain
void power_on()
{
	HAL_GPIO_WritePin(PWR_on_sig_GPIO_Port, PWR_on_sig_Pin, GPIO_PIN_RESET);
}

void clear_fault_hold()
{
	HAL_GPIO_WritePin(CLEAR_FAULT_GPIO_Port, CLEAR_FAULT_Pin, GPIO_PIN_RESET);
}

void clear_fault_release()
{
	HAL_GPIO_WritePin(CLEAR_FAULT_GPIO_Port, CLEAR_FAULT_Pin, GPIO_PIN_SET);
}

void clear_fault()
{
	uint8_t i=100;
	while(i--)
		HAL_GPIO_WritePin(CLEAR_FAULT_GPIO_Port, CLEAR_FAULT_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(CLEAR_FAULT_GPIO_Port, CLEAR_FAULT_Pin, GPIO_PIN_SET);
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
  // Load Config
  EEPROM_ReadBytes(&EEPROM, (uint8_t*)&cfg, sizeof(cfg));
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

  /* init LVGL */
  lv_init();
  // init screen
  static lv_disp_buf_t disp_buf;
  static lv_color_t lv_buf_mem[LV_HOR_RES_MAX * LV_VER_RES_MAX / 8];                     /*Declare a buffer for 48 lines*/
  lv_disp_buf_init(&disp_buf, lv_buf_mem, NULL, sizeof(lv_buf_mem) / sizeof(lv_color_t));    /*Initialize the display buffer*/
  lv_disp_drv_t disp_drv;               /*Descriptor of a display driver*/
  lv_disp_drv_init(&disp_drv);          /*Basic initialization*/
  disp_drv.flush_cb = LCD_LVGL_flush;    /*Set your driver function*/
  disp_drv.buffer = &disp_buf;          /*Assign the buffer to the display*/
  lv_disp_drv_register(&disp_drv);      /*Finally register the driver*/
  // init touch screen
  lv_indev_drv_t lvIndev_drv_touchscreen;
  lv_indev_drv_init(&lvIndev_drv_touchscreen);      /*Basic initialization*/
  lvIndev_drv_touchscreen.type = LV_INDEV_TYPE_POINTER;
  lvIndev_drv_touchscreen.read_cb = lvGetTouchscreenXY;
  /*Register the driver in LVGL and save the created input device object*/
  lv_indev_t * lvIndev_touchscreen = lv_indev_drv_register(&lvIndev_drv_touchscreen);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_FSMC_Init();
  MX_SPI1_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_ADC1_Init();
  MX_ADC3_Init();
  MX_CAN2_Init();
  MX_CRC_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  // Make sure main power is off (Vmain)
  power_off();
  //DEBUG: Invalidate Fault
  clear_fault_release();
  clear_fault_hold();
  // Saved Config: Load previous Fan settings
  fan_init(&fan1, FAN1_GPIO_Port, FAN1_Pin);
  fan_init_automode(&fan1, N_SEGS_AUTO_FAN_SPEED, AutoFanSpeed_tempC1, AutoFanSpeed_speed1, AutoFanSpeed_hyst1);
  fan_init(&fan2, FAN2_GPIO_Port, FAN2_Pin);
  fan_init_automode(&fan1, N_SEGS_AUTO_FAN_SPEED, AutoFanSpeed_tempC2, AutoFanSpeed_speed2, AutoFanSpeed_hyst2);
//  soft_pwm_driver_set_duty(&fan1, cfg.Fan1Speed);
//  soft_pwm_driver_set_duty(&fan2, cfg.Fan2Speed);

  //while(1)
  LCD_Init();
  // init GUI
//  if(!TSCalibAfterStart)
  init_main_widgets(); // 如果要求上电时就校准触摸屏，则不加载主界�???????????????
//  else
//	  init_touchscreen_calib_widgets();
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  const osMutexAttr_t mtxBandData_attr = {.name = "mtxBandData"};
  mtxBandDataHandle = osMutexNew(&mtxBandData_attr);
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
		  .stack_size = 1024
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
		  .stack_size = 1024 // 1KBytes
  };
  HumanInputTaskHandle = osThreadNew(StartHumanInputTask, NULL, &HumanInputTask_attr);
  const osThreadAttr_t TouchScreenTask_attr={
		  .name = "TouchScreenTask",
		  .priority = (osPriority_t)osPriorityAboveNormal,
		  .stack_size = 1024
  };
  TouchScreenTaskHandle = osThreadNew(StartTouchScreenTask, NULL, &TouchScreenTask_attr);

  static const osThreadAttr_t TouchScreenCalibTask_attr={
		  .name = "TouchScreenCalibTask",
		  .priority = (osPriority_t)osPriorityBelowNormal,
		  .stack_size = 1024
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
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 160;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV8;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
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
  hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_CC2;
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
  hadc3.Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T2_CC2;
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
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 5999;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 499;
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
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_OC2REF;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM2;
  sConfigOC.Pulse = 250;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */

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
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
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
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

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
  HAL_GPIO_WritePin(GPIOE, BAND_SEL6_Pin|BAND_SEL5_Pin|BAND_SEL4_Pin|BAND_SEL3_Pin
                          |BAND_SEL2_Pin|PWR_on_sig_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, BAND_SEL1_Pin|BAND_SEL_AUTO_Pin|LCD_RST_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, MCU_STATE_LED_Pin|CLEAR_FAULT_Pin|ALC_cancel_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LCD_BL_GPIO_Port, LCD_BL_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, FAN1_Pin|FAN2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(T_CS_GPIO_Port, T_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin : BAND_SEL6_Pin */
  GPIO_InitStruct.Pin = BAND_SEL6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BAND_SEL6_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : BAND_SEL5_Pin BAND_SEL4_Pin BAND_SEL3_Pin BAND_SEL2_Pin
                           PWR_on_sig_Pin */
  GPIO_InitStruct.Pin = BAND_SEL5_Pin|BAND_SEL4_Pin|BAND_SEL3_Pin|BAND_SEL2_Pin
                          |PWR_on_sig_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : BAND_SEL1_Pin BAND_SEL_AUTO_Pin LCD_RST_Pin */
  GPIO_InitStruct.Pin = BAND_SEL1_Pin|BAND_SEL_AUTO_Pin|LCD_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PC15 PC5 PC7 PC9 */
  GPIO_InitStruct.Pin = GPIO_PIN_15|GPIO_PIN_5|GPIO_PIN_7|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : MCU_STATE_LED_Pin ALC_cancel_Pin */
  GPIO_InitStruct.Pin = MCU_STATE_LED_Pin|ALC_cancel_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : BAND_Pin Transmit_Pin TEMP2_HIGH_Pin */
  GPIO_InitStruct.Pin = BAND_Pin|Transmit_Pin|TEMP2_HIGH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PB2 PB14 PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_2|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_BL_Pin T_CS_Pin */
  GPIO_InitStruct.Pin = LCD_BL_Pin|T_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : T_INT_Pin */
  GPIO_InitStruct.Pin = T_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(T_INT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : PD11 PD12 */
  GPIO_InitStruct.Pin = GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PC8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : CLEAR_FAULT_Pin */
  GPIO_InitStruct.Pin = CLEAR_FAULT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(CLEAR_FAULT_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : TEMP1_HIGH_Pin IMAIN_HIGH_Pin VMAIN_HIGH_Pin */
  GPIO_InitStruct.Pin = TEMP1_HIGH_Pin|IMAIN_HIGH_Pin|VMAIN_HIGH_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : FAN1_Pin FAN2_Pin */
  GPIO_InitStruct.Pin = FAN1_Pin|FAN2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : REV2_HIGH_Pin REV1_HIGH_Pin T_BUSY_Pin */
  GPIO_InitStruct.Pin = REV2_HIGH_Pin|REV1_HIGH_Pin|T_BUSY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PE1 */
  GPIO_InitStruct.Pin = GPIO_PIN_1;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 7, 0);
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

void Emergency_stop()
{
	HAL_GPIO_WritePin(PWR_on_sig_GPIO_Port, PWR_on_sig_Pin, GPIO_PIN_RESET);
}
// Alert handling task, where binary inputs are processed
// TODO design Alert window
void StartAlertTask()
{
	for(;;)
	{
		osDelay(10000);
	}
	bool Imain_high = false;
	bool Vmain_high = false;
	bool SWR1_high = false;
	bool SWR2_high = false;
	bool Temp1_high = false;
	bool Temp2_high = false;
	Transmission_State_t trans_state_new;
	bool trans_state_changed = false;
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
		Imain_high = false;
		Vmain_high = false;
		SWR1_high = false;
		SWR2_high = false;
		Temp1_high = false;
		Temp2_high = false;
		// PTT
		if(Transmit_GPIO_Port->IDR & Transmit_Pin)
		{
			// cancel ALC supression
			ALC_cancel_GPIO_Port->BSRR = ALC_cancel_Pin;
			trans_state_new = TRANSMITTING;
		}
		else
		{
			// Resume ALC supress voltage
			HAL_GPIO_WritePin(ALC_cancel_GPIO_Port, ALC_cancel_Pin, GPIO_PIN_RESET);
			if(!(PWR_on_sig_GPIO_Port->ODR & PWR_on_sig_Pin)) // if PWR_on_sig is low, the power is cut off. The state should be STANDBY
				trans_state_new = STANDBY;
			else
				trans_state_new = RECEIVING;
		}
		if(trans_state_new != trans_state) trans_state_changed= true;
		trans_state = trans_state_new;
		// Imain High
		if(IMAIN_HIGH_GPIO_Port->IDR & IMAIN_HIGH_Pin)
		{
			Emergency_stop();
			COM_send_message("Fault: Over-current protection triggered");
			Imain_high = true;
			osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
			show_msgbox_warning("警告", "主电流过�???????????????");
			osMutexRelease(mtxGUIWidgetsHandle);
			osSemaphoreAcquire(sphWarnMsgBoxDismissed, osWaitForever);
		}
		// Vmain High
		if(VMAIN_HIGH_GPIO_Port->IDR & VMAIN_HIGH_Pin)
		{
			Emergency_stop();
			COM_send_message("Fault: Over-voltage protection triggered");
			Vmain_high = true;
			osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
			show_msgbox_warning("警告", "主电压过�???????????????");
			osMutexRelease(mtxGUIWidgetsHandle);
			osSemaphoreAcquire(sphWarnMsgBoxDismissed, osWaitForever);
		}
		// RevPwr_high
		if(REV1_HIGH_GPIO_Port->IDR & REV1_HIGH_Pin)
		{
			Emergency_stop();
			COM_send_message("Fault: Mismatch protection triggered (PA --> LPF)");
			SWR1_high = true;
			osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
			show_msgbox_warning("警告", "来自滤波器的反射功率过高");
			osMutexRelease(mtxGUIWidgetsHandle);
			osSemaphoreAcquire(sphWarnMsgBoxDismissed, osWaitForever);
		}
		if(REV2_HIGH_GPIO_Port->IDR & REV2_HIGH_Pin)
		{
			Emergency_stop();
			COM_send_message("Fault: Mismatch protection triggered (LPF --> ANT)");
			SWR2_high = true;
			osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
			show_msgbox_warning("警告", "来自天线的反射功率过�???????????????");
			osMutexRelease(mtxGUIWidgetsHandle);
			osSemaphoreAcquire(sphWarnMsgBoxDismissed, osWaitForever);
		}
		// Temp1_high
		if(TEMP1_HIGH_GPIO_Port->IDR & TEMP1_HIGH_Pin)
		{
			Emergency_stop();
			COM_send_message("Fault: Over temperature protection triggered (Core)");
			Temp1_high = true;
			osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
			show_msgbox_warning("警告", "核心温度过高");
			osMutexRelease(mtxGUIWidgetsHandle);
			osSemaphoreAcquire(sphWarnMsgBoxDismissed, osWaitForever);
		}
		// Temp2_high
		if(TEMP2_HIGH_GPIO_Port->IDR & TEMP2_HIGH_Pin)
		{
			Emergency_stop();
			COM_send_message("Fault: Over temperature protection triggered (Ambient)");
			Temp2_high = true;
			osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
			show_msgbox_warning("警告", "电压过高");
			osMutexRelease(mtxGUIWidgetsHandle);
			osSemaphoreAcquire(sphWarnMsgBoxDismissed, osWaitForever);
		}
		// Overdrive

		osDelay(pdMS_TO_TICKS(100));
	}
}


void switch_band(band_t band)
{
	// Release all relays
	HAL_GPIO_WritePin(BAND_SEL1_GPIO_Port, BAND_SEL1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL2_GPIO_Port, BAND_SEL2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL3_GPIO_Port, BAND_SEL3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL4_GPIO_Port, BAND_SEL4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL5_GPIO_Port, BAND_SEL5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL6_GPIO_Port, BAND_SEL6_Pin, GPIO_PIN_SET);
	switch(band)
	{
//	case BAND_160M:
//		HAL_GPIO_WritePin(BAND_SEL5_GPIO_Port, BAND_SEL5_Pin, GPIO_PIN_RESET);
//		break;
	case BAND_80M:
		HAL_GPIO_WritePin(BAND_SEL4_GPIO_Port, BAND_SEL4_Pin, GPIO_PIN_RESET);
		break;
	case BAND_40M_30M:
		HAL_GPIO_WritePin(BAND_SEL3_GPIO_Port, BAND_SEL3_Pin, GPIO_PIN_RESET);
		break;
	case BAND_20M_17M:
		HAL_GPIO_WritePin(BAND_SEL2_GPIO_Port, BAND_SEL2_Pin, GPIO_PIN_RESET);
		break;
	case BAND_15M_10M:
		HAL_GPIO_WritePin(BAND_SEL1_GPIO_Port, BAND_SEL1_Pin, GPIO_PIN_RESET);
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
	HAL_GPIO_WritePin(BAND_SEL1_GPIO_Port, BAND_SEL1_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL2_GPIO_Port, BAND_SEL2_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL3_GPIO_Port, BAND_SEL3_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL4_GPIO_Port, BAND_SEL4_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL5_GPIO_Port, BAND_SEL5_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL6_GPIO_Port, BAND_SEL6_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(BAND_SEL_AUTO_GPIO_Port, BAND_SEL_AUTO_Pin, GPIO_PIN_SET);
	switch_band(BAND_switch);
	for(;;)
	{
		osMutexAcquire(mtxBandDataHandle, osWaitForever);
		band_t BAND_switch_new = BAND_15M_10M;
		if(!(BAND_SEL1_GPIO_Port->IDR & BAND_SEL1_Pin))
			BAND_switch_new = BAND_160M;
		else if(!(BAND_SEL2_GPIO_Port->IDR & BAND_SEL2_Pin))
			BAND_switch_new = BAND_80M;
		else if(!(BAND_SEL3_GPIO_Port->IDR & BAND_SEL3_Pin))
			BAND_switch_new = BAND_40M_30M;
		else if(!(BAND_SEL4_GPIO_Port->IDR & BAND_SEL4_Pin))
			BAND_switch_new = BAND_20M_17M;
		else if(!(BAND_SEL5_GPIO_Port->IDR & BAND_SEL5_Pin))
			BAND_switch_new = BAND_15M_10M;
		else if(!(BAND_SEL6_GPIO_Port->IDR & BAND_SEL6_Pin))
			BAND_switch_new = BAND_15M_10M;
		else if(!(BAND_SEL_AUTO_GPIO_Port->IDR & BAND_SEL_AUTO_Pin))
		{
			if(Enable_band_remote)
				cfg.Band_source = BAND_FROM_REMOTE;
			else
				cfg.Band_source = BAND_FROM_DECODER;
		}
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
		if(band != band_old)
		{
			// TODO: switch band
			switch_band(band);
			band_old = band;
		}
		// Refresh GUI widgets
		osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
		GUI_set_band_info(band);
		osMutexRelease(mtxGUIWidgetsHandle);
		osDelay(configTICK_RATE_HZ * 100/ 1000); // delay 100ms
	}
}


// Touch screen interrupt
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(HAL_GPIO_ReadPin(T_INT_GPIO_Port, T_INT_Pin) == GPIO_PIN_RESET)
	{
		osThreadFlagsSet(TouchScreenTaskHandle, 0x10); // awaken touch screen handle task
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

void vApplicationStackOverflowHook( TaskHandle_t xTask,
                                    signed char *pcTaskName )
{
	while(1);
}
/* 追踪Malloc失败的回调函�???????????? */
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
//		osThreadFlagsWait(OS_FLAG_POWEROFF, osFlagsWaitAll, osWaitForever);
//		taskENTER_CRITICAL();
//		EEPROM_WriteBytes(&EEPROM, &cfg, sizeof(cfg)); // save config
//		taskEXIT_CRITICAL();
//		power_off();
		osThreadFlagsWait(THREAD_FLAG_SAVE_CONFIG, osFlagsWaitAll, osWaitForever);
		taskENTER_CRITICAL();
		EEPROM_WriteBytes(&EEPROM, &cfg, sizeof(cfg)); // save config
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
  static bool isTransmitting_old = false;
  lv_tick_inc(5);
  if((++n) % 3 == 0)
  {
	  osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
	  lv_task_handler();
	  osMutexRelease(mtxGUIWidgetsHandle);
//	  HAL_IWDG_Refresh(&hiwdg); // feed dog
  }
  /* Check transmitting state*/
  isTransmitting = (HAL_GPIO_ReadPin(Transmit_GPIO_Port, Transmit_Pin) == GPIO_PIN_SET);
  if(isTransmitting != isTransmitting_old) // Transmitting state changed
  {
	  if(isMainWidgetsCreated)
	  {
		  osMutexAcquire(mtxGUIWidgetsHandle, osWaitForever);
		  if(MainPowerEnabled && isTransmitting)
		  {
			  lv_obj_set_style_local_bg_color(lvContTopBanner, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_TRANSMITTING);
			  lv_obj_set_style_local_bg_color(lvContBottomBanner, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_TRANSMITTING);
			  lv_obj_set_style_local_bg_color(lvBtnEnableMainPower, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_TRANSMITTING);
			  lv_label_set_text_static(lvLblTransmissionState, strTransmissionStateOnAir);
		  }
		  else
		  {
			  if(MainPowerEnabled)
			  {
				  lv_obj_set_style_local_bg_color(lvContTopBanner, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_ACTIVE);
				  lv_obj_set_style_local_bg_color(lvContBottomBanner, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_ACTIVE);
				  lv_obj_set_style_local_bg_color(lvBtnEnableMainPower, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_ACTIVE);
				  lv_label_set_text_static(lvLblTransmissionState, strTransmissionStateActive);
			  }
			  else
			  {
				  lv_obj_set_style_local_bg_color(lvContTopBanner, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_IDLE);
				  lv_obj_set_style_local_bg_color(lvContBottomBanner, LV_OBJ_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_IDLE);
				  lv_obj_set_style_local_bg_color(lvBtnEnableMainPower, LV_BTN_PART_MAIN, LV_STATE_DEFAULT, COLOR_BANNER_IDLE);
				  lv_label_set_text_static(lvLblTransmissionState, strTransmissionStateOnAir);
			  }
		  }
		  osMutexRelease(mtxGUIWidgetsHandle);
	  }
	  isTransmitting_old = isTransmitting;
  }
  /* Check MAIN main power state each 1 second */
  // Never change power state when transmitting
  if(n % 200 == 0 && !isTransmitting) // check main power state each 1 sec
  {
	  if(MainPowerEnabled)
	  {
		  power_on();
	  }
	  else
	  {
		  power_off();
	  }
  }
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
  // tick fan PWM drivers
  if (htim->Instance == TIM1) {
	  fan_tick(&fan1, 1000, cfg.Fan1Auto, Temp1);
	  fan_tick(&fan2, 1000, cfg.Fan2Auto, Temp2);
	  // Auto Fan
	  if(cfg.Fan1Auto) // core fan
	  {
		  // get current core temperature
	  }
  }
  else if (htim->Instance == TIM4)
  {
	  // tick Software UART
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