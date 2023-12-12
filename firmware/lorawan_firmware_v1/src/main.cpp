#include <Arduino.h>
#include <HardwareSerial.h>
#include "PMS.h"
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <SparkFunBQ27441.h>
/*pins connected to devices for control and digital reads
ERROR_LED_PIN is used to indicate device status 
.{
  to do
  blink different ways to indicate 
  1. failed sensor initialization , senosr1&2
  2. battery low warning (interesting)
  3. Seaarching for LORAWAN gateway and conection
  4...etc
}
*/
#define BATT_STATUS_PIN   PB5
#define ERROR_LED_PIN     PB13

#define SENSOR_1_SET_PIN  PB1
#define SENSOR_2_SET_PIN  PB0

#define GPS_ENABLE_PIN    PA15
#define GPS_RESET_PIN     PB12

#define CHARGER_EN1_PIN   PB4 // enable pin for usb charger modes en1
#define CHARGER_EN2_PIN   PB3 // enable pin for usb charger modes en2

#define SD_NSS_PIN        PA4
#define SEALEVELPRESSURE_HPA (1013.25)

const uint16_t BATTERY_CAPACITY=4300; //4300mAh batteries from 
const uint16_t TAPER_CURRENT = 60; //current at which the charger stops chargin the battery in mA
const uint16_t TERMINATE_VOLTAGE = 3000;  //lowest operational voltage in mV (3V)

float sensor_data_payload[4];
uint8_t sensor_error=0;
String data_payload,hex_data_payload;
unsigned long previousMillis = 0;   // will store the last time LED was updated
const long fadeInterval = 1000;     // interval at which to fade (milliseconds)
int fadeIncrement = 10;        // adjust the fade increment as needed
int brightness = 0;  // how bright the LED is

//for hardware serial ports 3 to 5, we have to declare them here as shown
HardwareSerial Serial3(USART3);   //  gps serial port
HardwareSerial Serial4(UART4);    //  pm sensor1 serial port
HardwareSerial Serial5(UART5);    //  pm sensor2 serial port

Adafruit_BME280 bme; // I2C for bme temp humidity sensor

PMS pm_sensor1(Serial4);
PMS pm_sensor2(Serial5);
PMS::DATA sensor_data;
void ScanI2c();
void test_bme();
void test_rak11720();
void stringToHexString(const String& input, String& output) ;
void sample_pm_sensors(uint8_t sample_count);
void read_battery_stats();
void fadeLED(int interval,int fadesCount);

void setup()
{
  pinMode(ERROR_LED_PIN, OUTPUT);  // Set the LED pin as an output
  pinMode(SENSOR_1_SET_PIN,OUTPUT);
  pinMode(SENSOR_2_SET_PIN,OUTPUT);
  pinMode(GPS_ENABLE_PIN,OUTPUT);
  pinMode(CHARGER_EN1_PIN,OUTPUT);
  pinMode(CHARGER_EN2_PIN,OUTPUT);

  digitalWrite(GPS_ENABLE_PIN,LOW); //gps is initially switched off
  /*
    | EN2 | EN1 |MAXIMUM INPUT CURRENT TO BATTERY CHARGER 
    | 0   | 0   |100mA .USB100 mode
    | 0   | 1   |500mA .USB500 mode
    | 1   | 0   |set by an external resistor from ILIM pin to GND/vss
    | 1   | 1   |standby (USB suspend mode)
  */
  digitalWrite(CHARGER_EN1_PIN,LOW); 
  digitalWrite(CHARGER_EN2_PIN,HIGH);

  Serial.begin(115200);             //debugging over usb connection 
  Serial2.begin(115200);            //rakk11720 serial
  Serial3.begin(9600);              //gps l80 serial port
  Serial4.begin(9600);              //pm sensor 1 serial
  Serial5.begin(9600);              //pm sensor 2 serial
  Wire.begin();                     //begin 12c communcation
  delay(5000);
  ScanI2c();                        //scan i2c interface for sensors connected.
  if(!lipo.begin())
  {
    // If communication fails, print an error message and loop forever.
    Serial.println("Error: Unable to communicate with BQ27441.");
    Serial.println("  Check wiring and try again.");
    Serial.println("  (Battery must be plugged into Battery Babysitter!)");
    while (1) ;
  }
  lipo.setCapacity(BATTERY_CAPACITY); // battery capacity 
  test_bme();
  delay(5000);
}

void loop()
{
  sample_pm_sensors(60);
  if(Serial3.available())
  {
    while(Serial3.available())
    {
      Serial.print(char(Serial3.read()));
    }
    //read_battery_stats();
    
  }
  test_rak11720();
  if(millis()-previousMillis>10000)
  {
    read_battery_stats();
    previousMillis=millis();
  }
  //delay(1000);
}
void read_battery_stats()
{
  // Read battery stats from the BQ27441-G1A
  unsigned int soc = lipo.soc(FILTERED);  // Read state-of-charge (%)
  unsigned int volts = lipo.voltage(); // Read battery voltage (mV)
  float current = lipo.current(AVG); // Read average current (mA)
  unsigned int fullCapacity = lipo.capacity(FULL); // Read full capacity (mAh)
  unsigned int capacity = lipo.capacity(REMAIN); // Read remaining capacity (mAh)
  int power = lipo.power(); // Read average power draw (mW)
  int health = lipo.soh(); // Read state-of-health (%) // Now print out those values:
  String toPrint = String(soc) + "% | ";
  toPrint += String(volts) + " mV | ";
  toPrint += String(current) + " mA | ";
  toPrint += String(capacity) + " / ";
  toPrint += String(fullCapacity) + " mAh | ";
  toPrint += String(power) + " mW | ";
  toPrint += String(health) + "%";
  Serial.println(toPrint);
  lipo.status();
}
void fadeLED(int interval,int fadesCount) 
{
  analogWrite(ERROR_LED_PIN,brightness);
  brightness=brightness+fadeIncrement;
  if(brightness<=0||brightness>=255)
  {
    fadeIncrement=-fadeIncrement;
  }
  delay(interval);
}
void ScanI2c()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++) {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.

    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);

      nDevices++;
    }
    else if (error == 4) {
      Serial.print("Unknown error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found");
  else
    Serial.println("done");
  
  //delay(5000);           // wait 5 seconds for next scan
}
void test_bme()
{
  unsigned status;
    // default settings
    status = bme.begin(0x76);  
    // You can also pass in a Wire library object like &Wire2
    // status = bme.begin(0x76, &Wire2)
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
        Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
        Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
        Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
        Serial.print("        ID of 0x60 represents a BME 280.\n");
        Serial.print("        ID of 0x61 represents a BME 680.\n");
        while (1) delay(10);
    }
    
    Serial.println("-- Default Test --");
}
void test_rak11720()
{
  if(Serial.available())
  {
    while(Serial.available())
    {
      Serial2.println(Serial.readString());
    }
    
  }
  if(Serial2.available())
  {
    while (Serial2.available())
    {
      /* code */
      Serial.println(Serial2.readString());
    }
  }
}
void sample_pm_sensors(uint8_t sample_count)
{
  float sensor1averagedPm1,sensor1averagedPm25,sensor1averagedPm10;
  float sensor2averagedPm1,sensor2averagedPm25,sensor2averagedPm10;
  bool pms_sensor_error[2]={false,false};
  digitalWrite(SENSOR_1_SET_PIN,HIGH);
  digitalWrite(SENSOR_2_SET_PIN,HIGH);
  for(uint8_t samples_taken=0;samples_taken<=sample_count;samples_taken++)
  {
    if (pm_sensor1.readUntil(sensor_data))
    {
      sensor1averagedPm1+=sensor_data.PM_AE_UG_1_0;
      sensor1averagedPm25+=sensor_data.PM_AE_UG_2_5;
      sensor1averagedPm10+=sensor_data.PM_AE_UG_10_0;      
      Serial.println(F("____________Sensor1__________"));
      Serial.print(F("sensor1 PM1     (ug/m3):"));
      Serial.println(sensor_data.PM_AE_UG_1_0);
      Serial.print(F("sensor1 PM2.5   (ug/m3):"));
      Serial.println(sensor_data.PM_AE_UG_2_5);
      Serial.print(F("sensor1 PM10.0  (ug/m3):"));
      Serial.println(sensor_data.PM_AE_UG_10_0);
      Serial.println(F(" "));
    }
    else{
      //Serial.println(F("Sensor1 data read error"));
      pms_sensor_error[0]=true;
      samples_taken=sample_count;
    }
    if (pm_sensor2.readUntil(sensor_data))
    {
      sensor2averagedPm1+=sensor_data.PM_AE_UG_1_0;
      sensor2averagedPm25+=sensor_data.PM_AE_UG_2_5;
      sensor2averagedPm10+=sensor_data.PM_AE_UG_10_0;      
      Serial.println(F("____________Sensor2__________"));
      Serial.print(F("sensor2 PM1     (ug/m3):"));
      Serial.println(sensor_data.PM_AE_UG_1_0);
      Serial.print(F("sensor2 PM2.5   (ug/m3):"));
      Serial.println(sensor_data.PM_AE_UG_2_5);
      Serial.print(F("sensor2 PM10.0  (ug/m3):"));
      Serial.println(sensor_data.PM_AE_UG_10_0);
      Serial.println(F(" "));
    }
    else{
      //Serial.println(F("Sensor2 data read error"));
      pms_sensor_error[1]=true;
      samples_taken=sample_count;
    }
  }
  if(pms_sensor_error[0]==true&&pms_sensor_error[1]==true)
  {
    sensor_data_payload[0]=sensor1averagedPm25/sample_count;
    sensor_data_payload[1]=sensor1averagedPm10/sample_count;
    sensor_data_payload[2]=sensor2averagedPm25/sample_count;
    sensor_data_payload[3]=sensor2averagedPm10/sample_count;
    for(uint8_t index=0;index<4;index++)
    {
      data_payload+=String(sensor_data_payload[index]);
      if(index<4)
      {
        data_payload+="#";
      }
    }   
  }
  else
  {
    Serial.println(F("Sensor read error"));
  }
  data_payload+=String(lipo.power())+","+String(bme.readTemperature())+","+String(bme.readHumidity());
  data_payload+=",\"";
  data_payload+=String(lipo.soc())+","+String(lipo.voltage())+",";
  data_payload+="\"";
  Serial.println(data_payload);
  sensor1averagedPm1=sensor2averagedPm1=sensor1averagedPm10=sensor1averagedPm25=sensor2averagedPm10=sensor2averagedPm25=0.00;
  stringToHexString(data_payload,hex_data_payload);
  Serial.println(hex_data_payload);
  hex_data_payload="";
  data_payload="";
}
// Function to convert a string to a hex string
void stringToHexString(const String& input, String& output) {
  for (size_t i = 0; i < input.length(); i++) {
    char hex[3];
    sprintf(hex, "%02X", input.charAt(i));
    output += hex;
  }
}
