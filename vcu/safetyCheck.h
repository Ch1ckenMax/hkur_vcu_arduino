#ifndef SAFETY_CHECK
#define SAFETY_CHECK
#include "delayHelper.h"

class SafetyCheck{
    private:
      DelayHelper* delayHelper;
      bool engineStop;
      bool implausibleInProgress;
};

#endif