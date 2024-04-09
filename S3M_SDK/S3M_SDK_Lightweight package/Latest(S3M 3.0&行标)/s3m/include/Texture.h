#if !defined(TEXTURE_H__7B66F789_1A72_4B27_8D89_5557F1D5CC57__INCLUDED_)
#define TEXTURE_H__7B66F789_1A72_4B27_8D89_5557F1D5CC57__INCLUDED_

#pragma once
#include "S3MBEnums.h"
#include "MemoryStream.h"
#include <string>
#include <iostream>

using namespace std;

namespace S3MB
{	
	// 纹理数据的封装类。
	class S3MB_API TextureData
	{
	public:
		// 构造函数
		TextureData();
		// 拷贝构造函数
		TextureData(const TextureData& other);
		// 赋值函数
		TextureData& operator=(const TextureData& other);

		// 克隆自己的数据
		virtual TextureData* Clone();
		// 析构函数
		virtual ~TextureData();
		// 释放纹理数据
		void Release();

	private:
#if defined OPENGL_ES_VERSION2
		// 压缩类型由DXT转抽象为NONE
		void CompressTypeDXTToNONE();
#endif
	public:
		// 纹理数据内存
		unsigned char* m_pBuffer;
		// 纹理的高
		unsigned int m_nHeight;
		// 纹理的宽
		unsigned int m_nWidth;
		// 纹理的深度
		unsigned int m_nDepth;
		// 纹理的像素格式
		PixelFormat m_enFormat;

		// 压缩纹理类型
		TextureCompressType m_eCompressType;

		// 数据的内存大小
		unsigned int m_nSize;
	};

	struct S3MB_API TextureDataInfo
	{
		TextureData* m_pTextureData;

		bool m_bMipmap;

		int m_nLevel;

		std::wstring m_strName;

		TextureDataInfo();

		TextureDataInfo(const TextureDataInfo& other);

		TextureDataInfo& operator = (const TextureDataInfo& other);

		~TextureDataInfo();
	};
}
#endif