#include <Arduino.h>
#include "filter.h"

MovingAVGFilter::MovingAVGFilter(uint8_t windowSize, int freq){
    this->windowSize = windowSize;
    window = new int[windowSize];
    delayHelper.setDelayDuration(1000 / freq);
}

MovingAVGFilter::~MovingAVGFilter(){
    if(window != NULL)
        delete[] window;
}

void MovingAVGFilter::resetFilter(int value){
    for(int i = 0; i < windowSize; i++){
        window[i] = value;
    }
    delayHelper.startTimer();
}

void MovingAVGFilter::feedData(int value){
    if(delayHelper.checkTimer()){ //Time to feed the data into the window
        windowIndex++;
        if(windowIndex >= windowSize) //wrap around the array
            windowIndex = 0;
        window[windowIndex] = value;
        delayHelper.startTimer(); //restart timer
    }
}

int MovingAVGFilter::getResult(){
    //Get the sum of the results
    int sum = 0;
    for(int i = 0; i < windowSize; i++){
        sum += window[i];

        //DEBUG
        //Serial.print(window[i]);
        //Serial.print(" ");
    }

    return sum / windowSize;
}

void doNothingFilter::resetFilter(int value){
    this->value = value;
}

void doNothingFilter::feedData(int value){
    this->value = value;
}

int doNothingFilter::getResult(){
    return this->value;
}

ExponentialFilter::ExponentialFilter(float newDataWeight){
    this->newDataWeight = newDataWeight;
}

void ExponentialFilter::resetFilter(int value){
    this->result = value;
}

void ExponentialFilter::feedData(int value){
    float oldDataWeight = (1 - this->newDataWeight);
    this->result = (oldDataWeight * this->result) + (this->newDataWeight * value);
 }
int ExponentialFilter::getResult(){
    return this->result;
}



