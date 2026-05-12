idf_component_register(
    SRCS "web_server.c"
    INCLUDE_DIRS "include"
    REQUIRES esp_http_server rgb_led
    
    EMBED_FILES
        "../../main/web/index.html"
        "../../main/web/style.css"
        "../../main/web/app.js"
)
