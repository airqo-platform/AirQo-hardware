  void enableSD(){
  if(SD.begin(sdcardChipSelect))
  {
      if(DebugEnable){Serial.println(F("SD card initialized successfully"));}
    SDInserted = true;
  //  CardDetails();
  }
  else if(!SD.begin(sdcardChipSelect)){
    if(SD.begin(sdcardChipSelect))
  {}else{
      if(DebugEnable){Serial.println(F("initialization of SD card failed"));}
     SDInserted = false;
        }
  }
  
  //scanI2c();
  if (! rtc.begin()) {
    RTCAvailable = false;
    if(DebugEnable){Serial.println(F("Couldn't find RTC"));}
  Serial.flush();
//  while (1) delay(10);
  }else{RTCAvailable = true; }
  if (rtc.lostPower()) {
      if(DebugEnable){Serial.println(F("RTC lost power, let's set the time!"));}
     //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  }
  

     void enableSDAlone(){
  if(SD.begin(sdcardChipSelect))
  {
      if(DebugEnable){Serial.println(F("SD card initialized successfully"));}
    SDInserted = true;
    //CardDetails();
  }
  else if(!SD.begin(sdcardChipSelect)){
    if(SD.begin(sdcardChipSelect))
  {}else{
      if(DebugEnable){Serial.println(F("initialization of SD card failed"));}
     SDInserted = false;
        }
                                      }
  }

 
void StoreTempData(char F1[4], char F2[4], char F3[4], char F4[4], char F5[4], char F6[4], char F7[4], char F8[4]){

    enableSDAlone();
      if(DebugEnable){Serial.println(F("Storing Data "));}
    if(SDFileStart == true){ SD.remove(fileName);}    
              
    delay(200);
    dataFile=SD.open(fileName, FILE_WRITE);
     if(dataFile)
    {
      

       if(SDFileStart == true){
        //Serial.println("SD file Start:**************");
        SDFileStart = false;
       // dataFile.println("{\"write_api_key\":\"" + String(writeAPIKey) +"\",\"updates\":[" );
        }
        Serial.println("Write Count:" +String(countCon));
         if(countCon == (countConMax - 1)){
                   digitalWrite(CommsSwitch, HIGH);                
                  }

        dataFile.println("{\"created_at\":" +String(obtainTime().c_str())+",");
        dataFile.println( "\"field1\":\"" +String(F1)+"\",");
        dataFile.println( "\"field2\":\"" +String(F2)+"\",");
        dataFile.println( "\"field3\":\"" +String(F3)+"\",");
        dataFile.println( "\"field4\":\"" +String(F4)+"\",");
        dataFile.println( "\"field5\":\"" +String(F5)+"\",");
        dataFile.println( "\"field6\":\"" +String(F6)+"\",");
        dataFile.println( "\"field7\":\"" +String(F7)+"\",");
        countCon++;
        
        if(countCon == countConMax){
         // Serial.println("finally correction");
          dataFile.println( "\"field8\":\"" +String(F8)+"\"" + "}]}");
          
          SDFileStart == true;
          if(DebugEnable){
          Serial.print(F("   ")); Serial.print(F("Size: "));
          Serial.print(dataFile.size());
          Serial.println(F("")); Serial.println(F(""));
          }
          dataFile.close();
          modemInfo(); 
          bulkUpdateSDContent();
          countCon=0;
          SD.remove(fileName);
           readLocation();
           FOTASyncControl();
           modemPowerOff();
          }else{
          dataFile.println( "\"field8\":\"" +String(F8)+"\"" + "},");
          Serial.println(dataFile.size());
          dataFile.close();
          }    

             }
          else { if(DebugEnable){Serial.println(F("error opening files on SD Card"));}  }
}



//**************no loss methods****************************************
void StoreTempDataNoLoss(char F1[4], char F2[4], char F3[4], char F4[4], char F5[4], char F6[4], char F7[4], char F8[4]){
//No Loss communication Storage Structure
    enableSDAlone();
      if(DebugEnable){Serial.println(F("Storing Data "));}
    if(SDFileStart == true && failtimes == 0){ SD.remove(fileName);}    
              
    delay(200);
    dataFile=SD.open(fileName, FILE_WRITE);
     if(dataFile)
    {     

       if(SDFileStart == true){       
        SDFileStart = false;
        Serial.println("Write Count:" +String(countCon));
         //Restore Communication One minute before final 
         if(countCon == (countConMax - 1)){
                   digitalWrite(CommsSwitch, HIGH);                
                  }

        dataFile.println("{\"created_at\":" +String(obtainTime().c_str())+",");
        dataFile.println( "\"field1\":\"" +String(F1)+"\",");
        dataFile.println( "\"field2\":\"" +String(F2)+"\",");
        dataFile.println( "\"field3\":\"" +String(F3)+"\",");
        dataFile.println( "\"field4\":\"" +String(F4)+"\",");
        dataFile.println( "\"field5\":\"" +String(F5)+"\",");
        dataFile.println( "\"field6\":\"" +String(F6)+"\",");
        dataFile.println( "\"field7\":\"" +String(F7)+"\",");
        //dataFile.println( "\"field8\":\"" +String(F8)+"\"" + "}]}");
        dataFile.println( "\"field8\":\"" +String(F8)+"\"" + "}");
        countCon++;
                 
       dataFile.close();
        }else {

          if(DebugEnable){Serial.println("Write Count:" +String(countCon));}
         if(countCon == (countConMax - 1)){
                   digitalWrite(CommsSwitch, HIGH);                
                  }
        dataFile.println( ",");
        dataFile.println("{\"created_at\":" +String(obtainTime().c_str())+",");
        dataFile.println( "\"field1\":\"" +String(F1)+"\",");
        dataFile.println( "\"field2\":\"" +String(F2)+"\",");
        dataFile.println( "\"field3\":\"" +String(F3)+"\",");
        dataFile.println( "\"field4\":\"" +String(F4)+"\",");
        dataFile.println( "\"field5\":\"" +String(F5)+"\",");
        dataFile.println( "\"field6\":\"" +String(F6)+"\",");
        dataFile.println( "\"field7\":\"" +String(F7)+"\",");
        dataFile.println( "\"field8\":\"" +String(F8)+"\"" + "}");
        countCon++;
        
        if(countCon == countConMax){
         // Serial.println("finally correction");
          
          
          SDFileStart == true;
          if(DebugEnable){
          Serial.println(dataFile.size());
          Serial.println(F("")); Serial.println(F(""));
          }
          dataFile.close();
          modemInfo(); 
          bulkUpdateSDContentNoLoss();
          countCon=0;
         // SD.remove(fileName);
           readLocation();
           FOTASyncControl();
           modemPowerOff();
          }else{
         
          Serial.println(dataFile.size());
          dataFile.close();
          }  
          
          }
          

             }
          else { if(DebugEnable){Serial.println(F("error opening files on SD Card"));}  }
}




//void CSVLogger(char F1[4], char F2[4], char F3[4], char F4[4], char F5[4], char F6[4], char F7[4], char F8[4], char F9[4], char F10[4], char F11[4], char F12[4],  String ALL){

void CSVLogger(char F1[4], char F2[4], char F3[4], char F4[4], char F5[4], char F6[4], char F7[4], char F8[4], char F9[4], String ALL){

    enableSDAlone();
    if(DebugEnable){Serial.println(F("CSV Logger"));}
   
    delay(200);

     File dataFileStart = SD.open(CSVName()+".CSV");
  if (dataFileStart) {
      dataFileStart.close();
      dataFile=SD.open(CSVName()+".csv", FILE_WRITE);     
        dataFile.print( String(obtainTime().c_str())+",");
        dataFile.print(DevName+",");
        dataFile.print(String(F1)+",");
        dataFile.print(String(F2)+",");
        dataFile.print(String(F3)+",");
        dataFile.print(String(F4)+",");
        dataFile.print(String(F5)+",");
        dataFile.print(String(F6)+",");
        dataFile.print(String(F7)+",");
        dataFile.print(String(F8)+",");
        dataFile.print(String(F9)+",");
//        dataFile.print(String(F10)+",");
//        dataFile.print(String(F11)+",");
//        dataFile.print(String(F12)+",");
        dataFile.println(ALL);
        dataFile.close();

    
      } else {
           
    dataFile=SD.open(CSVName()+".CSV", FILE_WRITE);
    dataFile.println( String("created_at:") +String(",")+ String("DevName") +String(",")+ String("PM1(1)") +String(",")+ String("PM2.5(1)") +String(",")+ String("PM10(1)") +String(",")+ String("PM1(2)")
    +String(",")+ String("PM2.5(2)") +String(",")+ String("PM10(2)")
    //+String(",")+ String("Latitude") +String(",")+ String("Longitude")+String(",")+String("Altitude")
    +String(",")+String("VBat")+String(",")+String("SignalQuality")+String(",")+String("GSM Temp")    
    +String(",")+ String("latitude") +String(",")+ String("Longitude")+String(",")+String("Altitude")+String(",")+String("Velocity")+String(",")+String("no.Sats")+String(",")+String("HDPD")+String(",")+String("Temp INT")
    +String(",")+ String("Humidity INT") +String(",")+ String("Temp Ext")+String(",")+String("Humidity Ext")+String(",")+String("Pressure Ext")+String(",")+String("Altitude Ext")
    );
                
        
        dataFile.print( String(obtainTime().c_str())+",");
        dataFile.print(DevName+",");
        dataFile.print(String(F1)+",");
        dataFile.print(String(F2)+",");
        dataFile.print(String(F3)+",");
        dataFile.print(String(F4)+",");
        dataFile.print(String(F5)+",");
        dataFile.print(String(F6)+",");
        dataFile.print(String(F7)+",");
        dataFile.print(String(F8)+",");
        dataFile.print(String(F9)+",");
//        dataFile.print(String(F10)+",");
//        dataFile.print(String(F11)+",");
//        dataFile.print(String(F12)+",");
        dataFile.println(ALL);
        dataFile.close();
    
  }
    
}
