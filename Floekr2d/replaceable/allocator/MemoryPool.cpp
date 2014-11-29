#include "./RBMathEx/Inc/RBMathUtilities.h"
#include "MemoryPool.h"

RBPoolAllctor::RBPoolAllctor()
{
	_bactive = false;
	_alignment = 4;
}

RBPoolAllctor::~RBPoolAllctor()
{

}

INI_RESULT RBPoolAllctor::init()
{
	_bactive = true;
	return true;
}

INI_RESULT RBPoolAllctor::shutdown()
{
	_bactive = false;
	return true;
}

void RBPoolAllctor::new_pool(void** header,size_t single_size)
{
	expand_free_list(single_size,header);
}

void* RBPoolAllctor::alloc(void** header,size_t single_size)
{
	if(NULL==NEXT_NODE(*header))
	{
		expand_free_list(single_size,&NEXT_NODE(*header));
#ifdef _DEBUG
		printf("NOTE:Memory pool expanded at %p\n",NEXT_NODE(*header));
#endif
	}
	void *r_header = *header;
	*header = NEXT_NODE(*header);
	return r_header;
}

void RBPoolAllctor::free(void** header,void *p,size_t single_size)
{
	//删掉之后要防止p被非法使用
	NEXT_NODE(p) = *header;
	*header = p;
	
}

void RBPoolAllctor::free(void** header,void *p)
{

}

//4/single_size/4
void RBPoolAllctor::expand_free_list(int single_size,void** header,int n /* = g_pool_expand_node_number = 32 */)
{
	CHECK(*header == NULL );

	/*     p    
	//XXX4/XXXXXXXXXXXXXXXX.../XXXX
	*/
	void* p = NEW_NODE(single_size);
	*header = (void*)p;
	//*(void**)&p[-4] = p;

	for(int i = 0;i<n-1;++i)
	{
		NEXT_NODE(p) = NEW_NODE(single_size);
		p = NEXT_NODE(p);
	}
	NEXT_NODE(p) = NULL;
}

void *RBPoolAllctor::allocate_aligned(uint tsize)
{
	CHECK(_alignment>1);
	CHECK(RBMath::is_pow_2(_alignment));

	size_t size = tsize + _alignment;
	uint origin_address = (uint)malloc(size);
	uint tmask = (_alignment - 1);
	uint misaligment = (origin_address & tmask);
	uint adjust = _alignment - misaligment;
	CHECK(adjust>0);

	uint aligned_address = origin_address + adjust;

	//save alignment adjustment 
	u8 *adjust_ptr = (u8*)aligned_address;
	CHECK(adjust<256);
	adjust_ptr[-1] = (u8)adjust;
	//save next address
	//XXX4/XXXXXXXXXXXXXXXX.../XXXX
	/*
	//return value is also aligned after 4 added.
	return (void*)(aligned_address + 4);
	*/
	return (void*)aligned_address;
}

//free aligned_address located on '\'(/XXX4\XXXXXXXXXX.../XXXX)
void RBPoolAllctor::free_aligned(void *p)
{
	uint aligned_address = (uint)p;
	u8 *adjust_ptr = (u8*)aligned_address;
	uint adjust = (uint)adjust_ptr[-1];

	uint origin_address = aligned_address - adjust;

	::free((void*)origin_address);

}


void RBPoolAllctor::set_alignment(uint val)
{
	CHECK(RBMath::is_pow_2(val)&&val!=1);
	_alignment = val;
}

void RBPoolAllctor::print_list(void *header,size_t single_size)
{
	//void* p = header;
	//while (NULL!=p)
	//{
	//	((ATEST*)p)->print();
	//	p = NEXT_NODE(p);
	//}
}