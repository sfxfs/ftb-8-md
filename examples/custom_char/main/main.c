/**
 * @file main.c
 * @brief Custom character example for Futaba 8-MD-06INK VFD display
 *
 * This example demonstrates:
 * - Defining custom 5x7 characters in CGRAM
 * - Displaying custom characters
 * - Creating simple animations with custom characters
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "esp_log.h"

#include "ftb-8-md.h"

static const char *TAG = "VFD_CUSTOM";

/* SPI Pin Configuration - Modify according to your hardware */
#define PIN_NUM_MOSI    23
#define PIN_NUM_CLK     18
#define PIN_NUM_CS      5
#define PIN_NUM_RST     4   /* Set to -1 if not connected */

/**
 * @brief Custom character patterns (5x7 dot matrix)
 * 
 * Each character is defined by 5 bytes, where each byte represents a column.
 * Bit 0 is the top row, bit 6 is the bottom row.
 * 
 * Example: Heart shape
 *   Column: 0    1    2    3    4
 *   Row 0:  .    *    .    *    .     0x0E  0x1F  0x1F  0x1F  0x0E
 *   Row 1:  *    *    *    *    *
 *   Row 2:  *    *    *    *    *
 *   Row 3:  *    *    *    *    *
 *   Row 4:  .    *    *    *    .
 *   Row 5:  .    .    *    .    .
 *   Row 6:  .    .    .    .    .
 */

/* Heart symbol */
static const uint8_t char_heart[5] = {
    0x0E,  // .***. (column 0)
    0x1F,  // ***** (column 1)
    0x1F,  // ***** (column 2)
    0x1F,  // ***** (column 3)
    0x0E   // .***. (column 4)
};

/* Smiley face */
static const uint8_t char_smiley[5] = {
    0x00,  // ..... (column 0)
    0x17,  // *.*.. (column 1) - left eye + mouth
    0x10,  // ....* (column 2) - mouth bottom
    0x17,  // *.*.. (column 3) - right eye + mouth
    0x00   // ..... (column 4)
};

/* Up arrow */
static const uint8_t char_arrow_up[5] = {
    0x04,  // ..*.. (column 0)
    0x02,  // .*... (column 1)
    0x7F,  // ***** (column 2)
    0x02,  // .*... (column 3)
    0x04   // ..*.. (column 4)
};

/* Down arrow */
static const uint8_t char_arrow_down[5] = {
    0x10,  // ...** (column 0)
    0x20,  // ...*. (column 1)
    0x7F,  // ***** (column 2)
    0x20,  // ...*. (column 3)
    0x10   // ...** (column 4)
};

/* Battery empty */
static const uint8_t char_battery_empty[5] = {
    0x7F,  // ***** (column 0)
    0x41,  // *...* (column 1)
    0x41,  // *...* (column 2)
    0x41,  // *...* (column 3)
    0x7F   // ***** (column 4)
};

/* Battery half */
static const uint8_t char_battery_half[5] = {
    0x7F,  // ***** (column 0)
    0x7F,  // ***** (column 1)
    0x41,  // *...* (column 2)
    0x41,  // *...* (column 3)
    0x7F   // ***** (column 4)
};

/* Battery full */
static const uint8_t char_battery_full[5] = {
    0x7F,  // ***** (column 0)
    0x7F,  // ***** (column 1)
    0x7F,  // ***** (column 2)
    0x7F,  // ***** (column 3)
    0x7F   // ***** (column 4)
};

/* Degree symbol */
static const uint8_t char_degree[5] = {
    0x06,  // .**.. (column 0)
    0x09,  // *..** (column 1)
    0x06,  // .**.. (column 2)
    0x00,  // ..... (column 3)
    0x00   // ..... (column 4)
};

void app_main(void)
{
    ESP_LOGI(TAG, "Initializing SPI bus...");

    /* Configure SPI bus */
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = -1,
        .sclk_io_num = PIN_NUM_CLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 32,
    };

    esp_err_t ret = spi_bus_initialize(SPI2_HOST, &bus_cfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SPI bus: %s", esp_err_to_name(ret));
        return;
    }

    /* Register VFD display device */
    spi_device_handle_t vfd = ftb8md_device_register(SPI2_HOST, PIN_NUM_CS, PIN_NUM_RST);
    if (vfd == NULL) {
        ESP_LOGE(TAG, "Failed to register VFD device");
        return;
    }

    ESP_LOGI(TAG, "Loading custom characters into CGRAM...");

    /* Load custom characters into CGRAM (indices 0-7) */
    ftb8md_write_custom_char(vfd, 0, char_heart);
    ftb8md_write_custom_char(vfd, 1, char_smiley);
    ftb8md_write_custom_char(vfd, 2, char_arrow_up);
    ftb8md_write_custom_char(vfd, 3, char_arrow_down);
    ftb8md_write_custom_char(vfd, 4, char_battery_empty);
    ftb8md_write_custom_char(vfd, 5, char_battery_half);
    ftb8md_write_custom_char(vfd, 6, char_battery_full);
    ftb8md_write_custom_char(vfd, 7, char_degree);

    ESP_LOGI(TAG, "Custom characters loaded!");

    /* Demo loop */
    while (1) {
        /* Display all custom characters */
        ESP_LOGI(TAG, "Displaying all custom characters...");
        ftb8md_clear_display(vfd);
        for (int i = 0; i < 8; i++) {
            ftb8md_set_addressed_char(vfd, i, i);
        }
        vTaskDelay(pdMS_TO_TICKS(3000));

        /* Heart animation - display hearts one by one */
        ESP_LOGI(TAG, "Heart animation...");
        ftb8md_clear_display(vfd);
        for (int i = 0; i < 8; i++) {
            ftb8md_set_addressed_char(vfd, i, 0);  /* Heart character */
            vTaskDelay(pdMS_TO_TICKS(200));
        }
        vTaskDelay(pdMS_TO_TICKS(1000));

        /* "I ♥ ESP32" message */
        ESP_LOGI(TAG, "Displaying 'I ♥ ESP32'...");
        ftb8md_clear_display(vfd);
        ftb8md_show_string(vfd, 0, "I");
        ftb8md_set_addressed_char(vfd, 1, 0);  /* Heart */
        ftb8md_show_string(vfd, 2, "ESP32 ");
        vTaskDelay(pdMS_TO_TICKS(3000));

        /* Temperature display with degree symbol */
        ESP_LOGI(TAG, "Temperature display...");
        ftb8md_clear_display(vfd);
        ftb8md_show_string(vfd, 0, "  25");
        ftb8md_set_addressed_char(vfd, 4, 7);  /* Degree symbol */
        ftb8md_show_string(vfd, 5, "C  ");
        vTaskDelay(pdMS_TO_TICKS(3000));

        /* Battery charging animation */
        ESP_LOGI(TAG, "Battery charging animation...");
        ftb8md_clear_display(vfd);
        ftb8md_show_string(vfd, 0, "CHARGE ");
        
        for (int cycle = 0; cycle < 5; cycle++) {
            ftb8md_set_addressed_char(vfd, 7, 4);  /* Empty battery */
            vTaskDelay(pdMS_TO_TICKS(500));
            ftb8md_set_addressed_char(vfd, 7, 5);  /* Half battery */
            vTaskDelay(pdMS_TO_TICKS(500));
            ftb8md_set_addressed_char(vfd, 7, 6);  /* Full battery */
            vTaskDelay(pdMS_TO_TICKS(500));
        }

        /* Arrow animation */
        ESP_LOGI(TAG, "Arrow animation...");
        ftb8md_clear_display(vfd);
        ftb8md_show_string(vfd, 0, " SCAN  ");
        
        for (int cycle = 0; cycle < 10; cycle++) {
            ftb8md_set_addressed_char(vfd, 7, 2);  /* Up arrow */
            vTaskDelay(pdMS_TO_TICKS(300));
            ftb8md_set_addressed_char(vfd, 7, 3);  /* Down arrow */
            vTaskDelay(pdMS_TO_TICKS(300));
        }

        /* Smiley greeting */
        ESP_LOGI(TAG, "Smiley greeting...");
        ftb8md_clear_display(vfd);
        ftb8md_set_addressed_char(vfd, 0, 1);  /* Smiley */
        ftb8md_show_string(vfd, 1, " HELLO ");
        ftb8md_set_addressed_char(vfd, 7, 1);  /* Smiley */
        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
