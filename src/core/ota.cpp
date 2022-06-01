#include "ota.h"
#include "sd-rountines.h"
#include "configuration.h"
#include <TinyGsmClient.h>
#include <ArduinoHttpClient.h>
#include <PubSubClient.h>
#include "eeprom_routines.h"
#include <ArduinoJson.h>
TinyGsm modem(SerialAT);
//explore the possibility of reading data from flash storage uisng the progmem feature
const char ThingspeakServer[]="api.thingspeak.com";
const char FireBaseServer[]="firebasestorage.googleapis.com";
const char FirebaseHost[]="airqo-hardware-01-default-rtdb.firebaseio.com";
const String FirebaseAuth="FHhUlDab1cKWpZvDmpbUcNl3Jd3DoJkAmLTulFkJ";
//for testing uploads to firebase storage.
//const char FirebaseDatabaseURL[]="smart-water-quality-moni-8953e-default-rtdb.europe-west1.firebasedatabase.app";
const char mqttServer[]= "mqtt3.thingspeak.com";
const int FireBasePort=443;
const char resource[]="/v0/b/airqo-hardware-01.appspot.com/o/";
String token="4ff5ab79-212f-494e-883e-081f8d68b346";
String postResource="update?api_key=BSZY6FE0Q961K71&field1=0";
String api_key="X0YQDRA4F6FMRDI2";
//these parameters should be configurable too, hopefully read from eeprom, or from the sd card on startup
//eeprom however stores strings in lower case !! this will affect the 
const char clientID[]="CicpCBIeECM8NBMwDDkcGTM";
const char mqttUserName[]="CicpCBIeECM8NBMwDDkcGTM";
const char mqttPass[]="iBCrWUtnKxtuIDx5RRDTW/Sn";
//int fieldsToPublish[8]={1,1,1,1,1,1,1,1};
float dataToPublish[8]={0.00,0.00,0.00,0.00,0.00,0.00,0.00,0.00}; //array of data to publish to fields
char controlData[400]="";
int year,month,day,hour,min,sec ;
float timezone;
int new_entry_id=0,old_entry_id=0;
bool gsmPowerStatus=true;
bool updateAvailable=false;
bool updateEligibility=false;
char endOfHeaders[]="\r\n\r\n";
String checkBalances()
{
    //simcard number
    if(modem.getOperator()=="MTN-UGANDA")
    {
        Serial.println(modem.sendUSSD("*135#"));
        return modem.sendUSSD("*131#");
    }
    else if(modem.getOperator()=="CelTel Cellular")
    {
        Serial.println (modem.sendUSSD("*100*7*2#"));
        return modem.sendUSSD("*175*4#");
    }
    else
        return modem.sendUSSD("*131#");
    // if(balance.indexOf("MB")>0||balance.indexOf("GB")>0)
    // {
    //     // Serial.println(balance.indexOf("MB"));
    //     balance=balance.substring(balance.indexOf("MB")-3);
    // }
    // return balance;
}
String getGsmTimeStamp()
{
    String timeStamp;
    if (modem.getNetworkTime(&year, &month, &day, &hour, &min, &sec, &timezone))
         {
             timeStamp=String(year)+"-"+String(month)+"-"+String(day)+"T"+String(hour)+":"+String(min)+":"+String(sec)+"-"+String(timezone);
         }
    return timeStamp;
}
bool turnOffGsm(uint8_t gsmSwitch)
{
    if(modem.isGprsConnected())
    {
        modem.gprsDisconnect();
        //modem.poweroff();
    }
    modem.radioOff();
    gsmPowerStatus=false;
    return gsmPowerStatus;
}
void startGsm(uint8_t gsmSwitch)
{
    Serial.println(F("starting gsm now"));
    pinMode(gsmSwitch,OUTPUT);
    digitalWrite(gsmSwitch,HIGH);
    SerialAT.begin(9600);
    modem.init();
    Serial.println(modem.getModemInfo());
    modem.waitForNetwork(20000L,true);
    if(modem.isNetworkConnected())
    {
        Serial.println(getGsmTimeStamp());
        Serial.print(F("connected to:"));
        Serial.println(modem.getOperator());
        getGsmTimeStamp();
        Serial.println(modem.getSignalQuality());
        Serial.println(checkBalances());
        modem.gprsConnect(apn);
        if(modem.isGprsConnected())
        {
            
            Serial.print(F("connected to:"));Serial.println(apn);
        }
    }
}
uint8_t downloadFile(String fileName)
{
    uint8_t downloadStatus=0;
    // File logFile;
    TinyGsmClientSecure client(modem);
    HttpClient http(client,FireBaseServer,FireBasePort);
    uint32_t expectedFileSize;
    if(!modem.isNetworkConnected())
    {
        modem.waitForNetwork(20000L,true);
    }
    if(modem.isNetworkConnected())
    {
        Serial.println(F("network already connected:"));
        if(!modem.isGprsConnected())
        {
            modem.gprsConnect(apn);
        }
        if(modem.isGprsConnected())
        {
            Serial.print(F("connecting to: "));Serial.println(FireBaseServer);
            http.connect(FirebaseHost,FireBasePort);
            http.connectionKeepAlive();
            int err=http.get(resource+fileName+"?alt=media&token="+token);
            if(err==0&&http.responseStatusCode()==200)
            {
                Serial.print(F("response status code:"));
                Serial.println(http.responseStatusCode());
                while(http.headerAvailable())
                {
                    if(http.readHeaderName()=="Content-Length")
                    {
                        Serial.print(F("expected file size: "));
                        String temp=http.readHeaderValue();
                        expectedFileSize=temp.toInt();
                        Serial.print(expectedFileSize);
                        Serial.println(F(" bytes"));
                    }
                }
                if (http.isResponseChunked()) 
                {
                    Serial.println(F("The response is chunked"));
                }
                unsigned long timeoutStart = millis();
                uint32_t startTime=0;
                deleteFile(fileName);
                dataFile=SD.open(fileName,FILE_WRITE);
                if(dataFile)//if file is successfully opened
                {
                    Serial.print(F("downloading file:"));Serial.println(fileName);
                    startTime=millis();
                    http.beginRequest();
                    //http.connectionKeepAlive();
                    while(http.connected()&&http.available()&&!http.endOfBodyReached()&&millis()-timeoutStart<30000L)
                    {
                        //we can also explore non blocking sd card writes,could improve the speed of downloads. but requires the 
                        //serverresponse to be treated a string, yet its best returned as a bytes for successfull flashing into memory
                        if(http.available())
                        {
                            dataFile.print(char(http.read()));
                        }
                        if(millis()-timeoutStart>=1000)
                        {
                            Serial.print(F("#"));
                        }
                        timeoutStart=millis();
                    }
                }
                else{Serial.println(F("SD card filewrite error"));downloadStatus=DOWNLOAD_FAILED;}
                if(dataFile.size()!=expectedFileSize)
                {
                    downloadStatus=DOWNLOAD_FAILED;
                    Serial.print(F("\t downlaod failed !!.."));
                    Serial.print(F("downloaded file size: "));
                    Serial.println(dataFile.size());
                }
                else if (dataFile.size()==expectedFileSize)
                {
                    downloadStatus=DOWNLOAD_COMPLETE;
                    Serial.println(F("\t download complete"));
                    Serial.print(F("\t downloaded file size: "));
                    Serial.println(dataFile.size());
                    writeStringToEEPROM(FIRMWARE_VERSION_ADDRESS_OFFSET,FIRMWARE_VERSION);//only update the firmware version if a download is complete
                }
                else;
                http.endRequest();
                http.stop();
                client.stop();
                Serial.print((millis()-startTime)/1000);Serial.println(F(" s"));
                dataFile.close();
                modem.gprsDisconnect();
            }
            else if(err!=0)
            {
                Serial.println(F("failed to connect to server"));
                downloadStatus=DOWNLOAD_FAILED;
                delay(2000);
            }
            else if(http.responseStatusCode()!=200)
            {
                //Serial.print(F("response status code:"));
                Serial.println(http.responseStatusCode());
                //Serial.println(F("check your connection parameters please"));
                downloadStatus=DOWNLOAD_FAILED;
            }
        }
    }
    return downloadStatus;
}
bool checkForUpdate(String channel)
{
    //Serial.println(ThingspeakServer);
    if(!modem.isGprsConnected())
    {
        modem.gprsConnect(apn);
    }
    if(modem.isGprsConnected())
    {
        Serial.println(modem.getLocalIP());
        TinyGsmClient client(modem);
        HttpClient http(client,ThingspeakServer,80);
        Serial.println(F("checking for update token from thingspeak"));
        String contentType = "application/json";
        http.connect(ThingspeakServer,80);
        http.connectionKeepAlive();
        int err=http.get("/channels/"+channel+"/feeds/last.json?results=1");
        if(err==0&&http.responseStatusCode()==200)
        {
            //Serial.println(http.responseStatusCode());
            if(!http.find(endOfHeaders))
            {
                Serial.println(F("invalid resonse"));
            }
            if(http.isResponseChunked())
            {
                Serial.println(F("response is chuncked"));
            }
            unsigned long timeoutStart=millis();
            StaticJsonDocument<400> doc;
            while(http.connected()&&http.available()&&!http.endOfBodyReached()&&millis()-timeoutStart<30000L)
            {
                 http.connectionKeepAlive();
                if(http.available())
                {
                    http.readBytesUntil('\r',controlData,sizeof(controlData));
                }
                timeoutStart=millis();
            }
            controlData[1]='{';
            DeserializationError error=deserializeJson(doc ,controlData);
            serializeJsonPretty(doc,Serial);
            if(error){
                Serial.print(F("deserializeJson() failed:"));
                Serial.println(error.f_str());
                return;
            }
            Serial.println();
            //capturing the control parameters to use to configure device remotely.
            new_entry_id=doc["entry_id"].as<int>();
            if(new_entry_id!=EEPROM.read(0))
            {
                Serial.println(F("update available"));
                updateAvailable=true;
                String Tnames=doc["field2"].as<String>();
                Tnames.toLowerCase();
                if(Tnames.indexOf(DEVICE_NAME)>0)
                {
                    //name exists in the update cluster.
                    Serial.println(Tnames);
                    Serial.println(F("found device name in cluster"));
                    updateEligibility=true;
                    FIRMWARE_VERSION=doc["field6"].as<String>();
                    Serial.print(F("New firmware version: "));Serial.println(FIRMWARE_VERSION);
                    //Serial.println(FIRMWARE_VERSION);
                    //first check is done.
                }
                else if(updateAvailable&&!updateEligibility)
                {
                    updateEligibility=false;
                    Serial.println(F("device doesn't qualify for the update"));
                }
            }
            else{
                updateAvailable=false;
            }
            EEPROM.update(0,new_entry_id);
            old_entry_id=new_entry_id;
            Serial.println(FIRMWARE_VERSION);
            http.endRequest();
            http.stop();
        }
        else if(err!=0)
        {
            Serial.println(F("failed to connect to server"));
            delay(2000);
        }
        else if(http.responseStatusCode()!=200)
            {
                Serial.print(F("response status code:"));
                Serial.println(http.responseStatusCode());
                Serial.println(F("check your connection parameters please"));
            }
        client.stop();
        
    }
    modem.gprsDisconnect();
    return updateAvailable; 
}
void mqttSubscriptionCallback( char* topic, byte* payload, unsigned int mesLength ) {
    
    char p[mesLength + 1];
    memcpy( p, payload, mesLength );
    p[mesLength] = NULL;
    Serial.print( F("Answer: " ));
    Serial.println( String(p) );
}
void mqttUpdate(String channel,float data[])
{   
    TinyGsmClientSecure client(modem);
    PubSubClient  mqttClient(client);
    if(!modem.isNetworkConnected())
    {
        modem.waitForNetwork(20000L,1);
    }
    if(modem.isNetworkConnected())
    {
        if(debugSwitch){Serial.println(F("network already connected"));}
        if(!modem.isGprsConnected()){modem.gprsConnect(apn);}
        if(modem.isGprsConnected())
        {
            Serial.println(F("connected to internet"));
            mqttClient.setServer(mqttServer,8883);
            mqttClient.setCallback( mqttSubscriptionCallback );
            mqttClient.setBufferSize( 2048 );
            uint8_t count=0;
            //connect to mqtt client
            while ( !mqttClient.connected() )
            {
                // Connect to the MQTT broker.
                Serial.println(F("connecting to thingspeak mqtt broker"));
                if ( mqttClient.connect( clientID, mqttUserName, mqttPass ) ) {
                Serial.print( F("MQTT to ") );
                Serial.print( mqttServer );
                Serial.print (F(" at port "));
                Serial.print( 8883 );
                Serial.println( F(" successful.") );
                } else {
                    Serial.print( F("MQTT connection failed, rc = " ));
                    // See https://pubsubclient.knolleary.net/api.html#state for the failure code explanation.
                    Serial.print( mqttClient.state() );
                    Serial.println( F(" Will try again in a few seconds") );
                    delay( 100 );
                    count++;
                    }
                if(count>=3)
                    break;
            }
            if(mqttClient.connected())
            {
                mqttClient.loop();
                uint8_t index=0;
                String dataString="";
                //int fieldArray[]={1,1,1,1,1,1,1,1}; //update allfields with data.
                // while (index<8){
                //     // Look at the field array to build the posting string to send to ThingSpeak.
                //     if (fieldArray[ index ]>0){
                //         dataString+="&field" + String( index+1 ) + "="+String( data [ index ] );
                //     }
                //     index++;
                // }
                 while (index<8){
                    //  build the posting string to send to ThingSpeak.
                    dataString+="&field" + String( index+1 ) + "="+String( data [ index ] );
                    index++;
                }
                Serial.println( dataString );
                // Create a topic string and publish data to ThingSpeak channel feed.
                String topicString ="channels/" + channel + "/publish";
                mqttClient.publish( topicString.c_str(), dataString.c_str() );
                //Serial.println( "Published to channel " +channel);
                delay(1000);
            }
            mqttClient.disconnect();
            client.stop();
        }
        modem.gprsDisconnect();
    }
}
void bulkUpdate(String fileName,long channel)
{
    if(!modem.isNetworkConnected())
    {
        modem.waitForNetwork(20000L,true);
    }
    if(modem.isNetworkConnected())
    {
        if(!modem.isGprsConnected())
        {
            modem.gprsConnect(apn);
        }
        if(modem.isGprsConnected())
        {
            //magic happens here
            TinyGsmClient client(modem);
            HttpClient http(client,ThingspeakServer,80);
            String contentType = "application/json";
            http.connectionKeepAlive();
            uint8_t err=http.post("/",contentType,"channels/"+ String(channel)+"/bulk_update.csv");
            if(err==0&&http.responseStatusCode()==200)
            {
                //heres where the magic happens
                //make a bulk update to thingspaeka channel and upload data.
            }
            else if(err!=0)
            {
                Serial.println("failed to connect to server");
                delay(2000);
            }
            else if(http.responseStatusCode()!=200)
            {
                Serial.print(F("response status code:"));
                Serial.println(http.responseStatusCode());
                Serial.println(F("check your connection parameters please"));
            }
            client.stop();
        }
    }
}
void FirebaseUpload(String data)
{
  if(!modem.isNetworkConnected())
    {
        modem.waitForNetwork(20000L,true);
    }
    if(modem.isNetworkConnected())
    {
        if(!modem.isGprsConnected())
        {
            modem.gprsConnect(apn);
        }
        if(modem.isGprsConnected())
        {
            //magic happens here
            TinyGsmClientSecure client(modem);
            HttpClient http(client,FirebaseHost,443);
            http.connect(FirebaseHost,443);
            String contentType = "application/json";
            if(http.connected())
            {
                /*open the sdcard and read data line by line till  empty*/
                // String url;
                http.connectionKeepAlive();
                // Serial.println("connected");
                // if (path[0] != '/') {
                //     url = "/";
                // }
                // url += path + ".json";
                // url += "?auth=" + FirebaseAuth;
                // Serial.print("POST:");
                // Serial.println(url);
                // Serial.print("Data:");
                // Serial.println(data);
                //http.put(url,contentType,data);

                http.post("/"+DEVICE_NAME+".json?auth="+FirebaseAuth+"print=silent","application/json",data); //create a new entery to the 
                Serial.println(http.responseStatusCode());
                Serial.println(http.responseBody());
            }
            http.endRequest();
            http.stop();

        }
        modem.gprsDisconnect();
    } 
}
// /.json?auth