#include <Arduino.h>
#include <PMS.h>
// put function declarations here:
PMS::DATA data;
#define SENSORSWITCH 56
double Sensor1averagePm25=0,Sensor1averagePm10=0;
double Sensor2averagePm25=0,Sensor2averagePm10=0;
void samplePmSensor(PMS sensor1,PMS sensor2,uint8_t sampleCount)
{
  digitalWrite(SENSORSWITCH,LOW); //turn the sensors on
  sensor1.wakeUp();
  sensor2.wakeUp();
  sensor2.requestRead();
  sensor1.requestRead();
  uint8_t failReadCount=0; //track the number of failed sensor communcaitions
  for (uint8_t samples=0;samples<=sampleCount;samples++)
  {
    if(sensor1.readUntil(data))
    {
      Sensor1averagePm25+=double(data.PM_AE_UG_2_5);
      Sensor1averagePm10+=double(data.PM_AE_UG_10_0);
      //debug messages here
    }
    else
    {
      samples=samples-1;
      failReadCount++;
      if(failReadCount>=5)
      {
        samples=sampleCount; //exit the loop 
      }
    }
    if(sensor2.readUntil(data))
    {
      Sensor2averagePm25+=double(data.PM_AE_UG_2_5);
      Sensor2averagePm10+=double(data.PM_AE_UG_10_0);
      //debug messages here
    }
    else
    {
      samples=samples-1;
      failReadCount++;
      if(failReadCount>=5)
      {
        samples=sampleCount; 
      }
    }
  }

}
void setup() {
  // put your setup code here, to run once:
}

void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here: