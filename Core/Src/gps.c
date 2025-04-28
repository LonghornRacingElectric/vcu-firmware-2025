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

const double KNOTS_TO_MPH = 1.15077945;


NightCANPacket gpsPacket;
// Updated function to parse Coordinates, Speed (MPH), and Heading from GPRMC
void parseGPRMC(char *nmea_sentence) {
    // Use a copy to avoid modifying the original buffer via strtok
    char sentence_copy[256];
    strncpy(sentence_copy, nmea_sentence, sizeof(sentence_copy) - 1);
    sentence_copy[sizeof(sentence_copy) - 1] = '\0'; // Ensure null termination

    char *token;
    int field_count = 0;
    bool lat_set = false;
    bool lon_set = false;
    bool speed_set = false;
    bool heading_set = false;

    // Temporary variables to hold NMEA format values before conversion
    double nmea_lat = 0.0;
    double nmea_lon = 0.0;
    double speed_knots = 0.0;
    double track_angle = 0.0;

    token = strtok(sentence_copy, ",");
    while (token != NULL) {
        // Check for empty tokens, treat them as 0 or handle as needed
        double current_val = (strlen(token) > 0) ? atof(token) : 0.0;

        switch (field_count) {
            case 2: // Status (A=Active, V=Void)
                if (strlen(token) > 0) {
                    gps_data->status = token[0];
                } else {
                    gps_data->status = 'V'; // Default to Void if empty
                }
                break;
            case 3: // Latitude (DDMM.MMMM)
                nmea_lat = current_val;
                lat_set = true;
                break;
            case 4: // Latitude hemisphere (N/S)
                if (lat_set && strlen(token) > 0 && token[0] == 'S') {
                    nmea_lat = -nmea_lat; // Apply sign
                }
                break;
            case 5: // Longitude (DDDMM.MMMM)
                nmea_lon = current_val;
                lon_set = true;
                break;
            case 6: // Longitude hemisphere (E/W)
                if (lon_set && strlen(token) > 0 && token[0] == 'W') {
                    nmea_lon = -nmea_lon; // Apply sign
                }
                break;
            case 7: // Speed over ground (Knots)
                speed_knots = current_val;
                speed_set = true;
                break;
            case 8: // Track angle / Course over ground (Degrees True)
                track_angle = current_val;
                heading_set = true;
                break;
                // Add cases for time (1), date (9), mode (12) if needed
        }
        token = strtok(NULL, ",");
        field_count++;
    }

    // --- CONVERSIONS AND ASSIGNMENTS ---

    // Position (Decimal Degrees)
    if (lat_set) {
        double degrees = floor(fabs(nmea_lat) / 100.0);
        double minutes = fmod(fabs(nmea_lat), 100.0);
        gps_data->latitude = copysign(degrees + (minutes / 60.0), nmea_lat);
    } else {
        gps_data->latitude = 0.0; // Or NAN, or keep old value
    }

    if (lon_set) {
        double degrees = floor(fabs(nmea_lon) / 100.0);
        double minutes = fmod(fabs(nmea_lon), 100.0);
        gps_data->longitude = copysign(degrees + (minutes / 60.0), nmea_lon);
    } else {
        gps_data->longitude = 0.0; // Or NAN, or keep old value
    }

    // Speed (Convert Knots to MPH)
    if (speed_set) {
        gps_data->speed = speed_knots * KNOTS_TO_MPH;
    } else {
        gps_data->speed = 0.0; // Default if not set
    }

    // Heading (Degrees True)
    if (heading_set) {
        gps_data->heading = track_angle;
    } else {
        gps_data->heading = 0.0; // Default if not set
    }
}

// This function is NO LONGER NEEDED if only using GPRMC
/*
void parseHeading(char *nmea_sentence){
    // ... (old code) ...
}
*/

// Simplified process_nmea to only handle GPRMC
bool process_nmea(char *nmea_GPS_BUFFER) {
    // Check for $GPRMC sentence
    if (strncmp(nmea_GPS_BUFFER, "$GPRMC", 6) == 0) {
        if (gps_data != NULL) {
            // Use the updated function name
            parseGPRMC(nmea_GPS_BUFFER);
            return true; // Indicate successful processing
        } else {
            // Handle error: gps_data pointer is NULL
            fprintf(stderr, "Error: gps_data pointer is NULL in process_nmea.\n");
            return false;
        }
    }

    // If you needed to parse other sentences like GGA, add checks here
    // if (strncmp(nmea_GPS_BUFFER, "$GPGGA", 6) == 0) { ... }

    // Sentence type not recognized or not handled
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

void setup_gps(GPSData *gps, NightCANInstance *canInstance) { //call in main outside loop
    int status = send_gps_command("PMTK251,115200"); //set baud rate to 9600
    if (status != HAL_OK){
        return; //there is a problem :( - set up fault thing
    }
    status = send_gps_command("PMTK314,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0"); //enable GPRMC and GPVTG
    if (status != HAL_OK){
        return;
    }
    status = send_gps_command("PMTK220,100"); //set update rate to 10Hz
    if (status != HAL_OK){
        return;
    }
    status = send_gps_command("$PGCMD,33,1*6C"); //request antenna status
    if (status != HAL_OK){
        return;
    }

    gps_data = gps;

    gpsPacket = CAN_create_packet(GPS_ID, GPS_FREQ, GPS_DLC);
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

void send_GPS_CAN() {
    CAN_writeFloat(GPS_REAR_HEADING_TYPE, &gpsPacket, GPS_REAR_HEADING_BYTE, gps_data->heading, GPS_REAR_HEADING_PREC);
    CAN_writeFloat(GPS_REAR_LATITUDE_TYPE, &gpsPacket, GPS_REAR_LATITUDE_BYTE, gps_data->latitude, GPS_REAR_LATITUDE_PREC);
    CAN_writeFloat(GPS_REAR_LONGITUDE_TYPE, &gpsPacket, GPS_REAR_LONGITUDE_BYTE, gps_data->longitude, GPS_REAR_LONGITUDE_PREC);
    CAN_writeFloat(GPS_REAR_SPEED_TYPE, &gpsPacket, GPS_REAR_SPEED_BYTE, gps_data->speed, GPS_REAR_SPEED_PREC);
}

