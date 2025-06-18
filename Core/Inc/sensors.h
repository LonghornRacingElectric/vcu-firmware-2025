//
// Created by Dhairya Gupta on 4/22/25.
//

#ifndef VCU_FIRMWARE_2025_DRIVE_H
#define VCU_FIRMWARE_2025_DRIVE_H

#include "VcuModel.h"
#include "main.h"
#include "night_can.h"

typedef struct Corner
{
  float wheelSpeed;  // rad/s
  float susPot;  // ?
  float strain;  // ?
  float rideHeight;
  float unsprungAccelX;
  float unsprungAccelY;
  float unsprungAccelZ;
  float sprungAccelX;
  float sprungAccelY;
  float sprungAccelZ;
  float sprungAngularVelocityX;
  float sprungAngularVelocityY;
  float sprungAngularVelocityZ;
} Corner;

typedef struct PedalBox
{
  float columnAngle;
  float appsVoltage1;
  float appsVoltage2;
  float bseVoltage1;
  float bseVoltage2;
  float bseVoltageMixed;
  float bppsVoltage1;
  float bppsVoltage2;
} PedalBox;

typedef struct SensorData {
  Corner fl;
  Corner fr;
  Corner rl;
  Corner rr;

  PedalBox pedalBox;
} SensorData;


void sensors_init(FDCAN_HandleTypeDef *hcan, NightCANInstance *canInstance);

/** Will call the other methods to update all inputs to the model */
void sensors_periodic(SensorData* sensorData, VcuModelInputs *vcuModelInputs);

/** Read the correct packet from CAN and update the drive switch setting */
static void sensors_updateDriveSwitch(SensorData* sensorData, VcuModelInputs *vcuModelInputs);
static void sensors_updateCorner(Corner* corner, NightCANReceivePacket* uprightPacket,
  NightCANReceivePacket* uprightAccelPacket, NightCANReceivePacket* undertrayPacket, NightCANReceivePacket* undertrayAngularRatePacket);
static void sensors_updateWheelSpeeds(SensorData* sensorData, VcuModelInputs *vcuModelInputs);
static void sensors_updatePedalBox(SensorData* sensorData, VcuModelInputs *vcuModelInputs);


#endif //VCU_FIRMWARE_2025_DRIVE_H
