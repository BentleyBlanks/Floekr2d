#pragma once
#ifndef __MEMORYFRAEM_H__
#define __MEMORYFRAME_H__


//#include "RBBasedata.h"
#include "IMemory.h"


struct MemoryFrame
{
	MPOS frame_pos;
	u8* memory_ptr;
};

class RBFrameAlloctor
{
public:
	INI_RESULT init(size_t tsize);

	void* alloc(size_t tsize,MPOS tpos);
	INI_RESULT shutdown();
	void getframe(MemoryFrame& mf,MPOS tpos);
	void release(MemoryFrame& mf);
	//++帧释放函数

	size_t get_all_memory();
	size_t get_allocated_memory();
	size_t get_remain_memory();
private:
	void* alloc(size_t tsize){return 0;}


	int _byte_alignment;
	u8* _pmemory;
	u8* _pbase;
	u8* _pcap;
	u8* _pframe_base;
	u8* _pframe_cap;
	size_t _all_memory;
};


//RBFrameAlloctor *GFrameAlloctor;

class RBSystemAlloctorSub
{

public:
	RBSystemAlloctorSub(){}
	//void *operator new(size_t tsize){int* p = new int;printf("你正在使用子类的新建！");return (void*)&p;}
};

#endif