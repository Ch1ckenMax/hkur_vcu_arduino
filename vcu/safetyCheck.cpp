#include "safetyCheck.h"
#include "delayHelper.h"
#include <Arduino.h>

bool SafetyCheck::SCSFailure(unsigned int* sensorValues, long throttle, int maxTorque){
    //Check throttle input range
    if (sensorValues[0] > 1023 || sensorValues[1] > 1023 || sensorValues[0] < 0 || sensorValues[1] < 0){
        Serial.println("Invalid sensor values: out of range");
        return true;
    }

    //Check data corruption
    //if there is very big sudden change in thottle input, 
    //im talking about imposible type of big sudden change
    // you know u fucked up
    //this could only happen when u connect to APPS1 and APPS2 from GND to 5V
    if(abs(throttle - previousThrottleValue) >= maxTorque){
        Serial.println("Data corruption: big sudden change in throttle input");
        return true;
    }

    //Check loss or delay of message
    // if (send_state != "CAN_OK") {
    //   CAN_errorCount++;
    // }
    // //chechError(): Return: "CAN_CTRLERROR" means it sends the control error; Contrarily, return "CAN_OK".
    // if (CAN.checkError() != "CAN_OK") {
    //   CAN_errorCount++;
    // }

    return false;
}

bool SafetyCheck::implausible(unsigned int* sensorValues, int throttleMin, int throttleMax, long throttle, int maxTorque){
    bool hasThrottleDeviaion = (abs(sensorValues[0] - sensorValues[1]) / (float) throttleMin - throttleMax) >= 0.1 ;
    return hasThrottleDeviaion || SCSFailure(sensorValues, throttle, maxTorque);
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