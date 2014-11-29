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
            // --!AABB���ټ������ɿ�����ײ��
            void broadPhase(f2List<f2Body*>& bodyList);

            // Narrow-Phase
            // --!SAT��ײ��� ���ɾ�����ײ��Ϣ(��� ��ײ���� �Ӵ��㼯)
            void narrowPhase(float32& dt, math::f2Vector2f& gravity);

            f2List<f2Contact*>& getContactList();

            // ���� / ���contact�б�
            void clearContactList();

        private:
            // broad-phase�Զ����ɿ�����ײ�� �����ֶ�
            void addContact(f2Contact* contact);

            // ����ɾ�� ���ɸ��� �����Լ�����ʵ��
            f2ContactController();
            f2ContactController(const f2ContactController&) {};
            ~f2ContactController() {};
            f2ContactController& operator=(const f2ContactController& generator) const {};

            // pimple
            class f2ContactGenerator;
            f2ContactGenerator* contactGenerator;

            // ������ײ�б�
            f2List<f2Contact*> contactList;
        };
    }
}

#endif