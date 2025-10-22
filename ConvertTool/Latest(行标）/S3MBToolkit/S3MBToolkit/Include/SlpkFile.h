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

		// 读取内存块
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

		// 查找文件的偏移量(路径为压缩包里的路径)
		long long FindFile(const std::wstring& strPath);
		// 是否为空
		bool IsEmpty() const;
		// 关闭
		bool Close();

	private:
		bool Load(MemoryStream& stream);
		// 获取路径的哈希值
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

	// i3s slpk(支持eslpk，即路径是slpk解压之后的文件夹的情况)
	class STK_API SlpkFile
	{
	public:
		SlpkFile();
		~SlpkFile();

		bool Open(const std::wstring& strPath);
		// 获取字符串类型的文件内容
		bool GetString(const std::wstring& strFileName, std::wstring& strContent);
		// 获取二进制类型的文件内容
        bool GetBuffer(const std::wstring& strFileName, unsigned char*& pBuffer, ulong& nBufferSize);
		// 关闭
		bool Close();

	private:
		// 从slpk/eslpk中获取字符串类型的文件内容
		bool GetStringFromSlpk(const std::wstring& strFileName, std::wstring& strContent);
		bool GetStringFromESlpk(const std::wstring& strPath, std::wstring& strContent);

		// 从slpk/eslpk中获取二进制类型的文件内容
        bool GetBufferFromSlpk(const std::wstring& strFileName, unsigned char*& pBuffer, ulong& nBufferSize);
        bool GetBufferFromESlpk(const std::wstring& strPath, unsigned char*& pBuffer, ulong& nBufferSize);

		// 获取文件(文件名称为压缩包里的路径)
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

		// 是否是slpk文件
		bool m_bIsSlpk;
		// 打开的路径
		std::wstring m_strRootPath;
		// 哈希索引表
		SlpkHashIndex m_hashIndex;
		// 文件流
		MemoryStream m_stream;
	};

	class STK_API SlpkUtils
	{
	public:
		// 获取标准化路径
		static std::wstring GetNormalizePath(const std::wstring& strPath);
		// 读取数据
		static unsigned short ReadUshort(const char* pData);
        static ulong ReadUlong(const char* pData);
		// 检查CRC
        static unsigned int CrcBuffer(const char* pData, ulong nSize, unsigned int nCrc = 0xffffffff);
		// 解压gzip
		template< class Source, class Des > static bool UncompressGzip(Source* source, Des* dest);
	};
}

#endif
