#ifndef EEPROM_ROUTINES_H_
#define EEPROM_ROUTINES_H_
#include <EEPROM.h>
#include <avr/wdt.h>
#include <Arduino.h>
void writeString(char add,String data);
String read_String(char add);
String ReadEEPROM();
int writeStringToEEPROM(int addrOffset, const String &strToWrite);
int readStringFromEEPROM(int addrOffset, String *strToRead);
void updateReboot();
void clearEEPROM();
void printMenu();
void checkForConfigurationInput();
#endif