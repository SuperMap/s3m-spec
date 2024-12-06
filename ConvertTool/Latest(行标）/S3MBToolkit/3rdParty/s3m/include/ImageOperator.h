#if !defined(IMAGEOPERATOR_H__506D4D04_3FA2_4009_9B39_77F396F82FE8__INCLUDED_)
#define IMAGEOPERATOR_H__506D4D04_3FA2_4009_9B39_77F396F82FE8__INCLUDED_

#pragma once
#include "Palette.h"
#include "S3MBCommon.h"
#include "Texture.h"
#include <vector>

namespace S3MB
{
	class S3MB_API ImageOperator
	{
	public:
		// format 原图像的像素格式
		// width,height 原图像的宽度和高度
		// in 原图像的数据
		// comtype 图像压缩类型
		// out  压缩后的输出,内部分配空间
		// return 返回压缩后图像数据的大小
		static unsigned int Encode(const unsigned int nPixSize, unsigned int& nWidth, unsigned int& nHeight, unsigned char* pBufferIn, unsigned char** ppBufferOut, unsigned int eCodecType = TC_DXT5, bool bGeneMipmaps = false);

		// 解压缩dxtn
		static unsigned int Decode(const unsigned int nPixSize, unsigned int nWidth, unsigned int nHeight, unsigned char** ppBufferOut, const unsigned char* pBufferIn, unsigned int eCodecType, bool bGeneMipmaps = false);

		// 按照指定的大小缩放图片
		static void Scale(const unsigned int components, unsigned int widthin, unsigned int heightin, unsigned char* pBufferIn,
			unsigned int widthout, unsigned int heightout, unsigned char* pBufferOut);
		// 获取mipmap的数据
		static unsigned int GetMipMapData(unsigned int nWidth, unsigned int nHeight, const unsigned char* pBufferIn, unsigned char** ppBufferOut, vector<unsigned int>& arrByteOffset);

		// 获取指定压缩格式的纹理压缩后的大小
		static unsigned int GetCompressedTextureSize(const unsigned int nPixSize, unsigned int nWidth, unsigned int nHeight, unsigned int eCodecType, bool bIsMipmaps = false);

		// 根据宽高获取mipMap层级
		static unsigned int GetMipMapLevel(const unsigned int nWidth, const unsigned int nHeight);
	};

	class S3MB_API ImgToolkit
	{
	public:
		// ==============================================================
		// 根据象素格式和象素宽度计算DWORD对齐的按照unsigned char计的实际需要宽度.
		//
		static unsigned int AlignWidth(PixelFormat PixelFormat, unsigned int lPixelWidth);
		static unsigned int GetValue(const unsigned char* pByte, int nWidthBytes, PixelFormat pixelFormat, unsigned int x, unsigned int y);
		//
		static void SetValue(unsigned char* pByte, int nWidthBytes, PixelFormat pixelFormat, unsigned int x, unsigned int y, unsigned int nValue);
		static TextureData* GetTextureData(wstring filePath, bool bReverse = true);
		static bool JpgToBuffer(wstring filePath, TextureData*& pTextureData, unsigned int& bufferSize, unsigned char*& pBits, int& nBitCnt, bool bReverse = true);
		static bool PngToBuffer(wstring filePath, TextureData*& pTextureData, unsigned int& BufferSize, unsigned char*& pBits, int& nBitCnt, Palette& palette, bool bReverse = true);
		static bool BufferToTextureData(TextureData*& pTextureData, unsigned int bufferSize, unsigned char*& pBits, int nBitCnt, Palette palette);
		// ===================================================
	};

	class S3MB_API MemImage
	{
	public:
		// format 原图像的像素格式
		// width,height 原图像的宽度和高度
		// in 原图像的数据
		// comtype 图像压缩类型
		// out  压缩后的输出,外部必须分配足够的空间
		// return 返回压缩后图像数据的大小
		static unsigned int Encode(const unsigned int pixsize, unsigned int width, unsigned int height, const unsigned char* in, unsigned char *out);

		// 解压缩dxtn
		static void Decode(const unsigned int pixsize, unsigned int width, unsigned int height, unsigned char* out, const unsigned char *in);

		// 按照指定的大小缩放图片
		static void Scale(const unsigned int components, unsigned int widthin, unsigned int heightin, unsigned char *in,
			unsigned int widthout, unsigned int heightout, unsigned char *out);

	};
}

#endif