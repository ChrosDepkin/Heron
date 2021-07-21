/*
    Code for configuring UART and sending MIDI messages.
    Config function configures two UART ports, and a test message is sent on UART0.
    Have only testing sending a message on UART0, and not receiving.
    Works fine.
*/
extern "C" {                    // Need to put these includes in here to make it work
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

void app_main(void);           // main needs to be in here too or it won't compile
}

#include "MIDIdefs.h" // Defines for various MIDI/UART things
#include "driver/uart.h" // UART driver
#include "rom/ets_sys.h" // Just for the delay used in main for testing

void UART_Config();

void app_main(void)
{
    UART_Config(); // Configures two UART ports
    char data[3] = {(CTRLCHANGE | 1), 20, 60}; // Store a full MIDI message here (Status byte - data byte - data byte). Status byte should be message type OR'd with MIDI channel
                                       // Note that not all MIDI messages are 3 bytes long
                                       // Also note that this is scaled for axoloti patches (eg send a value of 60 and the dial in the patch will go to 30 for a positive dial, or -4 for a bipolar)
    
    while(1) // Testing loop
    {
    uart_write_bytes(UART_NUM_0, data, 3); // Write the data stored in 'data' (above) on UART0, length of 3.
                                           // If you only want to send two bytes, store those in the first two bytes of 'data' array and change length value to 2
    ets_delay_us(500000); // Wait 500ms
    }
}





// Function for configuring UART controllers
void UART_Config()
{

// Doing UART0 first
const uart_port_t uart_num0 = UART_NUM_0;

// Create a config type with all the settings we want
uart_config_t uart0 = {
    .baud_rate = UART_0_BAUD, // Defined this in MIDIdefs.h
    .data_bits = UART_DATA_8_BITS, // The rest are defaults
    .parity = UART_PARITY_DISABLE, 
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, // Except disabled flowcontrol (only using TX/RX pins)
    .rx_flow_ctrl_thresh = 122,
};

uart_param_config(uart_num0, &uart0); // Associate the config we just defined above with UART0

uart_set_pin(UART_NUM_0, UART_0_TX, UART_0_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE); // Set the TX/RX pins for UART 0, 'no change' on flow control pins (Hopefully using them for other things is fine? There are defaults)

QueueHandle_t uart0_queue; // Presumably handles UART queue

uart_driver_install(UART_NUM_0, UART_0_BUF_SIZE, UART_0_BUF_SIZE, 10, &uart0_queue, 0); // Finally install the driver
                    // UART Number - TX Buffer - RX Buffer - Queue Size(?) - Queue - Flags
                    // Buffer size defined by me in MIDIdefs.h



// As above but for UART2
const uart_port_t uart_num2 = UART_NUM_2;

uart_config_t uart2 = {
    .baud_rate = UART_2_BAUD,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .rx_flow_ctrl_thresh = 122,
};

uart_param_config(uart_num2, &uart2);

uart_set_pin(UART_NUM_2, UART_2_TX, UART_2_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

QueueHandle_t uart2_queue;

uart_driver_install(UART_NUM_2, UART_2_BUF_SIZE, UART_2_BUF_SIZE, 10, &uart2_queue, 0);

}