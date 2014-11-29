#include <Floekr2d/protected/allocator/f2PoolAllocator.h>
#include <Floekr2d/public/f2Space.h>
#include <Floekr2d/protected/f2Settings.h>

#include <Floekr2d/public/shape/f2ShapeGenerator.h>
#include <Floekr2d/private/f2ContactController.h>

#include <Floekr2d/public/f2Body.h>
#include <Floekr2d/private/f2Contact.h>

namespace Floekr2d
{
    f2Space::f2Space(float32 interval)
    {
        // 默认为无重力环境
        gravity = math::f2Vector2f(0, 0);
        init();

        setUpdateInterval(interval);
        // 构造函数嵌套调用产生匿名临时对象
        // 调用完这一层的构造函数之后出来 临时对象被销毁 allocator任然没有内存被分配 初值为编译器设定的值
        //f2Space(math::f2Vector2f(0, 0));
    }

    f2Space::f2Space(math::f2Vector2f gravity, float32 interval) :gravity(gravity)
    {
        init();

        setUpdateInterval(interval);
    }

    void f2Space::init()
    {
        // 初始化body内存分配器
        bodyAllocator = new f2PoolAllocator();
        bodyAllocator->init();
        bodyAllocator->newPool(sizeof(f2Body));
    }

    // 物理碰撞流水线
    void f2Space::step()
    {
        // 碰撞检测
        contactController = &core::f2ContactController::getContactGenerator();

        // Broad-Phase
        // --!AABB快速检碰生成可能碰撞对
        contactController->broadPhase(bodyList);

        // Narrow-Phase
        // --!SAT碰撞检测 生成具体碰撞信息(深度 碰撞法线 接触点集)
        contactController->narrowPhase(dt, gravity);

        // 碰撞反馈
        // 获取最新的碰撞对集合
        contactList = contactController->getContactList();
        
        // --!重力附加的机制可以使得引擎在低帧率下也能够保证位置变动平滑
        // --!学习自 http://www.niksula.hut.fi/~hkankaan/Homepages/gravity.html
        intergrateForces();

        // 将narrow-phase步骤生成的冲量应用上去
        solveCollision();

        // 对速度积分
        // --!补回另一半的重力加速度
        intergrateVelocity();

        // 修正因为float IEEE754标准带来的多次计算误差而导致的物体互相陷入的情况
        penetrationCorrection();

        // 重置
        contactController->clearContactList();
        // 所有body重置
        for(int32 i = 0; i < bodyList.size(); i++)
            bodyList[i]->reset();
    }

    // 对力积分
    // --!实际作用的力目前只有重力
    void f2Space::intergrateForces()
    {
        for(int32 i = 0; i < bodyList.size(); i++)
        {
            f2Body* body = bodyList[i];

            // 不更新静态物体状态
            if(body->getInverseMass() == 0.0f)
                continue;

            // dv = ( F / m ) * dt
            math::f2Vector2f dv = body->getLinearVelocity();
            dv += (body->getForces() * body->getInverseMass() + gravity) * (dt / 2.0f);
            // dw = (M(力矩) / I(转动惯量)) * dt
            // 力矩则为合外力矩 可以理解为旋转中的合外力 转动惯量可以理解为旋转中的质量 
            float32 dw = body->getAngularVelocity();
            dw += (body->getTorque() * body->getInverseInertia()) * (dt / 2.0f);
            
            body->setLinearVelocity(dv);
            body->setAngularVelocity(dw);
        }
    }

    void f2Space::solveCollision()
    {
        for(int32 iteration = 0; iteration < 1; iteration++)
        {
            // 对所有确认的碰撞对作用冲量
            for(int32 i = 0; i < contactList.size(); i++)
            {
                core::f2Contact* contact = contactList[i];
                f2Body* A = contact->A, *B = contact->B;

                // 两个静态物体不参与碰撞且速度为0
                if(f2Equals(A->getInverseMass() + B->getInverseMass(), 0.0f))
                {
                    // 静态物体速度为0
                    A->setLinearVelocity(math::f2Vector2f(0.0f, 0.0f));
                    B->setLinearVelocity(math::f2Vector2f(0.0f, 0.0f));

                    continue;
                }

                core::f2Manifold* m = contact->getManifold();
                // 碰撞法线
                math::f2Vector2f normal = m->normal;

                for(int32 j = 0; j < m->touchPointsNum; j++)
                {
                    // AB质心到碰撞点的向量
                    math::f2Vector2f ra = m->touchPoints[j] - A->getPosition(),
                        rb = m->touchPoints[j] - B->getPosition();

                    // AB的相对速度
                    // 角速度根据右手定则 正则旋转方向为逆时针 与f2Cross()计算方向相同
                    math::f2Vector2f vAB = B->getLinearVelocity() + f2Cross(B->getAngularVelocity(), rb)
                        - A->getLinearVelocity() - f2Cross(A->getAngularVelocity(), ra);

                    // 冲量计算公式的分母 分子
                    float32 denominator, numerator;

                    float32 rvNorm = vAB * normal;
                    // 二者正在分离 无需处理
                    if(rvNorm > 0)
                        continue;

                    // --!亮点 巧妙的避开了再求一遍ra垂直向量的过程
                    // 这里 ra的垂直向量(-yr, xr)·normal(x, y) = -x*yr + xr*y = ra垂直 x normal(x, y)
                    float32 raNormDotNormal = ra.cross(normal);
                    float32 rbNormDotNormal = rb.cross(normal);

                    numerator = -(1.0f + m->e) * rvNorm;
                    denominator = A->getInverseMass() + B->getInverseMass() + 
                        f2Square(raNormDotNormal) * A->getInverseInertia() +
                        f2Square(rbNormDotNormal) * B->getInverseInertia();

                    float32 impulseValue = numerator / denominator;
                    // 另一部分冲量会在另一个接触点中得到计算
                    impulseValue /= (float32) m->touchPointsNum;

                    math::f2Vector2f impulse = normal * impulseValue;
                    A->applyImpulse(-impulse, ra);
                    B->applyImpulse(impulse, rb);
                }
            }
        }
    }

    // 对速度积分
    void f2Space::intergrateVelocity()
    {
        for(int32 i = 0; i < bodyList.size(); i++)
        {
            f2Body* body = bodyList[i];

            // 不更新静态物体状态
            if(body->getInverseMass() == 0.0f)
                continue;

            // 实质为形状上的旋转矩阵应用orient
            math::f2Vector2f p = body->getPosition();
            p += body->getLinearVelocity() * dt;

            float32 radians = body->getRadians();
            radians += body->getAngularVelocity() * dt;

            body->setRadians(radians);
            body->setPosition(p);

            // intergrateForces();
            // 另一半的重力积分
            // dv = ( F / m ) * dt
            math::f2Vector2f dv = body->getLinearVelocity();
            dv += (body->getForces() * body->getInverseMass() + gravity) * (dt / 2.0f);
            // dw = (M(力矩) / I(转动惯量)) * dt
            // 力矩则为合外力矩 可以理解为旋转中的合外力 转动惯量可以理解为旋转中的质量 
            float32 dw = body->getAngularVelocity();
            dw += (body->getTorque() * body->getInverseInertia()) * (dt / 2.0f);

            body->setLinearVelocity(dv);
            body->setAngularVelocity(dw);
        }
    }

    // 碰撞深度修正
    void f2Space::penetrationCorrection()
    {
        for(int32 i = 0; i < contactList.size(); i++)
        {
            f2Body* A = contactList[i]->A, *B = contactList[i]->B;
            float32 inverseMassA = A->getInverseMass(), inverseMassB = B->getInverseMass();
            math::f2Vector2f pA = A->getPosition(), pB = B->getPosition();
            core::f2Manifold* m = contactList[i]->getManifold();

            // 修正比例
            // 深度小于k_slop是不需要进行修正的
            const float32 k_slop = 0.05f;
            // 修正的比率
            const float32 percent = 0.4f;

            // 质量大的一方修正的较小
            math::f2Vector2f correction = (std::max(m->penetration - k_slop, 0.0f) / (inverseMassA + inverseMassB)) * m->normal * percent;
            
            pA -= correction * inverseMassA;
            pB += correction * inverseMassB;

            A->setPosition(pA);
            B->setPosition(pB);
        }
    }

    void f2Space::setUpdateInterval(float32 interval)
    {
        if(interval > FLT_MAX)
            interval = 1.0f / 60.0f;

        this->dt = interval;
    }

    void f2Space::setGravity(math::f2Vector2f gravity)
    {
        this->gravity = gravity;
    }

    f2Body* f2Space::createBody()
    {
        f2Body* body = (f2Body*) bodyAllocator->alloc(sizeof(f2Body));

        body->init();
        bodyList.pushback(body);

        return body;
    }

    void f2Space::deleteBody(f2Body* body)
    {
        if(body == NULL)
            return;

        bool find = false;
        // --!目前删除物体的方式还比较简陋
        // --!先找到该指针在bodyList的何处 之后再删去这个指定位置的元素
        for(int i = 0; i < bodyList.size(); i++)
        {
            if(body == bodyList[i])
            {
                find = true;
                bodyList.remove(i);

                bodyAllocator->free(body, sizeof(f2Body));
                body = NULL;
            }
        }

        if(!find)
        {
            // --!除非是我…
            //std::cout << "未能在list中找到(因f2Body和f2Shape的创建工作需要在space中显式调用create创建 因此不能在外部手动new)" << std::endl;
            f2Log("未能在list中找到需要删除的对象(f2Body和f2Shape的创建工作需要在space中调用create创建 不能在外部手动new生成)");
        }
    }

    // --!未包装
    f2Shape* f2Space::createShape(f2Shape::shapeType type)
    {
        f2Shape* shape;

        shapeGenerator = &f2ShapeGenerator::getShapeGenerator();

        shape = shapeGenerator->createShape(type);
        shapeList.pushback(shape);

        return shape;
    }


    //f2CircleShape* f2Space::createCircleShape()
    //{
    //    f2Shape* shape;
    //    shapeGenerator = &f2ShapeGenerator::getShapeGenerator();
    //    // 需要改进的shape管理
    //    // --!多次强制类型转换容易导致性能问题
    //    shape = (f2Shape*) shapeGenerator->createCircleShape();
    //    shapeList.pushback(shape);

    //    return (f2CircleShape*) shape;
    //}

    //f2PolygonShape* f2Space::createPolygonShape()
    //{
    //    f2Shape* shape;
    //    shapeGenerator = &f2ShapeGenerator::getShapeGenerator();

    //    shape = (f2Shape*)shapeGenerator->createPolygonShape();

    //    shapeList.pushback(shape);

    //    return (f2PolygonShape*) shape;
    //}

    void f2Space::deleteShape(f2Shape* shape)
    {
        // 为该物体举行葬礼
        if(shape == NULL)
            return;

        shapeGenerator = &f2ShapeGenerator::getShapeGenerator();

        bool find = false;
        // --!目前删除物体的方式还比较简陋
        // --!先找到该指针在bodyList的何处 之后再删去这个指定位置的元素
        for(int i = 0; i < shapeList.size(); i++)
        {
            if(shape == shapeList[i])
            {
                find = true;
                shapeList.remove(i);
                // 释放内存
                shapeGenerator->deleteShape(shape);
                // 在shapeGenerator delete之后就已经设shape为NULL 这里起强调
                shape = NULL;
            }
        }

        if(!find)
        {
            //std::cout << "未能在list中找到(因body和shape需要在space中显式调用create创建 不能在外部手动new生成对象)" << std::endl;
            f2Log("未能在list中找到需要删除的对象(body和shape需在space中调用create创建 不能在外部手动new生成)");
        }
    }

    f2List<f2Body*>& f2Space::getBodyList()
    {
        return bodyList;
    }

    f2List<f2Shape*>& f2Space::getShapeList()
    {
        return shapeList;
    }

    math::f2Vector2f f2Space::getGravity()
    {
        return gravity;
    }
}