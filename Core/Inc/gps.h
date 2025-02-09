//
// Created by Alex Huang on 2/9/25.
//

#ifndef VCU_FIRMWARE_2025_GPS_H
#define VCU_FIRMWARE_2025_GPS_H

#include<stdint.h>
#include<stdbool.h>

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

void setup_gps();
void receiveData();
bool process_nmea(char *nmea_buffer);
void parse_data(char *nmea_sentence);



#endif //VCU_FIRMWARE_2025_GPS_H
