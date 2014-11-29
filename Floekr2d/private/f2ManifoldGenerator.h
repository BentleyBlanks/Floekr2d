#ifndef F2_MANIFOLDFENERATOR_H
#define F2_MANIFOLDFENERATOR_H

// �����ϻ�ʹ�ò���
namespace Floekr2d
{
    namespace core
    {
        class f2Manifold;
        class f2ManifoldGenerator
        {
        public:
            static f2ManifoldGenerator& getManifoldGenerator();

            f2Manifold* createManifold();

            void deleteManifold(f2Manifold* manifold);

        private:
            // ����ɾ�� ���ɸ��� �����Լ�����ʵ��
            f2ManifoldGenerator();
            f2ManifoldGenerator(const f2ManifoldGenerator&) {};
            ~f2ManifoldGenerator() {};
            f2ManifoldGenerator& operator=(const f2ManifoldGenerator& generator) const {};

            // pimple
            class f2ManifoldAllocator;
            f2ManifoldAllocator *manifoldAllocator;
        };
    }
}

#endif