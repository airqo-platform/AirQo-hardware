#include "battery.h"
#include "configuration.h"
float batteryVoltage=0.00;
float measureBattery(uint8_t batteryTerminal)
{
    float batteryRaw=0.00;
    for(uint8_t sample=0;sample<10;sample++)
    {
        batteryRaw+=analogRead(batteryTerminal)*(5.00/1023.00);
        delay(100);
    }
    batteryVoltage=batteryRaw/10;
    dataToPublish[6]=batteryVoltage; //append datato the thingspeak 
    return(batteryVoltage);
}
void beep(uint8_t beeps)
{
    pinMode(22,OUTPUT);
    for(uint8_t i=0;i<beeps;i++)
    {
        digitalWrite(22,HIGH);
        delay(250);
        digitalWrite(22,LOW);
        delay(250);
    }
}