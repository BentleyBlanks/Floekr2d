#include <Floekr2d/private/f2CircleAndPolygonContact.h>

#include <Floekr2d/private/f2Collision.h>

#include <Floekr2d/protected/allocator/f2PoolAllocator.h>
#include <Floekr2d/public/shape/f2CircleShape.h>
#include <Floekr2d/public/shape/f2PolygonShape.h>

#include <Floekr2d/public/f2Body.h>

namespace Floekr2d
{
    namespace core
    {
        f2CircleAndPolygonContact::f2CircleAndPolygonContact(f2Body* A, f2Body* B)
        {
            f2Assert((A->getShape()->getType() == f2Shape::e_circle && B->getShape()->getType() == f2Shape::e_polygon) ||
                     (A->getShape()->getType() == f2Shape::e_polygon && B->getShape()->getType() == f2Shape::e_circle));

            init(A, B);
        }

        f2Contact* f2CircleAndPolygonContact::create(f2Body* A, f2Body* B, f2PoolAllocator* allocator)
        {
            void* mem = allocator->alloc(sizeof(f2CircleAndPolygonContact));
            // 在mem所代表的内存中构造一个新的指定对象
            return new (mem) f2CircleAndPolygonContact(A, B);
        }

        bool f2CircleAndPolygonContact::collide()
        {
            if(A->getShape()->getType() == f2Shape::e_circle)
                return f2CircletoPolygon(&manifold, (f2CircleShape*) A->getShape(), (f2PolygonShape*) B->getShape());
            else
                return f2PolygontoCircle(&manifold, (f2PolygonShape*) A->getShape(), (f2CircleShape*) B->getShape());
        }
    }
}