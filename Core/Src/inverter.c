//
// Created by Dhairya Gupta on 4/1/25.
//

#include "inverter.h"

static NightCANPacket torqueCommand;

void updateTorqueRequest(float torque_request) {
    CAN_writeFloat(INVERTER_TORQUE_COMMAND_TORQUE_REQUEST_TYPE, &torqueCommand,
                   INVERTER_TORQUE_COMMAND_TORQUE_REQUEST_BYTE, torque_request,
                   INVERTER_TORQUE_COMMAND_TORQUE_REQUEST_PREC); // torque request
}

void updateSpeedRequest(float speed_request) {
    CAN_writeFloat(INVERTER_TORQUE_COMMAND_RPM_REQUEST_TYPE, &torqueCommand, INVERTER_TORQUE_COMMAND_RPM_REQUEST_BYTE, speed_request, INVERTER_TORQUE_COMMAND_RPM_REQUEST_PREC); // speed request
}

void updateInverterDirection(uint8_t direction) {
    CAN_writeInt(uint8_t, &torqueCommand, INVERTER_TORQUE_COMMAND_DIRECTION_BYTE, direction); // inverter direction
}

void updateInverterEnable(uint8_t enable) {
    CAN_writeInt(uint8_t, &torqueCommand, INVERTER_TORQUE_COMMAND_ENABLE_BYTE, enable); // inverter enable
}

void updateInverterTorqueLimit(float torque_limit) {
    CAN_writeFloat(INVERTER_TORQUE_COMMAND_TORQUE_LIMIT_TYPE, &torqueCommand, INVERTER_TORQUE_COMMAND_TORQUE_LIMIT_BYTE, torque_limit, INVERTER_TORQUE_COMMAND_TORQUE_LIMIT_PREC);
}

void inverter_update_all_fields(float torque_request, float speed_request, uint8_t direction, uint8_t enable, float torque_limit) {
    updateTorqueRequest(torque_request);
    updateSpeedRequest(speed_request);
    updateInverterDirection(direction);
    updateInverterEnable(enable);
    updateInverterTorqueLimit(torque_limit);
}

NightCANPacket *inverter_init(NightCANInstance *can, uint8_t initial_direction, float torque_limit) {
    torqueCommand = CAN_create_packet(INVERTER_TORQUE_COMMAND_ID, INVERTER_TORQUE_COMMAND_FREQ, INVERTER_TORQUE_COMMAND_DLC);

    inverter_update_all_fields(0.0f, 0.0f, initial_direction, 0, torque_limit);

    CAN_AddTxPacket(can, &torqueCommand);

    return &torqueCommand;
};

void inverter_update_torque_request(float torque_request) {
    updateTorqueRequest(torque_request);
    updateInverterEnable(torque_request > 0 ? 1 : 0);
};
