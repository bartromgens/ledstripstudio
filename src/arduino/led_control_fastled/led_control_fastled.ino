#include "FastLED.h"

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    2
#define CLK_PIN     3

#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    158
CRGB leds[NUM_LEDS];

#define BRIGHTNESS  96

int nLEDsSet = 0;
int nLEDsPerWrite = 1;


void setup() {
  delay(3000); // 3 second delay for recovery
  
  // tell FastLED about the LED strip configuration
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  //FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  
  Serial.begin(2000000); // same value as in your c++ script
}


void loop() 
{ 
  if (Serial.available() > nLEDsPerWrite*4-1)
  {    
    uint8_t values[nLEDsPerWrite*4];
    
    for (int i = 0; i < nLEDsPerWrite*4; ++i)
    {
      values[i] = Serial.read(); // used to read incoming data
    }
    
    for (int i = 0; i < nLEDsPerWrite; ++i)
    {
      int offset = 4*i;  
      leds[values[offset]][0] = values[offset+1];
      leds[values[offset]][1] = values[offset+2];
      leds[values[offset]][2] = values[offset+3];
      nLEDsSet++;  
    } 
    
    if (nLEDsSet >= NUM_LEDS)
    {
      FastLED.show();
      nLEDsSet = 0;
    }
  }
}
