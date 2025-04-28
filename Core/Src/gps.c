//
// Created by Alex Huang on 2/9/25.
//
#include "gps.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NMEA_GPS_BUFFER_SIZE 128 //could prolly be smaller
GPSData *gps_data; //global variable for gps data

void parseCoordinates(char *nmea_sentence) {
    // Use a copy to avoid modifying the original buffer via strtok
    char sentence_copy[256];
    strncpy(sentence_copy, nmea_sentence, sizeof(sentence_copy) - 1);
    sentence_copy[sizeof(sentence_copy) - 1] = '\0';

    char *token;
    int field_count = 0;
    bool lat_set = false;
    bool lon_set = false;

    // Temporary variables to hold NMEA format values before conversion
    double nmea_lat = 0.0;
    double nmea_lon = 0.0;

    token = strtok(sentence_copy, ",");
    while (token != NULL) {
        switch (field_count) {
            case 2: // Status (A=Active, V=Void)
                if (strlen(token) > 0) {
                    gps_data->status = token[0];
                } else {
                    gps_data->status = 'V';
                }
                break;
            case 3: // Latitude (DDMM.MMMM)
                nmea_lat = atof(token);
                lat_set = true;
                break;
            case 4: // Latitude hemisphere (N/S)
                if (lat_set && strlen(token) > 0 && token[0] == 'S') {
                    nmea_lat = -nmea_lat; // Apply sign to NMEA value
                }
                break;
            case 5: // Longitude (DDDMM.MMMM)
                nmea_lon = atof(token);
                lon_set = true;
                break;
            case 6: // Longitude hemisphere (E/W)
                if (lon_set && strlen(token) > 0 && token[0] == 'W') {
                    nmea_lon = -nmea_lon; // Apply sign to NMEA value
                }
                break;
        }
        token = strtok(NULL, ",");
        field_count++;
    }

    // --- CONVERSION TO DECIMAL DEGREES ---
    if (lat_set) {
        double degrees = floor(fabs(nmea_lat) / 100.0);
        double minutes = fmod(fabs(nmea_lat), 100.0);
        // Apply the original sign after calculation
        gps_data->latitude = copysign(degrees + (minutes / 60.0), nmea_lat);
    } else {
        gps_data->latitude = 0.0; // Or some other default/invalid indicator
    }

    if (lon_set) {
        double degrees = floor(fabs(nmea_lon) / 100.0);
        double minutes = fmod(fabs(nmea_lon), 100.0);
        // Apply the original sign after calculation
        gps_data->longitude = copysign(degrees + (minutes / 60.0), nmea_lon);
    } else {
        gps_data->longitude = 0.0; // Or some other default/invalid indicator
    }
}

// parseHeading and process_nmea remain the same as the previous corrected version
// (assuming parseHeading doesn't need coordinate conversion)

void parseHeading(char *nmea_sentence){
    char sentence_copy[256];
    strncpy(sentence_copy, nmea_sentence, sizeof(sentence_copy) - 1);
    sentence_copy[sizeof(sentence_copy) - 1] = '\0';

    char *token;
    int field_count = 0;
    token = strtok(sentence_copy, ",");
    while (token != NULL){
        switch (field_count){
            case 1:
                gps_data->heading = atof(token);
                break;
            case 7:
                gps_data->speed = atof(token); //km/hr
                break;
        }
        token = strtok(NULL, ",");
        field_count++;
    }
}

bool process_nmea(char *nmea_GPS_BUFFER) {
    if ((strncmp(nmea_GPS_BUFFER, "$GPRMC", 6) == 0)){
        if (gps_data) {
            parseCoordinates(nmea_GPS_BUFFER);
        } else { return false; }
        return true;
    }
    if (strncmp(nmea_GPS_BUFFER, "$GPVTG", 6) == 0){
        if (gps_data) {
            parseHeading(nmea_GPS_BUFFER);
        } else { return false; }
        return true;
    }
    return false;
}

void calculate_checksum(char *command, char *checksum) {
    uint8_t sum = 0;
    for (int i = 0; command[i] != '\0'; i++) {
        sum ^= command[i];
    }
    sprintf(checksum, "%02X", sum);
}

int send_gps_command(const char *command) {
    int status;
    char send_command[128];
    char checksum[3];

    calculate_checksum((char *)command, checksum);
    snprintf(send_command, sizeof(send_command), "$%s*%s\r\n", command, checksum);

    status = HAL_UART_Transmit(&huart4, (uint8_t *)send_command, strlen(send_command), HAL_MAX_DELAY);
    //delay ?
    return status;
}

void setup_gps(GPSData *gps) { //call in main outside loop
    int status = send_gps_command("PMTK251,9600"); //set baud rate to 9600
    if (status != HAL_OK){
        return; //there is a problem :( - set up fault thing
    }
    status = send_gps_command("PMTK314,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"); //enable GPRMC and GPVTG
    if (status != HAL_OK){
        return;
    }
    status = send_gps_command("PMTK220,1000"); //set update rate to 1Hz
    if (status != HAL_OK){
        return;
    }
    status = send_gps_command("$PGCMD,33,1*6C"); //request antenna status
    if (status != HAL_OK){
        return;
    }

    gps_data = gps;
}


void receiveGPSData() { // call in main, put in loop
    memset(GPS_BUFFER, 0, sizeof(GPS_BUFFER)); //clear GPS_BUFFER
    uint8_t startChar;
    if (HAL_UART_Receive(&huart4, &startChar, 1, 100) == HAL_OK){
        if (startChar == "$"){//found start of NMEA package
            GPS_BUFFER[0] = startChar; //set start of GPS_BUFFER
            uint16_t numBytes = 0;
        }
    }
    HAL_UART_Receive(&huart4, GPS_BUFFER, 1, 1000);
    if (GPS_BUFFER[0] == "$") {
        uint16_t numBytes = 0;
        HAL_UARTEx_ReceiveToIdle(&huart4, GPS_BUFFER + 1, 1023, &numBytes, HAL_MAX_DELAY);
        HAL_StatusTypeDef status = HAL_UARTEx_ReceiveToIdle(&huart4, (uint8_t*)GPS_BUFFER + 1, NMEA_GPS_BUFFER_SIZE - 2, &numBytes, 100);
        if (status == HAL_OK && numBytes > 0){ //received data
            GPS_BUFFER[numBytes + 1] = '\0';
            if (process_nmea(GPS_BUFFER)){ //can use checksum to verify a valid message and if data is uncorrupted
                printf("Latitude: %.6f, Longitude: %.6f, Status: %c, Heading: %.6f, Speed: %.2f\n", gps_data->latitude, gps_data->longitude, gps_data->status, gps_data->heading, gps_data->speed);
            }
        }

    }
}
