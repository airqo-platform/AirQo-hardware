#include "sd-rountines.h"
#include "configuration.h"
uint8_t failCount=0;
bool initialize_SD(uint8_t ss_pin)
{
    /*
    attempt to initialize the sdcard on chip ss specified in h file.
    failed attempts are repeated thrice.
    */
    if(!SD.begin(ss_pin))
    {
        for(failCount=0;failCount<4;failCount++)
        {
            SD.begin(ss_pin);
            delay(100);
        }
        return false;
    }
    else
        return true;
}
void deleteFile(String fileName)
{
    if(SD.exists(fileName))
    {
        SD.remove(fileName);
    }
}
