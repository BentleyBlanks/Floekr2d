#ifndef F2_TIMER_H
#define F2_TIMER_H

// --!可更换式
// 可用于快速实现平台搭建工作
#include <Floekr2d/replaceable/timer/tkTimer.h>
#include <Floekr2d/protected/f2Settings.h>

namespace Floekr2d
{
    class f2Timer
    {
    public:
        f2Timer();

        void init();

        float32 getElapsedTime();

    private:
        TattyKit::device::tkTimer *timer;
    };
}

#endif