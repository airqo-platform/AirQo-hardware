# ESP32-C3 Mini — AirQo Hardware Design

A KiCad hardware design for the AirQo air quality monitor using the **Espressif ESP32-C3 Mini** module.

## Overview

| Property | Value |
|---|---|
| **MCU** | ESP32-C3 (RISC-V single-core, 160 MHz) |
| **Module** | ESP32-C3-MINI-1 |
| **Connectivity** | Wi-Fi 802.11 b/g/n · Bluetooth 5 (LE) |
| **Flash** | 4 MB (onboard) |
| **I/O** | GPIO, UART, SPI, I²C, ADC, USB Serial/JTAG |
| **Supply voltage** | 3.3 V |
| **PCB tool** | KiCad 7+ |

## Repository Structure

```
esp32-c3-mini/
├── esp32-c3-mini.kicad_pro     # KiCad project file
├── esp32-c3-mini.kicad_sch     # Schematic (main sheet)
├── esp32-c3-mini.kicad_pcb     # PCB layout
├── libraries/
│   ├── symbols/                # Custom schematic symbols (.kicad_sym)
│   └── footprints/             # Custom footprints (.pretty/)
├── fabrication/
│   ├── gerbers/                # Gerber + drill files for manufacture
│   ├── bom/                    # Bill of materials (CSV / interactive HTML)
│   └── assembly/               # Pick-and-place / CPL files
├── datasheets/                 # Component datasheets (PDFs)
└── README.md
```

## Getting Started

### Prerequisites

- [KiCad 7 or later](https://www.kicad.org/download/)
- ESP32-C3-MINI-1 symbol & footprint (available from [espressif/kicad-libraries](https://github.com/espressif/kicad-libraries))

### Opening the Project

```bash
# Clone the repo
git clone git@github.com:airqo-platform/AirQo-hardware.git
cd AirQo-hardware

# Open in KiCad
kicad hardware/esp32-c3-mini/esp32-c3-mini.kicad_pro
```

### Adding Espressif KiCad Libraries

1. Download or clone [espressif/kicad-libraries](https://github.com/espressif/kicad-libraries)
2. In KiCad → **Preferences → Manage Symbol Libraries** → add `espressif.kicad_sym`
3. In KiCad → **Preferences → Manage Footprint Libraries** → add the `espressif.pretty` folder

## Design Notes

- Target supply: **3.3 V** regulated (onboard LDO from 5 V USB or battery input)
- Antenna: Use the onboard PCB antenna — maintain **keep-out zone** per ESP32-C3-MINI-1 datasheet
- USB: Native USB Serial/JTAG on GPIO18/19 for flashing and debugging (no external UART chip required)

## Fabrication

Generate outputs from KiCad before submitting to a fab:

| Output | Location | Tool |
|---|---|---|
| Gerbers + drill | `fabrication/gerbers/` | KiCad PCB Editor → Plot |
| BOM | `fabrication/bom/` | KiCad → Tools → Generate BOM |
| Pick & Place | `fabrication/assembly/` | KiCad PCB Editor → Fabrication Outputs |

## Contributing

Raise a PR against the `hardware/esp32-c3-mini-design` branch. Follow the [AirQo hardware contributing guide](../../CONTRIBUTING.md).

## References

- [ESP32-C3-MINI-1 Datasheet](https://www.espressif.com/sites/default/files/documentation/esp32-c3-mini-1_datasheet_en.pdf)
- [ESP32-C3 Technical Reference Manual](https://www.espressif.com/sites/default/files/documentation/esp32-c3_technical_reference_manual_en.pdf)
- [KiCad Documentation](https://docs.kicad.org/)
