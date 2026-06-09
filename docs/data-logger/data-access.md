# Data Access & Debugging

---

## Platform Data Access

![QR Code Data Access](images/qr-code-data-access.jpg)

The data logger is shipped with a **QR code** corresponding to the device identification details on the AirQo Analytics platform. Scan the QR code to link the device to your AirQo account and begin viewing data.

For platform setup instructions, see the [Deployment Guide](../deployment.md).

---

## Serial Data Access & Debugging

The data logger exposes a **USB-C port** that provides serial data access via a serial monitoring terminal on a computer. This is useful for verifying a successful connection and communication between the BAM1022 and the data logger.

### Steps

!!! danger "Disconnect 12V Power First"
    You **must** disconnect the 12V power cable from the BAM1022 before connecting the USB-C cable to the device and your computer for debugging and configuration.

1. Disconnect the 12V DC power cable from the BAM1022
2. Connect a USB-C cable from the data logger to your computer
3. Open a serial monitor terminal (e.g. Arduino IDE Serial Monitor, PuTTY, or `screen`)
4. Set baud rate to **9600 bps**
5. Observe the serial output to verify BAM1022 data is being received

### What to Look For

| Output | Meaning |
|---|---|
| Sensor readings appearing | BAM1022 connection is working correctly |
| No output / blank terminal | Check wiring connections (TX/RX/GND) |
| Garbled characters | Baud rate mismatch — check BAM1022 serial settings |
| Transmission confirmations | Data is being streamed to AirQo platform |

---

## Related Pages

- [Data Logger Overview](index.md)
- [Technical Specification](technical-spec.md)
- [Installation](installation.md)
