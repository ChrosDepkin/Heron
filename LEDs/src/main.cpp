#include <Arduino.h>

#include <FastLED.h>
#define NUM_LEDS 73
#define DATA_PIN 13

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812B, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setCorrection(UncorrectedColor);
  FastLED.setTemperature(UncorrectedTemperature);
  FastLED.setDither(DISABLE_DITHER);
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Blue;
  }
    FastLED.setBrightness(50);
    FastLED.show();
  
}

void loop() {
      

/*
  for (int i = 0; i < 73; i++)
  {
    if(i == 0)
    {leds[72] = CRGB::Black;}
    if(i > 0)
    {leds[i-1] = CRGB::Black;}
    leds[i] = CRGB::Green;
    FastLED.setBrightness(50);
    FastLED.show();
    delay(100); 
  }
*/
}