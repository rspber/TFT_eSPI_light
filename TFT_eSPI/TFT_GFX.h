/*
  TFT_eSPI light
  
  Graphics level

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

/***************************************************************************************
**                         Section 1: Load required header files
***************************************************************************************/

#include "TFT_eeSPI.h"

/***************************************************************************************
**                         Section 2: Load library and processor specific header files
***************************************************************************************/

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

// Default color definitions
#define TFT_BLACK       BLACK
#define TFT_NAVY        NAVY
#define TFT_DARKGREEN   DARK_GREEN
#define TFT_DARKCYAN    DARK_CYAN
#define TFT_MAROON      MAROON
#define TFT_PURPLE      PURPLE
#define TFT_OLIVE       OLIVE
#define TFT_LIGHTGREY   LIGHT_GRAY
#define TFT_DARKGREY    RGB(128, 128, 128)
#define TFT_BLUE        BLUE
#define TFT_GREEN       GREEN
#define TFT_CYAN        CYAN
#define TFT_RED         RED
#define TFT_MAGENTA     MAGENTA
#define TFT_YELLOW      YELLOW
#define TFT_WHITE       WHITE
#define TFT_ORANGE      ORANGE
#define TFT_GREENYELLOW GREEN_YELLOW
#define TFT_PINK        PINK
#define TFT_BROWN       BROWN
#define TFT_GOLD        GOLD
#define TFT_SILVER      SILVER
#define TFT_SKYBLUE     SKYBLUE
#define TFT_VIOLET      VIOLET

// Next is a special 16-bit colour value that encodes to 8 bits
// and will then decode back to the same 16-bit value.
// Convenient for 8-bit and 16-bit transparent sprites.
#define TFT_TRANSPARENT 0x0120 // This is actually a dark green

uint16_t color24to16(rgb_t color888);

// Default palette for 4-bit colour sprites
static const uint16_t default_4bit_palette[] PROGMEM = {
  color24to16(TFT_BLACK),    //  0  ^
  color24to16(TFT_BROWN),    //  1  |
  color24to16(TFT_RED),      //  2  |
  color24to16(TFT_ORANGE),   //  3  |
  color24to16(TFT_YELLOW),   //  4  Colours 0-9 follow the resistor colour code!
  color24to16(TFT_GREEN),    //  5  |
  color24to16(TFT_BLUE),     //  6  |
  color24to16(TFT_PURPLE),   //  7  |
  color24to16(TFT_DARKGREY), //  8  |
  color24to16(TFT_WHITE),    //  9  v
  color24to16(TFT_CYAN),     // 10  Blue+green mix
  color24to16(TFT_MAGENTA),  // 11  Blue+red mix
  color24to16(TFT_MAROON),   // 12  Darker red colour
  color24to16(TFT_DARKGREEN),// 13  Darker green colour
  color24to16(TFT_NAVY),     // 14  Darker blue colour
  color24to16(TFT_PINK)      // 15
};

/***************************************************************************************
**                         Section 7: Diagnostic support
***************************************************************************************/

/***************************************************************************************
**                         Section 8: Class member and support functions
***************************************************************************************/

class TFT_GFX : public TFT_eeSPI {

  friend class TFT_CHAR;
  friend class TFT_Print;
  friend class TFT_eSPI;
  friend class TFT_eSprite;

public:
  TFT_GFX();

  virtual void
                   drawLine(int32_t xs, int32_t ys, int32_t xe, int32_t ye, rgb_t color),
                   drawFastVLine(int32_t x, int32_t y, int32_t h, rgb_t color),
                   drawFastHLine(int32_t x, int32_t y, int32_t w, rgb_t color),
                   fillRect(int32_t x, int32_t y, int32_t w, int32_t h, rgb_t color);

  // Graphics drawing
  void
           drawRect(int32_t x, int32_t y, int32_t w, int32_t h, rgb_t color),
           drawRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, rgb_t color),
           fillRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, rgb_t color);

  void     fillRectVGradient(int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color1, rgb_t color2);
  void     fillRectHGradient(int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color1, rgb_t color2);

  void     drawCircle(int32_t x, int32_t y, int32_t r, rgb_t color),
           drawCircleHelper(int32_t x, int32_t y, int32_t r, uint8_t cornername, rgb_t color),
           fillCircle(int32_t x, int32_t y, int32_t r, rgb_t color),
           fillCircleHelper(int32_t x, int32_t y, int32_t r, uint8_t cornername, int32_t delta, rgb_t color),

           drawEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, rgb_t color),
           fillEllipse(int16_t x, int16_t y, int32_t rx, int32_t ry, rgb_t color),

           //                 Corner 1               Corner 2               Corner 3
           drawTriangle(int32_t x1,int32_t y1, int32_t x2,int32_t y2, int32_t x3,int32_t y3, rgb_t color),
           fillTriangle(int32_t x1,int32_t y1, int32_t x2,int32_t y2, int32_t x3,int32_t y3, rgb_t color);

  // Smooth (anti-aliased) graphics drawing
           // Draw a pixel blended with the background pixel colour (bg_color) specified,  return blended colour
           // If the bg_color is not specified, the background pixel colour will be read from TFT or sprite
  rgb_t    drawAlphaPixel(int32_t x, int32_t y, rgb_t color, uint8_t alpha, rgb_t bg_color = WHITE);

           // Draw an anti-aliased (smooth) arc between start and end angles. Arc ends are anti-aliased.
           // By default the arc is drawn with square ends unless the "roundEnds" parameter is included and set true
           // Angle = 0 is at 6 o'clock position, 90 at 9 o'clock etc. The angles must be in range 0-360 or they will be clipped to these limits
           // The start angle may be larger than the end angle. Arcs are always drawn clockwise from the start angle.
  void     drawSmoothArc(int32_t x, int32_t y, int32_t r, int32_t ir, int32_t startAngle, int32_t endAngle, rgb_t fg_color, rgb_t bg_color, bool roundEnds = false);

           // As per "drawSmoothArc" except the ends of the arc are NOT anti-aliased, this facilitates dynamic arc length changes with
           // arc segments and ensures clean segment joints.
           // The sides of the arc are anti-aliased by default. If smoothArc is false sides will NOT be anti-aliased
  void     drawArc(int32_t x, int32_t y, int32_t r, int32_t ir, int32_t startAngle, int32_t endAngle, rgb_t fg_color, rgb_t bg_color, bool smoothArc = true);

           // Draw an anti-aliased filled circle at x, y with radius r
           // Note: The thickness of line is 3 pixels to reduce the visible "braiding" effect of anti-aliasing narrow lines
           //       this means the inner anti-alias zone is always at r-1 and the outer zone at r+1
  void     drawSmoothCircle(int32_t x, int32_t y, int32_t r, rgb_t fg_color, rgb_t bg_color);

           // Draw an anti-aliased filled circle at x, y with radius r
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     fillSmoothCircle(int32_t x, int32_t y, int32_t r, rgb_t color, rgb_t bg_color = WHITE);

           // Draw a rounded rectangle that has a line thickness of r-ir+1 and bounding box defined by x,y and w,h
           // The outer corner radius is r, inner corner radius is ir
           // The inside and outside of the border are anti-aliased
  void     drawSmoothRoundRect(int32_t x, int32_t y, int32_t r, int32_t ir, int32_t w, int32_t h, rgb_t fg_color, rgb_t bg_color = WHITE, uint8_t quadrants = 0xF);

           // Draw a filled rounded rectangle , corner radius r and bounding box defined by x,y and w,h
  void     fillSmoothRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, rgb_t color, rgb_t bg_color = WHITE);

           // Draw a small anti-aliased filled circle at ax,ay with radius r (uses drawWideLine)
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     drawSpot(float ax, float ay, float r, rgb_t fg_color, rgb_t bg_color = WHITE);

           // Draw an anti-aliased wide line from ax,ay to bx,by width wd with radiused ends (radius is wd/2)
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     drawWideLine(float ax, float ay, float bx, float by, float wd, rgb_t fg_color, rgb_t bg_color = WHITE);

           // Draw an anti-aliased wide line from ax,ay to bx,by with different width at each end aw, bw and with radiused ends
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     drawWedgeLine(float ax, float ay, float bx, float by, float aw, float bw, rgb_t fg_color, rgb_t bg_color = WHITE);

           // Draw bitmap
  void     drawBitmap( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, rgb_t fgcolor),
           drawBitmap( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, rgb_t fgcolor, rgb_t bgcolor),
           drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, rgb_t fgcolor),
           drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, rgb_t fgcolor, rgb_t bgcolor),
           setBitmapColor(rgb_t fgcolor, rgb_t bgcolor); // Define the 2 colours for 1bpp sprites

           // Write a block of pixels to the screen which have been read by readRect()
  void     pushRect(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);

           // These are used to render images or sprites stored in RAM arrays (used by Sprite class for 16bpp Sprites)
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data);
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *data, uint16_t transparent);

           // These are used to render images stored in FLASH (PROGMEM)
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data, uint16_t transparent);
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint16_t *data);

           // These are used by Sprite class pushSprite() member function for 1, 4 and 8 bits per pixel (bpp) colours
           // They are not intended to be used with user sketches (but could be)
           // Set bpp8 true for 8bpp sprites, false otherwise. The cmap pointer must be specified for 4bpp
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t  *data, bool bpp8 = true, uint16_t *cmap = nullptr);
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, uint8_t  *data, uint8_t  transparent, bool bpp8 = true, uint16_t *cmap = nullptr);
           // FLASH version
  void     pushImage(int32_t x, int32_t y, int32_t w, int32_t h, const uint8_t *data, bool bpp8,  uint16_t *cmap = nullptr);

           // Render a 16-bit colour image with a 1bpp mask
  void     pushMaskedImage(int32_t x, int32_t y, int32_t w, int32_t h, uint16_t *img, uint8_t *mask);



  // Colour conversion
           // Convert 8-bit red, green and blue to 16 bits
//  uint16_t color565(uint8_t red, uint8_t green, uint8_t blue);

           // Convert 8-bit colour to 16 bits
//  uint16_t color8to16(uint8_t color332);
           // Convert 16-bit colour to 8 bits
//  uint8_t  color16to8(uint16_t color565);

           // Convert 16-bit colour to/from 24-bit, R+G+B concatenated into LS 24 bits
  rgb_t    color16to24(uint16_t color565);
  uint16_t color24to16(rgb_t color888);

           // Alpha blend 2 colours, see generic "alphaBlend_Test" example
           // alpha =   0 = 100% background colour
           // alpha = 255 = 100% foreground colour
//  uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc);
           // 16-bit colour alphaBlend with alpha dither (dither reduces colour banding)
//  uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc, uint8_t dither);
           // 24-bit colour alphaBlend with optional alpha dither
  rgb_t    alphaBlend(uint8_t alpha, rgb_t fgc, rgb_t bgc, uint8_t dither = 0);


  rgb_t    bitmap_fg, bitmap_bg;           // Bitmap foreground (bit=1) and background (bit=0) colours

 private:
           // Smooth graphics helper
  uint8_t  sqrt_fraction(uint32_t num);

           // Helper function: calculate distance of a point from a finite length line between two points
  float    wedgeLineDistance(float pax, float pay, float bax, float bay, float dr);


};

// Swap any type
template <typename T> static inline void
transpose(T& a, T& b) { T t = a; a = b; b = t; }

// Fast alphaBlend
template <typename A, typename F, typename B> static inline uint16_t
fastBlend(A alpha, F fgc, B bgc)
{
  // Split out and blend 5-bit red and blue channels
  uint32_t rxb = bgc & 0xF81F;
  rxb += ((fgc & 0xF81F) - rxb) * (alpha >> 2) >> 6;
  // Split out and blend 6-bit green channel
  uint32_t xgx = bgc & 0x07E0;
  xgx += ((fgc & 0x07E0) - xgx) * alpha >> 8;
  // Recombine channels
  return (rxb & 0xF81F) | (xgx & 0x07E0);
}
