/* 
    This is meant to be the main project - combining all other elements worked on separately.

*/
extern "C" {                    // Need to put these includes in here to make it work
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/gpio.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Screen.h"
#include "nvs_flash.h"
#include "nvs.h"

void app_main(void);           // main needs to be in here too or it won't compile
}

#include "expanders.h"
#include "I2Cdefs.h"
#include "MIDIsys.h"
#include "axoVar.h"
#include "LED.h"
#include "BPM.h"
#include "Sequencer.h"
#include "Keyboard.h"



// Prototypes
void I2C_Config(void *pvParameter);
void Encoder_Task(void *pvParameter);
void Key_Task(void *pvParameter);
void MIDI_Task(void *pvParameter);
void UART_Config(void *pvParameter);
void varControl(void *pvParameter);
void LED(void *pvParameter);
void Timer(void *pvParameter);
static void BPMcallback(void* arg);
void Sequencer_Task(void *pvParameter);

// Queues for sending data between tasks
QueueHandle_t Q1; // Using this to send sequencer data from key task to variable control
QueueHandle_t Q2; // Encoder to AxoVar queue
QueueHandle_t Q3; // MIDI queue
QueueHandle_t Q4; // Track note assignment queue
QueueHandle_t Q5; // Save/Recall command queue

// Global Variables
uint8_t bank = 0; // Currently selected bank
uint8_t trackBank = 0;
uint8_t channel = 1; // Current MIDI channel (hardcoded to 1 for now)
uint8_t BPM = 60; // Current BPM
bool BPMflag = 0;
uint8_t mode = 0; // Used for tracking interface modes (Currently default, bank, or BPM)
TaskHandle_t LEDs = NULL; // LED task needs to be global as it suspends itself and other tasks resume it.
                          // There's probably a better solution but at least this works
TaskHandle_t MIDI = NULL;

// Things that don't go anywhere yet
MCP MCP_M(EXP_ADR_M, MCP_DEF_CONFIG, DIR_PA_M, DIR_PB_M, PU_PA_M, PU_PB_M);     // Misc. IO expander


SemaphoreHandle_t xGuiSemaphore;


struct CRGB leds[NUM_LEDS];
struct CRGB bankLED[NUM_LEDS]; // Shows selected bank
struct CRGB trackBankLED[NUM_LEDS];
struct CRGB seq[4][NUM_LEDS]; // Sequence LEDs


void app_main(void)
{
    // Make a handle for each OS task
    TaskHandle_t I2Cconfig = NULL;
    TaskHandle_t UARTconfig = NULL;
    TaskHandle_t Enc = NULL;
    TaskHandle_t Key = NULL;
    TaskHandle_t varCtrl = NULL;
    TaskHandle_t Timers = NULL;


    Q1 = xQueueCreate(8, sizeof(uint16_t)); // Create a queue 8 items long (chose arbitrarily) each item 16 bits long
    Q2 = xQueueCreate(8, sizeof(uint16_t));
    Q3 = xQueueCreate(8, sizeof(uint32_t));
    Q4 = xQueueCreate(8, sizeof(uint16_t));
    Q5 = xQueueCreate(8, sizeof(uint8_t));

    

    // Create each task for the OS
    xTaskCreate(I2C_Config, "I2Cconfig", 2048, NULL, 8, &I2Cconfig);
    xTaskCreate(UART_Config, "UARTconfig", 2048, NULL, 8, &UARTconfig);
    xTaskCreate(Encoder_Task, "EncTask", 2048, NULL, 4, &Enc);
    xTaskCreate(Key_Task, "KeyTask", 2048, NULL, 4, &Key);
    xTaskCreate(varControl, "VarControl", 4096, NULL, 5, &varCtrl);
    xTaskCreate(LED, "LEDtask", 2048, NULL, 6, &LEDs);
    xTaskCreate(Timer, "TimerTask", 2048, NULL, 3, &Timers); // Timer itself is called as an interrupt, so timer task doesn't need a high priority
    xTaskCreate(MIDI_Task, "LEDtask", 2048, NULL, 6, &MIDI);
    xTaskCreatePinnedToCore(guiTask, "gui", 4096*2, NULL, 4, NULL, 1);
    // Pass in Function - Name (just for debug)- Stack size - Parameters - Priority - Handle
    // Haven't done anything with stack depth or parameters for now
    // This function gives the task permission to run on both cores - can specify cores with xTaskCreatePinnedToCore()

    
}

// Callback - this is an interrupt that gets called whenever the BPM timer ticks
static void BPMcallback(void* arg)
{
    BPMclass* BPMC = (BPMclass*)arg; // This callback is given the address of the BPM object.
                                     // Just need to put it into a usable form
    // Debug stuff below:
    //const char* TAG = "Callback";
    //int64_t time_since_boot = esp_timer_get_time();
    //ESP_LOGI(TAG, "Periodic timer called, time since boot: %lld us", time_since_boot);

    if(BPMC->BPMp != BPM) // If the BPM has been changed
        {
            BPMC->BPMchanged = 1; // Reset the flag
        }
    BPMflag = !BPMflag;
    
}

void Timer(void *pvParameter)
{
    BPMclass BPMc;
    
    BPMc.BPMargs.callback = &BPMcallback; // Function called each time timer fires
            BPMc.BPMargs.arg = &BPMc;
            /* name is optional, but may help identify the timer when debugging */
            BPMc.BPMargs.name = "BPM";

    esp_timer_create(&BPMc.BPMargs, &BPMc.BPMtimer); // Create the timer with the given handle and arguments
    esp_timer_start_periodic(BPMc.BPMtimer, (60000000/BPM)); // 60000000 over BPM gives us BPM in us
    

    for(;;)
    {
    /*********Task Loop***********/
        if(BPMc.BPMchanged) // If BPM has changed
        {
            esp_timer_stop(BPMc.BPMtimer); // Need to stop timer before we can change period
            esp_timer_start_periodic(BPMc.BPMtimer, (60000000/BPM)); // Set new period based on new BPM
            BPMc.BPMchanged = 0;
        }
    taskYIELD();
    /*********Task Loop***********/
    }

}

void MIDI_Task(void *pvParameter)
{
 uint32_t Q3buff = 0; // Buffer for queue 3
 uint8_t status =0;
 uint8_t d1 =0;
 uint8_t d2 =0;

 for (;;)
 {
     xQueueReceive(Q3,(void *) &Q3buff,10);
     
     status = Q3buff & 0xFF;
     d1 = (Q3buff & 0xFF00) >> 8;
     d2 = (Q3buff & 0xFF0000) >> 16;
     MIDI_send(status, channel, d1, d2);
     vTaskSuspend(NULL);
 }
 

}




void LED(void *pvParameter)
{
    CLEDController* controller = &FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(50);
    bankLED[0] = CRGB::Orange;
    trackBankLED[0] = CRGB::Purple;


    for(;;)
    {
    /*********Task Loop***********/
    switch (mode)
    {
    case 0:
        /* code */
        break;
    case 1:
        controller->show(bankLED, NUM_LEDS, FastLED.getBrightness());
        break;
    case 2: // BPM Mode
            controller->show(trackBankLED, NUM_LEDS, FastLED.getBrightness());
        break;
    case 3: // Sequencer Mode
            controller->show(seq[bank], NUM_LEDS, FastLED.getBrightness());
        break;
    case 4: // Sequencer Keyboard Mode
            controller->show(seq[bank], NUM_LEDS, FastLED.getBrightness());
        break;
    
    default:
        break;
    }
    
    vTaskSuspend(NULL);
        

    /*********Task Loop***********/
    }

}

// Function for encoder task
void Encoder_Task(void *pvParameter)
{
    MCPE MCP_E(EXP_ADR_E, MCP_DEF_CONFIG, DIR_PA_E, DIR_PB_E, PU_PA_E, PU_PB_E); // Create encoder expander object

    MCP_E.setup(); // Setup function for that expander

    uint16_t Q2buff; // Buffer for queue 2

    uint8_t BPMp = BPM; // Local BPM variable


    

    for(;;) // Run this task continuously
    {
    /*********Task Loop***********/
        MCP_E.encoderRead(); // Read encoder values
        taskYIELD();

        if(mode != 2) // Mode 2 is BPM mode
        {
            for (int i = 0; i < 8; i++)
            {
                if(MCP_E.Turn[i] == 1)
                    {
                        Q2buff = INCCOM | (i << 9) | 1; // Increment command, encoder 'i', value 1 (inc)
                        xQueueSend(Q2,&Q2buff,10);
                    }
                else if(MCP_E.Turn[i] == 2)
                    {
                        Q2buff = INCCOM | (i << 9) | 2; // Increment command, encoder 'i', value 2 (dec)
                        xQueueSend(Q2,&Q2buff,10);
                    }
            }
        }

        else if (mode == 2)
        {
            if(MCP_E.Turn[0] == 1)
                {
                    if(BPMp < 255)BPMp++;
                }
            else if(MCP_E.Turn[0] == 2)
                {
                    if(BPMp > 1){BPMp--;}
                }
            else   
                BPM = BPMp;
        }
    
    taskYIELD();
    /*********Task Loop***********/
    }
}


// Function for button matrix task
void Key_Task(void *pvParameter)
{
    MCPB MCP_B(EXP_ADR_B, MCP_DEF_CONFIG, DIR_PA_B, DIR_PB_B, PU_PA_B, PU_PB_B); // Create matrix expander object

    MCP_B.setup(); // Setup function for matrix expander

    uint16_t Q1buff; // Buffer for queue 1
    uint16_t Q3buff; // Buffer for queue 1
    uint16_t Q4buff; // Buffer for queue 4
    uint8_t Q5buff; // Buffer for queue 5

    uint8_t octave = 0; // Current octave
    uint8_t check = 0;

    TickType_t xDelay = 200 / portTICK_PERIOD_MS; // 200ms delay (will dial this in)



    

    while(1)
    {
    /*********Task Loop***********/
        MCP_B.matrixRead(); // Read the button matrix

        if(MCP_B.matrixState[BUT21] == 1){if(octave > 0){octave--;}}
        if(MCP_B.matrixState[BUT16] == 1){if(octave < 7){octave++;}}

        if(MCP_B.matrixState[BUT9] == 1)
        {
            Q5buff = 1;
            xQueueSend(Q5,&Q5buff,10);
        }
        else if(MCP_B.matrixState[BUT10] == 1)
        {
            Q5buff = 2;
            xQueueSend(Q5,&Q5buff,10);
        }


        if(MCP_B.matrixState[BUT19] == 1){mode = 4;} // Track keyboard mode
        else if(MCP_B.matrixState[ BUT4] == 1){mode = 3;} // Sequencer Mode
        else if(MCP_B.matrixState[ BUT3] == 1){mode = 2;} // BPM mode
        else if(MCP_B.matrixState[BUT15] == 1){mode = 1;} // Bank Mode
        else if(MCP_B.matrixState[BUT16] == 1){mode = 0;} // Default Mode (nothing atm)

        if(mode == 0)
        {
            check = keyboard_check(MCP_B.matrixState, octave);
            if(check == 1){}
            else
            {
                Q3buff = NOTE_ON | (check << 8) | (64 << 16);
                xQueueSend(Q3,&Q3buff,10);
                check = 1;
                vTaskResume(MIDI);
            }
        }


        else if(mode == 1)
        {
            if(MCP_B.matrixState[KEY1] == 1)
                {
                    bank = 0;
                    bankLED[0] = CRGB::Orange;
                    bankLED[1] = CRGB::Black;
                    bankLED[2] = CRGB::Black;
                    bankLED[3] = CRGB::Black;
                    vTaskResume(LEDs);
                } 
            else if(MCP_B.matrixState[KEY2] == 1)
                {
                    bank = 1;
                    bankLED[0] = CRGB::Black;
                    bankLED[1] = CRGB::Orange;
                    bankLED[2] = CRGB::Black;
                    bankLED[3] = CRGB::Black;
                    vTaskResume(LEDs);
                } 
            else if(MCP_B.matrixState[KEY3] == 1)
                {
                    bank = 2;
                    bankLED[0] = CRGB::Black;
                    bankLED[1] = CRGB::Black;
                    bankLED[2] = CRGB::Orange;
                    bankLED[3] = CRGB::Black;
                    vTaskResume(LEDs);
                }  
            else if(MCP_B.matrixState[KEY4] == 1)
                {
                    bank = 3;
                    bankLED[0] = CRGB::Black;
                    bankLED[1] = CRGB::Black;
                    bankLED[2] = CRGB::Black;
                    bankLED[3] = CRGB::Orange;
                    vTaskResume(LEDs);
                } 
        }
        
        else if(mode == 2)
        {
            if(MCP_B.matrixState[KEY1] == 1)
            {
                trackBank = 0;
                trackBankLED[0] = CRGB::Purple;
                trackBankLED[1] = CRGB::Black;
                trackBankLED[2] = CRGB::Black;
                trackBankLED[3] = CRGB::Black;
                vTaskResume(LEDs);
            }
            else if(MCP_B.matrixState[KEY2] == 1)
            {
                trackBank = 1;
                trackBankLED[0] = CRGB::Black;
                trackBankLED[1] = CRGB::Purple;
                trackBankLED[2] = CRGB::Black;
                trackBankLED[3] = CRGB::Black;
                vTaskResume(LEDs);
            }
            else if(MCP_B.matrixState[KEY3] == 1)
            {
                trackBank = 2;
                trackBankLED[0] = CRGB::Black;
                trackBankLED[1] = CRGB::Black;
                trackBankLED[2] = CRGB::Purple;
                trackBankLED[3] = CRGB::Black;
                vTaskResume(LEDs);
            }
            else if(MCP_B.matrixState[KEY4] == 1)
            {
                trackBank = 3;
                trackBankLED[0] = CRGB::Black;
                trackBankLED[1] = CRGB::Black;
                trackBankLED[2] = CRGB::Black;
                trackBankLED[3] = CRGB::Purple;
                vTaskResume(LEDs);
            }
            
        }

        else if(mode == 3)
        {
            Q1buff = 0;
            int j = 0;
            for (int i = 0; i < 49; i = i+8)
            {
                Q1buff = Q1buff | (MCP_B.matrixState[i] << j);
                j++;
            }
            for (int i = 1; i < 50; i = i+8)
            {
                Q1buff = Q1buff | (MCP_B.matrixState[i] << j);
                j++;
            }
            for (int i = 2; i < 11; i = i+8)
            {
                Q1buff = Q1buff | (MCP_B.matrixState[i] << j);
                j++;
            }
            j = 0;

            xQueueSend(Q1,&Q1buff,10); // Send which keys were pressed
            
        }
        else if(mode == 4)
        {
            check = keyboard_check(MCP_B.matrixState, octave);
            if(check == 1){}
            else
            {
                Q4buff = (bank << 8);
                Q4buff = Q4buff | check;
                xQueueSend(Q4,&Q4buff,10);
                check = 1;
            }

        }

    vTaskDelay(xDelay);
    /*********Task Loop***********/
    }
}


void varControl(void *pvParameter)
{
    bool BPMflagLocal = 0;
    uint16_t Q1buff = 0;
    uint16_t Q2buff = 0;
    uint32_t Q3buff = 0;
    uint32_t Q4buff = 0;
    uint8_t Q5buff = 0;

    axoVar banks[4][8]; // 2D array - 4 banks of 8 encoders
    track tracks[4][2]; // 4 banks of 2 tracks

    tracks[0][1].type = NOTE_ON;
    tracks[0][1].d2 = 64; // Velocity 64
    tracks[0][1].bank = 0;
    tracks[1][1].type = NOTE_ON;
    tracks[1][1].d2 = 64; // Velocity 64
    tracks[1][1].bank = 1;
    tracks[2][1].type = NOTE_ON;
    tracks[2][1].d2 = 64; // Velocity 64
    tracks[2][1].bank = 2;
    tracks[3][1].type = NOTE_ON;
    tracks[3][1].d2 = 64; // Velocity 64
    tracks[3][1].bank = 3;

    
    nvs_flash_init();
    nvs_handle storage_handle;


    // Variables for deconstructing queue messages
    uint16_t enc = 0;
    uint16_t com = 0;
    uint16_t vlu = 0;


    // Encoder received message structure:
    // x/xxx/xxx/xx/xxxxxxx
    // Check/Unused/Enc/Com/Value
    // Commands - 00, write raw value
    //          - 01, inc/decrement value (0/1 -/+)
    //          - 10, change CC value
    //          - 11, unused
    // Top bit is set on any message (as an all 0 message would otherwise be valid)
    for(;;)
    {
    /*********Task Loop***********/
        xQueueReceive(Q1,(void *) &Q1buff,10); // Get the data from queue
        xQueueReceive(Q2,(void *) &Q2buff,10);
        xQueueReceive(Q4,(void *) &Q4buff,10);
        xQueueReceive(Q5,(void *) &Q5buff,10);

        enc = (Q2buff & ENCMASK) >> 9; 
        com = (Q2buff & COMMASK) >> 7;
        vlu = Q2buff & VALMASK;

        if(Q1buff) // If any steps were pressed
        {
           tracks[bank][1].steps = tracks[bank][1].steps ^ Q1buff;
           for (int i = 0; i < tracks[bank][1].length; i++)
           {
               if(tracks[bank][1].steps&(1 << i)){seq[bank][i] = CRGB::Red;}
               else if(tracks[bank][1].position != i){seq[bank][i] = CRGB::Black;}
               vTaskResume(LEDs);
           }
           taskYIELD();
        }

        if(Q4buff)
        {
            tracks[(Q4buff >> 8)][1].d1 = (Q4buff & 0xFF);
            taskYIELD();
        }

        if(BPMflagLocal != BPMflag)
        {
            for (int i = 0; i < 4; i++)
            {
                tracks[i][1].stepInc(Q3buff);
            }
            
            BPMflagLocal = BPMflag;
            if((mode == 2) | (mode == 3) | (mode == 4))
            {vTaskResume(LEDs);}
            taskYIELD();
        }

        if(Q2buff & CHKMASK) // Check we have a message
        {
            if(com == 0b00)
            {
                banks[bank][enc].val = vlu;

                Q3buff = 0;
                Q3buff = CTRLCHANGE | (banks[bank][enc].CC << 8) | (banks[bank][enc].val << 16);
                xQueueSend(Q3,&Q3buff,10);
                vTaskResume(MIDI);

                //MIDI_send(CTRLCHANGE, channel, banks[bank][enc].CC, banks[bank][enc].val);

            }
            else if(com == 0b01)
            {
                if(vlu == 1){banks[bank][enc].incVal(1);}
                else if(vlu == 2){banks[bank][enc].incVal(0);}
                
                Q3buff = 0;
                Q3buff = CTRLCHANGE | (banks[bank][enc].CC << 8) | (banks[bank][enc].val << 16);
                xQueueSend(Q3,&Q3buff,10);
                vTaskResume(MIDI);
            }
            else if(com == 0b10)
            {
                banks[bank][enc].CC = vlu;
            }
            else if(com == 0b11)
            {}

            Q2buff = 0; // Clear the buffer or it'll keep repeating the same command
            taskYIELD();              
        }
        if(Q5buff)
        {
            if(Q5buff == 1)
            {
                nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);

                nvs_set_u8(storage_handle, "B0E0", banks[0][0].CC);
                nvs_set_u8(storage_handle, "B0E1", banks[0][1].CC);
                nvs_set_u8(storage_handle, "B0E2", banks[0][2].CC);
                nvs_set_u8(storage_handle, "B0E3", banks[0][3].CC);
                nvs_set_u8(storage_handle, "B0E4", banks[0][4].CC);
                nvs_set_u8(storage_handle, "B0E5", banks[0][5].CC);
                nvs_set_u8(storage_handle, "B0E6", banks[0][6].CC);
                nvs_set_u8(storage_handle, "B0E7", banks[0][7].CC);

                nvs_set_u8(storage_handle, "B1E0", banks[1][0].CC);
                nvs_set_u8(storage_handle, "B1E1", banks[1][1].CC);
                nvs_set_u8(storage_handle, "B1E2", banks[1][2].CC);
                nvs_set_u8(storage_handle, "B1E3", banks[1][3].CC);
                nvs_set_u8(storage_handle, "B1E4", banks[1][4].CC);
                nvs_set_u8(storage_handle, "B1E5", banks[1][5].CC);
                nvs_set_u8(storage_handle, "B1E6", banks[1][6].CC);
                nvs_set_u8(storage_handle, "B1E7", banks[1][7].CC);

                nvs_set_u8(storage_handle, "B2E0", banks[2][0].CC);
                nvs_set_u8(storage_handle, "B2E1", banks[2][1].CC);
                nvs_set_u8(storage_handle, "B2E2", banks[2][2].CC);
                nvs_set_u8(storage_handle, "B2E3", banks[2][3].CC);
                nvs_set_u8(storage_handle, "B2E4", banks[2][4].CC);
                nvs_set_u8(storage_handle, "B2E5", banks[2][5].CC);
                nvs_set_u8(storage_handle, "B2E6", banks[2][6].CC);
                nvs_set_u8(storage_handle, "B2E7", banks[2][7].CC);

                nvs_set_u8(storage_handle, "B3E0", banks[3][0].CC);
                nvs_set_u8(storage_handle, "B3E1", banks[3][1].CC);
                nvs_set_u8(storage_handle, "B3E2", banks[3][2].CC);
                nvs_set_u8(storage_handle, "B3E3", banks[3][3].CC);
                nvs_set_u8(storage_handle, "B3E4", banks[3][4].CC);
                nvs_set_u8(storage_handle, "B3E5", banks[3][5].CC);
                nvs_set_u8(storage_handle, "B3E6", banks[3][6].CC);
                nvs_set_u8(storage_handle, "B3E7", banks[3][7].CC);

                nvs_commit(storage_handle);
                nvs_close(storage_handle);
                
            }
            else if(Q5buff == 2)
            {
                nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);

                nvs_get_u8(storage_handle, "B0E0", &banks[0][0].CC);
                nvs_get_u8(storage_handle, "B0E1", &banks[0][1].CC);
                nvs_get_u8(storage_handle, "B0E2", &banks[0][2].CC);
                nvs_get_u8(storage_handle, "B0E3", &banks[0][3].CC);
                nvs_get_u8(storage_handle, "B0E4", &banks[0][4].CC);
                nvs_get_u8(storage_handle, "B0E5", &banks[0][5].CC);
                nvs_get_u8(storage_handle, "B0E6", &banks[0][6].CC);
                nvs_get_u8(storage_handle, "B0E7", &banks[0][7].CC);

                nvs_get_u8(storage_handle, "B1E0", &banks[1][0].CC);
                nvs_get_u8(storage_handle, "B1E1", &banks[1][1].CC);
                nvs_get_u8(storage_handle, "B1E2", &banks[1][2].CC);
                nvs_get_u8(storage_handle, "B1E3", &banks[1][3].CC);
                nvs_get_u8(storage_handle, "B1E4", &banks[1][4].CC);
                nvs_get_u8(storage_handle, "B1E5", &banks[1][5].CC);
                nvs_get_u8(storage_handle, "B1E6", &banks[1][6].CC);
                nvs_get_u8(storage_handle, "B1E7", &banks[1][7].CC);

                nvs_get_u8(storage_handle, "B2E0", &banks[2][0].CC);
                nvs_get_u8(storage_handle, "B2E1", &banks[2][1].CC);
                nvs_get_u8(storage_handle, "B2E2", &banks[2][2].CC);
                nvs_get_u8(storage_handle, "B2E3", &banks[2][3].CC);
                nvs_get_u8(storage_handle, "B2E4", &banks[2][4].CC);
                nvs_get_u8(storage_handle, "B2E5", &banks[2][5].CC);
                nvs_get_u8(storage_handle, "B2E6", &banks[2][6].CC);
                nvs_get_u8(storage_handle, "B2E7", &banks[2][7].CC);

                nvs_get_u8(storage_handle, "B3E0", &banks[3][0].CC);
                nvs_get_u8(storage_handle, "B3E1", &banks[3][1].CC);
                nvs_get_u8(storage_handle, "B3E2", &banks[3][2].CC);
                nvs_get_u8(storage_handle, "B3E3", &banks[3][3].CC);
                nvs_get_u8(storage_handle, "B3E4", &banks[3][4].CC);
                nvs_get_u8(storage_handle, "B3E5", &banks[3][5].CC);
                nvs_get_u8(storage_handle, "B3E6", &banks[3][6].CC);
                nvs_get_u8(storage_handle, "B3E7", &banks[3][7].CC);

                nvs_close(storage_handle);
            }

            Q5buff = 0;
        }

    /*********Task Loop***********/
    }

}





/* 
    Config Functions
    Run once at startup, might shift them into another file.

*/


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
