//
// Created by Dhairya Gupta on 6/14/25.
//

#ifndef VCU_FIRMWARE_2025_PEDALS_H
#define VCU_FIRMWARE_2025_PEDALS_H

#include "main.h"
#include "night_can.h"
#include "VcuModel.h"

void pedals_init(FDCAN_HandleTypeDef *hcan, NightCANInstance *canInstance);

void pedals_periodic(VCUModelInputs *vcuModelInputs);

#endif //VCU_FIRMWARE_2025_PEDALS_H
