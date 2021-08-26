#include "LED.h"


void LEDsetup(CRGB leds[NUM_LEDS])
{
    FastLED.addLeds<LED_TYPE, DATA_PIN>(leds, NUM_LEDS);
}
/*
void LEDtest(CRGB leds[NUM_LEDS], TickType_t xDelay)
{
//const TickType_t xDelay = 10 / portTICK_PERIOD_MS;
//portMUX_TYPE myMutex = portMUX_INITIALIZER_UNLOCKED;


for (int i = 0; i < 73; i++)
  {
    if(i == 0)
    {leds[72] = CRGB::Black;}
    if(i > 0)
    {leds[i-1] = CRGB::Black;}
    leds[i] = CRGB::Green;
    FastLED.setBrightness(50);
    //vPortEnterCritical(&myMutex);
    FastLED.show();
    //vPortEnterCritical(&myMutex);
    vTaskDelay(xDelay);
  }
}
*/