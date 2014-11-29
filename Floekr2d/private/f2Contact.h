#ifndef F2_CONTACT_H
#define F2_CONTACT_H

#include <Floekr2d/private/f2Collision.h>

namespace Floekr2d
{
    class f2Body;
    class f2PoolAllocator;
    namespace core
    {
        class f2Contact
        {
        public:
            // --!���า��
            static f2Contact* create(f2Body* A, f2Body* B, f2PoolAllocator* allocator) {};

            virtual void init(f2Body* A, f2Body* B);

            virtual bool collide() = 0;

            f2Manifold* getManifold();

            // ����ײ����
            f2Body* A;
            f2Body* B;
        protected:
            // һ����ײ�� һ��manifold��Ϣ
            f2Manifold manifold;
        };
    }
}

#endif