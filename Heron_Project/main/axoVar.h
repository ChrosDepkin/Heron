#ifndef AXOVAR_H
#define AXOVAR_H

#include <stdint.h> // Needed to include this here for uint8_t types for no reason one day

#define CHKMASK 0b1000000000000000
#define ENCMASK 0b0000111000000000
#define COMMASK 0b0000000110000000
#define VALMASK 0b0000000001111111

#define RAWCOM 0b1000000000000000
#define INCCOM 0b1000000010000000
#define CCCOM  0b1000000100000000
#define NULCOM 0b1000000110000000

#define ENC1 0b0000000000000000
#define ENC2 0b0000010000000000
#define ENC3 0b0000100000000000
#define ENC4 0b0000110000000000
#define ENC5 0b0001000000000000
#define ENC6 0b0001010000000000
#define ENC7 0b0001100000000000
#define ENC8 0b0001110000000000

class axoVar
{

    public:
    uint8_t val = 0; // What is the current value?
    uint8_t CC = 0; // What CC number are you controlling?
    bool type = 0; // Unipolar/Bipolar on axo? Uni 0, Bi 1
    void incVal(bool direction);
    void incCC(bool direction);

};


#endif