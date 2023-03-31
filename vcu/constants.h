#ifndef CONSTANTS
#define CONSTANTS
#include <DFRobot_MCP2515.h>
#include "driveManager.h"

//CAN
const int SERIAL_BAUDRATE = 9600;
const int CAN_BAUDRATE = CAN_250KBPS;
const uint8_t csPIN = 10;

//Throttle
const uint8_t DRIVE_MODE_PIN = 4;
const uint8_t REVERSE_MODE_PIN = 5;
const uint8_t THROTTLE_PIN_A = A0;
const uint8_t THROTTLE_PIN_B = A1;

const int THROTTLE_MIN_A = 470;
const int THROTTLE_MIN_B = 540;
const int THROTTLE_MAX_A = 850;
const int THROTTLE_MAX_B = 920;
const int MAX_TORQUE = 1660; //Max torque value to be sent to the motor controller

//Ready to drive
const unsigned int R2D_BEEP_INTERVAL = 2000;
const uint8_t R2D_PIN = 8;

//Safety Check
const int IMPLAUSIBLE_TIME = 100; //ms

//Filter
const char FILTER_CHOICE = DriveManager::FILTER_MOVING_AVG; //DriveManager::FILTER_DISABLED DriveManager::FILTER_MOVING_AVG DriverManager::FILTER_EXPONENTIAL

//For moving average filter
//Generally gives a rather stable output
const int FILTER_FREQ = 1000;
const int MAW_SIZE = 4;

//for exponential filter
//Generally gives a less stable output, but there is only a little bit of latency
const float NEW_DATA_WEIGHT = 0.85;

#endif