//
// Created by Alex Huang on 2/9/25.
//
#include "gps.h"
#include "usart.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NMEA_BUFFER_SIZE 128 //could prolly be smaller
GPSData gps_data = {0}; //global variable for gps data

void parseCoordinates(char *nmea_sentence) {
    char *token;
    int field_count = 0;
    token = strtok(nmea_sentence, ",");
    while (token != NULL) {
        switch (field_count) {
            case 2:
                gps_data.status = token[0];
                break;
            case 3:
                gps_data.latitude = atof(token);
                break;
            case 4:
                if (token[0] == 'S') gps_data.latitude = -gps_data.latitude;
                break;
            case 5:
                gps_data.longitude = atof(token);
                break;
            case 6:
                if (token[0] == 'W') gps_data.longitude = -gps_data.longitude;

        }
        token = strtok(NULL, ",");
        field_count++;
    }
}

void parseHeading(char *nmea_sentence){
    char *token;
    int field_count = 0;
    token = strtok(nmea_sentence, ",");
    while (token != NULL){
        switch (field_count){
            case 1:
                gps_data.heading = atof(token);
            case 7:
                gps_data.speed = atof(token); //km/hr
        }

        token = strtok(NULL, ",");
        field_count++;
    }
}

bool process_nmea(char *nmea_buffer) {
    if ((strncmp(nmea_buffer, "$GPRMC", 6) == 0)){ //receiving GPRMC Data
        parseCoordinates(nmea_buffer);
        return true;
    }
    if (strncmp(nmea_buffer, "$GPVTG", 6) == 0){
        parseHeading(nmea_buffer);
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

    status = HAL_UART_Transmit(&huart7, (uint8_t *)send_command, strlen(send_command), HAL_MAX_DELAY);
    //delay ?
    return status;
}

void setup_gps() { //call in main outside loop
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

}

char buffer[NMEA_BUFFER_SIZE];
int buffer_index = 0;

void receiveData() { // call in main, put in loop
    memset(buffer, 0, sizeof(buffer)); //clear buffer
    uint8_t startChar;
    if (HAL_UART_Receive(&huart7, &startChar, 1, 100) == HAL_OK){
        if (startChar == "$"){//found start of NMEA package
            buffer[0] = startChar; //set start of buffer
            uint16_t numBytes = 0;
        }
    }
    HAL_UART_Receive(&huart7, buffer, 1, 1000);
    if (buffer[0] == "$") {
        uint16_t numBytes = 0;
        HAL_UARTEx_ReceiveToIdle(&huart7, buffer + 1, 1023, &numBytes, HAL_MAX_DELAY);
        HAL_StatusTypeDef status = HAL_UARTEx_ReceiveToIdle(&huart7, (uint8_t*)buffer + 1, NMEA_BUFFER_SIZE - 2, &numBytes, 100);
        if (status == HAL_OK && numBytes > 0){ //received data
            buffer[numBytes + 1] = '\0';
            if (process_nmea(buffer)){ //can use checksum to verify a valid message and if data is uncorrupted
                printf("Latitude: %.6f, Longitude: %.6f, Status: %c, Heading: %.6f, Speed: %.2f\n", gps_data.latitude, gps_data.longitude, gps_data.status, gps_data.heading, gps_data.speed);
            }
        }

    }
}
