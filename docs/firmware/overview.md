# Firmware Overview

Robustly developed firmware enables efficient operation of the device to deliver the best data availability and quality to the users. This section covers the key firmware features and how they work.

---

## Key Firmware Features

### Sensor Sampling

The device obtains metrological (temperature and humidity) and other sensing parameters like particulate matter and battery voltage using different sensor modules. Parameters sampled include:

- PM2.5
- PM10
- Temperature
- Humidity
- Battery voltage

### SD Card Data Logging

The data obtained from the sensors is logged onto a micro SD card in CSV file format with the necessary parameters to be sent to the cloud storage. This ensures data is not lost even when network connectivity is unavailable.

See [SD Card Data Organization](sd-card.md) for details on how data files are structured.

### Automatic Time Tracking

Device firmware utilizes a Real Time Clock (RTC) to track the date and time with accuracy. A backup battery powers the RTC to provide time tracking even when the main battery is depleted.

The RTC time domain informs the following parameters:

- Local filenames stored on the SD card (based on time of creation)
- Device data timestamps (based on the time of writing to the SD-card file)

### Transmission Failure Handling

The device firmware incorporates data transmission failure handling by buffering data entries that have failed in transmission to a backup file and scheduling retransmission. This ensures no data loss during periods of poor connectivity.

---

## Firmware Variants

| Variant | Description | Folder |
|---|---|---|
| **AirQo Rev C** | Standard production firmware (GSM/GPRS) | [`airqo_rev_c_firmware`](https://github.com/airqo-platform/AirQo-hardware/tree/main/firmware/airqo_rev_c_firmware) |
| **LoRaWAN v1** | For LoRaWAN network deployments | [`lorawan_firmware_v1`](https://github.com/airqo-platform/AirQo-hardware/tree/main/firmware/lorawan_firmware_v1) |
| **MQTT / ThingSpeak** | For MQTT-based integrations | [`mqtt_thingspeak_bi_directional_communication`](https://github.com/airqo-platform/AirQo-hardware/tree/main/firmware/mqtt_thingspeak_bi_directional_communication) |

---

## Related Pages

- [SD Card Data Organization](sd-card.md)
- [Firmware Configuration](configuration.md)
- [Technical Specification](../device/technical-spec.md)
- [Deployment](../deployment.md)
