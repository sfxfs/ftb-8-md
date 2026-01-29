# Custom Character Example for Futaba 8-MD-06INK VFD Display

This example demonstrates how to define and display custom characters using the CGRAM (Character Generator RAM) feature.

## Features Demonstrated

- Defining custom 5x7 dot matrix characters
- Loading characters into CGRAM
- Displaying custom characters on the screen
- Creating animations with custom characters
- Mixing standard text with custom symbols

## Custom Characters Defined

| Index | Symbol | Description |
|-------|--------|-------------|
| 0 | ♥ | Heart |
| 1 | ☺ | Smiley face |
| 2 | ↑ | Up arrow |
| 3 | ↓ | Down arrow |
| 4 | □ | Battery empty |
| 5 | ▣ | Battery half |
| 6 | ■ | Battery full |
| 7 | ° | Degree symbol |

## Character Definition Format

Custom characters use a 5x7 dot matrix format. Each character is defined by 5 bytes, where each byte represents a column:

```
Column:  0    1    2    3    4
Row 0:   .    .    *    .    .
Row 1:   .    *    *    *    .
Row 2:   *    *    *    *    *
Row 3:   *    *    *    *    *
Row 4:   .    *    *    *    .
Row 5:   .    .    *    .    .
Row 6:   .    .    .    .    .
```

Each column byte: Bit 0 = Row 0, Bit 6 = Row 6

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

## Build and Flash

```bash
idf.py build
idf.py flash monitor
```

## Expected Output

The display will cycle through:

1. All 8 custom characters displayed
2. Heart animation filling the screen
3. "I ♥ ESP32" message
4. Temperature display with degree symbol (25°C)
5. Battery charging animation
6. Scanning arrow animation
7. Smiley greeting

## Creating Your Own Characters

Use the following template to design custom characters:

```c
/* My custom character */
static const uint8_t my_char[5] = {
    0x00,  /* Column 0: 0b0000000 */
    0x00,  /* Column 1: 0b0000000 */
    0x00,  /* Column 2: 0b0000000 */
    0x00,  /* Column 3: 0b0000000 */
    0x00   /* Column 4: 0b0000000 */
};
```

Convert your design to hex values by setting bits for each lit pixel (bit 0 = top row).
