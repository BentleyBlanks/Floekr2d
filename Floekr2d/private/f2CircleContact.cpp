#include <Floekr2d/private/f2CircleContact.h>
#include <Floekr2d/private/f2Collision.h>

#include <Floekr2d/protected/allocator/f2PoolAllocator.h>
#include <Floekr2d/public/shape/f2CircleShape.h>
#include <Floekr2d/public/f2Body.h>

namespace Floekr2d
{
    namespace core
    {
        f2CircleContact::f2CircleContact(f2Body* A, f2Body* B)
        {
            f2Assert(A->getShape()->getType() == f2Shape::e_circle);
            f2Assert(B->getShape()->getType() == f2Shape::e_circle);

            init(A, B);
        }

        f2Contact* f2CircleContact::create(f2Body* A, f2Body* B, f2PoolAllocator* allocator)
        {
            void* mem = allocator->alloc(sizeof(f2CircleContact));
            // 在mem所代表的内存中构造一个新的指定对象
            return new (mem) f2CircleContact(A, B);
        }

        bool f2CircleContact::collide()
        {
            return f2CircletoCircle(&manifold, (f2CircleShape*) A->getShape(), (f2CircleShape*) B->getShape());
        }
    }
}