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
        // Ĭ��Ϊ����������
        gravity = math::f2Vector2f(0, 0);
        init();

        setUpdateInterval(interval);
        // ���캯��Ƕ�׵��ò���������ʱ����
        // ��������һ��Ĺ��캯��֮����� ��ʱ�������� allocator��Ȼû���ڴ汻���� ��ֵΪ�������趨��ֵ
        //f2Space(math::f2Vector2f(0, 0));
    }

    f2Space::f2Space(math::f2Vector2f gravity, float32 interval) :gravity(gravity)
    {
        init();

        setUpdateInterval(interval);
    }

    void f2Space::init()
    {
        // ��ʼ��body�ڴ������
        bodyAllocator = new f2PoolAllocator();
        bodyAllocator->init();
        bodyAllocator->newPool(sizeof(f2Body));
    }

    // ������ײ��ˮ��
    void f2Space::step()
    {
        // ��ײ���
        contactController = &core::f2ContactController::getContactGenerator();

        // Broad-Phase
        // --!AABB���ټ������ɿ�����ײ��
        contactController->broadPhase(bodyList);

        // Narrow-Phase
        // --!SAT��ײ��� ���ɾ�����ײ��Ϣ(��� ��ײ���� �Ӵ��㼯)
        contactController->narrowPhase(dt, gravity);

        // ��ײ����
        // ��ȡ���µ���ײ�Լ���
        contactList = contactController->getContactList();
        
        // --!�������ӵĻ��ƿ���ʹ�������ڵ�֡����Ҳ�ܹ���֤λ�ñ䶯ƽ��
        // --!ѧϰ�� http://www.niksula.hut.fi/~hkankaan/Homepages/gravity.html
        intergrateForces();

        // ��narrow-phase�������ɵĳ���Ӧ����ȥ
        solveCollision();

        // ���ٶȻ���
        // --!������һ����������ٶ�
        intergrateVelocity();

        // ������Ϊfloat IEEE754��׼�����Ķ�μ����������µ����廥����������
        penetrationCorrection();

        // ����
        contactController->clearContactList();
        // ����body����
        for(int32 i = 0; i < bodyList.size(); i++)
            bodyList[i]->reset();
    }

    // ��������
    // --!ʵ�����õ���Ŀǰֻ������
    void f2Space::intergrateForces()
    {
        for(int32 i = 0; i < bodyList.size(); i++)
        {
            f2Body* body = bodyList[i];

            // �����¾�̬����״̬
            if(body->getInverseMass() == 0.0f)
                continue;

            // dv = ( F / m ) * dt
            math::f2Vector2f dv = body->getLinearVelocity();
            dv += (body->getForces() * body->getInverseMass() + gravity) * (dt / 2.0f);
            // dw = (M(����) / I(ת������)) * dt
            // ������Ϊ�������� �������Ϊ��ת�еĺ����� ת�������������Ϊ��ת�е����� 
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
            // ������ȷ�ϵ���ײ�����ó���
            for(int32 i = 0; i < contactList.size(); i++)
            {
                core::f2Contact* contact = contactList[i];
                f2Body* A = contact->A, *B = contact->B;

                // ������̬���岻������ײ���ٶ�Ϊ0
                if(f2Equals(A->getInverseMass() + B->getInverseMass(), 0.0f))
                {
                    // ��̬�����ٶ�Ϊ0
                    A->setLinearVelocity(math::f2Vector2f(0.0f, 0.0f));
                    B->setLinearVelocity(math::f2Vector2f(0.0f, 0.0f));

                    continue;
                }

                core::f2Manifold* m = contact->getManifold();
                // ��ײ����
                math::f2Vector2f normal = m->normal;

                for(int32 j = 0; j < m->touchPointsNum; j++)
                {
                    // AB���ĵ���ײ�������
                    math::f2Vector2f ra = m->touchPoints[j] - A->getPosition(),
                        rb = m->touchPoints[j] - B->getPosition();

                    // AB������ٶ�
                    // ���ٶȸ������ֶ��� ������ת����Ϊ��ʱ�� ��f2Cross()���㷽����ͬ
                    math::f2Vector2f vAB = B->getLinearVelocity() + f2Cross(B->getAngularVelocity(), rb)
                        - A->getLinearVelocity() - f2Cross(A->getAngularVelocity(), ra);

                    // �������㹫ʽ�ķ�ĸ ����
                    float32 denominator, numerator;

                    float32 rvNorm = vAB * normal;
                    // �������ڷ��� ���账��
                    if(rvNorm > 0)
                        continue;

                    // --!���� ����ıܿ�������һ��ra��ֱ�����Ĺ���
                    // ���� ra�Ĵ�ֱ����(-yr, xr)��normal(x, y) = -x*yr + xr*y = ra��ֱ x normal(x, y)
                    float32 raNormDotNormal = ra.cross(normal);
                    float32 rbNormDotNormal = rb.cross(normal);

                    numerator = -(1.0f + m->e) * rvNorm;
                    denominator = A->getInverseMass() + B->getInverseMass() + 
                        f2Square(raNormDotNormal) * A->getInverseInertia() +
                        f2Square(rbNormDotNormal) * B->getInverseInertia();

                    float32 impulseValue = numerator / denominator;
                    // ��һ���ֳ���������һ���Ӵ����еõ�����
                    impulseValue /= (float32) m->touchPointsNum;

                    math::f2Vector2f impulse = normal * impulseValue;
                    A->applyImpulse(-impulse, ra);
                    B->applyImpulse(impulse, rb);
                }
            }
        }
    }

    // ���ٶȻ���
    void f2Space::intergrateVelocity()
    {
        for(int32 i = 0; i < bodyList.size(); i++)
        {
            f2Body* body = bodyList[i];

            // �����¾�̬����״̬
            if(body->getInverseMass() == 0.0f)
                continue;

            // ʵ��Ϊ��״�ϵ���ת����Ӧ��orient
            math::f2Vector2f p = body->getPosition();
            p += body->getLinearVelocity() * dt;

            float32 radians = body->getRadians();
            radians += body->getAngularVelocity() * dt;

            body->setRadians(radians);
            body->setPosition(p);

            // intergrateForces();
            // ��һ�����������
            // dv = ( F / m ) * dt
            math::f2Vector2f dv = body->getLinearVelocity();
            dv += (body->getForces() * body->getInverseMass() + gravity) * (dt / 2.0f);
            // dw = (M(����) / I(ת������)) * dt
            // ������Ϊ�������� �������Ϊ��ת�еĺ����� ת�������������Ϊ��ת�е����� 
            float32 dw = body->getAngularVelocity();
            dw += (body->getTorque() * body->getInverseInertia()) * (dt / 2.0f);

            body->setLinearVelocity(dv);
            body->setAngularVelocity(dw);
        }
    }

    // ��ײ�������
    void f2Space::penetrationCorrection()
    {
        for(int32 i = 0; i < contactList.size(); i++)
        {
            f2Body* A = contactList[i]->A, *B = contactList[i]->B;
            float32 inverseMassA = A->getInverseMass(), inverseMassB = B->getInverseMass();
            math::f2Vector2f pA = A->getPosition(), pB = B->getPosition();
            core::f2Manifold* m = contactList[i]->getManifold();

            // ��������
            // ���С��k_slop�ǲ���Ҫ����������
            const float32 k_slop = 0.05f;
            // �����ı���
            const float32 percent = 0.4f;

            // �������һ�������Ľ�С
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
        // --!Ŀǰɾ������ķ�ʽ���Ƚϼ�ª
        // --!���ҵ���ָ����bodyList�ĺδ� ֮����ɾȥ���ָ��λ�õ�Ԫ��
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
            // --!�������ҡ�
            //std::cout << "δ����list���ҵ�(��f2Body��f2Shape�Ĵ���������Ҫ��space����ʽ����create���� ��˲������ⲿ�ֶ�new)" << std::endl;
            f2Log("δ����list���ҵ���Ҫɾ���Ķ���(f2Body��f2Shape�Ĵ���������Ҫ��space�е���create���� �������ⲿ�ֶ�new����)");
        }
    }

    // --!δ��װ
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
    //    // ��Ҫ�Ľ���shape����
    //    // --!���ǿ������ת�����׵�����������
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
        // Ϊ�������������
        if(shape == NULL)
            return;

        shapeGenerator = &f2ShapeGenerator::getShapeGenerator();

        bool find = false;
        // --!Ŀǰɾ������ķ�ʽ���Ƚϼ�ª
        // --!���ҵ���ָ����bodyList�ĺδ� ֮����ɾȥ���ָ��λ�õ�Ԫ��
        for(int i = 0; i < shapeList.size(); i++)
        {
            if(shape == shapeList[i])
            {
                find = true;
                shapeList.remove(i);
                // �ͷ��ڴ�
                shapeGenerator->deleteShape(shape);
                // ��shapeGenerator delete֮����Ѿ���shapeΪNULL ������ǿ��
                shape = NULL;
            }
        }

        if(!find)
        {
            //std::cout << "δ����list���ҵ�(��body��shape��Ҫ��space����ʽ����create���� �������ⲿ�ֶ�new���ɶ���)" << std::endl;
            f2Log("δ����list���ҵ���Ҫɾ���Ķ���(body��shape����space�е���create���� �������ⲿ�ֶ�new����)");
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