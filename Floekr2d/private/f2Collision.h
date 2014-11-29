#ifndef F2_COLLISION_H
#define F2_COLLISION_H

#include <Floekr2d/protected/f2Settings.h>
#include <Floekr2d/protected/math/f2Vector2.h>

// 碰撞中基本数据类型
using namespace Floekr2d::math;

namespace Floekr2d
{
    class f2CircleShape;
    class f2PolygonShape;

    namespace core
    {
        class f2AABB
        {
        public:
            f2AABB(){ min.zero(); max.zero(); }

            //左上角右下角
            f2Vector2f min;
            f2Vector2f max;
        };

        // 碰撞信息集合体
        class f2Manifold
        {
        public:
            f2Manifold() :penetration(0.0f), touchPointsNum(0), e(0.0f)
            {
                normal.zero();
                touchPoints[0].zero();
                touchPoints[1].zero();
            }

            // 实际碰撞中计算使用到的恢复系数
            // 大小与两碰撞对象有关
            float32 e;
            // 实际静摩擦系数
            float32 staticFriction;
            // 实际滑动摩擦系数
            float32 dynamicFriction;
            // 碰撞深度
            float penetration;
            // 碰撞法线
            f2Vector2f normal;
            // 接触点(最多两个接触点)
            f2Vector2f touchPoints[2];
            int touchPointsNum;
        };


        /*
        // 两形状之间计算碰撞manifold
        void f2CircletoCircle(f2Manifold* m, const f2Body* A, const f2Body* B);

        void f2CircletoPolygon(f2Manifold* m, const f2Body* A, const f2Body* B);

        void f2PolygontoCircle(f2Manifold* m, const f2Body* A, const f2Body* B);

        void f2PolygontoPolygon(f2Manifold* m, const f2Body* A, const f2Body* B);

        // --!
        float32 f2FindMaxSeparation(uint32* faceIndex, const f2Body* A, const f2Body* B);

        void f2FindIncidentFace(f2Vector2f *v, const f2Body *referencePoly, const f2Body *incidentPoly, const uint32 referenceIndex);
        
        int32 f2Clip(const f2Vector2f n, const float32 c, f2Vector2f *face);
        */

        // --!增加bool返回值不浪费计算结果 并能够筛选正确contact对
        bool f2CircletoCircle(f2Manifold* m, f2CircleShape* A, f2CircleShape* B);

        bool f2CircletoPolygon(f2Manifold* m, f2CircleShape* A, f2PolygonShape* B);

        bool f2PolygontoCircle(f2Manifold* m, f2PolygonShape* A, f2CircleShape* B);

        bool f2PolygontoPolygon(f2Manifold* m, f2PolygonShape* A, f2PolygonShape* B);

        bool f2AABBAndAABB(f2AABB *a, f2AABB *b);

        // --!
        float32 f2FindMaxSeparation(uint32* faceIndex, f2PolygonShape* A, f2PolygonShape* B);

        void f2FindIncidentFace(f2Vector2f *v, f2PolygonShape *referencePoly, f2PolygonShape *incidentPoly, uint32 referenceIndex);

        int32 f2Clip(f2Vector2f n, float32 c, f2Vector2f *face);

        f2Vector2f getSupport(f2PolygonShape* shape, f2Vector2f& dir);

    }

    /*
    // --!以下都为临时Debug代码 f2Body f2Manifold
    // --!f2Body实际位置应该为f2Shape实际类型
    class f2Body
    {
    public:
        f2Body(float32 radius, f2Vector2f position) :radius(radius), position(position) 
        {
            u.setIdentity();
        }

        // 抄袭的
        void set(f2Vector2f *_vertices, uint32 count)
        {
            // No hulls with less than 3 vertices (ensure actual polygon)
            assert(count > 2 && count <= MaxPolyVertexCount);
            count = std::min((int32) count, MaxPolyVertexCount);

            // Find the right most point on the hull
            int32 rightMost = 0;
            float32 highestXCoord = _vertices[0].x;
            for(uint32 i = 1; i < count; ++i)
            {
                float32 x = _vertices[i].x;
                if(x > highestXCoord)
                {
                    highestXCoord = x;
                    rightMost = i;
                }

                // If matching x then take farthest negative y
                else if(x == highestXCoord)
                if(_vertices[i].y < _vertices[rightMost].y)
                    rightMost = i;
            }

            int32 hull[MaxPolyVertexCount];
            int32 outCount = 0;
            int32 indexHull = rightMost;

            for(;;)
            {
                hull[outCount] = indexHull;

                // Search for next index that wraps around the hull
                // by computing cross products to find the most counter-clockwise
                // vertex in the set, given the previos hull index
                int32 nextHullIndex = 0;
                for(int32 i = 1; i < (int32) count; ++i)
                {
                    // Skip if same coordinate as we need three unique
                    // points in the set to perform a cross product
                    if(nextHullIndex == indexHull)
                    {
                        nextHullIndex = i;
                        continue;
                    }

                    // Cross every set of three unique vertices
                    // Record each counter clockwise third vertex and add
                    // to the output hull
                    // See : http://www.oocities.org/pcgpe/math2d.html
                    f2Vector2f e1 = _vertices[nextHullIndex] - _vertices[hull[outCount]];
                    f2Vector2f e2 = _vertices[i] - _vertices[hull[outCount]];
                    float32 c = e1.cross(e2);
                    if(c < 0.0f)
                        nextHullIndex = i;

                    // Cross product is zero then e vectors are on same line
                    // therefor want to record vertex farthest along that line
                    if(c == 0.0f && e2.getLengthSquare() > e1.getLengthSquare())
                        nextHullIndex = i;
                }

                ++outCount;
                indexHull = nextHullIndex;

                // Conclude algorithm upon wrap-around
                if(nextHullIndex == rightMost)
                {
                    vertexCount = outCount;
                    break;
                }
            }

            // Copy vertices into shape's vertices
            for(uint32 i = 0; i < vertexCount; ++i)
                vertices[i] = _vertices[hull[i]];

            // Compute face normals
            for(uint32 i1 = 0; i1 < vertexCount; ++i1)
            {
                uint32 i2 = i1 + 1 < vertexCount ? i1 + 1 : 0;
                f2Vector2f face = vertices[i2] - vertices[i1];

                // Ensure no zero-length edges, because that's bad
                assert(face.getLengthSquare() > F2_EPSILON * F2_EPSILON);

                // Calculate normal with 2D cross product between vector and scalar
                normals[i1] = f2Vector2f(face.y, -face.x);
                normals[i1].normalize();
            }

            // Calculate centroid and moment of interia
            f2Vector2f c(0.0f, 0.0f); // centroid
            float32 area = 0.0f;
            float32 I = 0.0f;
            const float32 k_inv3 = 1.0f / 3.0f;

            for(uint32 i1 = 0; i1 < vertexCount; ++i1)
            {
                // Triangle vertices, third vertex implied as (0, 0)
                f2Vector2f p1(vertices[i1]);
                uint32 i2 = i1 + 1 < vertexCount ? i1 + 1 : 0;
                f2Vector2f p2(vertices[i2]);

                float32 D = p1.cross(p2);
                float32 triangleArea = 0.5f * D;

                area += triangleArea;

                // Use area to weight the centroid average, not just vertex position
                c += triangleArea * k_inv3 * (p1 + p2);

                float32 intx2 = p1.x * p1.x + p2.x * p1.x + p2.x * p2.x;
                float32 inty2 = p1.y * p1.y + p2.y * p1.y + p2.y * p2.y;
                I += (0.25f * k_inv3 * D) * (intx2 + inty2);
            }

            c *= 1.0f / area;

            // Translate vertices to centroid (make the centroid (0, 0)
            // for the polygon in model space)
            // Not really necessary, but I like doing this anyway
            for(uint32 i = 0; i < vertexCount; ++i)
                vertices[i] -= c;
        }

        f2Vector2f position;
        float32 radius;

        uint32 vertexCount;
        f2Vector2f vertices[MaxPolyVertexCount];
        f2Vector2f normals[MaxPolyVertexCount];

        f2Matrix2f u; // Orientation matrix from model to world
    };
    */

}
#endif