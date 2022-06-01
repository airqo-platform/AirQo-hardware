#include <Arduino.h>
#include "configuration.h"
#include "core/sampling_routine.h"
#include "core/ota.h"
#include "core/sd-rountines.h"
#include "core/battery.h"
#include "core/eeprom_routines.h"
#include "core/rtc_routines.h"
const String FirebaseAuth="FHhUlDab1cKWpZvDmpbUcNl3Jd3DoJkAmLTulFkJ"; //addded upload to a firebase srealtime database first attempt towards redundancy
//device parameter variables, will be updated in eeprom //
String FIRMWARE_NAME="AIRQO_FIRMWARE_OOTS";
String FIRMWARE_VERSION=" ";
String DEVICE_NAME=" ";
String WRITE_KEY=" ";
String CHANNELID=" ";
String CTRL_CHANNELID=" ";
bool PM1_TOO= true;
SoftwareSerial pmSerial(13,12);
PMS pms1(Serial3);
PMS pms2(pmSerial);
bool debugSwitch=true;
long channelID=1622708;
long controlChannelID= 807813;
const char apn[]="internet";
const uint8_t GSMSWITCH =A0;
int lastUpdateCheck=0;
uint8_t updateCheckInterval=2;
uint8_t count=0;
//setup function for the things.
void checkForConfigurationInput();
void printMenu();
void debugPrintOut();
void wakeUp();
void setup() {
  EEPROM.begin();
  Serial.begin(115200);
  Serial3.begin(9600);
  pmSerial.begin(9600);
  pinMode(SENSORSWITCH,OUTPUT);
  pinMode(2,INPUT_PULLUP);
  checkForConfigurationInput();
  beep(1); //beeper is weak on the gen5 revc design, need to improve hardware 
  //attachInterrupt(digitalPinToInterrupt(2),debugPrintOut,FALLING);
  //attachInterrupt(digitalPinToInterrupt(3),wakeUp,LOW);
  if(initialize_SD(SD_SS))
  {
    if(debugSwitch){ Serial.println(F("initalized SD_CARD succesfully"));}
  }
  else 
      Serial.println(F("failed to initialize SD_CARD, please check your card")); 
  startGsm(GSMSWITCH);
  //turnOffGsm(54);
  delay(1000);
  //init_RTC();
}
void loop() {
  Serial.println(getGsmTimeStamp());
  if(min-lastUpdateCheck>=updateCheckInterval) //updates are checked for evey 5 minutes, this is configurable;
  {
      if((checkForUpdate(CTRL_CHANNELID)&&updateEligibility==true)||updateEligibility==true)
      {
        Serial.println(F("preparing to download update"));
        if(downloadFile("firmware.bin")==DOWNLOAD_COMPLETE)
        {
          updateEligibility=false; //toggle update eligibility if download and was successful.
          //flash the downloded file to memeory or schedule a restart ;
          //ad here the routine to reset the controller eeprom address for update
          //we can add here a last broadcast to a n iot platofrm with data about the downloaded file, update version, date of update and status of device.
          //this will help us to track operations of the devices.
          updateReboot();
        }
      }
      else{
        Serial.println(F("No update available"));
        getGsmTimeStamp();
      }
      lastUpdateCheck=min;
  }
  samplePmSensor(pms1,pms2,10,!PM1_TOO);
  //Serial.println(measureBattery(A1));
  mqttUpdate( CHANNELID,dataToPublish);
  //FirebaseUpload("{updates:["+dataFile.read()+"]}");
  //Serial.println(obtainTime());
  //sampleDHT(20);
  //sampleSHT(20);
  //sampleBME(20);
  //Serial.println(getGsmTimeStamp());
  //checkForUpdate(CTRL_CHANNELID);
  delay(1000); //conventional dely for good old times!! 
}
void debugPrintOut()
{
  debugSwitch= !debugSwitch;
}
void wakeUp()
{

}
