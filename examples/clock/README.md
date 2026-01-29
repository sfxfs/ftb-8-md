# Digital Clock Example for Futaba 8-MD-06INK VFD Display

This example demonstrates how to create a digital clock using the VFD display.

## Features Demonstrated

- Time display in 24-hour format (HH.MM.SS)
- Time display in 12-hour format (HH.MM AM/PM)
- Date display (DD.MM.YYYY)
- Blinking colon using decimal points
- Automatic mode switching

## Display Modes

The clock cycles through three display modes every 10 seconds:

1. **24-Hour Time**: `12.34.56` (with blinking colons)
2. **12-Hour Time**: `12.34PM` (with blinking colon)
3. **Date**: `30.01.2024`

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

## Adding Real Time

This example uses a sample time. To use real time, you can:

### Option 1: Add SNTP Support

```c
#include "esp_sntp.h"

void init_sntp(void)
{
    esp_sntp_setoperatingmode(SNTP_OPMODE_POLL);
    esp_sntp_setservername(0, "pool.ntp.org");
    esp_sntp_init();
}
```

### Option 2: Add RTC Module

Connect an external RTC module (like DS3231) and use its time.

### Option 3: Manual Time Setting

Set the time through a serial command or button interface.

## Customization

### Change Time Format

Modify the `display_time_24h()` or `display_time_12h()` functions to customize the time format.

### Change Date Format

Modify the `display_date()` function. For US format (MM/DD/YY):

```c
snprintf(date_str, sizeof(date_str), "%02d%02d%04d",
         timeinfo->tm_mon + 1, timeinfo->tm_mday, 
         timeinfo->tm_year + 1900);
```

### Adjust Mode Switching Interval

Change the `mode_counter` threshold in the main loop:

```c
if (mode_counter >= 40) {  /* 40 * 500ms = 20 seconds */
```

## Power Saving

The VFD display consumes more power than LCD displays. Consider:

1. Reducing brightness at night using `ftb8md_set_dimming()`
2. Using standby mode during inactive periods
3. Implementing motion/light sensors to wake the display
