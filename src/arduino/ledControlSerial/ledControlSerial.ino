#include "LPD8806.h"
#include "SPI.h"
#include "stdlib.h"

// Number of RGB LEDs in strand:
int nLEDs = 160;
int nLEDsSet = 0;

// Chose 2 pins for output; can be any valid output pins:
int dataPin  = 2;
int clockPin = 3;

// First parameter is the number of LEDs in the strand.  The LED strips
// are 32 LEDs per meter but you can extend or cut the strip.  Next two
// parameters are SPI data and clock pins:
LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);

void setup() {
  Serial.begin(2000000); // same value as in your c++ script
    
  // Start up the LED strip
  strip.begin();
  
  for (int i = 0; i < nLEDs; i++)
  {
    strip.setPixelColor(i, strip.Color(127, 0, 0));
  }

  // Update the strip, to start they are all 'off'
  strip.show();
}


void loop() 
{
  if (Serial.available() > 3)
  {    
    int values[4];
    
    for (int i = 0; i < 4; ++i)
    {
      values[i] = Serial.read(); // used to read incoming data
    }
    
//    if (values[0] == 0)      
    { 
      strip.setPixelColor(values[0], strip.Color(values[1], values[2], values[3]));
      nLEDsSet++;
    }
    if (nLEDsSet >= nLEDs)
    {
      strip.show();
      clearStrip();
      nLEDsSet = 0;
    }
  }
}


void clearStrip()
{
  for (int i = 0; i < nLEDs; i++)
  {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
  }
}


void rainbow(uint8_t wait) 
{
  for (int i = 0; i < strip.numPixels(); i++) 
  {
    strip.setPixelColor(i, strip.Color(i, 0, 0));
  }  
  strip.show();   // write all the pixels out
  delay(wait);
}
