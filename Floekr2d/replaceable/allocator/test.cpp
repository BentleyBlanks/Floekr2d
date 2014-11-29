// Memory.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"


#include <stdlib.h>
#include <stdio.h>

#include "MemoryPool.h"
#include "MemoryFrame.h"
#include "MemoryManager.h"
#include "time.h"
#include "Trace.h"
#include "ATEST.h"

clock_t start, finish; 
double Total_time; 
void a2(),a3(),a4();
void a1()
{
	RBFN(a1);
	a2();
}

void a2()
{
	RBFN(a2);
	a3();
}

void a3()
{
	RBFN(a3);
	RBTrace::print_stack();
	a4();
}

void a4()
{
	RBFN(a4);
	RBTrace::print_stack();
}
//typedef unsigned int uint;
//#define ALIG(naddress,nbytes) ((((uint)naddress)+\
//	(nbytes)-1 ) & (~((nbytes)-1)))


RBFrameAlloctor* ATEST::all = 0;


void main()
{

	printf("%d\n",sizeof(ATEST));
	int num = 17;
	scanf_s("%d",&num);
	getchar();
	RBPoolAllctor pool_alloctor;
	pool_alloctor.init();
	//空指针需要一个值
	void* res;
	void* nodes;
	nodes = NULL;
	pool_alloctor.new_pool(&nodes,sizeof(ATEST));
	res = nodes;
	ATEST * aa,*aa1;
	for (int i=0;i<num;i++)
	{
		/*
		aa = ::new ATEST(1,true);
		//ATEST bb(1,1);
		aa->init();
		*/
		
		aa = (ATEST*)pool_alloctor.alloc(&nodes,sizeof(ATEST));
		aa->ATEST::ATEST(1,1);
		aa->init();
		

		aa1 = (ATEST*)pool_alloctor.alloc(&nodes,sizeof(ATEST));
		aa1->ATEST::ATEST(1,1);
		aa1->init();
		pool_alloctor.free(&nodes,&aa,sizeof(ATEST));
		pool_alloctor.free(&nodes,&aa1,sizeof(ATEST));
		
	}
	pool_alloctor.print_list(res,sizeof(ATEST));

	getchar();
	
	
	ATEST* l = (ATEST*)pool_alloctor.alloc(&nodes,sizeof(ATEST));
	//pool_alloctor.free(&nodes,l,sizeof(ATEST));
	l->ATEST::ATEST(1,1);
	l->print();
	//pool_alloctor.free(&nodes,l,sizeof(ATEST));
	ATEST* ttt = (ATEST*)pool_alloctor.alloc(&nodes,sizeof(ATEST));
	ttt->ATEST::ATEST(21,0);
	l->print();
	
	getchar();


}
