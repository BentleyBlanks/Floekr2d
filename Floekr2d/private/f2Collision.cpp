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
            // ��ײ����
            f2Vector2f normal = B->getPosition() - A->getPosition();
            float32 radiusSum = A->getRadius() + B->getRadius();

            float32 distanceSquare = normal.getLengthSquare();

            if(distanceSquare >= radiusSum * radiusSum)
            {
                // ����ײ����
                m->touchPointsNum = 0;
                return false;
            }

            // ȷ����һ����ײ��
            m->touchPointsNum = 1;

            float32 distance = std::sqrt(distanceSquare);
            // ԲAB�غ�
            if(distance == 0)
            {
                // ���ѡ��һ��������Ϊ���뷽��(��λ����)
                m->normal = f2Vector2f(1, 0);
                // ���Ϊ�뾶֮��
                m->penetration = radiusSum;
                m->touchPoints[0] = A->getPosition();
            }
            else
            {
                m->penetration = radiusSum - distance;
                // ����һ��sqrt()�ĵ���
                // m->normal.normalize();
                // ת��Ϊ��λ����
                m->normal = normal / distance;
                // �Ӵ���Ϊ��ԲA����ײ���ߵĽ���
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
            // ��A�ƶ���B��ģ�Ϳռ��� �����Ϳ���ʹ��λ��Bģ�Ϳռ��ŵ��淨�ߺͶ�������������
            // ��ƽ�ƺ���ת(B��������ת��)
            center = B->getOrient().getTranspose() * (center - B->getPosition());

            // ����Բ�ĵ��ߵ�λ�����淨����ͶӰ�ĳ��� �ҵ�����������Ǹ���
            // ��¼ ���ͶӰֵ �� �淨�����
            float32 projection = -FLT_MAX;
            uint32 faceNormal = 0;
            for(uint32 i = 0; i < B->getVertexCount(); i++)
            {
                // ��ǰ���㵽Բ���������淨���ϵ�ͶӰ
                float32 p = B->getNormal(i) * (center - B->getVertice(i));

                // һ�����ֻ��1~3������淨��ͶӰΪ��(�ڶ�������������ر��������)
                if(p > A->getRadius())
                    return false;

                if(p > projection)
                {
                    projection = p;
                    faceNormal = i;
                }
            }

            f2Vector2f v1 = B->getVertice(faceNormal);
            // ��һ��(��ֹ�����󶥵���Ŀ)
            uint32 i2 = faceNormal + 1 < B->getVertexCount() ? faceNormal + 1 : 0;
            f2Vector2f v2 = B->getVertice(i2);

            // ����Բ�����������ڷ�����ͶӰ������ �����Բ���Ƿ��ڶ�����ڲ�
            if(projection < F2_EPSILON)
            {
                m->touchPointsNum = 1;
                // ��ײ����ָ����뷽��ķ�����
                m->normal = -(B->getOrient() * B->getNormal(faceNormal));
                m->touchPoints[0] = m->normal * A->getRadius() + A->getPosition();
                // �ٽ�ֵ����Բ�ĸո��ڱ��� ��ʱ��͸���Ϊradius �˴��򻯲���
                m->penetration = A->getRadius();
            }

            // �жϽӽ������˵���һ��
            float32 dot1 = (center - v1) * (v2 - v1);
            float32 dot2 = (center - v2) * (v1 - v2);
            m->penetration = A->getRadius() - projection;

            // edge region
            // cosThea�Ƿ�С��0����(theta > 90)��edge region��
            // ����edge region�ڽӴ��� / ����Բ������� ���Ƕ˵㱾��
            if(dot1 <= 0.0f)
            {
                // ����ײ����
                if(center.getDistance(v1) > A->getRadius() * A->getRadius())
                    return false;

                m->touchPointsNum = 1;
                // ��ײ����ָ����뷽��ķ�����
                f2Vector2f nTemp = v1 - center;
                nTemp = B->getOrient() * nTemp;
                nTemp.normalize();
                m->normal = nTemp;
                // ת��������ռ�����
                v1 = B->getOrient() * v1 + B->getPosition();
                m->touchPoints[0] = v1;
            }
            else if(dot2 <= 0.0f)
            {
                // ����ײ����
                if(center.getDistance(v1) > A->getRadius() * A->getRadius())
                    return false;

                m->touchPointsNum = 1;
                // ��ײ����ָ����뷽��ķ�����
                f2Vector2f nTemp = v2 - center;
                nTemp = B->getOrient() * nTemp;
                nTemp.normalize();
                m->normal = nTemp;
                // ת��������ռ�����
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
                // ��ǰ�����ڸ��������ϵ�ͶӰ
                float32 projection = v*dir;

                // �������ͶӰֵ
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

            // ѭ������A�������淨��
            for(uint32 i = 0; i < A->getVertexCount(); i++)
            {
                f2Vector2f n = A->getNormal(i);
                f2Vector2f nWorld = A->getOrient() * n;

                // A�淨����ת����Bģ�Ϳռ�
                f2Matrix2f bTranspose = B->getOrient().getTranspose();
                n = bTranspose * nWorld;

                // ��A�淨���෴����������B�ϵ�֧��
                f2Vector2f supportPoint = getSupport(B, -n);

                // A������ת����B��ģ������ϵ��
                f2Vector2f v = A->getVertice(i);
                v = A->getOrient() * v + A->getPosition();
                v -= B->getPosition();
                v = bTranspose * v;

                // ��Ϊ�ҵ���s����-n�������ȵ� ������������ཻ ��ô��n�ĵ��һ��С��0 
                // ���ܾ������ ����������С��0��
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

            // reference�ķ�����ת������������������ϵ
            referenceNormal = referencePoly->getOrient() * referenceNormal;
            // ת����incidentģ������ϵ
            referenceNormal = incidentPoly->getOrient().getTranspose() * referenceNormal;

            // ���¼���������ҵ���referenceFace�淨��ͶӰ��С��(������)
            int32 incidentFace = 0;
            float32 minDot = FLT_MAX;
            for(uint32 i = 0; i < incidentPoly->getVertexCount(); ++i)
            {
                // �ڸ��������ϵ�ͶӰ
                float32 dot = referenceNormal * incidentPoly->getNormal(i);
                if(dot < minDot)
                {
                    minDot = dot;
                    incidentFace = i;
                }
            }

            // ����incidentFace���涥��
            v[0] = incidentPoly->getOrient() * incidentPoly->getVertice(incidentFace) + incidentPoly->getPosition();
            incidentFace = incidentFace + 1 >= (int32) incidentPoly->getVertexCount() ? 0 : incidentFace + 1;
            v[1] = incidentPoly->getOrient() * incidentPoly->getVertice(incidentFace) + incidentPoly->getPosition();
        }

        int32 f2Clip(f2Vector2f n, float32 c, f2Vector2f *face)
        {
            // �ü���ĵ���
            uint32 clipPointNum = 0;
            // �ü���ĵ�
            f2Vector2f result[2] = {face[0], face[1]};

            // ��ʵ�����(x0, y0)�������ֱ��֮��ľ�����
            //      |ax0 + by0 + c|
            // d = -----------------
            //      (a^2 + b^2)^1/2
            // ��Ϊ��ĸʼ�մ���0���������ʡ��Ϊ d = ax + by + c 
            // ����d�ķ������ж���ֱ��֮���λ�ù�ϵ(����c��+-��Ҫȡ�����ⲿ��c�ļ��㷽ʽ)
            // ����ļ������ݼ���������ֱ�ߴ�ֱ��ֱ�� ����ֱ�߱���(��Ϊ�ü���)
            // �����÷ǳ���ķ�ʽ�Ƶ� ���бȽϼ򵥵ķ���Ϊ ������� �õ�
            float32 d0 = n.dot(face[0]) - c;
            float32 d1 = n.dot(face[1]) - c;

            // <0�����ڲü����ڲ�
            // --�㷨��Դ��Sutherland-Hodgman Algorithm
            if(d0 <= 0.0f)
                result[clipPointNum++] = face[0];
            if(d1 <= 0.0f)
                result[clipPointNum++] = face[1];

            // ����λ�ڽ��������
            if(d0 * d1 < 0.0f)
            {
                // ��ֱ�߽���Ҳ�ǽ��һ����
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
            // ��ʼ���Ӵ���
            m->touchPointsNum = 0;

            // ������(��ֵ���)�������
            uint32 faceA;
            // ͨ��a�ϵķ�����Ѱ��b�ϵ�֧��
            float32 penetrationA = f2FindMaxSeparation(&faceA, A, B);
            if(penetrationA >= 0.0f)
                // ��ײû�з���
                return false;

            uint32 faceB;
            float32 penetrationB = f2FindMaxSeparation(&faceB, B, A);
            if(penetrationB >= 0.0f)
                // ��ײû�з���
                return false;

            // ��ײ������ ֮��������ײ����ȼ�������Ϣ
            uint32 referenceIndex;
            bool flip;

            // --!
            f2PolygonShape *referencePoly;
            f2PolygonShape *incidentPoly;

            // �����ĸ�����ΰ����˲�����(��ȴ�İ����ο���) ��Ƚ�С(��ֵ�ϴ�--С��0)�Ŀ�����Ϊ���������������������
            // ��������һ��С��0 ���淨���ϵ�ͶӰԽ��ԽС
            // A>B?�ṩһ�����̶�
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

            // �ҵ���ײ������
            f2Vector2f incidentFace[2];
            f2FindIncidentFace(incidentFace, referencePoly, incidentPoly, referenceIndex);

            // �ο��涥��
            f2Vector2f v1 = referencePoly->getVertice(referenceIndex);
            // ѭ��
            referenceIndex = referenceIndex + 1 == referencePoly->getVertexCount() ? 0 : referenceIndex + 1;
            f2Vector2f v2 = referencePoly->getVertice(referenceIndex);

            // ת�����ο�����ε���������ϵ��
            v1 = referencePoly->getOrient() * v1 + referencePoly->getPosition();
            v2 = referencePoly->getOrient() * v2 + referencePoly->getPosition();

            // ����������ϵ�м�����ն���εı�����
            f2Vector2f sidePlaneNormal = (v2 - v1);
            // �����normal��ʾ���ڼ���ʱ�������Ǵ�ֱ��sidePlane��ֱ�߶��䱾�����Ļ��Ǳ�(v2 - v1)
            sidePlaneNormal.normalize();

            //�õ��ߵķ�������(�Ե�ǰ��������Ϊ��׼ ˳ʱ����ת90�ȵõ�refNormal)
            f2Vector2f refFaceNormal(sidePlaneNormal.y, -sidePlaneNormal.x);

            // ax + by = c
            // ��������ʽֱ�߷���
            // c = x2y1 - x1y2
            // a = y2 - y1
            // b = x1 - x2
            // ����ֱ�߹�v1 v2��
            float32 referenceC = refFaceNormal * v1;

            //           ->n       ^
            //      +---c ------positivePlane--clip
            //      | i |\
                    //      +---+ c-----negetivePlane--clip
            //             \       v
            //              r
            //
            //  r : �ο���
            //  i : �¼������
            //  c : �ü���
            //  n : �¼��淨��

            // �ü���ֱ�߷��̵�c
            float32 negetiveC = -(sidePlaneNormal * v1);
            float32 positiveC = sidePlaneNormal * v2;

            // ��������ϲü�
            if(f2Clip(-sidePlaneNormal, negetiveC, incidentFace) < 2)
                return false;

            // ������ü�
            if(f2Clip(sidePlaneNormal, positiveC, incidentFace) < 2)
                return false;

            // �Ƿ�ת�ο����淨��
            m->normal = flip ? -refFaceNormal : refFaceNormal;

            // ���ֵ��ڲο����
            uint32 contactNum = 0;
            // ����õ���ʵ�ʷ���ʽΪax+by+c Ҳ���Ǿ���v1 v2���ֱ�� ����Ϊd = ax+by+c���������d ��Ӧ�ó���(a^2+b^2)^1/2�����²�Ӧ����Ϊ����߼����ٶ��й�ϵ
            float32 separation = refFaceNormal * incidentFace[0] - referenceC;
            if(separation <= 0.0f)
            {
                m->touchPoints[contactNum] = incidentFace[0];
                // ���Ϊ��ֵ
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

                // ƽ�����Ӵ���ĽӴ����
                m->penetration /= (float32) contactNum;
            }

            m->touchPointsNum = contactNum;

            return true;
        }

        bool f2AABBAndAABB(f2AABB *a, f2AABB *b)
        {
            //δ���ֽ�������(�ӿ��ж��ٶ�)
            if(a->max.x < b->min.x || a->min.x > b->max.x) return false;
            if(a->max.y < b->min.y || a->min.y > b->max.y) return false;

            //�ҵ��ص�����
            return true;
        }
    }
}