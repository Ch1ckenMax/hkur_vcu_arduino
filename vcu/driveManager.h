#ifndef DRIVE_MANAGER
#define DRIVE_MANAGER
#include <Arduino.h>
#include <DFRobot_MCP2515.h>
#include "driveManager.h"
#include "readyToDriveSound.h"
#include "filter.h"

class DriveManager{
    private:
        //Pin numbers
        uint8_t driveModePin; 
        uint8_t reverseModePin;
        uint8_t throttlePinA;
        uint8_t throttlePinB;

        int throttleMinA;
        int throttleMinB;
        int throttleMaxA;
        int throttleMaxB;
        int maxTorque;

        unsigned int throttleSensorValues[2]; //Temporary variables to store the results of the two sensors of the throttle

        //Container for the data packet to be sent to the motor controller
        uint8_t motorControllerPacket[8];

        //Parameters for the data packets
        uint8_t driveMode = DriveManager::DRIVE_MODE_NEUTRAL; //Drive mode of the car defined by the three constants: DRIVE_MODE_REVERSE, DRIVE_MODE_NEUTRAL, DRIVE_MODE_DRIVE
        bool inverterEnabled = false;
        bool driveForward = true;
        long throttle = 0;
        long throttle_A = 0;
        long throttle_B = 0;

        //set the values for the data packets to be sent to motor controller
        void setDataPacket(unsigned int torque, int angularVelocity, bool directionForward, bool inverter, bool inverterDischarge, bool speedMode, int torqueLimit);

        //filter
        Filter* filterA;
        Filter* filterB;
    
    public:
        //Drive Mode Constants (using char to save memory space)
        const static char DRIVE_MODE_REVERSE = 0;
        const static char DRIVE_MODE_NEUTRAL = 1;
        const static char DRIVE_MODE_DRIVE = 2;
        
        //Constructor for drive manager that uses moving average filter. Intializes variables. To be called in setup()
        DriveManager(uint8_t driveModePinNumber, uint8_t reverseModePinNumber, uint8_t throttleAPinNumber, uint8_t throttleBPinNumber, int throttleMinA, int throttleMinB, int throttleMaxA, int throttleMaxB, int maxTorque, int filterFreq, uint8_t windowSize);

        //Free memory
        ~DriveManager();
       
        //Set pin modes for driveModePin and reverseModePin
        void initializePinMode();

        //Read the throttle sensor values and make adjustments to them w.r.t. throttleSensorDiff. To be called in loop() and before processDriveInput(ReadyToDriveSound*)
        void readDriveInput();

        //Map the sensors to the appropriate range of throttle using Arduino's built-in map()
        void mapThrottle();

        //Do appropriate actions to the throttle value. To be called in loop()
        void processDriveInput(ReadyToDriveSound* r2DSound);

        //Set the data packets to appropriate values, and send signal via CAN BUS to the motor controller
        //this send function actually returns "CAN_OK" if send success,
        //returns "CAN_SENDMSGTIMEOUT" if timeout
        //or returns "CAN_GETTXBFTIMEOUT" if failed to get the next free buffer
        uint8_t sendPacketToMotorController(DFRobot_MCP2515* can);

        uint8_t sendStopEnginePacket(DFRobot_MCP2515* can);

        uint8_t getDriveMode();

        unsigned int* getThrottleSensorValues();

        long getThrottle();

        void printData();
};

#endif