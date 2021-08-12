//Defines for all I2C/IO Expander operations

//I2C General
#define ACK 0 //Acknowledge
#define NACK 1 //No acknowledge
#define ACK_EN 1 //Acknowledge Enable
#define NACK_EN i2c_ack_type_t::I2C_MASTER_ACK //No acknowledge Enable
#define I2C_SPEED 400000

//I2C Master
#define I2C_M_PORT 0 //I2C Master Port Number
#define I2C_M_SDA 21 //I2C Master SDA Pin
#define I2C_M_SCL 22 //I2C Master SCL Pin
#define I2C_MASTER_TX_BUF 128 // Just setting buffers to 128 at random - not sure what to put
#define I2C_MASTER_RX_BUF 128
//I2C Slave
#define I2C_S_PORT 1 //I2C Slave Port Number
#define I2C_S_SDA 33 //I2C Slave SDA Pin
#define I2C_S_SCL 32 //I2C Slave SCL Pin
#define I2C_S_ADR 0x10 //I2C Slave address
#define I2C_SLAVE_TX_BUF 128 // Just setting buffers to 128 at random - not sure what to put
#define I2C_SLAVE_RX_BUF 128

//IO Expander - Base address of MCP23017 is 0x20 (with all address pins low)
#define EXP_ADR_E      0x20 //Encoder IO expander address
#define EXP_ADR_M      0x21 //Misc. IO expander address
#define EXP_ADR_B      0x22 //Button IO expander address

//Register Address on IO Expander
//ASSUMING THAT IOCON.BANK IS SET TO 0 - IF 1, THESE ADDRESSES ARE WRONG
#define MCP_IODIRA   0x00 //Direction PortA
#define MCP_IODIRB   0x01 //          PortB
#define MCP_IPOLA    0x02 //Input Polarity PortA
#define MCP_IPOLB    0x03 //               PortB
#define MCP_GPINTENA 0x04 //Interrupt-on-change PortA
#define MCP_GPINTENB 0x05 //                    PortB
#define MCP_DEFVALA  0x06 //Default Value PortA
#define MCP_DEFVALB  0x07 //              PortB
#define MCP_INTCONA  0x08 //Interrupt Control PortA
#define MCP_INTCONB  0x09 //                  PortB
#define MCP_IOCONA   0x0A //Configuration Register
#define MCP_IOCONB   0x0B //Unclear if you need to configure both?
#define MCP_GPPUA    0x0C //Pullup Control PortA
#define MCP_GPPUB    0x0D //               PortB
#define MCP_INTFA    0x0E //Interrupt Flags PortA
#define MCP_INTFB    0x0F //                PortB
#define MCP_INTCAPA  0x10 //Interrupt Capture PortA
#define MCP_INTCAPB  0x11 //                  PortB
#define MCP_GPIOA    0x12 //Port A - Read these
#define MCP_GPIOB    0x13 //Port B
#define MCP_OLATA    0x14 //Output Latches PortA
#define MCP_OLATB    0x15 //               PortB

//Expander Defaults
#define MCP_DEF_CONFIG 0b01100000 //Value to write to IOCON

#define DIR_PA_E 0xFF             // Port A direction MCP_E
#define DIR_PB_E 0xFF             // Port B direction MCP_E
#define DIR_PA_M 0
#define DIR_PB_M 0
#define DIR_PA_B 0xFF             // Rows input for button matrix
#define DIR_PB_B 0                // Cols output

#define PU_PA_E 0xFF              // Port A pullups MCP_E
#define PU_PB_E 0xFF              // Port B pullups MCP_E
#define PU_PA_M 0xFF
#define PU_PB_M 0xFF
#define PU_PA_B 0xFF
#define PU_PB_B 0

//Button Matrix Row/Column Values
#define ROW0 0b00000001
#define ROW1 0b00000010
#define ROW2 0b00000100
#define ROW3 0b00001000
#define ROW4 0b00010000
#define ROW5 0b00100000
#define ROW6 0b01000000
#define ROW7 0b10000000

#define COL0 0b00000001
#define COL1 0b00000010
#define COL2 0b00000100
#define COL3 0b00001000
#define COL4 0b00010000
#define COL5 0b00100000
#define COL6 0b01000000
