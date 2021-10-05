#ifndef SEQ_H
#define SEQ_H

#include <stdint.h>
#include "LED.h"
#include "MIDIsys.h"

// Haven't needed to use these yet - could come in handy
#define STEP1  0b0000000000000001
#define STEP2  0b0000000000000010
#define STEP3  0b0000000000000100
#define STEP4  0b0000000000001000
#define STEP5  0b0000000000010000
#define STEP6  0b0000000000100000
#define STEP7  0b0000000001000000
#define STEP8  0b0000000010000000
#define STEP9  0b0000000100000000
#define STEP10 0b0000001000000000
#define STEP11 0b0000010000000000
#define STEP12 0b0000100000000000
#define STEP13 0b0001000000000000
#define STEP14 0b0010000000000000
#define STEP15 0b0100000000000000
#define STEP16 0b1000000000000000

extern uint8_t channel;
extern struct CRGB leds[NUM_LEDS];
extern struct CRGB seq[4][NUM_LEDS];

extern TaskHandle_t MIDI;
extern QueueHandle_t Q3;
extern uint8_t trackLen;

class track
{
    public:
    bool active; // Whether track is active
    uint8_t bank;  // Which bank is this track on?
    bool topBot; // What position in the bank (top or bottom physical row)
    uint8_t length = 15; // User configurable track length (up to 16 steps for now, length should be -1)
    uint8_t type; // Type of MIDI message triggered by a step
    uint8_t d1; // Value 1 - Midi message status byte is from 'type', these two are the data bytes,
    uint8_t d2; // Value 2 - Eg, for note on d1 is note and d2 is velocity
    uint16_t steps = 0; // One bit for each step (1 - activated, 0 - not)
    uint8_t position; // Current position in the track

    void stepInc(uint32_t& Q3buff);
    void setStep();
};



#endif