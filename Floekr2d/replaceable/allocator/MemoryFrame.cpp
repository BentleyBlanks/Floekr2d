#include "Trace.h"
#include "MemoryFrame.h"

/*���ĺ�������������ȫ���*/
INI_RESULT RBFrameAlloctor::init(size_t tsize)
{
	RBFN(RBFrameAlloctor::init);

#if _DEBUG
	printf("RBFrameAlloctor is starting up...\n");
#endif
	_byte_alignment = BYTEALIGN;
	tsize = ALIGNUP(tsize,_byte_alignment);
	_all_memory = tsize + _byte_alignment;
	//����_byte_alignment��ֹ�ڶ����ڴ�ʱ��ջ��Խ��
	_pmemory = (u8*)malloc(_all_memory);
	if(!_pmemory)
	{
		//log???
#ifdef _DEBUG
		printf("��ʼ���ڴ����ʧ�ܣ�\n");
#endif
		return SYSTEM_MEMORY_SHORT;
	}

	_pbase = (u8*)ALIGNUP(_pmemory,_byte_alignment);
	_pcap = (u8*)ALIGNUP(_pmemory+tsize,_byte_alignment);

	_pframe_base = _pbase;
	_pframe_cap = _pcap;

	return SYSTEM_MEMORY_SUCCESS;

}

INI_RESULT RBFrameAlloctor::shutdown()
{
	RBFN(RBFrameAlloctor::shutdown);
#if _DEBUG
	printf("RBFrameAlloctor is shutting down...\n");
#endif
	free(_pmemory);
	_pmemory = NULL;

	return INI_SUCCESS;
}

/*0 low 1 high*/
void* RBFrameAlloctor::alloc(size_t tsize,MPOS tpos)
{
	RBFN(RBFrameAlloctor::alloc);

	u8* _pret;

	tsize = ALIGNUP(tsize,_byte_alignment);

	if(_pframe_base+tsize>_pframe_cap)
	{
		//log???
#ifdef _DEBUG
		printf("��̬�ڴ���䲻�㣡�����˳���\n");
		getchar();
#endif
		exit(0);
		//��������
		return NULL;
	}

	if(!tpos)
	{
		_pret = _pframe_base;
		_pframe_base += tsize;
	}
	else
	{
		_pframe_cap -= tsize;
		_pret = _pframe_cap;
	}

	return (void*)_pret;

}

size_t RBFrameAlloctor::get_all_memory()
{
	RBFN(RBFrameAlloctor::get_all_memory);
	return _all_memory;
}

//��ȡ��ǰջ��Ϊ֡
void RBFrameAlloctor::getframe(MemoryFrame& mf,MPOS tpos)
{
	RBFN(RBFrameAlloctor::getframe);
	mf.frame_pos = tpos;
	if (!tpos)
	{
		mf.memory_ptr = _pframe_base;
	}
	else
	{
		mf.memory_ptr = _pframe_cap;
	}
}

//�ͷ�֡����������ڴ�
void RBFrameAlloctor::release(MemoryFrame& mf)
{
	RBFN(RBFrameAlloctor::release);
	if (mf.memory_ptr)
	{
		if(!mf.frame_pos)
		{
			_pframe_base = mf.memory_ptr;
		}
		else
		{
			_pframe_cap = mf.memory_ptr;
		}
		mf.memory_ptr = NULL;
	}
}

size_t RBFrameAlloctor::get_allocated_memory()
{
	return _pframe_base - _pbase + _pcap - _pframe_cap;
}

size_t RBFrameAlloctor::get_remain_memory()
{
	return _pframe_cap - _pframe_base;
}