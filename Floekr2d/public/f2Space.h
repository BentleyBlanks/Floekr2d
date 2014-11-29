#ifndef F2_SPACE_H
#define F2_SPACE_H

#include <Floekr2d/protected/datastruct/f2List.h>
#include <Floekr2d/protected/math/f2Vector2.h>
#include <Floekr2d/public/shape/f2Shape.h>

namespace Floekr2d
{
    class f2Body;
    class f2Shape;
    class f2CircleShape;
    class f2PolygonShape;
    class f2PoolAllocator;

    class f2ShapeGenerator;

    namespace core
    { 
        class f2Contact;
        class f2ContactController; 
        class f2Manifold;
    }
    

    class f2Space
    {
    public:
        f2Space(float32 interval = 1.0f / 60.0f);
        f2Space(math::f2Vector2f gravity, float32 interval = 1.0f / 60.0f);

        // ������ײ��ˮ��
        void step();

        // �������Ƶ��(���ܵ��ⲿ֡ѭ��Ӱ�� ������ʱ��)
        void setUpdateInterval(float32 interval);

        void setGravity(math::f2Vector2f gravity);

        // create & delete
        f2Body* createBody();

        void deleteBody(f2Body* body);

        // --!1.���ݸ�����������ָ�����Ͷ��� ���ػ���ָ�� 
        // --!���ⲿ��ʹ��ǿ������ת��ת��Ϊ��������ָ��
        // --!2.ֵ����ȶ �����Ƿ��ػ���ָ�������ⲿ����type��ǿ������ת��
        // --!����ֱ�������������ָ�뷵��
        f2Shape* createShape(f2Shape::shapeType type);
        //f2CircleShape* createCircleShape();
        //f2PolygonShape* createPolygonShape();

        void deleteShape(f2Shape* shape);

        // getter
        f2List<f2Body*>& getBodyList();

        f2List<f2Shape*>& getShapeList();

        math::f2Vector2f getGravity();

    private:
        // --!����ڴ�������ĳ�ʼ�� ��һ���ǲ���Ҫ���ⲿ��ʽ��������ɵ�
        void init();

        // ��������
        void intergrateForces();
        // ���ٶȻ���
        void intergrateVelocity();
        // Ӧ�ó���
        void solveCollision();
        // ��ײ�������
        void penetrationCorrection();

        // dt
        float32 dt;

        // �������ָ����б�
        f2List<f2Body*> bodyList;
        f2List<f2Shape*> shapeList;
        //f2List<core::f2Manifold*> manifoldList;
        f2List<core::f2Contact*> contactList;

        // ����
        math::f2Vector2f gravity;

        // --!��Ϊf2ShapeGeneratorΪ����������ֻ���������� ʵ�ʵ�����Ȼ��getInstance()��ʽ
        f2ShapeGenerator *shapeGenerator;
        //f2PoolAllocator *shapeAllocator;
        f2PoolAllocator *bodyAllocator;

        // contact������
        core::f2ContactController* contactController;
    };
}

#endif