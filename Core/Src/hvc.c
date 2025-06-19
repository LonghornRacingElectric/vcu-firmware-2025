//
// Created by Matthew on 6/18/2025.
//

#include "hvc.h"

NightCANReceivePacket hvcIndicators;
NightCANReceivePacket hvcContactors;
NightCANReceivePacket hvcPackStatus;


void hvc_init(NightCANInstance* carCan)
{
  hvcIndicators = CAN_create_receive_packet(INDICATORS_SHUTDOWN_STATUS_ID,
                                            INDICATORS_SHUTDOWN_STATUS_FREQ * 5,
                                            INDICATORS_SHUTDOWN_STATUS_DLC);
  hvcContactors = CAN_create_receive_packet(CONTACTOR_STATUS_ID,
                                            CONTACTOR_STATUS_FREQ * 5, CONTACTOR_STATUS_DLC);
  hvcPackStatus = CAN_create_receive_packet(BATTERY_PACK_STATUS_ID,
                                            BATTERY_PACK_STATUS_FREQ * 5, BATTERY_PACK_STATUS_DLC);
  CAN_addReceivePacket(carCan, &hvcIndicators);
  CAN_addReceivePacket(carCan, &hvcContactors);
  CAN_addReceivePacket(carCan, &hvcPackStatus);
}

void hvc_periodic(HvcStatus* status)
{
  status->isTimedOut = hvcIndicators.is_timed_out || hvcContactors.is_timed_out;
  status->energized = hvcContactors.data[1]; // positive contactor state
  status->batterySoc = CAN_readFloat(BATTERY_PACK_STATUS_STATE_OF_CHARGE_TYPE, &hvcPackStatus,
                                     BATTERY_PACK_STATUS_STATE_OF_CHARGE_BYTE,
                                     BATTERY_PACK_STATUS_STATE_OF_CHARGE_PREC);
  status->batteryMaxTemp = CAN_readFloat(BATTERY_PACK_STATUS_CELL_TOP_TEMP_TYPE, &hvcPackStatus,
                                         BATTERY_PACK_STATUS_CELL_TOP_TEMP_BYTE,
                                         BATTERY_PACK_STATUS_CELL_TOP_TEMP_PREC);
}
