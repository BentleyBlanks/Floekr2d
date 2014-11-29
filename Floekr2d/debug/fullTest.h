#include <Floekr2d/Floekr2d.h>
using namespace Floekr2d;

#define ESC_KEY 27

f2Space* space;

void Mouse(int button, int state, int x, int y)
{
    x /= 10.0f;
    y /= 10.0f;

    if(state == GLUT_DOWN)
    {
        switch(button)
        {
        case GLUT_LEFT_BUTTON:
        {
                                 f2PolygonShape* polygon = (f2PolygonShape*) space->createShape(f2Shape::e_polygon);

                                 uint32 count = (uint32) f2Random(3, F2_MAX_POLYVERTEXCOUNT);
                                 math::f2Vector2f *v = new math::f2Vector2f[count];

                                 float32 e = f2Random(5, 10);
                                 for(uint32 i = 0; i < count; ++i)
                                     v[i].set(f2Random(-e, e), f2Random(-e, e));

                                 polygon->setVertices(v, count);
                                 polygon->setPosition(math::f2Vector2f(x, y));
                                 polygon->setDensity(1.0f);

                                 f2Body* b = space->createBody();
                                 b->setShape(polygon);
                                 b->setRadians(f2Random(-PI, PI));
                                 b->restitution = 0.2f;
                                 b->dynamicFriction = 0.2f;
                                 b->staticFriction = 0.4f;
                                 delete[] v;
                                 break;
        }
        case GLUT_RIGHT_BUTTON:
        {
                                  f2CircleShape* circle = (f2CircleShape*) space->createShape(f2Shape::e_circle);
                                  circle->setRadius(f2Random(1.0f, 3.0f));
                                  circle->setPosition(math::f2Vector2f(x, y));
                                  circle->setDensity(1.0f);

                                  f2Body* body = space->createBody();
                                  body->setShape(circle);
                                  break;
        }
        }
    }
}

void Keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
    case ESC_KEY:
        exit(0);
        break;
    }
}

void PhysicsLoop(void)
{

    //glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    plugin::f2Renderer* renderer = &plugin::f2Renderer::getRenderer();
    //while(1)
    renderer->drawString("Left click to spawn a polygon", 1, 2);
    renderer->drawString("Right click to spawn a circle", 1, 4);
    
    space->step();

    //glColor3f(1.0, 0.0, 0.0);
    //glBegin(GL_LINES);
    //glVertex2i(30, 30);
    //glVertex2i(80, 50);
    //glEnd();

    for(int32 i = 0; i < space->getBodyList().size(); i++)
    {
        renderer->drawShape(space->getBodyList()[i]->getShape());
    }



    Sleep(1000.0f / 100.0f);

    glutSwapBuffers();
    //glFlush();
}

int main(int argc, char** argv)
{
    //glutInit(&argc, argv);
    //glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
    //glutInitWindowSize(800, 600);
    //glutCreateWindow("Floekr2d");
    //glutDisplayFunc(PhysicsLoop);
    //glutKeyboardFunc(Keyboard);
    //glutMouseFunc(Mouse);
    //glutIdleFunc(PhysicsLoop);



    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_DOUBLE);
    glutInitWindowPosition(50, 100);
    glutInitWindowSize(1334, 750);
    glutCreateWindow("Floekr2d");
    glClearColor(0.9, 0.9, 0.9, 0.0);
    glutDisplayFunc(PhysicsLoop);
    glutIdleFunc(PhysicsLoop);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();
    gluOrtho2D(0, 133, 75, 0);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glutKeyboardFunc(Keyboard);
    glutMouseFunc(Mouse);

    space = new f2Space();
    space->setGravity(math::f2Vector2f(0.0, 50.0f));

    //f2CircleShape *circle = (f2CircleShape*) space->createShape(f2Shape::e_circle);
    //circle->setRadius(5.0f);
    //circle->setPosition(math::f2Vector2f(40, 40));
    //circle->setDensity(0.0f);

    //f2Body* b = space->createBody();
    //b->setShape(circle);

    f2PolygonShape* poly = (f2PolygonShape*) space->createShape(f2Shape::e_polygon);
    poly->setAsBox(30.0f, 1.0f);
    poly->setDensity(0.0f);
    poly->setPosition(math::f2Vector2f(60, 65));
    
    f2Body* body = space->createBody();
    body->setShape(poly);
    body->setRadians(0.0f);

    f2PolygonShape* poly1 = (f2PolygonShape*) space->createShape(f2Shape::e_polygon);
    poly1->setAsBox(20.0f, 10.0f);
    poly1->setPosition(math::f2Vector2f(60, 60));

    f2Body* body1 = space->createBody();
    body1->setShape(poly1);
    body1->setRadians(0.0f);
    //f2PolygonShape* polygon = (f2PolygonShape*) space->createShape(f2Shape::e_polygon);

    //uint32 count = (uint32) f2Random(5, F2_MAX_POLYVERTEXCOUNT);
    //math::f2Vector2f *v = new math::f2Vector2f[count];

    //float32 e = f2Random(5, 10);
    //for(uint32 i = 0; i < count; ++i)
    //    v[i].set(f2Random(-e, e), f2Random(-e, e));

    //polygon->setVertices(v, count);
    //polygon->setPosition(math::f2Vector2f(40, 20));
    //polygon->setDensity(1.0f);

    //f2Body* b1 = space->createBody();
    //b1->setShape(polygon);
    //b1->setRadians(2);
    //b1->restitution = 0.2f;
    //b1->dynamicFriction = 0.2f;
    //b1->staticFriction = 0.4f;
    //delete[] v;


    //f2PolygonShape* polygon1 = (f2PolygonShape*) space->createShape(f2Shape::e_polygon);

    //uint32 count1 = (uint32) f2Random(5, F2_MAX_POLYVERTEXCOUNT);
    //math::f2Vector2f *v1 = new math::f2Vector2f[count1];

    //float32 e1 = f2Random(5, 10);
    //for(uint32 i = 0; i < count1; ++i)
    //    v1[i].set(f2Random(-e1, e1), f2Random(-e1, e1));

    //polygon1->setVertices(v1, count1);
    //polygon1->setPosition(math::f2Vector2f(40, 20));
    //polygon1->setDensity(1.0f);

    //f2Body* b2 = space->createBody();
    //b2->setShape(polygon1);
    //b2->setRadians(2);
    //b2->restitution = 0.2f;
    //b2->dynamicFriction = 0.2f;
    //b2->staticFriction = 0.4f;
    //delete[] v1;

    srand(1);
    glutMainLoop();

    return 0;
}
