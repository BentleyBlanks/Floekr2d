#ifndef __MEMORYMANAGER_H__
#define __MEMORYMANAGER_H__

#include "RBBasedata.h"
#include "MemoryFrame.h"
#include "MemoryPool.h"

extern const size_t frame_memory;

class RBMemoryManager
{
public:
	static RBMemoryManager* Instance();

	INI_RESULT StartUp();
	INI_RESULT ShutDown();

	RBFrameAlloctor* get_frameallocator();
protected:
	RBMemoryManager();
private:
	RBFrameAlloctor *_frame_allocator;

	static RBMemoryManager* _memorymanager;
};

#endif