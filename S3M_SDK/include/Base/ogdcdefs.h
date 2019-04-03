//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file ogdcdefs.h
//!  \brief 提供一些基本类型的宏 
//!  \details  
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCDEFS_H__EB930378_2B6A_4DDE_891C_41996E06B5EC__INCLUDED_)
#define AFX_OGDCDEFS_H__EB930378_2B6A_4DDE_891C_41996E06B5EC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>
#include <time.h>

#include <string>

#ifdef SYMBIAN60
#include <_map.h>
#include <_algo.h>
#else
#include <map>
#include <algorithm>
#endif 

#include <vector>
#include <list>
#include <iostream>
#include <functional>
#include <set>		//add by shuxl	10.7.1

#ifdef WIN32
	#pragma warning(disable: 4251)
	#pragma warning(disable: 4275)
	#pragma warning(disable: 4786)
#endif

#ifndef WIN32
// commited by zengzm 2007-3-30 把link.h放在这里，避免在Solaris中编译不过，
// 因为后面会定义_FILE_OFFSET_BITS为64，而link.h中include的libelf.h中
// 判断_FILE_OFFSET_BITS不等于32，且_ILP32也已经被定义，就会输出编译错误
// 估计是中间的某个头文件中定义了_ILP32，因此把link.h放到最前面中，以避免此编译错误
// modified by yanbg 08.1.6 增加多HP UNIX系统的判断
//#ifdef _AIX
#if !defined(OS_ANDROID) && !defined IOS 
#if defined(_AIX) || defined(hpux)
	#include <dlfcn.h> // for dlopen() & dlsym()
#else 
	#include <link.h>  // for dlopen() & dlsym()
#endif
#endif               

// 头文件包含
// 定义宏，使得可以处理大文件（>4GB）
	#undef _FILE_OFFSET_BITS
	#define _FILE_OFFSET_BITS 64
	#include <unistd.h>
	#include <dirent.h>	
	#include <fcntl.h>
//modified by xielin 这个头文件包含要放到前面来，否则在solaris下readdir出来的不对，具体愿意不祥
	#include <dirent.h> // for opendir(), readdir(), closedir() 
	#include <sys/types.h> // for lstat
	#include <sys/stat.h> // for lstat
	#include <sys/time.h>
	#include <assert.h>
#if !defined SYMBIAN60 && !defined OS_ANDROID && !defined IOS 
	#include <X11/Xlib.h>
	#include <X11/Xutil.h>
	#include <X11/cursorfont.h>
#endif

#ifdef APPLE
	#include <mach/mach_init.h>
	#include <mach/semaphore.h>
	#include <mach/task.h>
	#include <pthread.h>
#else
	#include <pthread.h>
	#include <semaphore.h>
#endif

	#include <unistd.h> // for getwcd()
	#include <dlfcn.h> // for dlopen() & dlsym()
// commited by zengzm 2007-3-30 放在这里，会导致在Solaris中编译不过，
// 因为已经定义了_FILE_OFFSET_BITS为64，而link.h中include的libelf.h中
// 判断_FILE_OFFSET_BITS不等于32，且_ILP32也已经被定义，就会输出编译错误
// 估计是中间的某个头文件中定义了_ILP32，因此把link.h放到ugnowin.h中，以避免此编译错误
// #include <link.h> // for dlopen() & dlsym()
#if !defined SYMBIAN60 && !defined OS_ANDROID && !defined IOS
	#include <X11/Xlib.h>
#endif 
	#include <cstring>
	#include <time.h>
	#include <fstream>
	#include "Base/prefix.h"
#endif

#include "Base/ogdcexports.h"
#include "Base/ogdccolordef.h"

// Truth values
#ifndef TRUE
	#define TRUE 1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#ifndef NULL
	#define NULL 0
#endif


#ifdef _UGUNICODE
#define _U(x)  L ## x
#else
#define _U(x)	x
#endif
#define OGDC_DLL_VERSION		_U("")		//! \brief  dll版本号
		      
//! \brief  极小值定义
#define EP 1e-10
#define NEP -1e-10
//!  定义各种类型的极值 caogf
#define OGDC_DBLMAX 1.7976931348623158e+308 /* max value */
#define OGDC_DBLMIN 2.2250738585072014e-308 /* min positive value */
#define OGDC_FLTMAX 3.402823466e+38F  /* max value */
#define OGDC_FLTMIN 1.175494351e-38F  /* min positive value */

// For Windows
#if defined(_DEBUG)
#if !defined(DEBUG)
	#define DEBUG
#endif
#endif

#ifdef _NDEBUG
	#ifndef NDEBUG
	#define NDEBUG
#endif
#endif

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC__ __FILE__ "(" __STR1__(__LINE__)") : "

#if (_MSC_VER >= 800)
    #define CALLBACK    __stdcall
#else
    #define CALLBACK
#endif

//!  1200, VC6
//! 1310 for Microsoft Visual C++ .NET 2003. 1310 represents version 13 and a 1.0 point release. 
//! The Visual C++ 2005 compiler version is 1400.
#ifdef WIN32
#if (_MSC_VER<=1400)
	#ifndef vsnprintf
		#define vsnprintf _vsnprintf
	#endif
#endif
#endif

#define OGDCCALLBACK CALLBACK

#ifdef WIN32
#define OGDCSTDCALL __stdcall
#else
#define OGDCSTDCALL
#endif

//! \brief 因为不同平台SYSTEM_WCHAR_SIZE 不同，通过变量确认，
//默认宽度为 2
#if !defined(SYSTEM_WCHAR_SIZE)
#define SYSTEM_WCHAR_SIZE 2
#endif


#ifdef WIN32
#define ISPATHSEP(c) ((c)==_U('/') || (c)==_U('\\'))
#else
#define ISPATHSEP(c) ((c)==_U('/'))
#endif

namespace OGDC {

//! \brief Streamable types; these are fixed size!
typedef unsigned char          OgdcUchar;
typedef OgdcUchar			   OgdcByte;
typedef OgdcUchar			   OgdcBool;
typedef unsigned short         OgdcUshort;
typedef short                  OgdcShort;
typedef unsigned int           OgdcUint;
//#if SYSTEM_WCHAR_SIZE==4
//typedef unsigned int           OgdcWchar;
//#else
//typedef wchar_t                OgdcWchar;
//#endif
typedef wchar_t		       OgdcWchar;
typedef char				   OgdcAchar;
typedef unsigned int		   OgdcWchar32;
typedef int                    OgdcInt;
typedef float                  OgdcFloat;
typedef double                 OgdcDouble;

#ifdef _UGUNICODE
typedef OgdcWchar OgdcChar;
#else
typedef OgdcAchar OgdcChar;
#endif

#ifdef OS_ANDROID
#ifndef WhiteBox_Ignore
#define WhiteBox_Ignore
#endif 
#endif

#if defined(_MSC_VER) || (defined(__BCPLUSPLUS__) && __BORLANDC__ > 0x500) || defined(__WATCOM_INT64__)
	typedef unsigned __int64       OgdcUlong;
	typedef __int64                OgdcLong;	

#elif defined(__GNUG__) || defined(__GNUC__) || defined(__SUNPRO_CC) || defined(__MWERKS__) || defined(__SC__) || defined(__xlC__)

	typedef unsigned long long int OgdcUlong;	//	WhiteBox_Ignore
	typedef long long int          OgdcLong;	//	WhiteBox_Ignore
	
#endif

typedef OgdcUint                 OgdcColor;
typedef size_t                   OgdcSizeT;


#ifndef EXTERN_C
#ifdef __cplusplus
#define EXTERN_C    extern "C"
#else
#define EXTERN_C    extern
#endif
#endif

#ifndef __FILEW__
#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __FILEW__ WIDEN(__FILE__)
#endif

//! \brief 坐标与距离单位定义 - 标准方式
//! \brief 毫米
#define AU_MILIMETER	10
//! \brief 厘米			
#define AU_CENTIMETER	100
//! \brief 分米			
#define AU_DECIMETER	1000
//! \brief 米		
#define AU_METER		10000
//! \brief 公里		
#define AU_KILOMETER	10000000
//! \brief 英里	
#define AU_MILE			16090000
//! \brief 英寸??	
#define AU_INCH			254	
//! \brief Yard码		
#define AU_YARD			9144
//! \brief Feet英尺		 
#define AU_FOOT			3048
//! \brief Yard码		
#define	AU_YARD_SEARS   9144		 

//! \remarks 角度单位以 (10e8)分之一弧度为 单位1为避免和长度单位混淆，都加上10e10
//   因此在计算时，应减去10e10，然后再计算
#define AU_DIFF              1000000000
#define AU_RADIAN            (100000000 + 1000000000)	//  Radian                                 
#define AU_DEGREE			 (1745329   + 1000000000)	//  Degree                                 
#define AU_MINUTE            (29089     + 1000000000)	//  Arc-minute                             
#define AU_SECOND            (485       + 1000000000)	//  Arc-second                             
#define AU_GRAD              (1570796   + 1000000000)	//  Grad (angle subtended by 1/400 circle) 
#define AU_MICRORADIAN       (100       + 1000000000)	//  Microradian ( 1e-6 radian )            
#define AU_MINUTE_CENTESIMAL (15708     + 1000000000)	//  Centesimal minute (1/100th Gon (Grad)) 
#define AU_SECOND_CENTESIMAL (157       + 1000000000)	//  Centesimal second(1/10000th Gon (Grad))
#define AU_MIL_6400          (98175     + 1000000000)	//  Mil (angle subtended by 1/6400 circle) 


//! \brief Abolute value
#define OGDCABS(val) (((val)>=0)?(val):-(val))

//是否为极值
#define OGDCEXTREFLT(x) (OGDCABS(OGDCABS(x)-OGDC_FLTMAX)<=0.000000001e+38F)
#define OGDCEXTREDBL(x) (OGDCABS(OGDCABS(x)-OGDC_DBLMAX)<=0.0000000000000001e+308)

//! \brief Return the maximum of a or b
#define OGDCMAX(a,b) (((a)>(b))?(a):(b))

//! \brief Return the minimum of a or b
#define OGDCMIN(a,b) (((a)>(b))?(b):(a))


//! \brief Swap a pair of numbers
#define OGDCSWAP(a,b,t) ((t)=(a),(a)=(b),(b)=(t))

//! \brief Clamp value x to range [lo..hi]
#define OGDCCLAMP(lo,x,hi) ((x)<(lo)?(lo):((x)>(hi)?(hi):(x)))

#define OGDCROUND(x) (((x)>0)?OgdcInt((x)+0.5):(OgdcInt((x)-0.5)))
//OGDCQuickRound速度快但是精度范围可能存在多1的问题,处理颜色时不能用这个定义为OGDCROUND2
//需要使用可以用这个
#define OGDCROUND2(x) OGDC::OGDCQuickRound(x)

//! \brief 两个数是否相等的判断
#define OGDCEQUAL(x,y) OGDC::ogdcequal(x,y)
#define OGDCEQ(x,y,d) OGDC::ogdcequal(x,y,d)

//! \brief 一个数是否为0 (或者0附近的极小数) 的判断
//! \brief 重要提示: 两个数是否相等的比较 请勿使用 OgdcIS0(a-b) , 而应该直接使用OgdcEQUAL(a,b)
#define  OGDCIS0(x)	(((x)<EP) && ((x)>NEP))

#ifdef OS_ANDROID
#include <android/log.h>
#define LOGD(TAG,...) __android_log_print(ANDROID_LOG_DEBUG, TAG,__VA_ARGS__)
#define LOGE(TAG,...) __android_log_print(ANDROID_LOG_ERROR, TAG,__VA_ARGS__)
#define LOGHERE(s) LOGD("Debug","%s In %d line:%s",__FILE__,__LINE__,s);
#endif

#ifndef NDEBUG
#ifdef IOS
    #define OGDCASSERT(exp) ((exp)?((void)0):(void)ogdcassert(#exp,__FILE__,__LINE__))
#else
	#define OGDCASSERT(exp) ((exp)?((void)0):(void)OGDC::ogdcassert(#exp,__FILE__,__LINE__))
#endif
#else
	#define OGDCASSERT(exp) ((void)0)
#endif

#define OGDCIsLeadByte(ch) (ch & 0x80)

extern BASE_API void ogdcassert(const char* pExpression,const char* pFilename,unsigned int nLineno);

extern BASE_API OgdcBool ogdcequal(OgdcDouble dX, OgdcDouble dY);

extern BASE_API OgdcBool ogdcequal(OgdcDouble dX, OgdcDouble dY, OgdcDouble dTolerance);

//! \brief 颜色相关
extern BASE_API OgdcColor OGDCRGB(OgdcByte chR,OgdcByte chG,OgdcByte chB);

extern BASE_API OgdcColor OGDCRGBA(OgdcUchar r,OgdcUchar g,OgdcUchar b,OgdcUchar a);

extern BASE_API OgdcUchar OGDCREDVAL(OgdcColor clr);

extern BASE_API OgdcUchar OGDCGREENVAL(OgdcColor clr);

extern BASE_API OgdcUchar OGDCBLUEVAL(OgdcColor clr);

extern BASE_API OgdcUchar OGDCALPHAVAL(OgdcColor clr);

extern BASE_API OgdcInt OGDCQuickRound(OgdcDouble dX);

static union
{
	int nTest;
	char cTest[sizeof(int)];
}un_bigendina = {1};
#define  OGDC_ISBIGENDIAN (un_bigendina.cTest[0] != 1)

}

#endif // !defined(AFX_OGDCDEFS_H__EB930378_2B6A_4DDE_891C_41996E06B5EC__INCLUDED_)

