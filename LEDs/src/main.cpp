#include <Arduino.h>

#include <FastLED.h>
#define NUM_LEDS 73
#define DATA_PIN 13

CRGB leds[NUM_LEDS];

void setup() {
  FastLED.addLeds<WS2812, DATA_PIN>(leds, NUM_LEDS);

  
}

void loop() {
      
  for (int i = 0; i < 73; i++)
  {
    leds[i] = CRGB::Red;
    //FastLED.setBrightness(50);
    FastLED.show();
    delay(30); 
  }
  for (int i = 0; i < 73; i++)
  {
    leds[i] = CRGB::Black;
    FastLED.show();
    delay(30); 
  }

}