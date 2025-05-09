//
// Created by Alex Huang on 2/9/25.
//

#ifndef VCU_FIRMWARE_2025_GPS_H
#define VCU_FIRMWARE_2025_GPS_H

#include<stdint.h>
#include<stdbool.h>
#include "night_can.h"


#define NMEA_BUFFER_SIZE 128
typedef struct {
    float latitude;
    float longitude;
    char status;
    float heading;
    float speed;
    int fix;
    int satellites;
} GPSData;

void setup_gps(GPSData *gps, NightCANInstance *canInstance);
void receiveGPSData();
bool process_nmea(char *nmea_buffer);
int send_gps_command(const char *command);
void calculate_checksum(char *command, char *checksum);
void parseHeading(char *nmea_sentence);
void parseCoordinates(char *nmea_sentence);

void send_GPS_CAN();

extern uint8_t GPS_BUFFER[256];

#endif //VCU_FIRMWARE_2025_GPS_H
