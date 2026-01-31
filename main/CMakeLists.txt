idf_component_register(SRCS "the_portal_gun.c" "utils.c"
                    PRIV_REQUIRES esp_driver_gpio esp_driver_dac
                    INCLUDE_DIRS ".")
