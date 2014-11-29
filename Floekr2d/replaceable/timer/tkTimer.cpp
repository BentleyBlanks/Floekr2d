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
			// ����������
			LARGE_INTEGER frequency;
			// �տ�ʼ(����һ��ʼ/ĳ��ѭ���е�һ��ʼ)�ļ�������
			LARGE_INTEGER start;
			// ��һ֡��������
            LARGE_INTEGER last;

			// --!��ʹ��const�޶� ���򲻷���QueryPerformanceCounter�����涨
            float getDeltaTime(LARGE_INTEGER& start, LARGE_INTEGER& end)
			{
				// ��ѯ��ǰ�ļ���������
				QueryPerformanceCounter(&end);
				// ��ǰ֡���ʼ��֡ʱ����
                return (float) (end.LowPart - start.LowPart) / frequency.LowPart;
			}
#else
#if defined TK_PLATFORM_OSX
			// �˴��������
#else 
#if defined TK_PLATFORM_LINUX
			// �˴��������

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
			// ��ʼ�����ܺ���ʼ������
			QueryPerformanceFrequency(&count->frequency);
			QueryPerformanceCounter(&count->start);
#else
#if defined TK_PLATFORM_OSX
			// �˴��������
#else 
#if defined TK_PLATFORM_LINUX
			// �˴��������
#endif
#endif
#endif
		}


        float tkTimer::tick()
		{
#ifdef TK_PLATFORM_WINDOWS
			LARGE_INTEGER current;

			// �ӵ�ǰ֡����һ֡��������֮���ʱ���
			float deltaTime = count->getDeltaTime(count->last, current);

			count->last = current;

			return deltaTime;

#else
#if defined TK_PLATFORM_OSX
			// �˴��������
#else 
#if defined TK_PLATFORM_LINUX
			// �˴��������
#endif
#endif
#endif
		}

        float tkTimer::getElapsedTime()
		{
#ifdef TK_PLATFORM_WINDOWS
			LARGE_INTEGER now;
			
			// �ӵ�ǰ֡����ʱ������ʼ������ʼֵ֮���ʱ���
			return count->getDeltaTime(count->start, now);
#else
#if defined TK_PLATFORM_OSX
			// �˴��������
#else 
#if defined TK_PLATFORM_LINUX
			// �˴��������
#endif
#endif
#endif
		}
	}
}