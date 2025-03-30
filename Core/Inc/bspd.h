//
// Created by Dhairya Gupta on 3/3/25.
//

#ifndef VCU_FIRMWARE_2025_BSPD_H
#define VCU_FIRMWARE_2025_BSPD_H

#include <stdbool.h>

typedef struct BSPDOutputs {
    bool hard_braking, motor_5kw, error, trigger, latch;
} BSPDOutputs;

void bspd_periodic(BSPDOutputs *bspd);

#endif //VCU_FIRMWARE_2025_BSPD_H
