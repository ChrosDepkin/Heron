/* 
    This is meant to be the main project - combining all other elements worked on separately.

*/
extern "C" {                    // Need to put these includes in here to make it work
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/UART.h"
#include "esp_log.h" // Just used for logging stuff

void app_main(void);           // main needs to be in here too or it won't compile
}

#include "expanders.h"
#include "I2Cdefs.h"
#include "MIDIdefs.h"
#include "axoVar.h"



// Prototypes
void I2C_Config(void *pvParameter);
void Encoder_Task(void *pvParameter);
void Key_Task(void *pvParameter);
void MIDI_Send(void *pvParameter);
void UART_Config(void *pvParameter);

// Queues for sending data between tasks
QueueHandle_t Q1; // Using this to test sending data from reading encoders to sending to Axo
QueueHandle_t Q2; // Using this to test sending data from button matrix to encoder task

// Things that don't go anywhere yet
MCP MCP_M(EXP_ADR_M, MCP_DEF_CONFIG, DIR_PA_M, DIR_PB_M, PU_PA_M, PU_PB_M);     // Misc. IO expander

void app_main(void)
{
    // Make a handle for each OS task
    TaskHandle_t I2Cconfig = NULL;
    TaskHandle_t UARTconfig = NULL;
    TaskHandle_t Enc = NULL;
    TaskHandle_t Key = NULL;
    TaskHandle_t MIDI = NULL;


    Q1 = xQueueCreate(8, sizeof(uint8_t)); // Create a queue 8 items long (chose arbitrarily) each item 8 bits long
                                           // Using this to update the value sent by UART when an encoder is turned
    Q2 = xQueueCreate(8, sizeof(uint8_t));

    axoVar vars[32];

    for (int i = 0; i < 32; i++)
    {
        if(i<=7){vars[i].bank = 0;}
        else if(i<=15){vars[i].bank = 1;}
        else if(i<=23){vars[i].bank = 2;}
        else if(i<=31){vars[i].bank = 3;}
    }
    

    // Create each task for the OS
    xTaskCreate(I2C_Config, "I2Cconfig", 2048, NULL, 5, &I2Cconfig);
    xTaskCreate(UART_Config, "UARTconfig", 2048, NULL, 4, &UARTconfig);
    xTaskCreate(Encoder_Task, "EncTask", 2048, NULL, 3, &Enc);
    xTaskCreate(Key_Task, "KeyTask", 2048, NULL, 3, &Key);
    xTaskCreate(MIDI_Send, "MIDIsend", 2048, NULL, 1, &MIDI);
    // Pass in Function - Name (just for debug)- Stack size - Parameters - Priority - Handle
    // Haven't done anything with stack depth or parameters for now
    // This function gives the task permission to run on both cores - can specify cores with xTaskCreatePinnedToCore()
    
}


// Function for encoder task
void Encoder_Task(void *pvParameter)
{
    MCPE MCP_E(EXP_ADR_E, MCP_DEF_CONFIG, DIR_PA_E, DIR_PB_E, PU_PA_E, PU_PB_E); // Create encoder expander object

    MCP_E.setup(); // Setup function for that expander

    // Encoder debug tags
    static const char* TAG1 = "EN1";
    static const char* TAG2 = "EN2";
    static const char* TAG3 = "EN3";
    static const char* TAG4 = "EN4";
    static const char* TAG5 = "EN5";
    static const char* TAG6 = "EN6";
    static const char* TAG7 = "EN7";
    static const char* TAG8 = "EN8";

    

    for(;;) // Run this task continuously
    {
        MCP_E.encoderRead(); // Read encoder values
        xQueueReceive(Q2,(void *) &MCP_E.bankstate,10); // Read changes to bank value
        xQueueSend(Q1,(void *) &MCP_E.Turn[7],10); // Send turn value of encoder 1 to queue, max wait 10 ticks

        if(MCP_E.bankstate == 0)
        {
         if(MCP_E.Turn[0] == 1) // If turn value is 1
            {ESP_LOGI(TAG8, "B1 - Right");}  // Then encoder has turned right
         if(MCP_E.Turn[0] == 2) // If turn value is 2
            {ESP_LOGI(TAG8, "B1 - Left");}  // Then encoder has turned left
        }
        if(MCP_E.bankstate == 1)
        {
         if(MCP_E.Turn[0] == 1) // If turn value is 1
            {ESP_LOGI(TAG8, "B2 - Right");}  // Then encoder has turned right
         if(MCP_E.Turn[0] == 2) // If turn value is 2
            {ESP_LOGI(TAG8, "B2 - Left");}  // Then encoder has turned left
        }
        if(MCP_E.bankstate == 2)
        {
         if(MCP_E.Turn[0] == 1) // If turn value is 1
            {ESP_LOGI(TAG8, "B3 - Right");}  // Then encoder has turned right
         if(MCP_E.Turn[0] == 2) // If turn value is 2
            {ESP_LOGI(TAG8, "B3 - Left");}  // Then encoder has turned left
        }
        if(MCP_E.bankstate == 3)
        {
         if(MCP_E.Turn[0] == 1) // If turn value is 1
            {ESP_LOGI(TAG8, "B4 - Right");}  // Then encoder has turned right
         if(MCP_E.Turn[0] == 2) // If turn value is 2
            {ESP_LOGI(TAG8, "B4 - Left");}  // Then encoder has turned left
        }

        /*
        // Encoder loggings
        if(MCP_E.Turn[0] == 1) // If turn value is 1
            {ESP_LOGI(TAG8, "Right");}  // Then encoder has turned right
         if(MCP_E.Turn[0] == 2) // If turn value is 2
            {ESP_LOGI(TAG8, "Left");}  // Then encoder has turned left
        if(MCP_E.Turn[1] == 1)  // Etc.
            {ESP_LOGI(TAG7, "Right");} 
         if(MCP_E.Turn[1] == 2) 
            {ESP_LOGI(TAG7, "Left");} 
        if(MCP_E.Turn[2] == 1) 
            {ESP_LOGI(TAG6, "Right");} 
         if(MCP_E.Turn[2] == 2) 
            {ESP_LOGI(TAG6, "Left");} 
        if(MCP_E.Turn[3] == 1) 
            {ESP_LOGI(TAG5, "Right");} 
         if(MCP_E.Turn[3] == 2) 
            {ESP_LOGI(TAG5, "Left");} 
        if(MCP_E.Turn[4] == 1) 
            {ESP_LOGI(TAG4, "Right");} 
         if(MCP_E.Turn[4] == 2) 
            {ESP_LOGI(TAG4, "Left");} 
        if(MCP_E.Turn[5] == 1) 
            {ESP_LOGI(TAG3, "Right");} 
         if(MCP_E.Turn[5] == 2) 
            {ESP_LOGI(TAG3, "Left");} 
        if(MCP_E.Turn[6] == 1) 
            {ESP_LOGI(TAG2, "Right");} 
         if(MCP_E.Turn[6] == 2) 
            {ESP_LOGI(TAG2, "Left");} 
        if(MCP_E.Turn[7] == 1) 
            {ESP_LOGI(TAG1, "Right");} 
         if(MCP_E.Turn[7] == 2) 
            {ESP_LOGI(TAG1, "Left");} 
        */
    }
}


// Function for button matrix task
void Key_Task(void *pvParameter)
{
    MCPB MCP_B(EXP_ADR_B, MCP_DEF_CONFIG, DIR_PA_B, DIR_PB_B, PU_PA_B, PU_PB_B); // Create matrix expander object

    MCP_B.setup(); // Setup function for matrix expander
    uint8_t bankB = 0; // buffer for sending bank changes to encoder task
    uint8_t mode = 0; // Used for tracking key 'mode'. Currently just for bank mode

    // Key debug tags (don't need all keys)
    static const char* K1 = "K1";
    static const char* K2 = "K2";
    static const char* K3 = "K3";
    static const char* K4 = "K4";
    static const char* K5 = "K5";
    static const char* K6 = "K6";
    static const char* K7 = "K7";
    static const char* K8 = "K8";

    while(1)
    {
        MCP_B.matrixRead(); // Read the button matrix

        if(MCP_B.matrixState[54]){mode = 1;} // B16
        if(MCP_B.matrixState[38]){mode = 0;} // B15

        if(mode == 1)
        {
        if(MCP_B.matrixState[0] == 1)
            {ESP_LOGI(K1, "Bank 1"); bankB = 0;} 
        if(MCP_B.matrixState[8] == 1)
            {ESP_LOGI(K2, "Bank 2"); bankB = 1;} 
        if(MCP_B.matrixState[16] == 1)
            {ESP_LOGI(K3, "Bank 3"); bankB = 2;} 
        if(MCP_B.matrixState[24] == 1)
            {ESP_LOGI(K4, "Bank 4"); bankB = 3;} 
        xQueueSend(Q2,(void *) &bankB,10);
        }

        /*
        // Key Logging (only checking first 8 keys)
        if(MCP_B.matrixState[0] == 1)
            {ESP_LOGI(K1, "Key 1");} 
        if(MCP_B.matrixState[8] == 1)
            {ESP_LOGI(K2, "Key 2");} 
        if(MCP_B.matrixState[16] == 1)
            {ESP_LOGI(K3, "Key 3");} 
        if(MCP_B.matrixState[24] == 1)
            {ESP_LOGI(K4, "Key 4");} 
        if(MCP_B.matrixState[32] == 1)
            {ESP_LOGI(K5, "Key 5");} 
        if(MCP_B.matrixState[40] == 1)
            {ESP_LOGI(K6, "Key 6");} 
        if(MCP_B.matrixState[48] == 1)
            {ESP_LOGI(K7, "Key 7");} 
        if(MCP_B.matrixState[1] == 1)
            {ESP_LOGI(K8, "Key 8");} 
        */


        xQueueSend(Q2,(void *) &bankB,10);
    }
}

// Function for sending MIDI data to Axoloti via UART0
void MIDI_Send(void *pvParameter)
{
    char data[3] = {(CTRLCHANGE | 1), 20, 0}; // Store a full MIDI message here (Status byte - data byte - data byte). Status byte should be message type OR'd with MIDI channel
                                       // Note that not all MIDI messages are 3 bytes long
                                       // Also note that this is scaled for axoloti patches (eg send a value of 60 and the dial in the patch will go to 30 for a positive dial, or -4 for a bipolar)
    uint8_t buff = 0; // Buffer to store data from queue (not much of a buffer, just trying it out)
    for(;;) // Tasks should never be allowed to return, so stick them in an endless loop or delete at the end
    {
        xQueueReceive(Q1,(void *) &buff,10); // Get the data from queue
        if(buff == 1) // If 1 - turned right
        {data[2]++; uart_write_bytes(UART_NUM_0, data, 3);} // Increase value and send
        if(buff == 2) // If 2 - turned left
        {data[2]--; uart_write_bytes(UART_NUM_0, data, 3);} // Decrease value and send
        //vTaskDelay(100); // Wait 1 ticks before sending again (seems to be about 1 second)
    }
}





// Function for configuring I2C controllers
void I2C_Config(void *pvParameter)
{
    //Below I create two configs for a master and a slave I2C
    i2c_config_t i2cM; //Config structure for first I2C
        i2cM.mode = I2C_MODE_MASTER; //Set to master
        i2cM.sda_io_num = I2C_M_SDA; //SDA pin 
        i2cM.scl_io_num = I2C_M_SCL; //SCL pin 
        i2cM.sda_pullup_en = GPIO_PULLUP_ENABLE; //Enable internal pullup on SDA pin
        i2cM.scl_pullup_en = GPIO_PULLUP_ENABLE; //Enable internal pullup on SCL pin
        i2cM.master.clk_speed = I2C_SPEED; //Clock speed on SCL
        //Don't set address mode or set slave address in master mode

    i2c_param_config(I2C_M_PORT,&i2cM); //Configure I2C Port 0 with the above config - Port 0 will be our master

    i2c_config_t i2cS; //Config structure for second I2C
        i2cS.mode = I2C_MODE_SLAVE; //Set to slave
        i2cS.sda_io_num = I2C_S_SDA; //SDA pin 
        i2cS.scl_io_num = I2C_S_SCL; //SCL pin 
        i2cS.sda_pullup_en = GPIO_PULLUP_ENABLE; //Enable internal pullup on SDA pin
        i2cS.scl_pullup_en = GPIO_PULLUP_ENABLE; //Enable internal pullup on SCL pin
        i2cS.slave.addr_10bit_en = 0; //Do we want 10 bit address mode?
        i2cS.slave.slave_addr = I2C_S_ADR; //Set our slave address
        //Don't set clock speed as slave

    i2c_param_config(I2C_S_PORT,&i2cS); //Configure I2C Port 1 with the above config - Port 1 will be our slave

    //Can also set a few other default values, most relevent probably lsb/msb first, and timeout
    //See: https://docs.espressif.com/projects/esp-idf/en/latest/esp32/api-reference/peripherals/i2c.html#i2c-api-customized-configuration

    i2c_driver_install(I2C_M_PORT,I2C_MODE_MASTER, I2C_MASTER_TX_BUF, I2C_MASTER_RX_BUF, 0); //Finally install driver for Port 0 - would also include flags for interrupt here (set to 0 for this example)
                    //Port, Mode, RX Buffer, TX Buffer, Flag - Buffer not needed (but good idea) for master mode
    i2c_driver_install(I2C_S_PORT,I2C_MODE_SLAVE, I2C_SLAVE_TX_BUF, I2C_SLAVE_RX_BUF, 0); //And driver for Port 1 - As its slave it needs (or should have) buffers

    vTaskDelete(NULL); // We don't want this to keep going, so delete the task once config is done
     
}


// Function for configuring UART controllers
void UART_Config(void *pvParameter)
{

gpio_set_pull_mode(GPIO_NUM_1,GPIO_PULLUP_PULLDOWN);

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

vTaskDelete(NULL); // We don't want this to keep going, so delete the task once config is done

}