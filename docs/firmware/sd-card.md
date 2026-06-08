# SD Card Data Organization

---

The AirQo device logs sensor readings to a micro SD card in CSV format. File organization follows a time-based naming convention driven by the onboard Real Time Clock (RTC).

## File Naming Convention

Files are created and named based on:

- **Time of creation** — a new file is created at the start of each logging session
- **Time of writing** — timestamps embedded in each CSV row reflect the exact time data was written to the SD card

## CSV Data Format

Each row in the CSV log file contains the following fields:

| Field | Description |
|---|---|
| Timestamp | Date and time from RTC |
| PM2.5 | Particulate matter 2.5 μm reading (μg/m³) |
| PM10 | Particulate matter 10 μm reading (μg/m³) |
| Temperature | Ambient temperature (°C) |
| Humidity | Relative humidity (%) |
| Battery Voltage | Current battery level (V) |

## Transmission Backup

When transmission to the cloud fails, undelivered data entries are buffered to a backup file on the SD card. The firmware automatically retries transmission of buffered data when connectivity is restored.

---

## Related Pages

- [Firmware Overview](overview.md) — Overview of all firmware features
- [Firmware Configuration](configuration.md) — Configuring the device firmware
