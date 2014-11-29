#include "Trace.h"
#include "MemoryManager.h"

 const size_t frame_memory = 1024000000;

RBMemoryManager* RBMemoryManager::_memorymanager = 0;

RBMemoryManager* RBMemoryManager::Instance()
{
	RBFN(RBMemoryManager::Instance);
	if(!_memorymanager)
	{
		_memorymanager = new RBMemoryManager;
		_memorymanager->_frame_allocator = new RBFrameAlloctor;
	}


	return _memorymanager;
}

RBMemoryManager::RBMemoryManager()
{
	RBFN(RBMemoryManager::RBMemoryManager);
	_frame_allocator = NULL;
	//仅私有初始化，慎用!
}

RBFrameAlloctor* RBMemoryManager::get_frameallocator()
{
	RBFN(RBMemoryManager::get_frameallocator);
	if (_frame_allocator)
	{
		return _frame_allocator;
	}
	return NULL;
}



INI_RESULT RBMemoryManager::StartUp()
{
	RBFN(RBMemoryManager::StartUp);
	if(!_memorymanager->_frame_allocator->init(frame_memory)) 
		return INI_FAIL;
	return INI_SUCCESS;
}

INI_RESULT RBMemoryManager::ShutDown()
{
	RBFN(RBMemoryManager::ShutDown);
	if(!_memorymanager->_frame_allocator->shutdown()) 
		return INI_FAIL;
	return INI_SUCCESS;
}