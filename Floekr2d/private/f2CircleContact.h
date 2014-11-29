#ifndef F2_CIRCLECONTACT_H
#define F2_CIRCLECONTACT_H

#include <Floekr2d/private/f2Contact.h>

namespace Floekr2d
{
    class f2Body;
    namespace core
    {
        class f2Manifold;
        class f2CircleContact:public f2Contact
        {
        public:
            f2CircleContact(f2Body* A, f2Body* B);

            static f2Contact* create(f2Body* A, f2Body* B, f2PoolAllocator* allocator);

            virtual bool collide();
        };
    }
}

#endif