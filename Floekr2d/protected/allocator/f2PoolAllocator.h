#ifndef F2_ALLOCATOR_H
#define F2_ALLOCATOR_H

// 底层来自WIP的内存管理库
#include <Floekr2d/replaceable/allocator/MemoryPool.h>
#include <Floekr2d/protected/f2Settings.h>

namespace Floekr2d
{
    class f2PoolAllocator
    {
    public:
        f2PoolAllocator();
        ~f2PoolAllocator();

        void init();

        void newPool(uint32 singleSize);

        void* alloc(uint32 singleSize);

        void free(void *p, uint32 singleSize);

        // 设置内存对齐字宽
        // 高级特性 可以留空
        void setAlignment(uint32 alignment);

    private:
        RBPoolAllctor *allocator;

        // 池分配器头指针
        void* header;
    };
}

#endif