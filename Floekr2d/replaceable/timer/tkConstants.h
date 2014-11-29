#ifndef TATTYKIT_CONSTANTS_H
#define TATTYKIT_CONSTANTS_H

// �궨������:
// http://www.ogre3d.org/docs/api/html/OgrePlatform_8h-source.html
// OpenFrameworks->ofConstants.h�궨��

#if defined( __WIN32__ ) || defined( _WIN32 )
#define TK_PLATFORM_WINDOWS
#include<Windows.h>

// OpenGL
// glewʹ�ò�֧��openGL��Windows����ʹ��OpenGL�ĸ߼�����
#define GLEW_STATIC
// glew/h��Ҫ������һ��glͷ�ļ�֮ǰ
//#include "glew.h"
//#include "wglew.h"
//#include "glu.h"

// Appleƽ̨
#elif defined( __APPLE_CC__)
#include <TargetConditionals.h>

// iOS֧�� ��û��iPad����
#if (TARGET_OS_IPHONE_SIMULATOR) || (TARGET_OS_IPHONE) || (TARGET_IPHONE)
#define TK_PLATFORM_IPHONE
#define TK_PLATFORM_IOS
#define TK_PLATFORM_OPENGLES

// Macƽ̨��֧��
#else
#define TK_PLATFORM_OSX
#endif

// Androidƽ̨��֧��
#elif defined (__ANDROID__)
#define TK_PLATFORM_ANDROID
#define TK_PLATFORM_OPENGLES

// Linuxƽ̨��֧��
#elif defined(__ARMEL__)
#define TK_PLATFORM_LINUX
#define TK_PLATFORM_OPENGLES
#define TK_PLATFORM_LINUX_ARM
#else
#define TK_PLATFORM_LINUX
#endif


//#ifdef TK_PLATFORM_WINDOWS
//	
//#endif
//
//#ifdef TK_PLATFORM_OSX
//
//#endif
//
//#ifdef TK_PLATFORM_IOS
//
//#endif
//
//#ifdef TK_PLATFORM_LINUX
//
//#endif
//
//#ifdef TK_PLATFORM_ANDROID
//
//#endif

#define TK_EXIT(value) std::exit(value);
// Ԥ����
//#include <WinBase.h>

#include <iostream>
// ʵ��cout << string �������򱨴�
#include <string>
#include <vector>
#include <stdlib.h>

//#include <ctime>

using namespace std;

// ��ѧ�궨��
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

// ������ֵ�ĵ���
#ifndef INVERSE_RAND_MAX
#define INVERSE_RAND_MAX 1 / (RAND_MAX + 1.0f)
#endif

// �������ж��ݲ�
#define TK_TOLERANCE_INT 0
#define TK_TOLERANCE_FLOAT32 0.000001f
#define TK_TOLERANCE_FLOAT64 0.00000001


// ��ѧ��
// --!�����Openframeworks->ofConstants.h->line:386
#ifndef MIN
	#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#endif

#ifndef MAX
	#define MAX(x,y) (((x) > (y)) ? (x) : (y))
#endif

#ifndef CLAMP
	#define CLAMP(val,min,max) ((val) < (min) ? (min) : ((val > max) ? (max) : (val)))
#endif

#ifndef ABS
	#define ABS(x) (((x) < 0) ? -(x) : (x))
#endif


// ö�ٳ���
enum tkWindowMode
{
	TK_WINDOW_WINDOWED, // ���ڻ�
	TK_WINDOW_NOTITLE,	// �ޱ��������
	TK_WINDOW_FULLSCREEN, // ȫ��
	TK_WINDOW_GAMEMODE // �ر��Ż����ȫ����Ϸģʽ
};

//enum tkWindowResizable
//{
//	TK_WINDOW_RESIZABLE,
//	TK_WINDOW_NORESIZABLE
//};

enum tkDriverType
{
	TK_DRIVER_OPENGL,
	TK_DRIVER_DIRECTX
};

//// ��ͬ�⹤�ߴ����Ĳ�ͬ�������� 
//enum tkWindowType
//{
//	TK_WINDOW_GLFW,
//	TK_WINDOW_DIRECTX
//};

#endif