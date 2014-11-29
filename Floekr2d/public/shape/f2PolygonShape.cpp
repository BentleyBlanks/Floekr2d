#include <Floekr2d/public/shape/f2PolygonShape.h>

using namespace Floekr2d::math;

#define MaxPolyVertexCount 64

namespace Floekr2d
{
    f2PolygonShape::f2PolygonShape()
    {
        init(1.0f);
    }

    void f2PolygonShape::init(float32 density)
    {
        setDensity(density);

        setOrient(0.0f);

        type = e_polygon;

        // 防止被调用时出现初值问题
        radius = 0.0f;

        position.zero();
    }

    void f2PolygonShape::setVertices(f2Vector2f *_vertices, uint32 count)
    {
        // No hulls with less than 3 vertices (ensure actual polygon)
        f2Assert(count > 2 && count <= MaxPolyVertexCount);
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
            f2Assert(face.getLengthSquare() > F2_EPSILON * F2_EPSILON);

            // Calculate normal with 2D cross product between vector and scalar
            normals[i1] = f2Vector2f(face.y, -face.x);
            normals[i1].normalize();
        }
    }

    void f2PolygonShape::setOrient(float32 radians)
    {
        this->radians = radians;
        // 使用弧度更新旋转矩阵
        orient.makeRotationMatrixr(this->radians);
    }

    void f2PolygonShape::setAsBox(float32 halfWidth, float32 halfHeight)
    {
        // --!借鉴自Box2d
        vertexCount = 4;
        vertices[0].set(-halfWidth, -halfHeight);
        vertices[1].set(halfWidth, -halfHeight);
        vertices[2].set(halfWidth, halfHeight);
        vertices[3].set(-halfWidth, halfHeight);
        normals[0].set(0.0f, -1.0f);
        normals[1].set(1.0f, 0.0f);
        normals[2].set(0.0f, 1.0f);
        normals[3].set(-1.0f, 0.0f);
    }

    // 计算转动惯量和质量
    f2MassData& f2PolygonShape::computeMass()
    {
        f2MassData massData;

        // --!借鉴自Impulse Engine
        // Calculate centroid and moment of interia
        math::f2Vector2f c(0.0f, 0.0f); // centroid
        float32 area = 0.0f;
        float32 I = 0.0f;
        const float32 k_inv3 = 1.0f / 3.0f;

        for(uint32 i1 = 0; i1 < vertexCount; ++i1)
        {
            // Triangle vertices, third vertex implied as (0, 0)
            math::f2Vector2f p1(vertices[i1]);
            uint32 i2 = i1 + 1 < vertexCount ? i1 + 1 : 0;
            math::f2Vector2f p2(vertices[i2]);

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


        massData.mass = density * area;
        massData.inertia = I * density;

        return massData;
    }

    core::f2AABB& f2PolygonShape::computeAABB()
    {
        float32 minX = FLT_MAX, minY = FLT_MAX, maxX = -FLT_MAX, maxY = -FLT_MAX;

        // 找到顶点中最大最小xy
        for(uint32 i = 0; i < vertexCount; i++)
        {
            float32 tempX = vertices[i].x, tempY = vertices[i].y;

            // x
            if(tempX < minX)
                minX = tempX;
            if(tempX > maxX)
                maxX = tempX;

            // y
            if(tempY < minY)
                minY = tempY;
            if(tempY > maxY)
                maxY = tempY;
        }

        aabb.min.x = minX + position.x;
        aabb.min.y = minY + position.y;
        aabb.max.x = maxX + position.x;
        aabb.max.y = maxY + position.y;

        return aabb;
    }

    uint32 f2PolygonShape::getVertexCount()
    {
        return vertexCount;
    }

    math::f2Vector2f f2PolygonShape::getVertice(uint32 index)
    {
        if(index > vertexCount)
        {
#ifdef _DEBUG
            //std::cout << "Error:目标序号" << index << "已超过当前拥有最大顶点数量" << std::endl;
            f2Log("Error:目标序号%d已超过当前拥有最大顶点数量", index);
#endif
            return math::f2Vector2fZero;
        }
        else
            return vertices[index];
    }

    math::f2Vector2f* f2PolygonShape::getVertices()
    {
        return vertices;
    }

    math::f2Vector2f f2PolygonShape::getNormal(uint32 index)
    {
        if(index > vertexCount)
        {
#ifdef _DEBUG
            //std::cout << "Error:目标序号" << index << "已超过当前拥有最大法线数量" << std::endl;
            f2Log("Error:目标序号%d已超过当前拥有最大法线数量", index);
#endif
            return math::f2Vector2fZero;
        }
        else
            return normals[index];
    }

    math::f2Vector2f* f2PolygonShape::getNormals()
    {
        return normals;
    }
}
