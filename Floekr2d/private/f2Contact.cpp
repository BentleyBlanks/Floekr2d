#include <Floekr2d/private/f2Contact.h>

namespace Floekr2d
{
    namespace core
    {
        void f2Contact::init(f2Body* A, f2Body* B)
        {
            this->A = A;
            this->B = B;
        }

        f2Manifold* f2Contact::getManifold()
        {
            return &manifold;
        }
    }
}