#ifndef CONSTANTS
#define CONSTANTS
#include <DFRobot_MCP2515.h>

//CAN
const int SERIAL_BAUDRATE = 9600;
const int CAN_BAUDRATE = CAN_250KBPS;
const uint8_t csPIN = 10;

//Throttle
const uint8_t DRIVE_MODE_PIN = 4;
const uint8_t REVERSE_MODE_PIN = 5;
const uint8_t THROTTLE_PIN_A = A0;
const uint8_t THROTTLE_PIN_B = A1;

const int THROTTLE_MIN_A = 550;
const int THROTTLE_MIN_B = 140;
const int THROTTLE_MAX_A = 780;
const int THROTTLE_MAX_B = 535;
const int MAX_TORQUE = 500; //Max torque value to be sent to the motor controller

//Ready to drive
const unsigned int R2D_BEEP_INTERVAL = 2000;
const uint8_t R2D_PIN = 8;

//Safety Check
const int IMPLAUSIBLE_TIME = 100; //ms

#endif