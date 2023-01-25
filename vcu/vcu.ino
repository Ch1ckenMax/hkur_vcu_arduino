#include <DFRobot_MCP2515.h>
#include "constants.h"
#include "driveManager.h"

//Pins

//CAN
const uint8_t csPIN = 10;

//Throttle
const uint8_t DRIVE_MODE_PIN = 4;
const uint8_t REVERSE_MODE_PIN = 5;
const uint8_t THROTTLE_PIN_A = A0;
const uint8_t THROTTLE_PIN_B = A1;

const int THROTTLE_MIN = 90;
const int THROTTLE_MAX = 700;
const int MAX_TORQUE = 50;

//Class variables
DFRobot_MCP2515 can(csPIN);
DriveManager driveManager(DRIVE_MODE_PIN, REVERSE_MODE_PIN, THROTTLE_PIN_A, THROTTLE_PIN_B, THROTTLE_MIN, THROTTLE_MAX, MAX_TORQUE);

void setup(){
    //Set-up serial
    Serial.begin(SERIAL_BAUDRATE);

    //Set-up drive manager
    driveManager.initializePinMode();
    
    //Set-up CAN
    while(can.begin(CAN_BAUDRATE)){ 
        Serial.println("DFROBOT's CAN BUS Shield init fail");
        Serial.println("Please Init CAN BUS Shield again");
        delay(1000); //Retry connection every one second
    }
    Serial.println("DFROBOT's CAN BUS Shield init ok!\n");

}

void loop(){
    driveManager.readDriveInput();
    
    //Do something to the value?
    driveManager.sendPacketToMotorController(can);
}