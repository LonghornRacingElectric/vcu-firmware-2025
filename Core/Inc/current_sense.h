//
// Created by Dhairya Gupta on 3/15/25.
//

#ifndef VCU_FIRMWARE_2025_CURRENT_SENSE_H
#define VCU_FIRMWARE_2025_CURRENT_SENSE_H
#include <stdint.h>

/* Access indexes for the DMA buffers */

/* ADC 1 */
#define THERM_BATT_2_IDX 0
#define THERM_MOTOR_1_IDX 1
#define V_SENSE_IDX 2
#define THERM_AMBIENT_IDX 3
#define THERM_MOTOR_2_IDX 4
#define THERM_MOTOR_3_IDX 5
#define BSE3_IDX 6
#define HALL_EFFECT_IDX 7
#define LIGHTS_IDX 8
#define MOTOR_COOLING_IDX 9
#define SHUTDOWN_IDX 10
#define BATTERY_COOLING_IDX 11
#define THERM_DISCHARGE_IDX 12
#define THERM_BATT_1_IDX 13

/* ADC 3 */
#define ANALOG_BRAKE_IDX 0
#define BOARDS_IDX 1

extern uint16_t ADC1_BUFFER[14]; // 14 element 16bit ADC resolution buffer
extern uint8_t ADC3_BUFFER[2];

#endif //VCU_FIRMWARE_2025_CURRENT_SENSE_H
