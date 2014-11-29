#include <Floekr2d/public/shape/f2CircleShape.h>

namespace Floekr2d
{
    f2CircleShape::f2CircleShape(float32 radius)
    {
        init(1.0f);

        setRadius(radius);
    }

    void f2CircleShape::init(float32 density)
    {
        setDensity(density);

        setOrient(0.0f);

        type = e_circle;

        position.zero();
    }

    void f2CircleShape::setRadius(float32 radius)
    {
        if(radius<0.0f || radius > FLT_MAX)
            radius = 0.0f;

        this->radius = radius;
    }

    void f2CircleShape::setOrient(float32 radians)
    {
        // Բ�����賯������
        this->radians = radians;
    }

    // ����ʵ�ּ����ض���״ת������������
    f2MassData& f2CircleShape::computeMass()
    {
        f2MassData massData;

        // Բ��� * �ܶ�
        massData.mass = PI * radius * radius * density;

        // ת������ = mass * radius^2
        massData.inertia = massData.mass * radius * radius;

        return massData;
    }

    core::f2AABB& f2CircleShape::computeAABB()
    {
        aabb.min.x = position.x - radius;
        aabb.min.y = position.y - radius;
        aabb.max.x = position.x + radius;
        aabb.max.y = position.y + radius;

        return aabb;
    }

    //float32 f2CircleShape::computeInertia()
    //{
    //    return computeMass() * radius * radius;
    //}
}