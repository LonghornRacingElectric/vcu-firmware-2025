//
// Created by Dhairya Gupta on 4/1/25.
//

#include "inverter.h"

static NightCANPacket torqueCommand;

void updateTorqueRequest(float torque_request) {
    can_writeFloat(uint16_t, &torqueCommand, 0, torque_request, 0.1f); // torque request
}

void updateSpeedRequest(float speed_request) {
    can_writeFloat(uint16_t, &torqueCommand, 2, speed_request, 0.1f); // speed request
}

void updateInverterDirection(uint8_t direction) {
    can_writeInt(uint8_t, &torqueCommand, 4, direction); // inverter direction
}

void updateInverterEnable(uint8_t enable) {
    can_writeInt(uint8_t, &torqueCommand, 5, enable); // inverter enable
}

void updateInverterTorqueLimit(float torque_limit) {
    can_writeFloat(uint16_t, &torqueCommand, 6, torque_limit, 0.1f);
}

void inverter_update_all_fields(float torque_request, float speed_request, uint8_t direction, uint8_t enable, float torque_limit) {
    updateTorqueRequest(torque_request);
    updateSpeedRequest(speed_request);
    updateInverterDirection(direction);
    updateInverterEnable(enable);
    updateInverterTorqueLimit(torque_limit);
}

NightCANPacket *inverter_init(NightCANInstance *can, uint8_t initial_direction, float torque_limit) {
    torqueCommand = CAN_create_packet(0xC0, 3, 8);

    inverter_update_all_fields(0.0f, 0.0f, initial_direction, 0, torque_limit);

    CAN_AddTxPacket(can, &torqueCommand);

    return &torqueCommand;
};

void inverter_update_torque_request(float torque_request) {
    updateTorqueRequest(torque_request);
    updateInverterEnable(torque_request > 0 ? 1 : 0);
};
