#ifndef F2_MANIFOLDFENERATOR_H
#define F2_MANIFOLDFENERATOR_H

// 理论上还使用不到
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
            // 不可删除 不可复制 不可自己创建实例
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