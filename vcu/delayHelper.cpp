#include <Arduino.h>
#include "delayHelper.h"

DelayHelper::DelayHelper(unsigned int delayDuration){
    this->delayDuration = delayDuration;
}

void DelayHelper::setDelayDuration(unsigned int delayDuration){
    this->delayDuration = delayDuration;
}

void DelayHelper::startTimer(){
    recordedTime = millis();
}

bool DelayHelper::checkTimer(){
    if(millis() - recordedTime >= delayDuration){
      return true;
    }
    else{
      return false;
    }
}