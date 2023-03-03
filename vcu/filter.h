#ifndef MAF 
#define MAF
#include <Arduino.h>
#include "Filter.h"
#include "delayHelper.h"

//Abstract class for filter
//To smoothen out the throttle input
class Filter{
    protected:
        DelayHelper delayHelper;

    public:
        //Set all the elements in the window to the value in the argument
        virtual void resetFilter(int value) = 0;

        //Feed the current data to the window
        virtual void feedData(int value) = 0;

        //Get the result from the filter
        virtual int getResult() = 0;

};

//Concrete class for moving average filter. Extends Filter class.
class MovingAVGFilter: public Filter{
    private:
        int* window;
        uint8_t windowIndex;
        uint8_t windowSize;
        int freq;

    public:
        MovingAVGFilter(uint8_t windowSize, int freq);
        ~MovingAVGFilter();

        //Set the values of all the elements in the window to the value in the argument
        virtual void resetFilter(int value);

        //Put the most recent data into the window
        virtual void feedData(int value);

        //Compute the average of the elements in the window
        virtual int getResult();

};

#endif