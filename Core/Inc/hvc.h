//
// Created by Matthew on 6/18/2025.
//

#ifndef HVC_H
#define HVC_H

#include "night_can.h"

typedef struct HvcStatus {
  float batteryMaxTemp;  // (deg C)
  float batterySoc;  // (%)
  bool energized;
  bool bmsError;
  bool imdError;
  bool isTimedOut;
} HvcStatus;


void hvc_init(NightCANInstance* canInstance);

void hvc_periodic(HvcStatus* status);

#endif //HVC_H
