#include <stdint.h>
#include "I2Cdefs.h"
#include "MIDIsys.h"

// MIDI Note Defines
#define A0 21
#define As0 22
#define B0 23

#define C1 24
#define Cs1 25
#define D1 26
#define Ds1 27
#define E1 28
#define F1 29
#define Fs1 30
#define G1 31
#define Gs1 32
#define A1 33
#define As1 34
#define B1 35

#define C2 36
#define Cs2 37
#define D2 38
#define Ds2 39
#define E2 40
#define F2 41
#define Fs2 42
#define G2 43
#define Gs2 44
#define A2 45
#define As2 46
#define B2 47

#define C3 48
#define Cs3 49
#define D3 50
#define Ds3 51
#define E3 52
#define F3 53
#define Fs3 54
#define G3 55
#define Gs3 56
#define A3 57
#define As3 58
#define B3 59

#define C4 60
#define Cs4 61
#define D4 62
#define Ds4 63
#define E4 64
#define F4 65
#define Fs4 66
#define G4 67
#define Gs4 68
#define A4 69
#define As4 70
#define B4 71

#define C5 72
#define Cs5 73
#define D5 74
#define Ds5 75
#define E5 76
#define F5 77
#define Fs5 78
#define G5 79
#define Gs5 80
#define A5 81
#define As5 82
#define B5 83

#define C6 84
#define Cs6 85
#define D6 86
#define Ds6 87
#define E6 88
#define F6 89
#define Fs6 90
#define G6 91
#define Gs6 92
#define A6 93
#define As6 94
#define B6 95

#define C7 96
#define Cs7 97
#define D7 98
#define Ds7 99
#define E7 100
#define F7 101
#define Fs7 102
#define G7 103
#define Gs7 104
#define A7 105
#define As7 106
#define B7 107

#define C8 108
#define Cs8 109
#define D8 110
#define Ds8 111
#define E8 112
#define F8 113
#define Fs8 114
#define G8 115
#define Gs8 116
#define A8 117
#define As8 118
#define B8 119

#define C9 120
#define Cs9 121
#define D9 122
#define Ds9 123
#define E9 124
#define F9 125
#define Fs9 126
#define G9 127

extern QueueHandle_t Q3;
class Notes;

void octaveClear(uint8_t octave, Notes (&note)[96]);
void keyboard_check(bool keys[56], uint8_t octave, Notes (&note)[96]);
uint8_t keyInterp(bool keys[56], uint8_t octave);

class Notes
{
    public:
    uint8_t note;
    uint8_t state = 0;
    // State 0 - Nil
    //       1 - Send On
    //       2 - Sent on
    //       3 - Send off
};