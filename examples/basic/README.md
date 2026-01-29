# Basic Example for Futaba 8-MD-06INK VFD Display

This example demonstrates basic operations of the Futaba 8-MD-06INK VFD display driver.

## Features Demonstrated

- Display initialization with SPI
- Displaying text strings
- Brightness (dimming) control
- Decimal point control
- Standby mode
- Scrolling text effect

## Hardware Required

- ESP32 development board
- Futaba 8-MD-06INK VFD display module
- Connecting wires

## Pin Assignment

| VFD Pin | ESP32 GPIO | Description |
|---------|------------|-------------|
| DIN     | GPIO23     | SPI MOSI |
| CLK     | GPIO18     | SPI Clock |
| CS      | GPIO5      | Chip Select |
| RST     | GPIO4      | Reset (optional) |
| VCC     | 3.3V/5V    | Power |
| GND     | GND        | Ground |

> **Note:** Modify the pin definitions in `main.c` according to your hardware setup.

## Build and Flash

1. Set up ESP-IDF environment
2. Configure the project (optional):
   ```bash
   idf.py menuconfig
   ```
3. Build the project:
   ```bash
   idf.py build
   ```
4. Flash to the device:
   ```bash
   idf.py flash monitor
   ```

## Expected Output

The display will cycle through the following demonstrations:

1. Display "HELLO"
2. Display "ESP32" at full brightness
3. Display "12345678" with decimal points
4. Brightness fade in/out effect
5. Standby mode demonstration
6. Scrolling text effect

## Troubleshooting

- If the display doesn't respond, check the SPI connections
- If characters appear garbled, verify the SPI mode and bit order
- If the display is dim, try increasing the dimming level
