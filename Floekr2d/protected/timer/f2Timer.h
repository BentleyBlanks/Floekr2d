#ifndef F2_TIMER_H
#define F2_TIMER_H

// --!�ɸ���ʽ
// �����ڿ���ʵ��ƽ̨�����
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