#include <string>
#include "stdio.h"
#include "Trace.h"

RBTrace::RBTrace(std::string finfo)
{
	push_function(finfo);
}

RBTrace::~RBTrace()
{
	pop_function();
}

void RBTrace::pop_function()
{
	if(_stack_top>0)
		--_stack_top;
}

void RBTrace::push_function(std::string finfo)
{
	if(_stack_top<=TRACESTACK_LEN-1)
		_tracestack[_stack_top++] = finfo;

}


int RBTrace::_stack_top = 0;
std::string RBTrace::_tracestack[TRACESTACK_LEN];

void RBTrace::print_stack()
{
	for(int i=0;i<_stack_top;i++)
	{
		for (int j=0;j<i;j++)
			printf(" ");
		printf(_tracestack[i].data());
		printf("\n");
	}
	printf("============\n");
}