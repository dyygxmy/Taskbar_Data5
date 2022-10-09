#ifndef MYSLEEP_H
#define MYSLEEP_H

#include "lib_taskbar_global.h"

class mySleep
{
public:
    mySleep();
    void myMsleep(unsigned int msec);
    void myMsleepEvent(unsigned int);
};

#endif // MYSLEEP_H
