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
            // ��������
            f2Manifold* manifold = (f2Manifold*) manifoldAllocator->alloc(sizeof(f2Manifold));
            // ��ʼ����������� ��Ϊ�Զ�����ڴ������������ù��캯��
            manifold->f2Manifold::f2Manifold();
            //shape->init();
            return manifold;
        }

        void f2ManifoldGenerator::f2ManifoldAllocator::deleteManifold(f2Manifold* manifold)
        {
            // ��������
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