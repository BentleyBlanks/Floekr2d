#include<iostream>

#include <Floekr2d/Floekr2d.h>
#include <Floekr2d/protected/timer/f2Timer.h>

using namespace std;
using namespace Floekr2d;

void main()
{
    // f2Timer
    f2Timer *timer = new f2Timer();

    timer->init();
    for(int i = 1; i<50000; i++)
    {
        cout << timer->getElapsedTime() << endl;
    }

    getchar();
    return;
}