#include<iostream>

#include <Floekr2d/Floekr2d.h>
using namespace std;
using namespace Floekr2d;

void main()
{
    // f2Vector2
    math::f2Vector2f v0(10, 20), v1(30, 40), v2(20, 50);
    cout << "cross:" << v0.cross(v1) << endl;
    cout << "distance:" << v0.getDistance(v1) << endl;
    cout << "length:" << v0.getLength() << endl;
    
    v0.normalize();
    v0.print();

    cout << "multiple:" << v1*v2 << endl << endl;

    //f2Matrix2
    math::f2Matrix2f m(10, 20, 30, 40);
    math::f2Vector2f v3(20, 30);

    math::f2Vector2f m1 = m*v3;
    m1.print("m1");
    math::f2Vector2f m2 = v3*m;
    m2.print("m2");

    math::f2Matrix2f m3;
    m3.makeRotationMatrixr(30);
    m3.print("m3");

    math::f2Matrix2f m4;
    m4.makeScaleMatrix(2, 3);
    m4.print("m4");

    math::f2Matrix2f m5, m6;
    m5.transpose();
    m6 = m5.getTranspose();
    m5.print("m5");
    m6.print("m6");

    cout << "==:" << (m4 == m5) << endl;
    cout << "!=:" << (m4 != m5) << endl;

    m4 = m5;
    m4.print();

    cin.get();
}