#ifndef SAFETY_CHECK
#define SAFETY_CHECK
#include "delayHelper.h"

//Class for handling safety checks in accordance to the rules
class SafetyCheck{
    private:
        int implausibleTime;

        DelayHelper* delayHelper;
        bool engineStop = false;
        bool implausibleInProgress = false;
        long previousThrottleValue = 0;

        //Check if there is a SCSFailure according to the rulebook
        //sensorValues must be a pointer to an array of int of size 2
        bool SCSFailure(unsigned int* sensorValues, long throttle, int maxTorque);

        //Check if throttle input is implausible according to rulebook T11.8.8 and T11.8.9
        bool implausible(unsigned int* sensorValues, int throttleMin, int throttleMax, long throttle, int maxTorque);
    public:
        SafetyCheck(int implausibleTime);
        ~SafetyCheck();

        bool shouldStopEngine();

        //sensorValues must be a pointer to an array of int of size 2
        void checkImplausibility(unsigned int* sensorValues, int throttleMin, int throttleMax, long throttle, int maxTorque);
};

#endif