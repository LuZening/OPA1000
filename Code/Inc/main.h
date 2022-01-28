/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "FS.h"
#include "cmsis_os.h"
#include "Sensor_task.h"
#include "RadioTypes.h"
#include "persistent.h"
#include "SoftPWMDriver.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
extern  osMutexId_t mtxBandDataHandle;
extern osEventFlagsId_t SensorDataReadyEvents_handle;
//extern osSemaphoreId_t GUIDataUpdatedSemaphore_handle;
extern osMessageQueueId_t COMBytesToSendQHandle;
extern osMessageQueueId_t COMBytesRecvedQHandle;
extern osThreadId_t defaultTaskHandle;
/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

#define OS_FLAG_POWEROFF 0x02

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define BAND_SEL6_Pin GPIO_PIN_2
#define BAND_SEL6_GPIO_Port GPIOE
#define BAND_SEL5_Pin GPIO_PIN_3
#define BAND_SEL5_GPIO_Port GPIOE
#define BAND_SEL4_Pin GPIO_PIN_4
#define BAND_SEL4_GPIO_Port GPIOE
#define BAND_SEL3_Pin GPIO_PIN_5
#define BAND_SEL3_GPIO_Port GPIOE
#define BAND_SEL2_Pin GPIO_PIN_6
#define BAND_SEL2_GPIO_Port GPIOE
#define BAND_SEL1_Pin GPIO_PIN_13
#define BAND_SEL1_GPIO_Port GPIOC
#define BAND_SEL_AUTO_Pin GPIO_PIN_14
#define BAND_SEL_AUTO_GPIO_Port GPIOC
#define MCU_STATE_LED_Pin GPIO_PIN_0
#define MCU_STATE_LED_GPIO_Port GPIOA
#define BAND_Pin GPIO_PIN_1
#define BAND_GPIO_Port GPIOA
#define LCD_BL_Pin GPIO_PIN_10
#define LCD_BL_GPIO_Port GPIOB
#define T_INT_Pin GPIO_PIN_11
#define T_INT_GPIO_Port GPIOB
#define T_INT_EXTI_IRQn EXTI15_10_IRQn
#define LCD_RS_Pin GPIO_PIN_13
#define LCD_RS_GPIO_Port GPIOD
#define LCD_RST_Pin GPIO_PIN_6
#define LCD_RST_GPIO_Port GPIOC
#define CLEAR_FAULT_Pin GPIO_PIN_8
#define CLEAR_FAULT_GPIO_Port GPIOA
#define ALC_cancel_Pin GPIO_PIN_11
#define ALC_cancel_GPIO_Port GPIOA
#define Transmit_Pin GPIO_PIN_12
#define Transmit_GPIO_Port GPIOA
#define TEMP2_HIGH_Pin GPIO_PIN_15
#define TEMP2_HIGH_GPIO_Port GPIOA
#define TEMP1_HIGH_Pin GPIO_PIN_10
#define TEMP1_HIGH_GPIO_Port GPIOC
#define IMAIN_HIGH_Pin GPIO_PIN_11
#define IMAIN_HIGH_GPIO_Port GPIOC
#define VMAIN_HIGH_Pin GPIO_PIN_12
#define VMAIN_HIGH_GPIO_Port GPIOC
#define FAN1_Pin GPIO_PIN_2
#define FAN1_GPIO_Port GPIOD
#define FAN2_Pin GPIO_PIN_3
#define FAN2_GPIO_Port GPIOD
#define LCD_RD_Pin GPIO_PIN_4
#define LCD_RD_GPIO_Port GPIOD
#define LCD_WR_Pin GPIO_PIN_5
#define LCD_WR_GPIO_Port GPIOD
#define LCD_CS_Pin GPIO_PIN_7
#define LCD_CS_GPIO_Port GPIOD
#define REV2_HIGH_Pin GPIO_PIN_6
#define REV2_HIGH_GPIO_Port GPIOB
#define REV1_HIGH_Pin GPIO_PIN_7
#define REV1_HIGH_GPIO_Port GPIOB
#define T_BUSY_Pin GPIO_PIN_8
#define T_BUSY_GPIO_Port GPIOB
#define T_CS_Pin GPIO_PIN_9
#define T_CS_GPIO_Port GPIOB
#define PWR_on_sig_Pin GPIO_PIN_0
#define PWR_on_sig_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */



// Band selector
extern uint8_t BAND_source; // select the control source of auto band
extern band_t BAND_switch; // the position of Band Rotary Switch
extern band_t BAND_soft; // Band set by software
// Current status
extern Transmission_State_t  trans_state; // transmission_state: STANDBY, RECEIVING, TRANSMITTING

// Menu configurations (to be save in EEPROM)
extern bool Enable_band_remote;

// Fan1, Fan2
extern SoftPWMDriver_t fan1, fan2;

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
