# Changelog

All notable MSP430 firmware changes are documented here. The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/), and version numbers follow [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-09-04

### Added

- Initial WULPUS PRO shield firmware, derived from the WULPUS-pro MSP430 firmware.
- BioGAP host configuration and 804-byte SPI frame transfer with a 4-byte header and 400 16-bit samples.
- GPIO control for the preamplifier, VGA, low-pass filter, envelope detector, high-voltage converters, high-voltage multiplexer, and pulser.
- VGA RC precharge timing and gain-slope wiper configuration for fixed-gain and time-gain-compensation modes.
- Dedicated timer support for precise VGA precharge control.

### Changed
