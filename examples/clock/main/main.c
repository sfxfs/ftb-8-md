/**
 * @file main.c
 * @brief Digital clock example for Futaba 8-MD-06INK VFD display
 *
 * This example demonstrates:
 * - Creating a digital clock display
 * - Time formatting with blinking colon
 * - Date display
 * - Using decimal points as separators
 */

#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/spi_master.h"
#include "esp_log.h"
#include "esp_sntp.h"

#include "ftb-8-md.h"

static const char *TAG = "VFD_CLOCK";

/* SPI Pin Configuration - Modify according to your hardware */
#define PIN_NUM_MOSI    23
#define PIN_NUM_CLK     18
#define PIN_NUM_CS      5
#define PIN_NUM_RST     4   /* Set to -1 if not connected */

/* Display modes */
typedef enum {
    DISPLAY_MODE_TIME_24H,
    DISPLAY_MODE_TIME_12H,
    DISPLAY_MODE_DATE,
    DISPLAY_MODE_MAX
} display_mode_t;

static spi_device_handle_t vfd_handle = NULL;

/**
 * @brief Display time in 24-hour format: HH.MM.SS
 */
static void display_time_24h(struct tm *timeinfo, bool blink_colon)
{
    char time_str[9];
    snprintf(time_str, sizeof(time_str), "%02d%02d%02d  ",
             timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
    
    ftb8md_show_string(vfd_handle, 0, time_str);
    
    /* Use decimal points as colons (blinking) */
    if (blink_colon) {
        ftb8md_set_dot(vfd_handle, 1, true);  /* HH.MM */
        ftb8md_set_dot(vfd_handle, 3, true);  /* MM.SS */
    } else {
        ftb8md_set_dot(vfd_handle, 1, false);
        ftb8md_set_dot(vfd_handle, 3, false);
    }
}

/**
 * @brief Display time in 12-hour format: HH.MM AM/PM
 */
static void display_time_12h(struct tm *timeinfo, bool blink_colon)
{
    int hour = timeinfo->tm_hour;
    const char *ampm = "AM";
    
    if (hour >= 12) {
        ampm = "PM";
        if (hour > 12) {
            hour -= 12;
        }
    }
    if (hour == 0) {
        hour = 12;
    }
    
    char time_str[9];
    snprintf(time_str, sizeof(time_str), "%2d%02d%s  ",
             hour, timeinfo->tm_min, ampm);
    
    ftb8md_show_string(vfd_handle, 0, time_str);
    
    /* Blinking colon */
    if (blink_colon) {
        ftb8md_set_dot(vfd_handle, 1, true);
    } else {
        ftb8md_set_dot(vfd_handle, 1, false);
    }
}

/**
 * @brief Display date: DD.MM.YY or MM.DD.YY
 */
static void display_date(struct tm *timeinfo)
{
    char date_str[9];
    /* Format: DD.MM.YYYY (European) - 8 digits */
    snprintf(date_str, sizeof(date_str), "%02d%02d%04d",
             timeinfo->tm_mday, timeinfo->tm_mon + 1, 
             timeinfo->tm_year + 1900);
    
    ftb8md_show_string(vfd_handle, 0, date_str);
    ftb8md_set_dot(vfd_handle, 1, true);   /* DD.MM */
    ftb8md_set_dot(vfd_handle, 3, true);   /* MM.YYYY */
}

/**
 * @brief Initialize a sample time (since we don't have NTP in this basic example)
 */
static void init_sample_time(void)
{
    struct timeval tv = {
        .tv_sec = 1706614800,  /* 2024-01-30 12:00:00 UTC */
        .tv_usec = 0
    };
    settimeofday(&tv, NULL);
}

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
    vfd_handle = ftb8md_device_register(SPI2_HOST, PIN_NUM_CS, PIN_NUM_RST);
    if (vfd_handle == NULL) {
        ESP_LOGE(TAG, "Failed to register VFD device");
        return;
    }

    ESP_LOGI(TAG, "VFD Clock initialized!");

    /* Initialize sample time */
    init_sample_time();

    /* Display startup message */
    ftb8md_clear_display(vfd_handle);
    ftb8md_show_string(vfd_handle, 0, "VFD-CLK ");
    vTaskDelay(pdMS_TO_TICKS(2000));

    display_mode_t current_mode = DISPLAY_MODE_TIME_24H;
    int mode_counter = 0;
    bool blink_state = true;

    /* Main clock loop */
    while (1) {
        time_t now;
        struct tm timeinfo;
        
        time(&now);
        localtime_r(&now, &timeinfo);

        /* Clear previous decimal points */
        for (int i = 0; i < 8; i++) {
            ftb8md_set_dot(vfd_handle, i, false);
        }

        switch (current_mode) {
            case DISPLAY_MODE_TIME_24H:
                display_time_24h(&timeinfo, blink_state);
                break;
            case DISPLAY_MODE_TIME_12H:
                display_time_12h(&timeinfo, blink_state);
                break;
            case DISPLAY_MODE_DATE:
                display_date(&timeinfo);
                break;
            default:
                break;
        }

        /* Toggle blink state every 500ms */
        blink_state = !blink_state;

        /* Switch display mode every 10 seconds */
        mode_counter++;
        if (mode_counter >= 20) {  /* 20 * 500ms = 10 seconds */
            mode_counter = 0;
            current_mode = (display_mode_t)((current_mode + 1) % DISPLAY_MODE_MAX);
            
            /* Show mode name briefly */
            ftb8md_clear_display(vfd_handle);
            switch (current_mode) {
                case DISPLAY_MODE_TIME_24H:
                    ftb8md_show_string(vfd_handle, 0, "24H TIME");
                    break;
                case DISPLAY_MODE_TIME_12H:
                    ftb8md_show_string(vfd_handle, 0, "12H TIME");
                    break;
                case DISPLAY_MODE_DATE:
                    ftb8md_show_string(vfd_handle, 0, "  DATE  ");
                    break;
                default:
                    break;
            }
            vTaskDelay(pdMS_TO_TICKS(1000));
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
