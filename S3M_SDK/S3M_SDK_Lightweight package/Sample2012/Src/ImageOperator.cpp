#include "ImageOperator.h"
#include <assert.h>
#include "Palette.h"

unsigned int ImageOperator::Encode(const unsigned int nPixSize, unsigned int &nWidth, unsigned int &nHeight, unsigned char* pBufferIn, unsigned char **ppBufferOut, unsigned int eCodecType /*= enrS3TCDXTN*/, bool bGeneMipmaps /*= false*/)
{
	//mipmap级别，默认1
	unsigned char *pMipmapBuffer = NULL;
	vector<unsigned int> arrByteOffset;
	unsigned char *pEncodeSoureBuffer = NULL;

	if (eCodecType == enrPVRTPF_PVRTC || eCodecType == enrPVRTPF_PVRTC_FAST)
	{
		pEncodeSoureBuffer = MakeSquareForPVR(nWidth, nHeight, pBufferIn);
	}
	else
	{
		pEncodeSoureBuffer = pBufferIn;
	}

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
	//the return value
	unsigned int nCompressedSize = 0;

	//先释放外部分配空间,由内部分配 wangxinhe 20121115
	if (*ppBufferOut != NULL)
	{
		delete[](*ppBufferOut);
		(*ppBufferOut) = NULL;
	}

	//分类型创建压缩纹理，with mipmap if required . wxh
	switch (eCodecType)
	{
	case enrS3TCDXTN:
	{
		unsigned int nW = nWidth;
		unsigned int nH = nHeight;
		unsigned int i;
		unsigned int nDxtSize = ImageOperator::GetCompressedTextureSize(nPixSize, nWidth, nHeight, enrS3TCDXTN, bGeneMipmaps);

		*ppBufferOut = new unsigned char[nDxtSize];
		unsigned char *pBufferPointer = *ppBufferOut;
		for (i = 0; i < nMipmapCount; i++)
		{
			unsigned int nOffsetSrc = arrByteOffset[i];
			MemImage::Encode(nPixSize, nW, nH, pMipmapBuffer + nOffsetSrc, pBufferPointer);
			pBufferPointer += ImageOperator::GetCompressedTextureSize(nPixSize, nW, nH, enrS3TCDXTN);

			nW /= 2;
			nH /= 2;

			if (nW == 0) nW = 1;
			if (nH == 0) nH = 1;
		}

		nCompressedSize = nDxtSize;
	}
	break;
	}
	//delete the temp buffer
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

unsigned int ImageOperator::Decode(const unsigned int nPixSize, unsigned int nWidth, unsigned int nHeight, unsigned char** ppBufferOut, const unsigned char *pBufferIn, unsigned int eCodecType, bool bGeneMipmaps /*= false*/)
{
	//若外部有数据，先清空
	if ((*ppBufferOut) != NULL)
	{
		delete[](*ppBufferOut);
		*ppBufferOut = NULL;
	}

	//返回的解压大小
	unsigned int nSizeOut = 0;

	switch (eCodecType)
	{
	case enrS3TCDXTN:
	{
		nSizeOut = nWidth * nHeight * 4;
		(*ppBufferOut) = new unsigned char[nSizeOut];
		MemImage::Decode(nPixSize, nWidth, nHeight, *ppBufferOut, pBufferIn);

		if (bGeneMipmaps)
		{
			unsigned char *pGenMipMap = NULL;
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

void ImageOperator::Scale(const unsigned int components, unsigned int widthin, unsigned int heightin, unsigned char *pBufferIn, unsigned int widthout, unsigned int heightout, unsigned char *pBufferOut)
{
	MemImage::Scale(components, widthin, heightin, pBufferIn, widthout, heightout, pBufferOut);
}

unsigned char* ImageOperator::MakeSquareForPVR(unsigned int& nWidth, unsigned int& nHeight, unsigned char* pBufferIn)
{
	unsigned char* pSquareBuffer = pBufferIn;
	//要求: 长必须等于宽、边长必须是2的N次幂且不能小于8
	unsigned int nLength = ImageOperator::NextP2((nWidth + nHeight) / 2);
	if (nLength < 8)
	{
		nLength = 8;
	}
	//送入处理的缓冲区（长宽相等）
	if (nWidth != nLength || nHeight != nLength)
	{
		pSquareBuffer = new  unsigned char[nLength*nLength * 4];
		ImageOperator::Scale(4, nWidth, nHeight, pBufferIn, nLength, nLength, pSquareBuffer);
		//缩放图片至合适大小
		nWidth = nLength;
		nHeight = nLength;
	}
	return pSquareBuffer;
}

unsigned int ImageOperator::GetMipMapData(unsigned int nWidth, unsigned int nHeight, const unsigned char* pBufferIn, unsigned char** ppBufferOut, vector<unsigned int>& arrByteOffset)
{
	unsigned int nW = nWidth, nH = nHeight;
	arrByteOffset.clear();

	//计算mipmap偏移量
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

	//先清除外部数据
	if (*ppBufferOut)
	{
		delete[](*ppBufferOut);
		(*ppBufferOut) = NULL;
	}

	(*ppBufferOut) = new unsigned char[nMipMapBufferSize];
	::memcpy((*ppBufferOut), pBufferIn, nWidth*nHeight * 4);

	unsigned int i = 0;
	unsigned int nMipmapCount = (unsigned int)arrByteOffset.size();
	nW = nWidth;
	nH = nHeight;

	//生成mipmap
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

unsigned int ImageOperator::NextP2(unsigned int a)
{
	unsigned int rval = 1;
	while (rval < a) rval <<= 1;
	return rval;
}

unsigned int ImageOperator::GetCompressedTextureSize(const unsigned int nPixSize, unsigned int nWidth, unsigned int nHeight, unsigned int eCodecType, bool bIsMipmaps /*= false*/)
{
	//包含mipmap大小	
	unsigned int nBufferSize = 0;

	do
	{
		switch (eCodecType)
		{
		case enrNONE:
		{
			nBufferSize += nWidth * nHeight * nPixSize;
		}
		break;
		case enrS3TCDXTN:
		{
			if (3 == nPixSize)
			{
				nBufferSize += DXT1_ENCODE_SIZE(nWidth, nHeight);
			}
			else if (4 == nPixSize)
			{
				nBufferSize += DXT5_ENCODE_SIZE(nWidth, nHeight);
			}
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
	uInt nMipMapLevel = 0;

	uInt nW = nWidth;
	uInt nH = nHeight;

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
	case IPF_MONO:
		return WIDTHBYTES(lPixelWidth);
	case IPF_FBIT:
		return WIDTHBYTES(lPixelWidth * IPF_FBIT_BITS);
	case IPF_UBYTE:
	case IPF_BYTE:
		return WIDTHBYTES(lPixelWidth * IPF_BYTE_BITS);
	case IPF_TBYTE:
	case IPF_UTBYTE:
		return WIDTHBYTES(lPixelWidth * IPF_TBYTE_BITS);
	case IPF_RGB:
		return WIDTHBYTES(lPixelWidth * IPF_RGB_BITS);
	case IPF_RGBA:
		return WIDTHBYTES(lPixelWidth * IPF_RGBA_BITS);
	case IPF_TRGB:
		return WIDTHBYTES(lPixelWidth * 48);
	case IPF_LONGLONG:// = 64,	//64位,长整型
		return WIDTHBYTES(lPixelWidth * 64);
	case IPF_LONG:// = 320:
	case IPF_ULONG:
		return WIDTHBYTES(lPixelWidth * 32);
	case IPF_FLOAT:// = 3200,	//32位,浮点型
		return WIDTHBYTES(lPixelWidth * 32);
	case IPF_DOUBLE:// = 6400,	//64位,双精度浮点型
		return WIDTHBYTES(lPixelWidth * 64);
	default:
		return WIDTHBYTES(lPixelWidth);
	}
}

unsigned int ImgToolkit::GetValue(const unsigned char* pByte, int nWidthBytes, PixelFormat pixelFormat, unsigned int x, unsigned int y)
{
	assert(pByte != NULL);
	unsigned char *pBits = (unsigned char*)pByte;

	unsigned char *lp = NULL;
	unsigned int nValue = 0;
	unsigned int Remain = 0;
	switch (pixelFormat)
	{
	case IPF_MONO:
		Remain = x % 8;
		lp = pBits + y * nWidthBytes + x / 8;
		nValue = ((*lp >> (7 - Remain))&(0x01));
		break;
	case IPF_FBIT:
		lp = pBits + y * nWidthBytes + x / 2;
		if (x % 2 == 0)
			nValue = (((*lp)&(0xF0)) >> 4);
		else
			nValue = ((*lp)&(0x0F));
		break;
	case IPF_UBYTE:
		lp = pBits + y * nWidthBytes + x;
		nValue = *((unsigned char*)lp);
		break;
	case IPF_BYTE:
		lp = pBits + y * nWidthBytes + x;
		nValue = *((char*)lp);
		break;
	case IPF_TBYTE:
		lp = pBits + y * nWidthBytes + x * 2;
		nValue = *((short*)lp);
		break;
	case IPF_UTBYTE:
		lp = pBits + y * nWidthBytes + x * 2;
		nValue = *((unsigned short*)lp);
		break;
	case IPF_RGB:
		lp = pBits + y * nWidthBytes + x * 3;
		memmove(&nValue, lp, 3);
		break;
	case IPF_RGBA:
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
	unsigned char *pBits = pByte;
	unsigned char* lp = NULL;
	unsigned int nBitPos = 0;
	switch (pixelFormat)
	{
	case IPF_MONO:
		lp = pBits + y * nWidthBytes + x / 8;
		nBitPos = x % 8;
		if (nValue != 0)
			*((unsigned char*)lp) = unsigned char((*lp) | (1 << (7 - nBitPos)));
		else
			*((unsigned char*)lp) = unsigned char((*lp)&(~(1 << (7 - nBitPos))));
		break;
	case IPF_FBIT:
		lp = pBits + y * nWidthBytes + x / 2;
		if (x % 2 == 0)
			*((unsigned char*)lp) = unsigned char(((*lp)&(0x0F)) | ((unsigned char)nValue) << 4);
		else
			*((unsigned char*)lp) = unsigned char(((*lp)&(0xF0)) | ((unsigned char)nValue));
		break;
	case IPF_UBYTE:  // 8位的灰度影像图也要处理
		lp = pBits + y * nWidthBytes + x * 1;
		*((unsigned char*)lp) = (unsigned char)nValue;
		break;
	case IPF_BYTE:  // 8位的灰度影像图也要处理
		lp = pBits + y * nWidthBytes + x * 1;
		*((char*)lp) = (char)nValue;
		break;
	case IPF_TBYTE:
		lp = pBits + y * nWidthBytes + x * 2;
		*((short*)lp) = (short)nValue;
		break;
	case IPF_UTBYTE:
		lp = pBits + y * nWidthBytes + x * 2;
		*((unsigned short*)lp) = (unsigned short)nValue;
		break;
	case IPF_RGB:
		lp = pBits + y * nWidthBytes + x * 3;
		memmove(lp, &nValue, 3);
		break;
	case IPF_RGBA:
		lp = pBits + y * nWidthBytes + x * 4;
		*((unsigned int*)lp) = nValue;
		break;
	default:
		break;
	}
}

TextureData* ImgToolkit::GetTextureData(string filePath, bool bReverse/* = true*/)
{
	TextureData* pTextureData = new TextureData;
	uInt bufferSize = 0;
	unsigned char* pBit;
	int format;
	Palette palette;
	string ext = StringHelper::StrToUpper(StringHelper::GetExt(filePath));
	if (StringHelper::CompareNoCase(ext, ".JPG") || StringHelper::CompareNoCase(ext, ".JPEG"))
	{
		JpgToBuffer(filePath, pTextureData, bufferSize, pBit, format);
	}
	else if (StringHelper::CompareNoCase(ext, ".PNG"))
	{
		PngToBuffer(filePath, pTextureData, bufferSize, pBit, format, palette);
	}
	else if(StringHelper::CompareNoCase(ext,".TIF")|| StringHelper::CompareNoCase(ext, ".TIFF")\
		|| StringHelper::CompareNoCase(ext, ".BMP"))
	{
		GDALToBuffer(filePath, pTextureData, bufferSize, pBit, format, palette);
	}
	BufferToTextureData(pTextureData, bufferSize, pBit, format, palette);
	return pTextureData;
}

bool ImgToolkit::JpgToBuffer(string filePath, TextureData*& pTextureData, uInt &bufferSize, unsigned char*& pBits, int& format, bool bReverse /*= true*/)
{
	FILE* m_File = (FILE*)fopen(filePath.c_str(), "rb");
	if (m_File == NULL)
	{
		return false;
	}
	else
	{
		//! \brief jpeg解压
		struct jpeg_decompress_struct m_dinfo;
		//! \brief 错误处理
		struct jpeg_error_mgr m_jerr;

		m_dinfo.err = jpeg_std_error(&m_jerr);
		size_t nsize = sizeof(struct jpeg_decompress_struct);
		jpeg_CreateDecompress(&m_dinfo, JPEG_LIB_VERSION, nsize);

		jpeg_stdio_src(&m_dinfo, m_File);
		if (jpeg_read_header(&m_dinfo, TRUE) != JPEG_HEADER_OK)
		{
			jpeg_destroy_decompress(&m_dinfo);
			return false;
		}

		short nBitCount = short(m_dinfo.num_components * 8);	//比特数

		pTextureData->m_nWidth = m_dinfo.image_width;
		pTextureData->m_nHeight = m_dinfo.image_height;
		//textureData->m_pBuffer = pBits;

		//BeginRead
		{
			jpeg_create_decompress(&m_dinfo);
			fseek(m_File, 0, SEEK_SET);
			jpeg_stdio_src(&m_dinfo, m_File);
			if (jpeg_read_header(&m_dinfo, TRUE) != JPEG_HEADER_OK)
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

		//读取像素行数据
		unsigned char* pByteLine;
		//每行像素所占字节数
		int nWidthBytes;
		//CMYK会被转化为RGB
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
			nWidthBytes = m_dinfo.image_width*m_dinfo.num_components;
			pByteLine = new unsigned char[nWidthBytes];
			pBits = new unsigned char[nWidthBytes * m_dinfo.image_height];
			::memset(pBits, 0, nWidthBytes * m_dinfo.image_height);
		}
		bufferSize = nWidthBytes * m_dinfo.image_height;
		for (int iLine = 0; iLine < pTextureData->m_nHeight; iLine++)
		{
			//CMYK转换为RGB显示，不影响显示效果
			if (m_dinfo.output_components == 4 &&
				m_dinfo.out_color_space == JCS_CMYK)
			{
				unsigned char* pBuffer = new unsigned char[m_dinfo.image_width*m_dinfo.num_components];
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
		//EndRead
		jpeg_destroy_decompress(&m_dinfo);
		format = JpgBit2Format(nBitCount, m_dinfo);
		if (NULL != m_File)
		{
			fclose(m_File);
			m_File = NULL;
		}
		return true;
	}
}

//----------------------------回调函数--------------------------------
static void png_user_error(png_structp png_ptr, const char *error_message)
{
	printf("libpng: %s", error_message);

	jmp_buf* psSetJmpContext = (jmp_buf*)png_get_error_ptr(png_ptr);
	if (psSetJmpContext)
	{
		longjmp(*psSetJmpContext, 1);
	}
}

static void png_user_warning(png_structp png_ptr, const char *error_message)
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

bool ImgToolkit::PngToBuffer(string filePath, TextureData*& pTextureData, uInt &BufferSize, unsigned char*& pBits, int& format, Palette& palette, bool bReverse /*= true*/)
{
	FILE* m_File = (FILE*)fopen(filePath.c_str(), "rb");
	if (m_File == NULL)
	{
		return false;
	}
	else
	{
		//! \breif png文件及相关属性指针
		png_structp m_pPngPtr;
		m_pPngPtr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if (NULL == m_pPngPtr)
		{
			fclose(m_File);
			m_File = NULL;

			return FALSE;
		}

		//! \breif png图片操作指针
		png_infop   m_pPngInfo;
		m_pPngInfo = png_create_info_struct(m_pPngPtr);
		if (NULL == m_pPngInfo)
		{
			png_destroy_read_struct(&m_pPngPtr, NULL, NULL);
			fclose(m_File);
			m_File = NULL;

			return FALSE;
		}

		jmp_buf m_jmpContext;
		png_set_error_fn(m_pPngPtr, &m_jmpContext, png_user_error, png_user_warning);
		if (setjmp(m_jmpContext) != 0)
		{
			png_destroy_read_struct(&m_pPngPtr, &m_pPngInfo, NULL);
			fclose(m_File);
			m_File = NULL;

			return FALSE;
		}

		//是否有交错存储
		bool m_bIsInterlace;
		try
		{
			png_set_read_fn(m_pPngPtr, m_File, png_read_data);
			png_read_info(m_pPngPtr, m_pPngInfo);

			pTextureData->m_nWidth = m_pPngInfo->width;
			pTextureData->m_nHeight = m_pPngInfo->height;
			int m_nColorType = m_pPngInfo->color_type;
			int nPixelDepth = m_pPngInfo->pixel_depth;
			m_bIsInterlace = m_pPngInfo->interlace_type;

			switch (nPixelDepth)
			{
			case 1:
				format = IPF_MONO;
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
				format = IPF_UBYTE;
				break;
			case 4:
				format = IPF_FBIT;
				break;
			case 8:
				format = IPF_UBYTE;
				break;
			case 16:
			{
				if (m_nColorType == PNG_COLOR_TYPE_GRAY)
				{
					format = IPF_UTBYTE;
				}
				else
				{
					format = IPF_TBYTE;
				}
			}
			break;
			case 24:
				format = IPF_RGB;
				break;
			case 32:
			{

				format = IPF_RGBA;
			}
			break;
			case 48:
			{
				format = IPF_TRGB;
			}
			break;
			case 64:
			{
				format = IPF_LONGLONG;
			}
			break;
			default:
			{
				assert("图片格式不支持！");
				png_destroy_read_struct(&m_pPngPtr, &m_pPngInfo, NULL);
				fclose(m_File);
				m_File = NULL;

				return FALSE;
			}
			break;
			}

			if (m_nColorType == PNG_COLOR_TYPE_PALETTE)
			{
				png_color *pasPNGPalette;
				int nColorCount;
				unsigned char* trans = NULL;
				png_color_16 *trans_values = NULL;
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

				Palette::PaletteEntry paletteEntry;
				if (num_trans > 0)
				{
					unsigned char alpha;
					for (int nColor = nColorCount - 1; nColor >= 0; nColor--)
					{
						UGColor Color;
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
						UGColor Color;

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
					int noValue = nNoDataIndex;//无效值，暂时没使用
				}
			}

		}
		catch (...)
		{
			png_destroy_read_struct(&m_pPngPtr, &m_pPngInfo, NULL);
			fclose(m_File);
			m_File = NULL;

			return FALSE;
		}

		//每行像素所占字节数
		int nWidthBytes = m_pPngPtr->rowbytes;
		//读取像素行数据
		unsigned char* pByteLine = new unsigned char[nWidthBytes];
		//! \breif 当前行索引，用于交错存储判断是否取数据
		int m_nCurrentRowIndex = -1;
		//! \breif 最新一行数据，用于取数据是判断是否取的是当前数据
		int m_nLastLineRead = -1;
		unsigned char* m_pBufferTmp = nullptr;
		pBits = new unsigned char[nWidthBytes*pTextureData->m_nHeight];
		BufferSize = nWidthBytes * pTextureData->m_nHeight;
		for (int iLine = 0; iLine < pTextureData->m_nHeight; iLine++)
		{
			if (setjmp(m_jmpContext) != 0)
			{
				return FALSE;
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

					m_pBufferTmp = new unsigned char[nWidthBytes*pTextureData->m_nHeight];
					::memset(m_pBufferTmp, 0, sizeof(unsigned char)*nWidthBytes*pTextureData->m_nHeight);

					png_byte **pRowPointers = NULL;
					pRowPointers = (png_bytepp)malloc(pTextureData->m_nHeight * sizeof(png_bytep));

					for (int i = 0; i < pTextureData->m_nHeight; i++)
					{
						pRowPointers[i] = m_pBufferTmp + i * m_pPngPtr->rowbytes;
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
					//Restart();
					//if (setjmp(m_jmpContext) != 0)
					//{
					return FALSE;
					//}
				}
				else if (iLine%pTextureData->m_nWidth == 0 && iLine != 0)
				{
					//Restart();
					//if (setjmp(m_jmpContext) != 0)
					//{
					return FALSE;
					//}
				}
				while (iLine > m_nLastLineRead)
				{
					png_read_rows(m_pPngPtr, &row, NULL, 1);
					m_nLastLineRead++;
				}
				if (format == IPF_LONGLONG) {}
				else if (format == IPF_TRGB) {}
				else
				{
					::memcpy(pByteLine, m_pBufferTmp, nWidthBytes);
				}
			}
			int nowLine = bReverse ? pTextureData->m_nHeight - iLine - 1 : iLine;
			::memcpy(pBits + nWidthBytes * nowLine, pByteLine, nWidthBytes);
			::memset(pByteLine, 0, nWidthBytes);
		}
		//close文件
		png_destroy_read_struct(&m_pPngPtr, &m_pPngInfo, NULL);
		if (NULL != m_File)
		{
			fclose(m_File);
			m_File = NULL;
		}
	}
}

bool ImgToolkit::GDALToBuffer(string filePath, TextureData*& pTextureData, uInt &BufferSize, unsigned char*& pBits, int& format, Palette& palette, bool bReverse /*= true*/)
{
	GDALAllRegister();//注册所有驱动
	//! \brief GDAL文件句柄
	GDALDataset *m_pDataset;
	filePath = StringHelper::ANSI_to_UTF8(filePath);
	m_pDataset = (GDALDataset*)GDALOpen(filePath.c_str(), GA_ReadOnly);
	if (NULL == m_pDataset)
	{
		return FALSE;
	}
	pTextureData->m_nWidth = m_pDataset->GetRasterXSize();
	pTextureData->m_nHeight = m_pDataset->GetRasterYSize();
	int m_nBandCount = m_pDataset->GetRasterCount();//波段数，暂时没使用
	GDALRasterBand *pRasterBand = m_pDataset->GetRasterBand(1);
	if (NULL == pRasterBand)
	{
		return false;
	}
	//获取导入方式扫描线
	//pRasterBand->GetBlockSize(&)//暂时不明白意图
	// 设置位深
	GDALDataType dataType = pRasterBand->GetRasterDataType();
	bool bForceAsOneBand = false;
	switch (dataType)
	{
	case GDT_Byte:
		//format = IPF_UBYTE;
		//int nBandCount = m_pDataset->GetRasterCount();//获取波段数
		//多波段转化为单波段
		if (m_pDataset->GetRasterCount() == 3)
		{
			format = IPF_RGB;
			bForceAsOneBand = true;
		}
		else if (m_pDataset->GetRasterCount() == 4)
		{
			format = IPF_RGBA;
			bForceAsOneBand = true;
		}
		break;
	case GDT_UInt16:
		format = IPF_UTBYTE;
		break;
	case GDT_Int16:
		format = IPF_TBYTE;
		break;
	case GDT_UInt32:
		format = IPF_ULONG;
		break;
	case GDT_Int32:
		format = IPF_LONG;
		break;
	case GDT_Float32:
		format = IPF_FLOAT;
		break;
	case GDT_Float64:
		format = IPF_DOUBLE;
		break;
	default:
		cout << "不支持的像素格式：" << filePath << endl;
		return FALSE;
	}

#pragma region 读取调色板
	if (pRasterBand->GetColorTable() != NULL)
	{
		GDALColorTable *poColorTable = pRasterBand->GetColorTable();
		int nColorCount = poColorTable->GetColorEntryCount();
		int nMaxColorCount;
		switch (format)
		{
		case IPF_MONO:
			nMaxColorCount = 2;
			break;
		case IPF_FBIT:
			nMaxColorCount = 16;
			break;
		case IPF_UBYTE:
		case IPF_BYTE:
			nMaxColorCount = 256;
			break;
		case IPF_TBYTE:
		case IPF_UTBYTE:
			nMaxColorCount = 256 * 256;
			break;
		default:
			nMaxColorCount = 0;
			break;
		}
		if (nColorCount > nMaxColorCount)
		{
			nColorCount = nMaxColorCount;
		}
		for (int i = 0; i < nColorCount; i++)
		{
			const GDALColorEntry* pColor = poColorTable->GetColorEntry(i);
			Palette::PaletteEntry paletteEntry;
			UGColor color = Palette::RGBA(unsigned int(pColor->c3), unsigned int(pColor->c2), unsigned int(pColor->c1), 255);
			paletteEntry.peRed = Palette::REDVAL(color);
			paletteEntry.peGreen = Palette::GREENVAL(color);
			paletteEntry.peBlue = Palette::BLUEVAL(color);
			paletteEntry.peFlags = Palette::ALPHAVAL(color);
			palette.Add(paletteEntry);
		}
	}
#pragma endregion 

	int nWidthBytes = AlignWidth((PixelFormat)format, pTextureData->m_nWidth);
	//读取像素行数据
	//unsigned char* pByteLine = new unsigned char[nWidthBytes];
	BufferSize = nWidthBytes * pTextureData->m_nHeight;
	pBits = new unsigned char[BufferSize];
	::memset(pBits, 0, BufferSize);

	//for (int iLine = 0; iLine < pTextureData->m_nHeight; iLine++)
	//{
	if (bForceAsOneBand)
	{
		const int nWordSize = (GDALGetDataTypeSize(GDT_Byte) + 1) / 8;
		int nBands = m_pDataset->GetRasterCount();
		int nPixelSpace = nBands * nWordSize;
		int nLineSpace = nBands * nWordSize*pTextureData->m_nWidth;
		int nBandSpace = nWordSize;
		CPLErr bResult = CE_Failure;
		if (format == IPF_RGB)
		{
			int bandMap[3] = { 1,2,3 };
			bResult = m_pDataset->RasterIO(GF_Read, 0, 0, pTextureData->m_nWidth, \
				pTextureData->m_nHeight, (void*)pBits, pTextureData->m_nWidth, \
				pTextureData->m_nHeight, GDT_Byte, nBands, bandMap, nPixelSpace, nLineSpace, nBandSpace, NULL);
		}
		else
		{
			int bandMap[4] = { 1,2,3,4 };
			bResult = m_pDataset->RasterIO(GF_Read, 0, 0, pTextureData->m_nWidth, \
				pTextureData->m_nHeight, (void*)pBits, pTextureData->m_nWidth, \
				pTextureData->m_nHeight, GDT_Byte, nBands, bandMap, nPixelSpace, nLineSpace, nBandSpace, NULL);
		}
		if (bResult != CE_None)
		{
			return false;
		}
	}
	else
	{
		GDALRasterBand *pRasterBand = m_pDataset->GetRasterBand(1);
		if (pRasterBand == NULL)
		{
			return FALSE;
		}
		CPLErr bResult = pRasterBand->RasterIO(GF_Read, 0, 0, pTextureData->m_nWidth, \
			pTextureData->m_nHeight, (void*)pBits, pTextureData->m_nWidth, \
			pTextureData->m_nHeight, pRasterBand->GetRasterDataType(), 0, 0);

		if (bResult != CE_None)
		{
			return FALSE;
		}
	}
	if (bReverse)
	{
		unsigned char* ptmp = new unsigned char[nWidthBytes];
		int iLine = 0;
		int nHeight = pTextureData->m_nHeight;
		for (; iLine < nHeight / 2; iLine++)
		{
			memcpy(ptmp, pBits + iLine * nWidthBytes, nWidthBytes);
			memcpy(pBits + iLine * nWidthBytes, pBits + (nHeight - iLine - 1)*nWidthBytes, nWidthBytes);
			memcpy(pBits + (nHeight - iLine - 1)*nWidthBytes, ptmp, nWidthBytes);
		}
		delete[] ptmp;
		ptmp = NULL;
	}
	//}
}

bool ImgToolkit::BufferToTextureData(TextureData*& pTextureData, uInt BitsSize, unsigned char*& pBits, int format, Palette palette)
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
	unsigned char* pDes = pTextureData->m_pBuffer;
	switch (format)
	{
	case 32:
	case 24:
		bufferSize = pTextureData->m_nWidth * pTextureData->m_nHeight * format / 8;
		pTextureData->m_pBuffer = new unsigned char[bufferSize];

		if (bufferSize != BitsSize)  assert(false);

		::memcpy(pTextureData->m_pBuffer, pBits, bufferSize);
		pTextureData->m_enFormat = format / 8 == 3 ? PF_BYTE_RGB : PF_BYTE_RGBA;
		break;
	case 16:
		pTextureData->m_enFormat = PF_BYTE_RGBA;
		bufferSize = pTextureData->m_nWidth * pTextureData->m_nHeight * 4;
		pTextureData->m_pBuffer = new unsigned char[bufferSize];
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
		pTextureData->m_enFormat = PF_BYTE_RGBA;
		bufferSize = pTextureData->m_nWidth * pTextureData->m_nHeight * 4;
		pTextureData->m_pBuffer = new unsigned char[bufferSize];
		if (palette.GetSize() > 0)
		{
			for (int i = 0; i < BitsSize; i++)
			{
				int nValue = pBits[i];
				pDes[3 * i / 2] = palette.GetAt(nValue).peRed;
				pDes[3 * i / 2 + 1] = palette.GetAt(nValue).peGreen;
				pDes[3 * i / 2 + 2] = palette.GetAt(nValue).peBlue;
				pDes[3 * i / 2 + 3] = palette.GetAt(nValue).peFlags;
			}
		}
		else
		{
			for (int i = 0; i < BitsSize; i++)
			{
				int nValue = pBits[i];
				pDes[3 * i / 2] = nValue;
				pDes[3 * i / 2 + 1] = nValue;
				pDes[3 * i / 2 + 2] = nValue;
				pDes[3 * i / 2 + 3] = 255;
			}
		}
		break;
	default:
		return false;
	}
	pTextureData->m_nSize = bufferSize;
	return true;
}

PixelFormat ImgToolkit::JpgBit2Format(unsigned short nBitCount, struct jpeg_decompress_struct m_dinfo)
{
	switch (nBitCount)
	{
	case 1:
		return IPF_MONO;
	case 4:
		return IPF_FBIT;
	case 8:
		return IPF_UBYTE;
	case 16:
		return IPF_TBYTE;
	case 24:
		return IPF_RGB;
		//{{ Modified by liyq  [11/13/2014] JPEG 32位其颜色空间为CMYK，将其转换为RGB	
	case 32:
	{
		if (m_dinfo.num_components == 4 &&
			m_dinfo.out_color_space == JCS_CMYK)
		{
			return IPF_RGB;
		}
		else
		{
			return IPF_RGBA;
		}
	}
	//}}
	case 48:
		return IPF_TRGB;
	case 64:
		return IPF_DOUBLE;
	default:
		return IPF_UNKNOWN;
	}
}
