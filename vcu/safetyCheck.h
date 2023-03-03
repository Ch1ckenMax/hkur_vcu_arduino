#ifndef SAFETY_CHECK
#define SAFETY_CHECK
#include "delayHelper.h"
#include <DFRobot_MCP2515.h>

//Class for handling safety checks in accordance to the rules
class SafetyCheck{
    private:
        int implausibleTime;

        DelayHelper delayHelper;
        bool engineStop = false;
        bool implausibleInProgress = false;
        long previousThrottleValue = 0;
        int canState;

        //Check if there is a SCSFailure according to the rulebook
        //Reference: T11.9.2
        //sensorValues must be a pointer to an array of int of size 2
        bool SCSFailure(unsigned int* sensorValues, long throttle, int maxTorque, int throttleMinA, int throttleMaxA, int throttleMinB, int throttleMaxB);

        //Check if throttle input is implausible according to rulebook
        //Reference: FSUK 2023 T11.8.8 and T11.8.9
        bool implausible(unsigned int* sensorValues, int throttleMinA, int throttleMaxA, int throttleMinB, int throttleMaxB, long throttle, int maxTorque);
    public:
        SafetyCheck(int implausibleTime);

        bool shouldStopEngine();

        //Reference: FSUK2023 T11.8.8
        //sensorValues must be a pointer to an array of int of size 2
        void checkImplausibility(unsigned int* sensorValues, int throttleMinA, int throttleMaxA, int throttleMinB, int throttleMaxB, long throttle, int maxTorque);
};

#endif