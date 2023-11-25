#include <HardwareSerial.h>
#define TINY_GSM_MODEM_SIM800

#include <PubSubClient.h> 
#include <TinyGsmClient.h> 
#include <ArduinoJson.h> 

#define Serialgsm Serial1
const char apn[] = "internet"; // apn for uganda 
const char* broker = "mqtt3.thingspeak.com"; 
const char clientID[] = ""; 
const char mqttUserName[] = ""; // replace these sections with your mqtt credentials 
const char mqttpassword[] = ""; 


uint32_t lastReconnectAttempt = 0; 
unsigned long last_subscriptiontime = 0; 
unsigned long subscriptionInterval = 30000; 

unsigned long last_publishtime = 0; 
unsigned long publishInterval = 30000; 

TinyGsm modem(Serialgsm); 
TinyGsmClientSecure client(modem); 
PubSubClient mqtt(client);


void setup() {
  Serial.begin(115200);
  Serialgsm.begin(115200,SERIAL_8N1,26,27);
  gsm_setup(); 
  mqtt.setServer(broker,8883);
  mqtt.setCallback(mymqttcallback); 
  mqtt.setBufferSize(2048);
}

void loop() {
  // assure internet is still connected 
    if (!modem.isGprsConnected()) {
      if (!modem.gprsConnect(apn)) {
        Serial.println(" fail");
        delay(10000);
        return;
      }
      if (modem.isGprsConnected()) { Serial.println("GPRS reconnected"); }
    }
    mqttConnect();
      mqtt.loop();
  }
mymqttcallback(1233434, "fdfdf", )

void mymqttcallback(char* topic, byte* payload, unsigned int length){
  // The call back functions informs the subscriber when a message is received 
  // handle received message
  Serial.print("Message on topic: ");
  Serial.print(topic);
  Serial.println(" received"); 
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println("  ");
  //--- when data is received then:
  DynamicJsonDocument doc(1024);// break down the received payload by the subscriber inorder to access the data set to the individual feeds 
  DeserializationError error = deserializeJson(doc,payload,length);
  if(!error){
    int field_one_value = doc["field1"];
    Serial.print("field_one_value =");
    Serial.println(field_one_value); 
  }
}

void mqttSubscribe(long pubChannelID){
  String topicString =  "channels/"+ String(pubChannelID) + "/subscribe"; 
  mqtt.subscribe(topicString.c_str());
}

// publish to a channel 
void mqttPublish(long pubChannelID, String message){ 
  String topicString = "channels/"+ String(pubChannelID) + "/publish"; 
  mqtt.publish(topicString.c_str(),message.c_str()); 
}
// JAwCCzUKJzIQAQc2DDAwJDI
void mqttConnect() {
  while ( !mqtt.connected() ){
    Serial.println("Attempting mqtt connection"); 
    if ( mqtt.connect(clientID,mqttUserName,mqttpassword) ) {
      Serial.print( "MQTT connection successful " );
      mqttSubscribe(1651527);
     // mqttPublish(1651527,(String("&field1=")+String(30))); 

    } 
    else {
      Serial.println( "mqtt connection failure" );
      Serial.println( mqtt.state() );
      delay(1000 );
    }
  }
      Serial.println( "MQTT connection successful"); 
      unsigned long currentMillis = millis();
    if(currentMillis - last_subscriptiontime >=subscriptionInterval){
      last_subscriptiontime = currentMillis; 
       mqttSubscribe(1651527); 
    }    
    if(currentMillis - last_publishtime >= publishInterval){
       mqttPublish(1651527,(String("&field1=")+String(30)+String("&field2=")+String(15)+String("&field3=")+String(10)+String("&field4=")+String(12)));
      last_publishtime = currentMillis;
    }   
}


void gsm_setup(){
if(!modem.init()){
        Serial.println("Failed to initialise modem.");
      return; 
      }
    String modem_name = modem.getModemName(); 
    Serial.print("modem_name is: ");
    Serial.println(modem_name);
    String modem_information = modem.getModemInfo(); 
    Serial.print("modem information:  "); 
    Serial.println(modem_information);
    Serial.println("Modem now waiting for network"); 
    if(!modem.waitForNetwork(600000L,true)){delay(6000);return;}
    if(modem.isNetworkConnected()){Serial.println("modem connected to the network");}
    if(!modem.gprsConnect(apn)){
      Serial.println("failed to connect to the apn"); 
      delay(1000); 
      return;
    }
    Serial.println("check point 2");
   if(modem.isGprsConnected()){
      Serial.println("gprs is connected"); 
   } 
    else{Serial.println("No gprs connection");}  
}


