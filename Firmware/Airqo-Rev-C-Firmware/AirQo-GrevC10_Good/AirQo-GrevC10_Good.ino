//************************************************************************************************





// Select your modem:
//#define TINY_GSM_MODEM_SIM7600
#define TINY_GSM_MODEM_SIM800

#if !defined(TINY_GSM_RX_BUFFER)
#define TINY_GSM_RX_BUFFER 1024
#endif

// See all AT commands, if wanted
// #define DUMP_AT_COMMANDS

// Define the serial console for debug prints, if needed
#define TINY_GSM_DEBUG Serial

// Range to attempt to autobaud
// NOTE:  DO NOT AUTOBAUD in production code.  Once you've established
// communication, set a fixed baud rate using modem.setBaud(#).
#define GSM_AUTOBAUD_MIN 4800
#define GSM_AUTOBAUD_MAX 115200

// Add a reception delay, if needed.
// This may be needed for a fast processor at a slow baud rate.
// #define TINY_GSM_YIELD() { delay(2); }





#define GSM_PIN ""

// Your GPRS credentials, if any
const char apn[]      = "TM"; //internet
const char gprsUser[] = "";
const char gprsPass[] = "";


#define TINY_GSM_TEST_GPRS true
#define TINY_GSM_TEST_TEMPERATURE true
#define TINY_GSM_TEST_GSM_LOCATION true
#define TINY_GSM_TEST_NTP true
#define TINY_GSM_TEST_TIME true


// set GSM PIN, if any


#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>

#ifdef DUMP_AT_COMMANDS
#include <StreamDebugger.h>
StreamDebugger debugger(Serial1, Serial);
TinyGsm        modem(debugger);
#else
TinyGsm        modem(Serial1);
#endif

//#ifdef USE_SSL
//TinyGsmClientSecure client(modem);
//const int           port = 443;
//#else
//TinyGsmClient  client(modem);
//const int      port = 80;
//#endif


//const int           sPort = 443;
const int      port = 80;



//****************************MQTT*****************************************************************
//const char* broker = "broker.mqttdashboard.com";
////const char* broker = "395123438a454425b8bfbdba25d4fa11.s1.eu.hivemq.cloud";
//const char* AirqoPubTopic="AirQo/Pub";
//const char* AirqoSubTopic="AirQo/Sub";
//uint32_t lastReconnectAttempt = 0;
//
//
//boolean runOnce = false;
//
//#include<PubSubClient.h>
//PubSubClient   mqtt(client);



//*********************************LOW POWER ****************************************************************
#include <SPI.h>
#include "LowPower.h"
#include <Wire.h>

///*****************SD card and RTC and Json ******************************************************************
//#include <pgmspace.h>
#include <SD.h>
#include "RTClib.h"

RTC_DS3231 rtc;
int8_t SDCS = "58";
#define sdcardChipSelect SDCS



int jsonEnd=0;
File dataFile;

boolean jsonStart = true;
boolean SDFileStart = true;
boolean SDInserted = false;
boolean RTCAvailable = false;
boolean SDFileReadable= false;

char*  fileName = "tempData.txt";
//String SDTemp = "tempData.txt";
char jsonBuffer [1300]; // Initialize the jsonBuffer to hold data

//char buff[500];  // Initialize the jsonBuffer to hold SD data


//int8_t count = 10;
int8_t countCon;
int8_t countConMax = 5; // STORAGE COUNT MAX HOW MANY ENTRIES STORED
int8_t bulkMax = 5;  // HOW MANY ENTRIES ARE MADE FOR BULK STRINGED UPLOAD. SRAM CAPACITY TO BE MINDED
int8_t countConMaxDeltat = 5; // STORAGE COUNT MAX DeltaT

long startMillis; long millsCTR = 0; 
//char dateTime[26];



//****************** FOTA PARAMS ******************************************************************************
#include <EEPROM.h>
#include <avr/wdt.h>


float fetchedFirmawareVersion = 0.0;
boolean fetchedUpdateCtrl = false;
String fetchedConfigFileToken = "";
String fetchedFirmawareFileToken = "";
long controlChannelID = 1694247;
String controlChannelReadKey = "23BLMB7RX4E3KUZB";
int8_t NoConfigLine;


int8_t DOWNLOAD_FAILED = 0;
int8_t DOWNLOAD_COMPLETE = 1;
int8_t DOWNLOAD_INCOMPLETE = 2;

long FOTATimer = 0; long FOTAInterval = 10800000; boolean FOTACTR = false; ///3hours check interval


///****************Location and time variables****************************************************
/// GSM/ internet time variables
  int   year3    = 0;
  int   month3   = 0;
  int   day3     = 0;
  int   hour3    = 0;
  int   min3     = 0;
  int   sec3     = 0;
  float timezone = 0;


  /// GPS time variables
  float lat2      = 0;  float lon2     = 0;  float speed2    = 0;  float alt2      = 0;  int   vsat2     = 0;  int   usat2     = 0;  float accuracy2 = 0;  
  
  int   year2     = 0;  int   month2    = 0;  int   day2      = 0;  int   hour2     = 0;  int   min2      = 0;  int   sec2      = 0;

 char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


///**********************************PMS5003 delacrations********************
#include "PMS.h"
#include <SoftwareSerial.h>
SoftwareSerial mySerial(13, 12); // RX, TX

#define pmsSwitch A2
PMS pms(Serial3);PMS::DATA data;
PMS pms2(mySerial);PMS::DATA data2;

//***********************************DHT11, BME, SHT25 declarations**************************************************************** 
#include "uFire_SHT20.h"
uFire_SHT20 sht20;
int8_t SHTIntTemp; int8_t SHTIntHumidity;

#include <dht11.h>
dht11 DHT11;
int8_t IntTemp; int8_t IntHumidity;
int8_t csq;
int8_t temp;

#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;
#define DHT11PIN A3
#define IandETempSwitch A8

float ExtTemp; float ExtHumidity; float ExtPressure; int ExtAltitude;



//************************************DEV props***************************************************************
boolean EnableAlarms;
boolean DebugEnable;


boolean DeploymentMode;
#define StaticDeploymentMode  0
#define MobileDeploymentMode  1
String deploymentDescription;

int8_t MobileOperator;
#define MTN  0
#define Airtel  1
#define General  2

//String DevRevDisplayValue[]={"Rev A","Rev B","Rev C"};
//int8_t DevRev;
//#define RevA  0
//#define RevB  1
//#define RevC  2

float FirmwareVersion = 2.0;
String FirwareVersionDisplay;
String DevName;
String DevGen;
int8_t GsmDEF;



String MobileOperatorDisplayValue[]={"MTN", "Airtel", "All"};


#define MAX_SLEEP_ITERATIONS   50
#define CommsSwitch A0
#define Battery A1
#define GSMReset 10 



int8_t pmSamples; 
int8_t start = 0; 
int8_t sleepCounter = 0; //int waitTime = 10000;
boolean started=false;
//*************************************************************************************************** 


//************************* Define the thingspeak information *****************************

int numdata;
//char inSerial[50];
int i = 0;

char datastreamId[] = "1";
bool GPSworks = false;
int8_t failtimes = 0;

long channelId; //=  1205188;
char server[] = "api.thingspeak.com"; // ThingSpeak Server
char thingSpeakAddress[] = "184.106.153.149";
char writeAPIKey[30]; // = "3D65XPOPLCI1IW20";

char toPostGPSData[100];    


//****************** EEPROM CONFIG *************************************************
#include <EEPROM.h>
String readString;

#define configTrigger 2 

//****************** Device properties.**********************************************
  void DevProperties(){
 
 
  //delay(1000);
  
 // DeploymentMode = StaticDeploymentMode;
  EnableAlarms = true;
  DebugEnable = true;

  //DebugEnable = false;
  //MobileOperator = Airtel;
  //MobileOperator = MTN;
    MobileOperator = General;

 //DevRev = RevC;
 // DevName = "AQG5"+DevNumber+""+ DevRevDisplayValue[DevRev];
 // DevName = read_String(1)+" "+ DevRevDisplayValue[DevRev];  FirmwareVersion
  DevName = read_String(1);
  FirwareVersionDisplay = "Firmware Version: "+ String(FirmwareVersion); 
  channelId = read_EE(31).toInt();
  
  //writeAPIKey = read_String(61); 
  readtobuffer(61, writeAPIKey);  writeAPIKey[strlen(writeAPIKey)-1] = '\0';
  DeploymentMode =  read_EE(100).toInt();
  SDCS = read_EE(103).toInt();
  if(SDCS =="" ||SDCS == "0"){ SDCS = 58;}
//  GsmDEF = read_String(108).toInt();
//
//  
//  
//  if(GsmDEF == 0 ){
//    Serial.println("haz");
//    #define TINY_GSM_MODEM_SIM800
//    }
//    else if(GsmDEF ==1 ){
//      Serial.println("hazel");
//      #define TINY_GSM_MODEM_SIM7600
//     }
//   else{
//    Serial.println("hazel joel");
//    #define TINY_GSM_MODEM_SIM7600
//   }

   
  if(DebugEnable){
  Serial.println(F(""));
  Serial.println(DevName);
  Serial.println(FirwareVersionDisplay);
  Serial.println("Mobile Operator: "+MobileOperatorDisplayValue[MobileOperator]);
  }
  //DeploymentMode = MobileDeploymentMode;
  if(DeploymentMode == StaticDeploymentMode){
    deploymentDescription = "Static Deployment"; 
    pmSamples = 5;    
        }else{
      deploymentDescription = "Mobile Deployment";
      pmSamples = 10;
      }if(DebugEnable){
      Serial.println(deploymentDescription);
      Serial.println("SD CS: "+String(SDCS));}
       if(EnableAlarms){
  noTone(22);  tone(22, 4000, 200); }
  }





void startGsm(uint8_t gsmSwitch)
{
    Serial.println(F("starting gsm now"));
    digitalWrite(gsmSwitch,HIGH);
   // Serial1.begin(9600);
    modem.init();
    Serial.println(modem.getModemInfo());
    modem.waitForNetwork(20000L,true);
    if(modem.isNetworkConnected())
    {
        //Serial.println(getGsmTimeStamp());
        Serial.print(F("connected to:"));
        Serial.println(modem.getOperator());
        //getGsmTimeStamp();
        Serial.println(modem.getSignalQuality());
        //Serial.println(checkBalances());
        modem.gprsConnect(apn);
        if(modem.isGprsConnected())
        {
            
            Serial.print(F("connected to:"));Serial.println(apn);
        }
    }
}
  

  void modemInfo(){
  digitalWrite(CommsSwitch, HIGH);
   if(DebugEnable){Serial.print(F("Starting"));
   Serial.print(F(" * ")); delay(1000);   Serial.print(F(" * "));delay(1000);   Serial.print(F(" * "));delay(1000);
   Serial.print(F(" * "));delay(1000);   Serial.print(F(" * "));delay(1000);   Serial.print(F(" * "));delay(1000);
   Serial.print(F(" * "));delay(1000);   Serial.print(F(" * "));delay(1000);   Serial.print(F(" * "));delay(1000);
   Serial.print(F(" * "));delay(1000);   Serial.print(F(" * "));delay(1000);   Serial.print(F(" * "));delay(1000);
   Serial.print(F(" * "));delay(1000);   Serial.print(F(" * "));delay(1000);   Serial.print(F(" * "));delay(1000);
   Serial.print(F(" * "));delay(1000);   Serial.print(F(" * "));Serial.println(F(""));}
   
  
     // Restart takes quite some time
  // To skip it, call init() instead of restart()  SerialDBG("Initializing modem...");
  //if (!modem.restart()) {
   if (!modem.init()) {
    if(DebugEnable){Serial.println(F("Failed to restart modem, delaying 10s and retrying"));}
    // restart autobaud in case GSM just rebooted
    // TinyGsmAutoBaud(SerialAT, GSM_AUTOBAUD_MIN, GSM_AUTOBAUD_MAX);
    return;
  }

  String name = modem.getModemName();
  DBG("Modem Name:", name);

  String modemInfo = modem.getModemInfo();
  DBG("Modem Info:", modemInfo);



  if(DebugEnable){Serial.println(F("Waiting for network..."));}
  if (!modem.waitForNetwork(600000L, true)) {
    delay(10000);
    return;
  }

  if (modem.isNetworkConnected()) {  if(DebugEnable){Serial.println(F("Network connected"));} }
 // modem.gprsConnectImpl();

#if TINY_GSM_TEST_GPRS
  DBG("Connecting to", apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
     delay(10000);
    return;
  }

  bool res = modem.isGprsConnected();
  DBG("GPRS status:", res ? "connected" : "not connected");

  String ccid = modem.getSimCCID();
  DBG("CCID:", ccid);

  String imei = modem.getIMEI();
  DBG("IMEI:", imei);

  String imsi = modem.getIMSI();
  DBG("IMSI:", imsi);

  String cop = modem.getOperator();
  DBG("Operator:", cop);

  IPAddress local = modem.localIP();
  DBG("Local IP:", local);

  csq = modem.getSignalQuality();
  DBG("Signal quality:", csq);
#endif


#if TINY_GSM_TEST_NTP && defined TINY_GSM_MODEM_HAS_NTP
  if(DebugEnable){Serial.println(F("Asking modem to sync with NTP"));}
  modem.NTPServerSync("132.163.96.5", 20);
#endif

#if TINY_GSM_TEST_TIME && defined TINY_GSM_MODEM_HAS_TIME

  for (int8_t i = 3; i; i--) {
    if(DebugEnable){Serial.println(F("Requesting current network time"));}
    if (modem.getNetworkTime(&year3, &month3, &day3, &hour3, &min3, &sec3,
                             &timezone)) {
      DBG("Year:", year3, "\tMonth:", month3, "\tDay:", day3);
      DBG("Hour:", hour3, "\tMinute:", min3, "\tSecond:", sec3);
      DBG("Timezone:", timezone);
      //delay(100);
      if(DebugEnable){Serial.println(F("Adjusting RTC"));}
     delay(50);
      rtc.adjust(DateTime(year3, month3, day3, hour3, min3, sec3));
       //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
       
     delay(50);

     if(DebugEnable){Serial.println(F("new clock data"));
     Serial.println(obtainTime());}
    // setRTC();
      break;
      

    } else {
      if(DebugEnable){Serial.println(F("Couldn't get network time, retrying in 15s."));}
      delay(15000L);
    }
  }
//  if(DebugEnable){Serial.println(F("Retrieving time again as a string"));}
//  String time = modem.getGSMDateTime(DATE_FULL);
//  DBG("Current Network Time:", time);
//

#endif

#if TINY_GSM_TEST_TEMPERATURE && defined TINY_GSM_MODEM_HAS_TEMPERATURE
  temp = modem.getTemperature();
  DBG("Chip temperature:", temp);
#endif


//delay(5000);

  }




  void GPRSconnect(){
  digitalWrite(CommsSwitch, HIGH);
  if (modem.isNetworkConnected()) {  if(DebugEnable){Serial.println(F("Network connected"));} }
   DBG("Connecting to", apn);
  if (!modem.gprsConnect(apn, gprsUser, gprsPass)) {
    delay(5000);
    return;
  }

//  bool res = modem.isGprsConnected();
//  DBG("GPRS status:", res ? "connected" : "not connected");
  }

  void modemPowerOff(){
    
         // delay(2000L);
          modem.gprsDisconnect();
          //delay(4000L);
          if (!modem.isGprsConnected()) {
            if(DebugEnable){Serial.println(F("GPRS disconnected"));}
          } else {
            if(DebugEnable){Serial.println(F("GPRS disconnect: Failed."));}
          }       
        
        
          // Try to power-off (modem may decide to restart automatically)
          // To turn off modem completely, please use Reset/Enable pins
          modem.poweroff();
          if(DebugEnable){Serial.println(F("Poweroff."));}
          delay(500);
          digitalWrite(CommsSwitch, LOW);
        
    }
   
//******************************************************************************



  


void setup() {
  
  Serial.begin(115200); Serial3.begin(9600);  mySerial.begin(9600);  
  //Serial1.begin(115200);  
  Serial1.begin(9600);  
  
  
  pinMode(IandETempSwitch, OUTPUT);
  pinMode(pmsSwitch, OUTPUT);
  pinMode(Battery, INPUT);
  pinMode(CommsSwitch, OUTPUT);
  pinMode(configTrigger, INPUT);

  
  
  digitalWrite(IandETempSwitch, LOW); //switch IE temp sensor off, power saving mode
  digitalWrite(pmsSwitch, HIGH);// switch sensor off, power saving mode
  //digitalWrite(CommsSwitch, HIGH);
  digitalWrite(configTrigger, HIGH);
  delay(700);
  if(digitalRead(configTrigger) == LOW){DevSetup();}
  DevProperties();
  
   
//  if(DevRev ==RevA){ Serial1.begin(9600); Serial.print("Rev A "); }
//  else if(DevRev ==RevB){Serial1.begin(9600);  Serial.print("Rev B "); }
//  else if(DevRev ==RevC){ Serial1.begin(115200);   Serial.print("Rev C "); }

  
  BatteryVoltage();
 if(DebugEnable){Serial.println(F(""));} 
 

     enableSD();  
       
     modemInfo();
     //startGsm(CommsSwitch);
     readLocation(); 
     modemPowerOff();
     
}



void loop() {
 
  Check_pms();
 
  
}
