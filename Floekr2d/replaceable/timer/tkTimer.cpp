#include "tkTimer.h"
#include "tkConstants.h"

namespace TattyKit
{
	namespace device
	{
		class tkTimer::tkCounter
		{
		public:
            tkCounter(){}

#ifdef TK_PLATFORM_WINDOWS
			// 性能运算数
			LARGE_INTEGER frequency;
			// 刚开始(程序一开始/某个循环中的一开始)的计数次数
			LARGE_INTEGER start;
			// 上一帧计数次数
            LARGE_INTEGER last;

			// --!不使用const限定 是因不符合QueryPerformanceCounter参数规定
            float getDeltaTime(LARGE_INTEGER& start, LARGE_INTEGER& end)
			{
				// 查询当前的计数器次数
				QueryPerformanceCounter(&end);
				// 当前帧与初始化帧时间间隔
                return (float) (end.LowPart - start.LowPart) / frequency.LowPart;
			}
#else
#if defined TK_PLATFORM_OSX
			// 此处添加内容
#else 
#if defined TK_PLATFORM_LINUX
			// 此处添加内容

#endif
#endif
#endif
		};

		tkTimer::tkTimer():count(new tkTimer::tkCounter())
		{
			
		}

		tkTimer::~tkTimer()
		{
			delete count;
			count = NULL;
		}

		void tkTimer::init()
		{
#ifdef TK_PLATFORM_WINDOWS
			// 初始化性能和起始计数器
			QueryPerformanceFrequency(&count->frequency);
			QueryPerformanceCounter(&count->start);
#else
#if defined TK_PLATFORM_OSX
			// 此处添加内容
#else 
#if defined TK_PLATFORM_LINUX
			// 此处添加内容
#endif
#endif
#endif
		}


        float tkTimer::tick()
		{
#ifdef TK_PLATFORM_WINDOWS
			LARGE_INTEGER current;

			// 从当前帧到上一帧计数次数之间的时间差
			float deltaTime = count->getDeltaTime(count->last, current);

			count->last = current;

			return deltaTime;

#else
#if defined TK_PLATFORM_OSX
			// 此处添加内容
#else 
#if defined TK_PLATFORM_LINUX
			// 此处添加内容
#endif
#endif
#endif
		}

        float tkTimer::getElapsedTime()
		{
#ifdef TK_PLATFORM_WINDOWS
			LARGE_INTEGER now;
			
			// 从当前帧到计时器被初始化的起始值之间的时间差
			return count->getDeltaTime(count->start, now);
#else
#if defined TK_PLATFORM_OSX
			// 此处添加内容
#else 
#if defined TK_PLATFORM_LINUX
			// 此处添加内容
#endif
#endif
#endif
		}
	}
}