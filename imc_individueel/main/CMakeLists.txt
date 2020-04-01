set(COMPONENT_REQUIRES i2c_driver nvs_flash flappy_bird)
set(COMPONENT_PRIV_REQUIRES i2c_driver nvs_flash flappy_bird)

set(COMPONENT_ADD_INCLUDEDIRS )
set(COMPONENT_SRCS "main.c")

register_component()