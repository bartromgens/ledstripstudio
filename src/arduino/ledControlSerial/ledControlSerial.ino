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
  
  LPD8806 strip = LPD8806(nLEDs, dataPin, clockPin);
  
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
  int nLEDsPerWrite = 5;
  
  if (Serial.available() > nLEDsPerWrite*4-1)
  {    
    int values[nLEDsPerWrite*4];
    
    for (int i = 0; i < nLEDsPerWrite*4; ++i)
    {
      values[i] = Serial.read(); // used to read incoming data
    }
    
    for (int i = 0; i < nLEDsPerWrite; ++i)
    {
      int offset = 4*i;  
      strip.setPixelColor(values[offset], strip.Color(values[offset+1], values[offset+2], values[offset+3]));
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
