#ifndef R2D_SOUND
#define R2D_SOUND
#include <Arduino.h>

class ReadyToDriveSound{
    private:
      uint8_t beepPin;
      int beepInterval;

    public:
      //Constants
      const static uint8_t BEEP_NOT_STARTED = 0;
      const static uint8_t BEEP_BEEPING = 1;
      const static uint8_t BEEP_FINISHED = 2;

      
};

#endif