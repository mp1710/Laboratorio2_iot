idf_component_register(
    SRCS "main.c"
    INCLUDE_DIRS "."
    REQUIRES
        nvs_flash
        rgb_led
        wifi_manager
        web_server
        touch
)

