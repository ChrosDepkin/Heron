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
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_wifi.h"

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

#define ADCsamples 16


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
void ADC_Task(void *pvParameter);

// Queues for sending data between tasks
QueueHandle_t Q1; // Using this to send sequencer data from key task to variable control
QueueHandle_t Q2; // Encoder to AxoVar queue
QueueHandle_t Q3; // MIDI queue
QueueHandle_t Q4; // Track note assignment queue
QueueHandle_t Q5; // Encoders to Screen queue
QueueHandle_t Q6; // Screen save queue
QueueHandle_t Q7; // ADC queue

// Global Variables
uint8_t bank = 0; // Currently selected bank
uint8_t track = 0;
uint8_t channel = 0; // Current MIDI channel (hardcoded to 0/1 for now)
uint8_t BPM = 60; // Current BPM
bool BPMflag = 0;
uint8_t mode = 0; // Used for tracking interface modes
TaskHandle_t LEDs = NULL; // LED task needs to be global as it suspends itself and other tasks resume it.
TaskHandle_t MIDI = NULL;
uint8_t trackLen = 15;
uint8_t octave = 0; // Current octave
uint8_t arcValues[8] = {0};
uint8_t arcCCVal[8] = {0};
uint8_t sliderCC[2] = {0};
uint8_t sliderVal[2] = {0};
extern uint8_t arcBool[8];
uint8_t keyboardVel = 64;
extern uint8_t sliderBool[2];

// Things that don't go anywhere yet
MCP MCP_M(EXP_ADR_M, MCP_DEF_CONFIG, DIR_PA_M, DIR_PB_M, PU_PA_M, PU_PB_M);     // Misc. IO expander



SemaphoreHandle_t xGuiSemaphore;



struct CRGB leds[NUM_LEDS];
struct CRGB utilityLED[NUM_LEDS]; // Shows selected bank
struct CRGB seq[4][NUM_LEDS]; // Sequence LEDs
struct CRGB seqLen[NUM_LEDS]; // Sequence Length LEDs


void app_main(void)
{
    // Make a handle for each OS task
    TaskHandle_t I2Cconfig = NULL;
    TaskHandle_t UARTconfig = NULL;
    TaskHandle_t Enc = NULL;
    TaskHandle_t Key = NULL;
    TaskHandle_t varCtrl = NULL;
    TaskHandle_t Timers = NULL;
    TaskHandle_t ADC = NULL;


    Q1 = xQueueCreate(8, sizeof(uint16_t)); // Create a queue 8 items long (chose arbitrarily) each item 16 bits long
    Q2 = xQueueCreate(8, sizeof(uint16_t));
    Q3 = xQueueCreate(64, sizeof(uint32_t));
    Q4 = xQueueCreate(8, sizeof(uint16_t));
    Q5 = xQueueCreate(8, sizeof(uint64_t));
    Q6 = xQueueCreate(8, sizeof(uint8_t));
    Q7 = xQueueCreate(8, sizeof(uint32_t));
    
    esp_wifi_stop();
    esp_wifi_deinit();

    // Create each task for the OS
    xTaskCreate(I2C_Config, "I2Cconfig", 2048, NULL, 8, &I2Cconfig);
    xTaskCreate(UART_Config, "UARTconfig", 2048, NULL, 8, &UARTconfig);
    xTaskCreate(Encoder_Task, "EncTask", 2048, NULL, 4, &Enc);
    xTaskCreate(Key_Task, "KeyTask", 2048, NULL, 4, &Key);
    xTaskCreate(varControl, "VarControl", 4096, NULL, 5, &varCtrl);
    xTaskCreate(Timer, "TimerTask", 2048, NULL, 2, &Timers); // Timer itself is called as an interrupt, so timer task doesn't need a high priority
    xTaskCreate(MIDI_Task, "MIDItask", 2048, NULL, 5, &MIDI);
    xTaskCreatePinnedToCore(ADC_Task, "ADCtask", 2048, NULL, 2, &ADC, 1);
    xTaskCreatePinnedToCore(guiTask, "gui", 4096*2, NULL, 6, NULL, 0);
    xTaskCreatePinnedToCore(LED, "LEDtask", 2048, NULL, 7, &LEDs, 1);
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

void ADC_Task(void *pvParameter)
{
    uint32_t Q7buff = 0;
    TickType_t xDelay = 75 / portTICK_PERIOD_MS;
    
    int joyX = 0;
    int joyY = 0;
    int sliderL = 0;
    int sliderLt = 0;
    int sliderR = 0;
    int sliderRt = 0;
    adc1_config_width(ADC_WIDTH_BIT_9);
    adc1_config_channel_atten(ADC1_CHANNEL_0, ADC_ATTEN_DB_11); // Joy Y
    adc1_config_channel_atten(ADC1_CHANNEL_3, ADC_ATTEN_DB_11); // Joy X
    adc2_config_channel_atten(ADC2_CHANNEL_7, ADC_ATTEN_DB_11); // Slider L
    adc2_config_channel_atten(ADC2_CHANNEL_9, ADC_ATTEN_DB_11); // Slider R

    for(;;)
    {
        joyX = 0;
        joyY = 0;
        sliderL = 0;
        sliderLt = 0;
        sliderR = 0;
        sliderRt = 0;
        for (int i = 0; i < ADCsamples; i++)
        {
            joyY += adc1_get_raw(ADC1_CHANNEL_0);
            joyX += adc1_get_raw(ADC1_CHANNEL_3);
            adc2_get_raw(ADC2_CHANNEL_7, ADC_WIDTH_BIT_9, &sliderLt);
            sliderL += sliderLt;
            adc2_get_raw(ADC2_CHANNEL_9, ADC_WIDTH_BIT_9, &sliderRt);
            sliderR += sliderRt;
        }
        joyX /= ADCsamples;
        joyY /= ADCsamples;
        sliderL /= ADCsamples;
        sliderR /= ADCsamples;
        
        joyY /= 4;
        joyX = ((~joyX)&0x1FF)/4;
        sliderL = ((~sliderL)&0x1FF)/4;
        sliderR = ((~sliderR)&0x1FF)/4;
        
        if(sliderBool[0] == 0){Q7buff = sliderL | (sliderR << 8) | (joyX << 16) | (joyY << 24) | (1 << 31);}
        else if(sliderBool[0] == 1){Q7buff = sliderL | (sliderR << 8) | (joyX << 16) | (joyY << 24) | (1 << 30);}
        xQueueSend(Q7,&Q7buff,10);
        vTaskDelay(xDelay);
    }

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
 uint8_t status = 0;
 uint8_t d1 = 0;
 uint8_t d2 = 0;

 for (;;)
 {
     while(uxQueueMessagesWaiting(Q3) != 0)
     {     
        xQueueReceive(Q3,(void *) &Q3buff,10);
        
        status = Q3buff & 0xFF;
        d1 = (Q3buff & 0xFF00) >> 8;
        d2 = (Q3buff & 0xFF0000) >> 16;
        MIDI_send(status, channel, d1, d2);
     }
     vTaskSuspend(NULL);
     
 }
 

}




void LED(void *pvParameter)
{
    CLEDController* controller = &FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS);
    FastLED.setBrightness(50);
    utilityLED[0] = CRGB::Orange;
    utilityLED[5] = CRGB::Purple;
    utilityLED[4] = CRGB::DimGray;

    for (int i = 0; i < 15; i++)
    {
        seqLen[i] = CRGB::DarkKhaki;
    }
    




    for(;;)
    {
    /*********Task Loop***********/

    switch (mode)
    {
    case 0:
        controller->show(leds, NUM_LEDS, FastLED.getBrightness());
        break;
    case 1: // Utility (BPM, Bank, Track select) Mode
        controller->show(utilityLED, NUM_LEDS, FastLED.getBrightness());
        break;
    case 2: // Sequencer Mode
        controller->show(seq[track], NUM_LEDS, FastLED.getBrightness());
        break;
    case 3: // Sequencer Keyboard Mode
        controller->show(seq[track], NUM_LEDS, FastLED.getBrightness());
        break;
    case 4: // Track Length Mode
        controller->show(seqLen, NUM_LEDS, FastLED.getBrightness());
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
    uint8_t velP = keyboardVel;


    

    for(;;) // Run this task continuously
    {
    /*********Task Loop***********/
        MCP_E.encoderRead(); // Read encoder values

        if((mode == 0) | (mode == 2) | (mode == 4))
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

        else if (mode == 1)
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

            if(MCP_E.Turn[2] == 1)
                {
                    if(velP < 127)velP++;
                }
            else if(MCP_E.Turn[2] == 2)
                {
                    if(velP > 1){velP--;}
                }
            else   
                keyboardVel = velP;
        }

        else if(mode == 3)
        {
            if(MCP_E.Turn[0] == 1)
                {
                    Q2buff = VELCOM | (1 << 9) | 1; // Increment command, encoder 'i', value 1 (inc)
                    xQueueSend(Q2,&Q2buff,10);
                }
            else if(MCP_E.Turn[0] == 2)
                {
                    Q2buff = VELCOM | (1 << 9) | 2; // Increment command, encoder 'i', value 2 (dec)
                    xQueueSend(Q2,&Q2buff,10);
                }
            
        }
    
    /*********Task Loop***********/
    }
}


// Function for button matrix task
void Key_Task(void *pvParameter)
{
    MCPB MCP_B(EXP_ADR_B, MCP_DEF_CONFIG, DIR_PA_B, DIR_PB_B, PU_PA_B, PU_PB_B); // Create matrix expander object

    MCP_B.setup(); // Setup function for matrix expander

    uint16_t Q1buff; // Buffer for queue 1
    uint32_t Q3buff; // Buffer for queue 3
    uint16_t Q4buff; // Buffer for queue 4
 



    Notes notes[96];

    uint8_t modeChange = 0;
    uint8_t check = 1;

    TickType_t xDelay = 125 / portTICK_PERIOD_MS; // 100ms delay (will dial this in)

    // Set up the notes with MIDI note values starting at C1
    int count = 0;
    for (int i = 24; i < 128; i++)
    {
        notes[count].note = i;
        count++;
    }
    /*Q3buff = NOTE_ON | (notes[0].note << 8) | (64 << 16);
    xQueueSend(Q3,&Q3buff,10); // And add it to the queue
    vTaskResume(MIDI);*/



    

    while(1)
    {
    /*********Task Loop***********/
        MCP_B.matrixRead(); // Read the button matrix

        if(MCP_B.matrixState[BUT21] == 1){if(octave > 0){octave--;}}
        if(MCP_B.matrixState[BUT6] == 1){if(octave < 6){octave++;}}


        if(MCP_B.matrixState[BUT19] == 1){mode = 4;} // Track length mode
        else if(MCP_B.matrixState[ BUT4] == 1){mode = 3;} // Track keyboard mode
        else if(MCP_B.matrixState[ BUT3] == 1){mode = 2;} // Sequencer Mode
        else if(MCP_B.matrixState[BUT15] == 1){mode = 1;} // Utility mode
        else if(MCP_B.matrixState[BUT16] == 1){mode = 0;} // Keyboard mode

        if(mode != modeChange) // Check if mode has changed and update LEDs
        {
            vTaskResume(LEDs);
            modeChange = mode;
        }
 
        if(mode == 0)
        {
            
            octaveClear(octave, notes); // Process all notes out of octave
            keyboard_check(MCP_B.matrixState, octave, notes); // Check all notes in octave
            for(int i = 0; i < 96; i ++) // Step through each note
            {
                if(notes[i].state == 1) // State 1 means send note on
                {
                    Q3buff = NOTE_ON | (notes[i].note << 8) | (keyboardVel << 16); // Load up buffer
                    xQueueSend(Q3,&Q3buff,10); // And add it to the queue
                    vTaskResume(MIDI);
                }
                else if(notes[i].state == 3) // State 3 means send note off
                {
                    Q3buff = NOTE_OFF | (notes[i].note << 8) | (keyboardVel << 16);
                    xQueueSend(Q3,&Q3buff,10);
                    vTaskResume(MIDI);
                }
                // States 0 & 2 will hold a note (either on or off) so no message needed
            }
            
        }


        else if(mode == 1)
        {
            // Bank Select
            if(MCP_B.matrixState[KEY1] == 1)
                {
                    bank = 0;
                    utilityLED[0] = CRGB::Orange;
                    utilityLED[1] = CRGB::Black;
                    utilityLED[2] = CRGB::Black;
                    utilityLED[3] = CRGB::Black;
                    vTaskResume(LEDs);
                } 
            else if(MCP_B.matrixState[KEY2] == 1)
                {
                    bank = 1;
                    utilityLED[0] = CRGB::Black;
                    utilityLED[1] = CRGB::Orange;
                    utilityLED[2] = CRGB::Black;
                    utilityLED[3] = CRGB::Black;
                    vTaskResume(LEDs);
                } 
            else if(MCP_B.matrixState[KEY3] == 1)
                {
                    bank = 2;
                    utilityLED[0] = CRGB::Black;
                    utilityLED[1] = CRGB::Black;
                    utilityLED[2] = CRGB::Orange;
                    utilityLED[3] = CRGB::Black;
                    vTaskResume(LEDs);
                }  
            else if(MCP_B.matrixState[KEY4] == 1)
                {
                    bank = 3;
                    utilityLED[0] = CRGB::Black;
                    utilityLED[1] = CRGB::Black;
                    utilityLED[2] = CRGB::Black;
                    utilityLED[3] = CRGB::Orange;
                    vTaskResume(LEDs);
                }


            // Track Select
            if(MCP_B.matrixState[KEY6] == 1)
                {
                    track = 0;
                    utilityLED[5] = CRGB::Purple;
                    utilityLED[6] = CRGB::Black;
                    utilityLED[7] = CRGB::Black;
                    utilityLED[8] = CRGB::Black;
                    vTaskResume(LEDs);
                } 
            else if(MCP_B.matrixState[KEY7] == 1)
                {
                    track = 1;
                    utilityLED[5] = CRGB::Black;
                    utilityLED[6] = CRGB::Purple;
                    utilityLED[7] = CRGB::Black;
                    utilityLED[8] = CRGB::Black;
                    vTaskResume(LEDs);
                } 
            else if(MCP_B.matrixState[KEY8] == 1)
                {
                    track = 2;
                    utilityLED[5] = CRGB::Black;
                    utilityLED[6] = CRGB::Black;
                    utilityLED[7] = CRGB::Purple;
                    utilityLED[8] = CRGB::Black;
                    vTaskResume(LEDs);
                }
            else if(MCP_B.matrixState[KEY9] == 1)
                {
                    track = 3;
                    utilityLED[5] = CRGB::Black;
                    utilityLED[6] = CRGB::Black;
                    utilityLED[7] = CRGB::Black;
                    utilityLED[8] = CRGB::Purple;
                    vTaskResume(LEDs);
                }
        }

        else if(mode == 2)
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
        else if(mode == 3)
        {
            
            check = keyInterp(MCP_B.matrixState, octave);
            if(check == 1){}
            else
            {
                Q4buff = (track << 8);
                Q4buff = Q4buff | check;
                xQueueSend(Q4,&Q4buff,10);
                check = 1;
            }
            

        }

        else if(mode == 4)
        {
            if(MCP_B.matrixState[KEY1] == 1)
            {
                trackLen = 0;
                for (int i = 0; i < 15; i++)
                {
                    if(i == 0){seqLen[i] = CRGB::DarkKhaki;}
                    else{seqLen[i] = CRGB::Black;}
                }
                vTaskResume(LEDs);
            }
            else if(MCP_B.matrixState[KEY2] == 1)
            {
                trackLen = 1;
                for (int i = 0; i < 15; i++)
                {
                    if(i <= trackLen){seqLen[i] = CRGB::DarkKhaki;}
                    else{seqLen[i] = CRGB::Black;}
                }
                vTaskResume(LEDs);         
            }
            else if(MCP_B.matrixState[KEY3] == 1)
            {
                trackLen = 2;
                for (int i = 0; i < 15; i++)
                {
                    if(i <= trackLen){seqLen[i] = CRGB::DarkKhaki;}
                    else{seqLen[i] = CRGB::Black;}
                } 
                vTaskResume(LEDs);        
            }
            else if(MCP_B.matrixState[KEY4] == 1)
            {
                trackLen = 3;
                for (int i = 0; i < 15; i++)
                {
                    if(i <= trackLen){seqLen[i] = CRGB::DarkKhaki;}
                    else{seqLen[i] = CRGB::Black;}
                } 
                vTaskResume(LEDs);        
            }
            else if(MCP_B.matrixState[KEY5] == 1)
            {
                trackLen = 4;
                for (int i = 0; i < 15; i++)
                {
                    if(i <= trackLen){seqLen[i] = CRGB::DarkKhaki;}
                    else{seqLen[i] = CRGB::Black;}
                }
                vTaskResume(LEDs);         
            }
            else if(MCP_B.matrixState[KEY6] == 1)
            {
                trackLen = 5;
                for (int i = 0; i < 15; i++)
                {
                    if(i <= trackLen){seqLen[i] = CRGB::DarkKhaki;}
                    else{seqLen[i] = CRGB::Black;}
                } 
                vTaskResume(LEDs);        
            }
            else if(MCP_B.matrixState[KEY7] == 1)
            {
                trackLen = 6;
                for (int i = 0; i < 15; i++)
                {
                    if(i <= trackLen){seqLen[i] = CRGB::DarkKhaki;}
                    else{seqLen[i] = CRGB::Black;}
                } 
                vTaskResume(LEDs);        
            }
            else if(MCP_B.matrixState[KEY8] == 1)
            {
                trackLen = 7;
                for (int i = 0; i < 15; i++)
                {
                    if(i <= trackLen){seqLen[i] = CRGB::DarkKhaki;}
                    else{seqLen[i] = CRGB::Black;}
                }  
                vTaskResume(LEDs);      
            }
            else if(MCP_B.matrixState[KEY9] == 1)
            {
                trackLen = 8;
                for (int i = 0; i < 15; i++)
                {
                    if(i <= trackLen){seqLen[i] = CRGB::DarkKhaki;}
                    else{seqLen[i] = CRGB::Black;}
                } 
                vTaskResume(LEDs);        
            }
            else if(MCP_B.matrixState[KEY10] == 1)
            {
                trackLen = 9;
                for (int i = 0; i < 15; i++)
                {
                    if(i <= trackLen){seqLen[i] = CRGB::DarkKhaki;}
                    else{seqLen[i] = CRGB::Black;}
                }   
                vTaskResume(LEDs);      
            }
            else if(MCP_B.matrixState[KEY11] == 1)
            {
                trackLen = 10;
                for (int i = 0; i < 15; i++)
                {
                    if(i <= trackLen){seqLen[i] = CRGB::DarkKhaki;}
                    else{seqLen[i] = CRGB::Black;}
                }  
                vTaskResume(LEDs);       
            }
            else if(MCP_B.matrixState[KEY12] == 1)
            {
                trackLen = 11;
                for (int i = 0; i < 15; i++)
                {
                    if(i <= trackLen){seqLen[i] = CRGB::DarkKhaki;}
                    else{seqLen[i] = CRGB::Black;}
                }   
                vTaskResume(LEDs);      
            }
            else if(MCP_B.matrixState[KEY13] == 1)
            {
                trackLen = 12;
                for (int i = 0; i < 15; i++)
                {
                    if(i <= trackLen){seqLen[i] = CRGB::DarkKhaki;}
                    else{seqLen[i] = CRGB::Black;}
                }    
                vTaskResume(LEDs);     
            }
            else if(MCP_B.matrixState[KEY14] == 1)
            {
                trackLen = 13;
                for (int i = 0; i < 15; i++)
                {
                    if(i <= trackLen){seqLen[i] = CRGB::DarkKhaki;}
                    else{seqLen[i] = CRGB::Black;}
                }   
                vTaskResume(LEDs);      
            }
            else if(MCP_B.matrixState[KEY15] == 1)
            {
                trackLen = 14;
                for (int i = 0; i < 15; i++)
                {
                    if(i <= trackLen){seqLen[i] = CRGB::DarkKhaki;}
                    else{seqLen[i] = CRGB::Black;}
                } 
                vTaskResume(LEDs);        
            }
            else if(MCP_B.matrixState[KEY16] == 1)
            {
                trackLen = 15;
                for (int i = 0; i < 15; i++)
                {
                    if(i <= trackLen){seqLen[i] = CRGB::DarkKhaki;}
                    else{seqLen[i] = CRGB::Black;}
                }    
                vTaskResume(LEDs);     
            }

        }

    vTaskDelay(xDelay);
    /*********Task Loop***********/
    }
}


void varControl(void *pvParameter)
{
    bool BPMflagLocal = 0;
    bool sendCheck = 0;
    uint16_t Q1buff = 0;
    uint16_t Q2buff = 0;
    uint32_t Q3buff = 0;
    uint32_t Q4buff = 0;
    uint64_t Q5buff = 0;
    uint8_t Q6buff = 0;
    uint32_t Q7buff = 0;

    axoVar banks[4][12]; // 2D array - 4 banks of 8 encoders, 2 sliders, 2 joystick axes
    TrackClass tracks[4][2]; // 4 banks of 2 tracks


    tracks[0][1].type = NOTE_ON;
    tracks[0][1].d2 = 64; // Velocity 64
    tracks[0][1].tB = 0;
    tracks[1][1].type = NOTE_ON;
    tracks[1][1].d2 = 64; // Velocity 64
    tracks[1][1].tB = 1;
    tracks[2][1].type = NOTE_ON;
    tracks[2][1].d2 = 64; // Velocity 64
    tracks[2][1].tB = 2;
    tracks[3][1].type = NOTE_ON;
    tracks[3][1].d2 = 64; // Velocity 64
    tracks[3][1].tB = 3;

    banks[0][8].CC = 124;
    banks[0][9].CC = 125;
    banks[0][10].CC = 126;
    banks[0][11].CC = 127;
    banks[1][8].CC = 124;
    banks[1][9].CC = 125;
    banks[1][10].CC = 126;
    banks[1][11].CC = 127;
    banks[2][8].CC = 124;
    banks[2][9].CC = 125;
    banks[2][10].CC = 126;
    banks[2][11].CC = 127;
    banks[3][8].CC = 124;
    banks[3][9].CC = 125;
    banks[3][10].CC = 126;
    banks[3][11].CC = 127;

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
    //          - 11, inc/decrement velocity (0/1 -/+)
    // Top bit is set on any message (as an all 0 message would otherwise be valid)
    for(;;)
    {
    /*********Task Loop***********/


        arcValues[0] = banks[bank][7].val;
        arcValues[1] = banks[bank][5].val;
        arcValues[2] = banks[bank][3].val;
        arcValues[3] = banks[bank][1].val;
        arcValues[4] = banks[bank][6].val;
        arcValues[5] = banks[bank][4].val;
        arcValues[6] = banks[bank][2].val;
        arcValues[7] = banks[bank][0].val;

        sliderVal[0] = banks[bank][8].val;
        sliderVal[1] = banks[bank][9].val;

        arcCCVal[0] = banks[bank][7].CC;
        arcCCVal[1] = banks[bank][5].CC;
        arcCCVal[2] = banks[bank][3].CC;
        arcCCVal[3] = banks[bank][1].CC;
        arcCCVal[4] = banks[bank][6].CC;
        arcCCVal[5] = banks[bank][4].CC;
        arcCCVal[6] = banks[bank][2].CC;
        arcCCVal[7] = banks[bank][0].CC;

        sliderCC[0] = banks[bank][8].CC;
        sliderCC[1] = banks[bank][9].CC;
        

        xQueueReceive(Q1,(void *) &Q1buff,10); // Get the data from queue
        xQueueReceive(Q2,(void *) &Q2buff,10);
        xQueueReceive(Q4,(void *) &Q4buff,10);
        xQueueReceive(Q6,(void *) &Q6buff,10);
        xQueueReceive(Q7,(void *) &Q7buff,10);

        enc = (Q2buff & ENCMASK) >> 9; 
        com = (Q2buff & COMMASK) >> 7;
        vlu = Q2buff & VALMASK;

        if(Q1buff) // If any steps were pressed
        {
           tracks[track][1].steps = tracks[track][1].steps ^ Q1buff;
           for (int i = 0; i < 15; i++)
           {
               if(tracks[track][1].steps&(1 << i)){seq[track][i] = CRGB::Red;}
               else if(tracks[track][1].position != i){seq[track][i] = CRGB::Black;}
               vTaskResume(LEDs);
           }
        }

        if(Q4buff)
        {
            tracks[(Q4buff >> 8)][1].d1 = (Q4buff & 0xFF);
        }

        if(BPMflagLocal != BPMflag)
        {
            for (int i = 0; i < 4; i++)
            {
                tracks[i][1].stepInc(Q3buff);
            }
            
            BPMflagLocal = BPMflag;
            if(mode == 2)
            {vTaskResume(LEDs);}
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

            }
            else if(com == 0b01)
            {
                if(vlu == 1)
                {
                    if(enc == 0)
                    {
                        if(arcBool[7] == 0){banks[bank][enc].incVal(1); sendCheck = 1;}
                        else{banks[bank][enc].incCC(1);}
                    }
                    if(enc == 1)
                    {
                        if(arcBool[3] == 0){banks[bank][enc].incVal(1); sendCheck = 1;}
                        else{banks[bank][enc].incCC(1);}
                    }
                    if(enc == 2)
                    {
                        if(arcBool[6] == 0){banks[bank][enc].incVal(1); sendCheck = 1;}
                        else{banks[bank][enc].incCC(1);}
                    }
                    if(enc == 3)
                    {
                        if(arcBool[2] == 0){banks[bank][enc].incVal(1); sendCheck = 1;}
                        else{banks[bank][enc].incCC(1);}
                    }
                    if(enc == 4)
                    {
                        if(arcBool[5] == 0){banks[bank][enc].incVal(1); sendCheck = 1;}
                        else{banks[bank][enc].incCC(1);}
                    }
                    if(enc == 5)
                    {
                        if(arcBool[1] == 0){banks[bank][enc].incVal(1); sendCheck = 1;}
                        else{banks[bank][enc].incCC(1);}
                    }
                    if(enc == 6)
                    {
                        if(arcBool[4] == 0){banks[bank][enc].incVal(1); sendCheck = 1;}
                        else{banks[bank][enc].incCC(1);}
                    }
                    if(enc == 7)
                    {
                        if(arcBool[0] == 0){banks[bank][enc].incVal(1); sendCheck = 1;}
                        else{banks[bank][enc].incCC(1);}
                    }
                    
                }
                else if(vlu == 2)
                {
                    if(enc == 0)
                    {
                        if(arcBool[7] == 0){banks[bank][enc].incVal(0); sendCheck = 1;}
                        else{banks[bank][enc].incCC(0);}
                    }
                    if(enc == 1)
                    {
                        if(arcBool[3] == 0){banks[bank][enc].incVal(0); sendCheck = 1;}
                        else{banks[bank][enc].incCC(0);}
                    }
                    if(enc == 2)
                    {
                        if(arcBool[6] == 0){banks[bank][enc].incVal(0); sendCheck = 1;}
                        else{banks[bank][enc].incCC(0);}
                    }
                    if(enc == 3)
                    {
                        if(arcBool[2] == 0){banks[bank][enc].incVal(0); sendCheck = 1;}
                        else{banks[bank][enc].incCC(0);}
                    }
                    if(enc == 4)
                    {
                        if(arcBool[5] == 0){banks[bank][enc].incVal(0); sendCheck = 1;}
                        else{banks[bank][enc].incCC(0);}
                    }
                    if(enc == 5)
                    {
                        if(arcBool[1] == 0){banks[bank][enc].incVal(0); sendCheck = 1;}
                        else{banks[bank][enc].incCC(0);}
                    }
                    if(enc == 6)
                    {
                        if(arcBool[4] == 0){banks[bank][enc].incVal(0); sendCheck = 1;}
                        else{banks[bank][enc].incCC(0);}
                    }
                    if(enc == 7)
                    {
                        if(arcBool[0] == 0){banks[bank][enc].incVal(0); sendCheck = 1;}
                        else{banks[bank][enc].incCC(0);}
                    }
                }
                
                if(sendCheck == 1)
                {
                    sendCheck = 0;
                    Q3buff = 0;
                    Q3buff = CTRLCHANGE | (banks[bank][enc].CC << 8) | (banks[bank][enc].val << 16);
                    xQueueSend(Q3,&Q3buff,10);
                    vTaskResume(MIDI);
                }
            }
            else if(com == 0b10)
            {
                banks[bank][enc].CC = vlu;
            }
            else if(com == 0b11)
            {
                if(vlu == 1){tracks[track][1].velInc(1);}
                if(vlu == 2){tracks[track][1].velInc(0);}
            }

            Q2buff = 0; // Clear the buffer or it'll keep repeating the same command           
        }
        if(Q7buff != 0x0F000000)
            {
                if(Q7buff & 0x80000000)
                {
                    if(banks[bank][8].val != (Q7buff & 0xFF))
                    {
                        banks[bank][8].val = Q7buff & 0xFF; 
                        Q3buff = 0;
                        Q3buff = CTRLCHANGE | (banks[bank][8].CC << 8) | (banks[bank][8].val << 16);
                        xQueueSend(Q3,&Q3buff,10);
                        vTaskResume(MIDI);
                    }
                    if(banks[bank][9].val != ((Q7buff >> 8) & 0xFF))
                    {
                        banks[bank][9].val = (Q7buff >> 8) & 0xFF;
                        Q3buff = 0;
                        Q3buff = CTRLCHANGE | (banks[bank][9].CC << 8) | (banks[bank][9].val << 16);
                        xQueueSend(Q3,&Q3buff,10);
                        vTaskResume(MIDI);
                    }
                    if(banks[bank][10].val != ((Q7buff >> 16) & 0xFF))
                    {
                        banks[bank][10].val = (Q7buff >> 16) & 0xFF;
                        Q3buff = 0;
                        Q3buff = CTRLCHANGE | (banks[bank][10].CC << 8) | (banks[bank][10].val << 16);
                        xQueueSend(Q3,&Q3buff,10);
                        vTaskResume(MIDI);
                    }
                    if(banks[bank][11].val != ((Q7buff >> 24) & 0xFF))
                    {
                        banks[bank][11].val = (Q7buff >> 24) & 0xFF;
                        Q3buff = 0;
                        Q3buff = CTRLCHANGE | (banks[bank][11].CC << 8) | (banks[bank][11].val << 16);
                        xQueueSend(Q3,&Q3buff,10);
                        vTaskResume(MIDI);
                    }
                }

                if(Q7buff & 0x40000000)
                {
                    if(banks[bank][8].CC != (Q7buff & 0xFF))
                    {
                        banks[bank][8].CC = Q7buff & 0xFF; 
                    }
                    if(banks[bank][9].CC != ((Q7buff >> 8) & 0xFF))
                    {
                        banks[bank][9].CC = (Q7buff >> 8) & 0xFF;
                    }
                }
                Q7buff = 0x0F000000;
            }
        


        if(Q6buff)//Save/Load/Clear command
        {
            if((Q6buff & 0b11000000) == 0b01000000) // Save
            {
                if((Q6buff & 0b00001111) == 1)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);

                    nvs_set_u8(storage_handle, "P1B0E0", banks[0][0].CC);
                    nvs_set_u8(storage_handle, "P1B0E1", banks[0][1].CC);
                    nvs_set_u8(storage_handle, "P1B0E2", banks[0][2].CC);
                    nvs_set_u8(storage_handle, "P1B0E3", banks[0][3].CC);
                    nvs_set_u8(storage_handle, "P1B0E4", banks[0][4].CC);
                    nvs_set_u8(storage_handle, "P1B0E5", banks[0][5].CC);
                    nvs_set_u8(storage_handle, "P1B0E6", banks[0][6].CC);
                    nvs_set_u8(storage_handle, "P1B0E7", banks[0][7].CC);
      
                    nvs_set_u8(storage_handle, "P1B1E0", banks[1][0].CC);
                    nvs_set_u8(storage_handle, "P1B1E1", banks[1][1].CC);
                    nvs_set_u8(storage_handle, "P1B1E2", banks[1][2].CC);
                    nvs_set_u8(storage_handle, "P1B1E3", banks[1][3].CC);
                    nvs_set_u8(storage_handle, "P1B1E4", banks[1][4].CC);
                    nvs_set_u8(storage_handle, "P1B1E5", banks[1][5].CC);
                    nvs_set_u8(storage_handle, "P1B1E6", banks[1][6].CC);
                    nvs_set_u8(storage_handle, "P1B1E7", banks[1][7].CC);
      
                    nvs_set_u8(storage_handle, "P1B2E0", banks[2][0].CC);
                    nvs_set_u8(storage_handle, "P1B2E1", banks[2][1].CC);
                    nvs_set_u8(storage_handle, "P1B2E2", banks[2][2].CC);
                    nvs_set_u8(storage_handle, "P1B2E3", banks[2][3].CC);
                    nvs_set_u8(storage_handle, "P1B2E4", banks[2][4].CC);
                    nvs_set_u8(storage_handle, "P1B2E5", banks[2][5].CC);
                    nvs_set_u8(storage_handle, "P1B2E6", banks[2][6].CC);
                    nvs_set_u8(storage_handle, "P1B2E7", banks[2][7].CC);
      
                    nvs_set_u8(storage_handle, "P1B3E0", banks[3][0].CC);
                    nvs_set_u8(storage_handle, "P1B3E1", banks[3][1].CC);
                    nvs_set_u8(storage_handle, "P1B3E2", banks[3][2].CC);
                    nvs_set_u8(storage_handle, "P1B3E3", banks[3][3].CC);
                    nvs_set_u8(storage_handle, "P1B3E4", banks[3][4].CC);
                    nvs_set_u8(storage_handle, "P1B3E5", banks[3][5].CC);
                    nvs_set_u8(storage_handle, "P1B3E6", banks[3][6].CC);
                    nvs_set_u8(storage_handle, "P1B3E7", banks[3][7].CC);

                    nvs_commit(storage_handle);
                    nvs_close(storage_handle);
                }

                if((Q6buff & 0b00001111) == 2)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);

                    nvs_set_u8(storage_handle, "P2B0E0", banks[0][0].CC);
                    nvs_set_u8(storage_handle, "P2B0E1", banks[0][1].CC);
                    nvs_set_u8(storage_handle, "P2B0E2", banks[0][2].CC);
                    nvs_set_u8(storage_handle, "P2B0E3", banks[0][3].CC);
                    nvs_set_u8(storage_handle, "P2B0E4", banks[0][4].CC);
                    nvs_set_u8(storage_handle, "P2B0E5", banks[0][5].CC);
                    nvs_set_u8(storage_handle, "P2B0E6", banks[0][6].CC);
                    nvs_set_u8(storage_handle, "P2B0E7", banks[0][7].CC);
      
                    nvs_set_u8(storage_handle, "P2B1E0", banks[1][0].CC);
                    nvs_set_u8(storage_handle, "P2B1E1", banks[1][1].CC);
                    nvs_set_u8(storage_handle, "P2B1E2", banks[1][2].CC);
                    nvs_set_u8(storage_handle, "P2B1E3", banks[1][3].CC);
                    nvs_set_u8(storage_handle, "P2B1E4", banks[1][4].CC);
                    nvs_set_u8(storage_handle, "P2B1E5", banks[1][5].CC);
                    nvs_set_u8(storage_handle, "P2B1E6", banks[1][6].CC);
                    nvs_set_u8(storage_handle, "P2B1E7", banks[1][7].CC);
      
                    nvs_set_u8(storage_handle, "P2B2E0", banks[2][0].CC);
                    nvs_set_u8(storage_handle, "P2B2E1", banks[2][1].CC);
                    nvs_set_u8(storage_handle, "P2B2E2", banks[2][2].CC);
                    nvs_set_u8(storage_handle, "P2B2E3", banks[2][3].CC);
                    nvs_set_u8(storage_handle, "P2B2E4", banks[2][4].CC);
                    nvs_set_u8(storage_handle, "P2B2E5", banks[2][5].CC);
                    nvs_set_u8(storage_handle, "P2B2E6", banks[2][6].CC);
                    nvs_set_u8(storage_handle, "P2B2E7", banks[2][7].CC);
      
                    nvs_set_u8(storage_handle, "P2B3E0", banks[3][0].CC);
                    nvs_set_u8(storage_handle, "P2B3E1", banks[3][1].CC);
                    nvs_set_u8(storage_handle, "P2B3E2", banks[3][2].CC);
                    nvs_set_u8(storage_handle, "P2B3E3", banks[3][3].CC);
                    nvs_set_u8(storage_handle, "P2B3E4", banks[3][4].CC);
                    nvs_set_u8(storage_handle, "P2B3E5", banks[3][5].CC);
                    nvs_set_u8(storage_handle, "P2B3E6", banks[3][6].CC);
                    nvs_set_u8(storage_handle, "P2B3E7", banks[3][7].CC);

                    nvs_commit(storage_handle);
                    nvs_close(storage_handle);
                }

                if((Q6buff & 0b00001111) == 3)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);

                    nvs_set_u8(storage_handle, "P3B0E0", banks[0][0].CC);
                    nvs_set_u8(storage_handle, "P3B0E1", banks[0][1].CC);
                    nvs_set_u8(storage_handle, "P3B0E2", banks[0][2].CC);
                    nvs_set_u8(storage_handle, "P3B0E3", banks[0][3].CC);
                    nvs_set_u8(storage_handle, "P3B0E4", banks[0][4].CC);
                    nvs_set_u8(storage_handle, "P3B0E5", banks[0][5].CC);
                    nvs_set_u8(storage_handle, "P3B0E6", banks[0][6].CC);
                    nvs_set_u8(storage_handle, "P3B0E7", banks[0][7].CC);
      
                    nvs_set_u8(storage_handle, "P3B1E0", banks[1][0].CC);
                    nvs_set_u8(storage_handle, "P3B1E1", banks[1][1].CC);
                    nvs_set_u8(storage_handle, "P3B1E2", banks[1][2].CC);
                    nvs_set_u8(storage_handle, "P3B1E3", banks[1][3].CC);
                    nvs_set_u8(storage_handle, "P3B1E4", banks[1][4].CC);
                    nvs_set_u8(storage_handle, "P3B1E5", banks[1][5].CC);
                    nvs_set_u8(storage_handle, "P3B1E6", banks[1][6].CC);
                    nvs_set_u8(storage_handle, "P3B1E7", banks[1][7].CC);
      
                    nvs_set_u8(storage_handle, "P3B2E0", banks[2][0].CC);
                    nvs_set_u8(storage_handle, "P3B2E1", banks[2][1].CC);
                    nvs_set_u8(storage_handle, "P3B2E2", banks[2][2].CC);
                    nvs_set_u8(storage_handle, "P3B2E3", banks[2][3].CC);
                    nvs_set_u8(storage_handle, "P3B2E4", banks[2][4].CC);
                    nvs_set_u8(storage_handle, "P3B2E5", banks[2][5].CC);
                    nvs_set_u8(storage_handle, "P3B2E6", banks[2][6].CC);
                    nvs_set_u8(storage_handle, "P3B2E7", banks[2][7].CC);
      
                    nvs_set_u8(storage_handle, "P3B3E0", banks[3][0].CC);
                    nvs_set_u8(storage_handle, "P3B3E1", banks[3][1].CC);
                    nvs_set_u8(storage_handle, "P3B3E2", banks[3][2].CC);
                    nvs_set_u8(storage_handle, "P3B3E3", banks[3][3].CC);
                    nvs_set_u8(storage_handle, "P3B3E4", banks[3][4].CC);
                    nvs_set_u8(storage_handle, "P3B3E5", banks[3][5].CC);
                    nvs_set_u8(storage_handle, "P3B3E6", banks[3][6].CC);
                    nvs_set_u8(storage_handle, "P3B3E7", banks[3][7].CC);

                    nvs_commit(storage_handle);
                    nvs_close(storage_handle);
                }

                if((Q6buff & 0b00001111) == 4)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);

                    nvs_set_u8(storage_handle, "P4B0E0", banks[0][0].CC);
                    nvs_set_u8(storage_handle, "P4B0E1", banks[0][1].CC);
                    nvs_set_u8(storage_handle, "P4B0E2", banks[0][2].CC);
                    nvs_set_u8(storage_handle, "P4B0E3", banks[0][3].CC);
                    nvs_set_u8(storage_handle, "P4B0E4", banks[0][4].CC);
                    nvs_set_u8(storage_handle, "P4B0E5", banks[0][5].CC);
                    nvs_set_u8(storage_handle, "P4B0E6", banks[0][6].CC);
                    nvs_set_u8(storage_handle, "P4B0E7", banks[0][7].CC);
      
                    nvs_set_u8(storage_handle, "P4B1E0", banks[1][0].CC);
                    nvs_set_u8(storage_handle, "P4B1E1", banks[1][1].CC);
                    nvs_set_u8(storage_handle, "P4B1E2", banks[1][2].CC);
                    nvs_set_u8(storage_handle, "P4B1E3", banks[1][3].CC);
                    nvs_set_u8(storage_handle, "P4B1E4", banks[1][4].CC);
                    nvs_set_u8(storage_handle, "P4B1E5", banks[1][5].CC);
                    nvs_set_u8(storage_handle, "P4B1E6", banks[1][6].CC);
                    nvs_set_u8(storage_handle, "P4B1E7", banks[1][7].CC);
      
                    nvs_set_u8(storage_handle, "P4B2E0", banks[2][0].CC);
                    nvs_set_u8(storage_handle, "P4B2E1", banks[2][1].CC);
                    nvs_set_u8(storage_handle, "P4B2E2", banks[2][2].CC);
                    nvs_set_u8(storage_handle, "P4B2E3", banks[2][3].CC);
                    nvs_set_u8(storage_handle, "P4B2E4", banks[2][4].CC);
                    nvs_set_u8(storage_handle, "P4B2E5", banks[2][5].CC);
                    nvs_set_u8(storage_handle, "P4B2E6", banks[2][6].CC);
                    nvs_set_u8(storage_handle, "P4B2E7", banks[2][7].CC);
      
                    nvs_set_u8(storage_handle, "P4B3E0", banks[3][0].CC);
                    nvs_set_u8(storage_handle, "P4B3E1", banks[3][1].CC);
                    nvs_set_u8(storage_handle, "P4B3E2", banks[3][2].CC);
                    nvs_set_u8(storage_handle, "P4B3E3", banks[3][3].CC);
                    nvs_set_u8(storage_handle, "P4B3E4", banks[3][4].CC);
                    nvs_set_u8(storage_handle, "P4B3E5", banks[3][5].CC);
                    nvs_set_u8(storage_handle, "P4B3E6", banks[3][6].CC);
                    nvs_set_u8(storage_handle, "P4B3E7", banks[3][7].CC);

                    nvs_commit(storage_handle);
                    nvs_close(storage_handle);
                }

                if((Q6buff & 0b00001111) == 5)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);

                    nvs_set_u8(storage_handle, "P5B0E0", banks[0][0].CC);
                    nvs_set_u8(storage_handle, "P5B0E1", banks[0][1].CC);
                    nvs_set_u8(storage_handle, "P5B0E2", banks[0][2].CC);
                    nvs_set_u8(storage_handle, "P5B0E3", banks[0][3].CC);
                    nvs_set_u8(storage_handle, "P5B0E4", banks[0][4].CC);
                    nvs_set_u8(storage_handle, "P5B0E5", banks[0][5].CC);
                    nvs_set_u8(storage_handle, "P5B0E6", banks[0][6].CC);
                    nvs_set_u8(storage_handle, "P5B0E7", banks[0][7].CC);
      
                    nvs_set_u8(storage_handle, "P5B1E0", banks[1][0].CC);
                    nvs_set_u8(storage_handle, "P5B1E1", banks[1][1].CC);
                    nvs_set_u8(storage_handle, "P5B1E2", banks[1][2].CC);
                    nvs_set_u8(storage_handle, "P5B1E3", banks[1][3].CC);
                    nvs_set_u8(storage_handle, "P5B1E4", banks[1][4].CC);
                    nvs_set_u8(storage_handle, "P5B1E5", banks[1][5].CC);
                    nvs_set_u8(storage_handle, "P5B1E6", banks[1][6].CC);
                    nvs_set_u8(storage_handle, "P5B1E7", banks[1][7].CC);
      
                    nvs_set_u8(storage_handle, "P5B2E0", banks[2][0].CC);
                    nvs_set_u8(storage_handle, "P5B2E1", banks[2][1].CC);
                    nvs_set_u8(storage_handle, "P5B2E2", banks[2][2].CC);
                    nvs_set_u8(storage_handle, "P5B2E3", banks[2][3].CC);
                    nvs_set_u8(storage_handle, "P5B2E4", banks[2][4].CC);
                    nvs_set_u8(storage_handle, "P5B2E5", banks[2][5].CC);
                    nvs_set_u8(storage_handle, "P5B2E6", banks[2][6].CC);
                    nvs_set_u8(storage_handle, "P5B2E7", banks[2][7].CC);
      
                    nvs_set_u8(storage_handle, "P5B3E0", banks[3][0].CC);
                    nvs_set_u8(storage_handle, "P5B3E1", banks[3][1].CC);
                    nvs_set_u8(storage_handle, "P5B3E2", banks[3][2].CC);
                    nvs_set_u8(storage_handle, "P5B3E3", banks[3][3].CC);
                    nvs_set_u8(storage_handle, "P5B3E4", banks[3][4].CC);
                    nvs_set_u8(storage_handle, "P5B3E5", banks[3][5].CC);
                    nvs_set_u8(storage_handle, "P5B3E6", banks[3][6].CC);
                    nvs_set_u8(storage_handle, "P5B3E7", banks[3][7].CC);

                    nvs_commit(storage_handle);
                    nvs_close(storage_handle);
                }

                if((Q6buff & 0b00001111) == 6)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);

                    nvs_set_u8(storage_handle, "P6B0E0", banks[0][0].CC);
                    nvs_set_u8(storage_handle, "P6B0E1", banks[0][1].CC);
                    nvs_set_u8(storage_handle, "P6B0E2", banks[0][2].CC);
                    nvs_set_u8(storage_handle, "P6B0E3", banks[0][3].CC);
                    nvs_set_u8(storage_handle, "P6B0E4", banks[0][4].CC);
                    nvs_set_u8(storage_handle, "P6B0E5", banks[0][5].CC);
                    nvs_set_u8(storage_handle, "P6B0E6", banks[0][6].CC);
                    nvs_set_u8(storage_handle, "P6B0E7", banks[0][7].CC);
      
                    nvs_set_u8(storage_handle, "P6B1E0", banks[1][0].CC);
                    nvs_set_u8(storage_handle, "P6B1E1", banks[1][1].CC);
                    nvs_set_u8(storage_handle, "P6B1E2", banks[1][2].CC);
                    nvs_set_u8(storage_handle, "P6B1E3", banks[1][3].CC);
                    nvs_set_u8(storage_handle, "P6B1E4", banks[1][4].CC);
                    nvs_set_u8(storage_handle, "P6B1E5", banks[1][5].CC);
                    nvs_set_u8(storage_handle, "P6B1E6", banks[1][6].CC);
                    nvs_set_u8(storage_handle, "P6B1E7", banks[1][7].CC);
      
                    nvs_set_u8(storage_handle, "P6B2E0", banks[2][0].CC);
                    nvs_set_u8(storage_handle, "P6B2E1", banks[2][1].CC);
                    nvs_set_u8(storage_handle, "P6B2E2", banks[2][2].CC);
                    nvs_set_u8(storage_handle, "P6B2E3", banks[2][3].CC);
                    nvs_set_u8(storage_handle, "P6B2E4", banks[2][4].CC);
                    nvs_set_u8(storage_handle, "P6B2E5", banks[2][5].CC);
                    nvs_set_u8(storage_handle, "P6B2E6", banks[2][6].CC);
                    nvs_set_u8(storage_handle, "P6B2E7", banks[2][7].CC);
      
                    nvs_set_u8(storage_handle, "P6B3E0", banks[3][0].CC);
                    nvs_set_u8(storage_handle, "P6B3E1", banks[3][1].CC);
                    nvs_set_u8(storage_handle, "P6B3E2", banks[3][2].CC);
                    nvs_set_u8(storage_handle, "P6B3E3", banks[3][3].CC);
                    nvs_set_u8(storage_handle, "P6B3E4", banks[3][4].CC);
                    nvs_set_u8(storage_handle, "P6B3E5", banks[3][5].CC);
                    nvs_set_u8(storage_handle, "P6B3E6", banks[3][6].CC);
                    nvs_set_u8(storage_handle, "P6B3E7", banks[3][7].CC);

                    nvs_commit(storage_handle);
                    nvs_close(storage_handle);
                }

                if((Q6buff & 0b00001111) == 7)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);

                    nvs_set_u8(storage_handle, "P7B0E0", banks[0][0].CC);
                    nvs_set_u8(storage_handle, "P7B0E1", banks[0][1].CC);
                    nvs_set_u8(storage_handle, "P7B0E2", banks[0][2].CC);
                    nvs_set_u8(storage_handle, "P7B0E3", banks[0][3].CC);
                    nvs_set_u8(storage_handle, "P7B0E4", banks[0][4].CC);
                    nvs_set_u8(storage_handle, "P7B0E5", banks[0][5].CC);
                    nvs_set_u8(storage_handle, "P7B0E6", banks[0][6].CC);
                    nvs_set_u8(storage_handle, "P7B0E7", banks[0][7].CC);
      
                    nvs_set_u8(storage_handle, "P7B1E0", banks[1][0].CC);
                    nvs_set_u8(storage_handle, "P7B1E1", banks[1][1].CC);
                    nvs_set_u8(storage_handle, "P7B1E2", banks[1][2].CC);
                    nvs_set_u8(storage_handle, "P7B1E3", banks[1][3].CC);
                    nvs_set_u8(storage_handle, "P7B1E4", banks[1][4].CC);
                    nvs_set_u8(storage_handle, "P7B1E5", banks[1][5].CC);
                    nvs_set_u8(storage_handle, "P7B1E6", banks[1][6].CC);
                    nvs_set_u8(storage_handle, "P7B1E7", banks[1][7].CC);
      
                    nvs_set_u8(storage_handle, "P7B2E0", banks[2][0].CC);
                    nvs_set_u8(storage_handle, "P7B2E1", banks[2][1].CC);
                    nvs_set_u8(storage_handle, "P7B2E2", banks[2][2].CC);
                    nvs_set_u8(storage_handle, "P7B2E3", banks[2][3].CC);
                    nvs_set_u8(storage_handle, "P7B2E4", banks[2][4].CC);
                    nvs_set_u8(storage_handle, "P7B2E5", banks[2][5].CC);
                    nvs_set_u8(storage_handle, "P7B2E6", banks[2][6].CC);
                    nvs_set_u8(storage_handle, "P7B2E7", banks[2][7].CC);
      
                    nvs_set_u8(storage_handle, "P7B3E0", banks[3][0].CC);
                    nvs_set_u8(storage_handle, "P7B3E1", banks[3][1].CC);
                    nvs_set_u8(storage_handle, "P7B3E2", banks[3][2].CC);
                    nvs_set_u8(storage_handle, "P7B3E3", banks[3][3].CC);
                    nvs_set_u8(storage_handle, "P7B3E4", banks[3][4].CC);
                    nvs_set_u8(storage_handle, "P7B3E5", banks[3][5].CC);
                    nvs_set_u8(storage_handle, "P7B3E6", banks[3][6].CC);
                    nvs_set_u8(storage_handle, "P7B3E7", banks[3][7].CC);

                    nvs_commit(storage_handle);
                    nvs_close(storage_handle);
                }

                if((Q6buff & 0b00001111) == 8)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);

                    nvs_set_u8(storage_handle, "P8B0E0", banks[0][0].CC);
                    nvs_set_u8(storage_handle, "P8B0E1", banks[0][1].CC);
                    nvs_set_u8(storage_handle, "P8B0E2", banks[0][2].CC);
                    nvs_set_u8(storage_handle, "P8B0E3", banks[0][3].CC);
                    nvs_set_u8(storage_handle, "P8B0E4", banks[0][4].CC);
                    nvs_set_u8(storage_handle, "P8B0E5", banks[0][5].CC);
                    nvs_set_u8(storage_handle, "P8B0E6", banks[0][6].CC);
                    nvs_set_u8(storage_handle, "P8B0E7", banks[0][7].CC);
      
                    nvs_set_u8(storage_handle, "P8B1E0", banks[1][0].CC);
                    nvs_set_u8(storage_handle, "P8B1E1", banks[1][1].CC);
                    nvs_set_u8(storage_handle, "P8B1E2", banks[1][2].CC);
                    nvs_set_u8(storage_handle, "P8B1E3", banks[1][3].CC);
                    nvs_set_u8(storage_handle, "P8B1E4", banks[1][4].CC);
                    nvs_set_u8(storage_handle, "P8B1E5", banks[1][5].CC);
                    nvs_set_u8(storage_handle, "P8B1E6", banks[1][6].CC);
                    nvs_set_u8(storage_handle, "P8B1E7", banks[1][7].CC);
      
                    nvs_set_u8(storage_handle, "P8B2E0", banks[2][0].CC);
                    nvs_set_u8(storage_handle, "P8B2E1", banks[2][1].CC);
                    nvs_set_u8(storage_handle, "P8B2E2", banks[2][2].CC);
                    nvs_set_u8(storage_handle, "P8B2E3", banks[2][3].CC);
                    nvs_set_u8(storage_handle, "P8B2E4", banks[2][4].CC);
                    nvs_set_u8(storage_handle, "P8B2E5", banks[2][5].CC);
                    nvs_set_u8(storage_handle, "P8B2E6", banks[2][6].CC);
                    nvs_set_u8(storage_handle, "P8B2E7", banks[2][7].CC);
      
                    nvs_set_u8(storage_handle, "P8B3E0", banks[3][0].CC);
                    nvs_set_u8(storage_handle, "P8B3E1", banks[3][1].CC);
                    nvs_set_u8(storage_handle, "P8B3E2", banks[3][2].CC);
                    nvs_set_u8(storage_handle, "P8B3E3", banks[3][3].CC);
                    nvs_set_u8(storage_handle, "P8B3E4", banks[3][4].CC);
                    nvs_set_u8(storage_handle, "P8B3E5", banks[3][5].CC);
                    nvs_set_u8(storage_handle, "P8B3E6", banks[3][6].CC);
                    nvs_set_u8(storage_handle, "P8B3E7", banks[3][7].CC);

                    nvs_commit(storage_handle);
                    nvs_close(storage_handle);
                }
                
            }

            else if((Q6buff & 0b11000000) == 0b10000000) // Load
            {
                if((Q6buff & 0b00001111) == 1)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);

                    nvs_get_u8(storage_handle, "P1B0E0", &banks[0][0].CC);
                    nvs_get_u8(storage_handle, "P1B0E1", &banks[0][1].CC);
                    nvs_get_u8(storage_handle, "P1B0E2", &banks[0][2].CC);
                    nvs_get_u8(storage_handle, "P1B0E3", &banks[0][3].CC);
                    nvs_get_u8(storage_handle, "P1B0E4", &banks[0][4].CC);
                    nvs_get_u8(storage_handle, "P1B0E5", &banks[0][5].CC);
                    nvs_get_u8(storage_handle, "P1B0E6", &banks[0][6].CC);
                    nvs_get_u8(storage_handle, "P1B0E7", &banks[0][7].CC);
  
                    nvs_get_u8(storage_handle, "P1B1E0", &banks[1][0].CC);
                    nvs_get_u8(storage_handle, "P1B1E1", &banks[1][1].CC);
                    nvs_get_u8(storage_handle, "P1B1E2", &banks[1][2].CC);
                    nvs_get_u8(storage_handle, "P1B1E3", &banks[1][3].CC);
                    nvs_get_u8(storage_handle, "P1B1E4", &banks[1][4].CC);
                    nvs_get_u8(storage_handle, "P1B1E5", &banks[1][5].CC);
                    nvs_get_u8(storage_handle, "P1B1E6", &banks[1][6].CC);
                    nvs_get_u8(storage_handle, "P1B1E7", &banks[1][7].CC);
  
                    nvs_get_u8(storage_handle, "P1B2E0", &banks[2][0].CC);
                    nvs_get_u8(storage_handle, "P1B2E1", &banks[2][1].CC);
                    nvs_get_u8(storage_handle, "P1B2E2", &banks[2][2].CC);
                    nvs_get_u8(storage_handle, "P1B2E3", &banks[2][3].CC);
                    nvs_get_u8(storage_handle, "P1B2E4", &banks[2][4].CC);
                    nvs_get_u8(storage_handle, "P1B2E5", &banks[2][5].CC);
                    nvs_get_u8(storage_handle, "P1B2E6", &banks[2][6].CC);
                    nvs_get_u8(storage_handle, "P1B2E7", &banks[2][7].CC);
  
                    nvs_get_u8(storage_handle, "P1B3E0", &banks[3][0].CC);
                    nvs_get_u8(storage_handle, "P1B3E1", &banks[3][1].CC);
                    nvs_get_u8(storage_handle, "P1B3E2", &banks[3][2].CC);
                    nvs_get_u8(storage_handle, "P1B3E3", &banks[3][3].CC);
                    nvs_get_u8(storage_handle, "P1B3E4", &banks[3][4].CC);
                    nvs_get_u8(storage_handle, "P1B3E5", &banks[3][5].CC);
                    nvs_get_u8(storage_handle, "P1B3E6", &banks[3][6].CC);
                    nvs_get_u8(storage_handle, "P1B3E7", &banks[3][7].CC);
  
                    nvs_close(storage_handle);
                }
  
                if((Q6buff & 0b00001111) == 2)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);
  
                    nvs_get_u8(storage_handle, "P2B0E0", &banks[0][0].CC);
                    nvs_get_u8(storage_handle, "P2B0E1", &banks[0][1].CC);
                    nvs_get_u8(storage_handle, "P2B0E2", &banks[0][2].CC);
                    nvs_get_u8(storage_handle, "P2B0E3", &banks[0][3].CC);
                    nvs_get_u8(storage_handle, "P2B0E4", &banks[0][4].CC);
                    nvs_get_u8(storage_handle, "P2B0E5", &banks[0][5].CC);
                    nvs_get_u8(storage_handle, "P2B0E6", &banks[0][6].CC);
                    nvs_get_u8(storage_handle, "P2B0E7", &banks[0][7].CC);
  
                    nvs_get_u8(storage_handle, "P2B1E0", &banks[1][0].CC);
                    nvs_get_u8(storage_handle, "P2B1E1", &banks[1][1].CC);
                    nvs_get_u8(storage_handle, "P2B1E2", &banks[1][2].CC);
                    nvs_get_u8(storage_handle, "P2B1E3", &banks[1][3].CC);
                    nvs_get_u8(storage_handle, "P2B1E4", &banks[1][4].CC);
                    nvs_get_u8(storage_handle, "P2B1E5", &banks[1][5].CC);
                    nvs_get_u8(storage_handle, "P2B1E6", &banks[1][6].CC);
                    nvs_get_u8(storage_handle, "P2B1E7", &banks[1][7].CC);
  
                    nvs_get_u8(storage_handle, "P2B2E0", &banks[2][0].CC);
                    nvs_get_u8(storage_handle, "P2B2E1", &banks[2][1].CC);
                    nvs_get_u8(storage_handle, "P2B2E2", &banks[2][2].CC);
                    nvs_get_u8(storage_handle, "P2B2E3", &banks[2][3].CC);
                    nvs_get_u8(storage_handle, "P2B2E4", &banks[2][4].CC);
                    nvs_get_u8(storage_handle, "P2B2E5", &banks[2][5].CC);
                    nvs_get_u8(storage_handle, "P2B2E6", &banks[2][6].CC);
                    nvs_get_u8(storage_handle, "P2B2E7", &banks[2][7].CC);
  
                    nvs_get_u8(storage_handle, "P2B3E0", &banks[3][0].CC);
                    nvs_get_u8(storage_handle, "P2B3E1", &banks[3][1].CC);
                    nvs_get_u8(storage_handle, "P2B3E2", &banks[3][2].CC);
                    nvs_get_u8(storage_handle, "P2B3E3", &banks[3][3].CC);
                    nvs_get_u8(storage_handle, "P2B3E4", &banks[3][4].CC);
                    nvs_get_u8(storage_handle, "P2B3E5", &banks[3][5].CC);
                    nvs_get_u8(storage_handle, "P2B3E6", &banks[3][6].CC);
                    nvs_get_u8(storage_handle, "P2B3E7", &banks[3][7].CC);
  
                    nvs_close(storage_handle);
                }
  
                if((Q6buff & 0b00001111) == 3)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);
  
                    nvs_get_u8(storage_handle, "P3B0E0", &banks[0][0].CC);
                    nvs_get_u8(storage_handle, "P3B0E1", &banks[0][1].CC);
                    nvs_get_u8(storage_handle, "P3B0E2", &banks[0][2].CC);
                    nvs_get_u8(storage_handle, "P3B0E3", &banks[0][3].CC);
                    nvs_get_u8(storage_handle, "P3B0E4", &banks[0][4].CC);
                    nvs_get_u8(storage_handle, "P3B0E5", &banks[0][5].CC);
                    nvs_get_u8(storage_handle, "P3B0E6", &banks[0][6].CC);
                    nvs_get_u8(storage_handle, "P3B0E7", &banks[0][7].CC);
  
                    nvs_get_u8(storage_handle, "P3B1E0", &banks[1][0].CC);
                    nvs_get_u8(storage_handle, "P3B1E1", &banks[1][1].CC);
                    nvs_get_u8(storage_handle, "P3B1E2", &banks[1][2].CC);
                    nvs_get_u8(storage_handle, "P3B1E3", &banks[1][3].CC);
                    nvs_get_u8(storage_handle, "P3B1E4", &banks[1][4].CC);
                    nvs_get_u8(storage_handle, "P3B1E5", &banks[1][5].CC);
                    nvs_get_u8(storage_handle, "P3B1E6", &banks[1][6].CC);
                    nvs_get_u8(storage_handle, "P3B1E7", &banks[1][7].CC);
  
                    nvs_get_u8(storage_handle, "P3B2E0", &banks[2][0].CC);
                    nvs_get_u8(storage_handle, "P3B2E1", &banks[2][1].CC);
                    nvs_get_u8(storage_handle, "P3B2E2", &banks[2][2].CC);
                    nvs_get_u8(storage_handle, "P3B2E3", &banks[2][3].CC);
                    nvs_get_u8(storage_handle, "P3B2E4", &banks[2][4].CC);
                    nvs_get_u8(storage_handle, "P3B2E5", &banks[2][5].CC);
                    nvs_get_u8(storage_handle, "P3B2E6", &banks[2][6].CC);
                    nvs_get_u8(storage_handle, "P3B2E7", &banks[2][7].CC);
  
                    nvs_get_u8(storage_handle, "P3B3E0", &banks[3][0].CC);
                    nvs_get_u8(storage_handle, "P3B3E1", &banks[3][1].CC);
                    nvs_get_u8(storage_handle, "P3B3E2", &banks[3][2].CC);
                    nvs_get_u8(storage_handle, "P3B3E3", &banks[3][3].CC);
                    nvs_get_u8(storage_handle, "P3B3E4", &banks[3][4].CC);
                    nvs_get_u8(storage_handle, "P3B3E5", &banks[3][5].CC);
                    nvs_get_u8(storage_handle, "P3B3E6", &banks[3][6].CC);
                    nvs_get_u8(storage_handle, "P3B3E7", &banks[3][7].CC);
  
                    nvs_close(storage_handle);
                }
  
                if((Q6buff & 0b00001111) == 4)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);
  
                    nvs_get_u8(storage_handle, "P4B0E0", &banks[0][0].CC);
                    nvs_get_u8(storage_handle, "P4B0E1", &banks[0][1].CC);
                    nvs_get_u8(storage_handle, "P4B0E2", &banks[0][2].CC);
                    nvs_get_u8(storage_handle, "P4B0E3", &banks[0][3].CC);
                    nvs_get_u8(storage_handle, "P4B0E4", &banks[0][4].CC);
                    nvs_get_u8(storage_handle, "P4B0E5", &banks[0][5].CC);
                    nvs_get_u8(storage_handle, "P4B0E6", &banks[0][6].CC);
                    nvs_get_u8(storage_handle, "P4B0E7", &banks[0][7].CC);
  
                    nvs_get_u8(storage_handle, "P4B1E0", &banks[1][0].CC);
                    nvs_get_u8(storage_handle, "P4B1E1", &banks[1][1].CC);
                    nvs_get_u8(storage_handle, "P4B1E2", &banks[1][2].CC);
                    nvs_get_u8(storage_handle, "P4B1E3", &banks[1][3].CC);
                    nvs_get_u8(storage_handle, "P4B1E4", &banks[1][4].CC);
                    nvs_get_u8(storage_handle, "P4B1E5", &banks[1][5].CC);
                    nvs_get_u8(storage_handle, "P4B1E6", &banks[1][6].CC);
                    nvs_get_u8(storage_handle, "P4B1E7", &banks[1][7].CC);
  
                    nvs_get_u8(storage_handle, "P4B2E0", &banks[2][0].CC);
                    nvs_get_u8(storage_handle, "P4B2E1", &banks[2][1].CC);
                    nvs_get_u8(storage_handle, "P4B2E2", &banks[2][2].CC);
                    nvs_get_u8(storage_handle, "P4B2E3", &banks[2][3].CC);
                    nvs_get_u8(storage_handle, "P4B2E4", &banks[2][4].CC);
                    nvs_get_u8(storage_handle, "P4B2E5", &banks[2][5].CC);
                    nvs_get_u8(storage_handle, "P4B2E6", &banks[2][6].CC);
                    nvs_get_u8(storage_handle, "P4B2E7", &banks[2][7].CC);
  
                    nvs_get_u8(storage_handle, "P4B3E0", &banks[3][0].CC);
                    nvs_get_u8(storage_handle, "P4B3E1", &banks[3][1].CC);
                    nvs_get_u8(storage_handle, "P4B3E2", &banks[3][2].CC);
                    nvs_get_u8(storage_handle, "P4B3E3", &banks[3][3].CC);
                    nvs_get_u8(storage_handle, "P4B3E4", &banks[3][4].CC);
                    nvs_get_u8(storage_handle, "P4B3E5", &banks[3][5].CC);
                    nvs_get_u8(storage_handle, "P4B3E6", &banks[3][6].CC);
                    nvs_get_u8(storage_handle, "P4B3E7", &banks[3][7].CC);
  
                    nvs_close(storage_handle);
                }

                if((Q6buff & 0b00001111) == 5)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);

                    nvs_get_u8(storage_handle, "P5B0E0", &banks[0][0].CC);
                    nvs_get_u8(storage_handle, "P5B0E1", &banks[0][1].CC);
                    nvs_get_u8(storage_handle, "P5B0E2", &banks[0][2].CC);
                    nvs_get_u8(storage_handle, "P5B0E3", &banks[0][3].CC);
                    nvs_get_u8(storage_handle, "P5B0E4", &banks[0][4].CC);
                    nvs_get_u8(storage_handle, "P5B0E5", &banks[0][5].CC);
                    nvs_get_u8(storage_handle, "P5B0E6", &banks[0][6].CC);
                    nvs_get_u8(storage_handle, "P5B0E7", &banks[0][7].CC);
  
                    nvs_get_u8(storage_handle, "P5B1E0", &banks[1][0].CC);
                    nvs_get_u8(storage_handle, "P5B1E1", &banks[1][1].CC);
                    nvs_get_u8(storage_handle, "P5B1E2", &banks[1][2].CC);
                    nvs_get_u8(storage_handle, "P5B1E3", &banks[1][3].CC);
                    nvs_get_u8(storage_handle, "P5B1E4", &banks[1][4].CC);
                    nvs_get_u8(storage_handle, "P5B1E5", &banks[1][5].CC);
                    nvs_get_u8(storage_handle, "P5B1E6", &banks[1][6].CC);
                    nvs_get_u8(storage_handle, "P5B1E7", &banks[1][7].CC);
  
                    nvs_get_u8(storage_handle, "P5B2E0", &banks[2][0].CC);
                    nvs_get_u8(storage_handle, "P5B2E1", &banks[2][1].CC);
                    nvs_get_u8(storage_handle, "P5B2E2", &banks[2][2].CC);
                    nvs_get_u8(storage_handle, "P5B2E3", &banks[2][3].CC);
                    nvs_get_u8(storage_handle, "P5B2E4", &banks[2][4].CC);
                    nvs_get_u8(storage_handle, "P5B2E5", &banks[2][5].CC);
                    nvs_get_u8(storage_handle, "P5B2E6", &banks[2][6].CC);
                    nvs_get_u8(storage_handle, "P5B2E7", &banks[2][7].CC);
  
                    nvs_get_u8(storage_handle, "P5B3E0", &banks[3][0].CC);
                    nvs_get_u8(storage_handle, "P5B3E1", &banks[3][1].CC);
                    nvs_get_u8(storage_handle, "P5B3E2", &banks[3][2].CC);
                    nvs_get_u8(storage_handle, "P5B3E3", &banks[3][3].CC);
                    nvs_get_u8(storage_handle, "P5B3E4", &banks[3][4].CC);
                    nvs_get_u8(storage_handle, "P5B3E5", &banks[3][5].CC);
                    nvs_get_u8(storage_handle, "P5B3E6", &banks[3][6].CC);
                    nvs_get_u8(storage_handle, "P5B3E7", &banks[3][7].CC);
  
                    nvs_close(storage_handle);
                }
  
                if((Q6buff & 0b00001111) == 6)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);
  
                    nvs_get_u8(storage_handle, "P6B0E0", &banks[0][0].CC);
                    nvs_get_u8(storage_handle, "P6B0E1", &banks[0][1].CC);
                    nvs_get_u8(storage_handle, "P6B0E2", &banks[0][2].CC);
                    nvs_get_u8(storage_handle, "P6B0E3", &banks[0][3].CC);
                    nvs_get_u8(storage_handle, "P6B0E4", &banks[0][4].CC);
                    nvs_get_u8(storage_handle, "P6B0E5", &banks[0][5].CC);
                    nvs_get_u8(storage_handle, "P6B0E6", &banks[0][6].CC);
                    nvs_get_u8(storage_handle, "P6B0E7", &banks[0][7].CC);
  
                    nvs_get_u8(storage_handle, "P6B1E0", &banks[1][0].CC);
                    nvs_get_u8(storage_handle, "P6B1E1", &banks[1][1].CC);
                    nvs_get_u8(storage_handle, "P6B1E2", &banks[1][2].CC);
                    nvs_get_u8(storage_handle, "P6B1E3", &banks[1][3].CC);
                    nvs_get_u8(storage_handle, "P6B1E4", &banks[1][4].CC);
                    nvs_get_u8(storage_handle, "P6B1E5", &banks[1][5].CC);
                    nvs_get_u8(storage_handle, "P6B1E6", &banks[1][6].CC);
                    nvs_get_u8(storage_handle, "P6B1E7", &banks[1][7].CC);
  
                    nvs_get_u8(storage_handle, "P6B2E0", &banks[2][0].CC);
                    nvs_get_u8(storage_handle, "P6B2E1", &banks[2][1].CC);
                    nvs_get_u8(storage_handle, "P6B2E2", &banks[2][2].CC);
                    nvs_get_u8(storage_handle, "P6B2E3", &banks[2][3].CC);
                    nvs_get_u8(storage_handle, "P6B2E4", &banks[2][4].CC);
                    nvs_get_u8(storage_handle, "P6B2E5", &banks[2][5].CC);
                    nvs_get_u8(storage_handle, "P6B2E6", &banks[2][6].CC);
                    nvs_get_u8(storage_handle, "P6B2E7", &banks[2][7].CC);
  
                    nvs_get_u8(storage_handle, "P6B3E0", &banks[3][0].CC);
                    nvs_get_u8(storage_handle, "P6B3E1", &banks[3][1].CC);
                    nvs_get_u8(storage_handle, "P6B3E2", &banks[3][2].CC);
                    nvs_get_u8(storage_handle, "P6B3E3", &banks[3][3].CC);
                    nvs_get_u8(storage_handle, "P6B3E4", &banks[3][4].CC);
                    nvs_get_u8(storage_handle, "P6B3E5", &banks[3][5].CC);
                    nvs_get_u8(storage_handle, "P6B3E6", &banks[3][6].CC);
                    nvs_get_u8(storage_handle, "P6B3E7", &banks[3][7].CC);
  
                    nvs_close(storage_handle);
                }
  
                if((Q6buff & 0b00001111) == 7)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);
  
                    nvs_get_u8(storage_handle, "P7B0E0", &banks[0][0].CC);
                    nvs_get_u8(storage_handle, "P7B0E1", &banks[0][1].CC);
                    nvs_get_u8(storage_handle, "P7B0E2", &banks[0][2].CC);
                    nvs_get_u8(storage_handle, "P7B0E3", &banks[0][3].CC);
                    nvs_get_u8(storage_handle, "P7B0E4", &banks[0][4].CC);
                    nvs_get_u8(storage_handle, "P7B0E5", &banks[0][5].CC);
                    nvs_get_u8(storage_handle, "P7B0E6", &banks[0][6].CC);
                    nvs_get_u8(storage_handle, "P7B0E7", &banks[0][7].CC);
  
                    nvs_get_u8(storage_handle, "P7B1E0", &banks[1][0].CC);
                    nvs_get_u8(storage_handle, "P7B1E1", &banks[1][1].CC);
                    nvs_get_u8(storage_handle, "P7B1E2", &banks[1][2].CC);
                    nvs_get_u8(storage_handle, "P7B1E3", &banks[1][3].CC);
                    nvs_get_u8(storage_handle, "P7B1E4", &banks[1][4].CC);
                    nvs_get_u8(storage_handle, "P7B1E5", &banks[1][5].CC);
                    nvs_get_u8(storage_handle, "P7B1E6", &banks[1][6].CC);
                    nvs_get_u8(storage_handle, "P7B1E7", &banks[1][7].CC);
  
                    nvs_get_u8(storage_handle, "P7B2E0", &banks[2][0].CC);
                    nvs_get_u8(storage_handle, "P7B2E1", &banks[2][1].CC);
                    nvs_get_u8(storage_handle, "P7B2E2", &banks[2][2].CC);
                    nvs_get_u8(storage_handle, "P7B2E3", &banks[2][3].CC);
                    nvs_get_u8(storage_handle, "P7B2E4", &banks[2][4].CC);
                    nvs_get_u8(storage_handle, "P7B2E5", &banks[2][5].CC);
                    nvs_get_u8(storage_handle, "P7B2E6", &banks[2][6].CC);
                    nvs_get_u8(storage_handle, "P7B2E7", &banks[2][7].CC);
  
                    nvs_get_u8(storage_handle, "P7B3E0", &banks[3][0].CC);
                    nvs_get_u8(storage_handle, "P7B3E1", &banks[3][1].CC);
                    nvs_get_u8(storage_handle, "P7B3E2", &banks[3][2].CC);
                    nvs_get_u8(storage_handle, "P7B3E3", &banks[3][3].CC);
                    nvs_get_u8(storage_handle, "P7B3E4", &banks[3][4].CC);
                    nvs_get_u8(storage_handle, "P7B3E5", &banks[3][5].CC);
                    nvs_get_u8(storage_handle, "P7B3E6", &banks[3][6].CC);
                    nvs_get_u8(storage_handle, "P7B3E7", &banks[3][7].CC);
  
                    nvs_close(storage_handle);
                }
  
                if((Q6buff & 0b00001111) == 8)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);
  
                    nvs_get_u8(storage_handle, "P8B0E0", &banks[0][0].CC);
                    nvs_get_u8(storage_handle, "P8B0E1", &banks[0][1].CC);
                    nvs_get_u8(storage_handle, "P8B0E2", &banks[0][2].CC);
                    nvs_get_u8(storage_handle, "P8B0E3", &banks[0][3].CC);
                    nvs_get_u8(storage_handle, "P8B0E4", &banks[0][4].CC);
                    nvs_get_u8(storage_handle, "P8B0E5", &banks[0][5].CC);
                    nvs_get_u8(storage_handle, "P8B0E6", &banks[0][6].CC);
                    nvs_get_u8(storage_handle, "P8B0E7", &banks[0][7].CC);
  
                    nvs_get_u8(storage_handle, "P8B1E0", &banks[1][0].CC);
                    nvs_get_u8(storage_handle, "P8B1E1", &banks[1][1].CC);
                    nvs_get_u8(storage_handle, "P8B1E2", &banks[1][2].CC);
                    nvs_get_u8(storage_handle, "P8B1E3", &banks[1][3].CC);
                    nvs_get_u8(storage_handle, "P8B1E4", &banks[1][4].CC);
                    nvs_get_u8(storage_handle, "P8B1E5", &banks[1][5].CC);
                    nvs_get_u8(storage_handle, "P8B1E6", &banks[1][6].CC);
                    nvs_get_u8(storage_handle, "P8B1E7", &banks[1][7].CC);
  
                    nvs_get_u8(storage_handle, "P8B2E0", &banks[2][0].CC);
                    nvs_get_u8(storage_handle, "P8B2E1", &banks[2][1].CC);
                    nvs_get_u8(storage_handle, "P8B2E2", &banks[2][2].CC);
                    nvs_get_u8(storage_handle, "P8B2E3", &banks[2][3].CC);
                    nvs_get_u8(storage_handle, "P8B2E4", &banks[2][4].CC);
                    nvs_get_u8(storage_handle, "P8B2E5", &banks[2][5].CC);
                    nvs_get_u8(storage_handle, "P8B2E6", &banks[2][6].CC);
                    nvs_get_u8(storage_handle, "P8B2E7", &banks[2][7].CC);
  
                    nvs_get_u8(storage_handle, "P8B3E0", &banks[3][0].CC);
                    nvs_get_u8(storage_handle, "P8B3E1", &banks[3][1].CC);
                    nvs_get_u8(storage_handle, "P8B3E2", &banks[3][2].CC);
                    nvs_get_u8(storage_handle, "P8B3E3", &banks[3][3].CC);
                    nvs_get_u8(storage_handle, "P8B3E4", &banks[3][4].CC);
                    nvs_get_u8(storage_handle, "P8B3E5", &banks[3][5].CC);
                    nvs_get_u8(storage_handle, "P8B3E6", &banks[3][6].CC);
                    nvs_get_u8(storage_handle, "P8B3E7", &banks[3][7].CC);
  
                    nvs_close(storage_handle);
                }
            }
  
            if((Q6buff & 0b11000000) == 0b11000000) // clear
            {
                if((Q6buff & 0b00001111) == 1)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);
  
                    nvs_set_u8(storage_handle, "P1B0E0", 0);
                    nvs_set_u8(storage_handle, "P1B0E1", 0);
                    nvs_set_u8(storage_handle, "P1B0E2", 0);
                    nvs_set_u8(storage_handle, "P1B0E3", 0);
                    nvs_set_u8(storage_handle, "P1B0E4", 0);
                    nvs_set_u8(storage_handle, "P1B0E5", 0);
                    nvs_set_u8(storage_handle, "P1B0E6", 0);
                    nvs_set_u8(storage_handle, "P1B0E7", 0);
  
                    nvs_set_u8(storage_handle, "P1B1E0", 0);
                    nvs_set_u8(storage_handle, "P1B1E1", 0);
                    nvs_set_u8(storage_handle, "P1B1E2", 0);
                    nvs_set_u8(storage_handle, "P1B1E3", 0);
                    nvs_set_u8(storage_handle, "P1B1E4", 0);
                    nvs_set_u8(storage_handle, "P1B1E5", 0);
                    nvs_set_u8(storage_handle, "P1B1E6", 0);
                    nvs_set_u8(storage_handle, "P1B1E7", 0);
  
                    nvs_set_u8(storage_handle, "P1B2E0", 0);
                    nvs_set_u8(storage_handle, "P1B2E1", 0);
                    nvs_set_u8(storage_handle, "P1B2E2", 0);
                    nvs_set_u8(storage_handle, "P1B2E3", 0);
                    nvs_set_u8(storage_handle, "P1B2E4", 0);
                    nvs_set_u8(storage_handle, "P1B2E5", 0);
                    nvs_set_u8(storage_handle, "P1B2E6", 0);
                    nvs_set_u8(storage_handle, "P1B2E7", 0);
  
                    nvs_set_u8(storage_handle, "P1B3E0", 0);
                    nvs_set_u8(storage_handle, "P1B3E1", 0);
                    nvs_set_u8(storage_handle, "P1B3E2", 0);
                    nvs_set_u8(storage_handle, "P1B3E3", 0);
                    nvs_set_u8(storage_handle, "P1B3E4", 0);
                    nvs_set_u8(storage_handle, "P1B3E5", 0);
                    nvs_set_u8(storage_handle, "P1B3E6", 0);
                    nvs_set_u8(storage_handle, "P1B3E7", 0);
  
                    nvs_commit(storage_handle);
                    nvs_close(storage_handle);
                }
  
                if((Q6buff & 0b00001111) == 2)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);
  
                    nvs_set_u8(storage_handle, "P2B0E0", 0);
                    nvs_set_u8(storage_handle, "P2B0E1", 0);
                    nvs_set_u8(storage_handle, "P2B0E2", 0);
                    nvs_set_u8(storage_handle, "P2B0E3", 0);
                    nvs_set_u8(storage_handle, "P2B0E4", 0);
                    nvs_set_u8(storage_handle, "P2B0E5", 0);
                    nvs_set_u8(storage_handle, "P2B0E6", 0);
                    nvs_set_u8(storage_handle, "P2B0E7", 0);
  
                    nvs_set_u8(storage_handle, "P2B1E0", 0);
                    nvs_set_u8(storage_handle, "P2B1E1", 0);
                    nvs_set_u8(storage_handle, "P2B1E2", 0);
                    nvs_set_u8(storage_handle, "P2B1E3", 0);
                    nvs_set_u8(storage_handle, "P2B1E4", 0);
                    nvs_set_u8(storage_handle, "P2B1E5", 0);
                    nvs_set_u8(storage_handle, "P2B1E6", 0);
                    nvs_set_u8(storage_handle, "P2B1E7", 0);
  
                    nvs_set_u8(storage_handle, "P2B2E0", 0);
                    nvs_set_u8(storage_handle, "P2B2E1", 0);
                    nvs_set_u8(storage_handle, "P2B2E2", 0);
                    nvs_set_u8(storage_handle, "P2B2E3", 0);
                    nvs_set_u8(storage_handle, "P2B2E4", 0);
                    nvs_set_u8(storage_handle, "P2B2E5", 0);
                    nvs_set_u8(storage_handle, "P2B2E6", 0);
                    nvs_set_u8(storage_handle, "P2B2E7", 0);
  
                    nvs_set_u8(storage_handle, "P2B3E0", 0);
                    nvs_set_u8(storage_handle, "P2B3E1", 0);
                    nvs_set_u8(storage_handle, "P2B3E2", 0);
                    nvs_set_u8(storage_handle, "P2B3E3", 0);
                    nvs_set_u8(storage_handle, "P2B3E4", 0);
                    nvs_set_u8(storage_handle, "P2B3E5", 0);
                    nvs_set_u8(storage_handle, "P2B3E6", 0);
                    nvs_set_u8(storage_handle, "P2B3E7", 0);
  
                    nvs_commit(storage_handle);
                    nvs_close(storage_handle);
                }
  
                if((Q6buff & 0b00001111) == 3)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);
  
                    nvs_set_u8(storage_handle, "P3B0E0", 0);
                    nvs_set_u8(storage_handle, "P3B0E1", 0);
                    nvs_set_u8(storage_handle, "P3B0E2", 0);
                    nvs_set_u8(storage_handle, "P3B0E3", 0);
                    nvs_set_u8(storage_handle, "P3B0E4", 0);
                    nvs_set_u8(storage_handle, "P3B0E5", 0);
                    nvs_set_u8(storage_handle, "P3B0E6", 0);
                    nvs_set_u8(storage_handle, "P3B0E7", 0);
  
                    nvs_set_u8(storage_handle, "P3B1E0", 0);
                    nvs_set_u8(storage_handle, "P3B1E1", 0);
                    nvs_set_u8(storage_handle, "P3B1E2", 0);
                    nvs_set_u8(storage_handle, "P3B1E3", 0);
                    nvs_set_u8(storage_handle, "P3B1E4", 0);
                    nvs_set_u8(storage_handle, "P3B1E5", 0);
                    nvs_set_u8(storage_handle, "P3B1E6", 0);
                    nvs_set_u8(storage_handle, "P3B1E7", 0);
  
                    nvs_set_u8(storage_handle, "P3B2E0", 0);
                    nvs_set_u8(storage_handle, "P3B2E1", 0);
                    nvs_set_u8(storage_handle, "P3B2E2", 0);
                    nvs_set_u8(storage_handle, "P3B2E3", 0);
                    nvs_set_u8(storage_handle, "P3B2E4", 0);
                    nvs_set_u8(storage_handle, "P3B2E5", 0);
                    nvs_set_u8(storage_handle, "P3B2E6", 0);
                    nvs_set_u8(storage_handle, "P3B2E7", 0);
  
                    nvs_set_u8(storage_handle, "P3B3E0", 0);
                    nvs_set_u8(storage_handle, "P3B3E1", 0);
                    nvs_set_u8(storage_handle, "P3B3E2", 0);
                    nvs_set_u8(storage_handle, "P3B3E3", 0);
                    nvs_set_u8(storage_handle, "P3B3E4", 0);
                    nvs_set_u8(storage_handle, "P3B3E5", 0);
                    nvs_set_u8(storage_handle, "P3B3E6", 0);
                    nvs_set_u8(storage_handle, "P3B3E7", 0);
  
                    nvs_commit(storage_handle);
                    nvs_close(storage_handle);
                }
  
                if((Q6buff & 0b00001111) == 4)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);
  
                    nvs_set_u8(storage_handle, "P4B0E0", 0);
                    nvs_set_u8(storage_handle, "P4B0E1", 0);
                    nvs_set_u8(storage_handle, "P4B0E2", 0);
                    nvs_set_u8(storage_handle, "P4B0E3", 0);
                    nvs_set_u8(storage_handle, "P4B0E4", 0);
                    nvs_set_u8(storage_handle, "P4B0E5", 0);
                    nvs_set_u8(storage_handle, "P4B0E6", 0);
                    nvs_set_u8(storage_handle, "P4B0E7", 0);
  
                    nvs_set_u8(storage_handle, "P4B1E0", 0);
                    nvs_set_u8(storage_handle, "P4B1E1", 0);
                    nvs_set_u8(storage_handle, "P4B1E2", 0);
                    nvs_set_u8(storage_handle, "P4B1E3", 0);
                    nvs_set_u8(storage_handle, "P4B1E4", 0);
                    nvs_set_u8(storage_handle, "P4B1E5", 0);
                    nvs_set_u8(storage_handle, "P4B1E6", 0);
                    nvs_set_u8(storage_handle, "P4B1E7", 0);
  
                    nvs_set_u8(storage_handle, "P4B2E0", 0);
                    nvs_set_u8(storage_handle, "P4B2E1", 0);
                    nvs_set_u8(storage_handle, "P4B2E2", 0);
                    nvs_set_u8(storage_handle, "P4B2E3", 0);
                    nvs_set_u8(storage_handle, "P4B2E4", 0);
                    nvs_set_u8(storage_handle, "P4B2E5", 0);
                    nvs_set_u8(storage_handle, "P4B2E6", 0);
                    nvs_set_u8(storage_handle, "P4B2E7", 0);
  
                    nvs_set_u8(storage_handle, "P4B3E0", 0);
                    nvs_set_u8(storage_handle, "P4B3E1", 0);
                    nvs_set_u8(storage_handle, "P4B3E2", 0);
                    nvs_set_u8(storage_handle, "P4B3E3", 0);
                    nvs_set_u8(storage_handle, "P4B3E4", 0);
                    nvs_set_u8(storage_handle, "P4B3E5", 0);
                    nvs_set_u8(storage_handle, "P4B3E6", 0);
                    nvs_set_u8(storage_handle, "P4B3E7", 0);
  
                    nvs_commit(storage_handle);
                    nvs_close(storage_handle);
                }

                if((Q6buff & 0b00001111) == 5)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);
  
                    nvs_set_u8(storage_handle, "P5B0E0", 0);
                    nvs_set_u8(storage_handle, "P5B0E1", 0);
                    nvs_set_u8(storage_handle, "P5B0E2", 0);
                    nvs_set_u8(storage_handle, "P5B0E3", 0);
                    nvs_set_u8(storage_handle, "P5B0E4", 0);
                    nvs_set_u8(storage_handle, "P5B0E5", 0);
                    nvs_set_u8(storage_handle, "P5B0E6", 0);
                    nvs_set_u8(storage_handle, "P5B0E7", 0);
  
                    nvs_set_u8(storage_handle, "P5B1E0", 0);
                    nvs_set_u8(storage_handle, "P5B1E1", 0);
                    nvs_set_u8(storage_handle, "P5B1E2", 0);
                    nvs_set_u8(storage_handle, "P5B1E3", 0);
                    nvs_set_u8(storage_handle, "P5B1E4", 0);
                    nvs_set_u8(storage_handle, "P5B1E5", 0);
                    nvs_set_u8(storage_handle, "P5B1E6", 0);
                    nvs_set_u8(storage_handle, "P5B1E7", 0);
  
                    nvs_set_u8(storage_handle, "P5B2E0", 0);
                    nvs_set_u8(storage_handle, "P5B2E1", 0);
                    nvs_set_u8(storage_handle, "P5B2E2", 0);
                    nvs_set_u8(storage_handle, "P5B2E3", 0);
                    nvs_set_u8(storage_handle, "P5B2E4", 0);
                    nvs_set_u8(storage_handle, "P5B2E5", 0);
                    nvs_set_u8(storage_handle, "P5B2E6", 0);
                    nvs_set_u8(storage_handle, "P5B2E7", 0);
  
                    nvs_set_u8(storage_handle, "P5B3E0", 0);
                    nvs_set_u8(storage_handle, "P5B3E1", 0);
                    nvs_set_u8(storage_handle, "P5B3E2", 0);
                    nvs_set_u8(storage_handle, "P5B3E3", 0);
                    nvs_set_u8(storage_handle, "P5B3E4", 0);
                    nvs_set_u8(storage_handle, "P5B3E5", 0);
                    nvs_set_u8(storage_handle, "P5B3E6", 0);
                    nvs_set_u8(storage_handle, "P5B3E7", 0);
  
                    nvs_commit(storage_handle);
                    nvs_close(storage_handle);
                }
  
                if((Q6buff & 0b00001111) == 6)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);
  
                    nvs_set_u8(storage_handle, "P6B0E0", 0);
                    nvs_set_u8(storage_handle, "P6B0E1", 0);
                    nvs_set_u8(storage_handle, "P6B0E2", 0);
                    nvs_set_u8(storage_handle, "P6B0E3", 0);
                    nvs_set_u8(storage_handle, "P6B0E4", 0);
                    nvs_set_u8(storage_handle, "P6B0E5", 0);
                    nvs_set_u8(storage_handle, "P6B0E6", 0);
                    nvs_set_u8(storage_handle, "P6B0E7", 0);
  
                    nvs_set_u8(storage_handle, "P6B1E0", 0);
                    nvs_set_u8(storage_handle, "P6B1E1", 0);
                    nvs_set_u8(storage_handle, "P6B1E2", 0);
                    nvs_set_u8(storage_handle, "P6B1E3", 0);
                    nvs_set_u8(storage_handle, "P6B1E4", 0);
                    nvs_set_u8(storage_handle, "P6B1E5", 0);
                    nvs_set_u8(storage_handle, "P6B1E6", 0);
                    nvs_set_u8(storage_handle, "P6B1E7", 0);
  
                    nvs_set_u8(storage_handle, "P6B2E0", 0);
                    nvs_set_u8(storage_handle, "P6B2E1", 0);
                    nvs_set_u8(storage_handle, "P6B2E2", 0);
                    nvs_set_u8(storage_handle, "P6B2E3", 0);
                    nvs_set_u8(storage_handle, "P6B2E4", 0);
                    nvs_set_u8(storage_handle, "P6B2E5", 0);
                    nvs_set_u8(storage_handle, "P6B2E6", 0);
                    nvs_set_u8(storage_handle, "P6B2E7", 0);
  
                    nvs_set_u8(storage_handle, "P6B3E0", 0);
                    nvs_set_u8(storage_handle, "P6B3E1", 0);
                    nvs_set_u8(storage_handle, "P6B3E2", 0);
                    nvs_set_u8(storage_handle, "P6B3E3", 0);
                    nvs_set_u8(storage_handle, "P6B3E4", 0);
                    nvs_set_u8(storage_handle, "P6B3E5", 0);
                    nvs_set_u8(storage_handle, "P6B3E6", 0);
                    nvs_set_u8(storage_handle, "P6B3E7", 0);
  
                    nvs_commit(storage_handle);
                    nvs_close(storage_handle);
                }
  
                if((Q6buff & 0b00001111) == 7)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);
  
                    nvs_set_u8(storage_handle, "P7B0E0", 0);
                    nvs_set_u8(storage_handle, "P7B0E1", 0);
                    nvs_set_u8(storage_handle, "P7B0E2", 0);
                    nvs_set_u8(storage_handle, "P7B0E3", 0);
                    nvs_set_u8(storage_handle, "P7B0E4", 0);
                    nvs_set_u8(storage_handle, "P7B0E5", 0);
                    nvs_set_u8(storage_handle, "P7B0E6", 0);
                    nvs_set_u8(storage_handle, "P7B0E7", 0);
  
                    nvs_set_u8(storage_handle, "P7B1E0", 0);
                    nvs_set_u8(storage_handle, "P7B1E1", 0);
                    nvs_set_u8(storage_handle, "P7B1E2", 0);
                    nvs_set_u8(storage_handle, "P7B1E3", 0);
                    nvs_set_u8(storage_handle, "P7B1E4", 0);
                    nvs_set_u8(storage_handle, "P7B1E5", 0);
                    nvs_set_u8(storage_handle, "P7B1E6", 0);
                    nvs_set_u8(storage_handle, "P7B1E7", 0);
  
                    nvs_set_u8(storage_handle, "P7B2E0", 0);
                    nvs_set_u8(storage_handle, "P7B2E1", 0);
                    nvs_set_u8(storage_handle, "P7B2E2", 0);
                    nvs_set_u8(storage_handle, "P7B2E3", 0);
                    nvs_set_u8(storage_handle, "P7B2E4", 0);
                    nvs_set_u8(storage_handle, "P7B2E5", 0);
                    nvs_set_u8(storage_handle, "P7B2E6", 0);
                    nvs_set_u8(storage_handle, "P7B2E7", 0);
  
                    nvs_set_u8(storage_handle, "P7B3E0", 0);
                    nvs_set_u8(storage_handle, "P7B3E1", 0);
                    nvs_set_u8(storage_handle, "P7B3E2", 0);
                    nvs_set_u8(storage_handle, "P7B3E3", 0);
                    nvs_set_u8(storage_handle, "P7B3E4", 0);
                    nvs_set_u8(storage_handle, "P7B3E5", 0);
                    nvs_set_u8(storage_handle, "P7B3E6", 0);
                    nvs_set_u8(storage_handle, "P7B3E7", 0);
  
                    nvs_commit(storage_handle);
                    nvs_close(storage_handle);
                }
  
                if((Q6buff & 0b00001111) == 8)
                {
                    nvs_open("CC_Storage", NVS_READWRITE, &storage_handle);
  
                    nvs_set_u8(storage_handle, "P8B0E0", 0);
                    nvs_set_u8(storage_handle, "P8B0E1", 0);
                    nvs_set_u8(storage_handle, "P8B0E2", 0);
                    nvs_set_u8(storage_handle, "P8B0E3", 0);
                    nvs_set_u8(storage_handle, "P8B0E4", 0);
                    nvs_set_u8(storage_handle, "P8B0E5", 0);
                    nvs_set_u8(storage_handle, "P8B0E6", 0);
                    nvs_set_u8(storage_handle, "P8B0E7", 0);
  
                    nvs_set_u8(storage_handle, "P8B1E0", 0);
                    nvs_set_u8(storage_handle, "P8B1E1", 0);
                    nvs_set_u8(storage_handle, "P8B1E2", 0);
                    nvs_set_u8(storage_handle, "P8B1E3", 0);
                    nvs_set_u8(storage_handle, "P8B1E4", 0);
                    nvs_set_u8(storage_handle, "P8B1E5", 0);
                    nvs_set_u8(storage_handle, "P8B1E6", 0);
                    nvs_set_u8(storage_handle, "P8B1E7", 0);
  
                    nvs_set_u8(storage_handle, "P8B2E0", 0);
                    nvs_set_u8(storage_handle, "P8B2E1", 0);
                    nvs_set_u8(storage_handle, "P8B2E2", 0);
                    nvs_set_u8(storage_handle, "P8B2E3", 0);
                    nvs_set_u8(storage_handle, "P8B2E4", 0);
                    nvs_set_u8(storage_handle, "P8B2E5", 0);
                    nvs_set_u8(storage_handle, "P8B2E6", 0);
                    nvs_set_u8(storage_handle, "P8B2E7", 0);
  
                    nvs_set_u8(storage_handle, "P8B3E0", 0);
                    nvs_set_u8(storage_handle, "P8B3E1", 0);
                    nvs_set_u8(storage_handle, "P8B3E2", 0);
                    nvs_set_u8(storage_handle, "P8B3E3", 0);
                    nvs_set_u8(storage_handle, "P8B3E4", 0);
                    nvs_set_u8(storage_handle, "P8B3E5", 0);
                    nvs_set_u8(storage_handle, "P8B3E6", 0);
                    nvs_set_u8(storage_handle, "P8B3E7", 0);
  
                    nvs_commit(storage_handle);
                    nvs_close(storage_handle);
                }

            }
            Q6buff = 0;
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
