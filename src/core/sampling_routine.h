#ifndef SAMPLING_ROUTINE_H_
#define SAMPLING_ROUTINE_H_
#include <Arduino.h>
#include "sd-rountines.h"
#include <SoftwareSerial.h>
#include <PMS.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_BME280.h>
#include <Wire.h>
#include <ArduinoJson.h>
#include <SHT2x.h>
void samplePmSensor(PMS sensor1,PMS Sensor2,uint8_t sampleCount ,bool pm1_too);
void sampleDHT(uint8_t sampleCount);
void sampleSHT(uint8_t sampleCount);
void sampleBME(uint8_t sampleCount);
String obtainTime();
#endif