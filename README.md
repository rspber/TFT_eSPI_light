<p dir="auto">
  <a href="https://github.com/vshymanskyy/StandWithUkraine/blob/main/docs/README.md">
    <img src="https://raw.githubusercontent.com/vshymanskyy/StandWithUkraine/main/banner-direct.svg" alt="SWUbanner" style="max-width: 100%;">
  </a>
</p>

# TFT_eSPI_light
TFT_eSPI clone

## News

* Print, println, String restored.

## Introduction

It is not the intention of this site to develop the TFT_eSPI library.
TFT_eSPI_light is and will remain a 100% compatible clone of the TFT_eSPI library.
The library version number is in the TFT_eSPI.h and currently is:
  ```
    #define TFT_ESPI_VERSION "2.5.0"
  ```
and is equivalent to the appropriate TFT_eSPI version.

## What is the difference between TFT_eSPI_light and TFT_eSPI

1. TFT_eSPI (light) class has no drivers, protocols and low level tft functions
   like drawPixel or readPixel.

2. Already implemented protocols and drivers are provided by TFT_Stack from tsdesktop,
   TFT_Stack consists of directories: gfx, tft, drivers and protocols,
   setup and env are also part of it.

3. Parallel 8 bit: PIO write, GPIO read/write, SPI: read/write, PIO read/write are available.

4. Read and write protocols are separated, so it is possible to write by PIO and read
   and touch by SPI on the same rail, this is done by automatic protocol switching.
   Currently it works in Arduino only.

5. GFXFF font is unavaiable due to collision with the GFXGlyph type redefined in tsdesktop,
   but is available by drawChar and drawText from SCREEN class available by inhertance.

6. Currently, TFT_eSPI_light runs in Arduino and pico-sdk on RP2040,
   Arduino: pic32, Arduino: esp32 in not optimized yet (slower) SPI.

## How to begin

1. ```Download``` or ```clone``` this repository,

2. ```Download``` or ```clone``` https://github.com/rspber/tsdesktop

3. Resolve links to the tsdesktop directories,

4. For pico-sdk:

* In VSCODE select pico-sdk folder and that's all.

5. For Arduino:

* Read further about Display.h.

## About Display.h

1. Display.h provides information about which class is on the top of the TFT_Stack.

2. In tsdesktop Display.h is in media directory and consists of:
  ```
    #define Display TFT_DRIVER
  ```
3. In TFT_eSPI_light this file should be invisible, or even all the media directory,
  the Display.h is in the TFT_eSPI subdirectory and consists of:
  ```
    #define Display TFT_eSPI
    #define display tft
  ```
  ,    in examples this translates
  ```
    TFT_eSPI tft;
  ```
  ,    to
  ```
    Display display;
  ```

## About colors

1. All colors should come from a macro:
  ```
    RGB(R,G,B)
  ```
  ,    or from the procedures intended for this purpose:
  ```
    rgb_t    rgb(uint16_t color565);           // from rgb.h
    rgb_t    color16to24(uint16_t color565);   // from TFT_eSPI
    rgb_t    alphaBlend(uint8_t alpha, rgb_t fgc, rgb_t bgc, uint8_t dither = 0);
  ```

2. The color carrier is rgb_t, which is defined as uint32_t, but this knowledge should be
   used responsibly.

3. However, the color is stored in three lowest bytes, looking from the top: R, G, B,
   this knowledge can be used with no restrictions.

4. The transformation of the color to the bus conditions takes place only at the end
   of the path to the display and is performed in the mdt_color procedure.

5. It's similar with reading color from display by readPixel, regardless of transport type,
   rgb_t will always be returned.

6. The pushImage procedures and similar, which takes colors as uint16_t
   are themselves responsible for converting the colors to rgb_t or using the appropriate
   functions to display color of that type, they are:
  ```
             // Write a set of pixels stored in memory,
             // use setSwapBytes(true/false) function to correct endianess
    void     pushPixels(const uint16_t* data_in, uint32_t len);
  ```
7. If we are not 100% sure that all colors come from the correct source, it is necessary
   to comment out in Setup.h:
  ```
    //#define OVERLAID
  ```
  ,    so as not to definitely stop a running program.

## Notes

1. In examples you shoud use ```tft.begin()``` instead of ```tft.init()```.

2. Some examples have been adapted to TFT_eSPI_light and have been placed in the example directory.

   https://youtube.com/shorts/iBeVsZnV1LM


## Advantages

1. You can easily and transparently add new protocols and modify the existing ones,
   drivers too,

2. All relevant examples should work, in Arduino and pico-sdk,

3. It is possible to switch all color transmission to 666 colors,

4. Function fillRectGradient takes percentage parameter,

5. Overlaid - floating objects.
