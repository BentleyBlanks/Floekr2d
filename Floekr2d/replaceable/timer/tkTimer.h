#ifndef TATTYKIT_TIMER_H
#define TATTYKIT_TIMER_H

namespace TattyKit
{
	namespace device
	{
		class tkTimer
		{
		public:
			tkTimer();
			~tkTimer();

			// 自动记录初始化的时间
			void init();
			// 返回本次计时与上次之间的时间间隔
            float tick();
			// 从计时器初始化到调用经过的时间
            float getElapsedTime();
		private:
			// --!pimpl惯用法
			class tkCounter;
			tkCounter* count;
		};
	}
}

#endif