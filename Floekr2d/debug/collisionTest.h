#include<iostream>

#include <Floekr2d/Floekr2d.h>
using namespace std;
using namespace Floekr2d;
using namespace math;

void main()
{
    //circle
    //f2Body *a = new f2Body(30, f2Vector2f(100, 100)),
    //    *b = new f2Body(40, f2Vector2f(110, 125));
    //f2Manifold *m = new f2Manifold(a, b);

    //f2CircletoCircle(m, a, b);

    //f2Body *a = new f2Body(40, f2Vector2f(70, 90)),
    //    *b = new f2Body(0, f2Vector2f(110, 125));
    //f2Manifold *m = new f2Manifold(a, b);

    //f2Vector2f vectices[4] = {f2Vector2f(10, 10), f2Vector2f(50, 10), f2Vector2f(5, 50), f2Vector2f(45, 50)};
    //b->set(vectices, 4);

    //f2CircletoPolygon(m, a, b);

    // polygon
    //f2Body *a = new f2Body(0, f2Vector2f(70, 90)),
    //    *b = new f2Body(0, f2Vector2f(100, 100));
    //f2Manifold *m = new f2Manifold(a, b);

    //f2Vector2f vectices[4] = {f2Vector2f(10, 10), f2Vector2f(50, 10), f2Vector2f(5, 50), f2Vector2f(45, 50)};
    //a->set(vectices, 4);

    //f2Vector2f vectice[4] = {f2Vector2f(0, 0), f2Vector2f(30, 0), f2Vector2f(30, 30), f2Vector2f(0, 30)};
    //b->set(vectice, 4);

    f2Vector2f vectices[4] = {f2Vector2f(10, 10), f2Vector2f(50, 10), f2Vector2f(5, 50), f2Vector2f(45, 50)};
    f2PolygonShape* p = new f2PolygonShape();
    p->setVertices(vectices, 4);
    p->setPosition(math::f2Vector2f(70, 90));

    f2Vector2f vectice[4] = {f2Vector2f(0, 0), f2Vector2f(30, 0), f2Vector2f(30, 30), f2Vector2f(0, 30)};
    f2PolygonShape* p1 = new f2PolygonShape();
    p1->setVertices(vectice, 4);
    p->setPosition(math::f2Vector2f(100, 100));

    f2Manifold *m = new f2Manifold();

    f2PolygontoPolygon(m, p, p1);


    cin.get();
}