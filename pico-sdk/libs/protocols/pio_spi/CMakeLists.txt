add_library(pio_spi INTERFACE)

target_include_directories(pio_spi INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../../../../protocols/pio_spi)

target_sources(pio_spi INTERFACE
  ${CMAKE_CURRENT_LIST_DIR}/../../../../protocols/pio_spi/RP2040_PIO_SPI.cpp  
)

target_link_libraries(pio_spi INTERFACE
  hardware_pio
  tft
)
