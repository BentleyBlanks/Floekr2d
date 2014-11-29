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
        // ����contact����create�ص�
        typedef f2Contact* f2ContactCreateCallback(f2Body* A, f2Body* B, f2PoolAllocator* allocator);

        class f2ContactController::f2ContactGenerator
        {
        public:
            f2ContactGenerator();

            f2Contact* createContact(f2Body* A, f2Body* B);

            void deleteContact(f2Contact* contact);

        private:
            // ��ע�����ע��������״��create�ص�
            void initRegister();

            // �Ƿ����ע��
            bool bRegister;

            // contact�ڴ������
            f2PoolAllocator* contactAllocator;

            // �ص�ע���
            // --!ʵ��Ϊ[count][count]��ά����
            f2ContactCreateCallback* contactRegister[f2Shape::e_typeCount][f2Shape::e_typeCount];
        };

        f2ContactController::f2ContactGenerator::f2ContactGenerator()
        {
            bRegister = false;
            // ��ʼ���ص���ע��
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

            // �˴�����֪��AB������״ ֱ�Ӹ���[][]��Ӧ�ص����ɼ��� ģ��ʵ�ֶ�̬
            return contactRegister[type1][type2](A, B, contactAllocator);
        }

        void f2ContactController::f2ContactGenerator::deleteContact(f2Contact* contact)
        {
            contactAllocator->free(contact, sizeof(f2Contact));
            contact = NULL;
        }

        void f2ContactController::f2ContactGenerator::initRegister()
        {
            // �ϸ�ֻ֤ע��һ��
            if(!bRegister)
            {
                // ע�������״��create����
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
        // --!AABB���ټ������ɿ�����ײ��
        void f2ContactController::broadPhase(f2List<f2Body*>& bodyList)
        {
            //f2Log("broadPhase\n");

            // ѭ������ ����AABB 
            // ���ɿ�����ײ�Ķ�
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
                    
                    // ���߶�Ϊ��̬���岻������ײ���
                    if(inverseMassA == 0 && inverseMassB == 0)
                        continue;

                    
                    //// ��ײ���ܷ���
                    //if(f2AABBAndAABB(&shapeA->computeAABB(), &shapeB->computeAABB()))
                    //{
                    //    // ����һ����ײ��
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
        // --!SAT��ײ��� ���ɾ�����ײ��Ϣ(��� ��ײ���� �Ӵ��㼯)
        void f2ContactController::narrowPhase(float32& dt, math::f2Vector2f& gravity)
        {
            //f2Log("narrowPhase\n");
            //f2Log("contact size:%d\n", contactList.size());

            for(int32 i = 0; i < contactList.size(); i++)
            {
                f2Contact* contact = contactList[i];
                // --!����֪��contact��body�������ͼ��ɵ���
                if(!contact->collide())
                {
                    // ��ײδ����
                    contactGenerator->deleteContact(contact);
                    // --!ɾ���ķ�ʽ����
                    // ��contactList��ɾȥ
                    contactList.remove(i);
                }
                else
                {
                    core::f2Contact* contact = contactList[i];
                    core::f2Manifold* m = contact->getManifold();
                    f2Body* A = contact->A, *B = contact->B;

                    // ʵ�ʻָ�ϵ��Ϊ��С��һ��
                    m->e = std::min(A->restitution, B->restitution);

                    // Ħ��ϵ��Ϊ����ƽ��
                    m->staticFriction = std::sqrt(A->staticFriction * B->staticFriction);
                    m->dynamicFriction = std::sqrt(A->dynamicFriction * B->dynamicFriction);

                    for(int32 i = 0; i < m->touchPointsNum; i++)
                    {
                        // �����ĵ���ײ��
                        math::f2Vector2f ra = m->touchPoints[i] - A->getPosition();
                        math::f2Vector2f rb = m->touchPoints[i] - B->getPosition();

                        // �����ٶ�����ٶȵĺ�(ʵ�����ٶ�)
                        // --!�˴�cross�������ǽ�rb�Ĵ�ֱ������angularVelocity����
                        math::f2Vector2f relativeV = B->getLinearVelocity() + f2Cross(B->getAngularVelocity(), rb) -
                            A->getLinearVelocity() - f2Cross(A->getAngularVelocity(), ra);

                        // �Ƿ�Ӧ�ûָ�ϵ�� ȡ���ڷ�������������������Ƿ�ֻ������ 
                        // ������ô��������ṩ�κλָ�ϵ����ʹ�����Ƿֿ�
                        // rvΪ����֮����ٶȲ�
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

        // ���� / ���contact�б�
        void f2ContactController::clearContactList()
        {
            //f2Log("clearContactList\n");
            for(int32 i = 0; i < contactList.size(); i++)
            {
                contactGenerator->deleteContact(contactList[i]);
            }
            contactList.clear();
        }

        // broad-phase�Զ����ɿ�����ײ�� �����ֶ�
        void f2ContactController::addContact(f2Contact* contact)
        {
            if(contact == NULL)
            {
#ifdef _DEBUG
                f2Log("addContact(contact)�����contact:%d����Ϊ��", contact);
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