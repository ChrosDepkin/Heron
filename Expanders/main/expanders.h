/*
    IO Expander Code for Heron
    Chris Deakin 30/4/21
    *************************

*/

#include "driver/i2c.h"


// Base class for IO Expanders
class MCP
{
    protected:
        uint8_t address;    // Device address
        uint8_t config;     // IOCON value
        uint8_t PAdir;      // Port A input/output directions
        uint8_t PBdir;      // Port B input/output directions
        uint8_t PApu;       // Port A pullups
        uint8_t PBpu;       // Port B pullups
        uint8_t PAval;      // Port A current value
        uint8_t PBval;      // Port B current value

    public:
        MCP(uint8_t addr, uint8_t conf, uint8_t PAd, uint8_t PBd, uint8_t PAp, uint8_t PBp); // Constructor - needs all variables except for Port Values
        void setup();                               // Will configure this IO expander with config, direction, and pullup variables
        void regWrite(uint8_t reg, uint8_t val);    // Writes a value to a register on the expander
        void regRead(uint8_t reg);                  // Reads a value from a register. If PA/PB will store in associated variable, else return is not actually stored anywhere

};

// Child class specifically for button matrix IO expander
class MCPB : public MCP // Inherits from MCP class
{
    protected:
    //bool matrixState[56]; // Has an array to store the current state of all keys in the matrix

    public:
    using MCP::MCP;       // Need this to inherit the constructor from parent class
    void matrixRead();    // Function to read all values on the button matrix
    bool matrixState[56]; // Has an array to store the current state of all keys in the matrix

};

// Child class for encoder button expander
class MCPE : public MCP // Inherits from MCP class
{
    protected:
    bool EV[16];    // Each encoder has two pins that we read, 8 encoders total
    bool EVP[16];   // We also need to store the previous values for reading encoders
    uint8_t Turn[8]; // Turn direction (if any) of encoder on last read

    public:
    using MCP::MCP;       // Need this to inherit the constructor from parent class
    void encoderRead();

};