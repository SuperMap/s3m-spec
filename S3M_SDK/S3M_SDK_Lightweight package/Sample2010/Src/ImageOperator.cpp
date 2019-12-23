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
