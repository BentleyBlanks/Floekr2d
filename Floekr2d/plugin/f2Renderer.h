#ifndef F2_RENDERER_H
#define F2_RENDERER_H

#include <Floekr2d/protected/f2Settings.h>
//#include <Floekr2d/plugin/glut.h>
//#include <GL/glut.h>
#include <GL/glew.h>
#include <GL/glut.h>

// �������ɾ�� ��������������Ӱ��
namespace Floekr2d
{
    class f2Shape;
    class f2Body;
    class f2CircleShape;
    class f2PolygonShape;
    namespace plugin
    {
        class f2Renderer
        {
        public:
            static f2Renderer& getRenderer();

            void drawShape(f2Shape* shape);

            void drawString(const char *s, int32 x, int32 y);

        private:
            void drawCircle(f2CircleShape* circle);

            void drawPolygon(f2PolygonShape* polygon);


            // ����ɾ�� ���ɸ��� �����Լ�����ʵ��
            // --!������ʹ��renderer������include��pluginҲ���������������ڴ�ռ����ɶ��������
            // --!�����ֶ�����getRenderer()��ô�����Զ��ڳ���ʼʱ����static��renderer
            f2Renderer();
            f2Renderer(const f2Renderer&) {};
            ~f2Renderer() {};
            f2Renderer& operator=(const f2Renderer& generator) const {};
        };
    }
}

#endif