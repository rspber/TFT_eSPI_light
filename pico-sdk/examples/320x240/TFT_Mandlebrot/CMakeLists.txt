add_executable(TFT_Mandlebrot
  TFT_Mandlebrot.cpp
)

target_link_libraries(TFT_Mandlebrot
  TFT_eSPI
)

pico_enable_stdio_usb(TFT_Mandlebrot 1)
pico_enable_stdio_uart(TFT_Mandlebrot 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(TFT_Mandlebrot)

