#include "safetyCheck.h"
#include "delayHelper.h"
#include <Arduino.h>

bool SafetyCheck::SCSFailure(unsigned int* sensorValues, long throttle, int maxTorque, int throttleMinA, int throttleMaxA, int throttleMinB, int throttleMaxB){
    //Check throttle input range
    //Reference: FSUK 2023 T11.9.2(c)
    if (sensorValues[0] > throttleMaxA + 15 || sensorValues[1] > throttleMaxB + 15 || sensorValues[0] < throttleMinA - 15 || sensorValues[1] < throttleMinB - 15){
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

bool SafetyCheck::implausible(unsigned int* sensorValues, int throttleMinA, int throttleMaxA, int throttleMinB, int throttleMaxB, long throttle, int maxTorque){
    //Reference: FSUK2023 T11.8.9
    float deviation = abs(map(sensorValues[0], throttleMinA, throttleMaxA, 0, maxTorque) - map(sensorValues[1], throttleMinB, throttleMaxB, 0, maxTorque))/maxTorque;
    bool hasThrottleDeviation = deviation >= 0.1;
    if(hasThrottleDeviation) {
      Serial.println("Throttle deviation larger than 10%");
    }
    return hasThrottleDeviation || SCSFailure(sensorValues, throttle, maxTorque, throttleMinA, throttleMaxA, throttleMinB, throttleMaxB);
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

void SafetyCheck::checkImplausibility(unsigned int* sensorValues, int throttleMinA, int throttleMaxA, int throttleMinB, int throttleMaxB, long throttle, int maxTorque){
    if(!implausible(sensorValues, throttleMinA, throttleMaxA, throttleMinB, throttleMaxB, throttle, maxTorque)){
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
