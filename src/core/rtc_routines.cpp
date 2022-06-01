#include "rtc_routines.h"
#include "configuration.h"
#include "ota.h"
RTC_DS3231 rtc;
void init_RTC()
{
    if(!rtc.begin())
    {
        Serial.println(F("couldnt find RTC"));
    }
    if(rtc.lostPower())
    {
        Serial.println(F("rtc lost power, adjusting time using gsm time"));
        rtc.adjust(DateTime(year,month,day,hour,min,sec));
    }
    rtc.adjust(DateTime(year,month,day,hour,min,sec));
}
String obtainTime()
{
  String timeStamp;
  DateTime now = rtc.now();
  //formating rtc output to required time formart for thingspeak bulk update example 2018-06-14T12:12:22-0500.
  timeStamp=String(now.year())+"-"+String(now.month())+"-"+String(now.day())+"T"+String(now.hour())+":"+String(now.minute())+":"+String(now.second())+"-"+String(timezone);
  return(timeStamp);
}