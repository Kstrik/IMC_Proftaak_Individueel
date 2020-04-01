set(COMPONENT_REQUIRES matrix_display gpio_button buzzer)
set(COMPONENT_PRIV_REQUIRES matrix_display gpio_button buzzer)

set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_SRCS "flappy_bird.c" "pipelane.c" "bird.c")
register_component()