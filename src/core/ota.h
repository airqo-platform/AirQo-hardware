#ifndef OTA_H_
#define OTA_H_
#include <Arduino.h>
#define DOWNLOAD_FAILED 0
#define DOWNLOAD_COMPLETE 1
#define DOWNLOAD_INCOMPLETE 2
void startGsm(uint8_t gsmSwitch);
bool checkForUpdate(String channel);
uint8_t downloadFile(String fileName);
void mqttSubscriptionCallback( char* topic, byte* payload, unsigned int mesLength );
void mqttUpdate(String channel,float data[]);
void updateFireBase();
void bulkUpdate(String fileName);
void FirebaseUpload(String data);
String getGsmTimeStamp();
bool turnOffGsm(uint8_t gsmSwitch);
String checkBalances();
#endif
