cmake_minimum_required(VERSION 3.12)

# Pull in SDK (must be before project)
include("pico_sdk_import.cmake")

project(tsdesktop C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.3.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.3.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

# Initialize the SDK
pico_sdk_init()

add_compile_options(-Wall
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        )

add_subdirectory(libs/env)
add_subdirectory(libs/gfx)
add_subdirectory(libs/tft)
add_subdirectory(libs/protocols/spi)
add_subdirectory(libs/protocols/pio_spi)
add_subdirectory(libs/protocols/pio_8bitp)
add_subdirectory(libs/protocols/gpio_8bitp)
add_subdirectory(libs/utils/snakes)
add_subdirectory(libs/TFT_eSPI)

add_subdirectory(examples/320x240/TFT_Clock)
add_subdirectory(examples/320x240/TFT_Meters)
add_subdirectory(examples/320x240/TFT_Mandlebrot)
add_subdirectory(examples/320x240/TFT_Print_Test)
add_subdirectory(examples/480x320/TFT_Meters)
