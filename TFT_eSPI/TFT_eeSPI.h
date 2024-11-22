/*
  TFT_eSPI light

  TFT low level procedures

  Copyright (c) 2024, rspber (https://github.com/rspber)

  Based on: TFT_eSPI

  Originally notes below:
*/

/***************************************************
  Arduino TFT graphics library targeted at ESP8266
  and ESP32 based boards.

  This is a stand-alone library that contains the
  hardware driver, the graphics functions and the
  proportional fonts.

  The built-in fonts 4, 6, 7 and 8 are Run Length
  Encoded (RLE) to reduce the FLASH footprint.

  Last review/edit by Bodmer: 04/02/22
 ****************************************************/

#pragma once

#define TFT_ESPI_VERSION "2.5.43"

// Bit level feature flags
// Bit 0 set: viewport capability
#define TFT_ESPI_FEATURES 1

/***************************************************************************************
**                         Section 1: Load required header files
***************************************************************************************/

//Standard support
#include <Setup.h>
#include <env.h>
#include "SnakeStamp.h"

/***************************************************************************************
**                         Section 2: Load library and processor specific header files
***************************************************************************************/

#include <User_Setup_Select.h>

/***************************************************************************************
**                         Section 3: Interface setup
***************************************************************************************/

/***************************************************************************************
**                         Section 4: Setup fonts
***************************************************************************************/

/***************************************************************************************
**                         Section 5: Font datum enumeration
***************************************************************************************/

/***************************************************************************************
**                         Section 6: Colour enumeration
***************************************************************************************/

/***************************************************************************************
**                         Section 7: Diagnostic support
***************************************************************************************/

/***************************************************************************************
**                         Section 8: Class member and support functions
***************************************************************************************/

// Class functions and variables
class TFT_eeSPI : public SnakeStamp {

  friend class TFT_GFX;
  friend class TFT_CHAR;
  friend class TFT_Print;
  friend class TFT_eSPI;
  friend class TFT_eSprite; // Sprite class has access to protected members

 //--------------------------------------- public ------------------------------------//
 public:

  TFT_eeSPI();

                   // Read the colour of a pixel at x,y and return value in 565 format
  virtual rgb_t    readPixel(int32_t x, int32_t y);

  virtual void     setWindow(int32_t xs, int32_t ys, int32_t xe, int32_t ye);   // Note: start + end coordinates

                   // Push (aka write pixel) colours to the set window
  virtual void     pushColor(rgb_t color);

                   // These are non-inlined to enable override
  virtual void     begin_nin_write();
  virtual void     end_nin_write();


  // The TFT_eSprite class inherits the following functions (not all are useful to Sprite class
  void     setAddrWindow(int32_t xs, int32_t ys, int32_t w, int32_t h); // Note: start coordinates + width and height

  // Viewport commands, see "Viewport_Demo" sketch
  void     setViewport(int32_t x, int32_t y, int32_t w, int32_t h, bool vpDatum = true);
  bool     checkViewport(int32_t x, int32_t y, int32_t w, int32_t h);
  int32_t  getViewportX(void);
  int32_t  getViewportY(void);
  int32_t  getViewportWidth(void);
  int32_t  getViewportHeight(void);
  bool     getViewportDatum(void);
  void     frameViewport(rgb_t color, int32_t w);
  void     resetViewport(void);

           // Clip input window to viewport bounds, return false if whole area is out of bounds
  bool     clipAddrWindow(int32_t* x, int32_t* y, int32_t* w, int32_t* h);
           // Clip input window area to viewport bounds, return false if whole area is out of bounds
  bool     clipWindow(int32_t* xs, int32_t* ys, int32_t* xe, int32_t* ye);

           // Push (aka write pixel) colours to the TFT (use setAddrWindow() first)
  void     pushColor(rgb_t color, int32_t len),  // Deprecated, use pushBlock()
           pushColors(uint16_t  *data, int32_t len, bool swap = true), // With byte swap option
           pushColors(uint8_t  *data, int32_t len); // Deprecated, use pushPixels()

           // Write a solid block of a single colour
//  void     pushBlock(rgb_t color, int32_t len);

           // Write a set of pixels stored in memory, use setSwapBytes(true/false) function to correct endianess
  void     pushPixels(const uint16_t* data_in, int32_t len);

           // Swap the byte order for pushImage() and pushPixels() - corrects endianness
  void     setSwapBytes(bool swap);
  bool     getSwapBytes(void);

  // Bare metal functions
  void     startWrite(void);                         // Begin SPI transaction
  void     writeColor(rgb_t color, int32_t len); // Deprecated, use pushBlock()
  void     endWrite(void);                           // End SPI transaction

 private:
           // New begin and end prototypes
           // begin/end a TFT write transaction
           // For SPI bus the transmit clock rate is set
  inline void begin_tft_write() __attribute__((always_inline));
  inline void end_tft_write()   __attribute__((always_inline));

  bool     locked, inTransaction, lockTransaction; // SPI transaction and mutex lock flags

 //-------------------------------------- protected ----------------------------------//
 protected:

  //int32_t  win_xe, win_ye;          // Window end coords - not needed

  // Viewport variables
  int32_t  _vpX, _vpY, _vpW, _vpH;    // Note: x start, y start, x end + 1, y end + 1
  int32_t  _xDatum;
  int32_t  _yDatum;
  int32_t  _xWidth;
  int32_t  _yHeight;
  bool     _vpDatum;
  bool     _vpOoB;

  bool     _swapBytes; // Swap the byte order for TFT pushImage()


};
