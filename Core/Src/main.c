/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "fdcan.h"
#include "lptim.h"
#include "memorymap.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

//extern "C" {
#include "usb_vcp.h"
#include "pdu.h"
#include "led.h"
#include "VcuModel.h"
#include "dfu.h"
#include "bspd.h"
#include "timer.h"
#include "current_sense.h"
#include "night_can.h"
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

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
bool isFinished;
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc) {
    isFinished = 1;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_ADC1_Init();
  MX_ADC3_Init();
  MX_FDCAN1_Init();
  MX_FDCAN2_Init();
  MX_LPTIM1_Init();
  MX_LPTIM2_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM4_Init();
  MX_TIM8_Init();
  MX_TIM12_Init();
  MX_TIM15_Init();
  MX_TIM16_Init();
  MX_UART4_Init();
  MX_USB_DEVICE_Init();
  MX_UART7_Init();
  /* USER CODE BEGIN 2 */
  PDUData pduData = {};
  BSPDOutputs bspd;
  BSPDOutputs *bspdaddr = &bspd;

  /* Initialize Structures/Subsystems */
  pdu_init(&pduData);
  led_init(TIM15, &htim15, 2); // missing a channel on the vcu
  dfu_init(GPIOA, GPIO_PIN_15);
  HAL_ADC_Start_DMA(&hadc1, (uint32_t *) ADC1_BUFFER, 14);
  HAL_ADC_Start_DMA(&hadc3, (uint32_t *) ADC3_BUFFER, 2);
  lib_timer_init();
  night_can_init(&hfdcan1);

    // --- Start the FDCAN peripheral ---
    //     Must be done once after init and before sending the first message

    VCUModelParameters params = {
            .torque = {
                    .mapPedalToTorqueRequest = {
                            .x0 = 0.0f,
                            .x1 = 1.0f,
                            .y = {},
                            .xP = 1.0f,
                            .yP = 230.0f
                    }
            },
            .stompp = {
                    .stomppAppsRecoveryThreshold = 0.05f,
                    .mechanicalBrakeThreshold = 0.08f,
                    .stomppAppsCutoffThreshold = 0.25f
            },
            .apps = {
                    .sensorInRangeUpperBound = 1.0f,
                    .sensorInRangeLowerBound = 0.0f,
                    .allowedPlausibilityRange = 0.1f,
                    .appsDeadzoneTopPercent = 0.1f,
                    .appsDeadzoneBottomPercent = 0.1f,
                    .appsMaxImplausibilityTime = 100.0f,
                    .pedal1Bias = 0.5f,
            },
            .brake_light = {
                    .bseLightOnPercent = 0.5f,
                    .bseTimeConstant = 0.8f
            }
    };

    VCUModelInputs inputs = {};
    VCUModelOutputs outputs = {};

    // set up the vcu model with the parameters
    VCUModel_set_parameters(&params);
//    HAL_TIM_PWM_Start(&htim12, TIM_CHANNEL_2);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_LPTIM_Init(&hlptim2);
    HAL_LPTIM_Counter_Start(&hlptim2, hlptim2.Instance->ARR);

    uint8_t packet1_data[4];
    uint8_t packet2_data[8];
    while (1)
    {
        uint32_t curtime = lib_timer_ms_elapsed();
        led_rainbow(curtime / 1000.0f);
        pduData.switches.green_status_light = 1;
        pduData.switches.battery_fans = 1;
        pdu_periodic(&pduData);

        if(checkDrive()) {
            inputs.apps.pedal1Percent = 0.101f;
            inputs.apps.pedal2Percent = 0.101f;

            ((int16_t*)packet2_data)[0] = outputs.torque.torqueRequest;
            ((int16_t*)packet2_data)[1] = 0;
            packet2_data[4] = 1;
            packet2_data[5] = outputs.torque.torqueRequest > 0 ? 1 : 0;
            ((int16_t*)packet2_data)[3] = 10;

            HAL_StatusTypeDef status = send_CAN_data(&hfdcan1, 0xDD, FDCAN_DLC_BYTES_4, packet1_data);
        } else {
            // need to send CAN packet to say disabled
            inputs.apps.pedal1Percent = 0.0f;
            inputs.apps.pedal2Percent = 0.0f;
        }

        VCUModel_evaluate(&inputs, &outputs, lib_timer_ms_elapsed()/1000.0f);
        receive_periodic();
        bspd_periodic(bspdaddr);

        usb_printf("Drive var was: %d, Torque request output was: %f, STOMPP was %d, BSE was %f", checkDrive(),  outputs.torque.torqueRequest, outputs.stompp.output, inputs.stompp.bse_percent);


//        usb_printf("BSPD outputs were: hard braking: %d, motor 5kw: %d, error: %d, trigger: %d, latch: %d",
//                   bspd.hard_braking, bspd.motor_5kw, bspd.error, bspd.trigger, bspd.latch);

        pduData.switches.brake_light = (float) outputs.brake_light.lightOn * 40;


        uint32_t free_level = HAL_FDCAN_GetTxFifoFreeLevel(&hfdcan1);


//        if(isFinished) {
//            usb_printf("Motor current sense: %d", ADC1_BUFFER[9]);
//            isFinished = 0;
//        }

//        if(isFinished) {
////            for (int i = 0; i < 14; i++) {
////
////                usb_printf("ADC 1: Data at %d, value: %d", i, ADC1_BUFFER[i]);
////            }
//
////            usb_printf("ADC 1: Data at %d, value: %d", 7, ADC1_BUFFER[7]);
////            usb_printf("ADC 1: Data at %d, value: %d", 6, ADC1_BUFFER[6]);
//
////                usb_printf("ADC 1: Data at %d, value: %d", 6, ADC1_BUFFER[6]);
//
////            for(int i = 0; i < 2; i++) {
////                usb_printf("ADC 3: Data at %d, value: %d", i, ADC3_BUFFER[i]);
////            }
//
//            isFinished = 0;
//        }
        uint32_t tach = HAL_LPTIM_ReadCounter(&hlptim2);
        float rpm = tach / 30.0f;

//        usb_printf("Tach info: %i, RPM: %f", tach, rpm);

        if(bspd.trigger) {
            pduData.switches.cooling_pump_1 = 0.2f;
            pduData.switches.cooling_pump_2 = 0.2f;
            pduData.switches.battery_fans = 0.00f;
            pduData.switches.rad_fans = 0.00f;
            pduData.switches.cooling_pump_1 = 0.00f;
            pduData.switches.cooling_pump_2 = 0.00f;
        } else {
            pduData.switches.cooling_pump_1 = 0.8f; // percent
            pduData.switches.cooling_pump_2 = 0.8f;
            pduData.switches.battery_fans = 1.0f;
            pduData.switches.rad_fans = 1.0f-0.0f;
            pduData.switches.cooling_pump_1 = 1.00f;
            pduData.switches.cooling_pump_2 = 1.00f;
        }


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

  /** Supply configuration update enable
  */
  HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

  /** Configure the main internal regulator output voltage
  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

  while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 275;
  RCC_OscInitStruct.PLL.PLLP = 1;
  RCC_OscInitStruct.PLL.PLLQ = 55;
  RCC_OscInitStruct.PLL.PLLR = 55;
  RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_1;
  RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
  RCC_OscInitStruct.PLL.PLLFRACN = 0;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                              |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
  RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInitStruct.PLL2.PLL2M = 16;
  PeriphClkInitStruct.PLL2.PLL2N = 256;
  PeriphClkInitStruct.PLL2.PLL2P = 2;
  PeriphClkInitStruct.PLL2.PLL2Q = 4;
  PeriphClkInitStruct.PLL2.PLL2R = 2;
  PeriphClkInitStruct.PLL2.PLL2RGE = RCC_PLL2VCIRANGE_0;
  PeriphClkInitStruct.PLL2.PLL2VCOSEL = RCC_PLL2VCOMEDIUM;
  PeriphClkInitStruct.PLL2.PLL2FRACN = 0;
  PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL2;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

 /* MPU Configuration */

void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
  MPU_InitStruct.SubRegionDisable = 0x87;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
//    __disable_irq();
    while (1)
    {
        led_set(255, 0, 0 );

        receive_periodic();
    }
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
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
