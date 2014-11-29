#include <Floekr2d/private/f2Collision.h>

// shape
#include <Floekr2d/public/shape/f2CircleShape.h>
#include <Floekr2d/public/shape/f2PolygonShape.h>

namespace Floekr2d
{
    namespace core
    {
        bool f2CircletoCircle(f2Manifold* m, f2CircleShape* A, f2CircleShape* B)
        {
            // 碰撞法线
            f2Vector2f normal = B->getPosition() - A->getPosition();
            float32 radiusSum = A->getRadius() + B->getRadius();

            float32 distanceSquare = normal.getLengthSquare();

            if(distanceSquare >= radiusSum * radiusSum)
            {
                // 无碰撞发生
                m->touchPointsNum = 0;
                return false;
            }

            // 确认有一个碰撞点
            m->touchPointsNum = 1;

            float32 distance = std::sqrt(distanceSquare);
            // 圆AB重合
            if(distance == 0)
            {
                // 随机选择一个方向作为分离方向(单位向量)
                m->normal = f2Vector2f(1, 0);
                // 深度为半径之和
                m->penetration = radiusSum;
                m->touchPoints[0] = A->getPosition();
            }
            else
            {
                m->penetration = radiusSum - distance;
                // 减少一次sqrt()的调用
                // m->normal.normalize();
                // 转换为单位向量
                m->normal = normal / distance;
                // 接触点为：圆A与碰撞法线的交点
                m->touchPoints[0] = m->normal * A->getRadius() + A->getPosition();
            }

            return true;
        }

        bool f2CircletoPolygon(f2Manifold* m, f2CircleShape* A, f2PolygonShape* B)
        {
            //                 |                |
            // ___edgeRegion___|___faceRegion___|___edgeRegion___
            //                 v1               v2

            f2Vector2f center = A->getPosition();
            // 将A移动到B的模型空间中 这样就可以使用位于B模型空间存放的面法线和顶点坐标数据了
            // 先平移后旋转(B朝向矩阵的转置)
            center = B->getOrient().getTranspose() * (center - B->getPosition());

            // 根据圆心到边的位置在面法线上投影的长短 找到距离最近的那个面
            // 记录 最大投影值 与 面法线序号
            float32 projection = -FLT_MAX;
            uint32 faceNormal = 0;
            for(uint32 i = 0; i < B->getVertexCount(); i++)
            {
                // 当前顶点到圆的向量在面法线上的投影
                float32 p = B->getNormal(i) * (center - B->getVertice(i));

                // 一般情况只有1~3个面的面法线投影为正(在多边形面数不是特别多的情况下)
                if(p > A->getRadius())
                    return false;

                if(p > projection)
                {
                    projection = p;
                    faceNormal = i;
                }
            }

            f2Vector2f v1 = B->getVertice(faceNormal);
            // 下一点(防止溢出最大顶点数目)
            uint32 i2 = faceNormal + 1 < B->getVertexCount() ? faceNormal + 1 : 0;
            f2Vector2f v2 = B->getVertice(i2);

            // 根据圆心与点的向量在发现上投影的正负 来检查圆心是否在多边形内部
            if(projection < F2_EPSILON)
            {
                m->touchPointsNum = 1;
                // 碰撞法线指向分离方向的反方向
                m->normal = -(B->getOrient() * B->getNormal(faceNormal));
                m->touchPoints[0] = m->normal * A->getRadius() + A->getPosition();
                // 临界值就是圆心刚刚在边上 此时渗透深度为radius 此处简化操作
                m->penetration = A->getRadius();
            }

            // 判断接近边两端的哪一端
            float32 dot1 = (center - v1) * (v2 - v1);
            float32 dot2 = (center - v2) * (v1 - v2);
            m->penetration = A->getRadius() - projection;

            // edge region
            // cosThea是否小于0表明(theta > 90)在edge region内
            // 处于edge region内接触点 / 距离圆心最近点 就是端点本身
            if(dot1 <= 0.0f)
            {
                // 无碰撞发生
                if(center.getDistance(v1) > A->getRadius() * A->getRadius())
                    return false;

                m->touchPointsNum = 1;
                // 碰撞法线指向分离方向的反方向
                f2Vector2f nTemp = v1 - center;
                nTemp = B->getOrient() * nTemp;
                nTemp.normalize();
                m->normal = nTemp;
                // 转换到世界空间坐标
                v1 = B->getOrient() * v1 + B->getPosition();
                m->touchPoints[0] = v1;
            }
            else if(dot2 <= 0.0f)
            {
                // 无碰撞发生
                if(center.getDistance(v1) > A->getRadius() * A->getRadius())
                    return false;

                m->touchPointsNum = 1;
                // 碰撞法线指向分离方向的反方向
                f2Vector2f nTemp = v2 - center;
                nTemp = B->getOrient() * nTemp;
                nTemp.normalize();
                m->normal = nTemp;
                // 转换到世界空间坐标
                v2 = B->getOrient() * v2 + B->getPosition();
                m->touchPoints[0] = v2;
            }
            // face region
            else
            {
                f2Vector2f n = B->getNormal(faceNormal);
                float32 projectionTemp = (center - v1) * n;
                if(projectionTemp > A->getRadius())
                    return false;

                n = B->getOrient() * n;
                m->normal = -n;
                m->touchPoints[0] = m->normal * A->getRadius() + A->getPosition();
                m->touchPointsNum = 1;
            }

            return true;
        }

        bool f2PolygontoCircle(f2Manifold* m, f2PolygonShape* A, f2CircleShape* B)
        {
            bool collided = f2CircletoPolygon(m, B, A);
            m->normal = -m->normal;

            return collided;
        }

        f2Vector2f getSupport(f2PolygonShape* shape, f2Vector2f& dir)
        {
            float32 bestProjection = -FLT_MAX;
            f2Vector2f bestVertex;

            for(uint32 i = 0; i < shape->getVertexCount(); ++i)
            {
                f2Vector2f v = shape->getVertice(i);
                // 当前顶点在给定方向上的投影
                float32 projection = v*dir;

                // 遍历最大投影值
                if(projection > bestProjection)
                {
                    bestVertex = v;
                    bestProjection = projection;
                }
            }
            return bestVertex;
        }

        float32 f2FindMaxSeparation(uint32* faceIndex, f2PolygonShape* A, f2PolygonShape* B)
        {
            float32 maxSeparation = -FLT_MAX;
            uint32 bestIndex;

            // 循环遍历A的所有面法线
            for(uint32 i = 0; i < A->getVertexCount(); i++)
            {
                f2Vector2f n = A->getNormal(i);
                f2Vector2f nWorld = A->getOrient() * n;

                // A面法向量转换到B模型空间
                f2Matrix2f bTranspose = B->getOrient().getTranspose();
                n = bTranspose * nWorld;

                // 在A面法线相反方向上找在B上的支点
                f2Vector2f supportPoint = getSupport(B, -n);

                // A各顶点转换至B的模型坐标系中
                f2Vector2f v = A->getVertice(i);
                v = A->getOrient() * v + A->getPosition();
                v -= B->getPosition();
                v = bTranspose * v;

                // 因为找到的s是在-n下最大深度的 倘若两多边形相交 那么与n的点积一定小于0 
                // 尽管距离最大 但是依旧是小于0的
                float32 separation = n * (supportPoint - v);

                if(separation > maxSeparation)
                {
                    maxSeparation = separation;
                    bestIndex = i;
                }
            }

            *faceIndex = bestIndex;
            return maxSeparation;
        }

        void f2FindIncidentFace(f2Vector2f *v, f2PolygonShape *referencePoly, f2PolygonShape *incidentPoly, uint32 referenceIndex)
        {
            f2Vector2f referenceNormal = referencePoly->getNormal(referenceIndex);

            // reference的法向量转换到其自身世界坐标系
            referenceNormal = referencePoly->getOrient() * referenceNormal;
            // 转换到incident模型坐标系
            referenceNormal = incidentPoly->getOrient().getTranspose() * referenceNormal;

            // 在事件多边形中找到与referenceFace面法线投影最小的(带符号)
            int32 incidentFace = 0;
            float32 minDot = FLT_MAX;
            for(uint32 i = 0; i < incidentPoly->getVertexCount(); ++i)
            {
                // 在给定方向上的投影
                float32 dot = referenceNormal * incidentPoly->getNormal(i);
                if(dot < minDot)
                {
                    minDot = dot;
                    incidentFace = i;
                }
            }

            // 保留incidentFace的面顶点
            v[0] = incidentPoly->getOrient() * incidentPoly->getVertice(incidentFace) + incidentPoly->getPosition();
            incidentFace = incidentFace + 1 >= (int32) incidentPoly->getVertexCount() ? 0 : incidentFace + 1;
            v[1] = incidentPoly->getOrient() * incidentPoly->getVertice(incidentFace) + incidentPoly->getPosition();
        }

        int32 f2Clip(f2Vector2f n, float32 c, f2Vector2f *face)
        {
            // 裁剪后的点数
            uint32 clipPointNum = 0;
            // 裁剪后的点
            f2Vector2f result[2] = {face[0], face[1]};

            // 事实情况点(x0, y0)与给定的直线之间的距离是
            //      |ax0 + by0 + c|
            // d = -----------------
            //      (a^2 + b^2)^1/2
            // 因为分母始终大于0所以这里就省略为 d = ax + by + c 
            // 根据d的符号来判断与直线之间的位置关系(这里c是+-主要取决于外部的c的计算方式)
            // 下面的计算内容计算的是与该直线垂直的直线 而非直线本身(即为裁剪线)
            // 可以用非常多的方式推导 其中比较简单的方法为 化简到最简 得到
            float32 d0 = n.dot(face[0]) - c;
            float32 d1 = n.dot(face[1]) - c;

            // <0代表在裁剪面内部
            // --算法来源于Sutherland-Hodgman Algorithm
            if(d0 <= 0.0f)
                result[clipPointNum++] = face[0];
            if(d1 <= 0.0f)
                result[clipPointNum++] = face[1];

            // 两点位于交点的两侧
            if(d0 * d1 < 0.0f)
            {
                // 两直线交点也是结果一部分
                float32 lambda = d0 / (d0 - d1);
                result[clipPointNum] = face[0] + lambda * (face[1] - face[0]);
                ++clipPointNum;
            }

            face[0] = result[0];
            face[1] = result[1];

            f2Assert(clipPointNum != 3);

            return clipPointNum;
        }

        bool f2PolygontoPolygon(f2Manifold* m, f2PolygonShape* A, f2PolygonShape* B)
        {
            // 初始化接触点
            m->touchPointsNum = 0;

            // 最短深度(数值最大)的面序号
            uint32 faceA;
            // 通过a上的法向量寻找b上的支点
            float32 penetrationA = f2FindMaxSeparation(&faceA, A, B);
            if(penetrationA >= 0.0f)
                // 碰撞没有发生
                return false;

            uint32 faceB;
            float32 penetrationB = f2FindMaxSeparation(&faceB, B, A);
            if(penetrationB >= 0.0f)
                // 碰撞没有发生
                return false;

            // 碰撞检测完毕 之后生成碰撞的深度及其他信息
            uint32 referenceIndex;
            bool flip;

            // --!
            f2PolygonShape *referencePoly;
            f2PolygonShape *incidentPoly;

            // 决定哪个多边形包含了参照面(深度大的包含参考面) 深度较小(数值较大--小于0)的可以作为后期用来分离两个多边形
            // 这里的深度一定小于0 在面法线上的投影越长越小
            // A>B?提供一定容忍度
            //const float32 tolerance = 0.1f * F2_LINEARSLOP;
            //if(penetrationA > penetrationB + tolerance)
            if(f2Greater(penetrationA, penetrationB))
            {
                referencePoly = A;
                incidentPoly = B;
                referenceIndex = faceA;
                flip = false;
            }
            else
            {
                referencePoly = B;
                incidentPoly = A;
                referenceIndex = faceB;
                flip = true;
            }

            // 找到碰撞发生面
            f2Vector2f incidentFace[2];
            f2FindIncidentFace(incidentFace, referencePoly, incidentPoly, referenceIndex);

            // 参考面顶点
            f2Vector2f v1 = referencePoly->getVertice(referenceIndex);
            // 循环
            referenceIndex = referenceIndex + 1 == referencePoly->getVertexCount() ? 0 : referenceIndex + 1;
            f2Vector2f v2 = referencePoly->getVertice(referenceIndex);

            // 转换到参考多边形的世界坐标系中
            v1 = referencePoly->getOrient() * v1 + referencePoly->getPosition();
            v2 = referencePoly->getOrient() * v2 + referencePoly->getPosition();

            // 在世界坐标系中计算参照多边形的边向量
            f2Vector2f sidePlaneNormal = (v2 - v1);
            // 这里的normal表示后期计算时操作的是垂直于sidePlane的直线而其本身代表的还是边(v2 - v1)
            sidePlaneNormal.normalize();

            //得到边的法线向量(以当前向量方向为基准 顺时针旋转90度得到refNormal)
            f2Vector2f refFaceNormal(sidePlaneNormal.y, -sidePlaneNormal.x);

            // ax + by = c
            // 联立两点式直线方程
            // c = x2y1 - x1y2
            // a = y2 - y1
            // b = x1 - x2
            // 这条直线过v1 v2点
            float32 referenceC = refFaceNormal * v1;

            //           ->n       ^
            //      +---c ------positivePlane--clip
            //      | i |\
                    //      +---+ c-----negetivePlane--clip
            //             \       v
            //              r
            //
            //  r : 参考面
            //  i : 事件多边形
            //  c : 裁剪点
            //  n : 事件面法线

            // 裁剪线直线方程的c
            float32 negetiveC = -(sidePlaneNormal * v1);
            float32 positiveC = sidePlaneNormal * v2;

            // 在相对面上裁剪
            if(f2Clip(-sidePlaneNormal, negetiveC, incidentFace) < 2)
                return false;

            // 相向面裁剪
            if(f2Clip(sidePlaneNormal, positiveC, incidentFace) < 2)
                return false;

            // 是否翻转参考面面法线
            m->normal = flip ? -refFaceNormal : refFaceNormal;

            // 保持点在参考面后方
            uint32 contactNum = 0;
            // 这里得到的实际方程式为ax+by+c 也就是经过v1 v2点的直线 含义为d = ax+by+c尽管这里的d 还应该除以(a^2+b^2)^1/2不过猜测应该是为了提高计算速度有关系
            float32 separation = refFaceNormal * incidentFace[0] - referenceC;
            if(separation <= 0.0f)
            {
                m->touchPoints[contactNum] = incidentFace[0];
                // 深度为负值
                m->penetration = -separation;
                ++contactNum;
            }
            else
                m->penetration = 0;

            separation = refFaceNormal * incidentFace[1] - referenceC;
            if(separation <= 0.0f)
            {
                m->touchPoints[contactNum] = incidentFace[1];

                m->penetration += -separation;
                ++contactNum;

                // 平均两接触点的接触深度
                m->penetration /= (float32) contactNum;
            }

            m->touchPointsNum = contactNum;

            return true;
        }

        bool f2AABBAndAABB(f2AABB *a, f2AABB *b)
        {
            //未发现交叠部分(加快判断速度)
            if(a->max.x < b->min.x || a->min.x > b->max.x) return false;
            if(a->max.y < b->min.y || a->min.y > b->max.y) return false;

            //找到重叠部分
            return true;
        }
    }
}