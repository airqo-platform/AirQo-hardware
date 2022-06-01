#include "eeprom_routines.h"
#include "configuration.h"
uint8_t DEVICE_NAME_ADDRESS_OFFSET=10;
uint8_t WRITE_KEY_ADDRESS_OFFSET=25;
uint8_t CHANNEL_ADDRESS_OFFSET=45;
uint8_t CONTROL_CAHNNELID_ADDRESS_OFFSET=60;
uint8_t UPDATE_ID_ADDRESSS_OFFSET=0;
uint8_t FIRMWARE_VERSION_ADDRESS_OFFSET=5;
int writeStringToEEPROM(int addrOffset, const String &strToWrite)
{
  byte len = strToWrite.length();
  EEPROM.update(addrOffset, len);

  for (int i = 0; i < len; i++)
  {
    EEPROM.update(addrOffset + 1 + i, strToWrite[i]);
  }

  return addrOffset + 1 + len;
}
int readStringFromEEPROM(int addrOffset, String *strToRead)
{
  int newStrLen = EEPROM.read(addrOffset);
  char data[newStrLen + 1];

  for (int i = 0; i < newStrLen; i++)
  {
    data[i] = EEPROM.read(addrOffset + 1 + i);
  }
  data[newStrLen] = '\0'; 

  *strToRead = String(data);
  return addrOffset + 1 + newStrLen;
}
void clearEEPROM()
{
  uint32_t stat=millis();
  for (uint16_t i = 0 ; i < EEPROM.length() ; i++) {
    if(millis()-stat>=500)
    {
      Serial.print(F("X"));
      stat=millis();
    }
    EEPROM.write(i, 0);
  }
  Serial.println(F("\n eeprom cleared, configuration reset"));
}
void updateReboot()
{
  //enable watchdog timer to reset the device 
  Serial.println(F("rebooting device now"));
  wdt_enable(WDTO_500MS);
  EEPROM.write(0x1FF,0xF0); //set the eeprom address that the bootlader will check for during flash writting.
  wdt_reset();
  delay(600);
}
void checkForConfigurationInput()
{
  readStringFromEEPROM(DEVICE_NAME_ADDRESS_OFFSET,&DEVICE_NAME);
  readStringFromEEPROM(WRITE_KEY_ADDRESS_OFFSET,&WRITE_KEY);
  readStringFromEEPROM(CONTROL_CAHNNELID_ADDRESS_OFFSET,&CTRL_CHANNELID);
  readStringFromEEPROM(CHANNEL_ADDRESS_OFFSET,&CHANNELID);
  readStringFromEEPROM(FIRMWARE_VERSION_ADDRESS_OFFSET,&FIRMWARE_VERSION);
  bool configFlag=false;
  Serial.println("******************************"+DEVICE_NAME+"..." +FIRMWARE_NAME+"_V"+FIRMWARE_VERSION+"****************************");
  Serial.println(F("##################### PRESS C TO CONFIGURE DEVICE OR X TO EXIT TO NORMAL OPERATION #######################"));
  Serial.print(F("Waiting."));
  for(uint8_t wait=0;wait<15;wait++)
    {
      char c;
      if(Serial.available())
      {
        c=Serial.read();
      }
      if(c=='C'||c=='c')
      {
        configFlag=true;
        break;
      }
      else if(c=='X'||c=='x')
      {
        Serial.println(F("DEVICE WILL CONTINUE OPERATION WITH PREVIOUS VALUES STORED IN EEPROM"));
        break;
      }
      Serial.print(F("."));
      delay(500);
    }
  Serial.println(F("DEVICE WILL CONTNUE OPERATION WITH PREVIOUS VALUES STORED IN EEPROM"));
  if(configFlag)
  {
    Serial.println(F("                      Configuration mode entered for next 30 seconds"));
    printMenu();
    uint32_t configStart=millis();
    uint8_t configTimeout=60;
    while (1)
    {
      if(Serial.available())
      {
        String config=Serial.readString();
        config.toLowerCase();
        if(config.startsWith("name"))
        {
          if(config.equals("name"))
          {
            //read eeprom name and print to screen.
            //Serial.println(DEVICE_NAME);
            readStringFromEEPROM(DEVICE_NAME_ADDRESS_OFFSET,&DEVICE_NAME);
            Serial.println(DEVICE_NAME);
          }
          else
          {
            DEVICE_NAME=config.substring(config.indexOf(':')+1);
            if(DEVICE_NAME.length()<10)
            {
              Serial.println(F("device name configured"));
              writeStringToEEPROM(DEVICE_NAME_ADDRESS_OFFSET,config.substring(config.indexOf(':')+1));
              // Serial.println(F("entred name is too long."));
            }
            else{
              Serial.println(F("invalid device name, device name should be <10 characters"));
            }
            //we can add a check for invalid device names here
            //writeStringToEEPROM(DEVICE_NAME_ADDRESS_OFFSET,config.substring(config.indexOf(':')+1));
            //store data in eeprom
          }
          
        }
        else if(config.startsWith("key"))
        {
          if(config.equalsIgnoreCase("key"))
          {
            //read eeprom key and print to screen.
            readStringFromEEPROM(WRITE_KEY_ADDRESS_OFFSET,&WRITE_KEY);
            WRITE_KEY.toUpperCase();
            Serial.println(WRITE_KEY);
          }
          else
          {
            Serial.println(F("device write key configured"));
            writeStringToEEPROM(WRITE_KEY_ADDRESS_OFFSET,config.substring(config.indexOf(':')+1));
            //store data in eeprom
          }
        }
        else if(config.startsWith("channel"))
        {
          if(config.equalsIgnoreCase("channel"))
          {
            //read eeprom channel and print to screeen
            readStringFromEEPROM(CHANNEL_ADDRESS_OFFSET,&CHANNELID);
            Serial.println(CHANNELID);
          }
          else
          {
            Serial.println(F("channel id configured"));
            writeStringToEEPROM(CHANNEL_ADDRESS_OFFSET,config.substring(config.indexOf(':')+1));
            //store data in eeprom captured data in eeprom
          }

        }
        else if(config.startsWith("c-channel"))
        {
          if(config.equalsIgnoreCase("c-channel"))
          {
            readStringFromEEPROM(CONTROL_CAHNNELID_ADDRESS_OFFSET,&CTRL_CHANNELID);
            Serial.println(CTRL_CHANNELID);
          }
          else{
            Serial.println(F("control channel id configured"));
            writeStringToEEPROM(CONTROL_CAHNNELID_ADDRESS_OFFSET,config.substring(config.indexOf(':')+1));
          }
        }
        else if (config.startsWith("dev"))
        {
          String easterEgg="found me ;-) this firmware was written by the airqo team, initiated by engineer OKEDI DEOGRATIUS, hes cool.";
          for(uint8_t i=0;i<=easterEgg.length();i++)
          {
            Serial.print(easterEgg[i]);delay(50);
          }
          Serial.println(F("      "));
        }
        else if(config.startsWith("clear"))
        {
          //clear eeprom values.
          Serial.println(F("clearing eeprom data, please wait"));
          clearEEPROM();
          configStart=millis();
        }
        else if(config.startsWith("done")||config.startsWith("x"))
        {
          Serial.println(F("done congifuring device, exiting now"));
          break;
        }
        else if(config.startsWith("check")||config.startsWith("c"))
        {
          Serial.println(F("stored configurations are>>>"));
          Serial.print(F("DEVICE NAME     :"));Serial.println(DEVICE_NAME);
          Serial.print(F("CHANNEL_ID      :"));Serial.println(CHANNELID);
          Serial.print(F("CTRL_CHANNEL_ID :"));Serial.println(CTRL_CHANNELID);
          Serial.print(F("FIRMWARE VERSION:"));Serial.println(FIRMWARE_NAME+"_V"+FIRMWARE_VERSION);
          //read all eeprom addresses.
        }
        else{
          Serial.println(F("invalid input"));
          printMenu();
        }
      }
      if(millis()-configStart>=(configTimeout*1000))
      {
        //jump out of loop if timeout has expired, ensure no hanging 
        break;
      }
    }
  }
  Serial.println(F(""));
}
void printMenu()
{
  Serial.println(F(" "));
  //dispaly a help menu with directions.
  Serial.println(F("              |**********************************MENU*************************************|"));
  Serial.println(F("              |enter: NAME:        followed by device name limitted to 10 characters!!!!  |"));
  Serial.println(F("              |enter: KEY:         followed by device write key                           |"));
  Serial.println(F("              |enter: CHANNEL:     followed by device thingspeak update channel-id        |"));//required for mqtt updates and bulk updates.
  Serial.println(F("              |enter: C-CHANNEL:   followed by device control channel_id                  |"));
  Serial.println(F("              |enter: DONE or X:   when desired configurations are made or wait 30 seconds|"));
  Serial.println(F("              |enter: CHECK or C:  to check and retrieve configuratons                    |"));
  Serial.println(F("              |enter: CLEAR:       to clear configurations                                |"));
  Serial.println("                |*****************************"+DEVICE_NAME+"*******************************|");
}