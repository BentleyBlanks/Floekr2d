#ifndef F2_CONTACTCONTROLLER_H
#define F2_CONTACTCONTROLLER_H

#include <Floekr2d/protected/datastruct/f2List.h>
#include <Floekr2d/protected/math/f2Vector2.h>

namespace Floekr2d
{
    class f2Body;
    namespace core
    {
        class f2Contact;
        class f2ContactController
        {
        public:
            static f2ContactController& getContactGenerator();

            // Broad-Phase
            // --!AABB快速检碰生成可能碰撞对
            void broadPhase(f2List<f2Body*>& bodyList);

            // Narrow-Phase
            // --!SAT碰撞检测 生成具体碰撞信息(深度 碰撞法线 接触点集)
            void narrowPhase(float32& dt, math::f2Vector2f& gravity);

            f2List<f2Contact*>& getContactList();

            // 重置 / 清除contact列表
            void clearContactList();

        private:
            // broad-phase自动生成可能碰撞对 无需手动
            void addContact(f2Contact* contact);

            // 不可删除 不可复制 不可自己创建实例
            f2ContactController();
            f2ContactController(const f2ContactController&) {};
            ~f2ContactController() {};
            f2ContactController& operator=(const f2ContactController& generator) const {};

            // pimple
            class f2ContactGenerator;
            f2ContactGenerator* contactGenerator;

            // 可能碰撞列表
            f2List<f2Contact*> contactList;
        };
    }
}

#endif