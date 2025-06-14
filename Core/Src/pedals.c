//
// Created by Dhairya Gupta on 6/14/25.
//

#include "pedals.h"

static NightCANReceivePacket apps;

void pedals_init(FDCAN_HandleTypeDef *hcan, NightCANInstance *canInstance) {
    // If we miss packets for 2 whole seconds, we're cooked, give up.
    apps = CAN_create_receive_packet(APPS_VOLTAGES_ID, 2000, APPS_VOLTAGES_DLC);
    CAN_addReceivePacket(canInstance, &apps);
};

void pedals_periodic(VCUModelInputs *vcuModelInputs) {
    if(apps.is_timed_out) {
        // OOR. Error! Disable the car.
        vcuModelInputs->apps.pedal1Percent = 0;
        vcuModelInputs->apps.pedal2Percent = 0;

        vcuModelInputs->apps.pedal1Voltage = 0;
        vcuModelInputs->apps.pedal2Voltage = 0;
    } else {
        // Update voltages
        vcuModelInputs->apps.pedal1Voltage = CAN_readFloat(APPS_VOLTAGES_APPS1_VOLTAGE_TYPE, &apps,
                                                           APPS_VOLTAGES_APPS1_VOLTAGE_BYTE,
                                                           APPS_VOLTAGES_APPS1_VOLTAGE_PREC);
        vcuModelInputs->apps.pedal2Voltage = CAN_readFloat(APPS_VOLTAGES_APPS2_VOLTAGE_TYPE, &apps,
                                                           APPS_VOLTAGES_APPS2_VOLTAGE_BYTE,
                                                           APPS_VOLTAGES_APPS2_VOLTAGE_PREC);
    }
};