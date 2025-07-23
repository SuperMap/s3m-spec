#ifdef WIN32
#include <corecrt_wstdio.h>
#else
#include <stdio.h>
#include <math.h>
#endif
extern "C"
{
#define JPEG_CJPEG_DJPEG	
#define JPEG_INTERNAL_OPTIONS	

#include "libjpeg/include/jpeglib.h"
#include "libjpeg/include/jerror.h"
}

#include "ImageOperator.h"
#include "S3MBUtils.h"
#include "libsquish/include/squish.h"
#include "libpng/include/png.h"
#include "libpng/include/pngconf.h"
#include <assert.h>

namespace S3MB
{
	unsigned int ImageOperator::Encode(const unsigned int nPixSize, unsigned int& nWidth, unsigned int& nHeight, unsigned char* pBufferIn, unsigned char** ppBufferOut, unsigned int eCodecType /*= enrS3TCDXTN*/, bool bGeneMipmaps /*= false*/)
	{
		// mipmap级别，默认1
		unsigned char* pMipmapBuffer = NULL;
		vector<unsigned int> arrByteOffset;
		unsigned char* pEncodeSoureBuffer = NULL;

		pEncodeSoureBuffer = pBufferIn;
		
		if (bGeneMipmaps)
		{
			ImageOperator::GetMipMapData(nWidth, nHeight, pEncodeSoureBuffer, &pMipmapBuffer, arrByteOffset);
		}
		else
		{
			pMipmapBuffer = pEncodeSoureBuffer;
			arrByteOffset.push_back(0);
		}

		unsigned int nMipmapCount = (unsigned int)arrByteOffset.size();
		// the return value
		unsigned int nCompressedSize = 0;

		// 先释放外部分配空间,由内部分配
		if (*ppBufferOut != NULL)
		{
			delete[](*ppBufferOut);
			(*ppBufferOut) = NULL;
		}

		// 分类型创建压缩纹理，with mipmap if required 
		switch (eCodecType)
		{
		case TC_DXT1_RGB:
		case TC_DXT5:
		{
			unsigned int nW = nWidth;
			unsigned int nH = nHeight;
			unsigned int i;
			unsigned int nDxtSize = ImageOperator::GetCompressedTextureSize(nPixSize, nWidth, nHeight, eCodecType, bGeneMipmaps);

			*ppBufferOut = new unsigned char[nDxtSize];
			unsigned char* pBufferPointer = *ppBufferOut;
			for (i = 0; i < nMipmapCount; i++)
			{
				unsigned int nOffsetSrc = arrByteOffset[i];
				if (eCodecType == TC_DXT1_RGB)
				{
					MemImage::Encode(3, nW, nH, pMipmapBuffer + nOffsetSrc, pBufferPointer);
				}
				else
				{
					MemImage::Encode(4, nW, nH, pMipmapBuffer + nOffsetSrc, pBufferPointer);
				}
				pBufferPointer += ImageOperator::GetCompressedTextureSize(nPixSize, nW, nH, eCodecType);

				nW /= 2;
				nH /= 2;

				if (nW == 0) nW = 1;
				if (nH == 0) nH = 1;
			}

			nCompressedSize = nDxtSize;
		}
		break;
		}
		// 释放临时缓存
		if (pMipmapBuffer != NULL && pMipmapBuffer != pEncodeSoureBuffer)
		{
			delete[]pMipmapBuffer;
			pMipmapBuffer = NULL;
		}

		if (pEncodeSoureBuffer != pBufferIn)
		{
			delete[] pEncodeSoureBuffer;
			pEncodeSoureBuffer = NULL;
		}

		return nCompressedSize;
	}

	unsigned int ImageOperator::Decode(const unsigned int nPixSize, unsigned int nWidth, unsigned int nHeight, unsigned char** ppBufferOut, const unsigned char* pBufferIn, unsigned int eCodecType, bool bGeneMipmaps /*= false*/)
	{
		// 若外部有数据，先清空
		if ((*ppBufferOut) != NULL)
		{
			delete[](*ppBufferOut);
			*ppBufferOut = NULL;
		}

		// 返回的解压大小
		unsigned int nSizeOut = 0;

		switch (eCodecType)
		{
		case TC_DXT1_RGB:
		case TC_DXT5:
		{
			nSizeOut = nWidth * nHeight * 4;
			(*ppBufferOut) = new unsigned char[nSizeOut];
			MemImage::Decode(nPixSize, nWidth, nHeight, *ppBufferOut, pBufferIn);

			if (bGeneMipmaps)
			{
				unsigned char* pGenMipMap = NULL;
				vector<unsigned int> arrOffset;
				nSizeOut = ImageOperator::GetMipMapData(nWidth, nHeight, *ppBufferOut, &pGenMipMap, arrOffset);
				delete[](*ppBufferOut);

				*ppBufferOut = new unsigned char[nSizeOut];
				::memcpy(*ppBufferOut, pGenMipMap, nSizeOut);

				delete[]pGenMipMap;
			}
		}
		break;
		}

		return nSizeOut;
	}

	void ImageOperator::Scale(const unsigned int components, unsigned int widthin, unsigned int heightin, unsigned char* pBufferIn, unsigned int widthout, unsigned int heightout, unsigned char* pBufferOut)
	{
		MemImage::Scale(components, widthin, heightin, pBufferIn, widthout, heightout, pBufferOut);
	}
	unsigned int ImageOperator::GetMipMapData(unsigned int nWidth, unsigned int nHeight, const unsigned char* pBufferIn, unsigned char** ppBufferOut, vector<unsigned int>& arrByteOffset)
	{
		unsigned int nW = nWidth, nH = nHeight;
		arrByteOffset.clear();

		// 计算mipmap偏移量
		unsigned int nMipMapBufferSize = 0;
		while (true)
		{
			arrByteOffset.push_back(nMipMapBufferSize);
			nMipMapBufferSize += nW * nH * 4;
			if (nW == 1 && nH == 1)
			{
				break;
			}

			nW /= 2;
			nH /= 2;

			if (0 == nW) nW = 1;
			if (0 == nH) nH = 1;
		};

		// 先清除外部数据
		if (*ppBufferOut)
		{
			delete[](*ppBufferOut);
			(*ppBufferOut) = NULL;
		}

		(*ppBufferOut) = new unsigned char[nMipMapBufferSize];
		::memcpy((*ppBufferOut), pBufferIn, nWidth * nHeight * 4);

		unsigned int i = 0;
		unsigned int nMipmapCount = (unsigned int)arrByteOffset.size();
		nW = nWidth;
		nH = nHeight;

		// 生成mipmap
		for (i = 0; i < nMipmapCount - 1; i++)
		{
			unsigned int nOffsetSrc = arrByteOffset[i];
			unsigned int nOffsetDst = arrByteOffset[i + 1];

			unsigned int nHalfWidth = (nW / 2 == 0) ? 1 : nW / 2;
			unsigned int nHalfHeight = (nH / 2 == 0) ? 1 : nH / 2;

			ImageOperator::Scale(4, nW, nH, (*ppBufferOut) + nOffsetSrc, nHalfWidth, nHalfHeight, (*ppBufferOut) + nOffsetDst);

			nW /= 2;
			nH /= 2;

			if (0 == nW) nW = 1;
			if (0 == nH) nH = 1;
		}

		return nMipMapBufferSize;
	}

	unsigned int ImageOperator::GetCompressedTextureSize(const unsigned int nPixSize, unsigned int nWidth, unsigned int nHeight, unsigned int eCodecType, bool bIsMipmaps /*= false*/)
	{
		// 包含mipmap大小	
		unsigned int nBufferSize = 0;

		do
		{
			switch (eCodecType)
			{
			case TC_NONE:
			{
				nBufferSize += nWidth * nHeight * nPixSize;
			}
			break;
			case TC_DXT1_RGB:
			{
				nBufferSize += DXT1_ENCODE_SIZE(nWidth, nHeight);
			}
			break;
			case TC_DXT5:
			{
				nBufferSize += DXT5_ENCODE_SIZE(nWidth, nHeight);				
			}
			break;
			}

			if (nWidth == 1 && nHeight == 1)
			{
				break;
			}

			nWidth /= 2;
			nHeight /= 2;

			if (0 == nWidth) nWidth = 1;
			if (0 == nHeight) nHeight = 1;

		} while (bIsMipmaps);

		return nBufferSize;
	}

	unsigned int ImageOperator::GetMipMapLevel(const unsigned int nWidth, const unsigned int nHeight)
	{
		unsigned int nMipMapLevel = 0;

		unsigned int nW = nWidth;
		unsigned int nH = nHeight;

		while (true)
		{
			nMipMapLevel++;

			if (nW == 1 && nH == 1)
			{
				break;
			}

			nW /= 2;
			nH /= 2;

			if (nW == 0) nW = 1;
			if (nH == 0) nH = 1;
		}

		return nMipMapLevel;
	}

	unsigned int ImgToolkit::AlignWidth(PixelFormat PixelFormat, unsigned int lPixelWidth)
	{
		switch (PixelFormat) {
		case PF_RGB8:
			return WIDTHBYTES(lPixelWidth * 24);
		case PF_RGBA8:
			return WIDTHBYTES(lPixelWidth * 32);
		}
		return 0;
	}

	unsigned int ImgToolkit::GetValue(const unsigned char* pByte, int nWidthBytes, PixelFormat pixelFormat, unsigned int x, unsigned int y)
	{
		assert(pByte != NULL);
		unsigned char* pBits = (unsigned char*)pByte;

		unsigned char* lp = NULL;
		unsigned int nValue = 0;
		unsigned int Remain = 0;
		switch (pixelFormat)
		{
		case PF_RGB8:
			lp = pBits + y * nWidthBytes + x * 3;
			memmove(&nValue, lp, 3);
			break;
		case PF_RGBA8:
			lp = pBits + y * nWidthBytes + x * 4;
			nValue = *((unsigned int*)lp);
			break;
		default:
			break;
		}
		return nValue;
	}

	void ImgToolkit::SetValue(unsigned char* pByte, int nWidthBytes, PixelFormat pixelFormat, unsigned int x, unsigned int y, unsigned int nValue)
	{
		assert(pByte != NULL);
		unsigned char* pBits = pByte;
		unsigned char* lp = NULL;
		unsigned int nBitPos = 0;
		switch (pixelFormat)
		{
		case PF_RGB8:
			lp = pBits + y * nWidthBytes + x * 3;
			memmove(lp, &nValue, 3);
			break;
		case PF_RGBA8:
			lp = pBits + y * nWidthBytes + x * 4;
			*((unsigned int*)lp) = nValue;
			break;
		default:
			break;
		}
	}

	TextureData* ImgToolkit::GetTextureData(wstring filePath, bool bReverse/* = true*/)
	{
		TextureData* pTextureData = new TextureData;
		unsigned int bufferSize = 0;
		unsigned char* pBit = nullptr;
		int nBitCnt;
		Palette palette;
		wstring ext = StringUtil::StrToUpper(StringUtil::GetExt(filePath));
		if (StringUtil::CompareNoCase(ext, U(".JPG")) || StringUtil::CompareNoCase(ext, U(".JPEG")))
		{
			JpgToBuffer(filePath, pTextureData, bufferSize, pBit, nBitCnt, bReverse);
		}
		else if (StringUtil::CompareNoCase(ext, U(".PNG")))
		{
			PngToBuffer(filePath, pTextureData, bufferSize, pBit, nBitCnt, palette, bReverse);
		}

		BufferToTextureData(pTextureData, bufferSize, pBit, nBitCnt, palette);
		if (pBit != nullptr)
		{
			delete pBit;
			pBit = nullptr;
		}
		return pTextureData;
	}

	bool ImgToolkit::JpgToBuffer(wstring filePath, TextureData*& pTextureData, unsigned int& bufferSize, unsigned char*& pBits, int& nBitCnt, bool bReverse /*= true*/)
	{
#ifdef WIN32
        FILE* m_File = (FILE*)_wfopen(filePath.c_str(), U("rb"));
#else
        FILE* m_File = (FILE*)fopen((StringUtil::UnicodeToANSI(filePath)).c_str(), "rb");
#endif
		if (m_File == NULL)
		{
			return false;
		}
		else
		{
			// jpeg解压
			struct jpeg_decompress_struct m_dinfo;
			// 错误处理
			struct jpeg_error_mgr m_jerr;

			m_dinfo.err = jpeg_std_error(&m_jerr);
			size_t nsize = sizeof(struct jpeg_decompress_struct);
			jpeg_CreateDecompress(&m_dinfo, JPEG_LIB_VERSION, nsize);

			jpeg_stdio_src(&m_dinfo, m_File);
			if (jpeg_read_header(&m_dinfo, true) != JPEG_HEADER_OK)
			{
				jpeg_destroy_decompress(&m_dinfo);
				return false;
			}

			short nBitCount = short(m_dinfo.num_components * 8);	// 比特数

			pTextureData->m_nWidth = m_dinfo.image_width;
			pTextureData->m_nHeight = m_dinfo.image_height;

			// BeginRead
			{
				jpeg_create_decompress(&m_dinfo);
				fseek(m_File, 0, SEEK_SET);
				jpeg_stdio_src(&m_dinfo, m_File);
				if (jpeg_read_header(&m_dinfo, true) != JPEG_HEADER_OK)
				{
					jpeg_finish_decompress(&m_dinfo);
					jpeg_destroy_decompress(&m_dinfo);
					return 0;
				}
				if (!jpeg_start_decompress(&m_dinfo))
				{
					jpeg_finish_decompress(&m_dinfo);
					jpeg_destroy_decompress(&m_dinfo);
					return 0;
				}
			}

			// 读取像素行数据
			unsigned char* pByteLine;
			// 每行像素所占字节数
			int nWidthBytes;
			// CMYK会被转化为RGB
			if (m_dinfo.output_components == 4 &&
				m_dinfo.out_color_space == JCS_CMYK)
			{
				nWidthBytes = m_dinfo.image_width * 3;
				pByteLine = new unsigned char[nWidthBytes];
				pBits = new unsigned char[nWidthBytes * m_dinfo.image_height];
				::memset(pBits, 0, nWidthBytes * m_dinfo.image_height);
			}
			else
			{
				nWidthBytes = m_dinfo.image_width * m_dinfo.num_components;
				pByteLine = new unsigned char[nWidthBytes];
				pBits = new unsigned char[nWidthBytes * m_dinfo.image_height];
				::memset(pBits, 0, nWidthBytes * m_dinfo.image_height);
			}
			bufferSize = nWidthBytes * m_dinfo.image_height;
			for (int iLine = 0; iLine < pTextureData->m_nHeight; iLine++)
			{
				// CMYK转换为RGB显示，不影响显示效果
				if (m_dinfo.output_components == 4 &&
					m_dinfo.out_color_space == JCS_CMYK)
				{
					unsigned char* pBuffer = new unsigned char[m_dinfo.image_width * m_dinfo.num_components];
					jpeg_read_scanlines(&m_dinfo, &pBuffer, 1);
					JSAMPROW src = pBuffer;
					JSAMPROW dst = pByteLine;
					for (int i = 0; i < m_dinfo.output_width; i++)
					{
						int C = (int)src[0];
						int M = (int)src[1];
						int Y = (int)src[2];
						int K = (int)src[3];

						dst[0] = (unsigned char)((K * C) / 255);	// C -> R
						dst[1] = (unsigned char)((K * M) / 255);	// M -> G
						dst[2] = (unsigned char)((K * Y) / 255);	// Y -> B

						src += 4;
						dst += 3;
					}
					if (NULL != pBuffer)
					{
						delete[] pBuffer;
						pBuffer = NULL;
					}
				}
				else
				{
					(void)jpeg_read_scanlines(&m_dinfo, &pByteLine, 1);
				}
				int nowLine = bReverse ? pTextureData->m_nHeight - iLine - 1 : iLine;
				::memcpy(pBits + nWidthBytes * nowLine, pByteLine, nWidthBytes);
				::memset(pByteLine, 0, nWidthBytes);
			}
			// EndRead
			jpeg_destroy_decompress(&m_dinfo);

#pragma region 通过位深得到jpg像素格式
			switch (nBitCount)
			{
			case 1:
				nBitCnt = 1;
				break;
			case 4:
				nBitCnt = 4;
				break;
			case 8:
				nBitCnt = 8;
				break;
			case 16:
				nBitCnt = 16;
				break;
			case 24:
				nBitCnt = 24;
				break;
			case 32:
			{
				if (m_dinfo.num_components == 4 &&
					m_dinfo.out_color_space == JCS_CMYK)
				{
					nBitCnt = 24;
					break;
				}
				else
				{
					nBitCnt = 32;
					break;
				}
			}
			case 48:
				nBitCnt = 48;
				break;
			case 64:
				nBitCnt = 64;
				break;
			default:
				nBitCnt = 24;
				break;
			}
#pragma endregion 
			if (NULL != m_File)
			{
				fclose(m_File);
				m_File = NULL;
			}
			return true;
		}
	}

	// ----------------------------回调函数--------------------------------
	static void png_user_error(png_structp png_ptr, const char* error_message)
	{
		printf("libpng: %s", error_message);

		jmp_buf* psSetJmpContext = (jmp_buf*)png_get_error_ptr(png_ptr);
		if (psSetJmpContext)
		{
			longjmp(*psSetJmpContext, 1);
		}
	}

	static void png_user_warning(png_structp png_ptr, const char* error_message)
	{
		printf("libpng: %s", error_message);
	}

	static void png_read_data(png_structp png_ptr, png_bytep data, png_size_t length)
	{
		png_size_t check;

		check = (png_size_t)fread(data, (png_size_t)1, length, (FILE*)png_get_io_ptr(png_ptr));

		if (check != length)
		{
			png_error(png_ptr, "Read Error");
		}
	}

	bool ImgToolkit::PngToBuffer(wstring filePath, TextureData*& pTextureData, unsigned int& BufferSize, unsigned char*& pBits, int& nBitCnt, Palette& palette, bool bReverse /*= true*/)
	{
#ifdef WIN32
        FILE* m_File = (FILE*)_wfopen(filePath.c_str(), U("rb"));
#else
        FILE* m_File = (FILE*)fopen((StringUtil::UnicodeToANSI(filePath)).c_str(), "rb");
#endif
		if (m_File == NULL)
		{
			return false;
		}
		else
		{
			// png文件及相关属性指针
			png_structp m_pPngPtr;
			m_pPngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
			if (NULL == m_pPngPtr)
			{
				fclose(m_File);
				m_File = NULL;

				return false;
			}

			// png图片操作指针
			png_infop   m_pPngInfo;
			m_pPngInfo = png_create_info_struct(m_pPngPtr);
			if (NULL == m_pPngInfo)
			{
				png_destroy_read_struct(&m_pPngPtr, NULL, NULL);
				fclose(m_File);
				m_File = NULL;

				return false;
			}

			jmp_buf m_jmpContext;
			png_set_error_fn(m_pPngPtr, &m_jmpContext, png_user_error, png_user_warning);
			if (setjmp(m_jmpContext) != 0)
			{
				png_destroy_read_struct(&m_pPngPtr, &m_pPngInfo, NULL);
				fclose(m_File);
				m_File = NULL;

				return false;
			}

			// 是否有交错存储
			bool m_bIsInterlace;
			try
			{
				png_set_read_fn(m_pPngPtr, m_File, png_read_data);
				png_read_info(m_pPngPtr, m_pPngInfo);

                int m_nColorType = 0, nPixelDepth = 0;
                int interlace_type = PNG_INTERLACE_NONE;
                m_bIsInterlace = interlace_type == PNG_INTERLACE_NONE;
                nPixelDepth = png_get_bit_depth(m_pPngPtr, m_pPngInfo) * png_get_channels(m_pPngPtr,m_pPngInfo);
                pTextureData->m_nWidth = png_get_image_width(m_pPngPtr, m_pPngInfo);
                pTextureData->m_nHeight = png_get_image_height(m_pPngPtr, m_pPngInfo);
                m_nColorType = png_get_color_type(m_pPngPtr, m_pPngInfo);
                m_bIsInterlace = png_get_interlace_type(m_pPngPtr, m_pPngInfo);

				switch (nPixelDepth)
				{
				case 1:
					nBitCnt = 1;
					break;
				case 2:
					if (m_nColorType == PNG_COLOR_TYPE_PALETTE)
					{
						png_set_packing(m_pPngPtr);
					}
					else
					{
						png_set_expand_gray_1_2_4_to_8(m_pPngPtr);
					}

					png_read_update_info(m_pPngPtr, m_pPngInfo);
					nBitCnt = 8;
					break;
				case 4:
					nBitCnt = 4;
					break;
				case 8:
					nBitCnt = 8;
					break;
				case 16:
				{
					if (m_nColorType == PNG_COLOR_TYPE_GRAY)
					{
						nBitCnt = 160;
					}
					else
					{
						nBitCnt = 16;
					}
				}
				break;
				case 24:
					nBitCnt = 24;
					break;
				case 32:
					nBitCnt = 32;			
				    break;
				case 48:
				{
					nBitCnt = 48;
				}
				break;
				case 64:
				{
					nBitCnt = 64;
				}
				break;
				default:
				{
#ifdef WIN32
                    assert(U("图片格式不支持！"));
#else
                    std::cout << "图片格式不支持！" << std::endl;
#endif
					png_destroy_read_struct(&m_pPngPtr, &m_pPngInfo, NULL);
					fclose(m_File);
					m_File = NULL;

					return false;
				}
				break;
				}

				if (m_nColorType == PNG_COLOR_TYPE_PALETTE)
				{
					png_color* pasPNGPalette;
					int nColorCount;
					unsigned char* trans = NULL;
					png_color_16* trans_values = NULL;
					int	num_trans = 0;
					int	nNoDataIndex = -1;

					if (png_get_PLTE(m_pPngPtr, m_pPngInfo,
						&pasPNGPalette, &nColorCount) == 0)
					{
						nColorCount = 0;
					}

					png_get_tRNS(m_pPngPtr, m_pPngInfo,
						&trans, &num_trans, &trans_values);

					palette.SetSize(nColorCount);
					for (int nIndex = 0; nIndex < num_trans; nIndex++)
					{
						if (trans[nIndex] == 0)
						{
							nNoDataIndex = nIndex;
							break;
						}
					}

					PaletteEntry paletteEntry;
					if (num_trans > 0)
					{
						unsigned char alpha;
						for (int nColor = nColorCount - 1; nColor >= 0; nColor--)
						{
							unsigned int Color;
							memset(&alpha, 255, sizeof(unsigned char));
							if (nColor == nNoDataIndex)
							{
								memset(&alpha, 0, sizeof(unsigned char));
							}

							Color = Palette::RGBA(pasPNGPalette[nColor].red,
								pasPNGPalette[nColor].green, pasPNGPalette[nColor].blue, alpha);

							paletteEntry.peRed = Palette::REDVAL(Color);
							paletteEntry.peGreen = Palette::GREENVAL(Color);
							paletteEntry.peBlue = Palette::BLUEVAL(Color);
							paletteEntry.peFlags = Palette::ALPHAVAL(Color);
							palette.SetAt(nColor, paletteEntry);
						}
					}
					else
					{
						for (int nColor = nColorCount - 1; nColor >= 0; nColor--)
						{
							unsigned int Color;

							Color = Palette::RGBA(pasPNGPalette[nColor].red,
								pasPNGPalette[nColor].green, pasPNGPalette[nColor].blue, 255);
							paletteEntry.peRed = Palette::REDVAL(Color);
							paletteEntry.peGreen = Palette::GREENVAL(Color);
							paletteEntry.peBlue = Palette::BLUEVAL(Color);
							paletteEntry.peFlags = Palette::ALPHAVAL(Color);
							palette.SetAt(nColor, paletteEntry);
						}
					}
					if (nNoDataIndex > -1)
					{
						int noValue = nNoDataIndex;// 无效值，暂时没使用
					}
				}

			}
			catch (...)
			{
				png_destroy_read_struct(&m_pPngPtr, &m_pPngInfo, NULL);
				fclose(m_File);
				m_File = NULL;

				return false;
			}

			// 每行像素所占字节数
            int nWidthBytes = png_get_rowbytes(m_pPngPtr, m_pPngInfo);
			// 读取像素行数据
			unsigned char* pByteLine = new unsigned char[nWidthBytes];
			// 当前行索引，用于交错存储判断是否取数据
			int m_nCurrentRowIndex = -1;
			// 最新一行数据，用于取数据是判断是否取的是当前数据
			int m_nLastLineRead = -1;
			unsigned char* m_pBufferTmp = NULL;
			pBits = new unsigned char[nWidthBytes * pTextureData->m_nHeight];
			BufferSize = nWidthBytes * pTextureData->m_nHeight;
			for (int iLine = 0; iLine < pTextureData->m_nHeight; iLine++)
			{
				if (setjmp(m_jmpContext) != 0)
				{
					return false;
				}
				if (m_bIsInterlace)
				{
					if (m_nCurrentRowIndex == -1)
					{
						if (NULL != m_pBufferTmp)
						{
							delete[] m_pBufferTmp;
							m_pBufferTmp = NULL;
						}

						m_pBufferTmp = new unsigned char[nWidthBytes * pTextureData->m_nHeight];
						::memset(m_pBufferTmp, 0, sizeof(unsigned char) * nWidthBytes * pTextureData->m_nHeight);

						png_byte** pRowPointers = NULL;
						pRowPointers = (png_bytepp)malloc(pTextureData->m_nHeight * sizeof(png_bytep));

						for (int i = 0; i < pTextureData->m_nHeight; i++)
						{
                            pRowPointers[i] = m_pBufferTmp + i * png_get_rowbytes(m_pPngPtr, m_pPngInfo);// m_pPngPtr->rowbytes;
						}

						png_read_image(m_pPngPtr, pRowPointers);

						::free(pRowPointers);
						pRowPointers = NULL;
					}

					m_nCurrentRowIndex = iLine;
					::memcpy(pByteLine, m_pBufferTmp + iLine * nWidthBytes, nWidthBytes);
				}
				else
				{
					if (NULL != m_pBufferTmp)
					{
						delete[] m_pBufferTmp;
						m_pBufferTmp = NULL;
					}
					m_pBufferTmp = new unsigned char[nWidthBytes];
					::memset(m_pBufferTmp, 0, nWidthBytes);

					png_bytep row;
					row = m_pBufferTmp;
					if (iLine <= m_nLastLineRead)
					{
						// Restart();
						// if (setjmp(m_jmpContext) != 0)
						// {
						return false;
						// }
					}
					//else if (iLine % pTextureData->m_nWidth == 0 && iLine != 0)
					//{
						// Restart();
						// if (setjmp(m_jmpContext) != 0)
						// {
					//	return false;
						// }
					//}
					while (iLine > m_nLastLineRead)
					{
						png_read_rows(m_pPngPtr, &row, NULL, 1);
						m_nLastLineRead++;
					}
					{
						::memcpy(pByteLine, m_pBufferTmp, nWidthBytes);
					}
				}
				int nowLine = bReverse ? pTextureData->m_nHeight - iLine - 1 : iLine;
				::memcpy(pBits + nWidthBytes * nowLine, pByteLine, nWidthBytes);
				::memset(pByteLine, 0, nWidthBytes);
			}
			if (m_pBufferTmp != nullptr)
			{
				delete m_pBufferTmp;
			}
			delete pByteLine;
			// close文件
			png_destroy_read_struct(&m_pPngPtr, &m_pPngInfo, NULL);
			if (NULL != m_File)
			{
				fclose(m_File);
				m_File = NULL;
			}
		}
		return true;
	}

	bool ImgToolkit::BufferToTextureData(TextureData*& pTextureData, unsigned int BitsSize, unsigned char*& pBits, int nBitCnt, Palette palette)
	{
		if (pTextureData == NULL || pBits == NULL || BitsSize == 0)
		{
			return false;
		}
		if (pTextureData->m_pBuffer != NULL)
		{
			delete pTextureData->m_pBuffer;
			pTextureData->m_pBuffer = NULL;
		}
		int bufferSize = 0;
		unsigned char* pDes = nullptr;
		switch (nBitCnt)
		{
		case 32:
		case 24:
			bufferSize = pTextureData->m_nWidth * pTextureData->m_nHeight * nBitCnt / 8;
			pTextureData->m_pBuffer = new unsigned char[bufferSize];

			if (bufferSize != BitsSize)  assert(false);

			::memcpy(pTextureData->m_pBuffer, pBits, bufferSize);
			pTextureData->m_enFormat = nBitCnt / 8 == 3 ? PF_RGB8 : PF_RGBA8;
			break;
		case 16:
			pTextureData->m_enFormat = PF_RGBA8;
			bufferSize = pTextureData->m_nWidth * pTextureData->m_nHeight * 4;
			pTextureData->m_pBuffer = new unsigned char[bufferSize];
			pDes = pTextureData->m_pBuffer;
			for (int i = 0; i < BitsSize; i += 2)
			{
				int nValue = pBits[i];
				int nValueA = pBits[i + 1];
				pDes[4 * i / 2] = nValue;
				pDes[4 * i / 2 + 1] = nValue;
				pDes[4 * i / 2 + 2] = nValue;
				pDes[4 * i / 2 + 3] = nValueA;
			}
			break;
		case 8:
			pTextureData->m_enFormat = PF_RGBA8;
			bufferSize = pTextureData->m_nWidth * pTextureData->m_nHeight * 4;
			pTextureData->m_pBuffer = new unsigned char[bufferSize];
			pDes = pTextureData->m_pBuffer;
			if (palette.GetSize() > 0)
			{
				for (int i = 0; i < BitsSize; i++)
				{
					int nValue = pBits[i];
					pDes[4 * i ] = palette.GetAt(nValue).peRed;
					pDes[4 * i + 1] = palette.GetAt(nValue).peGreen;
					pDes[4 * i + 2] = palette.GetAt(nValue).peBlue;
					pDes[4 * i + 3] = palette.GetAt(nValue).peFlags;
				}
			}
			else
			{
				for (int i = 0; i < BitsSize; i++)
				{
					int nValue = pBits[i];
					pDes[4 * i] = nValue;
					pDes[4 * i + 1] = nValue;
					pDes[4 * i + 2] = nValue;
					pDes[4 * i + 3] = 255;
				}
			}
			break;
		default:
			return false;
		}
		pTextureData->m_nSize = bufferSize;
		return true;
	}

	static void halveImage(int components, int width, int height,
		const unsigned char *datain, unsigned char *dataout)
	{
		int i, j, k;
		int newwidth, newheight;
		int delta;
		unsigned char *s;
		const unsigned char *t;

		newwidth = width / 2;
		newheight = height / 2;
		delta = width * components;
		s = dataout;
		t = datain;

		for (i = 0; i < newheight; i++) {
			for (j = 0; j < newwidth; j++) {
				for (k = 0; k < components; k++) {
					s[0] = (t[0] + t[components] + t[delta] +
						t[delta + components] + 2) / 4;
					s++; t++;
				}
				t += components;
			}
			t += delta;
		}
	}

	unsigned int MemImage::Encode(const unsigned int pixsize, unsigned int width, unsigned int height, const unsigned char* in, unsigned char *out)
	{
		if (in == NULL || out == NULL || width == 0 || height == 0 || pixsize < 3 || pixsize > 4)
		{
			return 0;
		}

		unsigned int size = 0;

		if (pixsize == 3)
		{
			squish::CompressImage(in, width, height, out, squish::kDxt1 | squish::kColourRangeFit);
			size = ((width - 1) / 4 + 1)*((height - 1) / 4 + 1) * 8;
		}
		else
		{
			squish::CompressImage(in, width, height, out, squish::kDxt5 | squish::kColourRangeFit);
			size = ((width - 1) / 4 + 1)*((height - 1) / 4 + 1) * 16;
		}

		return size;
	}

	void MemImage::Decode(const unsigned int pixsize, unsigned int width, unsigned int height, unsigned char* out, const unsigned char *in)
	{
		if (in == NULL || out == NULL || width == 0 || height == 0 || pixsize < 3 || pixsize > 4)
		{
			return;
		}

		if (pixsize == 3)
		{
			squish::DecompressImage(out, width, height, in, squish::kDxt1 | squish::kColourRangeFit);
		}
		else
		{
			squish::DecompressImage(out, width, height, in, squish::kDxt5 | squish::kColourRangeFit);
		}
	}

	void MemImage::Scale(const unsigned int components, unsigned int widthin, unsigned int heightin, unsigned char *in, unsigned int widthout, unsigned int heightout, unsigned char *out)
	{
		float x, lowx, highx, convx, halfconvx;
		float y, lowy, highy, convy, halfconvy;
		float xpercent, ypercent;
		float percent;
		/* Max components in a format is 4, so... */
		float totals[4];
		float area;
		int i, j, k, yint, xint, xindex, yindex;
		int temp;

		const unsigned char* datain = (const unsigned char *)in;
		unsigned char* dataout = (unsigned char*)out;

		if (widthin == widthout && heightin == heightout)
		{
			memcpy(dataout, datain, heightin*widthin*components);
			return;
		}

		if (widthin == widthout * 2 && heightin == heightout * 2) {
			halveImage(components, widthin, heightin, datain, dataout);
			return;
		}
		convy = (float)heightin / heightout;
		convx = (float)widthin / widthout;
		halfconvx = convx / 2;
		halfconvy = convy / 2;
		for (i = 0; i < heightout; i++) {
			y = (float)(convy * (i + 0.5));
			if (heightin > heightout) {
				highy = y + halfconvy;
				lowy = y - halfconvy;
			}
			else {
				highy = y + 0.5;
				lowy = y - 0.5;
			}
			for (j = 0; j < widthout; j++) {
				x = convx * (j + 0.5);
				if (widthin > widthout) {
					highx = x + halfconvx;
					lowx = x - halfconvx;
				}
				else {
					highx = x + 0.5;
					lowx = x - 0.5;
				}

				/*
				** Ok, now apply box filter to box that goes from (lowx, lowy)
				** to (highx, highy) on input data into this pixel on output
				** data.
				*/
				totals[0] = totals[1] = totals[2] = totals[3] = 0.0;
				area = 0.0;

				y = lowy;
				yint = floor(y);
				while (y < highy) {
					yindex = (yint + heightin) % heightin;
					if (highy < yint + 1) {
						ypercent = highy - y;
					}
					else {
						ypercent = yint + 1 - y;
					}

					x = lowx;
					xint = floor(x);

					while (x < highx) {
						xindex = (xint + widthin) % widthin;
						if (highx < xint + 1) {
							xpercent = highx - x;
						}
						else {
							xpercent = xint + 1 - x;
						}

						percent = xpercent * ypercent;
						area += percent;
						temp = (xindex + (yindex * widthin)) * components;
						for (k = 0; k < components; k++) {
							totals[k] += datain[temp + k] * percent;
						}

						xint++;
						x = xint;
					}
					yint++;
					y = yint;
				}

				temp = (j + (i * widthout)) * components;
				for (k = 0; k < components; k++) {
					/* totals[] should be rounded in the case of enlarging an RGB
					 * ramp when the type is 332 or 4444
					 */
					dataout[temp + k] = (totals[k] + 0.5) / area;
				}
			}
		}
	}

}
