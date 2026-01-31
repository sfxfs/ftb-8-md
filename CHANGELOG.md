# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.2] - 2026-01-31

### Added

- Add datasheet of VFD display

## [1.0.1] - 2026-01-30

### Added

- GitHub Workflow for automatic upload to ESP Component Registry

## [1.0.0] - 2026-01-30

### Added
- Initial release
- SPI driver for Futaba 8-MD-06INK VFD display
- Support for 8-digit alphanumeric display
- `ftb8md_device_register()` - Device initialization with optional hardware reset
- `ftb8md_show_string()` - Display text strings
- `ftb8md_set_dimming()` - Brightness control (0-255 levels)
- `ftb8md_set_dot()` - Decimal point control per digit
- `ftb8md_enter_standby()` - Power saving standby mode
- `ftb8md_clear_display()` - Clear all display contents
- `ftb8md_write_custom_char()` - Define custom 5x7 characters in CGRAM
- `ftb8md_set_addressed_char()` - Display custom characters
- `ftb8md_set_segment()` - Direct segment control
- Basic example project
- Custom character example project
- Digital clock example project
