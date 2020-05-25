#pragma once
#include "ImageOperator/include/MemImage.h"
#include <vector>
#include "Palette.h"
#include "Texture.h"

extern "C"
{
#define JPEG_CJPEG_DJPEG	/* define proper options in jconfig.h */
#define JPEG_INTERNAL_OPTIONS	/* cjpeg.c,djpeg.c need to see xxx_SUPPORTED */
#include "jpeg/include/jinclude.h"
#include "jpeg/include/jpeglib.h"
#include "jpeg/include/jerror.h"
}

#include "png/include/png.h"
#include "png/include/pngconf.h"

#include "gdal_priv.h"

using namespace std;
namespace S3MB
{
#define DXT1_ENCODE_SIZE(w,h) (((w-1)/4 + 1)*((h-1)/4 + 1)*8)
#define DXT3_ENCODE_SIZE(w,h) (((w-1)/4 + 1)*((h-1)/4 + 1)*16)
#define DXT5_ENCODE_SIZE DXT3_ENCODE_SIZE
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
//! \brief 得到以unsigned char为单位的DWORD对齐的宽度.
#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4) 

	class ImageOperator
	{
	public:
		// format 原图像的像素格式
		// width,height 原图像的宽度和高度
		// in 原图像的数据
		// comtype 图像压缩类型
		// out  压缩后的输出,内部分配空间
		// return 返回压缩后图像数据的大小
		static unsigned int Encode(const unsigned int nPixSize, unsigned int& nWidth, unsigned int& nHeight, unsigned char* pBufferIn, unsigned char** ppBufferOut, unsigned int eCodecType = enrS3TCDXTN, bool bGeneMipmaps = false);

		// 解压缩dxtn
		static unsigned int Decode(const unsigned int nPixSize, unsigned int nWidth, unsigned int nHeight, unsigned char** ppBufferOut, const unsigned char* pBufferIn, unsigned int eCodecType, bool bGeneMipmaps = false);

		// 按照指定的大小缩放图片
		static void Scale(const unsigned int components, unsigned int widthin, unsigned int heightin, unsigned char* pBufferIn,
			unsigned int widthout, unsigned int heightout, unsigned char* pBufferOut);

		//将影像数据缩放成正方形
		static unsigned char* MakeSquareForPVR(unsigned int& nWidth, unsigned int& nHeight, unsigned char* pBufferIn);

		// 获取mipmap的数据
		static unsigned int GetMipMapData(unsigned int nWidth, unsigned int nHeight, const unsigned char* pBufferIn, unsigned char** ppBufferOut, vector<unsigned int>& arrByteOffset);

		//! \brief  返回比a大的，并且是最接近a的2的次方的数。
		static unsigned int NextP2(unsigned int a);

		//! \brief 获取指定压缩格式的纹理压缩后的大小
		//! \brief
		static unsigned int GetCompressedTextureSize(const unsigned int nPixSize, unsigned int nWidth, unsigned int nHeight, unsigned int eCodecType, bool bIsMipmaps = false);

		//! \brief 根据宽高获取mipMap层级
		static unsigned int GetMipMapLevel(const unsigned int nWidth, const unsigned int nHeight);
		//
		//
		//	/** 转化为纹理地图集
		//		@remarks
		//		将textInfo的宽高均扩大一倍
		//		@note
		//		新的纹理左上1/4存储原来纹理的RGB,左下角1/4存储alpha,新纹理右半部分为空
		//	*/
		//	static void TextureToAtlas(TextureInfo& textInfo);
		//
		//	/** 纹理地图集转化为纹理
		//		@remarks
		//			将textInfo的宽高均缩小为原来的一半
		//		@note
		//			新的纹理的RGB来自原纹理左上1/4部分,alpha来自原纹理左下1/4部分
		//	*/
		//	static void AltasToTexture(TextureInfo& textInfo);
		//
		//	//! \是否是压缩纹理
		//	static bool IsCompressedTextureType(CodecType eType);
		//
		//#if defined OPENGL_ES_VERSION2
		//	//! \brief get the pvr file contenie
		//	static void GetPvrTextureData(const string &strFilePath, UGTextureInfo& textData);
		//	//! \brief get the pvr file contenie
		//	static void GetKtxTextureData(const string &strFilePath, UGTextureInfo& textData);
		//#endif
		//
		//private:
		//
		//
#if _MSC_VER > 1200
	//! \brief 根据UGC编码获取pvr像素格式
	//static unsigned int GetPVRPixelFormat(unsigned int eCodecType, unsigned int nComponent);
#endif
	};
	class ImgToolkit
	{
	public:
		//==============================================================
		//根据象素格式和象素宽度计算DWORD对齐的按照unsigned char计的实际需要宽度.
		//! \brief
		  //!  \brief Toolkit
		static unsigned int AlignWidth(PixelFormat PixelFormat, unsigned int lPixelWidth);
		static unsigned int GetValue(const unsigned char* pByte, int nWidthBytes, PixelFormat pixelFormat, unsigned int x, unsigned int y);
		//! \brief
		  //!  \brief Toolkit
		static void SetValue(unsigned char* pByte, int nWidthBytes, PixelFormat pixelFormat, unsigned int x, unsigned int y, unsigned int nValue);
		static TextureData* GetTextureData(string filePath, bool bReverse = true);
		static bool JpgToBuffer(string filePath, TextureData*& pTextureData, unsigned int& bufferSize, unsigned char*& pBits, int& format, bool bReverse = true);
		static bool PngToBuffer(string filePath, TextureData*& pTextureData, unsigned int& BufferSize, unsigned char*& pBits, int& format, Palette& palette, bool bReverse = true);
		static bool GDALToBuffer(string filePath, TextureData*& pTextureData, unsigned int& BufferSize, unsigned char*& pBits, int& format, Palette& palette, bool bReverse = true);
		static bool BufferToTextureData(TextureData*& pTextureData, unsigned int bufferSize, unsigned char*& pBits, int format, Palette palette);
		//===================================================
		//! \brief  通过位深得到jpg像素格式
		static PixelFormat JpgBit2Format(unsigned short nBitCount, struct jpeg_decompress_struct m_dinfo);
	};
}