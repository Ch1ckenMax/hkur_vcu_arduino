#include "calibrationInterrupt.h"
#include "delayHelper.h"

void calibrationInterrupt(){
    //Waiting for calibration button to be pressed. send appropriate signals.
      //read button press
      //if there is button press
        //change state to 1
        //record timestamp
      //else
        //Tell the driver to press throttle all the way down, press ready then wait for 1 second.

     //Check if 5 seconds has already passed
      //if passed
        //change state to 2
      //else
        //record throttle input (what to record?)

      //Waiting for calibration button to be pressed. send appropriate signals.

      //Calibrating throttle Min threshold....

      //Calibrating finished. Waiting for calibration button to be pressed to leave the calibration state?
      //read button press
      //If there is button press
        //calibrationMode = 0, calibrationState =0. Go back to normal mode.
      //else
        //print message to tell drive to leave calibration mode
}
