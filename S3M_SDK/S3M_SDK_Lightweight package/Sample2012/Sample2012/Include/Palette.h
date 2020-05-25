#ifndef UGPALETTE_H
#define UGPALETTE_H

#include <new>
#include <vector>
using namespace std;
namespace S3MB
{

	static union
	{
		int nTest;
		char cTest[sizeof(int)];
	}un_bigendina = { 1 };
#define  ISBIGENDIAN (un_bigendina.cTest[0] != 1)
	typedef unsigned int UGColor;
	class Palette
	{
	public:
		//! \brief
		struct PaletteEntry  //! \brief 一个调色板信息
		{
			unsigned char peRed;      //! \brief 红色的比重
			unsigned char peGreen;    //! \brief 绿色的比重
			unsigned char peBlue;     //! \brief 蓝色的比重
			unsigned char peFlags;    //! \brief 标识符

			PaletteEntry()
			{
				peRed = 0;
				peGreen = 0;
				peBlue = 0;
				peFlags = 0;
			}
		};

	public:
		//!  \brief Toolkit
		Palette();
		//!  \brief Toolkit
		~Palette();

	public:
		static UGColor UGRGB(unsigned int r, unsigned int g, unsigned int b);
		static UGColor RGBA(unsigned int r, unsigned int g, unsigned int b, unsigned int a);
		static unsigned char REDVAL(UGColor clr);

		static unsigned char GREENVAL(UGColor clr);

		static unsigned char BLUEVAL(UGColor clr);

		static unsigned char ALPHAVAL(UGColor clr);
		//! \brief 查找调色板的位置
		//! \param paletteEntry 要查找的调色板
		//! \return 返回找到的调色板的位置，找不到返回 -1
		int Find(PaletteEntry paletteEntry) const;
		//! \brief 在数组最后添加一个调色板
		//! \param paletteEntry 要添加的调色板
		//! \return 返回添加后,调色板在数组中的位置
		int Add(PaletteEntry paletteEntry);
		//! \brief 在数组中移出调色板
		//! \param nIndex 要移出的调色板在数组的位置
		//! \remarks 指定位置必须在数组范围内
		//! \param nCount 移出的个数,默认为1
		//! \return 返回实际移出面板的个数
		//! \remarks 如果 nCount>UGArray.size-nIndex 返回  nCount>UGArray.size-nIndex
		int RemoveAt(int nIndex, int nCount = 1);
		//! \brief 在数组指定位置插入一个调色板
		//! \remarks 如果指定位置不在数组范围内,会自动调整到[0,size]
		//! \param nIndex 要插入的位置
		//! \param paletteEntry 要插入的调色板
		void InsertAt(int nIndex, PaletteEntry paletteEntry);
		//! \brief 在数组的指定位置替换调色板
		//! \param nIndex 指定的位置
		//! \remarks 指定位置必须在数组size范围之内
		//! \param paletteEntry 要替换的调色板
		void SetAt(int nIndex, PaletteEntry paletteEntry);
		//! \brief 获取指定位置的调色板
		//! \param nIndex 指定的位置
		//! \remarks 指定位置必须在数组size范围之内
		//! \return 返回获得的调色板
		PaletteEntry GetAt(int nIndex)const;
		//! 移除所有元素,同时清空所有空间
		void RemoveAll();
		//! \brief 得到数组内部指针(地址连续)
		//! \return 返回数组内部指针(地址连续) 
		PaletteEntry* GetData();
		//! \brief 在数组最后增加一个数组
		//! \brief palette 要增加的数组
		//! \return 返回加入数组的起始位置
		int Append(const Palette& palette);
		//! \brief 数组拷贝,原来的内容被覆盖
		//! \param palette 传入的数组
		void Copy(const Palette& palette);
		//! \brief 调整m_Palette的内存空间为 nSize, 
		//! \remarks 如果原有空间不够, 内存空间会增加, 
		//! \param nSize 指定的新的调色板个数
		void SetSize(int nSize);
		//! \brief 得到有用的调色板的个数
		//! \return 返回的调色板的个数
		int GetSize()const;
		//! \brief 判断数组是否为空
		//! \return 如果空返回 true,否则返回 false.
		bool IsEmpty()const;

		//! \brief 将调色板颜色转成色集Colorset lugw
		//UGColorset ConvertToColorset();

	protected:
		//! \brief 将调色板用数组实现
		vector<PaletteEntry> m_Palette;

	};

	struct ImageData
	{
		//!  \brief Toolkit
		int nWidth;		//! \brief Image的宽
		  //!  \brief Toolkit
		int nHeight;		//! \brief Image的高
		  //!  \brief Toolkit
		int nWidthBytes;	//! \brief Image宽字节数，等于nWidth*每个象素字节数，比如32位字节数就是4
		  //!  \brief Toolkit
		unsigned char btPlanes;	//! \brief 调色板颜色数目
		  //!  \brief Toolkit
		unsigned char btBitsPixel; //! \brief 每个象素的bit数，比如32，24
		  //!  \brief Toolkit
		unsigned char btTextureCodecType;	//! \brief 纹理压缩格式
		  //!  \brief Toolkit
		bool bBkTransparent;
		//!  \brief Toolkit
		UGColor clrBack;
		//!  \brief Toolkit
		Palette palette;	//! \brief 调色板
		  //!  \brief Toolkit
		void* pBits;		//! \brief 内存数组，通常用来存储颜色值

		  //!  \brief Toolkit
		  //!  \brief Toolkit
		ImageData()
		{
			//!  \brief Toolkit
			nWidth = 0;
			//!  \brief Toolkit
			nHeight = 0;
			//!  \brief Toolkit
			nWidthBytes = 0;
			//!  \brief Toolkit
			btPlanes = 1;
			//!  \brief Toolkit
			btBitsPixel = 0;
			//!  \brief Toolkit
			btTextureCodecType = 0;
			//!  \brief Toolkit
			bBkTransparent = false;
			//!  \brief Toolkit
			clrBack = Palette::UGRGB(255, 255, 255);
			//!  \brief Toolkit
			pBits = NULL;
			//!  \brief Toolkit
		}

		void ConvertData()
		{
			if (pBits != NULL)
			{
				void* pByteDest = GetConvertedBits();
				delete[](unsigned char*)pBits;
				pBits = pByteDest;
			}
		}

		// 调用方负责释放内存
		void* GetConvertedBits()
		{
			unsigned char* pByteDest = NULL;
			if (pBits != NULL)
			{
				int nCount = nWidthBytes * nHeight;
				pByteDest = new unsigned char[nCount];
				unsigned char* pByteSrc = (unsigned char*)pBits;
				unsigned char* pByteTem = NULL;
				for (int i = 0; i < nHeight; i++)
				{
					pByteTem = pByteDest + (nHeight - 1 - i) * nWidthBytes;
					::memcpy(pByteTem, pByteSrc, nWidthBytes);
					pByteSrc += nWidthBytes;
				}
			}
			return pByteDest;
		}

		ImageData* Clone()
		{
			ImageData* pImageDataDes = NULL;
			try {
				pImageDataDes = new ImageData;
				pImageDataDes->pBits = new unsigned char[this->nHeight * this->nWidthBytes];
			}
			catch (std::bad_alloc) {
				delete pImageDataDes;
				return NULL;
			}
			pImageDataDes->btBitsPixel = this->btBitsPixel;
			pImageDataDes->btPlanes = this->btPlanes;
			pImageDataDes->nHeight = this->nHeight;
			pImageDataDes->nWidth = this->nWidth;
			pImageDataDes->nWidthBytes = this->nWidthBytes;
			pImageDataDes->bBkTransparent = this->bBkTransparent;
			pImageDataDes->clrBack = this->clrBack;
			pImageDataDes->palette.Copy(this->palette);
			::memcpy(pImageDataDes->pBits, this->pBits, pImageDataDes->nHeight * pImageDataDes->nWidthBytes);
			return pImageDataDes;
		}

		void DealWithTransparentColors()
		{
			if (this->pBits != NULL)
			{
				switch (this->btBitsPixel)
				{
				case 32:
				{
					int count = this->nHeight * this->nWidthBytes / 4;
					UGColor* pRGBAs = (UGColor*)this->pBits;
					UGColor ugColor;
					int transparent = 0;
					for (int i = 0; i < count; i++)
					{
						ugColor = pRGBAs[i];
						transparent = 255 - Palette::ALPHAVAL(ugColor);
						if (transparent >= 255)
						{
							pRGBAs[i] = Palette::RGBA(255, 255, 255, 0);
						}
						else if (transparent < 255 && transparent > 0)
						{
							pRGBAs[i] = Palette::RGBA(Palette::REDVAL(ugColor) * (255 - transparent) / 255 + transparent,
								Palette::GREENVAL(ugColor) * (255 - transparent) / 255 + transparent,
								Palette::BLUEVAL(ugColor) * (255 - transparent) / 255 + transparent,
								255);
						}
					}
				}
				break;
				}
			}
		}

		void ReverseData()
		{
			if (pBits != NULL)
			{
				unsigned char* pByteSrc = NULL;
				unsigned char* pByteDest = NULL;
				unsigned char* pByteTem = new unsigned char[nWidthBytes];
				int nCount = nHeight / 2;
				for (int i = 0; i < nCount; ++i)
				{
					pByteDest = (unsigned char*)pBits + (nHeight - 1 - i) * nWidthBytes;
					pByteSrc = (unsigned char*)pBits + i * nWidthBytes;
					::memcpy(pByteTem, pByteDest, nWidthBytes);
					::memcpy(pByteDest, pByteSrc, nWidthBytes);
					::memcpy(pByteSrc, pByteTem, nWidthBytes);
				}

				if (pByteTem != NULL)
				{
					delete[] pByteTem;
				}
			}
		}
	};

}
#endif
