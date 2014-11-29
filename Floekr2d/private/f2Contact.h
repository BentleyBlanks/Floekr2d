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
            // --!子类覆盖
            static f2Contact* create(f2Body* A, f2Body* B, f2PoolAllocator* allocator) {};

            virtual void init(f2Body* A, f2Body* B);

            virtual bool collide() = 0;

            f2Manifold* getManifold();

            // 两碰撞对象
            f2Body* A;
            f2Body* B;
        protected:
            // 一个碰撞对 一个manifold信息
            f2Manifold manifold;
        };
    }
}

#endif