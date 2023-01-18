add_library(setup INTERFACE)

target_include_directories(setup INTERFACE ${CMAKE_CURRENT_LIST_DIR})

target_sources(setup INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/Picoino.cpp
  ${CMAKE_CURRENT_LIST_DIR}/Wire.cpp
  ${CMAKE_CURRENT_LIST_DIR}/dtostrf.cpp
)

target_link_libraries(setup INTERFACE
  pico_stdlib
  hardware_i2c
  hardware_gpio
  spi
  pio_spi
)
