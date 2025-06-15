//
// Created by Dhairya Gupta on 4/1/25.
//

#ifndef VCU_FIRMWARE_2025_INVERTER_H
#define VCU_FIRMWARE_2025_INVERTER_H

#include "night_can.h"

typedef struct InverterData
{
  float dcBusVoltage;
  float outputVoltage; // peak line-neutral
  float dcBusCurrent;
  float motorRpm;
  float motorTemp;
  float electricalAngle;
} InverterData;


NightCANPacket *inverter_init(NightCANInstance *can);
void inverter_update_torque_request(float torque_request);

void inverter_update_all_fields(float torque_request, float speed_request, uint8_t direction,
                                uint8_t enable, float torque_limit);
void inverter_periodic(InverterData* inverterData);

#endif //VCU_FIRMWARE_2025_INVERTER_H
