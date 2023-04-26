#include <DFRobot_MCP2515.h>
#include "constants.h"
#include "driveManager.h"
#include "readyToDriveSound.h"
#include "safetyCheck.h"

//Class variables
DFRobot_MCP2515 can(csPIN);
DriveManager driveManager(DRIVE_MODE_PIN, REVERSE_MODE_PIN, THROTTLE_PIN_A, THROTTLE_PIN_B, THROTTLE_MIN_A, THROTTLE_MIN_B, THROTTLE_MAX_A, THROTTLE_MAX_B, MAX_TORQUE, FILTER_FREQ, MAW_SIZE, NEW_DATA_WEIGHT, FILTER_CHOICE);
ReadyToDriveSound r2DSound(R2D_BEEP_INTERVAL, R2D_PIN);
SafetyCheck safetyCheck(IMPLAUSIBLE_TIME);

void setup(){
    //Set-up serial
    Serial.begin(SERIAL_BAUDRATE);

    //Set-up drive pins and ready-to-drive pin
    driveManager.initializePinMode();
    r2DSound.initializePinMode();
    
    //Set-up CAN
    while(can.begin(CAN_BAUDRATE) == CAN_FAILINIT){ 
        Serial.println("DFROBOT's CAN BUS Shield init fail");
        Serial.println("Please Init CAN BUS Shield again");
        delay(1000); //Retry connection every one second
    }
    Serial.println("DFROBOT's CAN BUS Shield init ok!\n");
}

void loop(){
    unsigned long latencyCheck = millis(); //Check the latency of each iteration of the loop
  
    driveManager.readDriveInput();
    driveManager.mapThrottle();

    //driveManager.printData();

    safetyCheck.checkImplausibility(driveManager.getThrottleSensorValues(), THROTTLE_MIN_A, THROTTLE_MAX_A, THROTTLE_MIN_B, THROTTLE_MAX_B, driveManager.getThrottle(), MAX_TORQUE);
    if(safetyCheck.shouldStopEngine()){
        //Send the info to the fucking dashboard
        //Logic to be determined...
        Serial.println("Implausible state");
        driveManager.sendStopEnginePacket(&can);
        delay(100);
        return; //Halt the function here
    }

    driveManager.processDriveInput(&r2DSound);
    //R2D Sound


    if(driveManager.getDriveMode() != DriveManager::DRIVE_MODE_NEUTRAL) r2DSound.checkR2D();

    driveManager.sendPacketToMotorController(&can, OUTPUT_MODE);

    //Check Latency
    //Serial.print("Latency step 1: ");
    Serial.println(millis() - latencyCheck);
}