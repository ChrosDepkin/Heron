/*
   Test code for FreeRTOS on ESP32:
   I copied over the MIDI/UART code I was working on to test out running it as OS tasks
   The UART config task runs once then deletes itself, the MIDIsend task runs every 100 ticks.
   Seems to be working fine.
*/
extern "C" {                    // Need to put these includes in here to make it work
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

void app_main(void);           // main needs to be in here too or it won't compile
}

#include "MIDIdefs.h" // Copied from current version of MIDI code
#include "driver/UART.h"

// Function prototypes
void MIDIsend(void *pvParameter);
void UART_Config(void *pvParameter);

void app_main(void)
{
   TaskHandle_t UARTconf = NULL; // Handles used to reference our tasks
   TaskHandle_t MIDItask = NULL; // Values are assigned in the xTaskCreate function

   xTaskCreate(UART_Config, "UARTconfig", 2048, ( void * ) 1, tskIDLE_PRIORITY, &UARTconf); // Function to create a FreeRTOS task
               // Function containing the code - Name just for debugging - Stack Size - Parameter passed to it (not needed here) - Priority - Handle variable
   xTaskCreate(MIDIsend, "MIDIsend", 2048, ( void * ) 1, tskIDLE_PRIORITY, &MIDItask);
               // This function gives the task permission to run on both cores - can specify with xTaskCreatePinnedToCore()
}







// Send a MIDI message over UART
void MIDIsend(void *pvParameter)
{
   for(;;) // Tasks should never be allowed to return, so stick them in an endless loop or delete at the end
   {
   char data[3] = {(CTRLCHANGE | 1), 20, 60};
   uart_write_bytes(UART_NUM_0, data, 3);
   vTaskDelay(100); // Wait 100 ticks before sending again (seems to be about 1 second)
   }
}

// Config UART so we can send a message (copied from MIDI code just to test RTOS)
void UART_Config(void *pvParameter)
{
const uart_port_t uart_num0 = UART_NUM_0;
uart_config_t uart0 = {
    .baud_rate = UART_0_BAUD,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_DISABLE, 
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .rx_flow_ctrl_thresh = 122,
};
uart_param_config(uart_num0, &uart0);
uart_set_pin(UART_NUM_0, UART_0_TX, UART_0_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
QueueHandle_t uart0_queue;
uart_driver_install(UART_NUM_0, UART_0_BUF_SIZE, UART_0_BUF_SIZE, 10, &uart0_queue, 0);

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

vTaskDelete(NULL); // We don't want this to keep going, so delete the task once config is done
                   // As opposed to the sending task above, which repeats every 100 ticks.
}