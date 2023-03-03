#ifndef R2D_SOUND
#define R2D_SOUND
#include <Arduino.h>
#include "delayHelper.h"

//Reference: FSUK 2023 EV4.12
class ReadyToDriveSound{
    private:
      uint8_t beepPin;
      unsigned int beepInterval;
      uint8_t beepState = ReadyToDriveSound::BEEP_NOT_STARTED;
      DelayHelper delayHelper;

    public:
      //Constants
      const static uint8_t BEEP_NOT_STARTED = 0;
      const static uint8_t BEEP_BEEPING = 1;
      const static uint8_t BEEP_FINISHED = 2;

      //
      ReadyToDriveSound(unsigned int beepInterval, uint8_t pinNumber);

      uint8_t getBeepState();

      void setBeepState(uint8_t state);

      void initializePinMode();

      void turnOffIfBeeping();

      //Logic for the on and off of the ready to drive sound
      void checkR2D();





      
};

#endif