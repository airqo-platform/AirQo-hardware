// Updates the ThingSpeakchannel with data
//******************************************Comms bulk***********************************************************************************************
void commsBulk(char* jsonBuffer) {
 
    
  TinyGsmClient client(modem, 4);
  const int     port = 80;
  DBG("Connecting to", thingSpeakAddress);
  if (!client.connect(thingSpeakAddress, port)) {
    DBG("... failed");
    if(EnableAlarms){
      noTone(22);  tone(22, 4000, 100);delay(125);noTone(22); // tone(22, 4000, 100);
    }
    
    
    failtimes = failtimes + 1 ;
    if(DebugEnable){Serial.println(F("Uploading failed"));
    Serial.print(F("Fail times now is; "));
    Serial.println(failtimes);}
    if (failtimes == 5)
    {
      if(DebugEnable){Serial.println(F("System will restart the GSM and GPRS"));}
      failtimes = 0;
      GsmPowerDown(10000);
      asm volatile(" jmp 0");
    }
  } else {
  
  failtimes = 0;
  client.stop();
  String data_length = String(strlen(jsonBuffer)+1); //Compute the data buffer length
   if(DebugEnable){Serial.println(jsonBuffer);}
  // POST data to ThingSpeak
  if (client.connect(server, 80)) {
    client.println("POST /channels/"+String(channelId)+"/bulk_update.json HTTP/1.1"); // Replace YOUR-CHANNEL-ID with your ThingSpeak channel ID
    client.println("Host: api.thingspeak.com");
    client.println("User-Agent: mw.doc.bulk-update (Arduino ESP8266)");
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.println("Content-Length: "+data_length);
    client.println();
    client.println(jsonBuffer);
  }
  else {
    if(DebugEnable){Serial.println(F("Failure: Failed to connect to ThingSpeak"));}
  }
  delay(250); //Wait to receive the response
  client.parseFloat();
  String resp = String(client.parseInt());
   if(DebugEnable){Serial.print(F("Response code:"));Serial.println(resp);} // Print the response code. 202 indicates that the server has accepted the response
  jsonBuffer[0] = '['; //Reinitialize the jsonBuffer for next batch of data
  jsonBuffer[1] = '\0';

  }
   modem.gprsDisconnect();
        delay(5000L);
        if (!modem.isGprsConnected()) {
           if(DebugEnable){Serial.println(F("GPRS disconnected"));}
        } else {
           if(DebugEnable){Serial.println(F("GPRS disconnect: Failed."));};
        }
}

//***************************************************************************************************************************************************





void comms(double Sensor1PM25Mean, double Sensor1PM10Mean, double Sensor2PM25Mean, double Sensor2PM10Mean, char* batValue){
  
DBG("Commmmmmmmmmmmmmmmmmmmmmmmmmmm");
//delay(5000);
// ThingSpeak Post. Single entry

  char itoaBuffer[8];


//  char toPostSensor1PM25Mean[20]; char toPostSensor1PM10Mean[20]; char toPostSensor2PM25Mean[20]; char toPostSensor2PM10Mean[20];
//  dtostrf((Sensor1PM25Mean), 6, 2, toPostSensor1PM25Mean); 
//  dtostrf((Sensor1PM10Mean), 6, 2, toPostSensor1PM10Mean); 
//  dtostrf((Sensor2PM25Mean), 6, 2, toPostSensor2PM25Mean); 
//  dtostrf((Sensor2PM10Mean), 6, 2, toPostSensor2PM10Mean);

  
  TinyGsmClient client(modem, 4);
  const int     port = 80;
  DBG("Connecting to", thingSpeakAddress);
  if (!client.connect(thingSpeakAddress, port)) {
    DBG("... failed");
    
    if(EnableAlarms){
      noTone(22);  tone(22, 4000, 100);delay(125);noTone(22); // tone(22, 4000, 100);
    }
    failtimes = failtimes + 1 ;
     if(DebugEnable){
    Serial.println(F("Uploading failed"));    
    Serial.print(F("Fail times now is; "));}
    Serial.println(failtimes);
    if (failtimes == 2)
    {
      if(DebugEnable){
         if(DebugEnable){Serial.println(F("System will restart the GSM and GPRS"));}
      }
      failtimes = 0;
      GsmPowerDown(10000);
      modemInfo();
    }
    
    if (failtimes == 5)
    {
      if(DebugEnable){ Serial.println(F("System will restart the GSM and GPRS"));}
      failtimes = 0;
      GsmPowerDown(10000);
      asm volatile(" jmp 0");
    }
  } else {
     failtimes = 0;
     if(DebugEnable){Serial.println(F("HTTP GET Request"));}
   // client.print(String("GET ") + resource + " HTTP/1.0\r\n");
   // client.print(String("Host: ") + thingSpeakAddress + "\r\n");
    //client.print("Connection: close\r\n\r\n");

  if(DebugEnable){Serial.println(F("connected to thingspeak"));}

    client.print("POST /update HTTP/1.1\r\n");
    client.print("Host: api.thingspeak.com\r\n");
    client.print("Connection: close\r\n");
    client.print("X-THINGSPEAKAPIKEY: ");
    client.print(writeAPIKey);
    //client.print("3D65XPOPLCI1IW20");
    
    client.print("\r\n");
    client.print("Content-Type: application/x-www-form-urlencoded\r\n");
    client.print("Content-Length: ");    
    String str="GET https://api.thingspeak.com/update?api_key="+String(writeAPIKey)+"&field1=" + String(Sensor1PM25Mean)+"&field2="+ String(Sensor1PM10Mean)+
    "&field3="+ String(Sensor2PM25Mean)+"&field4="+ String(Sensor2PM10Mean)+"&field5="+ String(lat2)+"&field6="+ String(lon2)+"&field7="+ String(batValue)+"&field8="+ String(toPostGPSData);
   
    //Serial.println(str);
    
   // sprintf(sentMsg,str);
    itoa(strlen(str.c_str()), itoaBuffer, 10);
     if(DebugEnable){Serial.println(itoaBuffer);}

    client.print(itoaBuffer);

    client.print("\r\n\r\n");

    client.print(str.c_str());

    client.print("\r\n\r\n");

    delay(200);
    //gsm.SimpleWrite("AT+CIPCLOSE");

    Serial.println(str.c_str());

//   delay(5000);
   
//client.print(str);
delay(2000);

ShowSerialData();
 
  client.print((char)26);//sending
  delay(1000);//waitting for reply, important! the time is base on the condition of internet 
  client.print("");
   ShowSerialData();
    client.stop();
  }
  DBG(F("End of tests."));
delay(1500);
  }



void ShowSerialData()
{
  while(Serial1.available()!=0)
  Serial.write(Serial1.read());
  delay(1000);   
}


/////**************************************Bulk trans******************************************************************************************************************
void bulkUpdatemore() {
   
     if(modem.isNetworkConnected())
    {
    TinyGsmClient  client(modem);
    //const int      cport = 80;//443
    //End of Client start
    if (client.connect(server, port)) {
    failtimes = 0;
    //Serial.println(String(channelId));
    client.println("POST /channels/" + String(channelId)+"/bulk_update.json HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("User-Agent: Arduino MKRGSM");
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.print(strlen(jsonBuffer));
    //Serial.println((jsonBuffer));   
    client.print("\r\n\r\n");   
    client.print(jsonBuffer);
    
    delay(250);
    String rs;// = "Making bulkUpdates: ";
    client.parseFloat();
    rs += String(client.parseInt());
     if(DebugEnable){Serial.print(F("Making BulkUpdates: ")); Serial.println(rs);}
    } else {
   if(EnableAlarms){
      noTone(22);  tone(22, 4000, 100);delay(125);noTone(22); // tone(22, 4000, 100);
    }

    failtimes = failtimes + 1 ;
    if(DebugEnable){Serial.println(F("Uploading failed"));
    Serial.print(F("Fail times now is; "));
    Serial.println(failtimes);}
    if (failtimes == 2)
    {
      if(DebugEnable){Serial.println(F("System will restart the GSM and GPRS"));}
      failtimes = 0;
      GsmPowerDown(10000);
      asm volatile(" jmp 0");
    }
    Serial.println();
    }}else{
   if(EnableAlarms){
      noTone(22);  tone(22, 4000, 100);delay(125);noTone(22); // tone(22, 4000, 100);
    }

    failtimes = failtimes + 1 ;
    if(DebugEnable){Serial.println(F("Uploading failed"));
    Serial.print(F("Fail times now is; "));
    Serial.println(failtimes);}
    if (failtimes == 2)
    {
      if(DebugEnable){Serial.println(F("System will restart the GSM and GPRS"));}
      failtimes = 0;
      GsmPowerDown(10000);
      asm volatile(" jmp 0");
    }
    Serial.println();
    }
}




void bulkUpdateSDContent() {

//Serial.println("Channel ID: "+ String(channelId)+" Write Key: "+ String(writeAPIKey));
  char buff[500];         
  enableSDAlone();
  String connStart = "{\"write_api_key\":\"" + String(writeAPIKey) +"\",\"updates\":[";
  //Serial.println("constatrt length: "+ String(strlen(connStart.c_str())));
   modem.gprsConnect(apn, gprsUser, gprsPass);
   delay(1000);
    //client.stop();
      //Client start
      TinyGsmClient  client(modem);
      //const int      port = 80;//443
      //End of Client start
    if (client.connect(server, port)) {

      File dataFile = SD.open(fileName);
  if (dataFile) {
      } else {
    Serial.println(F("Error Opening file"));
    return;
  }
  
    
    client.println("POST /channels/" + String(channelId)+"/bulk_update.json HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("User-Agent: Arduino MKRGSM");
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.print((dataFile.size() + strlen(connStart.c_str())));
    //if(DebugEnable){Serial.print(strlen(jsonBuffer));}   
    client.print("\r\n\r\n");   
    client.print(connStart);

     if (dataFile) {
    int i = 0;
    while (dataFile.available() > 0) {
      buff[i] = dataFile.read();     //BINg
      i++;
      if (i == 500) {
        client.write(buff, i);  //AT + CIPSEND
        
        i = 0;
        memset(buff, 0, sizeof(buff));
      }
    }
    client.write(buff, i);
    memset(buff, 0, sizeof(buff));
    dataFile.close();
  }
    delay(1000);
    String rs;// = "Making bulkUpdates: ";
    client.parseFloat();
    rs += String(client.parseInt());
    if(DebugEnable){Serial.print(F("Making bulkUpdates: "));Serial.println(rs);}
    ShowSerialData();
    failtimes = 0;
    } else {
        
    if(EnableAlarms){
      noTone(22);  tone(22, 4000, 100);delay(125);noTone(22); // tone(22, 4000, 100);
    }
    failtimes = failtimes + 1 ;
    if(DebugEnable){Serial.println(F("Uploading failed"));
        Serial.print(F("Fail times now is; "));
        Serial.println(failtimes);}
    if (failtimes == 5)
    {
     if(DebugEnable){ Serial.println(F("System will restart the GSM and GPRS"));}
      failtimes = 0;
      GsmPowerDown(10000);
       asm volatile(" jmp 0");
      
    }
   
    
    }

    readLocation();
    modem.gprsDisconnect();
        delay(5000L);
        if (!modem.isGprsConnected()) {
          DBG(F("GPRS disconnected"));
        } else {
          DBG(F("GPRS disconnect: Failed."));
        }
delay(2000);
       
}


//**************no loss methods****************************************

void bulkUpdateSDContentNoLoss() {
            
  enableSDAlone();
  char buff[500];
  String connStart = "{\"write_api_key\":\"" + String(writeAPIKey) +"\",\"updates\":[";
  String connEnd = "]}";
  //Serial.println("constatrt length: "+ String(strlen(connStart.c_str())));
  
  // modem.gprsConnect(apn, gprsUser, gprsPass);
  // delay(1000);
  
  //  client.stop();
  
  //Client start
  TinyGsmClient  client(modem);
  //const int      port = 80;//443
  //End of Client start
    
     if(modem.isNetworkConnected()){
    
    if (client.connect(server, port)) {

      File dataFile = SD.open(fileName);
  if (dataFile) {
      } else {
    Serial.println(F("Error Opening file"));
    return;
  }
  
    
    client.println("POST /channels/" + String(channelId)+"/bulk_update.json HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("User-Agent: Arduino MKRGSM");
    client.println("Connection: close");
    client.println("Content-Type: application/json");
    client.print("Content-Length: ");
    client.print((dataFile.size() + strlen(connStart.c_str()) + strlen(connEnd.c_str())));
    //if(DebugEnable){Serial.print(strlen(jsonBuffer));}   
    client.print("\r\n\r\n");   
    client.print(connStart);
    

     if (dataFile) {
    int i = 0;
    while (dataFile.available() > 0) {
      buff[i] = dataFile.read();     //BINg
      i++;
      if (i == 500) {
        client.write(buff, i);  //AT + CIPSEND
        i = 0;
        memset(buff, 0, sizeof(buff));
      }
    }
    client.write(buff, i);
    memset(buff, 0, sizeof(buff));
    dataFile.close();
  }
    client.print(connEnd);
    delay(1000);
    String rs = "Making bulkUpdates: ";
    client.parseFloat();
    rs += String(client.parseInt());
    Serial.println(rs);
    ShowSerialData();
    failtimes = 0;
    } else {
        

    if(EnableAlarms){
      noTone(22);  tone(22, 4000, 100);delay(125);noTone(22); // tone(22, 4000, 100);
    }
    
    failtimes = failtimes + 1 ;
    if(DebugEnable){Serial.println(F("Uploading failed"));
    Serial.print(F("Fail times now is; "));
    Serial.println(failtimes);}
    if (failtimes == 30)
    {
      if(DebugEnable){Serial.println(F("System will restart the GSM and GPRS"));}
      failtimes = 0;
      asm volatile(" jmp 0");
    }
   
    
    }
     }
    else {
        

    if(EnableAlarms){
      noTone(22);  tone(22, 4000, 100);delay(125);noTone(22); // tone(22, 4000, 100);
    }
    
    failtimes = failtimes + 1 ;
    if(DebugEnable){Serial.println(F("Uploading failed"));
    Serial.print(F("Fail times now is; "));
    Serial.println(failtimes);}
    if (failtimes == 30)
    {
      if(DebugEnable){Serial.println(F("System will restart the GSM and GPRS"));}
      failtimes = 0;
      asm volatile(" jmp 0");
    }
   
    
    }
  
       
}



//////*************************************communication****************************************

void readLocation(){
  #if defined TINY_GSM_MODEM_HAS_GPS
   if(DebugEnable){Serial.println(F("Enabling GPS/GNSS/GLONASS and waiting 15s for warm-up"));}
  modem.enableGPS();
  delay(15000L);

  for (int8_t i = 1; i; i--) {
     if(DebugEnable){Serial.println(F("Requesting current GPS/GNSS/GLONASS location"));}
    if (modem.getGPS(&lat2, &lon2, &speed2, &alt2, &vsat2, &usat2, &accuracy2,
                     &year2, &month2, &day2, &hour2, &min2, &sec2)) {
      DBG("Latitude:", String(lat2, 8), "\tLongitude:", String(lon2, 8));
      DBG("Speed:", speed2, "\tAltitude:", alt2);
      DBG("Visible Satellites:", vsat2, "\tUsed Satellites:", usat2);
      DBG("Accuracy:", accuracy2);
      DBG("Year:", year2, "\tMonth:", month2, "\tDay:", day2);
      DBG("Hour:", hour2, "\tMinute:", min2, "\tSecond:", sec2);
      break;
    } else {
       if(DebugEnable){Serial.println(F("Couldn't get GPS/GNSS/GLONASS location, retrying in 15s."));}
      delay(100L);
    }
  }
   if(DebugEnable){Serial.println(F("Retrieving GPS/GNSS/GLONASS location again as a string"));}
  String gps_raw = modem.getGPSraw();
  DBG("GPS/GNSS Based Location String:", gps_raw);
   if(DebugEnable){Serial.println(F("Disabling GPS"));};
  modem.disableGPS();
#endif

  }
///////*****************************************************************************************
