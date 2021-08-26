#ifndef LED_H
#define LED_H

#include "FastLED.h"

#define NUM_LEDS 73
#define DATA_PIN 13
#define BRIGHTNESS  50
#define LED_TYPE    WS2812C // No 2812C defined - could add it myself if this doesn't work
#define COLOR_ORDER RGB

void LEDsetup(CRGB leds[NUM_LEDS]);
//void LEDtest(CRGB leds[NUM_LEDS], TickType_t xDelay);










#endif