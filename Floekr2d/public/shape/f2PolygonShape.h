#ifndef F2_POLYGON_SHAPE_H
#define F2_POLYGON_SHAPE_H

#include <Floekr2d/public/shape/f2Shape.h>
#include <Floekr2d/protected/math/f2Vector2.h>

namespace Floekr2d
{
    class f2PolygonShape:public f2Shape
    {
    public:
        f2PolygonShape();
        ~f2PolygonShape(){}

        virtual void init(float32 density = 1.0f);

        void setVertices(math::f2Vector2f *userVertices, uint32 count);

        void setAsBox(float32 halfWidth, float32 halfHeight);

        // ����ת������������
        virtual f2MassData& computeMass();

        // ����AABB
        virtual core::f2AABB& computeAABB();

        uint32 getVertexCount();

        math::f2Vector2f getVertice(uint32 index);

        math::f2Vector2f* getVertices();

        math::f2Vector2f* getNormals();

        math::f2Vector2f getNormal(uint32 index);

    protected:
        virtual void setOrient(float32 radians);

    private:
        // --!��ѡ�ر�private����Ȩ�޿�����߲���Ҫ�Ŀ���
        // ���������
        uint32 vertexCount;

        // �����б�
        math::f2Vector2f vertices[F2_MAX_POLYVERTEXCOUNT];

        // �淨��
        math::f2Vector2f normals[F2_MAX_POLYVERTEXCOUNT];
    };
}

#endif