#ifndef F2_POLYGONCONTACT_H
#define F2_POLYGONCONTACT_H

#include <Floekr2d/private/f2Contact.h>

namespace Floekr2d
{
    class f2Body;
    namespace core
    {
        class f2Manifold;
        class f2PolygonContact :public f2Contact
        {
        public:
            f2PolygonContact(f2Body* A, f2Body* B);

            static f2Contact* create(f2Body* A, f2Body* B, f2PoolAllocator* allocator);

            virtual bool collide();
        };
    }
}

#endif