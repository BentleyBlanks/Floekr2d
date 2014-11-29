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

        // --!������create��������shape��shapeAllocator�� ����ֻ��һ�������б�Ĺ���
        // --!���ﲻ�Ӳ�������Ҫ����ԭ������Ϊ�ڴ���������������Ĺ��캯��
        // --!δ����Ӳ����Ŀ���ԭ�����ں������е��ú������趨ֵ ������ά����Ϊ����
        //f2CircleShape* createCircleShape();
        //f2PolygonShape* createPolygonShape();
        f2Shape* createShape(f2Shape::shapeType type);

        // ��̬����ȷ���便������Ϊ��
        void deleteShape(f2Shape* shape);

    private:
        // ����ɾ�� ���ɸ��� �����Լ�����ʵ��
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