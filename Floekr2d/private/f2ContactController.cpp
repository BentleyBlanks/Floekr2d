#include <Floekr2d/private/f2ContactController.h>
#include <Floekr2d/protected/allocator/f2PoolAllocator.h>

#include <Floekr2d/public/shape/f2Shape.h>

// contact
#include <Floekr2d/private/f2CircleAndPolygonContact.h>
#include <Floekr2d/private/f2CircleContact.h>
#include <Floekr2d/private/f2PolygonContact.h>

// body--aabb
#include <Floekr2d/public/f2Body.h>
//#include <Floekr2d/private/f2ManifoldGenerator.h>

namespace Floekr2d
{
    namespace core
    {
        // f2ContactGenerator
        // 所有contact子类create回调
        typedef f2Contact* f2ContactCreateCallback(f2Body* A, f2Body* B, f2PoolAllocator* allocator);

        class f2ContactController::f2ContactGenerator
        {
        public:
            f2ContactGenerator();

            f2Contact* createContact(f2Body* A, f2Body* B);

            void deleteContact(f2Contact* contact);

        private:
            // 在注册机中注册所有形状的create回调
            void initRegister();

            // 是否完成注册
            bool bRegister;

            // contact内存分配器
            f2PoolAllocator* contactAllocator;

            // 回调注册机
            // --!实质为[count][count]二维数组
            f2ContactCreateCallback* contactRegister[f2Shape::e_typeCount][f2Shape::e_typeCount];
        };

        f2ContactController::f2ContactGenerator::f2ContactGenerator()
        {
            bRegister = false;
            // 初始化回调的注册
            initRegister();

            contactAllocator = new f2PoolAllocator();
            contactAllocator->init();
            contactAllocator->newPool(sizeof(f2Contact));
        }

        f2Contact* f2ContactController::f2ContactGenerator::createContact(f2Body* A, f2Body* B)
        {
            f2Shape::shapeType type1 = A->getShape()->getType();
            f2Shape::shapeType type2 = B->getShape()->getType();

            f2Assert(0 <= type1 && type1 < f2Shape::e_typeCount);
            f2Assert(0 <= type2 && type2 < f2Shape::e_typeCount);

            // 此处无需知晓AB具体形状 直接根据[][]对应回调生成即可 模拟实现多态
            return contactRegister[type1][type2](A, B, contactAllocator);
        }

        void f2ContactController::f2ContactGenerator::deleteContact(f2Contact* contact)
        {
            contactAllocator->free(contact, sizeof(f2Contact));
            contact = NULL;
        }

        void f2ContactController::f2ContactGenerator::initRegister()
        {
            // 严格保证只注册一次
            if(!bRegister)
            {
                // 注册各个形状的create函数
                contactRegister[f2Shape::e_circle][f2Shape::e_circle] = f2CircleContact::create;
                contactRegister[f2Shape::e_circle][f2Shape::e_polygon] = f2CircleAndPolygonContact::create;
                contactRegister[f2Shape::e_polygon][f2Shape::e_circle] = f2CircleAndPolygonContact::create;
                contactRegister[f2Shape::e_polygon][f2Shape::e_polygon] = f2PolygonContact::create;

                bRegister = true;
            }
        }


        // f2ContactController
        f2ContactController& f2ContactController::getContactGenerator()
        {
            static f2ContactController contactController;
            return contactController;
        }

        // Broad-Phase
        // --!AABB快速检碰生成可能碰撞对
        void f2ContactController::broadPhase(f2List<f2Body*>& bodyList)
        {
            //f2Log("broadPhase\n");

            // 循环遍历 计算AABB 
            // 生成可能碰撞的对
            for(int32 i = 0; i < bodyList.size(); i++)
            {
                f2Body* A = bodyList[i];
                f2Shape* shapeA = A->getShape();
                float32 inverseMassA = A->getInverseMass();

                for(int32 j = i + 1; j < bodyList.size(); j++)
                {
                    f2Body* B = bodyList[j];
                    f2Shape* shapeB = B->getShape();
                    float32 inverseMassB = B->getInverseMass();
                    
                    // 两者都为静态物体不参与碰撞检测
                    if(inverseMassA == 0 && inverseMassB == 0)
                        continue;

                    
                    //// 碰撞可能发生
                    //if(f2AABBAndAABB(&shapeA->computeAABB(), &shapeB->computeAABB()))
                    //{
                    //    // 增加一个碰撞对
                    //    addContact(contactGenerator->createContact(A, B));
                    //}
                    f2Contact* contact = contactGenerator->createContact(A, B);
                    if(contact->collide())
                    {
                        addContact(contact);
                    }
                    else
                        contactGenerator->deleteContact(contact);
                }
            }
        }

        // Narrow-Phase
        // --!SAT碰撞检测 生成具体碰撞信息(深度 碰撞法线 接触点集)
        void f2ContactController::narrowPhase(float32& dt, math::f2Vector2f& gravity)
        {
            //f2Log("narrowPhase\n");
            //f2Log("contact size:%d\n", contactList.size());

            for(int32 i = 0; i < contactList.size(); i++)
            {
                f2Contact* contact = contactList[i];
                // --!无需知晓contact两body具体类型即可调用
                if(!contact->collide())
                {
                    // 碰撞未发生
                    contactGenerator->deleteContact(contact);
                    // --!删除的方式有误
                    // 从contactList中删去
                    contactList.remove(i);
                }
                else
                {
                    core::f2Contact* contact = contactList[i];
                    core::f2Manifold* m = contact->getManifold();
                    f2Body* A = contact->A, *B = contact->B;

                    // 实际恢复系数为较小的一方
                    m->e = std::min(A->restitution, B->restitution);

                    // 摩擦系数为两者平均
                    m->staticFriction = std::sqrt(A->staticFriction * B->staticFriction);
                    m->dynamicFriction = std::sqrt(A->dynamicFriction * B->dynamicFriction);

                    for(int32 i = 0; i < m->touchPointsNum; i++)
                    {
                        // 从质心到碰撞点
                        math::f2Vector2f ra = m->touchPoints[i] - A->getPosition();
                        math::f2Vector2f rb = m->touchPoints[i] - B->getPosition();

                        // 自身速度与角速度的和(实际线速度)
                        // --!此处cross的作用是将rb的垂直线缩放angularVelocity倍数
                        math::f2Vector2f relativeV = B->getLinearVelocity() + f2Cross(B->getAngularVelocity(), rb) -
                            A->getLinearVelocity() - f2Cross(A->getAngularVelocity(), ra);

                        // 是否应用恢复系数 取决于分离这两个物体的因素是否只有重力 
                        // 若是那么外界无需提供任何恢复系数来使得他们分开
                        // rv为两者之间的速度差
                        if(relativeV.getLengthSquare() < (dt * gravity).getLengthSquare() + F2_EPSILON)
                            m->e = 0.0f;
                    }
                }
            }
        }

        f2List<f2Contact*>& f2ContactController::getContactList()
        {
            return contactList;
        }

        // 重置 / 清除contact列表
        void f2ContactController::clearContactList()
        {
            //f2Log("clearContactList\n");
            for(int32 i = 0; i < contactList.size(); i++)
            {
                contactGenerator->deleteContact(contactList[i]);
            }
            contactList.clear();
        }

        // broad-phase自动生成可能碰撞对 无需手动
        void f2ContactController::addContact(f2Contact* contact)
        {
            if(contact == NULL)
            {
#ifdef _DEBUG
                f2Log("addContact(contact)传入的contact:%d不能为空", contact);
#endif
                return;
            }

            contactList.pushback(contact);
        }

        f2ContactController::f2ContactController()
        {
            contactGenerator = new f2ContactGenerator();
        }
    }
}