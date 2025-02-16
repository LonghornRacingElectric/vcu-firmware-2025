//
// Created by Dhairya Gupta on 2/2/25.
//

#ifndef VCU_FIRMWARE_2025_PDU_H
#define VCU_FIRMWARE_2025_PDU_H
#include "stdbool.h"
#include "gpio.h"

static bool init = false;

// Can be turned into 1 unsigned integer bitmap if needing to save memory (24B -> 4B)
typedef struct PDUData {
    struct faults {
        bool rad_fans, cooling_pump_1, cooling_pump_2,
            brake_light, red_status_light, green_status_light,
            board_power, battery_fans;
    } faults;

    struct switches {
        bool rad_fans, cooling_pump_1, cooling_pump_2,
            brake_light, red_status_light, green_status_light,
            board_power, battery_fans;
    } switches;

    struct currents {
        bool rad_fans, cooling_pump_1, cooling_pump_2,
            brake_light, red_status_light, green_status_light,
            board_power, battery_fans;
    } currents;
} PDUData;

void pdu_init(PDUData *data);

void pdu_periodic(PDUData *data);

void checkPDUFaults(PDUData *data);
void checkPDUCurrents(PDUData *data);
void setPDUSwitches(PDUData *data);

void writePDUToCAN(PDUData *data);


#endif //VCU_FIRMWARE_2025_PDU_H
