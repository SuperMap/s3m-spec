#include "Texture.h"
#include "ImageOperator.h"
#include "S3MBUtils.h"
#include "zlib/include/zlib.h"
#include <string.h>

namespace S3MB
{
	TextureData::TextureData()
	{
		m_pBuffer = NULL;
		m_nWidth = 0;
		m_nHeight = 0;
		m_nDepth = 1;
		m_enFormat = PF_RGB8;
		m_nSize = 0;
		m_eCompressType = TextureCompressType::TC_NONE;
	}

	TextureData::TextureData(const TextureData& other)
	{
		m_nWidth = other.m_nWidth;
		m_nHeight = other.m_nHeight;
		m_nDepth = other.m_nDepth;
		int nLen = 0;
		if (other.m_enFormat == PF_RGB8)
		{
			nLen = m_nWidth * m_nHeight * 3;
		}
		else
		{
			nLen = m_nWidth * m_nHeight * 4;
		}

		m_pBuffer = NULL;
		m_enFormat = other.m_enFormat;
		m_nSize = other.m_nSize;
		m_eCompressType = other.m_eCompressType;
		if (nLen > 0 || m_nSize > 0)
		{
			if (m_eCompressType == 0)
			{
				m_pBuffer = new unsigned char[nLen];
				memcpy(m_pBuffer, other.m_pBuffer, nLen);
			}
			else
			{
				m_pBuffer = new unsigned char[m_nSize];
				memcpy(m_pBuffer, other.m_pBuffer, m_nSize);
			}
		}
		else
		{
			m_pBuffer = NULL;
		}
	}

	TextureData* TextureData::Clone()
	{
		return new TextureData(*this);
	}

	TextureData& TextureData::operator=(const TextureData& other)
	{
		if (this == &other)
		{
			return *this;
		}
		if (m_pBuffer != NULL)
		{
			delete[] m_pBuffer;
			m_pBuffer = NULL;
		}
		m_nWidth = other.m_nWidth;
		m_nHeight = other.m_nHeight;
		m_nDepth = other.m_nDepth;
		int nLen = 0;
		if (other.m_enFormat == PF_RGB8)
		{
			nLen = m_nWidth * m_nHeight * m_nDepth * 3;
		}
		else
		{
			nLen = m_nWidth * m_nHeight * m_nDepth * 4;
		}
		m_enFormat = other.m_enFormat;
		m_nSize = other.m_nSize;
		m_eCompressType = other.m_eCompressType;
		if (nLen > 0)
		{
			if (m_eCompressType == 0)
			{
				m_pBuffer = new unsigned char[nLen];
				memcpy(m_pBuffer, other.m_pBuffer, nLen);
			}
			else
			{
				m_pBuffer = new unsigned char[m_nSize];
				memcpy(m_pBuffer, other.m_pBuffer, m_nSize);
			}
		}
		return *this;
	}

	TextureData::~TextureData()
	{
		Release();
	}

	void TextureData::Release()
	{
		if (m_pBuffer != NULL)
		{
			delete[]m_pBuffer;
			m_pBuffer = NULL;
			m_nSize = 0;
		}
	}

#if defined OPENGL_ES_VERSION2
	void TextureData::CompressTypeDXTToNONE()
	{
		if (m_pBuffer != NULL)
		{
			// 解压DXT
			const unsigned int comp = 4;
			unsigned char* pOut = NULL;
			unsigned int nSize = ImageOperator::Decode(comp, m_nWidth, m_nHeight, &pOut, m_pBuffer, TextureCompressType::TC_DXT5);

			if (pOut != NULL)
			{
				// 释放原有压缩数据
				Release();

				// 转为非压缩类型
				m_eCompressType = 0;
				m_nSize = nSize;
				m_enFormat = PF_RGBA8;// 在读取的时候已处理成RGBA

				m_pBuffer = new(std::nothrow) unsigned char[m_nSize];
				int i = 0, j = 0, k = 0;
				for (i = 0; i < m_nHeight * m_nWidth; i++, j += 4, k += 4)// 压缩时做了转换，再转换回来
				{
					m_pBuffer[j] = pOut[k + 2];	// r
					m_pBuffer[j + 1] = pOut[k + 1];	// g
					m_pBuffer[j + 2] = pOut[k];	// b
					m_pBuffer[j + 3] = pOut[k + 3]; // a
				}

				delete[] pOut;
				pOut = NULL;
			}
		}
	}
#endif

	TextureDataInfo::TextureDataInfo()
	{
		m_pTextureData = NULL;
		m_bMipmap = false;
		m_nLevel = 0;
	}

	TextureDataInfo::TextureDataInfo(const TextureDataInfo& other)
	{
		m_pTextureData = NULL;
		*this = other;
	}

	TextureDataInfo& TextureDataInfo::operator = (const TextureDataInfo& other)
	{
		if (m_pTextureData != NULL)
		{
			delete m_pTextureData;
			m_pTextureData = NULL;
		}

		if (other.m_pTextureData != NULL)
		{
			m_pTextureData = new TextureData(*other.m_pTextureData);
		}

		m_bMipmap = other.m_bMipmap;
		m_nLevel = other.m_nLevel;
		m_strName = other.m_strName;
		return *this;
	}

	TextureDataInfo::~TextureDataInfo()
	{
		if (m_pTextureData != NULL)
		{
			delete m_pTextureData;
			m_pTextureData = NULL;
		}
	}
}