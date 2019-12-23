// 内存图像处理类,包括一些常用的图像处理
// 此类的所有方法均为静态方法
// 作者:王茂
// 日期:2010-12-15
// 版本:1.0.0

#ifndef  MEMIMAGE_H
#define MEMIMAGE_H

#ifdef WIN32
	#ifdef MEMIMAGE_EXPORTS
		#define MEMIMAGE_API __declspec(dllexport)
	#else
		#define MEMIMAGE_API __declspec(dllimport)
	#endif
#else
	#define MEMIMAGE_API 
#endif

class MEMIMAGE_API MemImage
{
public:
	// format 原图像的像素格式
	// width,height 原图像的宽度和高度
	// in 原图像的数据
	// comtype 图像压缩类型
	// out  压缩后的输出,外部必须分配足够的空间
	// return 返回压缩后图像数据的大小
	static unsigned int Encode( const unsigned int pixsize, unsigned int width, unsigned int height,const unsigned char* in,unsigned char *out );

	// 解压缩dxtn
	static void Decode( const unsigned int pixsize, unsigned int width, unsigned int height,unsigned char* out,const unsigned char *in );

	// 按照指定的大小缩放图片
	static void Scale(const unsigned int components, unsigned int widthin, unsigned int heightin,unsigned char *in,
					        unsigned int widthout, unsigned int heightout, unsigned char *out);

	// RGB,RGBA 到 BGR,BRGA之间相互转换,直接改变原始数据
	// RGB时components = 3,RGBA components = 4
	static void ImageSwapRB(const unsigned int components, unsigned int widthin, unsigned int heightin,unsigned char *in);
};
#endif
