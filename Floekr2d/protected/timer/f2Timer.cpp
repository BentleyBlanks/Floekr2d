#include <Floekr2d/protected/timer/f2Timer.h>

namespace Floekr2d
{
    f2Timer::f2Timer()
    {
        // ³õÊ¼»¯TattyKitÊ±ÖÓ
        timer = new TattyKit::device::tkTimer();
    }

    void f2Timer::init()
    {
        timer->init();
    }

    float32 f2Timer::getElapsedTime()
    {
        return timer->getElapsedTime();
    }
}