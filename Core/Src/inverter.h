//
// Created by Dhairya Gupta on 4/1/25.
//

#ifndef VCU_FIRMWARE_2025_INVERTER_H
#define VCU_FIRMWARE_2025_INVERTER_H

#include "night_can.h"

NightCANPacket inverter_init(NightCANInstance *can, uint8_t initial_direction, float torque_limit);
void inverter_update_torque_request(float torque_request);

void inverter_update_all_fields(float torque_request, float speed_request, uint8_t direction,
                                uint8_t enable, float torque_limit);

#endif //VCU_FIRMWARE_2025_INVERTER_H
