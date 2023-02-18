


///////////Json files************************************************************************





String obtainTime()
{

  DateTime now = rtc.now();
//
//    Serial.print(now.year(), DEC);
//    Serial.print('/');
//    Serial.print(now.month(), DEC);
//    Serial.print('/');
//    Serial.print(now.day(), DEC);
//    Serial.print(" (");
//    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
//    Serial.print(") ");
//    Serial.print(now.hour(), DEC);
//    Serial.print(':');
//    Serial.print(now.minute(), DEC);
//    Serial.print(':');
//    Serial.print(now.second(), DEC);
//    Serial.println();
  
  //DateTime now = rtc.now();
  //formating rtc output to required time formart for thingspeak bulk update.
 delay(10);
 //timezone = 3;
  return(String("\""+String(now.year())+"-"+String(now.month())+"-"+String(now.day())+" "+String(now.hour())+":"+String(now.minute())+":"+String(now.second())+" +"+timezone+ "\""));
  
}


String CSVName()
{
  DateTime now = rtc.now();
  delay(10);
  //return(String(String(now.year())+String(now.month())+String(now.day())));  


 String monthVal;
 String dayVal;
 
 if(int(now.month())<10){monthVal = "0"+String(now.month());}
 else{monthVal = String(now.month());}

  if(int(now.day())<10){dayVal = "0"+String(now.day());}
 else{dayVal = String(now.day());}
 
 return(String(String(now.year())+monthVal+dayVal));  
}

//
void scanI2c()
{
   digitalWrite(IandETempSwitch, HIGH);
   delay(500);
  int nDevices=0;
  Serial.println(F("Scanning..."));
  for (byte address = 1; address < 127; ++address) {
    Wire.beginTransmission(address);
    byte error = Wire.endTransmission();
    if (error == 0) {
      Serial.print(F("I2C device found at address 0x"));
      if (address < 16) {
        Serial.print("0");
      }
      Serial.print(address, HEX);
      Serial.println("  !");
      ++nDevices;
    } else if (error == 4) {
      Serial.print(F("Unknown error at address 0x"));
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }digitalWrite(IandETempSwitch, LOW);
}




void setRTC(){
   if (! rtc.begin()) {
  Serial.println(F("Couldn't find RTC"));
  //Serial.flush();
  //while (1) delay(10);
  }
  if (rtc.lostPower()) {
    Serial.println(F("RTC lost power, let's set the time!"));
    //pick time from gsm
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
     rtc.adjust(DateTime(year3, month3, day3, hour3, min3, sec3));
   
  }
  }




//*************joo


//** Updates the josnBuffer with data






void updatesJson2(char* jsonBuffer, char F1[4], char F2[4], char F3[4], char F4[4], char F5[4], char F6[4], char F7[4], char F8[4]){

  obtainTime();
  
  if(jsonStart == true){
    jsonStart = false;
    //strcat(jsonBuffer,"{\"write_api_key\":\"YOUR-CHANNEL-WRITEAPIKEY\",\"updates\":[");
    strcat(jsonBuffer,"{\"write_api_key\":\""); strcat(jsonBuffer,writeAPIKey); strcat(jsonBuffer,"\",\"updates\":[");   
        }

   Serial.println("Write Count:" +String(countCon));
   if(countCon == (bulkMax - 1)){
                   digitalWrite(CommsSwitch, HIGH);                
                  }
  
  strcat(jsonBuffer,"{\"created_at\":");
  strcat(jsonBuffer,obtainTime().c_str());
  //strcat(jsonBuffer,"2022-2-1 12:50:0 -3.00"); 
  strcat(jsonBuffer,",");
  
  
  strcat(jsonBuffer, "\"field1\":\"");
   strcat(jsonBuffer,F1);
  strcat(jsonBuffer,"\",");
  
   strcat(jsonBuffer, "\"field2\":\"");
   strcat(jsonBuffer,F2);
  strcat(jsonBuffer,"\",");

   strcat(jsonBuffer, "\"field3\":\"");
   strcat(jsonBuffer,F3);
   strcat(jsonBuffer,"\",");

   strcat(jsonBuffer, "\"field4\":\"");
   strcat(jsonBuffer,F4);
   strcat(jsonBuffer,"\",");

   strcat(jsonBuffer, "\"field5\":\"");
   strcat(jsonBuffer,F5);
   strcat(jsonBuffer,"\",");

   strcat(jsonBuffer, "\"field6\":\"");
   strcat(jsonBuffer,F6);
   strcat(jsonBuffer,"\",");

   strcat(jsonBuffer, "\"field7\":\"");
   strcat(jsonBuffer,F7);
   strcat(jsonBuffer,"\",");

   strcat(jsonBuffer, "\"field8\":\"");
   strcat(jsonBuffer,F8);
   strcat(jsonBuffer,"\"");
   strcat(jsonBuffer,"},");
   
  Serial.println(jsonBuffer);
  Serial.println(strlen(jsonBuffer));
  countCon++;

//   If posting interval time has reached 2 minutes, update the ThingSpeak channel with your data
 
 if(countCon == bulkMax){
      Serial.println(F(""));Serial.println(F(""));
     
      
      size_t len = strlen(jsonBuffer);
            jsonBuffer[len-1] = ']';
          //  httpRequest(jsonBuffer);
      strcat(jsonBuffer,"}");
      //Serial.println(jsonBuffer);
      
      Serial.println(strlen(jsonBuffer));
      Serial.println(F(""));Serial.println(F(""));

     // commsBulkfail();
     // httpRequestTesting(jsonBuffer);
     modemInfo(); 
     bulkUpdatemore();
      
      
     // jsonBuffer[0] = '['; //Reinitialize the jsonBuffer for next batch of data
      jsonStart = true;
      jsonBuffer[0] = '\0';
      countCon=0;
      readLocation();
      modemPowerOff();
      
            
      delay(400);
 
  }
  
    delay(400);
  
}
