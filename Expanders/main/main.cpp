/*
This file uses the expanders.h/.cpp code to test the IO expanders.
A config function for I2C is called first to set everything up, then
IO expander objects are created, and their setup functions setup the
actual IO expanders. A while loop continually checks a couple of buttons
for testing.
Button matrix is working, need to work on the encoders.
*/
extern "C" {                    // Need to put these includes in here to make it work
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_spi_flash.h"

void app_main(void);           // main needs to be in here too or it won't compile
}

#include "expanders.h"         // Deals with IO Expanders
#include "I2Cdefs.h"           // Just defines for various I2C stuff
#include "esp_log.h"

void I2C_Config();

void app_main()
{
    I2C_Config();   // Sets up I2C Master and Slave (only master used here)
    MCPE MCP_E(EXP_ADR_E, MCP_DEF_CONFIG, DIR_PA_E, DIR_PB_E, PU_PA_E, PU_PB_E);     // Create expander objects
    MCP MCP_M(EXP_ADR_M, MCP_DEF_CONFIG, DIR_PA_M, DIR_PB_M, PU_PA_M, PU_PB_M);     // Misc. expander is from base class
    MCPB MCP_B(EXP_ADR_B, MCP_DEF_CONFIG, DIR_PA_B, DIR_PB_B, PU_PA_B, PU_PB_B);
    MCP_E.setup();  // Setup function for each expander
    MCP_M.setup();
    MCP_B.setup();

    // Debug tag for each encoder (just a bit cleaner for the log)
    static const char* TAG1 = "EN1";
    static const char* TAG2 = "EN2";
    static const char* TAG3 = "EN3";
    static const char* TAG4 = "EN4";
    static const char* TAG5 = "EN5";
    static const char* TAG6 = "EN6";
    static const char* TAG7 = "EN7";
    static const char* TAG8 = "EN8";
    
    while(1) // while loop for testing
    {
    MCP_E.encoderRead(); // Read the encoders
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
    }

}


// Not sure exactly where I2C config function should go - not that far into organising the project yet
void I2C_Config()
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
     
}