#pragma once
#include <stdlib.h>
#include "stdio.h"
#include "RBBasedata.h"


//�ڴ���룺byte����
#define BYTEALIGN (int)8
//ǿ�ƶ���
#define ALIGNUP(naddress,nbytes) ((((uint)naddress)+\
	(nbytes)-1 ) & (~((nbytes)-1)))

typedef bool MPOS;
/*
class IRBBaseAlloctor abstract
{
public:
	virtual INI_RESULT init(size_t tsize) = 0;
	//���ؿ��򷵻�ʧ�ܣ���Ϣ��д����־����
	virtual void* alloc(size_t tsize) = 0;
	//virtual void* alloc(size_t tsize,MPOS tpos) = 0;
	virtual INI_RESULT shutdown() = 0;

private:
	//IRBBaseAlloctor(IRBBaseAlloctor&);

};

class  IRBSystemAlloctor abstract
{
public:

	void *operator new(size_t);
	void operator delete(void*){printf("������ɾ����������Ȼû�з����ڴ棡");}

	void *operator new[](size_t){void* p;return p;}
	void operator delete[](void*){}

};
*/