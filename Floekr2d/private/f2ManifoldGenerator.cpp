#include <Floekr2d/private/f2ManifoldGenerator.h>

#include <Floekr2d/protected/allocator/f2PoolAllocator.h>
#include <Floekr2d/private/f2Collision.h>

namespace Floekr2d
{
    namespace core
    {
        // f2ManifoldAllocator
        class f2ManifoldGenerator::f2ManifoldAllocator
        {
        public:
            f2ManifoldAllocator();

            f2Manifold* createManifold();

            void deleteManifold(f2Manifold* manifold);

        private:
            f2PoolAllocator* manifoldAllocator;
        };

        f2ManifoldGenerator::f2ManifoldAllocator::f2ManifoldAllocator()
        {
            manifoldAllocator = new f2PoolAllocator();
            manifoldAllocator->init();
            manifoldAllocator->newPool(sizeof(f2Manifold));
        }

        f2Manifold* f2ManifoldGenerator::f2ManifoldAllocator::createManifold()
        {
            // 具体类型
            f2Manifold* manifold = (f2Manifold*) manifoldAllocator->alloc(sizeof(f2Manifold));
            // 初始化其具体类型 因为自定义的内存分配器不会调用构造函数
            manifold->f2Manifold::f2Manifold();
            //shape->init();
            return manifold;
        }

        void f2ManifoldGenerator::f2ManifoldAllocator::deleteManifold(f2Manifold* manifold)
        {
            // 具体类型
            manifoldAllocator->free(manifold, sizeof(f2Manifold));
            manifold = NULL;
        }


        // f2ManifoldGenerator
        f2ManifoldGenerator& f2ManifoldGenerator::getManifoldGenerator()
        {
            static f2ManifoldGenerator manifoldGenerator;
            return manifoldGenerator;
        }

        f2Manifold* f2ManifoldGenerator::createManifold()
        {
            return manifoldAllocator->createManifold();
        }

        void f2ManifoldGenerator::deleteManifold(f2Manifold* manifold)
        {
            manifoldAllocator->deleteManifold(manifold);
        }

        f2ManifoldGenerator::f2ManifoldGenerator()
        {
            manifoldAllocator = new f2ManifoldAllocator();

        }
    }
}