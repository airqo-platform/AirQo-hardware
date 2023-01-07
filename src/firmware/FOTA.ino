




const char ThingspeakServer[]="api.thingspeak.com";
const char FireBaseServer[]="firebasestorage.googleapis.com";

const int FireBasePort=443;
String resource="/v0/b/airqo-hardware-01.appspot.com/o/";



boolean FileGetmain(String fileName, String token){
  boolean ret = false;
  const int           sPort = 443;
  TinyGsmClientSecure client(modem);

     {
        //client.stop();
       // sclient.stop();
      if (!client.connect(FireBaseServer, sPort)) {
        if(DebugEnable){Serial.println(F("connection failed"));}
        ret = false;
       // return;
        }else{
        //  http.connectionKeepAlive();
          ret = true;
        if(DebugEnable){Serial.println(F("connected"));}

//         Serial.println("GET "+resource+fileName+"?alt=media&token="+token);
//        client.println("GET "+resource+fileName+"?alt=media&token="+token);
        if(DebugEnable){Serial.println("GET "+resource+fileName+"?alt=media");}
        client.println("GET "+resource+fileName+"?alt=media");
        client.println("Host: https://www.firebasestorage.googleapis.com");
        //sclient.println(String("Content-Type: ") + "applicaton/json" );
        client.println(String("Content-Type: ") + "text/plain");
        //sclient.println(String("Content-Disposition: ") + "attachment; filename="+fileName);
        client.println("Connection: close");
        client.println();

        while (client.available() == 0);  // wait till something comes in; you may want to add a timeout here

        unsigned long lastRead = millis();  // keeps track of last time a byte was read
        while (millis() - lastRead < 2000) 
        {  // while 2 secs havent passed since a byte was read
          while (client.available()) {
            if(DebugEnable){Serial.print((char)client.read());}
            //data +=((char)client.read());
           //  data +=client.readStringUntil('entry_id');
            lastRead = millis();   // update last time something was read
          }
        }
       
        }
    }
    client.stop();
    return ret;
    }



void gsm_send_serial(String command) {
  if(DebugEnable){Serial.println("Send ->: " + command);}
  Serial1.println(command);
  long wtimer = millis();
  while (wtimer + 1000 > millis()) {
    while (Serial1.available()) {
      if(DebugEnable){Serial.write(Serial1.read());}
    }
  }
  if(DebugEnable){Serial.println(F(" "));}
}



boolean GetFieldValue2(){
  
    String currentLine = "";  String neededString = "";
    boolean StartPoint = false;  boolean ret;  
    
    //Client start
//    client.stop();
     //Serial.println(ThingspeakServer);
    if(!modem.isGprsConnected())
    {
        modem.gprsConnect(apn);
    }
    if(modem.isGprsConnected())
    {
    if(DebugEnable){Serial.println(modem.getLocalIP());}
    TinyGsmClient  client(modem);
    //const int      cport = 80;//443
    //End of Client start  
    
      if (!client.connect(thingSpeakAddress, 80)) {
        if(DebugEnable){Serial.println(F("connection failed"));}
        ret = false;
        return;
        }else{
        if(client.available() == 0){
       if(DebugEnable){ Serial.println(F("connected"));} //gsm_send_serial("AT+HTTPSSL=0");
        client.println("GET /channels/"+ String(controlChannelID) +"/feeds/last.json?results=1&api_key="+ controlChannelReadKey +" HTTP/1.1");
        //client.println("GET /channels/1694247/feeds/last.json?results=1&amp;api_key=23BLMB7RX4E3KUZB HTTP/1.1");
        //client.println("GET /channels/1694247/feeds/last.json?results=1 HTTP/1.1");
        client.println("Host: api.thingspeak.com");
        client.println(String("Content-Type: ") + "applicaton/json" );
        client.println("Connection: keep-alive");
        
        client.println();
        
        while (client.available() == 0);  // wait till something comes in; you may want to add a timeout here
        unsigned long lastRead = millis();  // keeps track of last time a byte was read
        while (millis() - lastRead < 2000) {
        if (client.connected()) {
        while (client.available()) {
           char inChar = (char)client.read();
            if (inChar == -1){return i;}
     
           currentLine += inChar;
           if (inChar == '\n') {currentLine = "";}
           if (currentLine.endsWith("entry_id")) {StartPoint = true;} 
           else if (StartPoint) {
            if (!currentLine.endsWith("}")) { neededString += inChar;} 
            else {StartPoint = false;}
                                 }}}
                  client.stop();         
                  //Serial.println(neededString);
        
                  String formatedString = neededString.substring(4, neededString.length()-1);
                  //Serial.println(formatedString);
                 
                  String formatedString1 ="";
                  String formatedString2 ="";
                  String formatedString3 ="";
                  String formatedString4 ="";
                  String formatedString5 ="";
                  
                  formatedString1 = formatedString.substring(formatedString.indexOf("field1")+9, formatedString.indexOf("field2")-3);
                  formatedString2 = formatedString.substring(formatedString.indexOf("field2")+9, formatedString.indexOf("field3")-3);
                  formatedString3 = formatedString.substring(formatedString.indexOf("field3")+9, formatedString.indexOf("field4")-3);
                  formatedString4 = formatedString.substring(formatedString.indexOf("field4")+9, formatedString.indexOf("field5")-3);
                  formatedString5 = formatedString.substring(formatedString.indexOf("field5")+9, formatedString.indexOf(formatedString.length()));

//                  Serial.println(formatedString1);
//                  Serial.println(formatedString2);
//                  Serial.println(formatedString3);
//                  Serial.println(formatedString4); 
//                  Serial.println(formatedString5);    

                  fetchedFirmawareVersion = formatedString1.toInt();
                  fetchedUpdateCtrl = formatedString2.toInt();
                  fetchedConfigFileToken = String(formatedString3);
                  fetchedFirmawareFileToken = String(formatedString4);
                  NoConfigLine = formatedString5.toInt();

                  ret = true;   
       
        }}

         //modem.gprsDisconnect();
        client.stop(); 
        //gsm_send_serial("AT+CIPSHUT");delay(2000);       
        }
        
        return ret;
        
      }
      }

void FOTASyncControl(){
  if(FOTACTR = false){
    FOTATimer = millis();
    FOTACTR = true;
      }
    else if(FOTACTR = true){
       FOTATimer = millis();
       if(FOTATimer>FOTAInterval){
        FOTACTR = false;
        FOTASync();
        FOTATimer = 0;
        }
      }
  }



void FOTASync(){
        //modemInfo();    
        //Serial.println("GETTING VALUE: "+ String(GetFieldValue2()));//+(1));
        if(GetFieldValue2()){
        if(DebugEnable){
        Serial.print(F("fetched Firmaware Version: ")); Serial.println((fetchedFirmawareVersion));
        Serial.println(F("fetched Update Ctrl: "));Serial.println((fetchedUpdateCtrl));
        Serial.println(F("fetched Config File Token: "));Serial.println((fetchedConfigFileToken));
        Serial.println(F("fetched Firmaware File Token: "));Serial.println((fetchedFirmawareFileToken));
        Serial.println(F("No. of Config file Lines : "));Serial.println((NoConfigLine));
        }
        }
        
        if(fetchedUpdateCtrl && fetchedFirmawareVersion > FirmwareVersion){
//    FileGet("config.txt", fetchedConfigFileToken);
//    FileGet("firmware.bin", fetchedFirmawareFileToken);
//    FileGetmain("config.txt", fetchedConfigFileToken);
//    FileGetmain("firmware.bin", fetchedFirmawareFileToken); 
//    fetchUpdate("config.txt", fetchedConfigFileToken);
//    fetchUpdate("firmware.bin", fetchedFirmawareFileToken);
      
       String buff; DevName.trim();
       if(downloadFile("config.txt", fetchedConfigFileToken)){
        if(DebugEnable){Serial.println(F("firware vesrion Verification and Device name verification."));}
        File myFile = SD.open("config.txt");
        if (!myFile) {
        Serial.print("The text file cannot be opened");     
        }else{
          int i =1; 
          while (myFile.available()) {
          buff = myFile.readStringUntil('\n');
          if(DebugEnable){Serial.print(i); Serial.println(F(" content read: ")); }
          i++;  
          //Serial.print(buff.c_str());  Serial.println("");  //Serial.println(DevName.c_str());   
          if(i >= NoConfigLine){
            if(DebugEnable){Serial.println(F("Config file reads exceeded. "));}  break;}
            char * pch; pch = strstr (buff.c_str(),(DevName).c_str());
          if (pch != NULL){
            if(DebugEnable){Serial.println(F("Starting FirmWare Download")); }          
            if(downloadFile("firmware.bin", fetchedFirmawareFileToken)){
              if(DebugEnable){Serial.println(F("Device Reset"));}               
                FirebaseUpload(udata(1,1,1)); 
                updateReboot();
              
            }else{
               FirebaseUpload(udata(1,1,0));               
              }
                
                
            }}
            }myFile.close();      
               }else{
                FirebaseUpload(udata(1,0,0)); 
                } 
               }else{
                if(DebugEnable){Serial.println(F("Installed FirmWare Version higher than Downloaded || Check install permission"));}
                FirebaseUpload(udata(0,0,0)); 
               }
   }

  String udata(int8_t x, int8_t y, int8_t z){
                modem.getNetworkTime(&year3, &month3, &day3, &hour3, &min3, &sec3,&timezone);
                String timo = String(String(year3)+"-"+String(month3)+"-"+String(day3)+" "+String(hour3)+":"+String(min3)+":"+String(sec3)+" +"+timezone+ "\"");
                String ret = "{\"Created_at\":\"" +String(timo)+",\"ThingspeakSyncState\":\"" +String(x)+"\",\"configFileDownload\":\"" +String(y)+"\",\"FirmwareFileDownload\":\"" +String(z)+"\"}";
                return ret;
    }



   int8_t downloadFile(String fileName, String token){   
//    GPRSconnect();   
    int8_t downloadStatus=0;
    File logFile;
    TinyGsmClientSecure clientx(modem);
    HttpClient http(clientx,FireBaseServer,FireBasePort);
    uint32_t expectedFileSize;
    if(!modem.isNetworkConnected())
    {
        modem.waitForNetwork(20000L,true);
    }
    if(modem.isNetworkConnected())
    {
        if(DebugEnable){Serial.println(F("network already connected:"));}
        if(!modem.isGprsConnected())
        {
            modem.gprsConnect(apn);
        }
        if(modem.isGprsConnected())
        {
            if(DebugEnable){Serial.print(F("connecting to: "));Serial.println(FireBaseServer);}
            http.connectionKeepAlive();
            int err=http.get(resource+fileName+"?alt=media&token="+token);
            if(err==0&&http.responseStatusCode()==200)
            {
                if(DebugEnable){Serial.print(F("response status code:"));
                Serial.println(http.responseStatusCode());}
                while(http.headerAvailable())
                {
                    if(http.readHeaderName()=="Content-Length")
                    {
                        if(DebugEnable){Serial.print(F("expected file size: "));}
                        String temp=http.readHeaderValue();
                        expectedFileSize=temp.toInt();
                        if(DebugEnable){Serial.print(expectedFileSize);
                        Serial.println(F(" bytes"));}
                    }
                }
                if (http.isResponseChunked()) 
                {
                    if(DebugEnable){Serial.println(F("The response is chunked"));}
                }
                unsigned long timeout = millis();
                uint32_t startTime=0;
                enableSDAlone();
                if(SDInserted == false){
                 if(DebugEnable){ Serial.println(F("SD Card failed, Device will restart:"));}
                  asm volatile(" jmp 0");
                  }
                deleteFile(fileName);
                logFile=SD.open(fileName,FILE_WRITE);
                if(logFile)//if file is successfully opened
                {
                    if(DebugEnable){Serial.print(F("downloading file:"));Serial.println(fileName);}
                    startTime=millis();
                    http.beginRequest();
                    http.connectionKeepAlive();
                    while(http.connected()&&http.available()&&!http.endOfBodyReached()&&millis()-timeout<30000L)
                    {
                        //we can also explore non blocking sd card writes,could improve the speed of downloads. but requires the 
                        //serverresponse to be treated a string, yet its best returned as a bytes for successfull flashing into memory
                        if(http.available())
                        {
                            logFile.print(char(http.read()));
                        }
                        if(millis()-timeout>=1000)
                        {
                            Serial.print(F("#"));
                        }
                        timeout=millis();
                    }
                }
                else{Serial.println(F("SD card filewrite error"));
                downloadStatus=DOWNLOAD_FAILED;
                }
                if(logFile.size()!=expectedFileSize)
                {
                    downloadStatus=DOWNLOAD_FAILED;
                    if(DebugEnable){Serial.print(F("\t downlaod failed !!.."));
                    Serial.print(F("downloaded file size: "));
                    Serial.println(logFile.size());}
                   // gsm_send_serial("AT+CIPSHUT");delay(2000);
                }
                else if (logFile.size()==expectedFileSize)
                {
                    downloadStatus=DOWNLOAD_COMPLETE;
                    if(DebugEnable){Serial.println(F("\t download complete"));
                    Serial.print(F("\t downloaded file size: "));
                    Serial.println(logFile.size());}
                   // writeStringToEEPROM(FIRMWARE_VERSION_ADDRESS_OFFSET,FIRMWARE_VERSION);//only update the firmware version if a download is complete
               //    gsm_send_serial("AT+CIPSHUT");delay(2000);
                }
                else;
                http.endRequest();
                http.stop();
                clientx.stop();
                if(DebugEnable){Serial.print((millis()-startTime)/1000);Serial.println(F(" s"));}
                logFile.close();
                modem.gprsDisconnect();
            }
            else if(err!=0)
            {
                if(DebugEnable){Serial.println(F("failed to connect to server"));}
                downloadStatus=DOWNLOAD_FAILED;
             //   gsm_send_serial("AT+CIPSHUT");delay(2000);
            }
            else if(http.responseStatusCode()!=200)
            {
                //Serial.print(F("response status code:"));
                if(DebugEnable){Serial.println(http.responseStatusCode());}
                //Serial.println(F("check your connection parameters please"));
                downloadStatus=DOWNLOAD_FAILED;
            //    gsm_send_serial("AT+CIPSHUT");delay(2000);
            }
        }
    }
//    http.stop();
//    clientx.stop();
   // gsm_send_serial("AT+CIPSHUT");delay(2000);
    return downloadStatus;
    
}

void deleteFile(String fileName)
{
    if(SD.exists(fileName))
    {
        SD.remove(fileName);
    }
}


void updateReboot()
{
  //enable watchdog timer to reset the device 
  if(DebugEnable){Serial.println(F("rebooting device now"));}
  wdt_enable(WDTO_500MS);
  EEPROM.write(0x1FF,0xF0); //set the eeprom address that the bootlader will check for during flash writting.
  wdt_reset();
  delay(600);
}
