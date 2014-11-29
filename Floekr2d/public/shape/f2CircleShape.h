#ifndef F2_CIRCLE_SHAPE_H
#define F2_CIRCLE_SHAPE_H

#include <Floekr2d/public/shape/f2Shape.h>

namespace Floekr2d
{
    class f2CircleShape :public f2Shape
    {
    public:
        f2CircleShape(float32 radius = 0.0f);
        ~f2CircleShape() {}

        virtual void init(float32 density = 1.0f);

        void setRadius(float32 radius);

        // 计算转动惯量和质量
        virtual f2MassData& computeMass();
    
        // 计算AABB
        virtual core::f2AABB& computeAABB();

    protected:
        virtual void setOrient(float32 radians);
    };
}
#endif