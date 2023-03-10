/*
  TFT_eSPI light

  Copyright (c) 2023, rspber (https://github.com/rspber)

  TFT_eSPI class without drivers and protocols,
  standard graphics routines were moved to gfx class,
  print and println are not supported,
  spi and pio spi in write mode are available,
  parallel protocols are not implemented yet,
  GFXFF font is unavaiable due to collision with the
  GFXGlyph type redefined in tsdesktop,
  all relevant examples should work,

  currently
  runs in Arduino and pico-sdk on RP2040 ONLY.

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

#pragma once

#define TFT_ESPI_VERSION "2.4.79"

// Bit level feature flags
// Bit 0 set: viewport capability
#define TFT_ESPI_FEATURES 1

/***************************************************************************************
**                         Section 1: Load required header files
***************************************************************************************/

#include <TFT_DRIVER.h>

/***************************************************************************************
**                         Section 2: Load library and processor specific header files
***************************************************************************************/

#include <User_Setup_Select.h>

// Handle FLASH based storage e.g. PROGMEM
#if defined(ARDUINO_ARCH_RP2040)
  #undef pgm_read_byte
  #define pgm_read_byte(addr)   (*(const unsigned char *)(addr))
  #undef pgm_read_word
  #define pgm_read_word(addr) ({ \
    typeof(addr) _addr = (addr); \
    *(const unsigned short *)(_addr); \
  })
  #undef pgm_read_dword
  #define pgm_read_dword(addr) ({ \
    typeof(addr) _addr = (addr); \
    *(const unsigned long *)(_addr); \
  })
#elif defined(__AVR__)
  #include <avr/pgmspace.h>
#elif defined(ARDUINO_ARCH_ESP8266) || defined(ESP32)
  #include <pgmspace.h>
#else
  #define PROGMEM
#endif

// for protocol select see Setup.h

/***************************************************************************************
**                         Section 3: Interface setup
***************************************************************************************/

// for driver, touch select see Setup.h

/***************************************************************************************
**                         Section 4: Setup fonts
***************************************************************************************/
// Use GLCD font in error case where user requests a smooth font file
// that does not exist (this is a temporary fix to stop ESP32 reboot)
#ifdef SMOOTH_FONT
  #ifndef LOAD_GLCD
    #define LOAD_GLCD
  #endif
#endif

// Only load the fonts defined in User_Setup.h (to save space)
// Set flag so RLE rendering code is optionally compiled
#ifdef LOAD_GLCD
  #include <Fonts/glcdfont.c>
#endif

#ifdef LOAD_FONT2
  #include <Fonts/Font16.h>
#endif

#ifdef LOAD_FONT4
  #include <Fonts/Font32rle.h>
  #define LOAD_RLE
#endif

#ifdef LOAD_FONT6
  #include <Fonts/Font64rle.h>
  #ifndef LOAD_RLE
    #define LOAD_RLE
  #endif
#endif

#ifdef LOAD_FONT7
  #include <Fonts/Font7srle.h>
  #ifndef LOAD_RLE
    #define LOAD_RLE
  #endif
#endif

#ifdef LOAD_FONT8
  #include <Fonts/Font72rle.h>
  #ifndef LOAD_RLE
    #define LOAD_RLE
  #endif
#elif defined LOAD_FONT8N // Optional narrower version
  #define LOAD_FONT8
  #include <Fonts/Font72x53rle.h>
  #ifndef LOAD_RLE
    #define LOAD_RLE
  #endif
#endif

#ifdef LOAD_GFXFF
  // We can include all the free fonts and they will only be built into
  // the sketch if they are used
  #include <Fonts/GFXFF/gfxfont.h>
  // Call up any user custom fonts
  #include <User_Setups/User_Custom_Fonts.h>
#endif // #ifdef LOAD_GFXFF

// Create a null default font in case some fonts not used (to prevent crash)
const  uint8_t widtbl_null[1] = {0};
PROGMEM const uint8_t chr_null[1] = {0};
PROGMEM const uint8_t* const chrtbl_null[1] = {chr_null};

// This is a structure to conveniently hold information on the default fonts
// Stores pointer to font character image address table, width table and height
typedef struct {
    const uint8_t *chartbl;
    const uint8_t *widthtbl;
    uint8_t height;
    uint8_t baseline;
    } fontinfo;

// Now fill the structure
const PROGMEM fontinfo fontdata [] = {
  #ifdef LOAD_GLCD
   { (const uint8_t *)font, widtbl_null, 0, 0 },
  #else
   { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },
  #endif
   // GLCD font (Font 1) does not have all parameters
   { (const uint8_t *)chrtbl_null, widtbl_null, 8, 7 },

  #ifdef LOAD_FONT2
   { (const uint8_t *)chrtbl_f16, widtbl_f16, chr_hgt_f16, baseline_f16},
  #else
   { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },
  #endif

   // Font 3 current unused
   { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },

  #ifdef LOAD_FONT4
   { (const uint8_t *)chrtbl_f32, widtbl_f32, chr_hgt_f32, baseline_f32},
  #else
   { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },
  #endif

   // Font 5 current unused
   { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },

  #ifdef LOAD_FONT6
   { (const uint8_t *)chrtbl_f64, widtbl_f64, chr_hgt_f64, baseline_f64},
  #else
   { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },
  #endif

  #ifdef LOAD_FONT7
   { (const uint8_t *)chrtbl_f7s, widtbl_f7s, chr_hgt_f7s, baseline_f7s},
  #else
   { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 },
  #endif

  #ifdef LOAD_FONT8
   { (const uint8_t *)chrtbl_f72, widtbl_f72, chr_hgt_f72, baseline_f72}
  #else
   { (const uint8_t *)chrtbl_null, widtbl_null, 0, 0 }
  #endif
};

/***************************************************************************************
**                         Section 5: Font datum enumeration
***************************************************************************************/
//These enumerate the text plotting alignment (reference datum point)
#define TL_DATUM 0 // Top left (default)
#define TC_DATUM 1 // Top centre
#define TR_DATUM 2 // Top right
#define ML_DATUM 3 // Middle left
#define CL_DATUM 3 // Centre left, same as above
#define MC_DATUM 4 // Middle centre
#define CC_DATUM 4 // Centre centre, same as above
#define MR_DATUM 5 // Middle right
#define CR_DATUM 5 // Centre right, same as above
#define BL_DATUM 6 // Bottom left
#define BC_DATUM 7 // Bottom centre
#define BR_DATUM 8 // Bottom right
#define L_BASELINE  9 // Left character baseline (Line the 'A' character would sit on)
#define C_BASELINE 10 // Centre character baseline
#define R_BASELINE 11 // Right character baseline

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

// Next is a special 16 bit colour value that encodes to 8 bits
// and will then decode back to the same 16 bit value.
// Convenient for 8 bit and 16 bit transparent sprites.
#define TFT_TRANSPARENT 0x0120 // This is actually a dark green

uint16_t color24to16(rgb_t color888);

// Default palette for 4 bit colour sprites
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
uint8_t  port;       // SPI port
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
// Swap any type
template <typename T> static inline void
swap_coord(T& a, T& b) { T t = a; a = b; b = t; }

// Callback prototype for smooth font pixel colour read
typedef uint16_t (*getColorCallback)(uint16_t x, uint16_t y);

// Class functions and variables
class TFT_eSPI : public TFT_DRIVER { friend class TFT_eSprite; // Sprite class has access to protected members

 //--------------------------------------- public ------------------------------------//
 public:

  TFT_eSPI();

  // in examples, init() must be renamed to begin()

  void     print(const char* text) {}   // Arduino Print library is not supported !.
  void     println(const char* text) {}   // Arduino Print library is not supported !.

  virtual void     drawChar(int32_t x, int32_t y, uint16_t c, rgb_t color, rgb_t bg, uint8_t size);

  virtual int16_t  drawChar(uint16_t uniCode, int32_t x, int32_t y, uint8_t font),
                   drawChar(uint16_t uniCode, int32_t x, int32_t y);

                   // Read the colour of a pixel at x,y and return value in 565 format
  virtual rgb_t    readPixel(int32_t x, int32_t y);

  virtual void     setWindow(int32_t xs, int32_t ys, int32_t xe, int32_t ye);   // Note: start + end coordinates

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

           // Write a solid block of a single colour
  void     pushBlock(uint16_t color, int32_t len);

           // Write a set of pixels stored in memory, use setSwapBytes(true/false) function to correct endianess
  void     pushPixels(const uint16_t* data_in, int32_t len);

  void     fillRectVGradient(int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color1, rgb_t color2);
  void     fillRectHGradient(int16_t x, int16_t y, int16_t w, int16_t h, rgb_t color1, rgb_t color2);

           // Draw a pixel blended with the pixel colour on the TFT or sprite, return blended colour
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  rgb_t    drawAlphaPixel(int32_t x, int32_t y, rgb_t color, uint8_t alpha, rgb_t bg_color = WHITE);

           // Draw a small anti-aliased filled circle at ax,ay with radius r (uses drawWideLine)
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     drawSpot(float ax, float ay, float r, rgb_t fg_color, rgb_t bg_color = WHITE);

           // Draw an anti-aliased filled circle at x, y with radius r
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     fillSmoothCircle(int32_t x, int32_t y, int32_t r, rgb_t color, rgb_t bg_color = WHITE);

  void     fillSmoothRoundRect(int32_t x, int32_t y, int32_t w, int32_t h, int32_t radius, rgb_t color, rgb_t bg_color = WHITE);

           // Draw an anti-aliased wide line from ax,ay to bx,by width wd with radiused ends (radius is wd/2)
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     drawWideLine(float ax, float ay, float bx, float by, float wd, rgb_t fg_color, rgb_t bg_color = WHITE);

           // Draw an anti-aliased wide line from ax,ay to bx,by with different width at each end aw, bw and with radiused ends
           // If bg_color is not included the background pixel colour will be read from TFT or sprite
  void     drawWedgeLine(float ax, float ay, float bx, float by, float aw, float bw, rgb_t fg_color, rgb_t bg_color = WHITE);

  // Image rendering
           // Swap the byte order for pushImage() and pushPixels() - corrects endianness
  void     setSwapBytes(bool swap);
  bool     getSwapBytes(void);

           // Draw bitmap
  void     drawBitmap( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, rgb_t fgcolor),
           drawBitmap( int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, rgb_t fgcolor, rgb_t bgcolor),
           drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, rgb_t fgcolor),
           drawXBitmap(int16_t x, int16_t y, const uint8_t *bitmap, int16_t w, int16_t h, rgb_t fgcolor, rgb_t bgcolor),
           setBitmapColor(rgb_t fgcolor, rgb_t bgcolor); // Define the 2 colours for 1bpp sprites

           // Set TFT pivot point (use when rendering rotated sprites)
  void     setPivot(int16_t x, int16_t y);
  int16_t  getPivotX(void), // Get pivot x
           getPivotY(void); // Get pivot y

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

  // Text rendering - value returned is the pixel width of the rendered text
  int16_t  drawNumber(long intNumber, int32_t x, int32_t y, uint8_t font), // Draw integer using specified font number
           drawNumber(long intNumber, int32_t x, int32_t y),               // Draw integer using current font

           // Decimal is the number of decimal places to render
           // Use with setTextDatum() to position values on TFT, and setTextPadding() to blank old displayed values
           drawFloat(float floatNumber, uint8_t decimal, int32_t x, int32_t y, uint8_t font), // Draw float using specified font number
           drawFloat(float floatNumber, uint8_t decimal, int32_t x, int32_t y),               // Draw float using current font

           // Handle char arrays
           // Use with setTextDatum() to position string on TFT, and setTextPadding() to blank old displayed strings
           drawString(const char *string, int32_t x, int32_t y, uint8_t font),  // Draw string using specified font number
           drawString(const char *string, int32_t x, int32_t y),                // Draw string using current font
//           drawString(const String& string, int32_t x, int32_t y, uint8_t font),// Draw string using specified font number
//           drawString(const String& string, int32_t x, int32_t y),              // Draw string using current font

           drawCentreString(const char *string, int32_t x, int32_t y, uint8_t font),  // Deprecated, use setTextDatum() and drawString()
           drawRightString(const char *string, int32_t x, int32_t y, uint8_t font);   // Deprecated, use setTextDatum() and drawString()
//           drawCentreString(const String& string, int32_t x, int32_t y, uint8_t font),// Deprecated, use setTextDatum() and drawString()
//           drawRightString(const String& string, int32_t x, int32_t y, uint8_t font); // Deprecated, use setTextDatum() and drawString()

  // Text rendering and font handling support funtions
  void     setCursor(int16_t x, int16_t y),                 // Set cursor for tft.print()
           setCursor(int16_t x, int16_t y, uint8_t font);   // Set cursor and font number for tft.print()

  int16_t  getCursorX(void),                                // Read current cursor x position (moves with tft.print())
           getCursorY(void);                                // Read current cursor y position

  void     setTextColor(rgb_t color),                    // Set character (glyph) color only (background not over-written)
           setTextColor(rgb_t fgcolor, rgb_t bgcolor, bool bgfill = false),  // Set character (glyph) foreground and background colour, optional background fill for smooth fonts
           setTextSize(uint8_t size);                       // Set character size multiplier (this increases pixel size)

  void     setTextWrap(bool wrapX, bool wrapY = false);     // Turn on/off wrapping of text in TFT width and/or height

  void     setTextDatum(uint8_t datum);                     // Set text datum position (default is top left), see Section 6 above
  uint8_t  getTextDatum(void);

  void     setTextPadding(uint16_t x_width);                // Set text padding (background blanking/over-write) width in pixels
  uint16_t getTextPadding(void);                            // Get text padding

#ifdef LOAD_GFXFF
  void     setFreeFont(const GFXfont *f = NULL),            // Select the GFX Free Font
           setTextFont(uint8_t font);                       // Set the font number to use in future
#else
  void     setFreeFont(uint8_t font),                       // Not used, historical fix to prevent an error
           setTextFont(uint8_t font);                       // Set the font number to use in future
#endif

  int16_t  textWidth(const char *string, uint8_t font),     // Returns pixel width of string in specified font
           textWidth(const char *string),                   // Returns pixel width of string in current font
//           textWidth(const String& string, uint8_t font),   // As above for String types
//           textWidth(const String& string),
           fontHeight(int16_t font),                        // Returns pixel height of string in specified font
           fontHeight(void);                                // Returns pixel width of string in current font

           // Used by library and Smooth font class to extract Unicode point codes from a UTF8 encoded string
  uint16_t decodeUTF8(uint8_t *buf, uint16_t *index, uint16_t remaining),
           decodeUTF8(uint8_t c);

           // Support function to UTF8 decode and draw characters piped through print stream
  size_t   write(uint8_t);
  // size_t   write(const uint8_t *buf, size_t len);

           // Used by Smooth font class to fetch a pixel colour for the anti-aliasing
  void     setCallback(getColorCallback getCol);

  uint16_t fontsLoaded(void); // Each bit in returned value represents a font type that is loaded - used for debug/error handling only

  // Colour conversion
           // Convert 8 bit red, green and blue to 16 bits
//  uint16_t color565(uint8_t red, uint8_t green, uint8_t blue);

           // Convert 8 bit colour to 16 bits
//  uint16_t color8to16(uint8_t color332);
           // Convert 16 bit colour to 8 bits
//  uint8_t  color16to8(uint16_t color565);

           // Convert 16 bit colour to/from 24 bit, R+G+B concatenated into LS 24 bits
  rgb_t    color16to24(uint16_t color565);
  uint16_t color24to16(rgb_t color888);

           // Alpha blend 2 colours, see generic "alphaBlend_Test" example
           // alpha =   0 = 100% background colour
           // alpha = 255 = 100% foreground colour
//  uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc);
           // 16 bit colour alphaBlend with alpha dither (dither reduces colour banding)
//  uint16_t alphaBlend(uint8_t alpha, uint16_t fgc, uint16_t bgc, uint8_t dither);
           // 24 bit colour alphaBlend with optional alpha dither
  rgb_t    alphaBlend(uint8_t alpha, rgb_t fgc, rgb_t bgc, uint8_t dither = 0);

  // Set/get an arbitrary library configuration attribute or option
  //       Use to switch ON/OFF capabilities such as UTF8 decoding - each attribute has a unique ID
  //       id = 0: reserved - may be used in future to reset all attributes to a default state
  //       id = 1: Turn on (a=true) or off (a=false) GLCD cp437 font character error correction
  //       id = 2: Turn on (a=true) or off (a=false) UTF8 decoding
  //       id = 3: Enable or disable use of ESP32 PSRAM (if available)
           #define CP437_SWITCH 1
           #define UTF8_SWITCH  2
           #define PSRAM_ENABLE 3
  void     setAttribute(uint8_t id = 0, uint8_t a = 0); // Set attribute value
  uint8_t  getAttribute(uint8_t id = 0);                // Get attribute value

           // Used for diagnostic sketch to see library setup adopted by compiler, see Section 7 above
  void     getSetup(setup_t& tft_settings); // Sketch provides the instance to populate
  bool     verifySetupID(uint32_t id);

  // Global variables
  rgb_t    textcolor, textbgcolor;         // Text foreground and background colours

  rgb_t    bitmap_fg, bitmap_bg;           // Bitmap foreground (bit=1) and background (bit=0) colours

  uint8_t  textfont,  // Current selected font number
           textsize,  // Current font size multiplier
           textdatum, // Text reference datum
           rotation;  // Display rotation (0-3)

  uint8_t  decoderState = 0;   // UTF8 decoder state        - not for user access
  uint16_t decoderBuffer;      // Unicode code-point buffer - not for user access

 //--------------------------------------- private ------------------------------------//
 private:
           // Helper function: calculate distance of a point from a finite length line between two points
  float    wedgeLineDistance(float pax, float pay, float bax, float bay, float dr);

           // Display variant settings
  uint8_t  tabcolor,                   // ST7735 screen protector "tab" colour (now invalid)
           colstart = 0, rowstart = 0; // Screen display area to CGRAM area coordinate offsets

  //uint32_t lastColor = 0xFFFF; // Last colour - used to minimise bit shifting overhead

  getColorCallback getColor = nullptr; // Smooth font callback function pointer


 //-------------------------------------- protected ----------------------------------//
 protected:

  //int32_t  win_xe, win_ye;          // Window end coords - not needed

  int16_t  _xPivot;   // TFT x pivot point coordinate for rotated Sprites
  int16_t  _yPivot;   // TFT x pivot point coordinate for rotated Sprites

  // Viewport variables
  int32_t  _vpX, _vpY, _vpW, _vpH;    // Note: x start, y start, x end + 1, y end + 1
  int32_t  _xDatum;
  int32_t  _yDatum;
  int32_t  _xWidth;
  int32_t  _yHeight;
  bool     _vpDatum;
  bool     _vpOoB;

  int32_t  cursor_x, cursor_y, padX;       // Text cursor x,y and padding setting
  int32_t  bg_cursor_x;                    // Background fill cursor
  int32_t  last_cursor_x;                  // Previous text cursor position when fill used

  uint32_t fontsloaded;               // Bit field of fonts loaded

  uint8_t  glyph_ab,   // Smooth font glyph delta Y (height) above baseline
           glyph_bb;   // Smooth font glyph delta Y (height) below baseline

  bool     isDigits;   // adjust bounding box for numbers to reduce visual jiggling
  bool     textwrapX, textwrapY;  // If set, 'wrap' text at right and optionally bottom edge of display
  bool     _swapBytes; // Swap the byte order for TFT pushImage()

                       // User sketch manages these via set/getAttribute()
  bool     _cp437;        // If set, use correct CP437 charset (default is ON)
  bool     _utf8;         // If set, use UTF-8 decoder in print stream 'write()' function (default ON)
  bool     _psram_enable; // Enable PSRAM use for library functions (TBD) and Sprites

  rgb_t    _lastColor; // Buffered value of last colour used

  bool     _fillbg;    // Fill background flag (just for for smooth fonts at the moment)

#if defined (SSD1963_DRIVER)
  uint16_t Cswap;      // Swap buffer for SSD1963
  uint8_t r6, g6, b6;  // RGB buffer for SSD1963
#endif

#ifdef LOAD_GFXFF
  GFXfont  *gfxFont;
#endif

/***************************************************************************************
**                         Section 9: TFT_eSPI class conditional extensions
***************************************************************************************/
// Load the Anti-aliased font extension
#ifdef SMOOTH_FONT
  #include "Extensions/Smooth_font.h"  // Loaded if SMOOTH_FONT is defined by user
#endif

}; // End of class TFT_eSPI

/***************************************************************************************
**                         Section 10: Additional extension classes
***************************************************************************************/
// Load the Button Class
#include "Extensions/Button.h"

// Load the Sprite Class
#include "Extensions/Sprite.h"
