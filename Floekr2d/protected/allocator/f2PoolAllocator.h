#ifndef F2_ALLOCATOR_H
#define F2_ALLOCATOR_H

// �ײ�����WIP���ڴ�����
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

        // �����ڴ�����ֿ�
        // �߼����� ��������
        void setAlignment(uint32 alignment);

    private:
        RBPoolAllctor *allocator;

        // �ط�����ͷָ��
        void* header;
    };
}

#endif