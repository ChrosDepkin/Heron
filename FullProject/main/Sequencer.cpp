#include "Sequencer.h"


void track::stepInc(uint32_t& Q3buff)
{
    this->position++; // Increment our position

    if(this->position > this->length) // If we've reached end of the track
    {
        this->position = 0; // Go back to start
        for (int i = this->length; i < this->length; i++) // Set all seq1 above length and last LED off
        {
            if((this->steps&(1 << i)) == 0){seq[this->bank][i] = CRGB::Black;}
        }
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
        if((this->steps&(1 << (this->position-1)))){seq[this->bank][this->position-1] = CRGB::Red;}
        else{seq[this->bank][this->position-1] = CRGB::Black;}
        seq[this->bank][this->position] = CRGB::Green;
    }
    else
    {
        if((this->steps&(1 << (this->position-1)))){seq[this->bank][this->position-1] = CRGB::Red;}
        else{seq[this->bank][this->position-1] = CRGB::Black;}
        seq[this->bank][this->position] = CRGB::Blue;
    }

}