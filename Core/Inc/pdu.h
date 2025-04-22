//
// Created by Dhairya Gupta on 2/2/25.
//

#ifndef VCU_FIRMWARE_2025_PDU_H
#define VCU_FIRMWARE_2025_PDU_H
#include "stdbool.h"
#include "gpio.h"
#include "night_can.h"

extern uint16_t ADC1_BUFFER[14]; // 14 element 16bit ADC resolution buffer
extern uint8_t ADC3_BUFFER[2];

static bool init = false;

// Can be turned into 1 unsigned integer bitmap if needing to save memory (24B -> 4B)
typedef struct PDUData {
    struct faults {
        bool rad_fans, motor_pump, battery_pump,
            brake_light, red_status_light, green_status_light,
            board_power, battery_fans, line_lock, shutdown, rtd;
    } faults;

    struct switches {
        float rad_fans, cooling_pump_1, cooling_pump_2,
            brake_light, red_status_light, green_status_light,
            board_power, battery_fans;
    } switches;

    struct currents {
        bool rad_fans, motor_pump, battery_pump,
            brake_light, red_status_light, green_status_light,
            board_power, battery_fans;
    } currents;

    struct voltages {
        float v_sense;
    } voltages;

    bool init;
} PDUData;

void pdu_init(PDUData *data, NightCANInstance *instance);

void pdu_periodic(PDUData *data);

void checkPDUFaults(PDUData *data);
void checkPDUCurrents(PDUData *data);
void setPDUSwitches(PDUData *data);
void checkPDUVoltages(PDUData *data);
void pdu_can_init(NightCANInstance *instance);
void unveiling_light_animation(float dt, PDUData *data);

void writePDUToCAN(PDUData *data);


#endif //VCU_FIRMWARE_2025_PDU_H
