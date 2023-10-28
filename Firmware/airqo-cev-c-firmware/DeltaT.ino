

void updatesJsonDeltaT(char* jsonBuffer, char F1[4], char F2[4], char F3[4], char F4[4], char F5[4], char F6[4], char F7[4], char F8[4]){

 // obtainTime();
 
  
  if(jsonStart == true){
    jsonStart = false;
    
    //strcat(jsonBuffer,"{\"write_api_key\":\"YOUR-CHANNEL-WRITEAPIKEY\",\"updates\":[");
    strcat(jsonBuffer,"{\"write_api_key\":\""); strcat(jsonBuffer,writeAPIKey); strcat(jsonBuffer,"\",\"updates\":[");   
    startMillis = millis();
        }

   Serial.println("Write Count:" +String(countCon));
         if(countCon == (bulkMax - 1)){
                   digitalWrite(CommsSwitch, HIGH);                
                  }
                  
   strcat(jsonBuffer,"{\"delta_t\":\"");
   strcat(jsonBuffer,String((millis()- startMillis)/1000).c_str());
   strcat(jsonBuffer,"\",");
  
  
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

  if(DebugEnable){Serial.println(F("************************************"));}
  Serial.println(obtainTime()); 
  Serial.println(jsonBuffer);
  Serial.println(strlen(jsonBuffer));
  if(DebugEnable){Serial.println(F("************************************"));}
  countCon++;
  startMillis = millis();

 if(countCon == bulkMax){
      Serial.println(F(""));Serial.println(F(""));
     
      
      size_t len = strlen(jsonBuffer);
            jsonBuffer[len-1] = ']';
          //  httpRequest(jsonBuffer);
      strcat(jsonBuffer,"}");
      Serial.println(jsonBuffer);
      
      Serial.println(strlen(jsonBuffer));
      Serial.println(F(""));Serial.println(F(""));

     // commsBulkfail();
     // httpRequestTesting(jsonBuffer);
      modemInfo(); 
      bulkUpdatemore();
      readLocation();
      FOTASyncControl();
      modemPowerOff();
      
      
     // jsonBuffer[0] = '['; //Reinitialize the jsonBuffer for next batch of data
      jsonStart = true;
      jsonBuffer[0] = '\0';
      countCon=0;
      
            
      delay(400);
 
  }
  
    delay(400);
  
}


void updatesJsonDeltaTWithSDStorage(char F1[4], char F2[4], char F3[4], char F4[4], char F5[4], char F6[4], char F7[4], char F8[4]){

  enableSDAlone();
      if(DebugEnable){Serial.println(F("Delta T Data SD STORAGE "));}
    if(SDFileStart == true){ SD.remove(fileName);}    
              
    delay(200);
    dataFile=SD.open(fileName, FILE_WRITE);
     if(dataFile)
    {  
  
  if(SDFileStart == true){
    SDFileStart = false;
    startMillis = millis();     
    dataFile.println("{\"write_api_key\":\"" + String(writeAPIKey) +"\",\"updates\":[" );      
        }
    
   Serial.println("Write Count:" +String(countCon));
         if(countCon == (countConMaxDeltat - 1)){
                   digitalWrite(CommsSwitch, HIGH);                
                  }



    dataFile.print("{\"delta_t\":\"");
    dataFile.print(String((millis()- startMillis)/1000).c_str());
    dataFile.println("\",");
   
    //dataFile.println("{\"created_at\":" +String(obtainTime().c_str())+",");
        dataFile.println( "\"field1\":\"" +String(F1)+"\",");
        dataFile.println( "\"field2\":\"" +String(F2)+"\",");
        dataFile.println( "\"field3\":\"" +String(F3)+"\",");
        dataFile.println( "\"field4\":\"" +String(F4)+"\",");
        dataFile.println( "\"field5\":\"" +String(F5)+"\",");
        dataFile.println( "\"field6\":\"" +String(F6)+"\",");
        dataFile.println( "\"field7\":\"" +String(F7)+"\",");
        //dataFile.println( "\"field8\":\"" +String(F8)+"\"},");
         countCon++;
        
        if(countCon == countConMaxDeltat){
         // Serial.println("finally correction");
          dataFile.println( "\"field8\":\"" +String(F8)+"\"" + "}]}");
          
          SDFileStart == true;
          if(DebugEnable){
          Serial.print(F("   ")); Serial.print(F("Size: "));
          Serial.print(dataFile.size());
          Serial.println(F("")); Serial.println(F(""));
          //Serial.println(dataFile.read());
          }
           
          dataFile.close();
          modemInfo(); 
          bulkUpdateSDContent();
          countCon=0;
          SD.remove(fileName);
           readLocation();
           FOTASyncControl();
           modemPowerOff();
           SDFileStart = true;
           countCon=0;
          }else{
            SDFileStart = false;
            dataFile.println( "\"field8\":\"" +String(F8)+"\"" + "},");
            Serial.println(dataFile.size());
            while(dataFile.available()){
              Serial.println(dataFile.readStringUntil('\n'));
              }
            dataFile.close();
          }    

             }
          else { if(DebugEnable){Serial.println(F("error opening files on SD Card"));}  
}
  
    delay(400);



  
}




void updatesJsonDeltaTWithSDStorageNoLoss(char F1[4], char F2[4], char F3[4], char F4[4], char F5[4], char F6[4], char F7[4], char F8[4]){

  enableSDAlone();
      if(DebugEnable){Serial.println(F("Delta T Data SD STORAGE "));}
    if(SDFileStart == true && failtimes == 0){ SD.remove(fileName);}    
              
    delay(200);
    dataFile=SD.open(fileName, FILE_WRITE);
     if(dataFile)
    {  
  
  if(SDFileStart == true){
    SDFileStart = false;
    startMillis = millis();     
    //dataFile.println("{\"write_api_key\":\"" + String(writeAPIKey) +"\",\"updates\":[" );      
        }
    
   Serial.println("Write Count:" +String(countCon));
         if(countCon == (countConMaxDeltat - 1)){
                   digitalWrite(CommsSwitch, HIGH);                
                  }
                



    dataFile.print("{\"delta_t\":\"");
    dataFile.print(String((millis()- startMillis)/1000).c_str());
    dataFile.println("\",");
   
    //dataFile.println("{\"created_at\":" +String(obtainTime().c_str())+",");
        dataFile.println( "\"field1\":\"" +String(F1)+"\",");
        dataFile.println( "\"field2\":\"" +String(F2)+"\",");
        dataFile.println( "\"field3\":\"" +String(F3)+"\",");
        dataFile.println( "\"field4\":\"" +String(F4)+"\",");
        dataFile.println( "\"field5\":\"" +String(F5)+"\",");
        dataFile.println( "\"field6\":\"" +String(F6)+"\",");
        dataFile.println( "\"field7\":\"" +String(F7)+"\",");
        countCon++;
        if(countCon == countConMaxDeltat){
         // Serial.println("finally correction");
          dataFile.println( "\"field8\":\"" +String(F8)+"\"" + "}");
          
          SDFileStart == true;
          if(DebugEnable){
          Serial.print(F("   ")); Serial.print(F("Size: "));
          Serial.print(dataFile.size());
          Serial.println(F("")); Serial.println(F(""));
          //Serial.println(dataFile.read());
          }
           
          dataFile.close();
          startMillis = millis();
          modemInfo();           
          bulkUpdateSDContentNoLoss();
          //SD.remove(fileName);
          SDFileStart = true;
          if(failtimes>0){
            dataFile=SD.open(fileName, FILE_WRITE);
             if(dataFile){
              dataFile.print( ",");
              SDFileStart = false;
              } 
              dataFile.close();
            } 
            
//                        File dataFile = SD.open(fileName);
//                        while (dataFile.available()) {
//                        Serial.println(dataFile.readStringUntil('\n'));
//                        }dataFile.close();
          
           readLocation();
           FOTASyncControl();
           modemPowerOff();
           
           countCon=0;
          }else{
            startMillis = millis(); 
            SDFileStart = false;
            dataFile.println( "\"field8\":\"" +String(F8)+"\"" + "},");
            Serial.println(dataFile.size());
            dataFile.close();
          }    

             }
          else { if(DebugEnable){Serial.println(F("error opening files on SD Card"));}  
}
  
    delay(400);

  
}
