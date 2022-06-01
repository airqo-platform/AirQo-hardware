#include "sampling_routine.h"
#include "sd-rountines.h"
#include "configuration.h"
#include "ota.h"
#include "rtc_routines.h"
#include "battery.h"
#define SEALEVELPRESSURE_HPA (1013.25)
const uint8_t SENSORSWITCH PROGMEM=56;
#define DHTTYPE DHT11
#define DHTPIN 57
PMS::DATA data;
File dataFile;
uint8_t bulk_samples=0;
float sensor1averagedPm1,sensor1averagedPm25,sensor1averagedPm10;
float sensor2averagedPm1,sensor2averagedPm25,sensor2averagedPm10;
void samplePmSensor(PMS sensor1,PMS sensor2,uint8_t sampleCount,bool pm1_too)
{
    digitalWrite(SENSORSWITCH,LOW);
    sensor1.wakeUp();
    sensor2.wakeUp();
    sensor1.requestRead();
    sensor2.requestRead();
    uint8_t failcount=0;
    for(uint8_t sample=0;sample<sampleCount;sample++)
    {

        
        Serial.print(F("        pm sample:"));Serial.println(sample);
        if(sensor1.readUntil(data))
        {
            sensor1averagedPm25+=float(data.PM_AE_UG_2_5);
            sensor1averagedPm10+=float(data.PM_AE_UG_10_0);
            if(pm1_too){sensor1averagedPm1+=float(data.PM_AE_UG_1_0);Serial.print(F("sensor1 PM1.0 (ug/m3):"));Serial.println(sensor1averagedPm1/sample);}
            Serial.print(F("sensor1 PM2.5 (ug/m3):"));
            Serial.println(sensor1averagedPm25/sample);
            Serial.print(F("sensor1 PM10.0 (ug/m3):"));
            Serial.println(sensor1averagedPm10/sample);
            Serial.println(F(" "));
        }
        else{
            sample=sample-1;
            Serial.println(F("failed to read sensor"));
            failcount++;
            if(failcount>=5)
            {
                sample=60;
            }
        }
        if(sensor2.readUntil(data))
        {
            sensor2averagedPm25+=float(data.PM_AE_UG_2_5);
            sensor2averagedPm10+=float(data.PM_AE_UG_10_0);
            if(pm1_too){sensor2averagedPm1+=float(data.PM_AE_UG_1_0);Serial.print(F("sensor2 PM1.0 (ug/m3):"));Serial.println(sensor2averagedPm1/sample);}
            Serial.print(F("sensor2 PM2.5 (ug/m3):"));
            Serial.println(sensor2averagedPm25/sample);
            Serial.print(F("sensor2 PM10.0 (ug/m3):"));
            Serial.println(sensor2averagedPm10/sample);
            Serial.println(F(" "));
        }
        else{
            sample=sample-1;
            Serial.println(F("failed to read sensor"));
            failcount++;
            if(failcount>=5)
            {
                sample=60;
            }
        }
    }
    dataToPublish[0]=sensor1averagedPm25/sampleCount;
    dataToPublish[1]=sensor1averagedPm10/sampleCount;
    dataToPublish[2]=sensor2averagedPm25/sampleCount;
    dataToPublish[3]=sensor2averagedPm10/sampleCount;
    dataToPublish[6]=measureBattery(A1);
    dataToPublish[7]=FIRMWARE_VERSION.toFloat();
    Serial.print(F("bulk log number: "));
    Serial.println(bulk_samples);
    StaticJsonDocument<200> doc;
    doc["created_at"]=getGsmTimeStamp();
    for(uint8_t i=0;i<8;i++)
    {
        doc["field"+String(i+1)]=dataToPublish[i];
    }
    dataFile=SD.open("Log.txt",FILE_WRITE);
    if(!serializeJson(doc,dataFile)){
        Serial.println(F("failed towrite to file"));
    }
    if(bulk_samples>10)
    {
        Serial.println(F("reading data file on sd card"));
        dataFile=SD.open("log.txt",FILE_READ);
        if(dataFile)
        {
            while(dataFile.available())
            {
            Serial.print((char)dataFile.read());
            }
        }
        bulk_samples=0; //reset the bulk countter.
    }
    else
        dataFile.print(',');
    dataFile.close();
    bulk_samples++;
    //serializeJsonPretty(doc,Serial);
    sensor1averagedPm1=sensor2averagedPm1=sensor1averagedPm10=sensor1averagedPm25=sensor2averagedPm10=sensor2averagedPm25=0.00;
}
void sampleDHT(uint8_t sampleCount)
{
    DHT dht(DHTPIN, DHTTYPE);
    pinMode(62,OUTPUT);
    digitalWrite(62,HIGH);
    dht.begin();
    for(uint8_t sample=0;sample<sampleCount;sample++)
    {
        float h = dht.readHumidity();
        float t = dht.readTemperature();
        if (isnan(h) || isnan(t)) {
            Serial.println(F("Failed to read from DHT sensor!"));
            return;
        }
        Serial.print(F("Humidity: "));
        Serial.print(h);
        Serial.print(F("%  Temperature: "));
        Serial.print(t);
        Serial.println(F("°C "));
    }
    digitalWrite(62,LOW);
}
void sampleSHT(uint8_t sampleCount)
{
    pinMode(62,OUTPUT);
    digitalWrite(62,HIGH);
    SHT2x sht;
    sht.begin();
    if(sht.isConnected())
    {
        Serial.println(sht.getTemperature(), 1);
        Serial.println(sht.getHumidity(), 1);
    }
}
void sampleBME(uint8_t sampleCount)
{
    pinMode(62,OUTPUT);
    digitalWrite(62,HIGH);
    Adafruit_BME280 bme;
    uint8_t status=bme.begin();
    if(status)
    {
        Serial.print("Temperature = ");
        Serial.print(bme.readTemperature());
        Serial.println(" °C");

        Serial.print("Pressure = ");

        Serial.print(bme.readPressure() / 100.0F);
        Serial.println(" hPa");

        Serial.print("Approx. Altitude = ");
        Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
        Serial.println(" m");

        Serial.print("Humidity = ");
        Serial.print(bme.readHumidity());
        Serial.println(" %");

        Serial.println();
    }
}