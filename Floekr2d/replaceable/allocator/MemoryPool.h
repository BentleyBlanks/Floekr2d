
#ifndef __MEMORYPOOL_H__
#define __MEMORYPOOL_H__
#pragma once

#include "IMemory.h"
#include "Assertion.h"
/*
struct MemoryPoolNode
{
	void* next;
};
*/

//TODO:�ڴ���λ�õĵ���ʹ�ó�����ô��Ť
                    
#define NEXT_NODE(p) *(void**)((uint)p+single_size)

#define NEW_NODE(single_size) (void*)(((uint)allocate_aligned(sizeof(void*)+single_size))+4)

const int g_pool_expand_node_number = 32;
//TODO:singleton
class RBPoolAllctor
{
public:
	RBPoolAllctor();
	~RBPoolAllctor();
	
	INI_RESULT init();
	void new_pool(void** header,size_t single_size);
	void * alloc(void** header,size_t single_size);
	void free(void** header,void *p);
	//TODO:��Ҫ��ɾ��֮���ֹ�û��ٴ�ʹ��pָ��.
	void free(void** header,void *p,size_t single_size);
	INI_RESULT shutdown();

	//default:4 bytes
	void set_alignment(uint alignment);


	//debug
	void print_list(void *header,size_t single_size);

private:
	void expand_free_list(int single_size,void** header,int n = g_pool_expand_node_number);
	//return aligned_address located on '\'(/XXX4\XXXXXXXXXX.../XXXX)
	void *allocate_aligned(uint size);
	//free aligned_address located on '\'(/XXX4\XXXXXXXXXX.../XXXX)
	void free_aligned(void *p);
	bool _bactive;
	uint _alignment;
};

#endif