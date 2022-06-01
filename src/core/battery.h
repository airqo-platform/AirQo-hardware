#ifndef BATTERY_H_
#define BATTERY_H_
#include <Arduino.h>
float measureBattery(uint8_t batteryTerminal);
void beep(uint8_t beeps);
#endif