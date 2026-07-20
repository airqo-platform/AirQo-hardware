#include "Arduino.h"
#include "pins.h"
#include <Wire.h>
#include <SdFat.h>
#include <DHT.h>
#include <Adafruit_HTU21DF.h>
#include <Adafruit_INA219.h>
#include <Adafruit_PM25AQI.h>
#include <SoftwareSerial.h>
#include <ArduinoJson.h>

#define SENSOR_WARMUP_MS  2000
#define READ_INTERVAL_MS  5000UL
#define LOG_INTERVAL_MS   60000UL
#define LOG_FILENAME      "airqo.csv"

DHT              dht(DHT_PIN, DHT11);
Adafruit_HTU21DF sht25;
Adafruit_INA219  ina219;
SdFat            sd;
SoftwareSerial   pmsSerial2(PMS2_RX_PIN, PMS2_TX_PIN);
Adafruit_PM25AQI pms1, pms2;

bool sdReady = false;

// Per-field running accumulator
struct Accum {
  float   sum   = 0;
  uint8_t count = 0;
  void  add(float v)  { if (!isnan(v)) { sum += v; count++; } }
  float avg()   const { return count ? sum / count : NAN; }
  void  reset()       { sum = 0; count = 0; }
} a_dht_temp, a_dht_hum,                          // DHT11
  a_sht_temp, a_sht_hum,                          // SHT25
  a_bus_v, a_shunt_mv, a_current_ma, a_power_mw,  // INA219
  a_pms1_pm1, a_pms1_pm25, a_pms1_pm10,           // PMS5003 sensor 1
  a_pms2_pm1, a_pms2_pm25, a_pms2_pm10;           // PMS5003 sensor 2

unsigned long lastReadTime = 0;
unsigned long lastLogTime  = 0;

// Latest PMS frames — updated every loop()
PM25_AQI_Data pmsData;
uint16_t pms1_pm1 = 0, pms1_pm25 = 0, pms1_pm10 = 0;
uint16_t pms2_pm1 = 0, pms2_pm25 = 0, pms2_pm10 = 0;
bool     pms1_valid = false, pms2_valid = false;

// Print value to SD file, or blank on NaN, then optional comma
static void csvField(SdFile& f, Accum& a, bool last = false) {
  float v = a.avg();
  if (!isnan(v)) f.print(v, 2);
  if (!last) f.print(',');
  a.reset();
}

void writeAvgToSD() {
  SdFile f;
  bool newFile = !sd.exists(LOG_FILENAME);
  if (!f.open(LOG_FILENAME, O_WRONLY | O_CREAT | O_AT_END)) {
    Serial.println(F("{\"sd_error\": \"failed to open log file\"}"));
    return;
  }
  if (newFile) {
    f.println(F("elapsed_s,"
                "dht_temp_c,dht_hum_pct,"
                "sht_temp_c,sht_hum_pct,"
                "bus_v,shunt_mv,current_ma,power_mw,"
                "pms1_pm1,pms1_pm25,pms1_pm10,"
                "pms2_pm1,pms2_pm25,pms2_pm10"));
  }
  f.print(millis() / 1000UL); f.print(',');
  csvField(f, a_dht_temp);   csvField(f, a_dht_hum);
  csvField(f, a_sht_temp);   csvField(f, a_sht_hum);
  csvField(f, a_bus_v);      csvField(f, a_shunt_mv);
  csvField(f, a_current_ma); csvField(f, a_power_mw);
  csvField(f, a_pms1_pm1);   csvField(f, a_pms1_pm25); csvField(f, a_pms1_pm10);
  csvField(f, a_pms2_pm1);   csvField(f, a_pms2_pm25); csvField(f, a_pms2_pm10, true);
  f.println();
  f.close();
}

void setup() {
  Serial.begin(115200);
  while (!Serial) { yield(); }

  pinMode(SENSOR_POWER_PIN, OUTPUT);
  digitalWrite(SENSOR_POWER_PIN, HIGH);
  pinMode(PMS_POWER_PIN, OUTPUT);
  digitalWrite(PMS_POWER_PIN, HIGH);
  delay(SENSOR_WARMUP_MS);

  dht.begin();

  if (!sht25.begin()) {
    JsonDocument err; err["error"] = "SHT25 not found - check wiring";
    serializeJsonPretty(err, Serial); Serial.println();
  }
  if (!ina219.begin()) {
    JsonDocument err; err["error"] = "INA219 not found - check wiring";
    serializeJsonPretty(err, Serial); Serial.println();
  }
  Serial3.begin(9600);
  pmsSerial2.begin(9600);
  pms1.begin_UART(&Serial3);
  pms2.begin_UART(&pmsSerial2);

  if (!sd.begin(SD_CS_PIN)) {
    JsonDocument err; err["error"] = "SD card init failed - check wiring";
    serializeJsonPretty(err, Serial); Serial.println();
  } else {
    sdReady = true;
  }

  lastReadTime = millis();
  lastLogTime  = millis();
}

void loop() {
  unsigned long now = millis();

  // Drain PMS serial buffers every iteration — keep latest frame
  if (pms1.read(&pmsData)) {
    pms1_pm1 = pmsData.pm10_env; pms1_pm25 = pmsData.pm25_env; pms1_pm10 = pmsData.pm100_env;
    pms1_valid = true;
  }
  if (pms2.read(&pmsData)) {
    pms2_pm1 = pmsData.pm10_env; pms2_pm25 = pmsData.pm25_env; pms2_pm10 = pmsData.pm100_env;
    pms2_valid = true;
  }

  // Read sensors every READ_INTERVAL_MS
  if (now - lastReadTime >= READ_INTERVAL_MS) {
    lastReadTime = now;

    float dht_temp = dht.readTemperature();
    float dht_hum  = dht.readHumidity();
    float sht_temp = sht25.readTemperature();
    float sht_hum  = sht25.readHumidity();
    float bus_v    = ina219.getBusVoltage_V();
    float shunt_mv = ina219.getShuntVoltage_mV();
    float cur_ma   = ina219.getCurrent_mA();
    float pow_mw   = ina219.getPower_mW();

    a_dht_temp.add(dht_temp);  a_dht_hum.add(dht_hum);
    a_sht_temp.add(sht_temp);  a_sht_hum.add(sht_hum);
    a_bus_v.add(bus_v);        a_shunt_mv.add(shunt_mv);
    a_current_ma.add(cur_ma);  a_power_mw.add(pow_mw);

    // Snapshot latest PMS readings then clear the valid flag
    bool     s1ok = pms1_valid, s2ok = pms2_valid;
    uint16_t s1_pm1 = pms1_pm1, s1_pm25 = pms1_pm25, s1_pm10 = pms1_pm10;
    uint16_t s2_pm1 = pms2_pm1, s2_pm25 = pms2_pm25, s2_pm10 = pms2_pm10;
    pms1_valid = false; pms2_valid = false;

    if (s1ok) { a_pms1_pm1.add(s1_pm1); a_pms1_pm25.add(s1_pm25); a_pms1_pm10.add(s1_pm10); }
    if (s2ok) { a_pms2_pm1.add(s2_pm1); a_pms2_pm25.add(s2_pm25); a_pms2_pm10.add(s2_pm10); }

    // Print live reading as pretty JSON
    JsonDocument doc;
    JsonObject dht_obj = doc["dht11"].to<JsonObject>();
    if (isnan(dht_temp)) dht_obj["temperature_c"] = nullptr; else dht_obj["temperature_c"] = dht_temp;
    if (isnan(dht_hum))  dht_obj["humidity_pct"]  = nullptr; else dht_obj["humidity_pct"]  = dht_hum;

    JsonObject sht_obj = doc["sht25"].to<JsonObject>();
    if (isnan(sht_temp)) sht_obj["temperature_c"] = nullptr; else sht_obj["temperature_c"] = sht_temp;
    if (isnan(sht_hum))  sht_obj["humidity_pct"]  = nullptr; else sht_obj["humidity_pct"]  = sht_hum;

    JsonObject ina_obj = doc["ina219"].to<JsonObject>();
    ina_obj["bus_voltage_v"]    = bus_v;
    ina_obj["shunt_voltage_mv"] = shunt_mv;
    ina_obj["current_ma"]       = cur_ma;
    ina_obj["power_mw"]         = pow_mw;

    JsonObject p1 = doc["pms5003_s1"].to<JsonObject>();
    if (s1ok) { p1["pm1_ug_m3"] = s1_pm1;  p1["pm25_ug_m3"] = s1_pm25; p1["pm10_ug_m3"] = s1_pm10; }
    else      { p1["pm1_ug_m3"] = nullptr; p1["pm25_ug_m3"] = nullptr; p1["pm10_ug_m3"] = nullptr; }

    JsonObject p2 = doc["pms5003_s2"].to<JsonObject>();
    if (s2ok) { p2["pm1_ug_m3"] = s2_pm1;  p2["pm25_ug_m3"] = s2_pm25; p2["pm10_ug_m3"] = s2_pm10; }
    else      { p2["pm1_ug_m3"] = nullptr; p2["pm25_ug_m3"] = nullptr; p2["pm10_ug_m3"] = nullptr; }

    serializeJsonPretty(doc, Serial);
    Serial.println();
  }

  // Log minutely average to SD card
  if (sdReady && (now - lastLogTime >= LOG_INTERVAL_MS)) {
    lastLogTime = now;
    writeAvgToSD();
  }
}