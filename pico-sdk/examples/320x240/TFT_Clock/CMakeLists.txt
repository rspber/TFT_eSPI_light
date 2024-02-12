add_executable(TFT_Clock
  TFT_Clock.cpp
)

target_link_libraries(TFT_Clock
  TFT_eSPI
)

pico_enable_stdio_usb(TFT_Clock 1)
pico_enable_stdio_uart(TFT_Clock 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(TFT_Clock)

