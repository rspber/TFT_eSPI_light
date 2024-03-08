// Mandlebrot

// This will run quite slowly due to the large number of floating point calculations per pixel

#include <TFT_eSPI.h> // Hardware-specific library

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

//#define TFT_GREY 0x7BEF
#define TFT_GREY RGB(0x78, 0x7C, 0x78)

unsigned long runTime = 0;

float sx = 0, sy = 0;
uint16_t x0 = 0, x1 = 0, yy0 = 0, yy1 = 0;

void setup()
{
  Serial.begin(250000);
  //randomSeed(analogRead(A0));
  Serial.println();
  // Setup the LCD
  tft.init();
  tft.setRotation(3);
}

void loop()
{
  runTime = millis();

  tft.fillScreen(TFT_BLACK);
  tft.startWrite();
  for (int px = 1; px < 320; px++)
  {
    for (int py = 0; py < 240; py++)
    {
      float x0 = (map(px, 0, 320, -250000/2, -242500/2)) / 100000.0; //scaled x coordinate of pixel (scaled to lie in the Mandelbrot X scale (-2.5, 1))
      float yy0 = (map(py, 0, 240, -75000/4, -61000/4)) / 100000.0; //scaled y coordinate of pixel (scaled to lie in the Mandelbrot Y scale (-1, 1))
      float xx = 0.0;
      float yy = 0.0;
      int iteration = 0;
      int max_iteration = 128;
      while ( ((xx * xx + yy * yy) < 4)  &&  (iteration < max_iteration) )
      {
        float xtemp = xx * xx - yy * yy + x0;
        yy = 2 * xx * yy + yy0;
        xx = xtemp;
        iteration++;
      }
      int color = rainbow((3*iteration+64)%128);
      yield();tft.drawPixel(px, py, color);
    }
  }
  tft.endWrite();

  Serial.println(millis()-runTime);
  while(1) yield();
}

unsigned int rainbow(int value)
{
  // Value is expected to be in range 0-127
  // The value is converted to a spectrum colour from 0 = blue through to red = blue

  byte red = 0; // Red is the top 5 bits of a 16-bit colour value
  byte green = 0;// Green is the middle 6 bits
  byte blue = 0; // Blue is the bottom 5 bits

  byte quadrant = value / 32;

  if (quadrant == 0) {
    blue = 255;
    green = value * 2;
    red = 0;
  }
  if (quadrant == 1) {
    blue = 255 - 2 * value;
    green = 255;
    red = 0;
  }
  if (quadrant == 2) {
    blue = 0;
    green = 255;
    red = value * 2;
  }
  if (quadrant == 3) {
    blue = 0;
    green = 255 - value * 2;
    red = 255;
  }
  return RGB(red, green, blue);
}


