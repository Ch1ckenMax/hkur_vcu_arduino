#include "safetyCheck.h"
#include "delayHelper.h"
#include <Arduino.h>

bool SafetyCheck::SCSFailure(unsigned int* sensorValues, long throttle, int maxTorque, int throttleMin, int throttleMax){
    //Check throttle input range
    //Reference: FSUK 2023 T11.9.2(c)
    throttleMax += 100; //A little bit of buffer to make the constraints not that strict
    throttleMin -= 100;
    if (sensorValues[0] > throttleMax || sensorValues[1] > throttleMax || sensorValues[0] < throttleMin || sensorValues[1] < throttleMin){
        Serial.println("Invalid sensor values: out of range");
        return true;
    }

    //Check data corruption
    //if there is very big sudden change in throttle input that is impossible mechanically, it would be true
    //this could only happen when u connect to APPS1 and APPS2 from GND to 5V
    if(abs(throttle - previousThrottleValue) >= maxTorque){
        Serial.println("Data corruption: big sudden change in throttle input");
        return true;
    }

    //Check loss or delay of message
    //Probably need to configure the motor controller to send message to acknowledge the receive of message?
    //Or probably has to be done in the motor controller

    return false;
}

bool SafetyCheck::implausible(unsigned int* sensorValues, int throttleMin, int throttleMax, long throttle, int maxTorque){
    //Reference: FSUK2023 T11.8.9
    bool hasThrottleDeviaion = (abs(sensorValues[0] - sensorValues[1]) / (float) throttleMin - throttleMax) >= 0.1 ;
    return hasThrottleDeviaion || SCSFailure(sensorValues, throttle, maxTorque, throttleMin, throttleMax);
}

SafetyCheck::SafetyCheck(int implausibleTime){
    this->implausibleTime = implausibleTime;
    this->delayHelper = new DelayHelper(this->implausibleTime);
}

SafetyCheck::~SafetyCheck(){
    //Free memory automatically
    if(delayHelper != NULL){
        delete delayHelper;
    }
}

bool SafetyCheck::shouldStopEngine(){
    return this->engineStop;
}

void SafetyCheck::checkImplausibility(unsigned int* sensorValues, int throttleMin, int throttleMax, long throttle, int maxTorque){
    if(!implausible(sensorValues, throttleMin, throttleMax, throttle, maxTorque)){
        implausibleInProgress = false;
    } //Implausible. Check if it is in the progress of implausible
    else if(implausibleInProgress){
        //If the input is considered to be implausible for more than the time specified in implausibleTime, shall stop the engine.
        if(delayHelper->checkTimer()){
            engineStop = true;
        }
    }
    else{ //Starting to be implausible, start the timer in the delay helper
        implausibleInProgress = true;
        delayHelper->startTimer();
    }
}
