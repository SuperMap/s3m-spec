#include "I3SLepccDecoder.h"
#include "S3MBCommon.h"

#include <assert.h>
#include <iostream>

#define LEPCC_ERROR_BUFFERTOOSMALL "buffer too small"
#define LEPCC_ERROR_NOTLEPCC "not lepcc"
#define LEPCC_ERROR_NOTRGB "not rgb"
#define LEPCC_ERROR_NOTINTENSITY "not intensity"
#define LEPCC_ERROR_WRONGVERSION "wrong version"
#define LEPCC_ERROR_WRONGCHECKSUM "wrong checksum"

static const int kCurrVersion = 1;

namespace S3MB
{
	bool LepccBitStuffer::Decode(const unsigned char** ppBuffer, std::vector<unsigned int>& vecData, int nLerc2Version)
	{
		if (ppBuffer == nullptr)
		{
			return false;
		}

		unsigned char nNumBitsByte = **ppBuffer;
		(*ppBuffer)++;

		int nBits67 = nNumBitsByte >> 6;
		int nNb = (nBits67 == 0) ? 4 : 3 - nBits67;

		bool bDoLut = (nNumBitsByte & (1 << 5)) ? true : false;
		nNumBitsByte &= 31;
		int nNumBits = nNumBitsByte;

		unsigned int nNumElements = 0;
		if (!DecodeUInt(ppBuffer, nNumElements, nNb))
		{
			return false;
		}

		if (!bDoLut)
		{
			if (nNumBits > 0)
			{
				if (nLerc2Version >= 3)
				{
					BitUnStuff(ppBuffer, vecData, nNumElements, nNumBits);
				}
				else
				{
					BitUnStuffBeforeLerc2v3(ppBuffer, vecData, nNumElements, nNumBits);
				}
			}
			else
			{
				vecData.resize(nNumElements);
				memset(&vecData[0], 0, nNumElements * sizeof(unsigned int));
			}
		}
		else
		{
			unsigned char nLutByte = **ppBuffer;
			(*ppBuffer)++;

			int nLut = nLutByte - 1;

			if (nLerc2Version >= 3)
			{
				BitUnStuff(ppBuffer, m_vecLut, nLut, nNumBits);
			}
			else
			{
				BitUnStuffBeforeLerc2v3(ppBuffer, m_vecLut, nLut, nNumBits);
			}

			int nBitsLut = 0;
			while (nLut >> nBitsLut)
			{
				nBitsLut++;
			}

			if (nLerc2Version >= 3)
			{
				BitUnStuff(ppBuffer, vecData, nNumElements, nBitsLut);
			}
			else
			{
				BitUnStuffBeforeLerc2v3(ppBuffer, vecData, nNumElements, nBitsLut);
			}

			m_vecLut.insert(m_vecLut.begin(), 0);
			for (unsigned int i = 0; i < nNumElements; i++)
			{
				vecData[i] = m_vecLut[vecData[i]];
			}
		}

		return true;
	}

	void LepccBitStuffer::BitUnStuff(const unsigned char** ppBuffer, std::vector<unsigned int>& vecData,
		unsigned int nNumElements, int nNumBits)
	{
		vecData.resize(nNumElements);

		unsigned int nNumUInts = (nNumElements * nNumBits + 31) / 32;
		unsigned int nNumBytes = nNumUInts * sizeof(unsigned int);

		m_vecBitStuff.resize(nNumUInts);
		m_vecBitStuff[nNumUInts - 1] = 0;

		int nNumBytesUsed = nNumBytes - NumTailBytesNotNeeded(nNumElements, nNumBits);
		memcpy(&m_vecBitStuff[0], *ppBuffer, nNumBytesUsed);

		unsigned int* pSrcPtr = &m_vecBitStuff[0];
		unsigned int* pDstPtr = &vecData[0];
		int nBitPos = 0;
		int nNb = 32 - nNumBits;

		for (unsigned int i = 0; i < nNumElements; i++)
		{
			if (nNb - nBitPos >= 0)
			{
				*pDstPtr++ = ((*pSrcPtr) << (nNb - nBitPos)) >> nNb;
				nBitPos += nNumBits;
				if (nBitPos == 32)
				{
					pSrcPtr++;
					nBitPos = 0;
				}
			}
			else
			{
				*pDstPtr = (*pSrcPtr++) >> nBitPos;
				*pDstPtr++ |= ((*pSrcPtr) << (64 - nNumBits - nBitPos)) >> nNb;
				nBitPos -= nNb;
			}
		}

		*ppBuffer += nNumBytesUsed;
	}

	void LepccBitStuffer::BitUnStuffBeforeLerc2v3(const unsigned char** ppBuffer, std::vector<unsigned int>& vecData,
		unsigned int nNumElements, int nNumBits)
	{
		vecData.resize(nNumElements, 0);

		unsigned int nNumUInts = (nNumElements * nNumBits + 31) / 32;
		unsigned int nNumBytes = nNumUInts * sizeof(unsigned int);
		unsigned int* pBuffer = (unsigned int*)(*ppBuffer);

		unsigned int* pSrcPtr = pBuffer;
		pSrcPtr += nNumUInts;

		pSrcPtr--;
		unsigned int nLastUInt = *pSrcPtr;
		unsigned int nNumBytesNotNeeded = NumTailBytesNotNeeded(nNumElements, nNumBits);
		unsigned int n = nNumBytesNotNeeded;

		while (n--)
		{
			unsigned int nVal;
			memcpy(&nVal, pSrcPtr, sizeof(unsigned int));
			nVal <<= 8;
			memcpy(pSrcPtr, &nVal, sizeof(unsigned int));
		}

		pSrcPtr = pBuffer;
		unsigned int* pDstPtr = &vecData[0];
		int nBitPos = 0;

		for (unsigned int i = 0; i < nNumElements; i++)
		{
			if (32 - nBitPos >= nNumBits)
			{
				unsigned int nVal;
				memcpy(&nVal, pSrcPtr, sizeof(unsigned int));
				unsigned int n = nVal << nBitPos;

				*pDstPtr++ = n >> (32 - nNumBits);
				nBitPos += nNumBits;
				if (nBitPos == 32)
				{
					nBitPos = 0;
					pSrcPtr++;
				}
			}
			else
			{
				unsigned int nVal;
				memcpy(&nVal, pSrcPtr, sizeof(unsigned int));
				pSrcPtr++;
				unsigned int n = nVal << nBitPos;
				*pDstPtr = n >> (32 - nNumBits);
				nBitPos -= (32 - nNumBits);
				memcpy(&nVal, pSrcPtr, sizeof(unsigned int));
				*pDstPtr++ |= nVal >> (32 - nBitPos);
			}
		}

		if (nNumBytesNotNeeded > 0)
			memcpy(pSrcPtr, &nLastUInt, sizeof(unsigned int));

		*ppBuffer += nNumBytes - nNumBytesNotNeeded;
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	bool LepccXYZ::Decode(const unsigned char** ppBuffer, int64_t nBufferSize, unsigned int& nPts, Point3D*& pPts)
	{
		if (ppBuffer == nullptr || *ppBuffer == nullptr)
		{
			return false;
		}

		int nHeaderSize = GetHeaderSize();
		if (nBufferSize <= nHeaderSize)
		{
			std::cout << LEPCC_ERROR_BUFFERTOOSMALL << std::endl;
			assert(false);
			return false;
		}

		const unsigned char* pBuffer = *ppBuffer;
		const unsigned char* pStart = pBuffer;

		TopHeader topHd;
		Header1 hd1;
		if (!ReadHeaders(pBuffer, nBufferSize, topHd, hd1))
		{
			return false;
		}

		pBuffer += nHeaderSize;

		if (nBufferSize < hd1.nBlobSize)
		{
			std::cout << LEPCC_ERROR_BUFFERTOOSMALL << std::endl;
			assert(false);
			return false;
		}

		unsigned int nCheckSum = LepccDecoder::ComputeChecksumFletcher32(pStart + sizeof(topHd), hd1.nBlobSize - sizeof(topHd));
		if (nCheckSum != topHd.nCheckSum)
		{
			std::cout << LEPCC_ERROR_WRONGCHECKSUM << std::endl;
			assert(false);
			return false;
		}

		LepccExtent3D extent = hd1.extent;
		Point3D pntMaxError = Point3D(hd1.pntMaxErrorX, hd1.pntMaxErrorY, hd1.pntMaxErrorZ);

		*ppBuffer = pBuffer;

		std::vector<unsigned int> vecYDelta, vecNumPointsPerRow, vecXDelta, vecZ;
		if (!DecodeCutInSegments(ppBuffer, vecYDelta))
		{
			return false;
		}
		if (!DecodeCutInSegments(ppBuffer, vecNumPointsPerRow))
		{
			return false;
		}
		if (!DecodeCutInSegments(ppBuffer, vecXDelta))
		{
			return false;
		}
		if (!DecodeCutInSegments(ppBuffer, vecZ))
		{
			return false;
		}

		//内部创建数组
		nPts = hd1.nNumPoints;
		delete[] pPts;
		pPts = new Point3D[nPts];

		Point3D pnt0 = Point3D(extent.pntLowerX, extent.pntLowerY, extent.pntLowerZ);
		Point3D pnt1 = Point3D(extent.pntUpperX, extent.pntUpperY, extent.pntUpperZ);

		Point3D pntCw(2 * pntMaxError.x, 2 * pntMaxError.y, 2 * pntMaxError.z);

		int nCnt = 0;
		int iy = 0;
		int nRows = (int)vecYDelta.size();

		for (int i = 0; i < nRows; i++)
		{
			iy += vecYDelta[i];
			int ix = 0;
			int nPtsPerRow = vecNumPointsPerRow[i];

			for (int j = 0; j < nPtsPerRow; j++)
			{
				ix += vecXDelta[nCnt];
				int iz = vecZ[nCnt];

				double x = pnt0.x + ix * pntCw.x;
				double y = pnt0.y + iy * pntCw.y;
				double z = pnt0.z + iz * pntCw.z;

				pPts[nCnt] = Point3D(MIN(x, pnt1.x), MIN(y, pnt1.y), MIN(z, pnt1.z));
				nCnt++;
			}
		}

		int64_t nBytesRead = (int64_t)(*ppBuffer - pStart);
		if (nBytesRead != hd1.nBlobSize || nBytesRead > nBufferSize)
		{
			nPts = 0;
			delete[] pPts;
			pPts = nullptr;
			return false;
		}

		return true;
	}

	bool LepccXYZ::DecodeCutInSegments(const unsigned char** ppBuffer, std::vector<unsigned int>& vecData)
	{
		if (ppBuffer == nullptr || *ppBuffer == nullptr)
		{
			return false;
		}

		vecData.resize(0);

		std::vector<unsigned int> vecSectionMin, vecZeroBasedData;
		LepccBitStuffer bitStuffer;
		if (!bitStuffer.Decode(ppBuffer, vecSectionMin, 3))
		{
			return false;
		}

		int nNumSections = (int)vecSectionMin.size();

		vecData.reserve(nNumSections * 128);

		for (int i = 0; i < nNumSections; i++)
		{
			if (!bitStuffer.Decode(ppBuffer, vecZeroBasedData, 3))
			{
				return false;
			}

			int nLen = (int)vecZeroBasedData.size();
			unsigned int nMinElem = vecSectionMin[i];

			for (int j = 0; j < nLen; j++)
			{
				vecData.push_back(vecZeroBasedData[j] + nMinElem);
			}
		}

		return true;
	}

	int LepccXYZ::GetHeaderSize()
	{
		return (int)(sizeof(TopHeader) + sizeof(Header1));
	}

	bool LepccXYZ::ReadHeaders(const unsigned char* pBuffer, int64_t nBufferSize, TopHeader& topHd, Header1& hd1)
	{
		if (pBuffer == nullptr)
		{
			return false;
		}

		if (nBufferSize <= GetHeaderSize())
		{
			std::cout << LEPCC_ERROR_BUFFERTOOSMALL << std::endl;
			assert(false);
			return false;
		}

		//对比file key
		TopHeader refHd;
		if (0 != memcmp(pBuffer, refHd.fileKey, refHd.GetFileKeyLength()))
		{
			std::cout << LEPCC_ERROR_NOTLEPCC << std::endl;
			assert(false);
			return false;
		}

		std::memcpy(&topHd, pBuffer, sizeof(topHd));
		pBuffer += sizeof(topHd);
		if (topHd.nVersion > kCurrVersion)
		{
			std::cout << LEPCC_ERROR_WRONGVERSION << std::endl;
			assert(false);
			return false;
		}

		std::memcpy(&hd1, pBuffer, sizeof(hd1));
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	bool LepccRGB::Decode(const unsigned char** ppBuffer, int64_t nBufferSize, unsigned int& nPts, RGB24*& pColors)
	{
		if (ppBuffer == nullptr || *ppBuffer == nullptr)
		{
			return false;
		}

		int nHeaderSize = GetHeaderSize();

		if (nBufferSize <= nHeaderSize)
		{
			std::cout << LEPCC_ERROR_BUFFERTOOSMALL << std::endl;
			assert(false);
			return false;
		}

		const unsigned char* pBuffer = *ppBuffer;
		const unsigned char* pStart = pBuffer;

		TopHeader topHd;
		Header1 hd1;
		if (!ReadHeaders(pBuffer, nBufferSize, topHd, hd1))
		{
			return false;
		}

		pBuffer += nHeaderSize;

		if (nBufferSize < hd1.nBlobSize)
		{
			std::cout << LEPCC_ERROR_BUFFERTOOSMALL << std::endl;
			assert(false);
			return false;
		}

		unsigned int nCheckSum = LepccDecoder::ComputeChecksumFletcher32(pStart + sizeof(topHd), hd1.nBlobSize - sizeof(topHd));
		if (nCheckSum != topHd.nCheckSum)
		{
			std::cout << LEPCC_ERROR_WRONGCHECKSUM << std::endl;
			assert(false);
			return false;
		}

		nPts = hd1.nNumPoints;
		delete[] pColors;
		pColors = new RGB24[nPts];

		unsigned short nNumColors = hd1.nNumColorsInColormap;
		if (nNumColors == 0)
		{
			int nBytes = hd1.nNumPoints * 3;

			if (nBufferSize < (int64_t)(nHeaderSize + nBytes))
			{
				nPts = 0;
				delete[] pColors;
				pColors = nullptr;
				std::cout << LEPCC_ERROR_BUFFERTOOSMALL << std::endl;
				assert(false);
				return false;
			}

			for (unsigned int i = 0; i < hd1.nNumPoints; i++)
			{
				pColors[i].red = pBuffer[0];
				pColors[i].green = pBuffer[1];
				pColors[i].blue = pBuffer[2];
				pBuffer += 3;
			}
		}
		else
		{
			if (nBufferSize < (int64_t)(nHeaderSize + nNumColors * 3))
			{
				nPts = 0;
				delete[] pColors;
				pColors = nullptr;
				std::cout << LEPCC_ERROR_BUFFERTOOSMALL << std::endl;
				assert(false);
				return false;
			}
			std::vector<RGBA32> mapColor;
			mapColor.resize(nNumColors);
			memset(&mapColor[0], 0, mapColor.size() * sizeof(mapColor[0]));

			for (unsigned short i = 0; i < nNumColors; i++)
			{
				mapColor[i].red = pBuffer[0];
				mapColor[i].green = pBuffer[1];
				mapColor[i].blue = pBuffer[2];
				pBuffer += 3;
			}

			if (hd1.nColorIndexCompressionMethod == NoCompression)
			{
				if (nBufferSize < (int64_t)(nHeaderSize + nNumColors * 3 + hd1.nNumPoints * 1))
				{
					nPts = 0;
					delete[] pColors;
					pColors = nullptr;
					std::cout << LEPCC_ERROR_BUFFERTOOSMALL << std::endl;
					assert(false);
					return false;
				}

				for (unsigned int i = 0; i < hd1.nNumPoints; i++)
				{
					unsigned char nIndex = *pBuffer++;
					pColors[i].red = mapColor[nIndex].red;
					pColors[i].green = mapColor[nIndex].green;
					pColors[i].blue = mapColor[nIndex].blue;
				}
			}
			else if (hd1.nColorIndexCompressionMethod == AllConst)
			{
				RGBA32 rgba = mapColor[0];

				for (unsigned int i = 0; i < hd1.nNumPoints; i++)
				{
					pColors[i].red = rgba.red;
					pColors[i].green = rgba.green;
					pColors[i].blue = rgba.blue;
				}
			}
			else
			{
				nPts = 0;
				delete[] pColors;
				pColors = nullptr;
				return false;
			}
		}

		*ppBuffer = pBuffer;

		int64_t nBytesRead = (int64_t)(*ppBuffer - pStart);
		if (nBytesRead != hd1.nBlobSize || nBytesRead > nBufferSize)
		{
			nPts = 0;
			delete[] pColors;
			pColors = nullptr;
			return false;
		}

		return true;
	}

	int LepccRGB::GetHeaderSize()
	{
		return (int)(sizeof(TopHeader) + sizeof(Header1));
	}

	bool LepccRGB::ReadHeaders(const unsigned char* pBuffer, int64_t nBufferSize, TopHeader& topHd, Header1& hd1)
	{
		if (pBuffer == nullptr)
		{
			return false;
		}

		if (nBufferSize <= GetHeaderSize())
		{
			std::cout << LEPCC_ERROR_BUFFERTOOSMALL << std::endl;
			assert(false);
			return false;
		}

		TopHeader refHd;
		if (0 != memcmp(pBuffer, refHd.fileKey, refHd.GetFileKeyLength()))
		{
			std::cout << LEPCC_ERROR_NOTRGB << std::endl;
			assert(false);
			return false;
		}

		memcpy(&topHd, pBuffer, sizeof(topHd));
		pBuffer += sizeof(topHd);

		if (topHd.nVersion > kCurrVersion)
		{
			std::cout << LEPCC_ERROR_WRONGVERSION << std::endl;
			assert(false);
			return false;
		}

		memcpy(&hd1, pBuffer, sizeof(hd1));
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	bool LepccIntensity::Decode(const unsigned char** ppBuffer, int64_t nBufferSize, unsigned int& nElem, unsigned short*& pIntensities)
	{
		if (ppBuffer == nullptr || *ppBuffer == nullptr)
		{
			return false;
		}

		int nHeaderSize = GetHeaderSize();

		if (nBufferSize <= nHeaderSize)
		{
			std::cout << LEPCC_ERROR_BUFFERTOOSMALL << std::endl;
			assert(false);
			return false;
		}

		const unsigned char* pBuffer = *ppBuffer;
		const unsigned char* pStart = pBuffer;

		TopHeader topHd;
		Header1 hd1;
		if (!ReadHeaders(pBuffer, nBufferSize, topHd, hd1))
		{
			return false;
		}

		pBuffer += nHeaderSize;

		if (nBufferSize < hd1.nBlobSize)
		{
			std::cout << LEPCC_ERROR_BUFFERTOOSMALL << std::endl;
			assert(false);
			return false;
		}

		unsigned int nCheckSum = LepccDecoder::ComputeChecksumFletcher32(pStart + sizeof(topHd), hd1.nBlobSize - sizeof(topHd));
		if (nCheckSum != topHd.nCheckSum)
		{
			std::cout << LEPCC_ERROR_WRONGCHECKSUM << std::endl;
			assert(false);
			return false;
		}

		nElem = hd1.nNumPoints;
		delete[] pIntensities;
		pIntensities = new unsigned short[nElem];

		unsigned short nScale = hd1.nScaleFactor;
		if (nScale < 1)
		{
			nElem = 0;
			delete[] pIntensities;
			pIntensities = nullptr;
			return false;
		}

		int nBpp = hd1.nBpp;
		if (nBpp > 16)
		{
			nElem = 0;
			delete[] pIntensities;
			pIntensities = nullptr;
			return false;
		}

		*ppBuffer = pBuffer;

		if (nBpp == 16)
		{
			int n = nElem * sizeof(unsigned short);
			memcpy(pIntensities, *ppBuffer, n);
			*ppBuffer += n;
		}
		else if (nBpp == 8 && nScale == 1)
		{
			for (unsigned int i = 0; i < nElem; i++)
			{
				pIntensities[i] = *pBuffer++;
			}

			*ppBuffer += nElem;
		}
		else
		{
			if (nBpp == 8)
			{
				m_vecData.resize(nElem);

				for (unsigned int i = 0; i < nElem; i++)
				{
					m_vecData[i] = *pBuffer++;
				}

				*ppBuffer += nElem;
			}
			else
			{
				LepccBitStuffer bitStuffer;
				if (!bitStuffer.Decode(ppBuffer, m_vecData, 3))
				{
					nElem = 0;
					delete[] pIntensities;
					pIntensities = nullptr;
					return false;
				}
			}

			if ((unsigned int)m_vecData.size() > nElem)
			{
				nElem = 0;
				delete[] pIntensities;
				pIntensities = nullptr;
				return false;
			}

			for (unsigned int i = 0; i < nElem; i++)
			{
				pIntensities[i] = (unsigned short)(m_vecData[i] * nScale);
			}
		}

		int64_t nBytesRead = (int64_t)(*ppBuffer - pStart);
		if (nBytesRead != hd1.nBlobSize || nBytesRead > nBufferSize)
		{
			nElem = 0;
			delete[] pIntensities;
			pIntensities = nullptr;
			return false;
		}

		return true;
	}

	void LepccIntensity::Clear()
	{
		m_vecData.clear();
	}

	int LepccIntensity::GetHeaderSize()
	{
		return (int)(sizeof(TopHeader) + sizeof(Header1));
	}

	bool LepccIntensity::ReadHeaders(const unsigned char* pBuffer, int64_t nBufferSize, TopHeader& topHd, Header1& hd1)
	{
		if (pBuffer == nullptr)
		{
			return false;
		}

		if (nBufferSize <= GetHeaderSize())
		{
			std::cout << LEPCC_ERROR_BUFFERTOOSMALL << std::endl;
			assert(false);
			return false;
		}

		TopHeader refHd;
		if (0 != memcmp(pBuffer, refHd.fileKey, refHd.GetFileKeyLength()))
		{
			std::cout << LEPCC_ERROR_NOTINTENSITY << std::endl;
			assert(false);
			return false;
		}

		memcpy(&topHd, pBuffer, sizeof(topHd));
		pBuffer += sizeof(topHd);

		if (topHd.nVersion > kCurrVersion)
		{
			std::cout << LEPCC_ERROR_WRONGVERSION << std::endl;
			assert(false);
			return false;
		}

		memcpy(&hd1, pBuffer, sizeof(hd1));
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	bool LepccDecoder::DecodeXYZ(const unsigned char* pBuffer, int64_t nBufferSize, unsigned int& nPts, Point3D*& pPts)
	{
		LepccXYZ lepccXYZ;
		return lepccXYZ.Decode(&pBuffer, nBufferSize, nPts, pPts);
	}

	bool LepccDecoder::DecodeRGB(const unsigned char* pBuffer, int64_t nBufferSize, unsigned int& nPts, RGB24*& pColors)
	{
		LepccRGB lepccRGB;
		return lepccRGB.Decode(&pBuffer, nBufferSize, nPts, pColors);
	}

	bool LepccDecoder::DecodeIntensity(const unsigned char* pBuffer, int64_t nBufferSize, unsigned int& nElem, unsigned short*& pIntensities)
	{
		LepccIntensity lepccIntensity;
		return lepccIntensity.Decode(&pBuffer, nBufferSize, nElem, pIntensities);
	}

	unsigned int LepccDecoder::ComputeChecksumFletcher32(const unsigned char* pBuffer, unsigned long long nBufferSize)
	{
		unsigned int nSum1 = 0xffff, nSum2 = 0xffff;
		unsigned long long nWords = nBufferSize / 2;

		while (nWords)
		{
			unsigned int nTlen = (nWords >= 359) ? 359 : (uint32_t)nWords;
			nWords -= nTlen;
			do {
				nSum1 += (*pBuffer++ << 8);
				nSum2 += nSum1 += *pBuffer++;
			} while (--nTlen);

			nSum1 = (nSum1 & 0xffff) + (nSum1 >> 16);
			nSum2 = (nSum2 & 0xffff) + (nSum2 >> 16);
		}

		if (nBufferSize & 1)
		{
			nSum2 += nSum1 += (*pBuffer << 8);
		}

		nSum1 = (nSum1 & 0xffff) + (nSum1 >> 16);
		nSum2 = (nSum2 & 0xffff) + (nSum2 >> 16);

		return nSum2 << 16 | nSum1;
	}
}
