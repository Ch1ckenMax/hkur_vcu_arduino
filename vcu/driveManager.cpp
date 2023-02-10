#include "driveManager.h"
#include "readyToDriveSound.h"
#include <DFRobot_MCP2515.h>

void DriveManager::setDataPacket(unsigned int torque, int angularVelocity, bool directionForward, bool inverter, bool inverterDischarge, bool speedMode, int torqueLimit) {
    motorControllerPacket[0] = torque % 256;
    motorControllerPacket[1] = torque / 256;
    motorControllerPacket[2] = angularVelocity % 256;
    motorControllerPacket[3] = angularVelocity / 256;
    motorControllerPacket[4] = (directionForward ? 1 : 0);
    motorControllerPacket[5] = (inverter ? 1 : 0) + 2 * (inverterDischarge ? 1 : 0) + 4 * (speedMode ? 1 : 0);
    motorControllerPacket[6] = torqueLimit % 256;
    motorControllerPacket[7] = torqueLimit / 256;
}

DriveManager::DriveManager(uint8_t driveModePinNumber, uint8_t reverseModePinNumber, uint8_t throttleAPinNumber, uint8_t throttleBPinNumber){
    //Initialize the private variables
    for(int i = 0; i < 8; i++) this->motorControllerPacket[i] = 0;

    //Set the pins
    this->driveModePin = driveModePinNumber;
    this->reverseModePin = reverseModePinNumber;
    this->throttlePinA = throttleAPinNumber;
    this->throttlePinB = throttleBPinNumber;   
}

void DriveManager::initializePinMode(){
    pinMode(driveModePin, INPUT_PULLUP);
    pinMode(reverseModePin, INPUT_PULLUP);
}

void DriveManager::readDriveInput(int throttleSensorDiff){
    //Get the values
    throttleSensorValues[0] = analogRead(throttlePinA) - throttleSensorDiff; //Take account into the difference of the two sensors
    throttleSensorValues[1] = analogRead(throttlePinB);
}

void DriveManager::mapThrottle(int throttleMin, int throttleMax, int maxTorque){
    throttle = map((throttleSensorValues[0] + throttleSensorValues[1]) / 2, throttleMin, throttleMax, 0, maxTorque);
}

void DriveManager::processDriveInput(ReadyToDriveSound* r2DSound, int maxTorque){
    // Prevent overflow
    if (throttle > maxTorque) {
        throttle = maxTorque;
    }
    else if (throttle < 0) {
        throttle = 0;
    }

    //Set-up the drive mode
    if (!digitalRead(driveModePin)) {
        driveMode = DriveManager::DRIVE_MODE_DRIVE;
        inverterEnabled = throttle >= 2; //only turn on the inverter if there is throttle signal for accelerating
        driveForward = true;
    }
    else if (!digitalRead(reverseModePin)) {
        driveMode = DriveManager::DRIVE_MODE_REVERSE;
        inverterEnabled = throttle >= 2; //only turn on the inverter if there is throttle signal for accelerating
        driveForward = false;
    }
    else {
        driveMode = DriveManager::DRIVE_MODE_NEUTRAL;
        inverterEnabled = false;
        driveForward = true;

        //Reset the ready to drive sound
        r2DSound->turnOffIfBeeping();
        r2DSound->setBeepState(ReadyToDriveSound::BEEP_NOT_STARTED);
    }

}

uint8_t DriveManager::sendPacketToMotorController(DFRobot_MCP2515* can){
    setDataPacket(this->throttle, 0, this->driveForward, this->inverterEnabled, !(this->inverterEnabled), false, 0);
    return can->sendMsgBuf(0x0c0, 0, 8, motorControllerPacket); //send. 0x0c0 is defined by the docs of the motor controller
}

uint8_t DriveManager::sendStopEnginePacket(DFRobot_MCP2515* can){
    setDataPacket(0, 0, this->driveForward, false, true, false, 0);
    return can->sendMsgBuf(0x0c0, 0, 8, motorControllerPacket);
}

uint8_t DriveManager::getDriveMode(){
    return this->driveMode;
}

unsigned int* DriveManager::getThrottleSensorValues(){
    return this->throttleSensorValues;
}

long DriveManager::getThrottle(){
    return this->throttle;
}