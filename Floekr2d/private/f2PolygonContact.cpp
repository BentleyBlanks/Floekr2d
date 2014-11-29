#include <Floekr2d/private/f2PolygonContact.h>

#include <Floekr2d/private/f2Collision.h>

#include <Floekr2d/protected/allocator/f2PoolAllocator.h>
#include <Floekr2d/public/shape/f2PolygonShape.h>

#include <Floekr2d/public/f2Body.h>

namespace Floekr2d
{
    namespace core
    {
        f2PolygonContact::f2PolygonContact(f2Body* A, f2Body* B)
        {
            f2Assert(A->getShape()->getType() == f2Shape::e_polygon);
            f2Assert(B->getShape()->getType() == f2Shape::e_polygon);

            init(A, B);
        }

        f2Contact* f2PolygonContact::create(f2Body* A, f2Body* B, f2PoolAllocator* allocator)
        {
            void* mem = allocator->alloc(sizeof(f2PolygonContact));
            // 在mem所代表的内存中构造一个新的指定对象
            return new (mem) f2PolygonContact(A, B);
        }

        bool f2PolygonContact::collide()
        {
            return f2PolygontoPolygon(&manifold, (f2PolygonShape*) A->getShape(), (f2PolygonShape*) B->getShape());
        }
    }
}