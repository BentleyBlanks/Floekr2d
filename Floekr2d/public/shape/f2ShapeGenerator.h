#ifndef F2_SHAPEGENERATOR_H
#define F2_SHAPEGENERATOR_H

#include <Floekr2d/public/shape/f2Shape.h>

namespace Floekr2d
{
    class f2Shape;
    //class f2CircleShape;
    //class f2PolygonShape;

    class f2ShapeGenerator
    {
    public:
        static f2ShapeGenerator& getShapeGenerator();

        // --!真正的create具体类型shape在shapeAllocator中 这里只做一个类型判别的工作
        // --!这里不加参数的主要考虑原因还是因为内存分配器不会调用类的构造函数
        // --!未来添加参数的可能原因是在函数体中调用函数来设定值 但这样维护较为复杂
        //f2CircleShape* createCircleShape();
        //f2PolygonShape* createPolygonShape();
        f2Shape* createShape(f2Shape::shapeType type);

        // 动态类型确定其究竟类型为何
        void deleteShape(f2Shape* shape);

    private:
        // 不可删除 不可复制 不可自己创建实例
        f2ShapeGenerator();
        f2ShapeGenerator(const f2ShapeGenerator&) {};
        ~f2ShapeGenerator() {};
        f2ShapeGenerator& operator=(const f2ShapeGenerator& generator) const {};

        // pimple
        class f2ShapeAllocator;
        f2ShapeAllocator *shapeAllocator;
    };
}

#endif