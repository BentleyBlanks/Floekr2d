// allocator
#include <Floekr2d/protected/allocator/f2PoolAllocator.h>
// shape
#include <Floekr2d/public/shape/f2CircleShape.h>
#include <Floekr2d/public/shape/f2PolygonShape.h>

#include <Floekr2d/public/shape/f2ShapeGenerator.h>

namespace Floekr2d
{
    // f2ShapeGenerator::f2ShapeAllocator
    class f2ShapeGenerator::f2ShapeAllocator
    {
    public:
        f2ShapeAllocator();
        ~f2ShapeAllocator();

        f2CircleShape* createCircleShape();

        f2PolygonShape* createPolygonShape();

        void deletePolygonShape(f2PolygonShape* polygon);

        void deleteCircleShape(f2CircleShape* circle);

    private:
        // ���ڴ������
        // --!��ͬ����״��Ҫ����ڴ������
        f2PoolAllocator *circleShapeAllocator;
        f2PoolAllocator *polygonShapeAllocator;
    };

    f2ShapeGenerator::f2ShapeAllocator::f2ShapeAllocator()
    {
        circleShapeAllocator = new f2PoolAllocator();
        circleShapeAllocator->init();
        circleShapeAllocator->newPool(sizeof(f2CircleShape));

        polygonShapeAllocator = new f2PoolAllocator();
        polygonShapeAllocator->init();
        polygonShapeAllocator->newPool(sizeof(f2PolygonShape));
    }

    f2ShapeGenerator::f2ShapeAllocator::~f2ShapeAllocator()
    {
        // ��ֹ��Ұ
        delete circleShapeAllocator;
        circleShapeAllocator = NULL;

        delete polygonShapeAllocator;
        polygonShapeAllocator = NULL;
    }

    f2CircleShape* f2ShapeGenerator::f2ShapeAllocator::createCircleShape()
    {
        // ��������
        f2CircleShape* shape = (f2CircleShape*) circleShapeAllocator->alloc(sizeof(f2CircleShape));
        // ��ʼ����������� ��Ϊ�Զ�����ڴ������������ù��캯��
        shape->f2CircleShape::f2CircleShape();
        //shape->init();
        return shape;
    }

    f2PolygonShape* f2ShapeGenerator::f2ShapeAllocator::createPolygonShape()
    {
        // ��������
        f2PolygonShape* shape = (f2PolygonShape*) polygonShapeAllocator->alloc(sizeof(f2PolygonShape));
        // ��ʼ����������� ��Ϊ�Զ�����ڴ������������ù��캯��
        shape->f2PolygonShape::f2PolygonShape();
        //shape->init();
        return shape;
    }

    void f2ShapeGenerator::f2ShapeAllocator::deletePolygonShape(f2PolygonShape* polygon)
    {
        // ��������
        circleShapeAllocator->free(polygon, sizeof(f2PolygonShape));
        polygon = NULL;
    }

    void f2ShapeGenerator::f2ShapeAllocator::deleteCircleShape(f2CircleShape* circle)
    {
        // ��������
        polygonShapeAllocator->free(circle, sizeof(f2CircleShape));
        circle = NULL;
    }



    // f2ShapeGenerator
    f2ShapeGenerator& f2ShapeGenerator::getShapeGenerator()
    {
        // �ϸ�ĵ���
        static f2ShapeGenerator generator;
        return generator;
    }

    f2ShapeGenerator::f2ShapeGenerator()
    {
        shapeAllocator = new f2ShapeAllocator();
    }

    //f2CircleShape* f2ShapeGenerator::createCircleShape()
    //{
    //    shapeAllocator->createCircleShape();
    //}

    //f2PolygonShape* f2ShapeGenerator::createPolygonShape()
    //{
    //    shapeAllocator->createPolygonShape();
    //}

    f2Shape* f2ShapeGenerator::createShape(f2Shape::shapeType type)
    {
        f2Shape* shape;

        switch(type)
        {
        case f2Shape::e_polygon:
            shape = (f2Shape*) shapeAllocator->createPolygonShape();
            break;
        case f2Shape::e_circle:
            shape = (f2Shape*) shapeAllocator->createCircleShape();
            break;

        default:
            shape = NULL;
#ifdef _DEBUG
            //std::cout << "��ָ��shape���� �޷�����shape" << std::endl;
            f2Log("��ָ��shape����Ϊ%d �޷�����shape", type);
#endif
            break;
        }

        return shape;
    }

    void f2ShapeGenerator::deleteShape(f2Shape *shape)
    {
        switch(shape->getType())
        {
        case f2Shape::e_polygon:
            shapeAllocator->deletePolygonShape((f2PolygonShape*)shape);
            shape = NULL;
            break;
        case f2Shape::e_circle:
            shapeAllocator->deleteCircleShape((f2CircleShape*)shape);
            shape = NULL;
            break;

        default:
#ifdef _DEBUG
            //std::cout << "��ָ��shape���� �޷�ɾ��shape" << std::endl;
            f2Log("��ָ��shape���� �޷�ɾ��shape");
#endif
            break;
        }
    }
}