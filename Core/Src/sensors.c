//
// Created by Dhairya Gupta on 4/22/25.
//

/*
 * This file defines the data read from CAN and from other inputs necessary for the torque path of the car
 * that are passed into the VCU model.
 */

#include "sensors.h"

static NightCANReceivePacket flWheelSpeeds;
static NightCANReceivePacket frWheelSpeeds;
static NightCANReceivePacket rlWheelSpeeds;
static NightCANReceivePacket rrWheelSpeeds;
static NightCANReceivePacket flUprightAccel; // TODO unused
static NightCANReceivePacket frUprightAccel;
static NightCANReceivePacket rlUprightAccel;
static NightCANReceivePacket rrUprightAccel;

static NightCANReceivePacket flUndertray; // TODO unused
static NightCANReceivePacket frUndertray;
static NightCANReceivePacket rlUndertray;
static NightCANReceivePacket rrUndertray;
static NightCANReceivePacket flUndertrayAngularRate;
static NightCANReceivePacket frUndertrayAngularRate;
static NightCANReceivePacket rlUndertrayAngularRate;
static NightCANReceivePacket rrUndertrayAngularRate;

static NightCANReceivePacket apps;
static NightCANReceivePacket bse;
// TODO BPPS
static NightCANReceivePacket steeringAngle;

static NightCANReceivePacket driveSwitch;


void sensors_init(FDCAN_HandleTypeDef* hcan, NightCANInstance* canInstance)
{
  flWheelSpeeds = CAN_create_receive_packet(WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FL_ID,
                                            WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FL_FREQ * 5,
                                            // will timeout after 5 missed packets -- at this point we should just disable driving until received again
                                            WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FL_DLC);

  frWheelSpeeds = CAN_create_receive_packet(WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FR_ID,
                                            WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FR_FREQ * 5,
                                            // will timeout after 5 missed packets -- at this point we should just disable driving until received again
                                            WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FR_DLC);

  rlWheelSpeeds = CAN_create_receive_packet(WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_RL_ID,
                                            WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_RL_FREQ * 5,
                                            // will timeout after 5 missed packets -- at this point we should just disable driving until received again
                                            WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_RL_DLC);

  rrWheelSpeeds = CAN_create_receive_packet(WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_RR_ID,
                                            WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_RR_FREQ * 5,
                                            // will timeout after 5 missed packets -- at this point we should just disable driving until received again
                                            WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_RR_DLC);



  flUndertray = CAN_create_receive_packet(ACCELERATION_VECTOR_SPRUNG_RIDE_HEIGHT_FL_ID,
  ACCELERATION_VECTOR_SPRUNG_RIDE_HEIGHT_FL_FREQ * 5, ACCELERATION_VECTOR_SPRUNG_RIDE_HEIGHT_FL_DLC);
  frUndertray = CAN_create_receive_packet(ACCELERATION_VECTOR_SPRUNG_RIDE_HEIGHT_FR_ID,
  ACCELERATION_VECTOR_SPRUNG_RIDE_HEIGHT_FR_FREQ * 5, ACCELERATION_VECTOR_SPRUNG_RIDE_HEIGHT_FR_DLC);
  rlUndertray = CAN_create_receive_packet(ACCELERATION_VECTOR_SPRUNG_RIDE_HEIGHT_RL_ID,
  ACCELERATION_VECTOR_SPRUNG_RIDE_HEIGHT_RL_FREQ * 5, ACCELERATION_VECTOR_SPRUNG_RIDE_HEIGHT_RL_DLC);
  rrUndertray = CAN_create_receive_packet(ACCELERATION_VECTOR_SPRUNG_RIDE_HEIGHT_RR_ID,
    ACCELERATION_VECTOR_SPRUNG_RIDE_HEIGHT_RR_FREQ * 5, ACCELERATION_VECTOR_SPRUNG_RIDE_HEIGHT_RR_DLC);

  // ACCELERATION_VECTOR_UNSPRUNG_FL_ID





  steeringAngle = CAN_create_receive_packet(RACK_STEERING_ID, RACK_STEERING_FREQ * 5, RACK_STEERING_DLC);


  driveSwitch = CAN_create_receive_packet(1738,
                                          100 * 5,
                                          // will timeout after 5 missed packets -- at this point we should just disable driving completely
                                          1);

  apps = CAN_create_receive_packet(APPS_VOLTAGES_ID, 100, APPS_VOLTAGES_DLC);
  bse = CAN_create_receive_packet(BSE_VOLTAGES_ID, 100, BSE_VOLTAGES_DLC);


  // Actually start tracking these newly set up packets.
  CAN_addReceivePacket(canInstance, &flWheelSpeeds);
  CAN_addReceivePacket(canInstance, &frWheelSpeeds);
  CAN_addReceivePacket(canInstance, &rlWheelSpeeds);
  CAN_addReceivePacket(canInstance, &rrWheelSpeeds);
  // TODO other corner stuff

  CAN_addReceivePacket(canInstance, &flUndertray);
  CAN_addReceivePacket(canInstance, &frUndertray);
  CAN_addReceivePacket(canInstance, &rlUndertray);
  CAN_addReceivePacket(canInstance, &rrUndertray);

  CAN_addReceivePacket(canInstance, &driveSwitch);
  CAN_addReceivePacket(canInstance, &apps);
  CAN_addReceivePacket(canInstance, &bse);
  CAN_addReceivePacket(canInstance, &steeringAngle);
};

/** Will call the other methods to update all inputs to the model */
void sensors_periodic(SensorData* sensorData, VcuModelInputs *vcuModelInputs)
{
  sensors_updateDriveSwitch(sensorData, vcuModelInputs);
  sensors_updateWheelSpeeds(sensorData, vcuModelInputs);
  sensors_updatePedalBox(sensorData, vcuModelInputs);
}

/** Read the correct packet from CAN and update the drive switch setting */
static void sensors_updateDriveSwitch(SensorData* sensorData, VcuModelInputs *vcuModelInputs)
{
    vcuModelInputs->driveSwitchEnabled = !driveSwitch.data[0];
}

static void sensors_updateWheelSpeeds(SensorData* sensorData, VcuModelInputs *vcuModelInputs)
{
  sensors_updateCorner(&(sensorData->fl), &flWheelSpeeds, &flUprightAccel, &flUndertray, &flUndertrayAngularRate);
  sensors_updateCorner(&(sensorData->fr), &frWheelSpeeds, &frUprightAccel, &frUndertray, &frUndertrayAngularRate);
  sensors_updateCorner(&(sensorData->rl), &rlWheelSpeeds, &rlUprightAccel, &rlUndertray, &rlUndertrayAngularRate);
  sensors_updateCorner(&(sensorData->rr), &rrWheelSpeeds, &rrUprightAccel, &rrUndertray, &rrUndertrayAngularRate);

  // TODO update vcu model inputs
}

static void sensors_updateCorner(Corner* corner, NightCANReceivePacket* uprightPacket,
  NightCANReceivePacket* uprightAccelPacket, NightCANReceivePacket* undertrayPacket, NightCANReceivePacket* undertrayAngularRatePacket)
{
  corner->wheelSpeed = CAN_readFloat(WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FL_SPEED_TYPE, uprightPacket,
  WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FL_SPEED_BYTE, WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FL_SPEED_PREC);
  corner->susPot = CAN_readFloat(WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FL_SPRING_DISPLACEMENT_TYPE, uprightPacket,
    WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FL_SPRING_DISPLACEMENT_BYTE, WHEEL_SPEED_STRAIN_GAUGE_PUSHROD_SPRING_DISP_FL_SPRING_DISPLACEMENT_PREC);
  corner->rideHeight = CAN_readFloat(ACCELERATION_VECTOR_SPRUNG_RIDE_HEIGHT_FL_RIDE_HEIGHT_TYPE, undertrayPacket, ACCELERATION_VECTOR_SPRUNG_RIDE_HEIGHT_FL_RIDE_HEIGHT_BYTE, ACCELERATION_VECTOR_SPRUNG_RIDE_HEIGHT_FL_RIDE_HEIGHT_PREC);
  // TODO...
}

static void sensors_updatePedalBox(SensorData* sensorData, VcuModelInputs *vcuModelInputs)
{
  if (apps.is_timed_out)
  {
    vcuModelInputs->apps1Voltage = 0;
    vcuModelInputs->apps2Voltage = 0;
  }
  else
  {
    // Update voltages
    vcuModelInputs->apps1Voltage = CAN_readFloat(APPS_VOLTAGES_APPS1_VOLTAGE_TYPE, &apps,
                                                       APPS_VOLTAGES_APPS1_VOLTAGE_BYTE,
                                                       APPS_VOLTAGES_APPS1_VOLTAGE_PREC);
    vcuModelInputs->apps2Voltage = CAN_readFloat(APPS_VOLTAGES_APPS2_VOLTAGE_TYPE, &apps,
                                                       APPS_VOLTAGES_APPS2_VOLTAGE_BYTE,
                                                       APPS_VOLTAGES_APPS2_VOLTAGE_PREC);
  }

  sensorData->pedalBox.appsVoltage1 = vcuModelInputs->apps1Voltage;
  sensorData->pedalBox.appsVoltage2 = vcuModelInputs->apps2Voltage;
  sensorData->pedalBox.columnAngle = CAN_readFloat(RACK_STEERING_STEERING_COLUMN_ANGLE_TYPE, &steeringAngle,
    RACK_STEERING_STEERING_COLUMN_ANGLE_BYTE, RACK_STEERING_STEERING_COLUMN_ANGLE_PREC);


  if(bse.is_timed_out)
  {
    vcuModelInputs->bseFVoltage = 0;
    vcuModelInputs->bseRVoltage = 0;
  } else
  {
    vcuModelInputs->bseFVoltage = CAN_readFloat(BSE_VOLTAGES_BSE_FRONT_VOLTAGE_TYPE, &bse,
                                                       BSE_VOLTAGES_BSE_FRONT_VOLTAGE_BYTE,
                                                       BSE_VOLTAGES_BSE_FRONT_VOLTAGE_PREC);
    vcuModelInputs->bseRVoltage = CAN_readFloat(BSE_VOLTAGES_BSE_REAR_VOLTAGE_TYPE, &bse,
                                                       BSE_VOLTAGES_BSE_REAR_VOLTAGE_BYTE,
                                                       BSE_VOLTAGES_BSE_REAR_VOLTAGE_PREC);
  }
  sensorData->pedalBox.bseFVoltage = vcuModelInputs->bseFVoltage;
  sensorData->pedalBox.bseRVoltage = vcuModelInputs->bseRVoltage;
};
