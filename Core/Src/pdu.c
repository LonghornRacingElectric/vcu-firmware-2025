//
// Created by Dhairya Gupta on 2/2/25.
//

#include "pdu.h"
#include "tim.h"
#include "usb_vcp.h"

#ifdef REVA
#define FAULT_BATTERY_FANS_GPIOX GPIOE
#define FAULT_BATTERY_FANS_PIN GPIO_PIN_3
#define SWITCH_BATTERY_FANS_GPIOX GPIOB
#define SWITCH_BATTERY_FANS_PIN GPIO_PIN_4

#define FAULT_RAD_FANS_GPIOX GPIOA
#define FAULT_RAD_FANS_PIN GPIO_PIN_9
#define SWITCH_RAD_FANS_GPIOX GPIOB
#define SWITCH_RAD_FANS_PIN GPIO_PIN_7

#define FAULT_BOARD_POWER_GPIOX GPIOE
#define FAULT_BOARD_POWER_PIN GPIO_PIN_0
#define SWITCH_BOARD_POWER_GPIOX GPIOB
#define SWITCH_BOARD_POWER_PIN GPIO_PIN_2

#define FAULT_BRAKE_LIGHT_GPIOX GPIOA
#define FAULT_BRAKE_LIGHT_PIN GPIO_PIN_8
#define SWITCH_BRAKE_LIGHT_GPIOX GPIOB
#define SWITCH_BRAKE_LIGHT_PIN GPIO_PIN_10

#define FAULT_COOLING_PUMP_1_GPIOX GPIOB
#define FAULT_COOLING_PUMP_1_PIN GPIO_PIN_5
#define SWITCH_COOLING_PUMP_1_GPIOX GPIOC
#define SWITCH_COOLING_PUMP_1_PIN GPIO_PIN_0

#define FAULT_COOLING_PUMP_2_GPIOX GPIOB
#define FAULT_COOLING_PUMP_2_PIN GPIO_PIN_6
#define SWITCH_COOLING_PUMP_2_GPIOX GPIOC
#define SWITCH_COOLING_PUMP_2_PIN GPIO_PIN_1

#define FAULT_GREEN_STATUS_GPIOX GPIOE
#define FAULT_GREEN_STATUS_PIN GPIO_PIN_1
#define SWITCH_GREEN_STATUS_GPIOX GPIOB
#define SWITCH_GREEN_STATUS_PIN GPIO_PIN_14

#define FAULT_RED_STATUS_GPIOX GPIOE
#define FAULT_RED_STATUS_PIN GPIO_PIN_11
#define SWITCH_RED_STATUS_GPIOX GPIOB
#define SWITCH_RED_STATUS_PIN GPIO_PIN_11
#elif defined(REVB)
#define FAULT_BATTERY_FANS_GPIOX GPIOB
#define FAULT_BATTERY_FANS_PIN GPIO_PIN_2
#define SWITCH_BATTERY_FANS_GPIOX GPIOB
#define SWITCH_BATTERY_FANS_PIN GPIO_PIN_15

#define PWM_BATTERY_FANS_TIM htim8
#define PWM_BATTERY_FANS CCR1
#define TIM_BATTERY_FANS_CH TIM_CHANNEL_1

#define FAULT_RAD_FANS_GPIOX GPIOD
#define FAULT_RAD_FANS_PIN GPIO_PIN_10
#define SWITCH_RAD_FANS_GPIOX GPIOC
#define SWITCH_RAD_FANS_PIN GPIO_PIN_8

#define PWR_RADIATOR_FANS_TIM htim8
#define PWR_RADIATOR_FANS CCR3
#define TIM_RADIATOR_FANS_CH TIM_CHANNEL_3

#define PWM_RADIATOR_FANS_TIM htim12
#define PWM_RADIATOR_FANS CCR1
#define PWM_RADIATOR_FANS_CH TIM_CHANNEL_1

#define FAULT_BOARD_POWER_GPIOX GPIOD
#define FAULT_BOARD_POWER_PIN GPIO_PIN_9
#define SWITCH_BOARD_POWER_GPIOX GPIOA
#define SWITCH_BOARD_POWER_PIN GPIO_PIN_9

#define FAULT_BRAKE_LIGHT_GPIOX GPIOB
#define FAULT_BRAKE_LIGHT_PIN GPIO_PIN_11
#define SWITCH_BRAKE_LIGHT_GPIOX GPIOD
#define SWITCH_BRAKE_LIGHT_PIN GPIO_PIN_13

#define PWM_BRAKE_LIGHT_TIM htim4
#define PWM_BRAKE_LIGHT CCR2
#define TIM_BRAKE_LIGHT_CH TIM_CHANNEL_2

#define FAULT_COOLING_PUMP_1_GPIOX GPIOE // Battery Pumps
#define FAULT_COOLING_PUMP_1_PIN GPIO_PIN_11
#define SWITCH_COOLING_PUMP_1_GPIOX GPIOC
#define SWITCH_COOLING_PUMP_1_PIN GPIO_PIN_7

#define PWM_COOLING_PUMP_1 htim8
#define PWM_COOLING_PUMP_CH CCR2
#define TIM_COOLING_PUMP_1_INIT_CH TIM_CHANNEL_2

#define FAULT_COOLING_PUMP_2_GPIOX GPIOD // Motor Pumps
#define FAULT_COOLING_PUMP_2_PIN GPIO_PIN_8
#define SWITCH_COOLING_PUMP_2_GPIOX GPIOC
#define SWITCH_COOLING_PUMP_2_PIN GPIO_PIN_9

#define PWM_COOLING_PUMP_2 htim8
#define PWM_COOLING_PUMP_2_CH CCR4
#define TIM_COOLING_PUMP_2_INIT_CH TIM_CHANNEL_4

#define FAULT_GREEN_STATUS_GPIOX GPIOE
#define FAULT_GREEN_STATUS_PIN GPIO_PIN_13
#define SWITCH_GREEN_STATUS_GPIOX GPIOD
#define SWITCH_GREEN_STATUS_PIN GPIO_PIN_15

#define PWM_GREEN_STATUS_TIM htim4
#define PWM_GREEN_STATUS CCR4
#define TIM_GREEN_STATUS_CH TIM_CHANNEL_4

#define FAULT_RED_STATUS_GPIOX GPIOE
#define FAULT_RED_STATUS_PIN GPIO_PIN_10
#define SWITCH_RED_STATUS_GPIOX GPIOD
#define SWITCH_RED_STATUS_PIN GPIO_PIN_14

#define PWM_RED_STATUS_TIM htim4
#define PWM_RED_STATUS CCR3
#define TIM_RED_STATUS_CH TIM_CHANNEL_3
#endif

void pdu_init(PDUData *pduData) {
    pduData->faults.battery_fans = 0;
    pduData->faults.rad_fans = 0;
    pduData->faults.board_power = 0;
    pduData->faults.brake_light = 0;
    pduData->faults.cooling_pump_1 = 0;
    pduData->faults.cooling_pump_2 = 0;
    pduData->faults.green_status_light = 0;
    pduData->faults.red_status_light = 0;

    pduData->currents.battery_fans = 0;
    pduData->currents.rad_fans = 0;
    pduData->currents.board_power = 0;
    pduData->currents.brake_light = 0;
    pduData->currents.cooling_pump_1 = 0;
    pduData->currents.cooling_pump_2 = 0;
    pduData->currents.green_status_light = 0;
    pduData->currents.red_status_light = 0;

    pduData->switches.battery_fans = 0.0f;
    pduData->switches.rad_fans = 0.0f;
    pduData->switches.board_power = 1.0f; // turn on all boards
    pduData->switches.brake_light = 1.0f; // flash brake light on startup
    pduData->switches.cooling_pump_1 = 0.0f;
    pduData->switches.cooling_pump_2 = 0.0f;
    pduData->switches.green_status_light = 1.0f; // flash
    pduData->switches.red_status_light = 1.0f; // flash

    /* Cooling */
    HAL_TIM_PWM_Start(&PWM_COOLING_PUMP_1, TIM_COOLING_PUMP_1_INIT_CH);
//    HAL_TIM_PWM_Start(&PWM_COOLING_PUMP_2, TIM_COOLING_PUMP_2_INIT_CH);
    HAL_TIM_PWM_Start(&PWM_BATTERY_FANS_TIM, TIM_BATTERY_FANS_CH);
    HAL_TIM_PWM_Start(&PWR_RADIATOR_FANS_TIM, TIM_RADIATOR_FANS_CH);
    HAL_TIM_PWM_Start(&PWM_RADIATOR_FANS_TIM, PWM_RADIATOR_FANS_CH);


    /* Lights */
    HAL_TIM_PWM_Start(&PWM_BRAKE_LIGHT_TIM, TIM_BRAKE_LIGHT_CH);
    HAL_TIM_PWM_Start(&PWM_GREEN_STATUS_TIM, TIM_GREEN_STATUS_CH);
    HAL_TIM_PWM_Start(&PWM_RED_STATUS_TIM, TIM_RED_STATUS_CH);

    pduData->init = true;
}


void pdu_periodic(PDUData *pduData) {
    if(!pduData->init) pdu_init(pduData); // safety for dumb programming mistakes
    // most critical -- write the data out to the pins
    setPDUSwitches(pduData);

    // read in data, add data to can outboxes (or optionally modify a passed in struct and let the general thing send CAN)
    checkPDUFaults(pduData);
    checkPDUCurrents(pduData);


    /* Update PWM on all signals */
    /* Cooling */
    PWM_COOLING_PUMP_1.Instance->PWM_COOLING_PUMP_CH =  pduData->switches.cooling_pump_1 * PWM_COOLING_PUMP_1.Instance->ARR; // testing with percentages
//    PWM_COOLING_PUMP_2.Instance->PWM_COOLING_PUMP_2_CH =  pduData->switches.cooling_pump_2 * PWM_COOLING_PUMP_1.Instance->ARR; // testing with percentages
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);
    PWM_BATTERY_FANS_TIM.Instance->PWM_BATTERY_FANS = pduData->switches.battery_fans * PWM_BATTERY_FANS_TIM.Instance->ARR;
    PWR_RADIATOR_FANS_TIM.Instance->PWR_RADIATOR_FANS = 1.0f * PWR_RADIATOR_FANS_TIM.Instance->ARR;

    PWM_RADIATOR_FANS_TIM.Instance->PWM_RADIATOR_FANS = pduData->switches.rad_fans * PWM_RADIATOR_FANS_TIM.Instance->ARR;

    /* Lights */
    PWM_BRAKE_LIGHT_TIM.Instance->PWM_BRAKE_LIGHT = pduData->switches.brake_light * PWM_BRAKE_LIGHT_TIM.Instance->ARR;
    PWM_GREEN_STATUS_TIM.Instance->PWM_GREEN_STATUS =  pduData->switches.green_status_light * PWM_GREEN_STATUS_TIM.Instance->ARR;
    PWM_RED_STATUS_TIM.Instance->PWM_RED_STATUS = pduData->switches.red_status_light * PWM_RED_STATUS_TIM.Instance->ARR;
    // write out data to the CAN bus
//    writePDUToCAN(pduData);
}


void checkPDUFaults(PDUData *pduData) {
    pduData->faults.battery_fans = HAL_GPIO_ReadPin(FAULT_BATTERY_FANS_GPIOX, FAULT_BATTERY_FANS_PIN);
    pduData->faults.rad_fans = HAL_GPIO_ReadPin(FAULT_RAD_FANS_GPIOX, FAULT_RAD_FANS_PIN);
    pduData->faults.board_power = HAL_GPIO_ReadPin(FAULT_BOARD_POWER_GPIOX, FAULT_BOARD_POWER_PIN);
    pduData->faults.brake_light = HAL_GPIO_ReadPin(FAULT_BRAKE_LIGHT_GPIOX, FAULT_BRAKE_LIGHT_PIN);
    pduData->faults.cooling_pump_1 = HAL_GPIO_ReadPin(FAULT_COOLING_PUMP_1_GPIOX, FAULT_COOLING_PUMP_1_PIN);
    pduData->faults.cooling_pump_2 = HAL_GPIO_ReadPin(FAULT_COOLING_PUMP_2_GPIOX, FAULT_COOLING_PUMP_2_PIN);
    pduData->faults.green_status_light = HAL_GPIO_ReadPin(FAULT_GREEN_STATUS_GPIOX, FAULT_GREEN_STATUS_PIN);
    pduData->faults.red_status_light = HAL_GPIO_ReadPin(FAULT_RED_STATUS_GPIOX, FAULT_RED_STATUS_PIN);
}


void checkPDUCurrents(PDUData *data) {
    //
}

void setPDUSwitches(PDUData *pduData) {
    HAL_GPIO_WritePin(SWITCH_BATTERY_FANS_GPIOX, SWITCH_BATTERY_FANS_PIN, pduData->switches.battery_fans);
    HAL_GPIO_WritePin(SWITCH_RAD_FANS_GPIOX, SWITCH_RAD_FANS_PIN, pduData->switches.rad_fans);
    HAL_GPIO_WritePin(SWITCH_BOARD_POWER_GPIOX, SWITCH_BOARD_POWER_PIN, pduData->switches.board_power);
    HAL_GPIO_WritePin(SWITCH_BRAKE_LIGHT_GPIOX, SWITCH_BRAKE_LIGHT_PIN, pduData->switches.brake_light);
    HAL_GPIO_WritePin(SWITCH_COOLING_PUMP_1_GPIOX, SWITCH_COOLING_PUMP_1_PIN, pduData->switches.cooling_pump_1);
    HAL_GPIO_WritePin(SWITCH_COOLING_PUMP_2_GPIOX, SWITCH_COOLING_PUMP_2_PIN, pduData->switches.cooling_pump_2);
    HAL_GPIO_WritePin(SWITCH_GREEN_STATUS_GPIOX, SWITCH_GREEN_STATUS_PIN, pduData->switches.green_status_light);
    HAL_GPIO_WritePin(SWITCH_RED_STATUS_GPIOX, SWITCH_RED_STATUS_PIN, pduData->switches.red_status_light);
}
