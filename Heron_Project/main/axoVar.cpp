#include "axoVar.h"

// Simple function to ensure we don't wrap around when inc/decrementing
void axoVar::incVal(bool direction)
{
    if(direction == 0)
    {
        if(this->val == 0){return;}
        else{this->val--;}
    }
    else if(direction == 1)
    {
        if(this->val == 127){return;}
        else{this->val = this->val+1;}
    }
}