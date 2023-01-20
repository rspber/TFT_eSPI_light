# TFT_eSPI_light
TFT_eSPI clone

## Introduction

It is not the intention of this site to develop the TFT_eSPI library.
TFT_eSPI_light is and will remain a 100% compatible clone of the TFT_eSPI library.
The library version number is in the TFT_eSPI.cpp and currently is:
  ```
    #define TFT_ESPI_VERSION "2.4.79"
  ```
and is equivalent to the appropriate TFT_eSPI version.

## What is the difference between TFT_eSPI_light and TFT_eSPI

1. TFT_eSPI (light) class has no drivers, protocols and low level tft functions
   like drawPixel or readPixel,

2. Already implemented protocols and drivers are provided by TFT_Stack from tsdesktop,
   TFT_Stack consists of directories: ctx, tft, drivers and protocols,
   setup is also part of it,

3. Standard graphics routines were moved (or already exists there) to the gfx class,
   but are still available due to the inheritance hierarchy,

4. This makes the ctx library 100% compatible with TFT_eSPI,

5. The print and println are not supported (for many reasons), you can always write
   their equivalents,

6. SPI read/write and PIO SPI in write mode are available,

7. Read and write protocols are separated, so it is possible to write by PIO and read
   and touch by SPI on the same rail, this is done by automatic protocol switching,

8. Parallel protocols (16BIT and 8BIT) are not implemented yet,

9. Plaing with inTransaction and lockTransaction is not continued, perhaps it will be restored
   but as an internal implementation of startWrite and endWrite and not as separate entries,

10. GFXFF font is unavaiable due to collision with the GFXGlyph type redefined in tsdesktop,
   but are available by drawChar and drawText from SCREEN class available by inhertance too,

11. String type is not supported in all functionality due to sticking with Arduino but also
    due to uselessness with hardcore software, of the String type only remained:
    ```
      typedef const char* String
    ```

12. Currently, TFT_eSPI_light runs in Arduino and pico-sdk on RP2040 ONLY.

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
  the Driver.h is in the TFT_eSPI subdirectory and consists of:
  ```
    #define Display TFT_eSPI
    #define display tft
  ```
  ,    it covers the
  ```
    TFT_eSPI tft;
  ```
  ,    in examples.

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

2. The color carrier in all cases is rgb_t, which is defined as uint32_t,
   but this knowledge should not be used.

3. However, the color is stored in three lowest bytes, looking from the top: R, G, B,
   this knowledge can be used.

4. The transformation of the color to the bus conditions takes place only at the end
   of the path to the display and is performed in the writeColor procedure.

5. It's similar with reading color from display by readPixel, regardless of transport type,
   rgb_t will always be returned.

6. The pushImage or drawBitmap procedures and similar, which takes colors as uint16_t
   are themselves responsible for converting the colors to rgb_t or using the appropriate
   functions to carying a color of that type, they are:
  ```
             // Write a solid block of a single colour
    void     pushBlock(uint16_t color, int32_t len);

             // Write a set of pixels stored in memory,
             // use setSwapBytes(true/false) function to correct endianess
    void     pushPixels(const uint16_t* data_in, int32_t len);
  ```
7. If we are not 100% sure that all colors come from the correct source, it is necessary
   to comment out in Setup.h:
  ```
    //#define OVERLAID
  ```
  ,    so as not to definitely stop a running program.

## Notes

1. In examples you shoud use ```tft.begin()``` instead of ```tft.init()```,

2. Some examples have been adapted to TFT_eSPI_light and have been placed in the example directory.


## Advantages

1. You can easily and transparently add new protocols and modify the existing ones,
   drivers too,

2. All suitable examples should works, in Arduino and pico-sdk,

3. It is possible to switch all color transmission to 666 colors,

4. Function fillRectGradient from TFT_Write takes percentage parameter,

5. Overlaid - floating objects.
