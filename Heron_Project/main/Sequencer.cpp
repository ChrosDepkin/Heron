#include "Sequencer.h"


void TrackClass::stepInc(uint32_t& Q3buff)
{
    this->position++; // Increment our position

    if(this->position > trackLen) // If we've reached end of the track
    {
        this->position = 0; // Go back to start
        for (int i = trackLen; i < 15; i++) // Set all seq1 above length and last LED off
        {
            if((this->steps&(1 << i)) == 0){seq[this->tB][i] = CRGB::Black;}
        }
    } 
    if((this->steps&(1 << (this->position-1)))) // If last step was activated
    {
        /* Then send Note Off */
        Q3buff = 0;
        Q3buff = NOTE_OFF | (this->d1 << 8) | (this->d2 << 16);
        xQueueSend(Q3,&Q3buff,10);
        vTaskResume(MIDI);
    }
    if((this->steps&(1 << this->position))) // If current step is activated
    {
        /* Then send the message */
        Q3buff = 0;
        Q3buff = this->type | (this->d1 << 8) | (this->d2 << 16);
        xQueueSend(Q3,&Q3buff,10);
        vTaskResume(MIDI);

        //MIDI_send(this->type, channel, this->d1, this->d2); // Send the assigned MIDI message for this track
        /* Also probably do a different LED colour */
        if((this->steps&(1 << (this->position-1)))){seq[this->tB][this->position-1] = CRGB::Red;}
        else{seq[this->tB][this->position-1] = CRGB::Black;}
        seq[this->tB][this->position] = CRGB::Green;
    }
    else
    {
        if((this->steps&(1 << (this->position-1)))){seq[this->tB][this->position-1] = CRGB::Red;}
        else{seq[this->tB][this->position-1] = CRGB::Black;}
        seq[this->tB][this->position] = CRGB::Blue;
    }

}

void TrackClass::velInc(bool direction)
{
    if(direction == 0)
    {
        if(this->d2 == 0){return;}
        else{this->d2--;}
    }
    else if(direction == 1)
    {
        if(this->d2 == 127){return;}
        else{this->d2 = this->d2+1;}
    }
}