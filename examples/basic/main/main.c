/**
 * @file main.c
 * @brief Basic example for Futaba 8-MD-06INK VFD display
 *
 * This example demonstrates basic operations:
 * - Initializing the display
 * - Displaying text strings
 * - Controlling brightness
 * - Using decimal points
 * - Clearing the display
 */

#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "esp_log.h"

#include "ftb-8-md.h"

static const char *TAG = "VFD_BASIC";

/* SPI Pin Configuration - Modify according to your hardware */
#define PIN_NUM_MOSI    23
#define PIN_NUM_CLK     18
#define PIN_NUM_CS      5
#define PIN_NUM_RST     4   /* Set to -1 if not connected */

void app_main(void)
{
    ESP_LOGI(TAG, "Initializing SPI bus...");

    /* Configure SPI bus */
    spi_bus_config_t bus_cfg = {
        .mosi_io_num = PIN_NUM_MOSI,
        .miso_io_num = -1,  /* VFD doesn't send data back */
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

    ESP_LOGI(TAG, "Registering VFD device...");

    /* Register VFD display device */
    spi_device_handle_t vfd = ftb8md_device_register(SPI2_HOST, PIN_NUM_CS, PIN_NUM_RST);
    if (vfd == NULL) {
        ESP_LOGE(TAG, "Failed to register VFD device");
        return;
    }

    ESP_LOGI(TAG, "VFD display initialized successfully!");

    /* Demo loop */
    while (1) {
        /* Display "HELLO" */
        ESP_LOGI(TAG, "Displaying 'HELLO'...");
        ftb8md_clear_display(vfd);
        ftb8md_show_string(vfd, 0, "HELLO   ");
        vTaskDelay(pdMS_TO_TICKS(2000));

        /* Display "ESP32" with full brightness */
        ESP_LOGI(TAG, "Displaying 'ESP32' at full brightness...");
        ftb8md_set_dimming(vfd, 255);
        ftb8md_clear_display(vfd);
        ftb8md_show_string(vfd, 1, "ESP32");
        vTaskDelay(pdMS_TO_TICKS(2000));

        /* Demonstrate decimal points */
        ESP_LOGI(TAG, "Displaying number with decimal point...");
        ftb8md_clear_display(vfd);
        ftb8md_show_string(vfd, 0, "12345678");
        ftb8md_set_dot(vfd, 1, true);   /* Decimal after 2nd digit */
        ftb8md_set_dot(vfd, 4, true);   /* Decimal after 5th digit */
        vTaskDelay(pdMS_TO_TICKS(2000));

        /* Brightness fade demonstration */
        ESP_LOGI(TAG, "Brightness fade demo...");
        ftb8md_clear_display(vfd);
        ftb8md_show_string(vfd, 0, "DIMMING ");

        /* Fade out */
        for (int level = 255; level >= 0; level -= 5) {
            ftb8md_set_dimming(vfd, level);
            vTaskDelay(pdMS_TO_TICKS(30));
        }

        /* Fade in */
        for (int level = 0; level <= 255; level += 5) {
            ftb8md_set_dimming(vfd, level);
            vTaskDelay(pdMS_TO_TICKS(30));
        }

        vTaskDelay(pdMS_TO_TICKS(1000));

        /* Standby mode demonstration */
        ESP_LOGI(TAG, "Entering standby mode...");
        ftb8md_clear_display(vfd);
        ftb8md_show_string(vfd, 0, "STANDBY ");
        vTaskDelay(pdMS_TO_TICKS(1000));

        ftb8md_enter_standby(vfd, true);
        vTaskDelay(pdMS_TO_TICKS(2000));

        ESP_LOGI(TAG, "Exiting standby mode...");
        ftb8md_enter_standby(vfd, false);
        vTaskDelay(pdMS_TO_TICKS(1000));

        /* Scrolling text effect */
        ESP_LOGI(TAG, "Scrolling text demo...");
        const char *scroll_text = "   FUTABA 8-MD-06INK VFD DISPLAY DEMO   ";
        size_t text_len = strlen(scroll_text);

        for (size_t i = 0; i < text_len - 7; i++) {
            char display_buf[9];
            strncpy(display_buf, &scroll_text[i], 8);
            display_buf[8] = '\0';
            ftb8md_show_string(vfd, 0, display_buf);
            vTaskDelay(pdMS_TO_TICKS(200));
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
