//
// Created by Dhairya Gupta on 2/2/25.
// Does power deliver, current sense, and voltage sense.
//

#include "pdu.h"
#include "tim.h"
#include "usb_vcp.h"
#include "current_sense.h"
#include "timer.h"

#include <math.h>    // For powf
#include <float.h>   // For FLT_EPSILON

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

static NightCANPacket vcuShutdownStatusPacket;
static NightCANPacket vcuFusesPacket;
static NightCANPacket vcuCurrentSensePacket;


void pdu_init(PDUData *pduData, NightCANInstance *hcan) {
    pduData->faults.battery_fans = 0;
    pduData->faults.rad_fans = 0;
    pduData->faults.board_power = 0;
    pduData->faults.brake_light = 0;
    pduData->faults.motor_pump = 0;
    pduData->faults.battery_pump = 0;
    pduData->faults.green_status_light = 0;
    pduData->faults.red_status_light = 0;

    pduData->currents.battery_fans = 0;
    pduData->currents.rad_fans = 0;
    pduData->currents.board_power = 0;
    pduData->currents.brake_light = 0;
    pduData->currents.motor_pump = 0;
    pduData->currents.battery_pump = 0;
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

    /* Line Lock */
    HAL_TIM_PWM_Start(&htim16, TIM_CHANNEL_1);

    pduData->init = true;

    pdu_can_init(hcan);
}

void pdu_can_init(NightCANInstance *instance) {
    vcuShutdownStatusPacket = CAN_create_packet(VCU_SHUTDOWN_STATUS_ID, VCU_SHUTDOWN_STATUS_FREQ, VCU_SHUTDOWN_STATUS_DLC);
    vcuCurrentSensePacket = CAN_create_packet(VCU_CURRENT_SENSE_ID, VCU_CURRENT_SENSE_FREQ, VCU_CURRENT_SENSE_DLC);
    vcuFusesPacket = CAN_create_packet(VCU_FUSES_ID, VCU_FUSES_FREQ, VCU_FUSES_DLC);

    CAN_AddTxPacket(instance, &vcuShutdownStatusPacket);
    CAN_AddTxPacket(instance, &vcuCurrentSensePacket);
    CAN_AddTxPacket(instance, &vcuFusesPacket);
}

float breathing_animation(float delta_time, float period, float
    scale_factor, float min) {
    if (period <= 0.0f) {
        return 0.0f;
    }

    float normalized_time = delta_time / period;

    // Scale the sine wave so its peak is at scale_factor
    float value = scale_factor * (min + 0.5f + 0.5f * sinf(2.0f * M_PI *
                                                    normalized_time));

    if (value < 0.0f) {
        value = 0.0f;
    } else if (value > 1.0f) {
        value = 1.0f;
    }

    return sqrtf(value);
}

 #define MAX(a, b) (((a) > (b)) ? (a) : (b))
 #define MIN(a, b) (((a) < (b)) ? (a) : (b))

/**
 * @brief Adjusts a PWM percentage based on the current voltage to maintain
 * perceived brightness relative to a nominal voltage
 * * Assumes brightness is proportional to the square of the voltage (Power ~ V^2).
 * The relationship might need tuning based on specific LED characteristics.
 *
 * @param nominalPctAt24V The desired PWM percentage (0.0f to 1.0f) if the voltage were exactly 24V.
 * @param curVoltage The actual measured voltage supplying the lights.
 * @return float The adjusted PWM percentage (0.0f to 1.0f) to apply at curVoltage.
 */
float normalizeLightWithVoltage(float nominalPctAt24V, float curVoltage) {

    const float nominalVoltage = 24.0f;
    const float voltageExponent = 2.0f; // Need to tune

    // 0, so doesn't matter, output 0
    if (nominalPctAt24V <= 0.0f) {
        return 0.0f;
    }

    if(curVoltage < 12.0f) {
        // LV is dead, don't PWM
        return 0.0f;
    }

    // Scaling the voltage
    float voltageRatio = nominalVoltage / curVoltage;
    float scalingFactor = powf(voltageRatio, voltageExponent);

    float adjustedPct = nominalPctAt24V * scalingFactor;

    // clamping output
     adjustedPct = MIN(adjustedPct, 1.0f); // Using MIN macro
     adjustedPct = MAX(adjustedPct, 0.0f); // Using MAX macro

    return adjustedPct;
}

void setPWMLights(float brake_light, float green_status, float red_status, float curVoltage) {
    PWM_BRAKE_LIGHT_TIM.Instance->PWM_BRAKE_LIGHT = (uint32_t) normalizeLightWithVoltage(brake_light, curVoltage) * PWM_BRAKE_LIGHT_TIM.Instance->ARR;
    PWM_GREEN_STATUS_TIM.Instance->PWM_GREEN_STATUS = (uint32_t) normalizeLightWithVoltage(green_status, curVoltage) * PWM_GREEN_STATUS_TIM.Instance->ARR;
    PWM_RED_STATUS_TIM.Instance->PWM_RED_STATUS = (uint32_t) normalizeLightWithVoltage(red_status, curVoltage) * PWM_RED_STATUS_TIM.Instance->ARR;
}

void pdu_periodic(PDUData *pduData) {
    if(!pduData->init) return; // safety for dumb programming mistakes
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
    setPWMLights(pduData->switches.brake_light, pduData->switches.green_status_light,
                 pduData->switches.red_status_light, pduData->voltages.v_sense);


    htim16.Instance->CCR1 = 0.2f * htim16.Instance->ARR;

    checkPDUVoltages(pduData);

    // write out data to the CAN bus
    writePDUToCAN(pduData);
}

void writePDUToCAN(PDUData *data) {
    /** Fuse packet 1 */
    CAN_writeBitfield(&vcuFusesPacket, VCU_FUSES_VCU_FUSES_1_BYTE, VCU_FUSES_VCU_FUSES_1_BOARDS_FUSE_IDX, data->faults.board_power);
    CAN_writeBitfield(&vcuFusesPacket, VCU_FUSES_VCU_FUSES_1_BYTE, VCU_FUSES_VCU_FUSES_1_BATT_FANS_FUSE_IDX, data->faults.battery_fans);
    CAN_writeBitfield(&vcuFusesPacket, VCU_FUSES_VCU_FUSES_1_BYTE, VCU_FUSES_VCU_FUSES_1_BATT_PUMP_FUSE_IDX, data->faults.battery_pump);
    CAN_writeBitfield(&vcuFusesPacket, VCU_FUSES_VCU_FUSES_1_BYTE, VCU_FUSES_VCU_FUSES_1_LL_FUSE_IDX, data->faults.line_lock);
    CAN_writeBitfield(&vcuFusesPacket, VCU_FUSES_VCU_FUSES_1_BYTE, VCU_FUSES_VCU_FUSES_1_MOTOR_PUMP_FUSE_IDX, data->faults.motor_pump);
    CAN_writeBitfield(&vcuFusesPacket, VCU_FUSES_VCU_FUSES_1_BYTE, VCU_FUSES_VCU_FUSES_1_SHTDN_FUSE_IDX, data->faults.shutdown);
    CAN_writeBitfield(&vcuFusesPacket, VCU_FUSES_VCU_FUSES_1_BYTE, VCU_FUSES_VCU_FUSES_1_TSSI_GREEN_FUSE_IDX, data->faults.green_status_light);
    CAN_writeBitfield(&vcuFusesPacket, VCU_FUSES_VCU_FUSES_1_BYTE, VCU_FUSES_VCU_FUSES_1_TSSI_RED_FUSE_IDX, data->faults.red_status_light);

    /** Fuse Packet 2 */
    CAN_writeBitfield(&vcuFusesPacket, VCU_FUSES_VCU_FUSES_2_BYTE, VCU_FUSES_VCU_FUSES_2_BRAKE_LIGHT_FUSE_IDX, data->faults.brake_light);
    CAN_writeBitfield(&vcuFusesPacket, VCU_FUSES_VCU_FUSES_2_BYTE, VCU_FUSES_VCU_FUSES_2_RTD_FUSE_IDX, data->faults.rtd);


    /** VCU Current Sense */
    CAN_writeFloat(VCU_CURRENT_SENSE_BATTERY_COOLING_CURRENT_TYPE, &vcuCurrentSensePacket,
                   VCU_CURRENT_SENSE_BATTERY_COOLING_CURRENT_BYTE, data->currents.battery_pump,
                   VCU_CURRENT_SENSE_BATTERY_COOLING_CURRENT_PREC);
}

void unveiling_light_animation(float dt, PDUData *data) {
    data->switches.red_status_light = breathing_animation(lib_timer_elapsed_ms()/1000.0f, 5.5f, 0.00005f, 0.0003f);; // off
    data->switches.green_status_light = breathing_animation(lib_timer_elapsed_ms()/1000.0f, 5.5f, 0.005f, 0.0001f); // 5s period, 0.4 max value
    data->switches.brake_light = breathing_animation(lib_timer_elapsed_ms()/1000.0f, 5.5f, 0.002f, 0.0001f); // 5s period, 0.6 max value
}

void checkPDUFaults(PDUData *pduData) {
    pduData->faults.battery_fans = HAL_GPIO_ReadPin(FAULT_BATTERY_FANS_GPIOX, FAULT_BATTERY_FANS_PIN);
    pduData->faults.rad_fans = HAL_GPIO_ReadPin(FAULT_RAD_FANS_GPIOX, FAULT_RAD_FANS_PIN);
    pduData->faults.board_power = HAL_GPIO_ReadPin(FAULT_BOARD_POWER_GPIOX, FAULT_BOARD_POWER_PIN);
    pduData->faults.brake_light = HAL_GPIO_ReadPin(FAULT_BRAKE_LIGHT_GPIOX, FAULT_BRAKE_LIGHT_PIN);
    pduData->faults.motor_pump = HAL_GPIO_ReadPin(FAULT_COOLING_PUMP_1_GPIOX, FAULT_COOLING_PUMP_1_PIN);
    pduData->faults.battery_pump = HAL_GPIO_ReadPin(FAULT_COOLING_PUMP_2_GPIOX, FAULT_COOLING_PUMP_2_PIN);
    pduData->faults.green_status_light = HAL_GPIO_ReadPin(FAULT_GREEN_STATUS_GPIOX, FAULT_GREEN_STATUS_PIN);
    pduData->faults.red_status_light = HAL_GPIO_ReadPin(FAULT_RED_STATUS_GPIOX, FAULT_RED_STATUS_PIN);
    // TODO: MISSING 2 PINS
}


void checkPDUCurrents(PDUData *data) {
    // Read currents from Current sense
}

inline void checkPDUVoltages(PDUData *data) {
#define V_T (0.70f * UINT16_MAX)
    uint16_t v_sense_raw = ADC1_BUFFER[V_SENSE_IDX];
    float v_sense_raw_pct = (float) v_sense_raw / V_T; // comparing to 24.0V ref.
    data->voltages.v_sense = v_sense_raw_pct * 24.0f;
};

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
