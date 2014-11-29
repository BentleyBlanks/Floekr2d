#ifndef F2_SETTINGS_H_
#define F2_SETTINGS_H_

#include <iostream>
// 保证string能够cout
#include <string>
// 包含min&max函数
#include <algorithm>
#include <assert.h>
#include <math.h>

typedef signed char	int8;
typedef signed short int16;
typedef signed int int32;
typedef unsigned short uint16;
typedef unsigned int uint32;
#ifdef WIN32
typedef __int64 int64;
typedef unsigned __int64 uint64;
#endif
typedef float float32;
typedef double float64;

// 中断
#define f2Assert(A) assert(A)

// 数学宏定义
#ifndef PI
#define PI       3.14159265359
#endif

#ifndef TWO_PI
#define TWO_PI   6.28318530718
#endif

#ifndef INVERSE_PI
#define INVERSE_PI   0.31830988618
#endif

#ifndef FOUR_PI
#define FOUR_PI   12.5663706144
#endif

#ifndef HALF_PI
#define HALF_PI  1.57079632679
#endif

#ifndef DEGREE_TO_RADIAN
#define DEGREE_TO_RADIAN (PI/180.0)
#endif

#ifndef RADIAN_TO_DEGREE
#define RADIAN_TO_DEGREE (180.0/PI)
#endif

/// A small length used as a collision and constraint tolerance. Usually it is
/// chosen to be numerically significant, but visually insignificant.
// 来自Box2d b2Settings.h /68
#define F2_LINEARSLOP			0.005f

// 浮点数判断容差
#define F2_TOLERANCE_INT 0
#define F2_TOLERANCE_FLOAT32 0.000001f
#define F2_TOLERANCE_FLOAT64 0.00000001
#define F2_EPSILON 0.0001f

#define F2_MAX_POLYVERTEXCOUNT 64

// 是否启用插件系统
#define F2_USEPLUGIN 

// log借鉴于box2d的b2Log
void f2Log(const char* string, ...);


// Floekr2d版本信息
// --!来自box2d http://en.wikipedia.org/wiki/Software_versioning
struct f2Version
{
    int32 major;		///< significant changes
    int32 minor;		///< incremental changes
    int32 revision;		///< bug fixes

    void print()
    {
        std::cout << "Floekr2d当前版本号为:" << major << "." << minor << "." << revision << std::endl;
    }
};

// 当前版本号
extern f2Version floekrVersion;

#endif