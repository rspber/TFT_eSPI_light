/*
  TFT_eSPI light

  Copyright (c) 2023-2024, rspber (https://github.com/rspber)

  TFT_eSPI class without drivers and protocols.

  Bodmer's license in /licenses/Bodmer_license.txt

  originally notes below:
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

// Stop fonts etc. being loaded multiple times
#pragma once

#define TFT_ESPI_VERSION "2.5.43"

// Bit level feature flags
// Bit 0 set: viewport capability
#define TFT_ESPI_FEATURES 1

/***************************************************************************************
**                         Section 1: Load required header files
***************************************************************************************/

#include "TFT_Print.h"

/***************************************************************************************
**                         Section 2: Load library and processor specific header files
***************************************************************************************/

// for protocol select see Setup.h

/***************************************************************************************
**                         Section 3: Interface setup
***************************************************************************************/

// for driver, touch select see Setup.h

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
// #define TFT_eSPI_DEBUG     // Switch on debug support serial messages  (not used yet)
// #define TFT_eSPI_FNx_DEBUG // Switch on debug support for function "x" (not used yet)

// This structure allows sketches to retrieve the user setup parameters at runtime
// by calling getSetup(), zero impact on code size unless used, mainly for diagnostics
typedef struct
{
String  version = TFT_ESPI_VERSION;
String  setup_info;  // Setup reference name available to use in a user setup
uint32_t setup_id;   // ID available to use in a user setup
int32_t esp;         // Processor code
uint8_t trans;       // SPI transaction support
uint8_t serial;      // Serial (SPI) or parallel
#ifndef GENERIC_PROCESSOR
uint8_t  port;       // SPI port
#endif
uint8_t overlap;     // ESP8266 overlap mode
uint8_t interface;   // Interface type

uint16_t tft_driver; // Hexadecimal code
uint16_t tft_width;  // Rotation 0 width and height
uint16_t tft_height;

uint8_t r0_x_offset; // Display offsets, not all used yet
uint8_t r0_y_offset;
uint8_t r1_x_offset;
uint8_t r1_y_offset;
uint8_t r2_x_offset;
uint8_t r2_y_offset;
uint8_t r3_x_offset;
uint8_t r3_y_offset;

int8_t pin_tft_mosi; // SPI pins
int8_t pin_tft_miso;
int8_t pin_tft_clk;
int8_t pin_tft_cs;

int8_t pin_tft_dc;   // Control pins
int8_t pin_tft_rd;
int8_t pin_tft_wr;
int8_t pin_tft_rst;

int8_t pin_tft_d0;   // Parallel port pins
int8_t pin_tft_d1;
int8_t pin_tft_d2;
int8_t pin_tft_d3;
int8_t pin_tft_d4;
int8_t pin_tft_d5;
int8_t pin_tft_d6;
int8_t pin_tft_d7;

int8_t pin_tft_led;
int8_t pin_tft_led_on;

int8_t pin_tch_cs;   // Touch chip select pin

int16_t tft_spi_freq;// TFT write SPI frequency
int16_t tft_rd_freq; // TFT read  SPI frequency
int16_t tch_spi_freq;// Touch controller read/write SPI frequency
} setup_t;

/***************************************************************************************
**                         Section 8: Class member and support functions
***************************************************************************************/

// Class functions and variables
class TFT_eSPI : public TFT_Print {

  friend class TFT_eSprite;

 public:

  TFT_eSPI();

  void init() { begin(); }

  // Image rendering

           // Set TFT pivot point (use when rendering rotated sprites)
  void     setPivot(int16_t x, int16_t y);
  int16_t  getPivotX(void), // Get pivot x
           getPivotY(void); // Get pivot y

  void     setAttribute(uint8_t id = 0, uint8_t a = 0); // Set attribute value
  uint8_t  getAttribute(uint8_t id = 0);                // Get attribute value

           // Used for diagnostic sketch to see library setup adopted by compiler, see Section 7 above
  void     getSetup(setup_t& tft_settings); // Sketch provides the instance to populate
  bool     verifySetupID(uint32_t id);

  uint8_t  rotation;  // Display rotation (0-3)

 protected:

  //int32_t  win_xe, win_ye;          // Window end coords - not needed

  int16_t  _xPivot;   // TFT x pivot point coordinate for rotated Sprites
  int16_t  _yPivot;   // TFT x pivot point coordinate for rotated Sprites

}; // End of class TFT_eSPI

/***************************************************************************************
**                         Section 10: Additional extension classes
***************************************************************************************/
// Load the Button Class
#include "Extensions/Button.h"

// Load the Sprite Class
#include "Extensions/Sprite.h"
