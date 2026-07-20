#ifndef PINS_H
#define PINS_H

#define SENSOR_POWER_PIN A8  // MOSFET gate — powers DHT11, SHT25, INA219
#define DHT_PIN          A3  // DHT11 data pin
#define SD_CS_PIN        28  // SD card chip select

#define PMS_POWER_PIN    A2  // MOSFET gate — powers both PMS5003 sensors
#define PMS2_RX_PIN      12  // SoftwareSerial RX — PMS5003 sensor 2 TX
#define PMS2_TX_PIN      13  // SoftwareSerial TX — PMS5003 sensor 2 RX

#endif  // PINS_H