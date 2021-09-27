#include "Keyboard.h"

uint8_t keyboard_check(bool keys[54], uint8_t octave)
{
    if(keys[KEY1] == 1)
    {     
        switch (octave)
            {
            case 0:
                return C1;
                break;
            case 1:
                return C2;
                break;
            case 2:
                return C3;
                break;
            case 3:
                return C4;
                break;
            case 4:
                return C5;
                break;
            case 5:
                return C6;
                break;
            case 6:
                return C7;
                break;
            case 7:
                return C8;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY17] == 1)
    {
        switch (octave)
            {
            case 0:
                return Cs1;
                break;
            case 1:
                return Cs2;
                break;
            case 2:
                return Cs3;
                break;
            case 3:
                return Cs4;
                break;
            case 4:
                return Cs5;
                break;
            case 5:
                return Cs6;
                break;
            case 6:
                return Cs7;
                break;
            case 7:
                return Cs8;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY2] == 1)
    {
        switch (octave)
            {
            case 0:
                return D1;
                break;
            case 1:
                return D2;
                break;
            case 2:
                return D3;
                break;
            case 3:
                return D4;
                break;
            case 4:
                return D5;
                break;
            case 5:
                return D6;
                break;
            case 6:
                return D7;
                break;
            case 7:
                return D8;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY18] == 1)
    {
        switch (octave)
            {
            case 0:
                return Ds1;
                break;
            case 1:
                return Ds2;
                break;
            case 2:
                return Ds3;
                break;
            case 3:
                return Ds4;
                break;
            case 4:
                return Ds5;
                break;
            case 5:
                return Ds6;
                break;
            case 6:
                return Ds7;
                break;
            case 7:
                return Ds8;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY3] == 1)
    {
        switch (octave)
            {
            case 0:
                return E1;
                break;
            case 1:
                return E2;
                break;
            case 2:
                return E3;
                break;
            case 3:
                return E4;
                break;
            case 4:
                return E5;
                break;
            case 5:
                return E6;
                break;
            case 6:
                return E7;
                break;
            case 7:
                return E8;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY4] == 1)
    {
        switch (octave)
            {
            case 0:
                return F1;
                break;
            case 1:
                return F2;
                break;
            case 2:
                return F3;
                break;
            case 3:
                return F4;
                break;
            case 4:
                return F5;
                break;
            case 5:
                return F6;
                break;
            case 6:
                return F7;
                break;
            case 7:
                return F8;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY20] == 1)
    {
        switch (octave)
            {
            case 0:
                return Fs1;
                break;
            case 1:
                return Fs2;
                break;
            case 2:
                return Fs3;
                break;
            case 3:
                return Fs4;
                break;
            case 4:
                return Fs5;
                break;
            case 5:
                return Fs6;
                break;
            case 6:
                return Fs7;
                break;
            case 7:
                return Fs8;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY5] == 1)
    {
        switch (octave)
            {
            case 0:
                return G1;
                break;
            case 1:
                return G2;
                break;
            case 2:
                return G3;
                break;
            case 3:
                return G4;
                break;
            case 4:
                return G5;
                break;
            case 5:
                return G6;
                break;
            case 6:
                return G7;
                break;
            case 7:
                return G8;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY21] == 1)
    {
        switch (octave)
            {
            case 0:
                return Gs1;
                break;
            case 1:
                return Gs2;
                break;
            case 2:
                return Gs3;
                break;
            case 3:
                return Gs4;
                break;
            case 4:
                return Gs5;
                break;
            case 5:
                return Gs6;
                break;
            case 6:
                return Gs7;
                break;
            case 7:
                return Gs8;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY6] == 1)
    {
        switch (octave)
            {
            case 0:
                return A1;
                break;
            case 1:
                return A2;
                break;
            case 2:
                return A3;
                break;
            case 3:
                return A4;
                break;
            case 4:
                return A5;
                break;
            case 5:
                return A6;
                break;
            case 6:
                return A7;
                break;
            case 7:
                return A8;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY22] == 1)
    {
        switch (octave)
            {
            case 0:
                return As1;
                break;
            case 1:
                return As2;
                break;
            case 2:
                return As3;
                break;
            case 3:
                return As4;
                break;
            case 4:
                return As5;
                break;
            case 5:
                return As6;
                break;
            case 6:
                return As7;
                break;
            case 7:
                return As8;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY7] == 1)
    {
        switch (octave)
            {
            case 0:
                return B1;
                break;
            case 1:
                return B2;
                break;
            case 2:
                return B3;
                break;
            case 3:
                return B4;
                break;
            case 4:
                return B5;
                break;
            case 5:
                return B6;
                break;
            case 6:
                return B7;
                break;
            case 7:
                return B8;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY8] == 1)
    {
        switch (octave)
            {
            case 0:
                return C2;
                break;
            case 1:
                return C3;
                break;
            case 2:
                return C4;
                break;
            case 3:
                return C5;
                break;
            case 4:
                return C6;
                break;
            case 5:
                return C7;
                break;
            case 6:
                return C8;
                break;
            case 7:
                return C9;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY24] == 1)
    {
        switch (octave)
            {
            case 0:
                return Cs2;
                break;
            case 1:
                return Cs3;
                break;
            case 2:
                return Cs4;
                break;
            case 3:
                return Cs5;
                break;
            case 4:
                return Cs6;
                break;
            case 5:
                return Cs7;
                break;
            case 6:
                return Cs8;
                break;
            case 7:
                return Cs9;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY9] == 1)
    {
        switch (octave)
            {
            case 0:
                return D2;
                break;
            case 1:
                return D3;
                break;
            case 2:
                return D4;
                break;
            case 3:
                return D5;
                break;
            case 4:
                return D6;
                break;
            case 5:
                return D7;
                break;
            case 6:
                return D8;
                break;
            case 7:
                return D9;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY25] == 1)
    {
        switch (octave)
            {
            case 0:
                return Ds2;
                break;
            case 1:
                return Ds3;
                break;
            case 2:
                return Ds4;
                break;
            case 3:
                return Ds5;
                break;
            case 4:
                return Ds6;
                break;
            case 5:
                return Ds7;
                break;
            case 6:
                return Ds8;
                break;
            case 7:
                return Ds9;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY10] == 1)
    {
        switch (octave)
            {
            case 0:
                return E2;
                break;
            case 1:
                return E3;
                break;
            case 2:
                return E4;
                break;
            case 3:
                return E5;
                break;
            case 4:
                return E6;
                break;
            case 5:
                return E7;
                break;
            case 6:
                return E8;
                break;
            case 7:
                return E9;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY11] == 1)
    {
        switch (octave)
            {
            case 0:
                return F2;
                break;
            case 1:
                return F3;
                break;
            case 2:
                return F4;
                break;
            case 3:
                return F5;
                break;
            case 4:
                return F6;
                break;
            case 5:
                return F7;
                break;
            case 6:
                return F8;
                break;
            case 7:
                return F9;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY27] == 1)
    {
        switch (octave)
            {
            case 0:
                return Fs2;
                break;
            case 1:
                return Fs3;
                break;
            case 2:
                return Fs4;
                break;
            case 3:
                return Fs5;
                break;
            case 4:
                return Fs6;
                break;
            case 5:
                return Fs7;
                break;
            case 6:
                return Fs8;
                break;
            case 7:
                return Fs9;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY12] == 1)
    {
        switch (octave)
            {
            case 0:
                return G2;
                break;
            case 1:
                return G3;
                break;
            case 2:
                return G4;
                break;
            case 3:
                return G5;
                break;
            case 4:
                return G6;
                break;
            case 5:
                return G7;
                break;
            case 6:
                return G8;
                break;
            case 7:
                return G9;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY28] == 1)
    {
        switch (octave)
            {
            case 0:
                return Gs2;
                break;
            case 1:
                return Gs3;
                break;
            case 2:
                return Gs4;
                break;
            case 3:
                return Gs5;
                break;
            case 4:
                return Gs6;
                break;
            case 5:
                return Gs7;
                break;
            case 6:
                return Gs8;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY13] == 1)
    {
        switch (octave)
            {
            case 0:
                return A2;
                break;
            case 1:
                return A3;
                break;
            case 2:
                return A4;
                break;
            case 3:
                return A5;
                break;
            case 4:
                return A6;
                break;
            case 5:
                return A7;
                break;
            case 6:
                return A8;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY29] == 1)
    {
        switch (octave)
            {
            case 0:
                return As2;
                break;
            case 1:
                return As3;
                break;
            case 2:
                return As4;
                break;
            case 3:
                return As5;
                break;
            case 4:
                return As6;
                break;
            case 5:
                return As7;
                break;
            case 6:
                return As8;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY14] == 1)
    {
        switch (octave)
            {
            case 0:
                return B2;
                break;
            case 1:
                return B3;
                break;
            case 2:
                return B4;
                break;
            case 3:
                return B5;
                break;
            case 4:
                return B6;
                break;
            case 5:
                return B7;
                break;
            case 6:
                return B8;
                break;
            
            default:
                break;
            }
    }

    else if(keys[KEY15] == 1)
    {
        switch (octave)
            {
            case 0:
                return C3;
                break;
            case 1:
                return C4;
                break;
            case 2:
                return C5;
                break;
            case 3:
                return C6;
                break;
            case 4:
                return C7;
                break;
            case 5:
                return C8;
                break;
            case 6:
                return C9;
                break;
            default:
                break;
            }
    }

    else if(keys[KEY31] == 1)
    {
        switch (octave)
            {
            case 0:
                return Cs3;
                break;
            case 1:
                return Cs4;
                break;
            case 2:
                return Cs5;
                break;
            case 3:
                return Cs6;
                break;
            case 4:
                return Cs7;
                break;
            case 5:
                return Cs8;
                break;
            case 6:
                return Cs9;
                break;
            default:
                break;
            }
    }

    else if(keys[KEY16] == 1)
    {
        switch (octave)
            {
            case 0:
                return D3;
                break;
            case 1:
                return D4;
                break;
            case 2:
                return D5;
                break;
            case 3:
                return D6;
                break;
            case 4:
                return D7;
                break;
            case 5:
                return D8;
                break;
            case 6:
                return D9;
                break;
            default:
                break;
            }
    }

    else if(keys[KEY32] == 1)
    {
        switch (octave)
            {
            case 0:
                return Ds3;
                break;
            case 1:
                return Ds4;
                break;
            case 2:
                return Ds5;
                break;
            case 3:
                return Ds6;
                break;
            case 4:
                return Ds7;
                break;
            case 5:
                return Ds8;
                break;
            case 6:
                return Ds9;
                break;
            default:
                break;
            }
    }
return 1;
}

























