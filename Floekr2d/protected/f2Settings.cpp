#include <Floekr2d/protected/f2Settings.h>
#include <stddef.h>
#include <assert.h>
#include <float.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

// --���ش����:С�Ķ�:BUG�޸�
f2Version floekrVersion = {1, 0, 0};

void f2Log(const char* string, ...)
{
    va_list args;
    va_start(args, string);
    vprintf(string, args);
    va_end(args);
}