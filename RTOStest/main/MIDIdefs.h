// Defines for MIDI operations

// UART Settings
#define UART_0_BAUD 115200 // Just chucking these in as defaults
#define UART_2_BAUD 115200 // Check/test baud later
#define UART_0_TX 1
#define UART_0_RX 3
#define UART_2_TX 17
#define UART_2_RX 16
#define UART_0_BUF_SIZE 1024 // Not sure what size for buffers
#define UART_2_BUF_SIZE 1024

// Status Byte Types - Top 4 bits represent message type, bottom four bytes represent the channel
#define NOTE_OFF 0x80
#define NOTE_ON 0x90
#define PLYPRESSURE 0xA0
#define CTRLCHANGE 0xB0
#define PRGCHANGE 0xC0
#define CHNLPRESSURE 0xD0
#define PITCHBEND 0xE0
#define CHNLMODE 0xB0
#define SYSEXCL 0xF0
