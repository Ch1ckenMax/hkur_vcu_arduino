#include <Arduino.h>
#include "readyToDriveSound.h"

ReadyToDriveSound::ReadyToDriveSound(unsigned int beepInterval, uint8_t pinNumber){
    this->beepInterval = beepInterval;
    this->beepPin = pinNumber;
    this->delayHelper = new DelayHelper(this->beepInterval);
}

ReadyToDriveSound::~ReadyToDriveSound(){
    if(delayHelper != NULL){
      free(delayHelper);
    }
}

uint8_t ReadyToDriveSound::getBeepState(){
    return beepState;
}

void ReadyToDriveSound::setBeepState(uint8_t state){
    this->beepState = state;
}

void ReadyToDriveSound::initializePinMode(){
    pinMode(beepPin, OUTPUT);
}

void ReadyToDriveSound::turnOffIfBeeping(){
    if(ReadyToDriveSound::BEEP_BEEPING == beepState){
        Serial.println("Ready to drive sound OFF");
        digitalWrite(beepPin, LOW);
    }
}

void ReadyToDriveSound::checkR2D(){
    if(beepState == ReadyToDriveSound::BEEP_NOT_STARTED){ //Start beeping if it is not beeping
        Serial.println("Ready to Drive Sound ON");
        digitalWrite(beepPin, HIGH);
        beepState = ReadyToDriveSound::BEEP_BEEPING;
        delayHelper->startTimer(); //Start the timer by recording the time
    }
    else if(beepState == ReadyToDriveSound::BEEP_BEEPING){
        //Check if it has been beeped for more than the beepInterval
        if(delayHelper->checkTimer()){
            Serial.println("Ready to Drive Sound OFF");
            digitalWrite(beepPin, LOW);
            beepState = ReadyToDriveSound::BEEP_FINISHED;
        }
    }
    //Else, do nothing.
}