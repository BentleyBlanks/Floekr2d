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

			// �Զ���¼��ʼ����ʱ��
			void init();
			// ���ر��μ�ʱ���ϴ�֮���ʱ����
            float tick();
			// �Ӽ�ʱ����ʼ�������þ�����ʱ��
            float getElapsedTime();
		private:
			// --!pimpl���÷�
			class tkCounter;
			tkCounter* count;
		};
	}
}

#endif