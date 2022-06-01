#ifndef SD_ROUTINES_H_
#define SD__ROUTINES_H_
#define SD_SS 58 // sd card chipselct pin
#include<Arduino.h>
#include <SPI.h>
#include <SD.h>
bool initialize_SD(uint8_t ss_pin);
bool writeSD_data(String fileName);
void deleteFile(String fileName);
#endif