//
// Created by Dhairya Gupta on 2/2/25.
//

#include "pdu.h"

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

    pduData->switches.battery_fans = 0;
    pduData->switches.rad_fans = 0;
    pduData->switches.board_power = 1; // turn on all boards
    pduData->switches.brake_light = 1; // flash brake light on startup
    pduData->switches.cooling_pump_1 = 0;
    pduData->switches.cooling_pump_2 = 0;
    pduData->switches.green_status_light = 1; // flash
    pduData->switches.red_status_light = 1; // flash
}


void pdu_periodic(PDUData *pduData) {
//    if(!init) pdu_init(pduData); // safety for dumb programming mistakes

    // read in data, add data to can outboxes (or optionally modify a passed in struct and let the general thing send CAN)
    checkPDUFaults(pduData);
}


void checkPDUFaults(PDUData *pduData) {
    pduData->faults.battery_fans = HAL_GPIO_ReadPin(GPIOE, 3);
    pduData->faults.rad_fans = HAL_GPIO_ReadPin(GPIOA, 9);
    pduData->faults.board_power = HAL_GPIO_ReadPin(GPIOE, 0);
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

    pduData->switches.battery_fans = 0;
    pduData->switches.rad_fans = 0;
    pduData->switches.board_power = 1; // turn on all boards
    pduData->switches.brake_light = 1; // flash brake light on startup
    pduData->switches.cooling_pump_1 = 0;
    pduData->switches.cooling_pump_2 = 0;
    pduData->switches.green_status_light = 1; // flash
    pduData->switches.red_status_light = 1; // flash
}
