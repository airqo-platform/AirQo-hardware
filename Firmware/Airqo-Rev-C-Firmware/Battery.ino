


//////////////////////////////////Power Saving methods,...
int8_t VBatPt(float vBat){  
  // Bat charge source https://hobbygraderc.com/lipo-voltage-chart/
   int8_t ret;
   if(DebugEnable){Serial.print(F("VBat Percentage "));}

   if(vBat >=4.2){ret = 100;}
     else if(4.2>vBat && vBat>4.15){ret = 95;}  
     else if(4.15>vBat && vBat>4.11){ret = 90;}
     else if(4.11>vBat && vBat>4.08){ret = 85;}
     else if(4.08>vBat && vBat>4.02){ret = 80;}
     else if(4.02>vBat && vBat>3.98){ret = 75;}    
     else if(3.98>vBat && vBat>3.95){ret = 70;}
     else if(3.95>vBat && vBat>3.91){ret = 65;}    
     else if(3.91>vBat && vBat>3.87){ret = 60;}
     else if(3.87>vBat && vBat>3.85){ret = 55;}   
     else if(3.85>vBat && vBat>3.84){ret = 50;}
     else if(3.84>vBat && vBat>3.82){ret = 45;}
     else if(3.82>vBat && vBat>3.80){ret = 40;}
     else if(3.80>vBat && vBat>3.79){ret = 35;}
     else if(3.79>vBat && vBat>3.77){ret = 30;}
     else if(3.77>vBat && vBat>3.75){ret = 25;}
     else if(3.75>vBat && vBat>3.73){ret = 20;}
     else if(3.73>vBat && vBat>3.71){ret = 15;}
     else if(3.71>vBat && vBat>3.69){ret = 10;}
     else if(3.69>vBat && vBat>3.61){ret = 5;}
     else if(3.61>vBat && vBat>3.4){ret = 1;}    
return ret;
                                              
  }


//void AfterPosSleep(){
//           
//          
//          int counter = 0;
//          
//          if(BatteryVoltageRef() < 3.2){
//            
////               pms.sleep(); pms2.sleep();
////               delay(100);               
////               mySerial.end(); Serial3.end(); 
////               DBGln("PMS sensors PowerOff"); 
//               digitalWrite(pmsSwitch, HIGH);//switch the sensors on for read.
//               digitalWrite(CommsSwitch, LOW);
//               digitalWrite(IandETempSwitch, LOW);
//          loop:
//         while(counter < 450){//30 minutes complete sleep interval.
//                 if(EnableAlarms){
//                    noTone(22);  tone(22, 4000, 50);delay(50);noTone(22);  tone(22, 4500, 100);          
//                  }
//                   if(DebugEnable){Serial.print(F("AFTER POSTING SLEEP POWERING DOWN SLEEP REGHARGE BATTERY: "));
//                  Serial.println(counter);
//                  delay(100);
//                   }
//                  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
//                   if(DebugEnable){Serial.print(F("watch dog bite #")); Serial.println(sleepCounter);  }
//                  counter++;
//                  if(counter < 449) goto loop;
//                //  asm volatile(" jmp 0");
//                 }
//                 if(DebugEnable){Serial.println(F("\n")); Serial.println(F("PMS sensors PowerOn")); }
////                 mySerial.begin(9600); Serial3.begin(9600); 
////                 digitalWrite(pmsSwitch, LOW);//switch the sensors on for read.
////                 pms.wakeUp(); pms2.wakeUp();
////                 pms.activeMode(); pms2.activeMode();
////                 delay(100); 
//              digitalWrite(CommsSwitch, HIGH);
//              digitalWrite(pmsSwitch, LOW);
//              digitalWrite(IandETempSwitch, HIGH);
//               if(DebugEnable){Serial.print(F("POWERING UP WAKING FROM SLEEP"));}
//          
//              start = 0;
//          }
//                    
//}


int8_t BatteryVoltage(){
            
          
          int pnMeasurements = 10; double sumSensorValues = 0.0;  double meanSensorValues = 0.0;
            for(int i = 0; i < pnMeasurements; i++){
            sumSensorValues += (analogRead(Battery));
            delay(100);
            }
          
          
          float voltage = (sumSensorValues / pnMeasurements) * (5.0 / 1023.0);
          // print out the value you read:
           if(DebugEnable){Serial.print(F("Battery Voltage: "));    Serial.println(voltage);
          Serial.print(VBatPt(voltage)); Serial.println("%");}
          int counter = 0;
          
          if(voltage < 3.2){
            digitalWrite(CommsSwitch, LOW);
            digitalWrite(pmsSwitch, HIGH);          
          loop:
         while(counter < 150){//10 minutes complete sleep interval.
                  // if(EnableAlarms){noTone(22);  tone(22, 4000, 100);delay(125);noTone(22);  tone(22, 4000, 100);}
                  if(EnableAlarms){
                    noTone(22);  tone(22, 4000, 50);delay(50);noTone(22);  tone(22, 4500, 100);
                    }           
                   if(DebugEnable){Serial.print(F("POWERING DOWN SLEEP REGHARGE BATTERY: "));
                  Serial.print(counter);}
                  delay(100);
                  
                  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
                  counter++;
                  if(counter < 75) goto loop;
                  asm volatile(" jmp 0");
                 }
          }
          return voltage;
}


double BatteryVoltageRef(){
          int nMeasurements = 10;  double sumSensorValues = 0.0; double meanSensorValues = 0.0;  double SensorValues = 0.0;
          for(int i = 0; i < nMeasurements; i++){
            sumSensorValues += analogRead(Battery);
            delay(100);
            }
          meanSensorValues = sumSensorValues / nMeasurements;
          float aveVoltage = meanSensorValues * (5.0 / 1023.0);
          // print out the value you read:
           if(DebugEnable){Serial.print(F("Battery Voltage: "));    Serial.println(aveVoltage);
           Serial.print(VBatPt(aveVoltage)); Serial.println("%");
           }
          int counter = 0;
          
          return aveVoltage;
}


//void PowerSaver(){
//
//          int nMeasurements = 30; double sumSensorValues = 0.0;  double meanSensorValues = 0.0;  double SensorValues = 0.0;
//          for(int i = 0; i < nMeasurements; i++){
//            sumSensorValues += analogRead(Battery);
//            delay(100);
//            }
//          meanSensorValues = sumSensorValues / nMeasurements;
//          float aveVoltage = meanSensorValues * (5.0 / 1023.0);         
//
//           if(DebugEnable){Serial.println(F("Average Battery Voltage: "));  Serial.println(aveVoltage);
//           Serial.print(VBatPt(aveVoltage)); Serial.println("%");
//           }
//        if (aveVoltage < 3.3){// sleeping for five minute intervals
//          digitalWrite(CommsSwitch, LOW);
//        loop:
//        while (sleepCounter < MAX_SLEEP_ITERATIONS){
//              if(EnableAlarms){
//                noTone(22);  tone(22, 4000, 50);delay(50);noTone(22);  tone(22, 4500, 100);
//                }                  
//               if(DebugEnable){Serial.println(F("POWERING DOWN ENTERING SLEEP MODE"));}
//              digitalWrite(CommsSwitch, LOW);
//              digitalWrite(pmsSwitch, HIGH);
//              digitalWrite(IandETempSwitch, LOW);
//              LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);  
//               if(DebugEnable){Serial.print(F("watch dog bite #")); Serial.print(sleepCounter);}
//              sleepCounter++;
//              goto loop;
//                                  
//             }
//              sleepCounter = 0;
//              digitalWrite(CommsSwitch, HIGH);
//              digitalWrite(pmsSwitch, LOW);
//              digitalWrite(IandETempSwitch, HIGH);
//               if(DebugEnable){Serial.println(F("POWERING UP WAKING FROM SLEEP"));}
//             
//              start = 0;
//        }
//        start = 0;
//  }



void GsmPowerDown(int waitTime){
    if(EnableAlarms){noTone(22);  tone(22, 3800, 100);delay(75);noTone(22);  tone(22, 4000, 100);}
    if(DebugEnable){
    Serial.print(F("GSM shut Down."));}
    //SerialDBG("GSM shut Down.");
    //gsm.SimpleWrite("AT");
    delay(1000);
    //gsm.SimpleWrite("AT+CPOWD=1");
    delay(1000);
    digitalWrite(CommsSwitch, LOW);
    delay(waitTime);
    digitalWrite(CommsSwitch, HIGH);
    if(DebugEnable){Serial.print(F("Comms Power Restored."));}
    
  
    //startupGSM900();
    //startupGPRS();
   
  }

 void GsmPowerDownNoGPRSStart(int waitTime){
    if(EnableAlarms){noTone(22);  tone(22, 3800, 100);delay(75);noTone(22);  tone(22, 4000, 100);}
     if(DebugEnable){Serial.print(F("GSM shut Down."));}
    // gsm.SimpleWrite("AT");
    delay(1000);
    // gsm.SimpleWrite("AT+CPOWD=1");
    delay(1000);
    digitalWrite(CommsSwitch, LOW);
    delay(waitTime);
    digitalWrite(CommsSwitch, HIGH);
     if(DebugEnable){Serial.print(F("Comms Power Restored."));};
    //startupGSM900();
   
   
  }

  //************************************************************************************************

   
