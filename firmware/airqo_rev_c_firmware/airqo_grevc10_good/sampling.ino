void Check_pms()
{

  
 Serial.println(F("\n")); Serial.println(F("PMS sensors PowerOn")); 
 mySerial.begin(9600); Serial3.begin(9600); 
 digitalWrite(pmsSwitch, LOW);//switch the sensors on for read.
 pms.wakeUp(); pms2.wakeUp();
 pms.activeMode(); pms2.activeMode();
 delay(100);  
 test_pms();
    
   if(DebugEnable){  Serial.println(F("PM measurements"));}   
    
     // Sums for calculating statistics
              

              double Sensor1PM1Mean = 0.0; double Sensor1PM25Mean = 0.0;  double Sensor1PM10Mean = 0.0;
              double Sensor2PM1Mean = 0.0; double Sensor2PM25Mean = 0.0;  double Sensor2PM10Mean = 0.0;

              double Sensor1PM1Sum = 0.0;  double Sensor1PM25Sum = 0.0;   double Sensor1PM10Sum = 0.0;
              double Sensor2PM1Sum = 0.0;  double Sensor2PM25Sum = 0.0;   double Sensor2PM10Sum = 0.0;

              // Arrays to hold multiple measurements
             
//              int Sensor1PM1Array[pmSamples]; int Sensor1PM25Array[pmSamples];  int Sensor1PM10Array[pmSamples];
//              int Sensor2PM1Array[pmSamples]; int Sensor2PM25Array[pmSamples];  int Sensor2PM10Array[pmSamples];
              int i = 0;

               

               // Make measurements and add to arrays
              //for (int i = 0; i < nMeasurements; i++) 
                do{
                       
                  // Print out the pms5003
                   if (pms.read(data))
                    {
                      if(DebugEnable){
                      Serial.print(F("\nNumber of samples:\t")); Serial.println(i);
                      Serial.print(F("Sensor1 PM 1.0 (ug/m3): ")); Serial.println(data.PM_AE_UG_1_0);                  
                      Serial.print(F("Sensor1 PM 2.5 (ug/m3): ")); Serial.println(data.PM_AE_UG_2_5);                  
                      Serial.print(F("Sensor1PM 10.0 (ug/m3): ")); Serial.println(data.PM_AE_UG_10_0);                  
                      Serial.println(F(""));
                      }
                      Sensor1PM1Sum += data.PM_AE_UG_1_0;
                      Sensor1PM25Sum += data.PM_AE_UG_2_5;
                      Sensor1PM10Sum += data.PM_AE_UG_10_0;
                      
                      
                    }
                  
                    if (pms2.read(data2))
                    {
                      if(DebugEnable){
                      Serial.print(F("Sensor2 PM 1.0 (ug/m3): ")); Serial.println(data2.PM_AE_UG_1_0);                  
                      Serial.print(F("Sensor2 PM 2.5 (ug/m3): ")); Serial.println(data2.PM_AE_UG_2_5);                  
                      Serial.print(F("Sensor2 PM 10.0 (ug/m3): "));  Serial.println(data2.PM_AE_UG_10_0);                  
                      Serial.println(F(""));
                      }
                      Sensor2PM1Sum += data2.PM_AE_UG_1_0;
                      Sensor2PM25Sum += data2.PM_AE_UG_2_5;
                      Sensor2PM10Sum += data2.PM_AE_UG_10_0;
                      i++;
                    }
                 // Serial.println();
                
                        
                }while(i<pmSamples);
              
               pms.sleep(); pms2.sleep();

               delay(100);
               
               mySerial.end(); Serial3.end(); 
               if(DebugEnable){Serial.println(F("PMS sensors PowerOff"));} 
               digitalWrite(pmsSwitch, HIGH);//switch the sensors on for read.
               

               Sensor1PM1Mean = Sensor1PM1Sum /pmSamples;
               Sensor2PM1Sum = Sensor2PM1Sum / pmSamples;
               
               Sensor1PM25Mean = Sensor1PM25Sum / pmSamples;
               Sensor1PM10Mean = Sensor1PM10Sum / pmSamples;
               Sensor2PM25Mean = Sensor2PM25Sum / pmSamples;
               Sensor2PM10Mean = Sensor2PM10Sum / pmSamples;

           
                
              //Find the size of array. 
//              int n = sizeof(Sensor1PM25Array)/sizeof(Sensor1PM25Array[0]); 
//              float pm25Corel  = correlationCoefficient(Sensor1PM25Array, Sensor2PM25Array, 30);
//              float pm10Corel  = correlationCoefficient(Sensor1PM10Array, Sensor2PM10Array, 30);
//             
             if(DebugEnable){
             Serial.println(F(""));
             Serial.println(F("**AVERAGE READINGS**"));
             Serial.print(F("\nNumber of samples:\t")); Serial.println(pmSamples);
             Serial.print(F("Sensor1 PM2.5: ")); Serial.println(Sensor1PM25Mean);
             Serial.print(F("Sensor1 PM10: ")); Serial.println(Sensor1PM10Mean);
             Serial.print(F("Sensor2 PM2.5: ")); Serial.println(Sensor2PM25Mean);
             Serial.print(F("Sensor2 PM10: ")); Serial.println(Sensor2PM10Mean);
//             Serial.print("PM2.5 Coeffient of correlation: "); Serial.println(corr);
//             Serial.print("PM10 Coeffient of correlation: "); Serial.println(pm10Corel);
             Serial.println(F(""));
             //Serial.println("********************************************************");
             }
             postActivities(Sensor1PM1Mean, Sensor1PM25Mean, Sensor1PM10Mean, Sensor2PM1Sum ,Sensor2PM25Mean, Sensor2PM10Mean); 
             //comms(Sensor1PM1Mean, Sensor1PM25Mean, Sensor1PM10Mean, Sensor2PM1Sum ,Sensor2PM25Mean, Sensor2PM10Mean); 
              

                                      
}



//************************************sensor test
void test_pms()
{
   if(DebugEnable){Serial.println(F("Testing PMS dual sensors"));}
    int i = 0;
    int nMeasurements = 1; 
              do{
                       
                  // Print out the pms5003
                   if (pms.read(data))
                    {
//                      SerialDBG("\nNumber of samples:\t"); SerialDBGln(i);
//                      SerialDBG("Sensor1 PM 1.0 (ug/m3): "); SerialDBGln(data.PM_AE_UG_1_0);                  
//                      SerialDBG("Sensor1 PM 2.5 (ug/m3): "); SerialDBGln(data.PM_AE_UG_2_5);                  
//                      SerialDBG("Sensor1PM 10.0 (ug/m3): "); SerialDBGln(data.PM_AE_UG_10_0); 
//                      SerialDBGln();
                      
                    }
                  
                    if (pms2.read(data2))
                    {
//                      SerialDBG("Sensor2 PM 1.0 (ug/m3): "); SerialDBGln(data2.PM_AE_UG_1_0);                  
//                      SerialDBG("Sensor2 PM 2.5 (ug/m3): "); SerialDBGln(data2.PM_AE_UG_2_5);                  
//                      SerialDBG("Sensor2 PM 10.0 (ug/m3): ");  SerialDBGln(data2.PM_AE_UG_10_0);                  
//                      SerialDBGln();

                      i++;
                    }
                 // SerialDBGln();
                        
                }while(i<nMeasurements);
       // SerialDBGln("");      
        if(DebugEnable){Serial.println(F("Done.")); }    
                                      
}


///*******************************************************************************************

void Read_Status(float Sensor1PM25, float Sensor1PM10, float Sensor2PM25, float Sensor2PM10)
{
 if(DebugEnable){ 
  Serial.print(F("AVERAGE Sensor1 PM2.5 CONCENTRATION :"));  Serial.print(Sensor1PM25);  Serial.print(F("ug/M3"));  Serial.print(F(" | "));
  Serial.print(F("AVERAGE Sensor1 PM10 CONCENTRATION :"));  Serial.print(Sensor1PM10);  Serial.print(F("ug/M3"));  Serial.print(F(" | "));
  Serial.print(F("AVERAGE Sensor2 PM2.5 CONCENTRATION :"));  Serial.print(Sensor2PM25);  Serial.print(F("ug/M3"));  Serial.print(F(" | "));
  Serial.print(F("AVERAGE Sensor2 PM10 CONCENTRATION :"));  Serial.print(Sensor2PM10);  Serial.print(F("ug/M3"));
  Serial.print(F("LATITUDE:"));  Serial.print(lat2);  Serial.print(F("LONGITUDE"));  Serial.print(lon2);
 }
 }




////////////////////////////////Temaperature and Humidity Data taken here.
void getTempData(){
 if(DebugEnable){Serial.println(F("\n")); 
 Serial.println(F("Int/Ext RH and Temperature Data.")); 
 }
 digitalWrite(IandETempSwitch, HIGH);//switch the sensors on for read.
 delay(100);
 
 bme.begin();
 if (!bme.begin()){ 
    if(DebugEnable){Serial.println(F("Error! No BME280 Sensor Detected!!!"));
    
    Serial.println(F("Restarting internal and external Sensors"));
    }
    digitalWrite(IandETempSwitch, LOW);
    delay(100);
    if(DebugEnable){Serial.println(F("Staring internal and external Sensors"));}
    digitalWrite(IandETempSwitch, HIGH);
    delay(100);
    bme.begin();
     if (!bme.begin()){ 
    if(DebugEnable){Serial.println(F("BME Sensor Startup Failure!!!"));}
    delay(100);
     }
    //while (1);
  }else{
   digitalWrite(IandETempSwitch, HIGH);
   delay(400);
    ReadExternalTempHumidity();
    }
 digitalWrite(IandETempSwitch, HIGH);
 delay(400);
 readInternalTempHumidity();
 checkSHT();
 
 delay(400);
 if(DebugEnable){Serial.println(F("\n"));
 Serial.println(F("Done taking Temp data."));}
 digitalWrite(IandETempSwitch, LOW);
  }
//************************************************************************************************

  

double Fahrenheit(double celsius) 
{
        return 1.8 * celsius + 32;
}   
double Kelvin(double celsius)
{
        return celsius + 273.15;
}     
double dewPoint(double celsius, double humidity)
{
        double A0= 373.15/(273.15 + celsius);
        double SUM = -7.90298 * (A0-1);
        SUM += 5.02808 * log10(A0);
        SUM += -1.3816e-7 * (pow(10, (11.344*(1-1/A0)))-1) ;
        SUM += 8.1328e-3 * (pow(10,(-3.49149*(A0-1)))-1) ;
        SUM += log10(1013.246);
        double VP = pow(10, SUM-3) * humidity;
        double T = log(VP/0.61078);   // temp var
        return (241.88 * T) / (17.558-T);
}

double dewPointFast(double celsius, double humidity)
{
        double a = 17.271;
        double b = 237.7;
        double temp = (a * celsius) / (b + celsius) + log(humidity/100);
        double Td = (b * temp) / (a - temp);
        return Td;
}


void readInternalTempHumidity()
{
  if(DebugEnable){Serial.println(F("\n"));}
  int chk = DHT11.read(DHT11PIN);
  if(DebugEnable){Serial.println(F("Read sensor: "));}
   switch (chk)
  {
    case DHTLIB_OK: 
                if(DebugEnable){Serial.println(F("OK"));} 
                break;
    case DHTLIB_ERROR_CHECKSUM: 
                if(DebugEnable){Serial.println(F("Checksum error"));} 
                break;
    case DHTLIB_ERROR_TIMEOUT: 
                if(DebugEnable){Serial.println(F("Time out error"));} 
                break;
    default: 
                if(DebugEnable){Serial.println(F("Unknown error"));}
                break;
  }
  if(DebugEnable){Serial.println(F("DHT11 Data")); }
 
  IntTemp = DHT11.temperature;  IntHumidity = DHT11.humidity;
  delay(500);
  if(DebugEnable){Serial.print(F("Humidity (%): "));  Serial.println(IntHumidity);  
  Serial.print(F("Temperature (oC): "));  Serial.println(IntTemp);}
 delay(400);
}



  void ReadExternalTempHumidity(){
    ExtTemp = bme.readTemperature(); ExtHumidity = bme.readHumidity(); ExtPressure = bme.readPressure()/ 100; ExtAltitude = bme.readAltitude(1013.25);
     if(DebugEnable){
      Serial.println(F("\n"));
      Serial.println(F("-BME280 Data-\n"));  
    
      Serial.println(F("Temperature = ")); Serial.print(ExtTemp); Serial.println(F(" *C"));
      Serial.println(F("Humidity = "));    Serial.print(ExtHumidity);    Serial.println(F(" %"));
      Serial.println(F("Pressure = "));    Serial.print(ExtPressure); Serial.println(F(" mb"));// 100 Pa = 1 millibar (Pa = newton per square meter)
      Serial.println(F("Approx Altitude = "));   Serial.print(ExtAltitude); Serial.println(F(" m")); // This should be lined up (atmospheric pressure at sea level is 1013 millibars)
    
      Serial.println(F("--------------------------------\n\n"));
    }  
    
}





  void checkSHT(){
  if(DebugEnable){
   Serial.println(F("\n")); 
   Serial.println(F("Checking SHT25 Data,."));
  } 
 
 digitalWrite(IandETempSwitch, HIGH);//switch the sensors on for read.
 delay(100);
 
 sht20.begin();
 if (!sht20.begin()){ 
     if(DebugEnable){Serial.println(F("Error! No SHT25 Sensor Detected!!!"));}
    delay(100);
     if(DebugEnable){Serial.println(F("Restarting internal and external Sensors"));}
    digitalWrite(IandETempSwitch, LOW);
    delay(100);
     if(DebugEnable){Serial.println(F("Staring internal and external Sensors"));}
    digitalWrite(IandETempSwitch, HIGH);
    delay(100);
    sht20.begin();
     if (!sht20.begin()){ 
     if(DebugEnable){Serial.println(F("SHT25 Sensor Startup Failure!!!"));}
    delay(100);
     }
    //while (1);
  }else{
   digitalWrite(IandETempSwitch, HIGH);
   delay(400);
    sht20.measure_all();
    SHTIntTemp = sht20.tempC; SHTIntHumidity = sht20.RH;
  Serial.print((String)sht20.tempC + "°C  ");
  Serial.print((String)sht20.RH + " %RH  ");
  Serial.println((String)sht20.vpd() + " kPa VPD");
   delay(500);
    }
 digitalWrite(IandETempSwitch, HIGH);
 delay(400);
 
  if(DebugEnable){Serial.println(F("Sensors Off."));}
 digitalWrite(IandETempSwitch, LOW);
  }
 

///*********************************************************************************************
