//
// Created by Dhairya Gupta on 6/14/25.
//

#include "pedals.h"

static NightCANReceivePacket apps;

void pedals_init(FDCAN_HandleTypeDef *hcan, NightCANInstance *canInstance) {
    // If we miss 5 packets, we're cooked, give up.
    apps = CAN_create_receive_packet(APPS_VOLTAGES_ID, APPS_VOLTAGES_FREQ * 5, APPS_VOLTAGES_DLC);
    CAN_addReceivePacket(canInstance, &apps);
};

void pedals_periodic(VCUModelInputs *vcuModelInputs) {
    if(apps.is_timed_out) {
        vcuModelInputs->apps.pedal2Percent = 0;
        vcuModelInputs->apps.pedal1Percent = 0;
    } else {
        vcuModelInputs->apps.pedal1Voltage = CAN_readFloat(APPS_VOLTAGES_APPS1_VOLTAGE_TYPE, &apps);
    }
};