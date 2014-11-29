#ifndef F2_SHAPE_H
#define F2_SHAPE_H

#include <Floekr2d/protected/f2Settings.h>
#include <Floekr2d/protected/math/f2Matrix2.h>
#include <Floekr2d/private/f2Collision.h>


// ������� ��׼ʵ����
namespace Floekr2d
{
    // --!����Box2d
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

        // --!��״ֻ���м������� ת�������ķ��� ˭������shape�����޹�
        // ����ʵ�ּ����ض���״ת������������
        virtual f2MassData& computeMass() = 0;

        // ������״AABB���㷽ʽ��ͬ
        // --!ʵʱ����AABB ��ΪAABB��Ҫ���ݵ�ǰ���ڵ�λ�ö����������� ��˲�û�н�����Ϊһ�����Է���shape����
        virtual core::f2AABB& computeAABB() = 0;

        // getter
        shapeType getType();
        
        float32 getDensity();

        float32 getRadius();

        float32 getRadians();

        math::f2Vector2f getPosition();

        math::f2Matrix2f getOrient();

    protected:
        // ��������Ԫ Ϊ�˷�ֹ���ⲿ�ı�shape��orientֵ
        friend class f2Body;

        virtual void setOrient(float32 radians) = 0;

        // λ��
        // --!�������趨λ�÷�ʽѧϰ��box2d �����������趨��ԭ��������shape����shapeDef����� ��˲�����ָ��ָ��ͬһ����״������
        // --!��ѡ�Ľ�������Խ���body 
        // --!Ϊ�˷�����ײ��⴦�� ֱ�ӽ�position��shape
        math::f2Vector2f position;
        
        float32 density;

        // --!���ظ�������ת������ ���ܹ������ؼ�����ת�˵ĽǶȵ���Ϣ
        // ��״�������
        math::f2Matrix2f orient;
        // ��ǰ��ת�Ļ���
        float32 radians;

        // --!Ŀǰ��������Բ�� δ������ε���;���ڱ߿�ĺ��
        // --!Դ˼����Դ��Box2d->b2Shape
        float32 radius;
        
        // ��ǰ��״���� ����ʱ������ϢRTTI����ʵ��
        shapeType type;

        // AABB
        core::f2AABB aabb;
    };
}

#endif