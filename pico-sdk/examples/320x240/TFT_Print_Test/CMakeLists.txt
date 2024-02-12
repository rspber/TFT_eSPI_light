add_executable(TFT_Print_Test
  TFT_Print_Test.cpp
)

target_link_libraries(TFT_Print_Test
  TFT_eSPI
)

pico_enable_stdio_usb(TFT_Print_Test 1)
pico_enable_stdio_uart(TFT_Print_Test 0)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(TFT_Print_Test)

