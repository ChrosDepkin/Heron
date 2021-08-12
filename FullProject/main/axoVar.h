#include <stdint.h> // Needed to include this here for uint8_t types for no reason one day

#define CHKMASK 0b1000000000000000
#define ENCMASK 0b0000111000000000
#define COMMASK 0b0000000110000000
#define VALMASK 0b0000000001111111

class axoVar
{

    public:
    uint8_t val = 0; // What is the current value?
    uint8_t CC = 0; // What CC number are you controlling?
    bool type = 0; // Unipolar/Bipolar on axo? Uni 0, Bi 1
    void incVal(bool direction);

};