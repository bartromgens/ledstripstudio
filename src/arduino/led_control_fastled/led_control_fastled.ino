
#include "FastLED.h"

FASTLED_USING_NAMESPACE

#if FASTLED_VERSION < 3001000
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN   7
#define CLK_PIN    14

#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
#define NUM_LEDS    156
CRGB leds[NUM_LEDS];

#define BRIGHTNESS  96

int nLEDsSet = 0;
int nLEDsPerWrite = 2;
int nValuesPerLED = 4;


void setup() 
{
  // tell FastLED about the LED strip configuration
  //FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, DATA_PIN, CLK_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  //FastLED.setBrightness(BRIGHTNESS);

  Serial.begin(2000000); // same value as in your c++ script
}


void loop() 
{  
  if (Serial.available() >= nLEDsPerWrite*nValuesPerLED)
  {    
    uint8_t values[nLEDsPerWrite*nValuesPerLED];
    
    for (int i = 0; i < nLEDsPerWrite*nValuesPerLED; ++i)
    {
      values[i] = Serial.read();
    }

    for (int i = 0; i < nLEDsPerWrite; ++i)
    {
      int offset = nValuesPerLED*i;  
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

