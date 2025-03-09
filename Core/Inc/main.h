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
#define REVB
#ifndef SELF_BOOT_DFU
#define SELF_BOOT_DFU
#endif

#ifndef USB_VCP
#define USB_VCP
#endif
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
#define BSPD1_Pin GPIO_PIN_2
#define BSPD1_GPIO_Port GPIOE
#define BSPD2_Pin GPIO_PIN_3
#define BSPD2_GPIO_Port GPIOE
#define LED_B_Pin GPIO_PIN_4
#define LED_B_GPIO_Port GPIOE
#define LED_R_Pin GPIO_PIN_5
#define LED_R_GPIO_Port GPIOE
#define LED_G_Pin GPIO_PIN_6
#define LED_G_GPIO_Port GPIOE
#define BSPD3_Pin GPIO_PIN_13
#define BSPD3_GPIO_Port GPIOC
#define BSPD4_Pin GPIO_PIN_14
#define BSPD4_GPIO_Port GPIOC
#define BSPD5_Pin GPIO_PIN_15
#define BSPD5_GPIO_Port GPIOC
#define FUSE1_Pin GPIO_PIN_2
#define FUSE1_GPIO_Port GPIOB
#define STLINK_RX_Pin GPIO_PIN_7
#define STLINK_RX_GPIO_Port GPIOE
#define STLINK_TX_Pin GPIO_PIN_8
#define STLINK_TX_GPIO_Port GPIOE
#define FUSE2_Pin GPIO_PIN_9
#define FUSE2_GPIO_Port GPIOE
#define FUSE3_Pin GPIO_PIN_10
#define FUSE3_GPIO_Port GPIOE
#define FUSE4_Pin GPIO_PIN_11
#define FUSE4_GPIO_Port GPIOE
#define FUSE5_Pin GPIO_PIN_12
#define FUSE5_GPIO_Port GPIOE
#define FUSE6_Pin GPIO_PIN_13
#define FUSE6_GPIO_Port GPIOE
#define FUSE7_Pin GPIO_PIN_14
#define FUSE7_GPIO_Port GPIOE
#define FUSE8_Pin GPIO_PIN_15
#define FUSE8_GPIO_Port GPIOE
#define TACH1_Pin GPIO_PIN_10
#define TACH1_GPIO_Port GPIOB
#define FUSE9_Pin GPIO_PIN_11
#define FUSE9_GPIO_Port GPIOB
#define PWM11_Pin GPIO_PIN_14
#define PWM11_GPIO_Port GPIOB
#define PWM10_Pin GPIO_PIN_15
#define PWM10_GPIO_Port GPIOB
#define FUSE10_Pin GPIO_PIN_8
#define FUSE10_GPIO_Port GPIOD
#define FUSE11_Pin GPIO_PIN_9
#define FUSE11_GPIO_Port GPIOD
#define FUSE12_Pin GPIO_PIN_10
#define FUSE12_GPIO_Port GPIOD
#define TACH2_Pin GPIO_PIN_12
#define TACH2_GPIO_Port GPIOD
#define PWM9_Pin GPIO_PIN_13
#define PWM9_GPIO_Port GPIOD
#define PWM8_Pin GPIO_PIN_14
#define PWM8_GPIO_Port GPIOD
#define PWM7_Pin GPIO_PIN_15
#define PWM7_GPIO_Port GPIOD
#define PWM6_Pin GPIO_PIN_6
#define PWM6_GPIO_Port GPIOC
#define BattPumpPWM_Pin GPIO_PIN_7
#define BattPumpPWM_GPIO_Port GPIOC
#define PWM4_Pin GPIO_PIN_8
#define PWM4_GPIO_Port GPIOC
#define PWM3_Pin GPIO_PIN_9
#define PWM3_GPIO_Port GPIOC
#define TACH3_Pin GPIO_PIN_8
#define TACH3_GPIO_Port GPIOA
#define SW_boards_Pin GPIO_PIN_9
#define SW_boards_GPIO_Port GPIOA
#define SW_shutdown_Pin GPIO_PIN_10
#define SW_shutdown_GPIO_Port GPIOA
#define BOOT0_Trig_Pin GPIO_PIN_15
#define BOOT0_Trig_GPIO_Port GPIOA
#define GPS_UART_TX_Pin GPIO_PIN_10
#define GPS_UART_TX_GPIO_Port GPIOC
#define GPS_UART_RX_Pin GPIO_PIN_11
#define GPS_UART_RX_GPIO_Port GPIOC
#define SHDN0_Pin GPIO_PIN_12
#define SHDN0_GPIO_Port GPIOC
#define FDCAN_RX_Pin GPIO_PIN_0
#define FDCAN_RX_GPIO_Port GPIOD
#define FDCAN_TX_Pin GPIO_PIN_1
#define FDCAN_TX_GPIO_Port GPIOD
#define SHDN1_Pin GPIO_PIN_2
#define SHDN1_GPIO_Port GPIOD
#define SHDN8_Pin GPIO_PIN_3
#define SHDN8_GPIO_Port GPIOD
#define SHDN9_Pin GPIO_PIN_4
#define SHDN9_GPIO_Port GPIOD
#define SHDN10_Pin GPIO_PIN_5
#define SHDN10_GPIO_Port GPIOD
#define SHDN11_Pin GPIO_PIN_6
#define SHDN11_GPIO_Port GPIOD
#define SHDN12_Pin GPIO_PIN_7
#define SHDN12_GPIO_Port GPIOD
#define TACH4_Pin GPIO_PIN_4
#define TACH4_GPIO_Port GPIOB
#define PWM2_Pin GPIO_PIN_6
#define PWM2_GPIO_Port GPIOB
#define PWM1_Pin GPIO_PIN_8
#define PWM1_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
