#include <Floekr2d/protected/allocator/f2PoolAllocator.h>

namespace Floekr2d
{
    f2PoolAllocator::f2PoolAllocator()
    {
        allocator = new RBPoolAllctor();
        this->header = NULL;
    }

    f2PoolAllocator::~f2PoolAllocator()
    {
        delete allocator;
        allocator = NULL;
    }

    void f2PoolAllocator::init()
    {
        allocator->init();
    }

    void f2PoolAllocator::newPool(uint32 singleSize)
    {
        allocator->new_pool(&this->header, singleSize);
    }

    void* f2PoolAllocator::alloc(uint32 singleSize)
    {
        return allocator->alloc(&this->header, singleSize);
    }

    void f2PoolAllocator::free(void *p, uint32 singleSize)
    {
        allocator->free(&this->header, p, singleSize);
    }

    // ÉèÖÃÄÚ´æ¶ÔÆë×Ö¿í
    void f2PoolAllocator::setAlignment(uint32 alignment)
    {
        allocator->set_alignment(alignment);
    }

}