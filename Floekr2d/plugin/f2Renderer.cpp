#include <Floekr2d/plugin/f2Renderer.h>
#include <Floekr2d/public/f2Body.h>

#include <Floekr2d/public/shape/f2PolygonShape.h>
#include <Floekr2d/public/shape/f2CircleShape.h>

namespace Floekr2d
{
    namespace plugin
    {
        f2Renderer::f2Renderer()
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glEnable(GL_BLEND);

            glEnable(GL_POINT_SMOOTH);

            glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);

            glEnable(GL_LINE_SMOOTH);

            glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);

            glEnable(GL_POLYGON_SMOOTH);

            glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
        }

        f2Renderer& f2Renderer::getRenderer()
        {
            static f2Renderer renderer;
            return renderer;
        }

        void f2Renderer::drawShape(f2Shape* shape)
        {
            f2Shape::shapeType type = shape->getType();
            switch(type)
            {
            case f2Shape::e_circle:
                drawCircle((f2CircleShape*)shape);
                break;

            case f2Shape::e_polygon:
                drawPolygon((f2PolygonShape*)shape);
                break;

            default:
                f2Log("无法找到该body的shape类型:%d 无法绘制", type);
                break;
            }
        }

        void f2Renderer::drawCircle(f2CircleShape* circle)
        {
            // --!以下代码可以直接修改为特定平台的绘制函数

            const uint32 k_segments = 30;

            // 多边形逼近圆形
            glColor3f(230 / 256, 40 / 256, 40 / 256);
            glBegin(GL_LINE_LOOP);
            float32 theta = circle->getRadians();
            float32 inc = PI * 2.0f / (float32) k_segments;
            for(uint32 i = 0; i < k_segments; ++i)
            {
                theta += inc;
                math::f2Vector2f p(std::cos(theta), std::sin(theta));
                p *= circle->getRadius();
                p += circle->getPosition();
                glVertex2f(p.x, p.y);
            }
            glEnd();

            // 一个圆连带一根线 用于判别朝向
            glBegin(GL_LINE_STRIP);
            math::f2Vector2f r(0, 1.0f);
            float32 c = std::cos(circle->getRadians());
            float32 s = std::sin(circle->getRadians());
            r.set(r.x * c - r.y * s, r.x * s + r.y * c);
            r *= circle->getRadius();
            r = r + circle->getPosition();
            glVertex2f(circle->getPosition().x, circle->getPosition().y);
            glVertex2f(r.x, r.y);
            glEnd();
        }

        void f2Renderer::drawPolygon(f2PolygonShape* polygon)
        {
            // 以下代码可以修正为特定平台

            glColor3f(230 / 256, 0 / 256, 0 / 256);
            glBegin(GL_LINE_LOOP);
            for(uint32 i = 0; i < polygon->getVertexCount(); ++i)
            {
                math::f2Vector2f v = polygon->getPosition() + polygon->getOrient() * polygon->getVertice(i);
                glVertex2f(v.x, v.y);
            }
            glEnd();
        }

        void f2Renderer::drawString(const char *s, int32 x, int32 y)
        {
            glColor3f(0.5f, 0.5f, 0.9f);
            glRasterPos2i(x, y);
            uint32 l = (uint32) std::strlen(s);
            for(uint32 i = 0; i < l; ++i)
                glutBitmapCharacter(GLUT_BITMAP_9_BY_15, *(s + i));
        }

    }
}