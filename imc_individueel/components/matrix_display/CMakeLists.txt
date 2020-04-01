set(COMPONENT_REQUIRES i2c_driver)
set(COMPONENT_PRIV_REQUIRES i2c_driver)

set(COMPONENT_ADD_INCLUDEDIRS include)
set(COMPONENT_SRCS "matrix_display.c" "matrix_array.c")
register_component()