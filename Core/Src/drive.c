//
// Created by Dhairya Gupta on 4/22/25.
//

/*
 * This file defines the data read from CAN and from other inputs necessary for the torque path of the car
 * that are passed into the VCU model.
 */

#include "drive.h"

static NightCANReceivePacket flWheelSpeeds;
static NightCANReceivePacket frWheelSpeeds;
static NightCANReceivePacket rlWheelSpeeds;
static NightCANReceivePacket rrWheelSpeeds;

void drive_system_init(FDCAN_HandleTypeDef *hcan, NightCANInstance *canInstance) {
    flWheelSpeeds = CAN_create_receive_packet(WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FL_ID,
                                              WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FL_FREQ * 5, // will timeout after 5 missed packets -- at this point we should just disable driving until received again
                                              WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FL_DLC);

    frWheelSpeeds = CAN_create_receive_packet(WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FR_ID,
                                              WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FR_FREQ * 5, // will timeout after 5 missed packets -- at this point we should just disable driving until received again
                                              WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FR_DLC);

    rlWheelSpeeds = CAN_create_receive_packet(WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_RL_ID,
                                              WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_RL_FREQ * 5, // will timeout after 5 missed packets -- at this point we should just disable driving until received again
                                              WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_RL_DLC);

    rrWheelSpeeds = CAN_create_receive_packet(WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_RR_ID,
                                              WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_RR_FREQ * 5, // will timeout after 5 missed packets -- at this point we should just disable driving until received again
                                              WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_RR_DLC);
};

/** Will call the other methods to update all inputs to the model */
void drive_system_periodic();

/** Read the correct packet from CAN and update the drive switch setting */
void updateDriveSwitch(VCUModelInputs *vcuModelInputs);

void updateWheelSpeeds(VCUModelInputs *vcuModelInputs) {
//    vcuModelInputs.
}