#include <Floekr2d/public/shape/f2Shape.h>

namespace Floekr2d
{
    // ���麯��Ҳ������ȱʡ������ Effective C++����˵��
    // δ���ִ��麯������ȱʡ�����������
    void f2Shape::init(float32 density)
    {
        
    }

    void f2Shape::setDensity(float32 density)
    {
        if(density < 0.0f || density >= FLT_MAX)
            density = 0.0f;

        this->density = density;
    }

    void f2Shape::setPosition(math::f2Vector2f position)
    {
        this->position = position;
    }

    f2Shape::shapeType f2Shape::getType()
    {
        return type;
    }

    float32 f2Shape::getDensity()
    {
        return density;
    }

    math::f2Matrix2f f2Shape::getOrient()
    {
        return orient;
    }

    float32 f2Shape::getRadius()
    {
        return radius;
    }

    float32 f2Shape::getRadians()
    {
        return radians;
    }

    math::f2Vector2f f2Shape::getPosition()
    {
        return this->position;
    }
}