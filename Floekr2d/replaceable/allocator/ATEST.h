#pragma once
#include "stdio.h"
class RBFrameAlloctor;
class ATEST
{
public:
	ATEST(int a,bool b)
	{
		w=a;
		aa=b;
	}
	int a;
	void init(){a=1;k=1;w=1;}
	void print(){printf("%d  %d  %d\n",a,k,w);}

	/*
	static void * operator new(size_t size)
	{
		return all->alloc(size,0);
	}
	*/

	static RBFrameAlloctor* all;
private:
	int k;
	int w;
	bool aa;
};
