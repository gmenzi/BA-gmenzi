# WULPUS PRO Shield Hardware

This directory contains the Altium design sources and released manufacturing files for revision **1.0.0** of the WULPUS PRO shield for BioGAP-Ultra.

## Project Layout

| Path | Contents |
| --- | --- |
| [`wulpus_pro_shield/wulpus_pro_shield.PrjPcb`](wulpus_pro_shield/wulpus_pro_shield.PrjPcb) | Top-level Altium PCB project. |
| [`wulpus_pro_shield/`](wulpus_pro_shield/) | Schematic sheets, PCB layout, output jobs, harness definitions, and integrated library. |
| [`wulpus_pro_shield/libs/`](wulpus_pro_shield/libs/) | Project-specific schematic symbols, footprints, and 3D models. |
| [`wulpus_pro_shield/docs/`](wulpus_pro_shield/docs/) | Released fabrication and assembly package. |

## Main Hardware Blocks

- **MSP430FR5043** ultrasound controller and 12-bit, 8 MS/s acquisition subsystem.
- **HV2707** 16-channel high-voltage multiplexer.
- **IXDD604** high-voltage pulser driver and MD0100 transmit/receive protection.
- **AD8338** variable-gain amplifier with **TPL0501** digital control for fixed gain or time-gain compensation.
- **LT3463** positive/negative high-voltage supply.
- BioGAP-compatible board-to-board connectors for power, SPI, handshaking, and MSP430 programming.

## Released Manufacturing Files

The release package is under [`wulpus_pro_shield/docs/`](wulpus_pro_shield/docs/):

- `WULPUS_pro_schematics.PDF` — eight-page schematic set.
- `WULPUS_pro_shield_assembly.PDF` — top and bottom assembly drawings.
- `BOM/BOM_WULPUS_pro_BioGAP_shield.xlsx` — bill of materials.
- `Gerber/` — PCB fabrication layers.
- `NC Drill/` — drill files and reports.
- `Pick Place/` — placement data for assembly.

Before ordering boards, confirm that the BOM, placement variant, stack-up, drill data, and Gerber set match the intended assembly. 

## Editing the Design

1. Open `wulpus_pro_shield/wulpus_pro_shield.PrjPcb` in Altium Designer.
2. Compile the project and resolve any missing library references before editing.
3. Regenerate fabrication outputs with the supplied output job after a design change.
4. Record released hardware changes in the repository-level [`CHANGELOG.md`](../CHANGELOG.md).


## License

The PCB design files are licensed under the [Solderpad Hardware License v0.51](LICENSE).
