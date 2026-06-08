# Firmware Configuration

---

This page covers how to configure the AirQo device firmware for your deployment environment.

## Configuration Parameters

The firmware exposes several configurable parameters that can be set before flashing or via runtime configuration:

| Parameter | Description |
|---|---|
| **Network APN** | GSM/GPRS APN for your SIM provider |
| **Server endpoint** | URL of the AirQo data ingestion API |
| **Device ID** | Unique identifier for this monitor unit |
| **Sampling interval** | How often sensor readings are taken (default: 60–90s) |
| **Transmission interval** | How often data is sent to the cloud |
| **RTC sync** | Enable/disable time sync via GSM network |

## Flashing the Firmware

The firmware is developed with C++ using the Arduino framework.

**Prerequisites:**

- [Arduino IDE](https://www.arduino.cc/en/software) or [PlatformIO](https://platformio.org/)
- USB cable (Micro USB)
- Board: **Arduino Mega (ATMEGA2560)**

**Steps:**

1. Clone the repository and open the relevant firmware project folder
2. Configure parameters in the config header file
3. Connect the device via USB
4. Select board: **Arduino Mega (ATMEGA2560)**
5. Upload

!!! warning
    Ensure the device is powered off from its main supply before connecting via USB for flashing.

## Firmware Variants

| Variant | Folder |
|---|---|
| AirQo Rev C (standard) | [`airqo_rev_c_firmware`](https://github.com/airqo-platform/AirQo-hardware/tree/main/firmware/airqo_rev_c_firmware) |
| LoRaWAN v1 | [`lorawan_firmware_v1`](https://github.com/airqo-platform/AirQo-hardware/tree/main/firmware/lorawan_firmware_v1) |
| MQTT / ThingSpeak | [`mqtt_thingspeak_bi_directional_communication`](https://github.com/airqo-platform/AirQo-hardware/tree/main/firmware/mqtt_thingspeak_bi_directional_communication) |

---

## Related Pages

- [Firmware Overview](overview.md) — Key firmware features
- [SD Card Data Organization](sd-card.md) — SD card data structure
