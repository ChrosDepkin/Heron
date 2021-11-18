#include "MIDIsys.h"
#include <stdint.h> // Needed to include this here for uint8_t types for no reason one day
#include "driver/UART.h"

void MIDI_send(uint8_t type, uint8_t channel, uint8_t val1, uint8_t val2)
{
    char b1 = type | channel;
    char data[3] = {b1, val1, val2};

   
    
    if((type == CHNLPRESSURE) | (type == PRGCHANGE))
    {
        uart_write_bytes(UART_NUM_0, data, 2);
    }
    else if((type == NOTE_ON) | (type == NOTE_OFF))
    {
        uart_write_bytes(UART_NUM_0, data, 3);

    }
    else
    {
        uart_write_bytes(UART_NUM_0, data, 3);
    }

}