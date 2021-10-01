#include "Keyboard.h"

void octaveClear(uint8_t octave, Notes (&note)[96])
{
    // Deal with notes outside of octave
for (int i = 0; i < 96; i++)
{
    if(i < 12) // Octave 0 area
    {
        if(octave != 0) // If not in current octave (ie key cannot be held down for this note)
        {
            if((note[i].state == 1) | (note[i].state == 2)) // If note on was sent last round, or earlier
            {
                note[i].state = 3; // Send note off
            }
            else if(note[i].state == 3) // If note off was sent
            {
                note[i].state = 0; // Set to nil
            }
        }
    }
    else if(i < 24) // Octave 0 area
    {
        if((octave != 0) & (octave != 1)) // If not in current octave (ie key cannot be held down for this note)
        {
            if((note[i].state == 1) | (note[i].state == 2)) // If note on was sent last round, or earlier
            {
                note[i].state = 3; // Send note off
            }
            else if(note[i].state == 3) // If note off was sent
            {
                note[i].state = 0; // Set to nil
            }
        }
    }
    else if(i < 36) // Octave 1 area
    {
        if((octave != 1) & (octave != 2))
        {
            if((note[i].state == 1) | (note[i].state == 2))
            {
                note[i].state = 3;
            }
            else if(note[i].state == 3)
            {
                note[i].state = 0;
            }
        }
    }
    else if(i < 48) // Etc
    {
        if((octave != 2) & (octave != 3))
        {
            if((note[i].state == 1) | (note[i].state == 2))
            {
                note[i].state = 3;
            }
            else if(note[i].state == 3)
            {
                note[i].state = 0;
            }
        }
    }
    else if(i < 60)
    {
        if((octave != 3) & (octave != 4))
        {
            if((note[i].state == 1) | (note[i].state == 2))
            {
                note[i].state = 3;
            }
            else if(note[i].state == 3)
            {
                note[i].state = 0;
            }
        }
    }
    else if(i < 72)
    {
        if((octave != 4) & (octave != 5))
        {
            if((note[i].state == 1) | (note[i].state == 2))
            {
                note[i].state = 3;
            }
            else if(note[i].state == 3)
            {
                note[i].state = 0;
            }
        }
    }
    else if(i < 84)
    {
        if((octave != 5) & (octave != 6))
        {
            if((note[i].state == 1) | (note[i].state == 2))
            {
                note[i].state = 3;
            }
            else if(note[i].state == 3)
            {
                note[i].state = 0;
            }
        }
    }
    else
    {
        if((octave != 6) & (octave != 7))
        {
            if((note[i].state == 1) | (note[i].state == 2))
            {
                note[i].state = 3;
            }
            else if(note[i].state == 3)
            {
                note[i].state = 0;
            }
        }
    }

}
}

void keyboard_check(bool keys[56], uint8_t octave, Notes (&note)[96])
{
// Used to set octave range for note array
int j = 0; 
int k = 0;

switch(octave) // Check what octave we're in
{
    case 0:
        j = 0; // Set search boundaries accordingly
        k = 24;// Used in for loop below
        break;
    case 1:
        j = 12;
        k = 36;
        break;
    case 2:
        j = 24;
        k = 48;
        break;
    case 3:
        j = 36;
        k = 60;
        break;
    case 4:
        j = 48;
        k = 72;
        break;
    case 5:
        j = 60;
        k = 84;
        break;
    case 6:
        j = 72;
        k = 96;
        break;
}

for (int i = 0; j < k; i++) // For uses i for stepping through keys, and j/k for end condition and note indexing
{
    switch(i)
    {
        case 0: // For first note in this octave section
            if(keys[KEY1] == 1) // If key 1 pressed
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;} // If note is either nil or note off last check, set to note on
                else{note[j].state = 2;} // Else note on has already been sent, so hold
            }
            else // If key 1 was not pressed 
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;} // If note on or hold last check, key has been released, so set to note off
                else{note[j].state = 0;} // Else note off has already been sent, so set to nil
            }
            break;
        case 1: // Etc for each key
            if(keys[KEY17] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 2:
            if(keys[KEY2] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 3:
            if(keys[KEY18] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 4:
            if(keys[KEY3] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 5:
            if(keys[KEY4] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 6:
            if(keys[KEY20] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 7:
            if(keys[KEY5] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 8:
            if(keys[KEY21] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 9:
            if(keys[KEY6] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 10:
            if(keys[KEY22] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 11:
            if(keys[KEY7] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 12:
            if(keys[KEY8] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 13:
            if(keys[KEY24] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 14:
            if(keys[KEY9] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 15:
            if(keys[KEY25] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 16:
            if(keys[KEY10] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 17:
            if(keys[KEY11] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 18:
            if(keys[KEY27] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 19:
            if(keys[KEY12] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 20:
            if(keys[KEY28] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 21:
            if(keys[KEY13] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 22:
            if(keys[KEY29] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
        case 23:
            if(keys[KEY14] == 1)
            {
                if((note[j].state == 0) | (note[j].state == 3)){note[j].state = 1;}
                else{note[j].state = 2;}
            }
            else
            {
                if((note[j].state == 1) | (note[j].state == 2)){note[j].state = 3;}
                else{note[j].state = 0;}
            }
            break;
    }
    j++; // Increment j value for array index
}


}





uint8_t keyInterp(bool keys[56], uint8_t octave)
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

















