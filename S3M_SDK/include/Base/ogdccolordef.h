#if !defined(OGDCCOLORDEF_H)
#define OGDCCOLORDEF_H

namespace OGDC
 {
//! \brief 象素格式
enum PixelFormat 
{
	//! \brief 尚未设定, 内部使用
	IPF_UNKNOWN			= 0,
	//! \brief 1位,单色
	IPF_MONO			= 1,
	//! \brief 4位,16色
	IPF_FBIT			= 4,
	//! \brief 8位无符号,256色
	IPF_UBYTE			= 8,
	//! \brief 8位有符号,256色,新增
	IPF_BYTE			= 80,
	//! \brief 16位有符号,彩色
	IPF_TBYTE			= 16,
	//! \brief 16位无符号,彩色，新增
	IPF_UTBYTE			= 160,
	//! \brief 24位,真彩色
	IPF_RGB				= 24,
	//! \brief 32位,增强真彩色
	IPF_RGBA			= 32,
	//! \brief 48位,真彩色
	IPF_TRGB			= 48,
	//! \brief 64位,长整型
	IPF_LONGLONG		= 64,
	//! \brief LONG
	IPF_LONG			= 320,
	//! \brief LONG
	IPF_ULONG			= 321,
	//! \brief 32位,浮点型
	IPF_FLOAT			= 3200,
	//! \brief 96位格式，R通道32位（float），B通道32位（float），G通道32位（float）
	IPF_FLOAT32_RGB     = 4800,
	//! \brief 64位,双精度浮点型
	IPF_DOUBLE			= 6400	
};

//! \brief 色彩空间格式类型.
typedef enum
{
	//! \brief 未知色彩空间
	ICS_UNKNOW =0,
		//! \brief 显示系统使用.
		ICS_RGB = 1,
		//! \brief 显示系统使用.
		ICS_RGBA = 2,
		//! \brief 印刷系统使用(Cyan,Magenta,Yellow).
		ICS_CMY = 3,
		//! \brief 印刷系统使用(Cyan,Magenta,Yellow,Black)
		ICS_CMYK = 4,
		//! \brief 北美电视系统使用(NTSC,Y:Luminance,Chrominance).
		ICS_YIQ = 5,
		//! \brief 欧洲电视系统使用(PAL).
		ICS_YUV = 6,
		//! \brief 与YUV类似,JPEG使用.
		ICS_YCC = 7		
} ImgColorSpace;

//! \brief 定义象素格式的位数为1位.
#define IPF_MONO_BITS 1
//! \brief 定义象素格式的位数为4位.
#define IPF_FBIT_BITS 4
//! \brief 定义象素格式的位数为8位.
#define IPF_BYTE_BITS 8
//! \brief 定义象素格式的位数为16位.
#define IPF_TBYTE_BITS 16
//! \brief 定义象素格式的位数为24位.
#define IPF_RGB_BITS  24
//! \brief 定义象素格式的位数为32位.
#define IPF_RGBA_BITS 32

//! \brief 定义象素格式的颜色数.
#define IPF_MONO_COLOR	0x00000001
//! \brief 定义象素格式的颜色数.
#define IPF_FBIT_COLOR	0x0000000F
//! \brief 定义象素格式的颜色数.
#define IPF_BYTE_COLOR	0x000000FF
//! \brief 定义象素格式的颜色数.
#define IPF_TBYTE_COLOR 0x0000FFFF
//! \brief 定义象素格式的颜色数.
#define IPF_RGB_COLOR	0x00FFFFFF
//! \brief 定义象素格式的颜色数.
#define IPF_RGBA_COLOR	0xFFFFFFFF

///////////////////////////////////////////////////////////////////////
//! \brief 得到以Byte为单位的DWORD对齐的宽度.
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 
//! \brief 得到以Byte为单位的DWORD对齐的宽度.
#define WIDTHBYTESBYTE(bits)    (((bits) + 7) / 8 )  
//! \brief 得到以Byte为单位的DWORD对齐的宽度.
#define WIDTHBYTESWORD(bits)    (((bits) + 15) / 16 * 2)  
}

#endif //OGDCCOLORDEF_H

