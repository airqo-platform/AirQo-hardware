#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_
#define TINY_GSM_MODEM_SIM800
#define TINY_GSM_RX_BUFFER 1024
#define TINY_GSM_USE_GPRS true
#define SerialAT Serial1
#include "core/sd-rountines.h"
extern File dataFile;
extern String DEVICE_NAME;
extern String WRITE_KEY;
extern String CHANNELID;
extern String CTRL_CHANNELID;
extern uint8_t DEVICE_NAME_ADDRESS_OFFSET;
extern uint8_t WRITE_KEY_ADDRESS_OFFSET;
extern uint8_t CHANNEL_ADDRESS_OFFSET;
extern uint8_t UPDATE_ID_ADDRESSS_OFFSET;
extern uint8_t CONTROL_CAHNNELID_ADDRESS_OFFSET;
extern uint8_t FIRMWARE_VERSION_ADDRESS_OFFSET;
extern bool debugSwitch;
extern String FIRMWARE_NAME;
extern String FIRMWARE_VERSION;
extern bool gsmPowerStatus;
extern bool PM1_TOO;
extern int new_entry_id,old_entry_id;
extern bool updateAvailable;
extern bool updateEligibility;
extern String token;
extern long channelID;
extern const char apn[];
extern const uint8_t GSMSWITCH;
extern const uint8_t SENSORSWITCH; 
extern const char clientID[];
extern const char mqttUserName[];
extern const char mqttPass[];;
extern float dataToPublish[8];
extern int year,month,day,hour,min,sec;
extern float timezone;
extern uint8_t updateCheckInterval;
extern uint8_t samples;
extern uint8_t bulk_samples;
extern float batteryVolatge;
extern float sensor1pmReadings_pm1[];
extern float sensor1pmReadings_pm25[];
extern float sensor1pmReadings_pm10[];
extern float sensor2pmReadings_pm1[];
extern float sensor2pmReadings_pm25[];
extern float sensor2pmReadings_pm10[];
extern float sensor1averagedPm1,sensor1averagedPm25,sensor1averagedPm10;
extern float sensor2averagedPm1,sensor2averagedPm25,sensor2averagedPm10;
#endif