# AirQo-hardware
The AirQo air qaulity monitoring device is an IoT connected device that provides realtime measurement of particualte matter concetrations and environmental readings (atmospheric pressure, relative humidity and temperature). the devices is powered by a Li-ion battery and connects to the iinternet over a 2g GSM network.

## Hardware design
The device is currently designed around an [Atmega2560](https://ww1.microchip.com/downloads/aemDocuments/documents/OTH/ProductDocuments/DataSheets/ATmega640-1280-1281-2560-2561-Datasheet-DS40002211A.pdf) microcontroller, that hosts the device firmware.
### block diagram 
(https://drive.google.com/file/d/12EIbsMi5HRd4mX6RLyXkjsDdcvxmGoht/view?usp=sharing)
### Sensors
*Particulate matter sensors [Plantower Pms5003]()
*Environmenatal sensors     [BME280](),[SHT25]()
*GPS                        [NEO6-GPS-module]()
*Communication              [Sim800l-GSM-GPRS]()
### CAD tools
list of tolls used to design the airqo monitor
pcb tools, 3d tools, links to design files here on github
f
## Firmware design
### developed with cpp and arduino framework
The firmware provides the airqo monitor with a number of features
*Realtime measurements of particualte matter concetrations
*Logging of data to local storage
*Over The Air Firmware Updates (OTA)
## License
This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details