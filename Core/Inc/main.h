/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Bat_Fans_Fault_Pin GPIO_PIN_3
#define Bat_Fans_Fault_GPIO_Port GPIOE
#define Pump1_SW_Pin GPIO_PIN_0
#define Pump1_SW_GPIO_Port GPIOC
#define Pump2_SW_Pin GPIO_PIN_1
#define Pump2_SW_GPIO_Port GPIOC
#define Boards_Current_Pin GPIO_PIN_2
#define Boards_Current_GPIO_Port GPIOC
#define Shutdown_Current_Pin GPIO_PIN_3
#define Shutdown_Current_GPIO_Port GPIOC
#define LED_R_Pin GPIO_PIN_0
#define LED_R_GPIO_Port GPIOA
#define LED_G_Pin GPIO_PIN_1
#define LED_G_GPIO_Port GPIOA
#define LED_B_Pin GPIO_PIN_2
#define LED_B_GPIO_Port GPIOA
#define Rad_Fans_Current_Pin GPIO_PIN_3
#define Rad_Fans_Current_GPIO_Port GPIOA
#define Thermistor_Pin GPIO_PIN_4
#define Thermistor_GPIO_Port GPIOA
#define Pump_2_Current_Pin GPIO_PIN_5
#define Pump_2_Current_GPIO_Port GPIOA
#define Status_Red_Current_Pin GPIO_PIN_6
#define Status_Red_Current_GPIO_Port GPIOA
#define Pump_1_Current_Pin GPIO_PIN_7
#define Pump_1_Current_GPIO_Port GPIOA
#define Shutdown_Fault_Pin GPIO_PIN_4
#define Shutdown_Fault_GPIO_Port GPIOC
#define Brake_Lights_Current_Pin GPIO_PIN_5
#define Brake_Lights_Current_GPIO_Port GPIOC
#define Status_Green_Current_Pin GPIO_PIN_0
#define Status_Green_Current_GPIO_Port GPIOB
#define Bat_Fans_Current_Pin GPIO_PIN_1
#define Bat_Fans_Current_GPIO_Port GPIOB
#define Boards_SW_Pin GPIO_PIN_2
#define Boards_SW_GPIO_Port GPIOB
#define Status_Red_Fault_Pin GPIO_PIN_11
#define Status_Red_Fault_GPIO_Port GPIOE
#define Brake_Light_Switch_Pin GPIO_PIN_10
#define Brake_Light_Switch_GPIO_Port GPIOB
#define Status_Red_Switch_Pin GPIO_PIN_11
#define Status_Red_Switch_GPIO_Port GPIOB
#define Status_Green_Switch_Pin GPIO_PIN_14
#define Status_Green_Switch_GPIO_Port GPIOB
#define Shutdown_BSPD_Switch_Pin GPIO_PIN_15
#define Shutdown_BSPD_Switch_GPIO_Port GPIOB
#define Brake_Light_Fault_Pin GPIO_PIN_8
#define Brake_Light_Fault_GPIO_Port GPIOA
#define Radiator_Fans_Pin GPIO_PIN_9
#define Radiator_Fans_GPIO_Port GPIOA
#define Bat_Fans_SW_Pin GPIO_PIN_4
#define Bat_Fans_SW_GPIO_Port GPIOB
#define Pump1_Fault_Pin GPIO_PIN_5
#define Pump1_Fault_GPIO_Port GPIOB
#define Pump2_Fault_Pin GPIO_PIN_6
#define Pump2_Fault_GPIO_Port GPIOB
#define Radiator_Fans_SW_Pin GPIO_PIN_7
#define Radiator_Fans_SW_GPIO_Port GPIOB
#define Boards_Fault_Pin GPIO_PIN_0
#define Boards_Fault_GPIO_Port GPIOE
#define Status_Green_Fault_Pin GPIO_PIN_1
#define Status_Green_Fault_GPIO_Port GPIOE

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
