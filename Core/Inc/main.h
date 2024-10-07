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
#define Temp3_Analog_Pin GPIO_PIN_0
#define Temp3_Analog_GPIO_Port GPIOC
#define Flow_Rate_Analog_Pin GPIO_PIN_1
#define Flow_Rate_Analog_GPIO_Port GPIOC
#define Temp2_Analog_Pin GPIO_PIN_3
#define Temp2_Analog_GPIO_Port GPIOA
#define Brake_Light_Current_Pin GPIO_PIN_6
#define Brake_Light_Current_GPIO_Port GPIOA
#define Radiator_Fans_Current_Pin GPIO_PIN_7
#define Radiator_Fans_Current_GPIO_Port GPIOA
#define Status_Red_Current_Pin GPIO_PIN_4
#define Status_Red_Current_GPIO_Port GPIOC
#define Status_Green_Current_Pin GPIO_PIN_5
#define Status_Green_Current_GPIO_Port GPIOC
#define Pump2_Current_Pin GPIO_PIN_1
#define Pump2_Current_GPIO_Port GPIOB
#define Brake_Light_Fault_Pin GPIO_PIN_11
#define Brake_Light_Fault_GPIO_Port GPIOE
#define Radiator_Fans_Fault_Pin GPIO_PIN_12
#define Radiator_Fans_Fault_GPIO_Port GPIOE
#define Status_Red_Fault_Pin GPIO_PIN_13
#define Status_Red_Fault_GPIO_Port GPIOE
#define Pump1_Fault_Pin GPIO_PIN_15
#define Pump1_Fault_GPIO_Port GPIOE
#define Pump2_Fault_Pin GPIO_PIN_10
#define Pump2_Fault_GPIO_Port GPIOB
#define LED_R_Pin GPIO_PIN_13
#define LED_R_GPIO_Port GPIOB
#define LED_G_Pin GPIO_PIN_14
#define LED_G_GPIO_Port GPIOB
#define LED_B_Pin GPIO_PIN_15
#define LED_B_GPIO_Port GPIOB
#define Status_Red_Switch_Pin GPIO_PIN_6
#define Status_Red_Switch_GPIO_Port GPIOC
#define Break_Light_Switch_Pin GPIO_PIN_7
#define Break_Light_Switch_GPIO_Port GPIOC
#define Pump2_Switch_Pin GPIO_PIN_8
#define Pump2_Switch_GPIO_Port GPIOC
#define Pump1_Switch_Pin GPIO_PIN_9
#define Pump1_Switch_GPIO_Port GPIOC
#define Radiator_Fans_Switch_Pin GPIO_PIN_8
#define Radiator_Fans_Switch_GPIO_Port GPIOA
#define SPI1_CS_Pin GPIO_PIN_5
#define SPI1_CS_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
