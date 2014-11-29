#ifndef __TRACE_H__
#define __TRACE_H__
/*
 *����׷���������ڵ���ʱ��ʹ�ã����Ĵ�ͨ��RB_TRACE_FUNC����
 */
//����׷��������
//#define RB_TRACE_FUNC


#define TRACESTACK_LEN 32
#ifdef RB_TRACE_FUNC
	#define RBFN(finfo) RBTrace _tempobj(#finfo)
#else
	#define RBFN(finfo) 
#endif

#include <string>
\
class RBTrace
{
public :
	RBTrace(std::string finfo);
	~RBTrace();

	static void print_stack();
	
private:
	static void push_function(std::string finfo);
	static void pop_function();
	static std::string _tracestack[TRACESTACK_LEN];
	static int _stack_top;
	//static void _op_add_stack();
	//static void _op_sub_stack();
};

#endif