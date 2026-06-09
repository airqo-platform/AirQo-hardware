# Device Diagnosis

This page helps field engineers and technicians diagnose common faults with the AirQo Binos Air Monitor.

---

## Diagnostic Checklist

Before escalating an issue, run through the following checklist:

- [ ] Device is powered on (LED indicator active)
- [ ] Solar panel is clean and correctly oriented
- [ ] Battery voltage is above 3.5V (check with multimeter)
- [ ] SIM card is inserted and active
- [ ] GSM signal available at site (check with mobile phone)
- [ ] SD card is inserted and not full
- [ ] Sensor inlet is free of obstructions

---

## Common Issues

| Symptom | Likely Cause | Resolution |
|---|---|---|
| No power / device off | Depleted battery, faulty charging | Check solar panel orientation; measure battery voltage; replace if below 3.3V |
| No data on platform | GSM connectivity failure | Verify SIM credit/data; check signal strength; inspect antenna connection |
| Incorrect readings | Sensor contamination or failure | Clean sensor inlet; inspect PMS5003 for blockages; replace sensor if persistent |
| SD card not logging | Full card or card failure | Replace micro SD card; format as FAT32 |
| Device not transmitting | Firmware crash or config error | Connect via USB, open serial monitor at 9600 baud; reflash firmware if needed |
| RTC time drift | Backup battery depleted | Replace RTC backup battery (CR2032) |

---

## Serial Diagnostic Output

Connect the device via USB and open a serial monitor at **9600 baud** to view real-time diagnostic output:

- Sensor readings per sampling cycle
- Transmission status (success / failure)
- SD card write confirmation
- RTC timestamp

---

## Related Pages

- [Maintenance Overview](index.md)
- [Maintenance Toolkit](toolkit.md)
- [System Maintenance](system.md)
- [Firmware Configuration](../firmware/configuration.md)
