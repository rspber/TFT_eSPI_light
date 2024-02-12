add_executable(TFT_Meters_480x320
  TFT_Meters.cpp
)

target_link_libraries(TFT_Meters_480x320
  TFT_eSPI
)

pico_enable_stdio_usb(TFT_Meters_480x320 1)
pico_enable_stdio_uart(TFT_Meters_480x320 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(TFT_Meters_480x320)

