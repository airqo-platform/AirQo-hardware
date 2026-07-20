#include <SPI.h>
#include <SD.h>
#include <DFRobot_DHT11.h>

DFRobot_DHT11 DHT;

// ------------------------
// Pin Definitions
// ------------------------
#define DHT11_PIN      A3
#define SD_CS_PIN      28
#define SD_POWER_PIN   30    // Active LOW

File dataFile;

void setup() {

  // Power the DHT11 (if required)
  pinMode(A8, OUTPUT);
  digitalWrite(A8, HIGH);

  // Power ON the SD Card
  pinMode(SD_POWER_PIN, OUTPUT);
  digitalWrite(SD_POWER_PIN, LOW);   // Active LOW = ON

  Serial.begin(115200);
  delay(500);

  Serial.println("Initializing SD Card...");

  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("ERROR: SD Card initialization failed!");
    while (1);
  }

  Serial.println("SD Card Ready.");

  // Create file and write header if it does not exist
  if (!SD.exists("dht11.txt")) {

    dataFile = SD.open("dht11.txt", FILE_WRITE);

    if (dataFile) {
      dataFile.println("Temperature(C),Humidity(%),Millis");
      dataFile.close();
    }
  }
}

void loop() {

  DHT.read(DHT11_PIN);

  int temperature = DHT.temperature;
  int humidity    = DHT.humidity;

  // Display readings
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C\tHumidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  // Save readings
  dataFile = SD.open("dht11.txt", FILE_WRITE);

  if (dataFile) {

    dataFile.print(temperature);
    dataFile.print(",");

    dataFile.print(humidity);
    dataFile.print(",");

    dataFile.println(millis());

    dataFile.close();

    Serial.println("Saved to dht11.txt");

  } else {

    Serial.println("Failed to open dht11.txt");

  }

  delay(1000);
}