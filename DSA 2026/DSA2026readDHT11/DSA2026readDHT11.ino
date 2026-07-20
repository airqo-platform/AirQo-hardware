#include <DFRobot_DHT11.h> 
DFRobot_DHT11 DHT; 
#define DHT11_PIN A3 
void setup(){ 
  pinMode(A8, OUTPUT); 
  digitalWrite(A8,1); 
  Serial.begin(115200); 
  } 

  void loop(){
     DHT.read(DHT11_PIN); 
     Serial.print("temp:"); 
     Serial.print(DHT.temperature); 
     Serial.print(" humi:"); 
     Serial.println(DHT.humidity); 
     delay(1000);
      }