#ifndef F2_BODY_H
#define F2_BODY_H

#include <Floekr2d/protected/f2Settings.h>
//#include <Floekr2d/private/f2Vector2.h>
#include <Floekr2d/protected/math/f2Matrix2.h>

namespace Floekr2d
{
    class f2Shape;
    // --!f2Body��ʵ�ʴ�������ȫ������f2Space�д���allocator��ɵ�
    // --!��������ֶ�new / delete �෴�� �������ܻ�������벻���Ĵ���
    class f2Body
    {
    public:
        f2Body();

        void init();

        // setter
        // ��ʼ��������ת����������Ϣ
        void setShape(f2Shape* shape);

        void setPosition(math::f2Vector2f position);

        void setLinearVelocity(math::f2Vector2f linearVelocity);

        void setAngularVelocity(float32 angularVelocity);
        
        // ��תһ������ Ŀǰ����ֱ���趨shape��orient 
        // --!����Ϊ��ת�������ϰ󶨵���״
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

        // ʩ���������
        void applyForce(const math::f2Vector2f& force);

        void applyImpulse(const math::f2Vector2f& impulse, const math::f2Vector2f& contactVector);
        
        // ���� ������һ��ѭ��
        void reset();

        // debug
        void print();

        // ����Ħ��ϵ��
        float32 staticFriction;
        // ���Ħ��ϵ��
        float32 dynamicFriction;

        // ���Իָ�ϵ��
        float32 restitution;

    private:
        // ���ٶ� ���ٶ�
        math::f2Vector2f linearVelocity;
        float32 angularVelocity;

        // ���� ת������ ��֮����
        // --!ֱ�Ӵ���body�����������κ�������Ϣ body����Ҳ�����ṩ�κ��趨��������ط���
        // --!����ֻ�ᱻ�趨Ϊ��̬���� mass = inertia = 0.0)
        // --!������ת������һ����Ҫshape�İ󶨲��ܱ��ض�����״��ʼ�� 
        // --!��ֵ���ܶ���� �Ҳ�ͬ����״�����ɵ�������С��������ͬ
        float32 mass, inverseMass;
        float32 inertia, inverseInertia;

        // ���屻ʩ�ӵĺ����������
        math::f2Vector2f force;
        float32 torque;

        f2Shape *shape;
    };
}

#endif