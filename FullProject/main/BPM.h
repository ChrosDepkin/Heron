#ifndef BPM_H
#define BPM_H

// Object of this class is passed between BPM timer task and timer callback
// This lets us update the BPM timer in the task rather than the callback (which is an interrupt)
// But also makes sure the timer keeps running as we update it by forcing it to wait for a timer tick before changing timer period
class BPMclass
{
    public:
    uint8_t BPMp; // Previous BPM value used to check if BPM has been changed
    bool BPMchanged = 0; // This will be set if a change in BPM is detected
    esp_timer_handle_t BPMtimer; // Timer handle
    esp_timer_create_args_t BPMargs; // Arguments for the timer - set in timer task
};

#endif