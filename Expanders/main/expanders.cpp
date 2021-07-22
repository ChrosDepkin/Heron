/*
    Functions defined within expander class in Expanders.h
*/

#include "expanders.h" // IO Expander classes are defined here
#include "I2Cdefs.h" // Various useful defines here



MCP::MCP(uint8_t addr, uint8_t conf, uint8_t PAd, uint8_t PBd, uint8_t PAp, uint8_t PBp) // Constructor
{ address = addr; config = conf; PAdir = PAd; PBdir = PBd; PApu = PAp; PBpu = PBp; }

//Function to read from an MCP23017 IO Expander Register
void MCP::regRead(uint8_t reg)
{
    /*First send register address*/
    i2c_cmd_handle_t cmd = i2c_cmd_link_create(); //Create a command link for the following messages
    i2c_master_start(cmd); //Start 
    i2c_master_write_byte(cmd, (this->address << 1) | I2C_MASTER_WRITE, ACK_EN); //Send a read message to the device address, wait for ack
    i2c_master_write_byte(cmd, reg, ACK_EN); //Send the register address we want to read
	i2c_master_stop(cmd); //End
    esp_err_t ret = i2c_master_cmd_begin(I2C_M_PORT, cmd, 1000/portTICK_PERIOD_MS); //This function sends the message composed above
                                                                                    //Keep the result so we can do some error checking below
    i2c_cmd_link_delete(cmd); //No longer using command link

    //Print result type - just for debugging
    /*
    if( ret == ESP_OK ) 
    {
	   printf("\nRead1 OK");
    }
    if( ret == ESP_ERR_INVALID_ARG ) 
    {
	   printf("\nRead1 Inv. Arg");
    }
    if( ret == ESP_FAIL ) 
    {
	   printf("\nRead1 Fail");
    }
    if( ret == ESP_ERR_INVALID_STATE ) 
    {
	   printf("\nRead1 Inv. State");
    }
    if( ret == ESP_ERR_TIMEOUT ) 
    {
	   printf("\nRead1 TO");
    }
    */
    /*Then read the data from that register*/
    //All as above except for 'master_read_byte' function
    cmd = i2c_cmd_link_create();
    i2c_master_start(cmd);
    i2c_master_write_byte(cmd, (this->address << 1) | I2C_MASTER_READ, ACK_EN); 
    if(reg == MCP_GPIOA)
    {
    i2c_master_read_byte(cmd, &PAval, NACK_EN); //MCP should send us the value on 'reg', store in appropriate variable
    }
    else if(reg == MCP_GPIOB)
    {
    i2c_master_read_byte(cmd, &PBval, NACK_EN);
    }
    else // Incorrect input given
	i2c_master_stop(cmd);
    ret = i2c_master_cmd_begin(I2C_M_PORT, cmd, 1000/portTICK_PERIOD_MS); 
    i2c_cmd_link_delete(cmd);
    /*
    if( ret == ESP_OK ) 
    {
	   printf("\nRead2 OK");
    }
    if( ret == ESP_ERR_INVALID_ARG ) 
    {
	   printf("\nRead2 Inv. Arg");
    }
    if( ret == ESP_FAIL ) 
    {
	   printf("\nRead2 Fail");
    }
    if( ret == ESP_ERR_INVALID_STATE ) 
    {
	   printf("\nRead2 Inv. State");
    }
    if( ret == ESP_ERR_TIMEOUT ) 
    {
	   printf("\nRead2 TO");
    }
    */
}

//Function to write a value to an MCP23017 IO Expander Register
void MCP::regWrite(uint8_t reg, uint8_t val)
{
    i2c_cmd_handle_t cmd = i2c_cmd_link_create(); //Create a command link for the following messages
    i2c_master_start(cmd); //Start
    i2c_master_write_byte(cmd, (this->address << 1) | I2C_MASTER_WRITE, ACK_EN); //Send a write message to the device address, wait for ack
    i2c_master_write_byte(cmd, reg, ACK_EN); //Send the register address
    i2c_master_write_byte(cmd, val, ACK_EN); //Send the value to write to the register
    i2c_master_stop(cmd); //Stop
    esp_err_t ret = i2c_master_cmd_begin(I2C_M_PORT, cmd, 1000/portTICK_PERIOD_MS); //Send the message composed above
    i2c_cmd_link_delete(cmd); //Delete command link - not using it anymore

    /*
    if( ret == ESP_OK ) 
    {
	   printf("\nW OK");
    }
    if( ret == ESP_ERR_INVALID_ARG ) 
    {
	   printf("\nW Inv. Arg");
    }
    if( ret == ESP_FAIL ) 
    {
	   printf("\nW Fail");
    }
    if( ret == ESP_ERR_INVALID_STATE ) 
    {
	   printf("\nW Inv. State");
    }
    if( ret == ESP_ERR_TIMEOUT ) 
    {
	   printf("\nW TO");
    }
    */
}

// Function to configure an IO expander
void MCP::setup()
{
    regWrite(MCP_IOCONA, MCP_DEF_CONFIG);       // Write config value first
    regWrite(MCP_IODIRA, this->PAdir);          // Port A IO Direction
    regWrite(MCP_IODIRB, this->PBdir);          // Port B IO Direction
    regWrite(MCP_GPPUA, this->PApu);            // Port A Pullup Value
    regWrite(MCP_GPPUB, this->PBpu);            // Port B Pullup Value
}


// Function to read the button matrix on an IO expander
void MCPB::matrixRead()
{
    regWrite(MCP_GPIOB, ~COL0);                         // Set all columns but COL0 high
    regRead(MCP_GPIOA);                                 // Read the value on the rows (PORTA)
    this->matrixState[0] = (~this->PAval&(ROW0));       // If the value on ROW0 is low while COL0 is low, then that key is pressed
    this->matrixState[1] = (~this->PAval&(ROW1));
    this->matrixState[2] = (~this->PAval&(ROW2));       // We're inverting the read values because we're setting the columns low
    this->matrixState[3] = (~this->PAval&(ROW3));       // while rows are pulled high. So if a row is low, the column must be
    this->matrixState[4] = (~this->PAval&(ROW4));       // pulling it low.
    this->matrixState[5] = (~this->PAval&(ROW5));
    this->matrixState[6] = (~this->PAval&(ROW6));
    this->matrixState[7] = (~this->PAval&(ROW7));
    regWrite(MCP_GPIOB, ~COL1);                         // Repeat for each column
    regRead(MCP_GPIOA);
    this->matrixState[8] = (~this->PAval&(ROW0));
    this->matrixState[9] = (~this->PAval&(ROW1));
    this->matrixState[10] = (~this->PAval&(ROW2));
    this->matrixState[11] = (~this->PAval&(ROW3));
    this->matrixState[12] = (~this->PAval&(ROW4));
    this->matrixState[13] = (~this->PAval&(ROW5));
    this->matrixState[14] = (~this->PAval&(ROW6));
    this->matrixState[15] = (~this->PAval&(ROW7));
    regWrite(MCP_GPIOB, ~COL2);
    regRead(MCP_GPIOA);
    this->matrixState[16] = (~this->PAval&(ROW0));
    this->matrixState[17] = (~this->PAval&(ROW1));
    this->matrixState[18] = (~this->PAval&(ROW2));
    this->matrixState[19] = (~this->PAval&(ROW3));
    this->matrixState[20] = (~this->PAval&(ROW4));
    this->matrixState[21] = (~this->PAval&(ROW5));
    this->matrixState[22] = (~this->PAval&(ROW6));
    this->matrixState[23] = (~this->PAval&(ROW7));
    regWrite(MCP_GPIOB, ~COL3);
    regRead(MCP_GPIOA);
    this->matrixState[24] = (~this->PAval&(ROW0));
    this->matrixState[25] = (~this->PAval&(ROW1));
    this->matrixState[26] = (~this->PAval&(ROW2));
    this->matrixState[27] = (~this->PAval&(ROW3));
    this->matrixState[28] = (~this->PAval&(ROW4));
    this->matrixState[29] = (~this->PAval&(ROW5));
    this->matrixState[30] = (~this->PAval&(ROW6));
    this->matrixState[31] = (~this->PAval&(ROW7));
    regWrite(MCP_GPIOB, ~COL4);
    regRead(MCP_GPIOA);
    this->matrixState[32] = (~this->PAval&(ROW0));
    this->matrixState[33] = (~this->PAval&(ROW1));
    this->matrixState[34] = (~this->PAval&(ROW2));
    this->matrixState[35] = (~this->PAval&(ROW3));
    this->matrixState[36] = (~this->PAval&(ROW4));
    this->matrixState[37] = (~this->PAval&(ROW5));
    this->matrixState[38] = (~this->PAval&(ROW6));
    this->matrixState[39] = (~this->PAval&(ROW7));
    regWrite(MCP_GPIOB, ~COL5);
    regRead(MCP_GPIOA);
    this->matrixState[40] = (~this->PAval&(ROW0));
    this->matrixState[41] = (~this->PAval&(ROW1));
    this->matrixState[42] = (~this->PAval&(ROW2));
    this->matrixState[43] = (~this->PAval&(ROW3));
    this->matrixState[44] = (~this->PAval&(ROW4));
    this->matrixState[45] = (~this->PAval&(ROW5));
    this->matrixState[46] = (~this->PAval&(ROW6));
    this->matrixState[47] = (~this->PAval&(ROW7));
    regWrite(MCP_GPIOB, ~COL6);
    regRead(MCP_GPIOA);
    this->matrixState[48] = (~this->PAval&(ROW0));
    this->matrixState[49] = (~this->PAval&(ROW1));
    this->matrixState[50] = (~this->PAval&(ROW2));
    this->matrixState[51] = (~this->PAval&(ROW3));
    this->matrixState[52] = (~this->PAval&(ROW4));
    this->matrixState[53] = (~this->PAval&(ROW5));
    this->matrixState[54] = (~this->PAval&(ROW6));
    this->matrixState[55] = (~this->PAval&(ROW7));

}

// Function to read the current values on the encoder pins
void MCPE::encoderRead()
{
    // Read register values first
    regRead(MCP_GPIOA);
    regRead(MCP_GPIOB);
\
    // Update current values
    EV[0] = PAval & 0b00000001;
    EV[1] = PAval & 0b00000010;
    EV[2] = PAval & 0b00000100;
    EV[3] = PAval & 0b00001000;
    EV[4] = PAval & 0b00010000;
    EV[5] = PAval & 0b00100000;
    EV[6] = PAval & 0b01000000;
    EV[7] = PAval & 0b10000000;
    EV[8] = PBval & 0b10000000;
    EV[9] = PBval & 0b01000000;
    EV[10] = PBval & 0b00100000;
    EV[11] = PBval & 0b00010000;
    EV[12] = PBval & 0b00001000;
    EV[13] = PBval & 0b00000100;
    EV[14] = PBval & 0b00000010;
    EV[15] = PBval & 0b00000001;
    
    
    for (int i = 0; i < 16; i = i + 2)
    {
        Turn[i/2] = 0; // Reset turn value first
        if(EV[i] != EVP[i]) // If + pin has changed
        {
            if(EV[i] == EV[i+1]) // And both pins are high
            {
                Turn[i/2] = 1;  // Turn in one direction
            }
            else                // Else only one pin is high
            {
                Turn[i/2] = 2;  // Turn in other direction
            }
        }
        // Store previous values for next round
        EVP[i] = EV[i];
        EVP[i+1] = EV[i+1];
    }
    

}