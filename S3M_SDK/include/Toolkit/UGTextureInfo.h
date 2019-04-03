#ifndef UGTEXTUREWRAP_H
#define  UGTEXTUREWRAP_H

#include "Stream/ugdefs.h"
#include "Base/OgdcString.h"
#include "Stream/UGStream.h"

#define MACHINE_TAG (_U("TexImage@")) 

namespace UGC
{
	class TOOLKIT_API UGTextureInfo
	{
	public:
		UGTextureInfo();
		~UGTextureInfo();
		const UGTextureInfo& operator=(const UGTextureInfo& Info);
		UGTextureInfo(const UGTextureInfo& Info);
		// 接管了外边给的内存
		UGTextureInfo(OGDC::PixelFormat enFormat,OgdcUint Width,OgdcUint Height,void* data,OgdcUint nSize, const OgdcString& path = _U(""));

		//! \brief 从流文件中加载
		//! \param fStream 文件流[in]。
		void Load(UGStream& fStream);

		//! \brief 存入流文件中
		//! \param fStream 文件流[in]。
		void Save(UGStream& fStream);

		//! \brief 获取数据存储时的大小
		//! \return 返回存储大小
		//! \remark 暂未考虑编码情况，与Save()一起维护
		//! \attention 字符串的长度要+4
		UGint GetDataSize();
	public:
		UGbool			m_bLoadImage;

		// 纹理图片的像素格式
		OGDC::PixelFormat		m_enFormat;

		// 纹理图片的宽度
		OgdcUint		m_nWidth;

		// 纹理图片的高度
		OgdcUint		m_nHeight;

		// 纹理图片流数据
		OgdcByte*		m_pBuffer;

		// 纹理图片流数据的长度,带压缩时很重要
		OgdcUint		m_nSize;

		// 纹理图片的索引路径
		OgdcString		m_strPath;

		OgdcString		m_strName;

		// 纹理图片数据中一共有几层mipmap
		OgdcUint        m_nMipLev;

		// 纹理图片的压缩类型
		OgdcInt			m_nCompress;
	};
}
#endif

