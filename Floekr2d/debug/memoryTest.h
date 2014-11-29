// Memory.cpp : 定义控制台应用程序的入口点。
//
#include <stdlib.h>
#include <stdio.h>

#include <Floekr2d/replaceable/allocator/MemoryPool.h>
#include <Floekr2d/replaceable/allocator/ATEST.h>

RBFrameAlloctor* ATEST::all = 0;


void main()
{

    printf("%d\n", sizeof(ATEST));
    int num = 17;
    scanf_s("%d", &num);
    getchar();
    RBPoolAllctor pool_alloctor;
    pool_alloctor.init();
    //空指针需要一个值
    void* res;
    void* nodes;
    nodes = NULL;
    pool_alloctor.new_pool(&nodes, sizeof(ATEST));
    res = nodes;
    ATEST * aa, *aa1;
    for(int i = 0; i<num; i++)
    {
        /*
        aa = ::new ATEST(1,true);
        //ATEST bb(1,1);
        aa->init();
        */
        /*
        aa = (ATEST*) pool_alloctor.alloc(&nodes, sizeof(ATEST));
        aa->ATEST::ATEST(1, 1);
        aa->init();


        aa1 = (ATEST*) pool_alloctor.alloc(&nodes, sizeof(ATEST));
        aa1->ATEST::ATEST(1, 1);
        aa1->init();
        pool_alloctor.free(&nodes, &aa, sizeof(ATEST));
        pool_alloctor.free(&nodes, &aa1, sizeof(ATEST));
        */
    }
    pool_alloctor.print_list(res, sizeof(ATEST));

    getchar();


    ATEST* l = (ATEST*) pool_alloctor.alloc(&nodes, sizeof(ATEST));
    //pool_alloctor.free(&nodes,l,sizeof(ATEST));
    l->ATEST::ATEST(1, 1);
    l->print();
    //pool_alloctor.free(&nodes,l,sizeof(ATEST));
    ATEST* ttt = (ATEST*) pool_alloctor.alloc(&nodes, sizeof(ATEST));
    ttt->ATEST::ATEST(21, 0);
    l->print();

    getchar();


}
