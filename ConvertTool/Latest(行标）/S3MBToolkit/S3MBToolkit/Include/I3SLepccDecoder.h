#if !defined(I3SLEPCCDECODER_H__712DA6DA_CDA2_4874_8315_3713C208244F__INCLUDED_)
#define I3SLEPCCDECODER_H__712DA6DA_CDA2_4874_8315_3713C208244F__INCLUDED_

#pragma once
#include "Common.h"
#include "Point3D.h"
#include "stdafx.h"
#include <cstring>

#include <vector>

namespace S3MB
{
	typedef struct
	{
		double pntLowerX;
		double pntLowerY;
		double pntLowerZ;
		double pntUpperX;
		double pntUpperY;
		double pntUpperZ;
	} LepccExtent3D;

	class STK_API LepccBitStuffer
	{
	public:
		bool Decode(const unsigned char** ppBuffer, std::vector<unsigned int>& vecData, int nLerc2Version);

	private:
		inline bool DecodeUInt(const unsigned char** ppBuffer, unsigned int& k, int nNumBuffer)
		{
			const unsigned char* pBuffer = *ppBuffer;

			if (nNumBuffer == 1)
			{
				k = *pBuffer;
			}
			else if (nNumBuffer == 2)
			{
				unsigned short s;
				memcpy(&s, pBuffer, sizeof(unsigned short));
				k = s;
			}
			else if (nNumBuffer == 4)
			{
				memcpy(&k, pBuffer, sizeof(unsigned int));
			}
			else
			{
				return false;
			}

			*ppBuffer += nNumBuffer;
			return true;
		}

		inline unsigned int NumTailBytesNotNeeded(unsigned int nNumElem, int nNumBits)
		{
			int nNumBitsTail = (nNumElem * nNumBits) & 31;
			int nNumBytesTail = (nNumBitsTail + 7) >> 3;
			return (nNumBytesTail > 0) ? 4 - nNumBytesTail : 0;
		}

		void BitUnStuff(const unsigned char** ppBuffer, std::vector<unsigned int>& vecData,
			unsigned int nNumElements, int nNumBits);

		void BitUnStuffBeforeLerc2v3(const unsigned char** ppBuffer, std::vector<unsigned int>& vecData,
			unsigned int nNumElements, int nNumBits);

	private:
		mutable std::vector<unsigned int>  m_vecLut, m_vecIndex, m_vecBitStuff;
	};

	class STK_API LepccXYZ
	{
	public:
		bool Decode(const unsigned char** ppBuffer, int64_t nBufferSize, unsigned int& nPts, Point3D*& pPts);

	private:
		struct TopHeader
		{
			char fileKey[10];
			unsigned short nVersion; // °æ±¾
			unsigned int nCheckSum;

			TopHeader() : nVersion(1), nCheckSum(0)
			{
				std::string fk = "LEPCC     ";
				std::memcpy(&fileKey[0], fk.c_str(), fk.length());
			}

			static int GetFileKeyLength() { return 10; }
		};
		static_assert(sizeof(TopHeader) == 16, "Unexpected size/packing");

		struct Header1
		{
			int64_t nBlobSize;
			LepccExtent3D extent;
			double pntMaxErrorX;
			double pntMaxErrorY;
			double pntMaxErrorZ;
			unsigned int nNumPoints;
			unsigned int nReserved;

			Header1() : nBlobSize(0), nNumPoints(0), nReserved(0) {}
		};
		static_assert(sizeof(Header1) == 88, "Unexpected size/packing");

		bool DecodeCutInSegments(const unsigned char** ppBuffer, std::vector<unsigned int>& vecData);

		static int GetHeaderSize();
		static bool ReadHeaders(const unsigned char* pBuffer, int64_t nBufferSize, TopHeader& topHd, Header1& hd1);
	};

	class STK_API LepccRGB
	{
	public:
		bool Decode(const unsigned char** ppBuffer, int64_t nBufferSize, unsigned int& nPts, RGB24*& pColors);

	private:
		enum ColorIndexCompressionMethod { NoCompression = 0, AllConst, HuffmanCodec };

		struct TopHeader
		{
			char fileKey[10];
			unsigned short nVersion;
			unsigned int nCheckSum;

			TopHeader() : nVersion(1), nCheckSum(0)
			{
				std::string fk("ClusterRGB");
				memcpy(&fileKey[0], fk.c_str(), fk.length());
			}

			static int GetFileKeyLength() { return 10; }
		};
		static_assert(sizeof(TopHeader) == 16, "Unexpected size/packing");

		struct Header1
		{
			int64_t nBlobSize;
			unsigned int nNumPoints;
			unsigned short nNumColorsInColormap;
			unsigned char nColorLookupMethod;
			unsigned char nColorIndexCompressionMethod;

			Header1() : nBlobSize(0), nNumPoints(0), nNumColorsInColormap(0), nColorLookupMethod(0), nColorIndexCompressionMethod(0) {}
		};
		static_assert(sizeof(Header1) == 16, "Unexpected size/packing");

		static int GetHeaderSize();
		static bool ReadHeaders(const unsigned char* pBuffer, int64_t nBufferSize, TopHeader& topHd, Header1& hd1);
	};

	class STK_API LepccIntensity
	{
	public:
		bool Decode(const unsigned char** ppBuffer, int64_t nBufferSize, unsigned int& nElem, unsigned short*& pIntensities);

		void Clear();

	private:
		struct TopHeader
		{
			char fileKey[10];
			unsigned short nVersion;
			unsigned int nCheckSum;

			TopHeader() : nVersion(1), nCheckSum(0)
			{
				static const char kIntensity[] = "Intensity ";
				memcpy(fileKey, kIntensity, 10);
			}

			static int GetFileKeyLength() { return 10; }
		};
		static_assert(sizeof(TopHeader) == 16, "Unexpected size/packing");

		struct Header1
		{
			int64_t nBlobSize;
			unsigned int nNumPoints;
			unsigned short nScaleFactor;
			unsigned char nBpp;
			unsigned char nReserved;

			Header1() : nBlobSize(0), nNumPoints(0), nScaleFactor(0), nBpp(0), nReserved(0) {}
		};
		static_assert(sizeof(Header1) == 16, "Unexpected size/packing");

		mutable std::vector<unsigned int>  m_vecData;

		static int GetHeaderSize();
		static bool ReadHeaders(const unsigned char* pBuffer, int64_t nBufferSize, TopHeader& topHd, Header1& hd1);
	};

	// i3s lepcc
	class STK_API LepccDecoder
	{
	public:
		static bool DecodeXYZ(const unsigned char* pBuffer, int64_t nBufferSize, unsigned int& nPts, Point3D*& pPts);
		static bool DecodeRGB(const unsigned char* pBuffer, int64_t nBufferSize, unsigned int& nPts, RGB24*& pColors);
		static bool DecodeIntensity(const unsigned char* pBuffer, int64_t nBufferSize, unsigned int& nElem, unsigned short*& pIntensities);

		static unsigned int ComputeChecksumFletcher32(const unsigned char* pBuffer, unsigned long long nBufferSize);
	};
}

#endif