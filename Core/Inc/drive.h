//
// Created by Dhairya Gupta on 4/22/25.
//

#ifndef VCU_FIRMWARE_2025_DRIVE_H
#define VCU_FIRMWARE_2025_DRIVE_H

#include "VcuModel.h"
#include "main.h"
#include "night_can.h"

void drive_system_init(FDCAN_HandleTypeDef *hcan, NightCANInstance *canInstance);

/** Will call the other methods to update all inputs to the model */
void drive_system_periodic();

/** Read the correct packet from CAN and update the drive switch setting */
void updateDriveSwitch(VCUModelInputs *vcuModelInputs);

void updateWheelSpeeds(VCUModelInputs *vcuModelInputs);

#endif //VCU_FIRMWARE_2025_DRIVE_H
