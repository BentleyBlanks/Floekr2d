#ifndef F2_SHAPE_H
#define F2_SHAPE_H

#include <Floekr2d/protected/f2Settings.h>
#include <Floekr2d/protected/math/f2Matrix2.h>
#include <Floekr2d/private/f2Collision.h>


// 纯虚基类 不准实例化
namespace Floekr2d
{
    // --!来自Box2d
    class f2MassData
    {
    public:
        f2MassData() :mass(0.0), inertia(0.0) {}

        /// The mass of the shape, usually in kilograms.
        float32 mass;

        /// The rotational inertia of the shape about the local origin.
        float32 inertia;
    };

    class f2Shape
    {
    public:
        enum shapeType
        {
            e_circle = 0,
            e_polygon = 1,
            e_typeCount = 2
        };

        virtual void init(float32 density = 1.0f) = 0;

        virtual void setDensity(float32 density);

        virtual void setPosition(math::f2Vector2f position);

        // --!形状只具有计算质量 转动惯量的方法 谁调用与shape本身无关
        // 子类实现计算特定性状转动惯量和质量
        virtual f2MassData& computeMass() = 0;

        // 特殊形状AABB计算方式不同
        // --!实时计算AABB 因为AABB需要根据当前所在的位置而更新其属性 因此并没有将它作为一个属性放在shape当中
        virtual core::f2AABB& computeAABB() = 0;

        // getter
        shapeType getType();
        
        float32 getDensity();

        float32 getRadius();

        float32 getRadians();

        math::f2Vector2f getPosition();

        math::f2Matrix2f getOrient();

    protected:
        // 无奈设友元 为了防止在外部改变shape的orient值
        friend class f2Body;

        virtual void setOrient(float32 radians) = 0;

        // 位置
        // --!这样的设定位置方式学习自box2d 他可以这样设定的原因是他的shape是由shapeDef构造的 因此不存在指针指向同一个形状的问题
        // --!可选的将这个属性交给body 
        // --!为了方便碰撞检测处理 直接将position给shape
        math::f2Vector2f position;
        
        float32 density;

        // --!有重复定义旋转的嫌疑 但能够获得最关键的旋转了的角度的信息
        // 形状朝向矩阵
        math::f2Matrix2f orient;
        // 当前旋转的弧度
        float32 radians;

        // --!目前仅适用于圆形 未来多边形的用途在于边框的厚度
        // --!源思想来源于Box2d->b2Shape
        float32 radius;
        
        // 当前形状类型 运行时类型信息RTTI最简便实现
        shapeType type;

        // AABB
        core::f2AABB aabb;
    };
}

#endif