void DevSetup(){
  int doOnce = 0; boolean op = true; 
  //String channelId;  String devName;   String writeAPIKey;
  
 // boolean MobileOperator;  
  
  while(true){
  if (doOnce == 0){
  Serial.println(F("##################### WELCOME TO DEVICE CONFIGURATION. USE THE # AS DELIMETER. USE ^ TO ERASE. USE > TO SKIP ENTRY  #######################:")); 
  Serial.print(F("Do wish to reset EEPROM data(Y || N)")); 
  doOnce = 1;}
  {    
    if (Serial.available()){
    char c = Serial.read(); if (c == 'Y' || c == 'y' ) {
        Serial.println(F(" : Y"));
        Serial.println(F("Reseting EEPROM data")); 
        Serial.print(F("EEPROM length: ")); 
        Serial.println(EEPROM.length()); Serial.println(F("Wait please###"));
        for (int i = 0 ; i < EEPROM.length() ; i++) {
        //Serial.print(F("Reseting Address: ")); Serial.println(i);
        EEPROM.write(i, 0);
        }
        Serial.println(F("Reset Done"));
        doOnce = 0; break; }
        else if (c == 'N' || c == 'n' ) {Serial.println(F(" : N"));doOnce = 0; break;} 
        else {readString += c; } } 
    
    
 }}

   while(op){
  if (doOnce == 0){
  Serial.print(F("Do wish to configure Device communication params(Y || N)")); 
  doOnce = 1;}
  {    
    if (Serial.available()){
    char c = Serial.read(); if (c == 'Y' || c == 'y' ) {
        Serial.println(F(" : Y"));delay(50);
        readString = ""; doOnce = 0; op = true;  break; }
        else if (c == 'N' || c == 'n' ) {
           Serial.println(F(" : N"));
          Serial.println(F("Done"));
          op = false; 
           Serial.print(F("Device will restart in 5 seconds, dettach the config jumper. ")); delay(2000);
           Serial.print(F(" 5 ")); delay(1000);
           Serial.print(F(" 4 "));delay(1000);
           Serial.print(F(" 3 "));delay(1000);
           Serial.print(F(" 2 "));delay(1000);
           Serial.print(F(" 1 "));delay(1000);
           Serial.print(F(" Restarting "));delay(2000);
           asm volatile(" jmp 0");
          return;} 
        else {readString += c; } } 
    
    
 }}


  
  while(op){
  if (doOnce == 0){
  readString = "";   
  runx:
  Serial.print(F("Input Device Name(22 characters MAX):")); 
  doOnce = 1;}

  if (Serial.available()>0){
    Serial.flush();Serial.flush();
    char c = Serial.read(); if (c == '#') {   
      
      Serial.println(readString);    doOnce = 0;
      while(true){
      if (doOnce == 0){
      Serial.print(F("Do you wish to commit this entry(Y || N)"));     doOnce = 1;}
       
            if (Serial.available()>0){
            Serial.flush();Serial.flush();
            char c = Serial.read(); if (c == 'Y' || c == 'y' ) {
            Serial.println(" : Y");
            Serial.println(F("Commit Done"));
            DevName = readString;   doOnce = 0; readString= ""; writeString(1, DevName); delay(10);
            op = false;
            break;              }
            else if (c == 'N' || c == 'n' ) {
              Serial.println(F(" : N")); doOnce = 0; readString= "";
              Serial.println(F("Characters Erased, continue with input"));
              goto runx;
              } 
            //else  { if ((c == ' ') || (c =='\n')){
            else  { if( (c =='\n')){  
               }else{readString += c;}
              } }         
       }
    
    } else if (c == '^') {readString= "";Serial.println(F("Characters Erased, continue with input")); }
      else if (c == '>') {readString= "";Serial.println(F("Entry skipped")); doOnce = 0; readString= ""; break; }
      else {if ((c == ' ') || (c =='\n')){
      }else{readString += c;}
      }   }     
  }op = true;


  while(op){
  if (doOnce == 0){
  readString = "";   
  runx6:
  Serial.print(F("Input Device Generation(xx characters MAX):")); 
  doOnce = 1;}

  if (Serial.available()>0){
    Serial.flush();Serial.flush();
    char c = Serial.read(); if (c == '#') {   
      
      Serial.println(readString);    doOnce = 0;
      while(true){
      if (doOnce == 0){
      Serial.print(F("Do you wish to commit this entry(Y || N)"));     doOnce = 1;}
       
            if (Serial.available()>0){
            Serial.flush();Serial.flush();
            char c = Serial.read(); if (c == 'Y' || c == 'y' ) {
            Serial.println(F(" : Y"));
            Serial.println(F("Commit Done"));
            DevGen = readString;   doOnce = 0; readString= ""; writeString(1, DevGen); delay(10);
            op = false;
            break;              }
            else if (c == 'N' || c == 'n' ) {
              Serial.println(F(" : N")); doOnce = 0; readString= "";
              Serial.println(F("Characters Erased, continue with input"));
              goto runx6;
              } 
            //else  { if ((c == ' ') || (c =='\n')){
            else  { if( (c =='\n')){  
               }else{readString += c;}
              } }         
       }
    
    } else if (c == '^') {readString= "";Serial.println(F("Characters Erased, continue with input")); }
      else if (c == '>') {readString= "";Serial.println(F("Entry skipped")); doOnce = 0; readString= ""; break; }
      else {if ((c == ' ') || (c =='\n')){
      }else{readString += c;}
      }   }     
  }op = true;


 while(op){
  if (doOnce == 0){
  readString = "";   
  runx1:
  Serial.print(F("Input Channel ID(XX characters MAX):")); 
  doOnce = 1;}

  if (Serial.available()>0){
    Serial.flush();Serial.flush();
    char c = Serial.read(); if (c == '#') {   
      
      Serial.println(readString);    doOnce = 0;
      while(true){
      if (doOnce == 0){
      Serial.print(F("Do you wish to commit this entry(Y || N)"));     doOnce = 1;}
       
            if (Serial.available()>0){
            Serial.flush();Serial.flush();
            char c = Serial.read(); if (c == 'Y' || c == 'y' ) {
            Serial.println(F(" : Y"));
            Serial.println(F("Commit Done"));
            channelId = readString.toInt();   
            doOnce = 0; readString= ""; writeString(31, String(channelId)); delay(10);
            op = false;
            break;              }
            else if (c == 'N' || c == 'n' ) {
              doOnce = 0; readString= "";
              Serial.println(F(" : N"));
              Serial.println(F("Characters Erased, continue with input"));
              goto runx1;
              } 
            else  { if ((c == ' ') || (c =='\n')){
               }else{readString += c;}
              } }         
       }
    
    } else if (c == '^') {readString= "";Serial.println(F("Characters Erased, continue with input")); }
      else if (c == '>') {readString= "";Serial.println(F("Entry skipped")); doOnce = 0; readString= ""; break; }
      else {if ((c == ' ') || (c =='\n')){
      }else{readString += c;}
      }   }     
  }op = true;


while(op){
  if (doOnce == 0){
  readString = "";   
  runx2:
  Serial.print(F("Input API Write key(XX characters MAX):")); 
  doOnce = 1;}

  if (Serial.available()>0){
    Serial.flush();Serial.flush();
    char c = Serial.read(); if (c == '#') {   
      
      Serial.println(readString);    doOnce = 0;
      while(true){
      if (doOnce == 0){
      Serial.print(F("Do you wish to commit this entry(Y || N)"));     doOnce = 1;}
       
            if (Serial.available()>0){
            Serial.flush();Serial.flush();
            char c = Serial.read(); if (c == 'Y' || c == 'y' ) {
            Serial.println(F(" : Y"));
            Serial.println(F("Commit Done"));
            //writeAPIKey = String(readString);  
            strcat(writeAPIKey,(readString.c_str()));
            writeBuffer(61, writeAPIKey);  delay(10); 
            doOnce = 0; 
            //writeString(61, String(writeAPIKey));
            delay(10); readString= ""; 
            op = false;
            break;              }
            else if (c == 'N' || c == 'n' ) {
              doOnce = 0; readString= "";
              Serial.println(F(" : N"));
              Serial.println(F("Characters Erased, continue with input"));
              goto runx2;
              } 
            else  { if ((c == ' ') || (c =='\n')){
               }else{readString += c;}
              } }         
       }
    
    } else if (c == '^') {readString= "";Serial.println(F("Characters Erased, continue with input")); }
      else if (c == '>') {readString= "";Serial.println(F("Entry skipped")); doOnce = 0; readString= ""; break; }
      else {if ((c == ' ') || (c =='\n')){
      }else{readString += c;}
      }   }     
  }op = true;




  while(op){
  if (doOnce == 0){
  readString = "";   
  runx3:
  Serial.print(F("Input Deployment Mode( 0 || 1), StaticDeploymentMode(0) || MobileDeploymentMode(1) : ")); 
  doOnce = 1;}

  if (Serial.available()>0){
    Serial.flush();Serial.flush();
    char c = Serial.read(); if (c == '#') {   
      
      Serial.println(readString);    doOnce = 0;
      while(true){
      if (doOnce == 0){
      Serial.print(F("Do you wish to commit this entry(Y || N)"));     doOnce = 1;}
       
            if (Serial.available()>0){
            Serial.flush();Serial.flush();
            char c = Serial.read(); if (c == 'Y' || c == 'y' ) {Serial.println(F(" : Y"));
            Serial.println(F("Commit Done"));
            DeploymentMode = (readString).toInt();   doOnce = 0; readString= ""; writeString(100, String(DeploymentMode));  delay(10);
            op = false;
            break;              }
            else if (c == 'N' || c == 'n' ) {Serial.println(F(" : N"));
              doOnce = 0; readString= "";
              Serial.println(F("Characters Erased, continue with input"));
              goto runx3;
              } 
            else  { if ((c == ' ') || (c =='\n')){
               }else{readString += c;}
              } }         
       }
    
    } else if (c == '^') {readString= "";Serial.println(F("Characters Erased, continue with input")); }
      else if (c == '>') {readString= "";Serial.println(F("Entry skipped")); doOnce = 0; readString= ""; break; }
      else {if ((c == ' ') || (c =='\n')){
      }else{readString += c;}
      }   }     
  }op = true;


  while(op){
    if (doOnce == 0){
    readString = "";   
    runx4:
    Serial.print(F("Define SD card CS pin number (5REVC 28) || CS 58 for REVB and Below:")); 
    doOnce = 1;}
  
    if (Serial.available()>0){
      Serial.flush();Serial.flush();
      char c = Serial.read(); if (c == '#') {   
        
        Serial.println(readString);    doOnce = 0;
        while(true){
        if (doOnce == 0){
        Serial.print(F("Do you wish to commit this entry(Y || N)"));     doOnce = 1;}
         
              if (Serial.available()>0){
              Serial.flush();Serial.flush();
              char c = Serial.read(); if (c == 'Y' || c == 'y' ) {
              Serial.println(F(" : Y"));
              Serial.println(F("Commit Done"));
              SDCS = readString.toInt();   
              doOnce = 0; readString= ""; writeString(103, String(SDCS)); delay(10);
              op = false;
              break;              }
              else if (c == 'N' || c == 'n' ) {
                Serial.println(F(" : N")); doOnce = 0; readString= "";
                Serial.println(F("Characters Erased, continue with input"));
                goto runx4;
                } 
              //else  { if ((c == ' ') || (c =='\n')){
              else  { if( (c =='\n')){  
                 }else{readString += c;}
                } }         
         }
      
      } else if (c == '^') {readString= "";Serial.println(F("Characters Erased, continue with input")); }
        else if (c == '>') {readString= "";Serial.println(F("Entry skipped")); doOnce = 0; readString= ""; break; }
        else {if ((c == ' ') || (c =='\n')){
        }else{readString += c;}
        }   }     
    }op = true;


// while(op){
//    if (doOnce == 0){
//    readString = "";   
//    runx5:
//    Serial.print(("GSM DEFINITION. SIM800(0) || SIM7600(1):")); 
//    doOnce = 1;}
//  
//    if (Serial.available()>0){
//      Serial.flush();Serial.flush();
//      char c = Serial.read(); if (c == '#') {   
//        
//        Serial.println(readString);    doOnce = 0;
//        while(true){
//        if (doOnce == 0){
//        Serial.print(F("Do you wish to commit this entry(Y || N)"));     doOnce = 1;}
//         
//              if (Serial.available()>0){
//              Serial.flush();Serial.flush();
//              char c = Serial.read(); if (c == 'Y' || c == 'y' ) {
//              Serial.println(" : Y");
//              Serial.println(F("Commit Done"));
//              GsmDEF = readString.toInt();  doOnce = 0; readString= ""; writeString(108, String(GsmDEF)); delay(10);
//              op = false;
//              break;              }
//              else if (c == 'N' || c == 'n' ) {
//                Serial.println(" : N"); doOnce = 0; readString= "";
//                Serial.println(F("Characters Erased, continue with input"));
//                goto runx5;
//                } 
//              //else  { if ((c == ' ') || (c =='\n')){
//              else  { if( (c =='\n')){  
//                 }else{readString += c;}
//                } }         
//         }
//      
//      } else if (c == '^') {readString= "";Serial.println(F("Characters Erased, continue with input")); }
//        else if (c == '>') {readString= "";Serial.println(F("Entry skipped")); doOnce = 0; readString= ""; break; }
//        else {if ((c == ' ') || (c =='\n')){
//        }else{readString += c;}
//        }   }     
//    }op = true;




    Serial.println(F("OutPut"));
    Serial.print(F("Device Name: ")); Serial.println(read_String(1));  
    Serial.print(F("Channel ID: ")); Serial.println(read_EE(31));   
    Serial.print(F("Write Key: ")); Serial.println(read_String(61)); 
    Serial.print(F("SDCS PIN no.: ")); Serial.println(read_EE(103)); 
   // Serial.print(F("GSM Definition: ")); Serial.println(read_String(108));  
    Serial.print(F("Deployment Mode: ")); Serial.println(read_EE(100));  

//    Serial.println(F("Read Values."));
    
//    writeString(1, devName);
//    writeString(31, channelId);
//    writeString(61, writeAPIKey);
{Serial.print(F("Device will restart in 5 seconds, dettach the config jumper. ")); delay(2000);
   Serial.print(F(" 5 ")); delay(1000);
   Serial.print(F(" 4 "));delay(1000);
   Serial.print(F(" 3 "));delay(1000);
   Serial.print(F(" 2 "));delay(1000);
   Serial.print(F(" 1 "));delay(1000);
   Serial.print(F(" Restarting "));delay(2000);
   asm volatile(" jmp 0");}
  }





void eerpromView(int startAdd, int endAdd){
  Serial.println(F("EEPROM memoery view"));
  Serial.println(EEPROM.length());
        for (int i = startAdd ; i < endAdd ; i++) {
        Serial.print(F("Address: ")); Serial.print(i);
        Serial.print(F(" Value: ")); Serial.println(EEPROM.read(i));
               }
  }

void writeString(char add,String data)
{
  int _size = data.length();
  int i;
  for(i=0;i<_size;i++)
  {
    EEPROM.write(add+i,data[i]);
  }
  EEPROM.write(add+_size,'\0');   //Add termination null character for String Data
 // EEPROM.commit();
}


String read_String(char add)
{
  int i;
  char data[100]; //Max 100 Bytes
  int len=0;
  unsigned char k;
  k=EEPROM.read(add);
  while(k != '\0' && len<500)   //Read until null character
  {    
    k=EEPROM.read(add+1+len);
    data[len]=k;
    len++;
  }
  data[len]='\0';
  return String(data);
}

String read_EE(char add)
{
  int i;
  char data[100]; //Max 100 Bytes
  int len=0;
  unsigned char k;
  k=EEPROM.read(add);
  while(k != '\0' && len<500)   //Read until null character
  {    
    k=EEPROM.read(add+len);
    data[len]=k;
    len++;
  }
  data[len]='\0';
  return String(data);
}

void writeBuffer(char add, char* Buffer)
{
  int _size = strlen(Buffer);
  int i;
  for(i=0;i<_size;i++)
  {
    EEPROM.write(add+i,Buffer[i]);
  }
}

void readtobuffer(char add, char *point)
{
  int i;
  //char data[100]; //Max 100 Bytes
  int len=0;
  unsigned char k;
  k=EEPROM.read(add);
  while(k != '\0' && len<500)   //Read until null character
  {  if(k =='\0'|| k==' '|| k==" "|| k=='13'){
   
    }
    k=EEPROM.read(add+1+len);
    point[len]=k;
    len++;
  }
 
}
