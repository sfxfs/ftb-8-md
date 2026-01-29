idf_component_register(SRCS "ftb-8-md.c"
                    PRIV_REQUIRES esp_driver_gpio
                    REQUIRES esp_driver_spi
                    INCLUDE_DIRS "include")
