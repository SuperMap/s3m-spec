#if !defined(SLPKFILE_H__748FF634_5C0D_4604_8BC5_FE86C7074E4C__INCLUDED_)
#define SLPKFILE_H__748FF634_5C0D_4604_8BC5_FE86C7074E4C__INCLUDED_

#pragma once
#include "Common.h"
#include "s3m/include/MemoryStream.h"
#include "stdafx.h"

#include <array>
#include <string>

namespace S3MB
{
	class TraversalMemory
	{
	public:
		TraversalMemory(const void* pBuff, unsigned int nBytes);
		~TraversalMemory();

		// ��ȡ�ڴ��
		bool ReadChunk(unsigned char*& pChunk, unsigned int& nSize, bool& bFinish);

	private:
		const unsigned char* m_pBegBuffer;
		const unsigned char* m_pEndBuffer;
	};

	template< class Vec_t >
	class TraversalVector
	{
	public:
		typedef typename Vec_t::value_type T;
		TraversalVector(Vec_t* vec, unsigned int nSize = 16 * 1024);

		bool ReserveChunk(unsigned char*& pChunk, unsigned int& nSize);

		void Rewind(unsigned int nSize);

	private:
		Vec_t* m_vec;
		size_t m_nChunkSize;
		size_t m_nCurSize;
	};

	class STK_API SlpkHashIndex
	{
	public:
		SlpkHashIndex();
		~SlpkHashIndex();

		bool Load(const std::wstring& strPath);
        bool Load(unsigned char* pBuffer, ulong nBufferSize);

		// �����ļ���ƫ����(·��Ϊѹ�������·��)
		long long FindFile(const std::wstring& strPath);
		// �Ƿ�Ϊ��
		bool IsEmpty() const;
		// �ر�
		bool Close();

	private:
		bool Load(MemoryStream& stream);
		// ��ȡ·���Ĺ�ϣֵ
		std::array<unsigned char, 16> GetHash(const std::wstring& strPath);

	private:
		struct Md5Offset
		{
			std::array<unsigned char, 16> pathKey;
            ulong nOffset;

			Md5Offset() :nOffset(0) {};
            Md5Offset(const std::array<unsigned char, 16>& key, ulong nOff) : pathKey(key), nOffset(nOff) {}
			bool operator<(const Md5Offset& other) const noexcept;
		};
		static_assert(sizeof(Md5Offset) == 24, "Unexpected size");

		std::vector<Md5Offset> m_vecPending;
	};

	// i3s slpk(֧��eslpk����·����slpk��ѹ֮����ļ��е����)
	class STK_API SlpkFile
	{
	public:
		SlpkFile();
		~SlpkFile();

		bool Open(const std::wstring& strPath);
		// ��ȡ�ַ������͵��ļ�����
		bool GetString(const std::wstring& strFileName, std::wstring& strContent);
		// ��ȡ���������͵��ļ�����
        bool GetBuffer(const std::wstring& strFileName, unsigned char*& pBuffer, ulong& nBufferSize);
		// �ر�
		bool Close();

	private:
		// ��slpk/eslpk�л�ȡ�ַ������͵��ļ�����
		bool GetStringFromSlpk(const std::wstring& strFileName, std::wstring& strContent);
		bool GetStringFromESlpk(const std::wstring& strPath, std::wstring& strContent);

		// ��slpk/eslpk�л�ȡ���������͵��ļ�����
        bool GetBufferFromSlpk(const std::wstring& strFileName, unsigned char*& pBuffer, ulong& nBufferSize);
        bool GetBufferFromESlpk(const std::wstring& strPath, unsigned char*& pBuffer, ulong& nBufferSize);

		// ��ȡ�ļ�(�ļ�����Ϊѹ�������·��)
        bool GetFileFromSlpk(const std::wstring& strFileName, unsigned char*& pBuffer, ulong& nBufferSize);

	private:
#pragma pack( push )
#pragma pack( 2 )
		struct SlpkHeader {
			unsigned int nSig;
			unsigned short nVerToExtract;
			unsigned short nGeneralBits;
			unsigned short nCompressionType;
			unsigned short nLastModFileTime;
			unsigned short nLastModFileDate;
			unsigned int nCrc32;
			unsigned int nPackedSize;
			unsigned int nUnpackedSize;
			unsigned short nFnLength;
			unsigned short nExtraLength;

			SlpkHeader();
            bool Load(MemoryStream& stream, ulong nOffset, const std::wstring& strActualPath, ulong& nActualPackedSize);
            bool ParseExtraRecord(const char* pExtraData, int nExtraSize, unsigned int nOffset, unsigned int nPackedSize, ulong& nActualPackedSize);
		};
#pragma pack(pop)
		static_assert(sizeof(SlpkHeader) == 30, "Unexpected size");

		// �Ƿ���slpk�ļ�
		bool m_bIsSlpk;
		// �򿪵�·��
		std::wstring m_strRootPath;
		// ��ϣ������
		SlpkHashIndex m_hashIndex;
		// �ļ���
		MemoryStream m_stream;
	};

	class STK_API SlpkUtils
	{
	public:
		// ��ȡ��׼��·��
		static std::wstring GetNormalizePath(const std::wstring& strPath);
		// ��ȡ����
		static unsigned short ReadUshort(const char* pData);
        static ulong ReadUlong(const char* pData);
		// ���CRC
        static unsigned int CrcBuffer(const char* pData, ulong nSize, unsigned int nCrc = 0xffffffff);
		// ��ѹgzip
		template< class Source, class Des > static bool UncompressGzip(Source* source, Des* dest);
	};
}

#endif
