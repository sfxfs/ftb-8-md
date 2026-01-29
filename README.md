# Futaba 8-MD-06INK VFD Display Driver

ESP-IDF driver for the **Futaba 8-MD-06INK** Vacuum Fluorescent Display (VFD) module. This 8-digit VFD display communicates via SPI and supports customizable characters and brightness control.

## Features

- 8-digit alphanumeric display support
- SPI communication (up to 500 kHz)
- Adjustable brightness (dimming) control
- Custom character definition (CGRAM)
- Decimal point control for each digit
- Standby mode for power saving
- Direct segment control

## Hardware Connection

| VFD Pin | ESP32 Pin | Description |
|---------|-----------|-------------|
| VCC     | 3.3V/5V   | Power supply |
| GND     | GND       | Ground |
| DIN     | MOSI      | SPI Data In |
| CLK     | SCLK      | SPI Clock |
| CS      | GPIO (CS) | Chip Select (Active Low) |
| RST     | GPIO (RST)| Reset (Optional) |

> **Note:** The SPI interface uses LSB-first bit order with CPOL=1, CPHA=1 (SPI Mode 3).

## Installation

### Using ESP Component Registry (Recommended)

```bash
idf.py add-dependency "ftb-8-md"
```

### Manual Installation

Copy the `ftb-8-md` folder to your project's `components` directory.

## Quick Start

```c
#include "ftb-8-md.h"
#include "driver/spi_master.h"

void app_main(void)
{
    // Initialize SPI bus
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = GPIO_NUM_23,
        .miso_io_num = -1,
        .sclk_io_num = GPIO_NUM_18,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32,
    };
    spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);

    // Register VFD device
    spi_device_handle_t vfd = ftb8md_device_register(
        SPI2_HOST,
        GPIO_NUM_5,   // CS pin
        GPIO_NUM_4    // Reset pin (use -1 if not connected)
    );

    if (vfd != NULL) {
        // Display a string
        ftb8md_show_string(vfd, 0, "HELLO   ");
        
        // Set brightness (0-255)
        ftb8md_set_dimming(vfd, 128);
        
        // Turn on decimal point at digit 3
        ftb8md_set_dot(vfd, 3, true);
    }
}
```

## API Reference

### Initialization

#### `ftb8md_device_register()`

```c
spi_device_handle_t ftb8md_device_register(spi_host_device_t host_id, int cs_pin, int reset_pin);
```

Register and initialize the VFD display device on the SPI bus.

**Parameters:**
- `host_id`: SPI host peripheral (e.g., `SPI2_HOST`, `SPI3_HOST`)
- `cs_pin`: GPIO pin for chip select
- `reset_pin`: GPIO pin for reset (use `-1` if not connected)

**Returns:** SPI device handle on success, `NULL` on failure.

### Display Control

#### `ftb8md_show_string()`

```c
esp_err_t ftb8md_show_string(spi_device_handle_t handle, int digit, const char *str);
```

Display a string starting at the specified digit position.

**Parameters:**
- `handle`: SPI device handle
- `digit`: Starting digit position (0-7)
- `str`: Null-terminated string to display

#### `ftb8md_clear_display()`

```c
esp_err_t ftb8md_clear_display(spi_device_handle_t handle);
```

Clear all digits and decimal points on the display.

#### `ftb8md_set_dimming()`

```c
esp_err_t ftb8md_set_dimming(spi_device_handle_t handle, uint8_t level);
```

Set the display brightness level (0-255).

#### `ftb8md_set_dot()`

```c
esp_err_t ftb8md_set_dot(spi_device_handle_t handle, int digit, bool dot_on);
```

Control the decimal point for a specific digit.

#### `ftb8md_enter_standby()`

```c
esp_err_t ftb8md_enter_standby(spi_device_handle_t handle, bool standby);
```

Enter or exit standby (low power) mode.

### Custom Characters

#### `ftb8md_write_custom_char()`

```c
esp_err_t ftb8md_write_custom_char(spi_device_handle_t handle, int char_index, const uint8_t grid_data[5]);
```

Define a custom 5x7 character pattern in CGRAM.

**Parameters:**
- `handle`: SPI device handle
- `char_index`: CGRAM index (0-7)
- `grid_data`: 5-byte array containing character pattern

#### `ftb8md_set_addressed_char()`

```c
esp_err_t ftb8md_set_addressed_char(spi_device_handle_t handle, int digit, int char_index);
```

Display a custom character from CGRAM at specified digit.

### Advanced Control

#### `ftb8md_set_segment()`

```c
esp_err_t ftb8md_set_segment(spi_device_handle_t handle, int digit, uint8_t segments);
```

Directly control individual segments of a digit.

## Display Memory Architecture

The Futaba 8-MD-06INK has several memory areas:

| Memory | Description |
|--------|-------------|
| DCRAM | Display Character RAM - stores character codes for each digit |
| CGROM | Character Generator ROM - built-in character patterns |
| CGRAM | Character Generator RAM - 8 user-defined characters |
| ADRAM | Additional Display RAM - controls decimal points |

## SPI Timing

- **Clock Frequency:** Max 500 kHz
- **Mode:** SPI Mode 3 (CPOL=1, CPHA=1)
- **Bit Order:** LSB First
- **CS:** Active Low

## Examples

See the `examples` directory for complete usage examples:

- [basic](examples/basic) - Basic display operations
- [custom_char](examples/custom_char) - Custom character definition
- [clock](examples/clock) - Digital clock implementation

## Troubleshooting

1. **Display not responding:**
   - Check SPI connections and pin assignments
   - Verify power supply voltage
   - Try using the reset pin

2. **Garbled display:**
   - Reduce SPI clock frequency
   - Check LSB-first bit order configuration

3. **Dim display:**
   - Increase dimming level with `ftb8md_set_dimming()`
   - Check power supply current capability

## License

MIT License - see [LICENSE](LICENSE) file for details.

## Contributing

Contributions are welcome! Please submit issues and pull requests on GitHub.
