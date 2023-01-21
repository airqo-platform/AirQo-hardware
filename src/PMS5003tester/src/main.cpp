/*
  this code tests the PMS5003 sensors on the airqo Gen5 devices.
  the code is setup to run on any revison of the airqo monitors which may have varrying hardware designs 

*/
#include <Arduino.h>
#include <PMS.h>
#include <ArduinoJson.h>
#define SENSORSWITCH 56 //sensors are switched by a P-channel mosfet controlled by IO56 on the atmega2560 MCU.
PMS::DATA data;
float sensor1Pm1,sensor1Pm25,sensor1Pm10;
float sensor2Pm1,sensor2Pm25,sensor2Pm10;
void SamplePmSensor(PMS sensor1,PMS sensor2,uint8_t sampleCount,bool pm1_too);// sample (pmsensor1,pm sensor1, sample count,1=include pm1 readings,0=excludepm1 readings)
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
}
void SamplePmSensor(PMS sensor1,PMS sensor2,uint8_t sampleCount,bool pm1_too)
{
  digitalWrite(SENSORSWITCH,LOW); // logic 0 turns the mosfet  on >> turnig the sensors on
  sensor1.wakeUp();
  sensor2.wakeUp();
  sensor1.requestRead();
  sensor2.requestRead();
  uint8_t failCount=0; //variable to count the number of times data was unsecefully read from the sensor
    for(uint8_t sample=0;sample<sampleCount;sample++)
    {        
        Serial.print(F("        pm sample:"));Serial.println(sample);
        if(sensor1.readUntil(data))
        {
            sensor1Pm25+=float(data.PM_AE_UG_2_5);
            sensor1Pm10+=float(data.PM_AE_UG_10_0);
            if(pm1_too){sensor1Pm1+=float(data.PM_AE_UG_1_0);Serial.print(F("sensor1 PM1.0 (ug/m3):"));Serial.println(sensor1Pm1/sample);}
            Serial.print(F("sensor1 PM2.5 (ug/m3):"));
            Serial.println(sensor1Pm25/sample);
            Serial.print(F("sensor1 PM10.0 (ug/m3):"));
            Serial.println(sensor1Pm10/sample);
            Serial.println(F(" "));
        }
        else{
            sample=sample-1;
            Serial.println(F("failed to read sensor"));
            failCount++;
            if(failCount>=5)
            {
                sample=sampleCount;
            }
        }
        if(sensor2.readUntil(data))
        {
            sensor2Pm25+=float(data.PM_AE_UG_2_5);
            sensor2Pm10+=float(data.PM_AE_UG_10_0);
            if(pm1_too){sensor2Pm1+=float(data.PM_AE_UG_1_0);Serial.print(F("sensor2 PM1.0 (ug/m3):"));Serial.println(sensor2Pm1/sample);}
            Serial.print(F("sensor2 PM2.5 (ug/m3):"));
            Serial.println(sensor2Pm25/sample);
            Serial.print(F("sensor2 PM10.0 (ug/m3):"));
            Serial.println(sensor2Pm10/sample);
            Serial.println(F(" "));
        }
        else{
            sample=sample-1;
            Serial.println(F("failed to read sensor"));
            failCount++;
            if(failCount>=5)
            {
                sample=sampleCount;
            }
        }
    }
    StaticJsonDocument<200> PmSensorData;
    PmSensorData["time"]="YR/DD/MM/HR/MIN/SEC -ZONE";
    PmSensorData["s1_pm2_5"]=sensor1Pm25/sampleCount;
    PmSensorData["s1_pm10"]=sensor1Pm10/sampleCount;
    PmSensorData["s2_pm2_5"]=sensor2Pm25/sampleCount;
    PmSensorData["s2_pm10"]=sensor2Pm10/sampleCount;
    serializeJsonPretty(PmSensorData,Serial);
}


// could we explore the windowing function for this particular type code structure: get PM values with in 60 seconds: then carry out a cumulative 
// averaging 
