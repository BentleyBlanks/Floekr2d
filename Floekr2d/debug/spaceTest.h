#include<iostream>

#include <Floekr2d/Floekr2d.h>

using namespace std;
using namespace Floekr2d;
using namespace Floekr2d::core;

void main()
{
    /*
    // f2Space
    f2Space *space = new f2Space();

    // circle
    f2CircleShape* circle = (f2CircleShape*) space->createShape(f2Shape::e_circle);
    circle->setRadius(3.0);
    circle->setDensity(5.0);

    // polygon
    f2Vector2f vectices[4] = {f2Vector2f(10, 10), f2Vector2f(50, 10), f2Vector2f(5, 50), f2Vector2f(45, 50)};
    f2PolygonShape* polygon = (f2PolygonShape*) space->createShape(f2Shape::e_polygon);
    polygon->setVertices(vectices, 4);
    polygon->setDensity(1.0);

    // body
    f2Body* body = space->createBody();
    body->setShape(circle);
    body->setAngularVelocity(2.0);
    body->setLinearVelocity(math::f2Vector2f());
    body->setOrient(1 / 2*PI);
    body->setPosition(math::f2Vector2f(100, 200));

    f2Body* body1 = space->createBody();
    body1->setShape(polygon);
    body1->setAngularVelocity(3.0);
    body1->setLinearVelocity(math::f2Vector2f(20, 30));
    body1->setOrient(1 / 3 * PI);
    body1->setPosition(math::f2Vector2f(50, 60));
    

    f2List<f2Body*> bodyList = space->getBodyList();
    f2List<f2Shape*> shapeList = space->getShapeList();

    for(int i = 0; i < bodyList.size(); i++)
    {
        cout << "第" << i << "个body:" << endl << endl;
        bodyList[i]->print();
    }

    cout << "shape元素数量为:" << shapeList.size() << endl;

    getchar();

    // 因body中含有shape的指针 因此在delete时应该先删除body 再删除shape
    space->deleteBody(body);
    space->deleteShape(circle);

    bodyList = space->getBodyList();
    shapeList = space->getShapeList();
    cout << "删除后bodyList元素数量为:" << bodyList.size() << endl;
    cout << "删除后shape元素数量为:" << shapeList.size() << endl;

    getchar();
    */

    // f2Collision
    f2Space *space = new f2Space();

    f2Vector2f vectices[4] = {f2Vector2f(10, 10), f2Vector2f(50, 10), f2Vector2f(5, 50), f2Vector2f(45, 50)};
    f2PolygonShape* polygon = (f2PolygonShape*) space->createShape(f2Shape::e_polygon);
    polygon->setVertices(vectices, 4);
    polygon->setDensity(1.0);

    f2Vector2f vectice[4] = {f2Vector2f(0, 0), f2Vector2f(30, 0), f2Vector2f(30, 30), f2Vector2f(0, 30)};
    f2PolygonShape* polygon1 = (f2PolygonShape*) space->createShape(f2Shape::e_polygon);
    polygon1->setVertices(vectice, 4);
    polygon1->setDensity(1.0);

    f2Manifold *m = new f2Manifold();

    // body会调用computeMass 而这个函数会将space移到模型空间
    f2Body* body = space->createBody();
    body->setShape(polygon);
    body->setPosition(math::f2Vector2f(70, 90));

    f2Body* body1 = space->createBody();
    body1->setShape(polygon1);
    body1->setPosition(math::f2Vector2f(100, 100));

    f2PolygontoPolygon(m, polygon, polygon1);

    // 版本号
    floekrVersion.print();

    int32 i = 1, i1 = 3;
    float32 f = 1.0f, f1 = 3.0f;
    f2Log("Log测试:i = %d, i1 = %d, f = %f, f1 = %f\n", i, i1, f, f1);

    space->step();

    getchar();
    return;
}