# FAQs

Frequently asked questions about the AirQo Binos Air Monitor.

---

## General

**What does the AirQo monitor measure?**

The device measures Particulate Matter (PM2.5 and PM10), temperature, and humidity. It also tracks GPS location and battery voltage.

**How does the device connect to the internet?**

The device supports 2G GSM/GPRS, Wi-Fi, and LoRaWAN connectivity. The most common deployment uses 2G GSM with a local SIM card.

**How is the device powered?**

The device can be powered by a solar panel (5–12V input) or via USB (5V). It has an onboard rechargeable LiPo/Li-ion battery for backup power.

---

## Deployment

**How do I access my device's data?**

Log in to the [AirQo Analytics Platform](https://analytics.airqo.net), navigate to Analytics, and add your deployment location. See the [Deployment Guide](deployment.md) for the full walkthrough.

**How many sites can I monitor at once?**

You can select up to **4 sites** to view real-time data simultaneously on the dashboard.

**What height should I mount the device at?**

Mount between **2–5 metres** above ground level for representative ambient air quality readings.

---

## Maintenance

**How often should I clean the sensor?**

Clean the PM sensor inlet **monthly** using a soft brush or compressed air. Clean the solar panel surface monthly as well.

**How do I know if the battery needs replacing?**

Measure the battery voltage with a multimeter. If it reads below **3.5V** under load, replace the battery.

**The device stopped reporting data — what should I do?**

Run through the [Device Diagnosis](maintenance/diagnosis.md) checklist: check power, SIM card, GSM signal, and SD card. Connect via USB at 9600 baud to read serial diagnostic output.

---

## Firmware

**How do I update the firmware?**

See [Firmware Configuration](firmware/configuration.md) for step-by-step flashing instructions using Arduino IDE or PlatformIO.

**What firmware variants are available?**

Three variants are available:

- **AirQo Rev C** — standard production firmware (GSM/GPRS)
- **LoRaWAN v1** — for LoRaWAN network deployments
- **MQTT/ThingSpeak** — for MQTT-based integrations

---

## Related Pages

- [Deployment Guide](deployment.md)
- [Maintenance Overview](maintenance/index.md)
- [Device Diagnosis](maintenance/diagnosis.md)
