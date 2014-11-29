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

        // 物理碰撞流水线
        void step();

        // 物理更新频率(不受到外部帧循环影响 单独计时器)
        void setUpdateInterval(float32 interval);

        void setGravity(math::f2Vector2f gravity);

        // create & delete
        f2Body* createBody();

        void deleteBody(f2Body* body);

        // --!1.根据给定类型生成指定类型对象 返回基类指针 
        // --!在外部可使用强制类型转换转换为所需类型指针
        // --!2.值得商榷 究竟是返回基类指针再在外部根据type做强制类型转换
        // --!还是直接生成子类对象指针返回
        f2Shape* createShape(f2Shape::shapeType type);
        //f2CircleShape* createCircleShape();
        //f2PolygonShape* createPolygonShape();

        void deleteShape(f2Shape* shape);

        // getter
        f2List<f2Body*>& getBodyList();

        f2List<f2Shape*>& getShapeList();

        math::f2Vector2f getGravity();

    private:
        // --!完成内存分配器的初始化 这一点是不需要在外部显式调用来完成的
        void init();

        // 对力积分
        void intergrateForces();
        // 对速度积分
        void intergrateVelocity();
        // 应用冲量
        void solveCollision();
        // 碰撞深度修正
        void penetrationCorrection();

        // dt
        float32 dt;

        // 存放物体指针的列表
        f2List<f2Body*> bodyList;
        f2List<f2Shape*> shapeList;
        //f2List<core::f2Manifold*> manifoldList;
        f2List<core::f2Contact*> contactList;

        // 重力
        math::f2Vector2f gravity;

        // --!因为f2ShapeGenerator为单例在这里只是声明而已 实际调用仍然是getInstance()方式
        f2ShapeGenerator *shapeGenerator;
        //f2PoolAllocator *shapeAllocator;
        f2PoolAllocator *bodyAllocator;

        // contact管理器
        core::f2ContactController* contactController;
    };
}

#endif