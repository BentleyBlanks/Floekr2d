#include<iostream>

#include <Floekr2d/Floekr2d.h>
#include <Floekr2d/protected/datastruct/f2List.h>

using namespace std;
using namespace Floekr2d;

typedef struct hello
{
    void print()
    {
        cout << "x:" << x << ",";
        cout << "y:" << y << endl;
    }


    int x;
    float y;
}point;

void main()
{
    // f2List
    f2List<point> list;

    point x[20];

    cout << "pushback:" << endl;
    for(int i = 0; i < 20; i++)
    {
        x[i].x = i;
        x[i].y = i * 2;

        list.pushback(x[i]);
    }

    for(int i = 0; i < list.size(); i++)
    {
        list[i].print();
    }

    getchar();

    cout << "popback:" << endl;
    for(int i = 0; i < 3; i++)
    {
        list.popback();
    }

    for(int i = 0; i < list.size(); i++)
    {
        list[i].print();
    }

    getchar();

    cout << "remove:" << endl;
    list.remove(30);

    for(int i = 0; i < list.size(); i++)
    {
        list[i].print();
    }

    getchar();
    cout << "clear finish" << endl;
    list.clear();

    cout << "size:";
    cout << list.size() << endl;

    getchar();
    return;
}