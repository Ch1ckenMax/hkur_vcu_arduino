#ifndef DELAY_HELPER
#define DELAY_HELPER

//Helper class to "stop" a task without blocking the program like using delay()
//Idea: 
//1. record the time using millis() function with startTimer() to "stop" the task
//2. check for every loop: how many milliseconds have passed since the time is recorded with checkTimer()
class DelayHelper{
    private:
      unsigned long recordedTime = 0;
      unsigned int delayDuration = 0;

    public:
      void setDelayDuration(unsigned int delayDuration);

      //Record the current time to the timer
      void startTimer();

      //Returns true if delayDuration or more has passed since recordedTime. Returns false otherwise
      bool checkTimer();
};

#endif