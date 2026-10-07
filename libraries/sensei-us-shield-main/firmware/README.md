# WULPUS PRO MSP430 Firmware

This directory contains the shield-side firmware for the **MSP430FR5043** ultrasound controller mounted on the WULPUS PRO BioGAP shield.

The MSP430 configures ultrasound timing, pulse generation, transmit/receive channel routing, the receive chain, envelope detection, and power domains. It operates as an SPI peripheral: the BioGAP-Ultra nRF5340 sends acquisition settings and receives the completed ultrasound frames.

## Project

The Code Composer Studio project is located at [`wulpus_msp430_firmware/`](wulpus_msp430_firmware/). Its key sources are:

| Path | Purpose |
| --- | --- |
| `main.c` | Configuration reception, acquisition loop, frame headers, and event callbacks. |
| `system_pre_init.c` | MSP430 clock and early-system initialization. |
| `wulpus/us_spi.*` | DMA-backed SPI interface to the BioGAP host. |
| `wulpus/us_hv_mux.*` | 16-channel high-voltage multiplexer control. |
| `wulpus/wulpus_sys.*` | Configuration decoding, GPIO mapping, power control, VGA/TGC, filter, envelope detector, and pulser control. |

## Toolchain and Dependencies

The checked-in project metadata targets:

- TI Code Composer Studio **11.0**.
- TI MSP430 Code Generation Tools **21.6.0.LTS**.
- MSP430FR5043.
- An MSP-FET-compatible programmer/debugger.

The complete repository includes the `driverlib/`, `uslib/`, and `targetConfigs/` directories required by the CCS project. Keep these directories alongside the project sources when cloning, copying, or importing the firmware project.

The upstream [MSP430 toolchain and flashing guide](https://github.com/pulp-bio/wulpus/blob/main/fw/msp430/how_to_setup_msp_430_toolchain_and_flash.md) provides the detailed CCS setup procedure.

## Build and Flash

1. Install Code Composer Studio with **MSP430 ultra-low-power MCU** support.
2. Confirm that the included `driverlib/`, `uslib/`, and `targetConfigs/` directories are present inside `wulpus_msp430_firmware/`.
3. In CCS, select **File → Open Projects from File System** and choose `firmware/wulpus_msp430_firmware`.
4. Select **Project → Build Configurations → Set Active → Debug** or **Release**.
5. Build the project.
6. With power disconnected, attach the MSP-FET-compatible programmer to the shield programming interface.
7. Power the BioGAP/shield stack and program `wulpus_msp430_firmware.out` from CCS.

## BioGAP and BioGUI Integration

The MSP430 firmware is only the shield-side part of the acquisition chain:

1. [BioGUI](https://github.com/pulp-bio/biogui) creates the WULPUS PRO configuration and sends it to BioGAP.
2. BioGAP forwards the configuration to the MSP430 over SPI.
3. The MSP430 acquires the selected transmit/receive sequence and returns an 804-byte frame: a 4-byte header followed by 400 signed 16-bit samples.
4. BioGAP transports the frame over BLE, and BioGUI's `interface_biogapultra_wulpus_pro.py` implementation reassembles, decodes, displays, and records it.

Use a BioGAP firmware revision and BioGUI revision with matching WULPUS PRO packet definitions.

## Release History

See [`wulpus_msp430_firmware/CHANGELOG.md`](wulpus_msp430_firmware/CHANGELOG.md).

## License

The project contains sources under their respective file-header licenses, primarily Apache-2.0 and BSD-style terms. TI DriverLib, USSlib, and other imported dependencies retain their upstream licenses; review the license notices within those directories.
