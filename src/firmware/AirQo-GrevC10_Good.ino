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

///*********************************************************************************************
void Check_pms()
{

  
 Serial.println(F("\n")); Serial.println(F("PMS sensors PowerOn")); 
 mySerial.begin(9600); Serial3.begin(9600); 
 digitalWrite(pmsSwitch, LOW);//switch the sensors on for read.
 pms.wakeUp(); pms2.wakeUp();
 pms.activeMode(); pms2.activeMode();
 delay(100);  
 test_pms();
    
   if(DebugEnable){  Serial.println(F("PM measurements"));}   
    
     // Sums for calculating statistics
              

              double Sensor1PM1Mean = 0.0; double Sensor1PM25Mean = 0.0;  double Sensor1PM10Mean = 0.0;
              double Sensor2PM1Mean = 0.0; double Sensor2PM25Mean = 0.0;  double Sensor2PM10Mean = 0.0;

              double Sensor1PM1Sum = 0.0;  double Sensor1PM25Sum = 0.0;   double Sensor1PM10Sum = 0.0;
              double Sensor2PM1Sum = 0.0;  double Sensor2PM25Sum = 0.0;   double Sensor2PM10Sum = 0.0;

              // Arrays to hold multiple measurements
             
//              int Sensor1PM1Array[pmSamples]; int Sensor1PM25Array[pmSamples];  int Sensor1PM10Array[pmSamples];
//              int Sensor2PM1Array[pmSamples]; int Sensor2PM25Array[pmSamples];  int Sensor2PM10Array[pmSamples];
              int i = 0;

               

               // Make measurements and add to arrays
              //for (int i = 0; i < nMeasurements; i++) 
                do{
                       
                  // Print out the pms5003
                   if (pms.read(data))
                    {
                      if(DebugEnable){
                      Serial.print(F("\nNumber of samples:\t")); Serial.println(i);
                      Serial.print(F("Sensor1 PM 1.0 (ug/m3): ")); Serial.println(data.PM_AE_UG_1_0);                  
                      Serial.print(F("Sensor1 PM 2.5 (ug/m3): ")); Serial.println(data.PM_AE_UG_2_5);                  
                      Serial.print(F("Sensor1PM 10.0 (ug/m3): ")); Serial.println(data.PM_AE_UG_10_0);                  
                      Serial.println(F(""));
                      }
                      Sensor1PM1Sum += data.PM_AE_UG_1_0;
                      Sensor1PM25Sum += data.PM_AE_UG_2_5;
                      Sensor1PM10Sum += data.PM_AE_UG_10_0;
                      
                      
                    }
                  
                    if (pms2.read(data2))
                    {
                      if(DebugEnable){
                      Serial.print(F("Sensor2 PM 1.0 (ug/m3): ")); Serial.println(data2.PM_AE_UG_1_0);                  
                      Serial.print(F("Sensor2 PM 2.5 (ug/m3): ")); Serial.println(data2.PM_AE_UG_2_5);                  
                      Serial.print(F("Sensor2 PM 10.0 (ug/m3): "));  Serial.println(data2.PM_AE_UG_10_0);                  
                      Serial.println(F(""));
                      }
                      Sensor2PM1Sum += data2.PM_AE_UG_1_0;
                      Sensor2PM25Sum += data2.PM_AE_UG_2_5;
                      Sensor2PM10Sum += data2.PM_AE_UG_10_0;
                      i++;
                    }
                 // Serial.println();
                
                        
                }while(i<pmSamples);
              
               pms.sleep(); pms2.sleep();

               delay(100);
               
               mySerial.end(); Serial3.end(); 
               if(DebugEnable){Serial.println(F("PMS sensors PowerOff"));} 
               digitalWrite(pmsSwitch, HIGH);//switch the sensors on for read.
               

               Sensor1PM1Mean = Sensor1PM1Sum /pmSamples;
               Sensor2PM1Sum = Sensor2PM1Sum / pmSamples;
               
               Sensor1PM25Mean = Sensor1PM25Sum / pmSamples;
               Sensor1PM10Mean = Sensor1PM10Sum / pmSamples;
               Sensor2PM25Mean = Sensor2PM25Sum / pmSamples;
               Sensor2PM10Mean = Sensor2PM10Sum / pmSamples;

           
                
              //Find the size of array. 
//              int n = sizeof(Sensor1PM25Array)/sizeof(Sensor1PM25Array[0]); 
//              float pm25Corel  = correlationCoefficient(Sensor1PM25Array, Sensor2PM25Array, 30);
//              float pm10Corel  = correlationCoefficient(Sensor1PM10Array, Sensor2PM10Array, 30);
//             
             if(DebugEnable){
             Serial.println(F(""));
             Serial.println(F("**AVERAGE READINGS**"));
             Serial.print(F("\nNumber of samples:\t")); Serial.println(pmSamples);
             Serial.print(F("Sensor1 PM2.5: ")); Serial.println(Sensor1PM25Mean);
             Serial.print(F("Sensor1 PM10: ")); Serial.println(Sensor1PM10Mean);
             Serial.print(F("Sensor2 PM2.5: ")); Serial.println(Sensor2PM25Mean);
             Serial.print(F("Sensor2 PM10: ")); Serial.println(Sensor2PM10Mean);
//             Serial.print("PM2.5 Coeffient of correlation: "); Serial.println(corr);
//             Serial.print("PM10 Coeffient of correlation: "); Serial.println(pm10Corel);
             Serial.println(F(""));
             //Serial.println("********************************************************");
             }
             postActivities(Sensor1PM1Mean, Sensor1PM25Mean, Sensor1PM10Mean, Sensor2PM1Sum ,Sensor2PM25Mean, Sensor2PM10Mean); 
             //comms(Sensor1PM1Mean, Sensor1PM25Mean, Sensor1PM10Mean, Sensor2PM1Sum ,Sensor2PM25Mean, Sensor2PM10Mean); 
              

                                      
}



//************************************sensor test
void test_pms()
{
   if(DebugEnable){Serial.println(F("Testing PMS dual sensors"));}
    int i = 0;
    int nMeasurements = 1; 
              do{
                       
                  // Print out the pms5003
                   if (pms.read(data))
                    {
//                      SerialDBG("\nNumber of samples:\t"); SerialDBGln(i);
//                      SerialDBG("Sensor1 PM 1.0 (ug/m3): "); SerialDBGln(data.PM_AE_UG_1_0);                  
//                      SerialDBG("Sensor1 PM 2.5 (ug/m3): "); SerialDBGln(data.PM_AE_UG_2_5);                  
//                      SerialDBG("Sensor1PM 10.0 (ug/m3): "); SerialDBGln(data.PM_AE_UG_10_0); 
//                      SerialDBGln();
                      
                    }
                  
                    if (pms2.read(data2))
                    {
//                      SerialDBG("Sensor2 PM 1.0 (ug/m3): "); SerialDBGln(data2.PM_AE_UG_1_0);                  
//                      SerialDBG("Sensor2 PM 2.5 (ug/m3): "); SerialDBGln(data2.PM_AE_UG_2_5);                  
//                      SerialDBG("Sensor2 PM 10.0 (ug/m3): ");  SerialDBGln(data2.PM_AE_UG_10_0);                  
//                      SerialDBGln();

                      i++;
                    }
                 // SerialDBGln();
                        
                }while(i<nMeasurements);
       // SerialDBGln("");      
        if(DebugEnable){Serial.println(F("Done.")); }    
                                      
}


///*******************************************************************************************

void Read_Status(float Sensor1PM25, float Sensor1PM10, float Sensor2PM25, float Sensor2PM10)
{
 if(DebugEnable){ 
  Serial.print(F("AVERAGE Sensor1 PM2.5 CONCENTRATION :"));  Serial.print(Sensor1PM25);  Serial.print(F("ug/M3"));  Serial.print(F(" | "));
  Serial.print(F("AVERAGE Sensor1 PM10 CONCENTRATION :"));  Serial.print(Sensor1PM10);  Serial.print(F("ug/M3"));  Serial.print(F(" | "));
  Serial.print(F("AVERAGE Sensor2 PM2.5 CONCENTRATION :"));  Serial.print(Sensor2PM25);  Serial.print(F("ug/M3"));  Serial.print(F(" | "));
  Serial.print(F("AVERAGE Sensor2 PM10 CONCENTRATION :"));  Serial.print(Sensor2PM10);  Serial.print(F("ug/M3"));
  Serial.print(F("LATITUDE:"));  Serial.print(lat2);  Serial.print(F("LONGITUDE"));  Serial.print(lon2);
 }
 }



void postActivities(double Sensor1PM1Sum, double Sensor1PM25Mean, double Sensor1PM10Mean, double Sensor2PM1Sum, double Sensor2PM25Mean, double Sensor2PM10Mean){   
            enableSDAlone();
            getTempData();
            //readLocation();
            Read_Status(Sensor1PM25Mean, Sensor1PM10Mean, Sensor2PM25Mean, Sensor2PM10Mean);
              //Check_SMS();
               // if(GPSworks == true)
               {

                 

                                                  strcat(toPostGPSData,String(lat2).c_str()); strcat(toPostGPSData,",");  strcat(toPostGPSData,String(lon2).c_str()); strcat(toPostGPSData,","); 
                                                  strcat(toPostGPSData,String(alt2).c_str()); strcat(toPostGPSData,",");  strcat(toPostGPSData,String(speed2).c_str()); strcat(toPostGPSData,",");  
                                                  strcat(toPostGPSData,String(vsat2).c_str()); strcat(toPostGPSData,","); strcat(toPostGPSData,String(accuracy2).c_str()); strcat(toPostGPSData,",");
                                                  strcat(toPostGPSData,String(SHTIntTemp).c_str()); strcat(toPostGPSData,",");  strcat(toPostGPSData,String(SHTIntHumidity).c_str()); strcat(toPostGPSData,","); 
                                                  strcat(toPostGPSData,String(ExtTemp).c_str()); strcat(toPostGPSData,",");  strcat(toPostGPSData,String(ExtHumidity).c_str()); strcat(toPostGPSData,",");  
                                                  strcat(toPostGPSData,String(ExtPressure).c_str()); strcat(toPostGPSData,","); strcat(toPostGPSData,String(ExtAltitude).c_str());
                                                  
                                                  if(DebugEnable){
                                                  Serial.println(F(" "));
                                                  Serial.println(F("**************"));
                                                  Serial.println(toPostGPSData);
                                                  Serial.println(strlen(toPostGPSData));                                                  
                                                  Serial.println(F("**************"));
                                                  }
                                                
                                               
//                          SDInserted = true;
                          RTCAvailable = false;
                       
                        if(SDInserted && RTCAvailable){
                                                    Serial.println(F("Mode 1 SDInserted && RTCAvailable"));
                                                    CSVLogger(String(Sensor1PM1Sum).c_str(), String(Sensor1PM25Mean).c_str(), String(Sensor1PM10Mean).c_str(), String(Sensor1PM1Sum).c_str(), String(Sensor2PM25Mean).c_str(), String(Sensor2PM10Mean).c_str(),
                                                    //String(lat2).c_str(), String(lon2).c_str(), String(lat2).c_str(),
                                                    String(BatteryVoltageRef()).c_str(), String(csq).c_str(), String(temp).c_str(), toPostGPSData); 

                                                  
                                                    StoreTempDataNoLoss(String(Sensor1PM25Mean).c_str(), String(Sensor1PM10Mean).c_str(), String(Sensor2PM25Mean).c_str(), String(Sensor2PM10Mean).c_str(),String(lat2).c_str(), String(lon2).c_str(), 
                                                    String(BatteryVoltageRef()).c_str(), toPostGPSData); 
                                           
                         }
                         else if(SDInserted && RTCAvailable == false){
                                                    Serial.println(F("Mode 3 SDInserted && RTCAvailable == false"));
//                                                  updatesJsonDeltaT(jsonBuffer,String(Sensor1PM25Mean).c_str(), String(Sensor1PM10Mean).c_str(), String(Sensor2PM25Mean).c_str(), String(Sensor2PM10Mean).c_str(), 
//                                                  String(lat2).c_str(), String(lon2).c_str(), String(BatteryVoltageRef()).c_str(), toPostGPSData);           
                                                    //updatesJsonDeltaTWithSDStorage(String(Sensor1PM25Mean).c_str(), String(Sensor1PM10Mean).c_str(), String(Sensor2PM25Mean).c_str(), String(Sensor2PM10Mean).c_str(), 
                                                    //String(lat2).c_str(), String(lon2).c_str(), String(BatteryVoltageRef()).c_str(), toPostGPSData); 
                                                    updatesJsonDeltaTWithSDStorageNoLoss(String(Sensor1PM25Mean).c_str(), String(Sensor1PM10Mean).c_str(), String(Sensor2PM25Mean).c_str(), String(Sensor2PM10Mean).c_str(), 
                                                    String(lat2).c_str(), String(lon2).c_str(), String(BatteryVoltageRef()).c_str(), toPostGPSData);                    
                         }  
                                                         
                         else if(SDInserted == false && RTCAvailable){
                                                    Serial.println(F("Mode 4 SDInserted == false && RTCAvailable"));
                                                   // digitalWrite(CommsSwitch, HIGH);
                                                   // comms(Sensor1PM25Mean, Sensor1PM10Mean, Sensor2PM25Mean, Sensor2PM10Mean, String(BatteryVoltageRef()).c_str()); 
                                                    updatesJson2(jsonBuffer,String(Sensor1PM25Mean).c_str(), String(Sensor1PM10Mean).c_str(), String(Sensor2PM25Mean).c_str(), String(Sensor2PM10Mean).c_str(), 
                                                    String(lat2).c_str(), String(lon2).c_str(), String(BatteryVoltageRef()).c_str(), toPostGPSData);
                         }
                         else if(SDInserted == false && RTCAvailable == false){
                                                                          
                                                     Serial.println(F("Mode 2 SDInserted == false && RTCAvailable == false"));
                                                    if(VBatPt(BatteryVoltageRef())<60){
                                                      updatesJsonDeltaT(jsonBuffer, String(Sensor1PM25Mean).c_str(), String(Sensor1PM10Mean).c_str(), String(Sensor2PM25Mean).c_str(), String(Sensor2PM10Mean).c_str(), 
                                                      String(lat2).c_str(), String(lon2).c_str(), String(BatteryVoltageRef()).c_str(), toPostGPSData); 
                                                      }
                                                    else if(VBatPt(BatteryVoltageRef())>=60){                                                     
                                                    digitalWrite(CommsSwitch, HIGH);
                                                  //  ModemStart();
                                                    readLocation();
                                                    GPRSconnect();
                                                    //startGsm(CommsSwitch);
                                                    comms(Sensor1PM25Mean, Sensor1PM10Mean, Sensor2PM25Mean, Sensor2PM10Mean, String(BatteryVoltageRef()).c_str()); 
                                                     }
                         } 
                                                  
                              }    
                             // start++;
             
                             toPostGPSData[0] = '\0';    
}

///****************************************************************************
