//
// Created by Dhairya Gupta on 3/3/25.
//

#include "bspd.h"
#include "gpio.h"

#ifdef REVB

#define BSPD_HARD_BRAKING_GPIOX GPIOC
#define BSPD_HARD_BRAKING_PIN GPIO_PIN_13

#define BSPD_MOTOR_5KW_GPIOX GPIOC
#define BSPD_MOTOR_5KW_PIN GPIO_PIN_14

#define BSPD_ERROR_GPIOX GPIOC
#define BSPD_ERROR_PIN GPIO_PIN_15

#define BSPD_TRIGGER_GPIOX GPIOE
#define BSPD_TRIGGER_PIN GPIO_PIN_2

#define BSPD_LATCH_GPIOX GPIOE
#define BSPD_LATCH_PIN GPIO_PIN_3

void bspd_periodic(BSPDOutputs *bspd) {
    bspd->error  = !HAL_GPIO_ReadPin(BSPD_ERROR_GPIOX, BSPD_ERROR_PIN);
    bspd->hard_braking = !HAL_GPIO_ReadPin(BSPD_HARD_BRAKING_GPIOX, BSPD_HARD_BRAKING_PIN);
    bspd->motor_5kw = !HAL_GPIO_ReadPin(BSPD_MOTOR_5KW_GPIOX, BSPD_MOTOR_5KW_PIN);
    bspd->latch = HAL_GPIO_ReadPin(BSPD_LATCH_GPIOX, BSPD_LATCH_PIN);
    bspd->trigger = HAL_GPIO_ReadPin(BSPD_TRIGGER_GPIOX, BSPD_TRIGGER_PIN);
}

#endif
