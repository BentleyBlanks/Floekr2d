#include <Floekr2d/public/f2Body.h>
#include <Floekr2d/public/shape/f2Shape.h>

namespace Floekr2d
{
    f2Body::f2Body()
    {
        init();
    }

    void f2Body::init()
    {
        linearVelocity.zero();
        angularVelocity = 0.0f;

        restitution = 0.2f;
        staticFriction = 0.0f;
        dynamicFriction = 0.0f;

        mass = 0.0f;
        inverseMass = 0.0f;
        inertia = 0.0f;
        inverseInertia = 0.0f;

        //radians = 0.0f;

        force.zero();
        torque = 0.0f;

        shape = NULL;
    }

    // 初始化质量与转动惯量的信息
    void f2Body::setShape(f2Shape* shape)
    {
        // 排除0作为分母的情况
        f2MassData massData = shape->computeMass();

        mass = massData.mass;
        inverseMass = mass ? 1.0f / mass : 0.0f;

        inertia = massData.inertia;
        inverseInertia = inertia ? 1.0f / inertia : 0.0f;

        this->shape = shape;
    }

    //void f2Body::setOrient(float32 radians)
    //{

    //}

    void f2Body::setPosition(math::f2Vector2f position)
    {
        if(shape == NULL)
        {
#ifdef _DEBUG
            //std::cout << "shape为空 请先setShape()过后再设定值" << std::endl;
            f2Log("shape为空 请先setShape()过后再设定值\n");
#endif
            return;
        }

        shape->setPosition(position);
    }

    void f2Body::setLinearVelocity(math::f2Vector2f linearVelocity)
    {
        this->linearVelocity = linearVelocity;
    }

    void f2Body::setAngularVelocity(float32 angularVelocity)
    {
        this->angularVelocity = angularVelocity;
    }

    // 旋转一个物体
    // --!核心为旋转该物体上绑定的形状
    void f2Body::setRadians(float32 radians)
    {
        if(shape == NULL)
        {
#ifdef _DEBUG
            //std::cout << "shape为空 请先setShape()过后再设定值为" << radians << "弧度的朝向" << std::endl;
            f2Log("shape为空 请先setShape()过后再设定值为%f弧度的朝向\n");
#endif
            return;
        }

        shape->setOrient(radians);
    }

    // getter
    math::f2Matrix2f f2Body::getOrient()
    {
        return shape->getOrient();
    }

    math::f2Vector2f f2Body::getPosition()
    {
        return shape->getPosition();
    }

    math::f2Vector2f f2Body::getLinearVelocity()
    {
        return linearVelocity;
    }

    math::f2Vector2f f2Body::getForces()
    {
        return force;
    }

    float32 f2Body::getAngularVelocity()
    {
        return angularVelocity;
    }

    float32 f2Body::getMass()
    {
        return mass;
    }

    float32 f2Body::getInverseMass()
    {
        return inverseMass;
    }

    float32 f2Body::getInertia()
    {
        return inertia;
    }

    float32 f2Body::getInverseInertia()
    {
        return inverseInertia;
    }

    //float32 f2Body::getRestitution()
    //{
    //    return restitution;
    //}

    float32 f2Body::getRadians()
    {
        return shape->getRadians();
    }

    float32 f2Body::getTorque()
    {
        return torque;
    }

    f2Shape* f2Body::getShape()
    {
        if(shape == NULL)
        {
#ifdef _DEBUG
            f2Log("shape == NULL 切忌在外部使用此指针\n");
#endif
        }
        return shape;
    }

    // 施加力与冲量
    void f2Body::applyForce(const math::f2Vector2f& force)
    {
        this->force += force;
    }

    void f2Body::applyImpulse(const math::f2Vector2f& impulse, const math::f2Vector2f& contactVector)
    {
        linearVelocity += inverseMass * impulse;
        angularVelocity += inverseInertia * contactVector.cross(impulse);
    }

    void f2Body::reset()
    {
        force.set(0, 0);
        torque = 0.0f;
    }

    void f2Body::print()
    {
#ifdef _DEBUG
        // 线速度 角速度
        linearVelocity.print("linearVelocity");

        std::cout << "angularVelocity:" << angularVelocity << std::endl << std::endl;

        // 位置
        shape->getPosition().print("position");

        // 滑动摩擦 最大静摩擦
        std::cout << "staticFriction:" << staticFriction << std::endl;
        std::cout << "dynamicFriction:" << dynamicFriction << std::endl << std::endl;

        std::cout << "restitution:" << restitution << std::endl << std::endl;

        // 质量 转动惯量 与之倒数
        // --!直接创建body并不会生成任何质量信息 body本身也不会提供任何设定质量的相关方法
        // --!他们只会被设定为静态物体 mass = inertia = 0.0)
        // --!质量和转动惯量一定需要shape的绑定才能被特定的形状初始化 
        // --!其值与密度相关 且不同的形状所生成的质量大小都有所不同
        std::cout << "inverseMass:" << inverseMass << std::endl;
        std::cout << "mass:" << mass << std::endl;
        std::cout << "inertia:" << inertia << std::endl;
        std::cout << "inverseInertia:" << inverseInertia << std::endl << std::endl;

        // 物体被施加的合力与合力矩
        force.print("force");
        std::cout << "torque:" << torque << std::endl;
#endif
    }
}
