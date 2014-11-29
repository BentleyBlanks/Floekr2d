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
        // 圆形无需朝向设置
        this->radians = radians;
    }

    // 子类实现计算特定性状转动惯量和质量
    f2MassData& f2CircleShape::computeMass()
    {
        f2MassData massData;

        // 圆面积 * 密度
        massData.mass = PI * radius * radius * density;

        // 转动惯量 = mass * radius^2
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