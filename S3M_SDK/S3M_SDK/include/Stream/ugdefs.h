#ifndef UGDEFS_H
#define UGDEFS_H

#include "Base/OgdcArray.h"
#include "Base/OgdcList.h"
#include "Base/OgdcDict.h"
#include "Base/OgdcRect2D.h"
#include "Base/OgdcRect.h"
#include "Base/OgdcRectF.h"
#include "Base/OgdcSize2D.h"
#include "Base/OgdcPoint3D.h"
#include "Base/OgdcPointEPS.h"
#include "Base/OgdcStyle.h"
#include "Base/OgdcTextStyle.h"
#include "Base/OgdcSystem.h"
#include "Base/OgdcVariant.h"
#include "Base/OgdcColorTable.h"
#include "Base/OgdcColorDictTable.h"
#include "Base/OgdcHistogram.h"
#include <climits>
#include <memory>
//#include "Element/OgdcRasterBlock.h"

// #include "Stream/ugplatform.h"
#include "Stream/ugexports.h"
#include "Stream/ugversion.h"
#include "Stream/ugkeydef.h"

using namespace OGDC;

#define UGS_DLL_VERSION		(_U(""))	// dll版本号
#define WhiteBox_Ignore

#define UG_MMPERINCH 25.4    //毫米每英寸


//! Pi
#ifndef PI
	#define PI      3.1415926535897932384626433833
#endif

#define TILE_GROUP_SIZE 128
//! Euler constant
#define EULER   2.7182818284590452353602874713

//! Multiplier for degrees to radians
#define DTOR    0.0174532925199432957692369077

//! Multiplier for radians to degrees
#define RTOD    57.295779513082320876798154814

#define UNIT_UNKNOWN	-1   // 未知的单位
#define UNIT_LINEAR		10  // 长度单位
#define UNIT_ANGULAR    20  // 角度单位  

//{{坐标与距离单位定义 - 缩略方式
#define AU_MM	AU_MILIMETER			// mm毫米
#define AU_CM	AU_CENTIMETER			// 厘米
#define AU_DM	AU_DECIMETER			// 分米
#define AU_M	AU_METER				// 米
#define AU_KM	AU_KILOMETER			// 公里
#define AU_MI	AU_MILE					// 英里
#define AU_IN	AU_INCH					// 英寸(好象是这么多)
#define AU_YD	AU_YARD					// Yard码
#define AU_FT	AU_FOOT					// Feet英尺
#define AU_RD	0						// Radian弧度(5.0及其以前版本使用的角度单位，发现是零就当作角度计算了degree)
//}}坐标与距离单位定义


//{{坐标与距离单位定义 - 缩略方式 - MIF字符串
#define AUS_MM	(_U("mm"))	//mm毫米
#define AUS_CM	(_U("cm"))	//厘米
#define AUS_DM	(_U("dm"))	//分米
#define AUS_M	(_U("m"))		//米
#define AUS_KM	(_U("km"))	//公里
#define AUS_MI	(_U("mi"))	//英里
#define AUS_IN	(_U("in"))	//英寸
#define AUS_YD	(_U("yd"))	//Yard码
#define AUS_FT	(_U("ft"))	//Feet英尺
#define AUS_RD  (_U("dg"))    //Radian弧度

// 角度名称的字符串是按照 Arc/info PE取得 
#define AUS_RDN  (_U("Radian"))   
#define AUS_DG	 (_U("Degree"))	
#define AUS_MN	 (_U("Minute"))
#define AUS_SC   (_U("Second"))
#define AUS_GR   (_U("Grad"))          
//#define AUS_GN   ("Gon")           
#define AUS_MR	 (_U("Microradian"))
#define AUS_MC   (_U("Minute_Centesimal"))
#define AUS_SCT   (_U("Second_Centesimal"))
#define AUS_ML6000  (_U("Mil_6400"))		      

//}}坐标与距离单位定义

//{{坐标与距离单位定义 - 标准方式 - MIF字符串
#define AUS_MILIMETER	(_U("mm"))	//mm毫米
#define AUS_CENTIMETER	(_U("cm"))	//厘米
#define AUS_DECIMETER	(_U("dm"))	//分米
#define AUS_METER		(_U("m"))		//米
#define AUS_KILOMETER	(_U("km"))	//公里
#define AUS_MILE		(_U("mi"))	//英里
#define AUS_INCH		(_U("in"))	//英寸??
#define AUS_YARD		(_U("yd"))	//Yard码
#define AUS_FOOT		(_U("ft"))	//Feet英尺
#define AUS_DEGREE		(_U("dg"))	//Degree度


#define AUS_RADIAN          (_U("Radian")) 
#define AUS_DEGREE_NEW      (_U("Degree"))      
#define AUS_MINUTE			(_U("Minute")) 	    
#define AUS_SECOND			(_U("Second"))    
#define AUS_GRAD			(_U("Grad"))      
//#define AUS_GON			("Gon")               
#define AUS_MICRORADIAN		        (_U("Microradian"))    
#define AUS_MINUTE_CENTESIMAL		(_U("Minute_Centesimal"))
#define AUS_SECOND_CENTESIMAL		(_U("Second_Centesimal"))
#define AUS_MIL_6400				(_U("Mil_6400"))		      

//}}坐标与距离单位定义
//{{/* Device Parameters for GetDeviceCaps() */
// modified by zengzm 2007-3-24 各个平台都一样的,就不用再用WIN32宏分别写了
#define UG_HORZSIZE      4		/* Horizontal size in millimeters           */
#define UG_VERTSIZE      6     /* Vertical size in millimeters             */
#define UG_HORZRES       8     /* Horizontal width in pixels               */
#define UG_VERTRES       10    /* Vertical height in pixels                */
//}}/* Device Parameters for GetDeviceCaps() */

#define UG_CXFULLSCREEN         16
#define UG_CYFULLSCREEN         17
// 这里的值来源于WIN32API，使用VIRTUAL的宏获取多屏环境下的的虚拟屏幕长宽
// SM_CXVIRTUALSCREEN == 78 SM_CYVIRTUALSCREEN == 79 
// SM_CXSCREEN == 0  SM_CYSCREEN == 1
// 为了适应多屏环境，UG_C*SCREEN采用VIRTUAL的值 --lirui 2016/8/5
#define UG_CXSCREEN				78
#define UG_CYSCREEN				79

#define AM_FIELD_NAME_HEADER_UPPER	(_U("SM"))
#define AM_FIELD_NAME_HEADER		(_U("sm"))

//定义各种类型的极值 caogf
#define UG_DBLMAX OGDC_DBLMAX /* max value */
#define UG_DBLMIN OGDC_DBLMIN /* min positive value */
#define UG_FLTMAX OGDC_FLTMAX/* max value */
#define UG_FLTMIN OGDC_FLTMIN/* min positive value */

//! 通用的NoValue
#ifndef NOVALUE
	#define NOVALUE -9999
#endif

// Win32下RAND_MAX也是0x7fff,应此就别分开写了.
#define UG_RAND_MAX RAND_MAX

#define LENGTH_MAX_PATH 2048
#define LENGTH_MAX_DRIVE 5
#define LENGTH_MAX_DIR   1024
#define LENGTH_MAX_TITLE 1024
#define LENGTH_MAX_EXT   8

// 无穷大数
#define  INFINITEBIG UGC::uginfinitebig()

//add by cuiwz 2011-03-21 由于这个函数大量调用，函数调用比较浪费时间，
//修改下面的方式，会提速很多
//! \brief 判断字节序
#define  UGC_ISBIGENDIAN OGDC_ISBIGENDIAN

// Checking printf and scanf format strings
#if defined(_CC_GNU_) || defined(__GNUG__) || defined(__GNUC__)
	#define UG_PRINTF(fmt,arg) __attribute__((format(printf,fmt,arg)))
	#define UG_SCANF(fmt,arg)  __attribute__((format(scanf,fmt,arg)))
#else
	#define UG_PRINTF(fmt,arg)
	#define UG_SCANF(fmt,arg)
#endif

//! \brief  高精度的极小值定义,很多地图的比例都小于1e-10，所以导致出现很多莫名的问题，故此处定义一个高精度
#define HEP 1e-20
#define NHEP -1e-20

// 用于加载Android字体
#define ANDROID_SYSTEM_FONT_PATH _U("/system/fonts/")
#define SYSTEM_FONTS_FILE	_U("/system/etc/system_fonts.xml")
#define SYSTEM_ETC_FONTS_FILE _U("/system/etc/fonts.xml")
#define FALLBACK_FONTS_FILE _U("/system/etc/fallback_fonts.xml")
#define VENDOR_FONTS_FILE	_U("/vendor/etc/fallback_fonts.xml")

#ifdef WIN32
#define UGFINITE(v) _finite(v)
#else
#define UGFINITE(v) finite(v)
#endif

namespace UGC {

// Streamable types; these are fixed size!
typedef OgdcAchar          UGachar;
typedef OgdcUchar          UGuchar;
typedef OgdcByte		   UGbyte;
typedef OgdcBool		   UGbool;
typedef OgdcUshort         UGushort;
typedef OgdcShort          UGshort;
typedef OgdcUint           UGuint;
typedef OgdcWchar		   UGwchar;
typedef OgdcWchar32		   UGwchar32;
typedef OgdcInt            UGint;
typedef OgdcFloat          UGfloat;
typedef OgdcDouble         UGdouble;
typedef OgdcChar		   UGchar;

#if defined(_MSC_VER) || (defined(__BCPLUSPLUS__) && __BORLANDC__ > 0x500) || defined(__WATCOM_INT64__)
	#define UG_LONG
	typedef unsigned __int64       UGulong;
	typedef __int64                UGlong;	
	#define UG_ULONG_MAX		   _UI64_MAX
	#define UG_LONG_MIN			   _I64_MIN	
	#define UG_LONG_MAX			   _I64_MAX
#elif defined(__GNUG__) || defined(__GNUC__) || defined(__SUNPRO_CC) || defined(__MWERKS__) || defined(__SC__) || defined(__xlC__)
	#define UG_LONG
	typedef unsigned long long int UGulong;		//	WhiteBox_Ignore
	typedef long long int          UGlong;		//	WhiteBox_Ignore
	#define UG_ULONG_MAX		   18446744073709551615ULL	
	#define UG_LONG_MAX			   9223372036854775807LL
	#define UG_LONG_MIN			   (-UG_LONG_MAX - 1LL)	
#endif

#define UG_UINT_MAX		0xffffffffUL

// Integral types large enough to hold value of a pointer
#if defined(_MSC_VER) && defined(_WIN64)
	typedef __int64                UGival;
	typedef unsigned __int64       UGuval;
#else
	typedef long                   UGival;		//	WhiteBox_Ignore
	typedef unsigned long          UGuval;		//	WhiteBox_Ignore
#endif

// added by zengzm 2007-1-8 增加一个数值类型,
//	专门用来存储指针类型,以解决32位和64位下指针类型长度不一样的问题
typedef UGuval UGptr;
typedef UGuint                 UGColor;
typedef UGuint				   UGPixel;
typedef OgdcSizeT              UGSizeT;


typedef struct UGRGB16  // 16位色彩
{
	UGushort b : 5;		// 蓝，占5位
	UGushort g : 6;		// 绿，占6位	
	UGushort r : 5;		// 红，占5位

}UGRGB16, *LPUGRGB16;

typedef struct UGRGB24  //24位真彩
{
	UGbyte blue;		//蓝，占8位
	UGbyte green;		//绿，占8位
	UGbyte red;			//红，占8位	
}UGRGB24, *LPUGRGB24;

typedef struct UGRGBA32  //32位真彩
{
	UGbyte blue;		//蓝，占8位
	UGbyte green;		//绿，占8位
	UGbyte red;			//红，占8位	
	UGbyte alpha;		//alpha，占8位	
}UGRGB32, *LPUGRGB32;

typedef OgdcRect2D UGRect2D;
typedef OgdcRect UGRect;
typedef OgdcRectF UGRectF;
typedef OgdcSize2D UGSize2D;
typedef OgdcSize UGSize;
typedef OgdcVariant UGVariant;
typedef OgdcMBString UGMBString;
typedef OgdcUnicodeString UGUnicodeString;
typedef OgdcString UGString;
typedef OgdcColorset UGColorset;
typedef OgdcColorTable UGColorTable;
typedef OgdcColorDictTable UGColorDictTable;
typedef OgdcHistogram UGHistogram;
typedef OgdcTime UGTime;
typedef OgdcTimeSpan UGTimeSpan;
typedef OgdcPoint UGPoint;
typedef OgdcPoint2D UGPoint2D;
typedef OgdcPoint3D UGPoint3D;
typedef OgdcPointEPS UGPointEPS;
//这两个请直接Include Geometry/UGGeometry.h
//typedef OgdcRasterBlock UGImgBlock;
//typedef OgdcRasterScanline UGImgScanline;
typedef OGDC_ENUM_STOCK_GRADIENT_SET_STYLE UG_ENUM_STOCK_GRADIENT_SET_STYLE;

//! \brief UGPCTSTR为了不与微软的LPCTSTR冲突，因为指针可能是4位或者8位，去掉L，加上UG前缀。
#define UGPCTSTR OGDCPCTSTR 

#define UGArray OgdcArray
#define UGList OgdcList
#define UGDict OgdcDict

typedef UGArray<UGString> UGStrArray;
typedef UGArray<UGString> UGStrings; 

typedef UGArray<UGPoint2D> UGPoint2Ds;
typedef UGArray<UGPoint3D> UGPoint3Ds;
typedef UGArray<UGPoint> UGPoints;
typedef UGArray<UGPointEPS> UGPointEPSs;

#ifdef _UGUNICODE
#if _MSC_VER == 1200
#define __STR2WSTR(str)    L##str
#define _STR2WSTR(str)     __STR2WSTR(str)
#define __FILEW__         _STR2WSTR(__FILE__)
#endif
#define  __UGFILE__ __FILEW__ 
#else
#define	 __UGFILE__ __FILE__ 
#endif

//是否为极值
#define UGEXTREFLT(x) OGDCEXTREFLT(x)
#define UGEXTREDBL(x) OGDCEXTREDBL(x)

//! Abolute value
#define UGABS(val) (((val)>=0)?(val):-(val))

//! Return the maximum of a or b
#define UGMAX(a,b) OGDCMAX(a,b)

//! Return the minimum of a or b
#define UGMIN(a,b) OGDCMIN(a,b)

//! Return the minimum of x, y and z
#define UGMIN3(x,y,z) ((x)<(y)?UGMIN(x,z):UGMIN(y,z))

//! Return the maximum of x, y and z
#define UGMAX3(x,y,z) ((x)>(y)?UGMAX(x,z):UGMAX(y,z))

//! Swap a pair of numbers
#define UGSWAP(a,b,t) OGDCSWAP(a,b,t)

//! Clamp value x to range [lo..hi]
#define UGCLAMP(lo,x,hi) OGDCCLAMP(lo,x,hi)
// ceil 四舍五入
//#define UGROUND(x) (((x)>0)?UGint((x)+0.5):(UGint((x)-0.5)))
#define UGROUND(x) OGDCROUND(x)

// ceil 数值
#define UGCEIL(x) ((UGint)ceil(x))

// floor 数值
#define UGFLOOR(x) ((UGint)floor(x))

//获取低位
#define UGLOWORD(l)           ((UGushort)(l))

//获取高位
#define UGHIWORD(l)           ((UGushort)(((UGuint)(l) >> 16) & 0xFFFF))

//四舍五入宏
#ifndef ROUNDLONG
	#define ROUNDLONG(x) ((x>0)?UGlong(x+0.5):(UGlong(x-0.5)))	
#endif
// 两个数是否相等的判断
#define UGEQUAL(x,y) OGDCEQUAL(x,y)
// 两个数是否相等的判断
#define UGEQ(x,y,d) OGDCEQ(x,y,d) 

// 判断double是否是一个非法值(如：1.#QNAN0000000)  NAN：not a number
#define UGISNAN(x) !UGEQUAL(x,x)


// 一个数是否为0 (或者0附近的极小数) 的判断
// 重要提示: 两个数是否相等的比较 请勿使用 UGIS0(a-b) , 而应该直接使用UGEQUAL(a,b)
#define  UGIS0(x)	OGDCIS0(x)
// 与UGIS0(x)功能相同，但是精度高要一倍，判断容限更小
#define  UGHIS0(x)	(((x)<HEP) && ((x)>NHEP))

// 与UGIS0(x)功能相同，但是精度为13位,和iserver保持一致，原来15位还有问题
#define  UGMIS0(x)	(((x)<1e-13) && ((x)>-1e-13))
//! 在UGC中全面禁止使用_T宏的说明
//! 在MFC中,CString在定义MBCS宏时,内部使用的是char*,在定义UNICODE宏时,内部使用的是wchar*;
//! 因此,在给CString赋值时,字符串前都添加_T,以便代码可以在MBCS和UNICODE两种不同的编码方式编译下都可行.
//! SFC是基于MFC开发的,因此也继承了这种使用习惯, 由于UGC很多代码从SFC移植而来, 很多地方也都使用_T宏.
//! 但是, 这种做法在UGC中是不必要,甚至导致问题的. 原因如下: 1)UGString内部限定于char*,使用_T宏起不了作用,
//! 事实上,现在就是#define _U(str) str; 2)系统的很多地方都定义了_T宏,UGC内部再重复定义,会导致编译警告和混乱;
//! 3)在WinCE等系统中,由于其系统内的字符串固定为Unicode,即#define _U(str) L(str), UGC中再定义和使用_T,
//! 要么导致UGC内的字符串错误(前面加了不应该有的L),要么导致系统的字符串错误(_T被取消了)
//! 因此, 最好的做法是:1)在UGC中,不定义_T宏; 2)UGC中的字符串前不添加_T; 3)给UGStringW赋值时,添加L(UGStringW采用wstring实现)
//! 所以, 这里取消UGC中的_T宏(再使用会导致编译错误), 并把UGC中所有_T删除; 另外,请再编写或移植代码的时候注意删除_T

//! \brief assert exp.
#define UGASSERT(exp) OGDCASSERT(exp)
//! \brief trace argument.
#ifndef NDEBUG
	#define UGTRACE(arguments) UGC::ugtrace arguments
#else
	#define UGTRACE(arguments) ((void)0)
#endif
//! \brief malloc 内存
#define UGMALLOC(ptr,type,no)     (UGC::ugmalloc((void **)(ptr),(UGSizeT)(sizeof(type)*(no))))	
//! \brief calloc 内存
#define UGCALLOC(ptr,type,no)     (UGC::ugcalloc((void **)(ptr),(UGSizeT)(sizeof(type)*(no))))	
//! \brief 重新设置指针内存大小
#define UGRESIZE(ptr,type,no)     (UGC::ugresize((void **)(ptr),(UGSizeT)(sizeof(type)*(no))))
//! \brief 内存拷贝
#define UGMEMDUP(ptr,src,type,no) (UGC::ugmemdup((void **)(ptr),(const void*)(src),(UGSizeT)(sizeof(type)*(no))))
//! \brief 释放空间
#define UGFREE(ptr)               (UGC::ugfree((void **)(ptr)))
//dpi(每英寸象素个数)
#define UGDPI	(UGC::ugdpi())

//! \brief trace内存
extern STREAM_API void ugtrace(const UGchar* format,...);

//! \brief malloc ptr内存
extern STREAM_API UGint ugmalloc(void** ptr,UGSizeT size);

//! \brief calloc ptr内存
extern STREAM_API UGint ugcalloc(void** ptr,UGSizeT size);

//! \brief 重新设置指针内存大小
extern STREAM_API UGint ugresize(void** ptr,UGSizeT size);

//! \brief 内存拷贝
extern STREAM_API UGint ugmemdup(void** ptr,const void* src,UGSizeT size);

//! \brief 释放空间
extern STREAM_API void ugfree(void** ptr);

//dpi(每英寸象素个数)
extern STREAM_API UGint ugdpi();

//! \brief 获取当前的最大double数值
extern STREAM_API UGdouble uginfinitebig();

//! \brief linux和windoiws的路径分隔符 '/','\\',在移除分隔符的地方使用， add by qianjn
extern STREAM_API UGchar PathSet[];

//! \brief 通过四个颜色构造出一个RGBA颜色值
// extern STREAM_API UGColor UGRGBA(UGbyte r,UGbyte g,UGbyte b,UGbyte a) ;

//! \brief 获取颜色的ALPHA值
//extern STREAM_API UGuchar UGALPHAVAL(UGColor rgba);


//! \brief 通过三个颜色构造出一个RGB颜色值
//extern STREAM_API UGColor UGRGB(UGbyte r,UGbyte g,UGbyte b);
#define  UGRGB(r,g,b) OGDCRGB((OgdcByte)(r),(OgdcByte)(g),(OgdcByte)(b))
#define  UGRGBA(r,g,b,a) OGDCRGBA((OgdcByte)(r),(OgdcByte)(g),(OgdcByte)(b),(OgdcByte)(a))
#define  UGREDVAL(rgba) OGDCREDVAL(rgba)
#define  UGGREENVAL(rgba) OGDCGREENVAL(rgba)
#define  UGBLUEVAL(rgba) OGDCBLUEVAL(rgba)
#define  UGALPHAVAL(rgba) OGDCALPHAVAL(rgba)

//! \brief 判断两个颜色是否一
extern STREAM_API UGbool UGISSAMERGB(UGColor color1, UGColor color2);

//! \brief windows上的color（存储标准）转换成当前平台的color，涉及到读取Color的地方都需要用它来转换
extern STREAM_API UGColor UGWinColortoCurrentColor(UGColor rgba);

//! \brief 当前平台的color转换成windows上的color(存储标准)，涉及到存储Color的地方都需要用它来转换
extern STREAM_API UGColor UGCurrentColortoWinColor(UGColor rgba);

//! \brief 当前平台的color转换成windows上的color(存储标准)，涉及到存储Color的地方都需要用它来转换
extern STREAM_API UGColor UGCurrentColortoWinColorRGB(UGColor rgb);

//! \brief 该函数能够将任何POD类型的值改为与之endian相反的对应值。该函数没有检查传入的类型是否为POD！
template<typename T>
inline T UGEndianConvert(const T& value) {
	size_t nSize = sizeof(value);
	T ret = 0;
	for(size_t i = 0; i < nSize; ++i) {
		ret = ret | (((value >> (i * CHAR_BIT)) & 0xff) << (CHAR_BIT * (nSize - 1 - i)));
	}
	return ret;
}

//API PACKAGE
#define PACKAGE_UGC 0
#define PACKAGE_UGE 1 
}

#ifdef OS_ANDROID
	#define UGNOTHROW 
#else
	#define UGNOTHROW (nothrow)
#endif

#ifdef IOS
using namespace UGC;
#endif
#endif //#ifndef UGDEFS_H




