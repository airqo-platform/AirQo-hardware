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
