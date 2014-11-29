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

        // 计算转动惯量和质量
        virtual f2MassData& computeMass();

        // 计算AABB
        virtual core::f2AABB& computeAABB();

        uint32 getVertexCount();

        math::f2Vector2f getVertice(uint32 index);

        math::f2Vector2f* getVertices();

        math::f2Vector2f* getNormals();

        math::f2Vector2f getNormal(uint32 index);

    protected:
        virtual void setOrient(float32 radians);

    private:
        // --!可选关闭private访问权限可以提高不必要的开销
        // 多边形线列
        uint32 vertexCount;

        // 顶点列表
        math::f2Vector2f vertices[F2_MAX_POLYVERTEXCOUNT];

        // 面法线
        math::f2Vector2f normals[F2_MAX_POLYVERTEXCOUNT];
    };
}

#endif