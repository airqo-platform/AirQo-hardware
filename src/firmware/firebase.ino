void fetchUpdate(String fileName, String token)
{
  String fbr = "/v0/b/airqo-hardware-01.appspot.com/o/config.txt?alt=media";
  if(DebugEnable){Serial.print(F("Connecting to "));
  Serial.println(FireBaseServer);}

GPRSconnect();
//Client start
TinyGsmClientSecure  client(modem);
//const int      port = 80;//443
//End of Client start
  if (!client.connect("firebasestorage.googleapis.com", 443)) {
    if(DebugEnable){Serial.println(F("failed"));}
    client.stop();
  
  modem.gprsDisconnect();
   // delay(10000);
    return;
  }

  if(DebugEnable){Serial.println(F("Creating firmware.bin..."));
  Serial.println(F("Performing HTTP GET request..."));}
  //client.print(String("GET ") + resource + " HTTP/1.1\r\n");
  client.println(String("GET ") + fbr);
  client.println(String("Host: firebasestorage.googleapis.com"));
  client.println(String("Content-Type: text/plain"));
  //client.println(String("Content-Disposition: ") + "attachment; filename=firmware.bin" );
  client.println("Connection: close");
  client.println();
 
  uint32_t timeout = millis();
  
 // myFile = SD.open("firmware.bin", FILE_WRITE);
  while (client.connected() && millis() - timeout < 10000L) {
    if(client.available())
    {
      
      if(DebugEnable){Serial.print((char)client.read());}
     
    }
  }
  
  client.stop();
  Serial.println(F("Server disconnected"));
  modem.gprsDisconnect();
  Serial.println(F("GPRS disconnected"));
  //updateInitialization(); //flash the code to the mcu after reset.
}


void FirebaseUpload(String data)
{
  const String FirebaseAuth="FHhUlDab1cKWpZvDmpbUcNl3Jd3DoJkAmLTulFkJ";
  const char FirebaseHost[]="airqo-hardware-01-default-rtdb.firebaseio.com";
  
//  if(!modem.isNetworkConnected())
//    {
//        modem.waitForNetwork(20000L,true);
//    }
    if(modem.isNetworkConnected())
    {
        if(!modem.isGprsConnected())
        {
            modem.gprsConnect(apn);
        }
        if(modem.isGprsConnected())
        {
            //magic happens here
            TinyGsmClientSecure client(modem);
            HttpClient http(client,FirebaseHost,443);
            http.connect(FirebaseHost,443);
            String contentType = "application/json";
            if(http.connected())
            {
                
                http.post("/"+(DevName)+".json?auth="+FirebaseAuth,"application/json",data); //create a new entery to the 
                Serial.println(http.responseStatusCode());
                Serial.println(http.responseBody());
            }
            http.endRequest();
            http.stop();

        }
        modem.gprsDisconnect();
    } 
}
