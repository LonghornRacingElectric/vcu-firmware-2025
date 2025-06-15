//
// Created by Dhairya Gupta on 4/1/25.
//

#include "inverter.h"

static NightCANPacket torqueCommand;
static NightCANReceivePacket inverterMotorPosition;
static NightCANReceivePacket inverterVoltage;
static NightCANReceivePacket inverterCurrent;

void updateTorqueRequest(float torque_request) {
    CAN_writeFloat(INVERTER_TORQUE_COMMAND_TORQUE_REQUEST_TYPE, &torqueCommand,
                   INVERTER_TORQUE_COMMAND_TORQUE_REQUEST_BYTE, torque_request,
                   INVERTER_TORQUE_COMMAND_TORQUE_REQUEST_PREC); // torque request
}

void updateSpeedRequest(float speed_request) {
    CAN_writeFloat(INVERTER_TORQUE_COMMAND_RPM_REQUEST_TYPE, &torqueCommand, INVERTER_TORQUE_COMMAND_RPM_REQUEST_BYTE, speed_request, INVERTER_TORQUE_COMMAND_RPM_REQUEST_PREC); // speed request
}

void updateInverterDirection(uint8_t direction) {
    CAN_writeInt(INVERTER_TORQUE_COMMAND_DIRECTION_TYPE, &torqueCommand, INVERTER_TORQUE_COMMAND_DIRECTION_BYTE, direction); // inverter direction
}

void updateInverterEnable(uint8_t enable) {
    CAN_writeInt(INVERTER_TORQUE_COMMAND_ENABLE_TYPE, &torqueCommand, INVERTER_TORQUE_COMMAND_ENABLE_BYTE, enable); // inverter enable
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

NightCANPacket *inverter_init(NightCANInstance *can) {
    torqueCommand = CAN_create_packet(INVERTER_TORQUE_COMMAND_ID, INVERTER_TORQUE_COMMAND_FREQ, INVERTER_TORQUE_COMMAND_DLC);

    inverter_update_all_fields(0.0f, 0.0f, 0, 0, 0);

    CAN_AddTxPacket(can, &torqueCommand);

    inverterVoltage = CAN_create_receive_packet(INVERTER_VOLTAGE_ID, INVERTER_VOLTAGE_FREQ * 5, INVERTER_VOLTAGE_DLC);
    inverterCurrent = CAN_create_receive_packet(INVERTER_CURRENT_ID, INVERTER_CURRENT_FREQ * 5, INVERTER_CURRENT_DLC);
    inverterMotorPosition = CAN_create_receive_packet(INVERTER_STATUS_ID, INVERTER_STATUS_FREQ * 5, INVERTER_STATUS_DLC);

    CAN_addReceivePacket(can, &inverterVoltage);
    CAN_addReceivePacket(can, &inverterCurrent);
    CAN_addReceivePacket(can, &inverterMotorPosition);

    return &torqueCommand;
};

void inverter_periodic(InverterData* inverterData)
{
    inverterData->dcBusVoltage = CAN_readFloat(INVERTER_VOLTAGE_DC_BUS_VOLTAGE_TYPE, &inverterVoltage,
        INVERTER_VOLTAGE_DC_BUS_VOLTAGE_BYTE, 0.1f);
    inverterData->outputVoltage = CAN_readFloat(INVERTER_VOLTAGE_NEUTRAL_OUTPUT_VOLTAGE_TYPE, &inverterVoltage,
        INVERTER_VOLTAGE_NEUTRAL_OUTPUT_VOLTAGE_BYTE, 0.1f);
    inverterData->dcBusCurrent = CAN_readFloat(INVERTER_CURRENT_DC_BUS_CURRENT_TYPE, &inverterCurrent,
        INVERTER_CURRENT_DC_BUS_CURRENT_BYTE, 0.1f);
    inverterData->motorRpm = CAN_readFloat(INVERTER_STATUS_MOTOR_SPEED_TYPE, &inverterMotorPosition,
        INVERTER_STATUS_MOTOR_SPEED_BYTE, INVERTER_STATUS_MOTOR_SPEED_PREC);
    inverterData->electricalAngle = CAN_readFloat(INVERTER_STATUS_MOTOR_ANGLE_TYPE, &inverterMotorPosition,
        INVERTER_STATUS_MOTOR_ANGLE_BYTE, INVERTER_STATUS_MOTOR_ANGLE_PREC);
    inverterData->motorTemp = 0;
}

void inverter_update_torque_request(float torque_request) {
    updateTorqueRequest(torque_request);
    updateInverterEnable(torque_request != 0 ? 1 : 0);

    // TODO; VERIFY THIS WORKS
    updateInverterDirection(0);
};
