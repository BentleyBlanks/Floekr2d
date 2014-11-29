#ifndef F2_BODY_H
#define F2_BODY_H

#include <Floekr2d/protected/f2Settings.h>
//#include <Floekr2d/private/f2Vector2.h>
#include <Floekr2d/protected/math/f2Matrix2.h>

namespace Floekr2d
{
    class f2Shape;
    // --!f2Body的实际创建工作全都是由f2Space中搭配allocator完成的
    // --!因此无需手动new / delete 相反的 这样可能会带来意想不到的错误
    class f2Body
    {
    public:
        f2Body();

        void init();

        // setter
        // 初始化质量与转动惯量的信息
        void setShape(f2Shape* shape);

        void setPosition(math::f2Vector2f position);

        void setLinearVelocity(math::f2Vector2f linearVelocity);

        void setAngularVelocity(float32 angularVelocity);
        
        // 旋转一个物体 目前不能直接设定shape的orient 
        // --!核心为旋转该物体上绑定的形状
        void setRadians(float32 radians);

        // getter
        math::f2Matrix2f getOrient();

        math::f2Vector2f getPosition();

        math::f2Vector2f getLinearVelocity();

        math::f2Vector2f getForces();

        float32 getAngularVelocity();

        float32 getMass();

        float32 getInverseMass();

        float32 getInertia();

        float32 getInverseInertia();

        float32 getRadians();

        float32 getTorque();

        f2Shape* getShape();

        // 施加力与冲量
        void applyForce(const math::f2Vector2f& force);

        void applyImpulse(const math::f2Vector2f& impulse, const math::f2Vector2f& contactVector);
        
        // 重置 进行下一步循环
        void reset();

        // debug
        void print();

        // 滑动摩擦系数
        float32 staticFriction;
        // 最大静摩擦系数
        float32 dynamicFriction;

        // 弹性恢复系数
        float32 restitution;

    private:
        // 线速度 角速度
        math::f2Vector2f linearVelocity;
        float32 angularVelocity;

        // 质量 转动惯量 与之倒数
        // --!直接创建body并不会生成任何质量信息 body本身也不会提供任何设定质量的相关方法
        // --!他们只会被设定为静态物体 mass = inertia = 0.0)
        // --!质量和转动惯量一定需要shape的绑定才能被特定的形状初始化 
        // --!其值与密度相关 且不同的形状所生成的质量大小都有所不同
        float32 mass, inverseMass;
        float32 inertia, inverseInertia;

        // 物体被施加的合力与合力矩
        math::f2Vector2f force;
        float32 torque;

        f2Shape *shape;
    };
}

#endif