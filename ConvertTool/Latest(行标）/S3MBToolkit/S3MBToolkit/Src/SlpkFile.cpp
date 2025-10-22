#include "SlpkFile.h"
#include "JsonValue.h"
#include "Utils/Md5.h"
#include "Utils/Utils.h"
#include "Utils/ZipUtil.h"

#include "zlib.h"

#include <assert.h>
#include <fstream>
#include <sstream>

#define I3S_EXT_SLPK U(".slpk")
#define I3S_EXT_JSON U(".json")
#define I3S_SPECIAL_INDEX_FILE U("@specialIndexFileHASH128@")

namespace S3MB
{
	TraversalMemory::TraversalMemory(const void* pBuff, unsigned int nBytes) :
		m_pBegBuffer(reinterpret_cast<const unsigned char*>(pBuff)),
		m_pEndBuffer(reinterpret_cast<const unsigned char*>(pBuff) + nBytes)
	{
	}

	TraversalMemory::~TraversalMemory()
	{
	}

	bool TraversalMemory::ReadChunk(unsigned char*& pChunk, unsigned int& nSize, bool& bFinish)
	{
		if (m_pBegBuffer >= m_pEndBuffer)
		{
			return false;
		}

		pChunk = const_cast<unsigned char*>(m_pBegBuffer);
        nSize = (unsigned int)(m_pEndBuffer - m_pBegBuffer);
		bFinish = true;
		m_pBegBuffer += nSize;
		return true;
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	template< class Vec_t >
	TraversalVector<Vec_t>::TraversalVector(Vec_t* vec, unsigned int nSize) :
		m_vec(vec), m_nChunkSize(nSize / sizeof(T)), m_nCurSize(0)
	{
	}

	template< class Vec_t >
	bool TraversalVector<Vec_t>::ReserveChunk(unsigned char*& pChunk, unsigned int& nSize)
	{
		m_vec->resize(m_vec->size() + m_nChunkSize);
		assert(m_nCurSize < sizeof(T)*m_vec->size());
		pChunk = reinterpret_cast<unsigned char*>(&((*m_vec)[0])) + m_nCurSize;
        nSize = (unsigned int)(m_vec->size() * sizeof(T) - m_nCurSize);
		m_nCurSize += nSize;
		return true;
	}

	template< class Vec_t >
	void TraversalVector<Vec_t>::Rewind(unsigned int nSize)
	{
		m_nCurSize -= nSize;
		assert(nSize % sizeof(T) == 0);
		size_t nElem = nSize / sizeof(T);
		m_vec->resize(m_vec->size() - nElem);
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	SlpkHashIndex::SlpkHashIndex()
	{

	}

	SlpkHashIndex::~SlpkHashIndex()
	{
		Close();
	}

	bool SlpkHashIndex::Load(const std::wstring& strPath)
	{
		if (!StringUtil::IsFileExist(strPath))
		{
			return false;
		}

		unsigned char* pBuffer = nullptr;
        ulong nBufferSize = 0;
		if (!ZipUtil::GetFile(strPath, I3S_SPECIAL_INDEX_FILE, pBuffer, nBufferSize))
		{
			delete[] pBuffer;
			pBuffer = nullptr;
			return false;
		}

		bool bResult = Load(pBuffer, nBufferSize);
		delete[] pBuffer;
		pBuffer = nullptr;

		return bResult;
	}

    bool SlpkHashIndex::Load(unsigned char* pBuffer, ulong nBufferSize)
	{
		if (pBuffer == nullptr || nBufferSize == 0)
		{
			return false;
		}

		MemoryStream stream;
		stream.Init(pBuffer, nBufferSize);
		return Load(stream);
	}

	long long SlpkHashIndex::FindFile(const std::wstring& strPath)
	{
		if (strPath.empty() || m_vecPending.empty())
		{
			return -1;
		}

		std::wstring strNorPath = SlpkUtils::GetNormalizePath(strPath);
		std::array<unsigned char, 16> pathHash = GetHash(strNorPath);
        auto key = Md5Offset(pathHash, ~(ulong)0);
		auto found = std::lower_bound(m_vecPending.begin(), m_vecPending.end(), key);
		return (found == m_vecPending.end() || found->pathKey != pathHash) ? -1 : found->nOffset;
	}

	bool SlpkHashIndex::IsEmpty() const
	{
		return m_vecPending.empty();
	}

	bool SlpkHashIndex::Close()
	{
		m_vecPending.clear();
		return true;
	}

	bool SlpkHashIndex::Load(MemoryStream& stream)
	{
        ulong ulLength = stream.GetLength();
		if (ulLength == 0)
		{
			ulLength = stream.GetLength();
		}

        ulong ulMd5OffsetSize = (ulong)sizeof(Md5Offset);
        ulong ulCount = ulLength / ulMd5OffsetSize;
		if (ulCount == 0)
		{
			return false;
		}

		m_vecPending.clear();
		m_vecPending.resize(ulCount);
		stream.SetReadPosition(0);
		stream.Load(reinterpret_cast<char*>(&m_vecPending[0]), ulCount * sizeof(Md5Offset));

		//排序
		std::sort(m_vecPending.begin(), m_vecPending.end());
		return !m_vecPending.empty();
	}

	std::array<unsigned char, 16> SlpkHashIndex::GetHash(const std::wstring& strPath)
	{
		std::string path = StringUtil::UnicodeToANSI(strPath);
		std::array<unsigned char, 16> digest;
		Md5 hasher(path);
		return hasher.Digest();
	}

	bool SlpkHashIndex::Md5Offset::operator<(const Md5Offset& other) const noexcept
	{
        const ulong* a = reinterpret_cast<const ulong*>(pathKey.data());
        const ulong* b = reinterpret_cast<const ulong*>(other.pathKey.data());
		return a[0] == b[0] ? a[1] < b[1] : a[0] < b[0];
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	SlpkFile::SlpkFile()
	{
		m_bIsSlpk = true;
	}

	SlpkFile::~SlpkFile()
	{
		Close();
	}

	bool SlpkFile::Open(const std::wstring& strPath)
	{
		if (!StringUtil::IsFileExist(strPath))
		{
			return false;
		}

		m_strRootPath = strPath;
		std::wstring strExt = StringUtil::GetExt(strPath);
		m_bIsSlpk = StringUtil::CompareNoCase(strExt, I3S_EXT_SLPK);

		if (m_bIsSlpk)
		{
			unsigned char* pBuffer = nullptr;
            ulong nBufferSize = 0;
			if (!ZipUtil::GetFile(strPath, I3S_SPECIAL_INDEX_FILE, pBuffer, nBufferSize))
			{
				delete[] pBuffer;
				pBuffer = nullptr;
				return false;
			}

			if (!m_hashIndex.Load(pBuffer, nBufferSize))
			{
				delete[] pBuffer;
				pBuffer = nullptr;
				return false;
			}

			delete[] pBuffer;
			pBuffer = nullptr;

			std::string cstrPath = StringUtil::UnicodeToANSI(strPath);
			std::ifstream ifs(cstrPath, ios::in | ios::binary);
			if (!ifs)
			{
				return false;
			}

			ifs.seekg(0, ios::end);
			int size = ifs.tellg();
			ifs.seekg(0, ios::beg);
			char* buffer = new char[size];
			ifs.read(buffer, size);
			ifs.close();

			m_stream.Init(buffer, size, true);
		}

		return true;
	}

	bool SlpkFile::GetString(const std::wstring& strFileName, std::wstring& strContent)
	{
		bool bResult = false;
		if (m_bIsSlpk)
		{
			bResult = GetStringFromSlpk(strFileName, strContent);
		}
		else
		{
			std::wstring strFilePath = m_strRootPath + U("/") + strFileName;
			bResult = GetStringFromESlpk(strFilePath, strContent);
		}
		return bResult;
	}

    bool SlpkFile::GetBuffer(const std::wstring& strFileName, unsigned char*& pBuffer, ulong& nBufferSize)
	{
		bool bResult = false;
		if (m_bIsSlpk)
		{
			bResult = GetBufferFromSlpk(strFileName, pBuffer, nBufferSize);
		}
		else
		{
			std::wstring strFilePath = m_strRootPath + U("/") + strFileName;
			bResult = GetBufferFromESlpk(strFilePath, pBuffer, nBufferSize);
		}
		return bResult;
	}

	bool SlpkFile::Close()
	{
		m_bIsSlpk = true;
		m_strRootPath.clear();
		m_hashIndex.Close();
		m_stream.Close();
		return true;
	}

	bool SlpkFile::GetStringFromSlpk(const std::wstring& strFileName, std::wstring& strContent)
	{
		unsigned char* pBuffer = nullptr;
        ulong nBufferSize = 0;
		if (!GetFileFromSlpk(strFileName, pBuffer, nBufferSize))
		{
			delete[] pBuffer;
			pBuffer = nullptr;
			return false;
		}

		bool bResult = false;
		std::string content;
		std::wstring strExt = StringUtil::GetExt(strFileName);
		if (StringUtil::CompareNoCase(strExt, FILE_EXT_GZ))
		{
			TraversalMemory src(pBuffer, nBufferSize);
			TraversalVector<std::string> des(&content);
			bResult = SlpkUtils::UncompressGzip(&src, &des);
		}
		else if (StringUtil::CompareNoCase(strExt, I3S_EXT_JSON))
		{
			content = std::string(reinterpret_cast<const char*>(pBuffer), nBufferSize);
			bResult = true;
		}
		else
		{
			assert(false);
		}

		strContent = StringUtil::UTF8_to_UNICODE(content);

		delete[] pBuffer;
		pBuffer = nullptr;
		return bResult;
	}

	bool SlpkFile::GetStringFromESlpk(const std::wstring& strPath, std::wstring& strContent)
	{
		if (!StringUtil::IsFileExist(strPath))
		{
			return false;
		}

		bool bResult = false;
		std::wstring strExt = StringUtil::GetExt(strPath);
		if (StringUtil::CompareNoCase(strExt, FILE_EXT_GZ))
		{
			std::string path = StringUtil::UnicodeToANSI(strPath);
			gzFile file = gzopen(path.c_str(), "rb");
			if (!file)
			{
				assert(false);
				return false;
			}
			char buffer[1024];
			std::ostringstream oss;
			while (true)
			{
				int nByteRead = gzread(file, buffer, sizeof(buffer));
				if (nByteRead <= 0)
				{
					break;
				}
				oss << std::string(buffer, nByteRead);
			}
			gzclose(file);
			std::string content = oss.str();
			strContent = StringUtil::UTF8_to_UNICODE(content);
			bResult = true;
		}
		else if (StringUtil::CompareNoCase(strExt, I3S_EXT_JSON))
		{
			JsonValue jsonFile;
			if (!jsonFile.LoadFromFile(strPath))
			{
				return false;
			}

			jsonFile.SaveToString(strContent);
			bResult = true;
		}
		else
		{
			assert(false);
		}

		return bResult;
	}

    bool SlpkFile::GetBufferFromSlpk(const std::wstring& strFileName, unsigned char*& pBuffer, ulong& nBufferSize)
	{
		unsigned char* pOriBuffer = nullptr;
        ulong nOriBufferSize = 0;
		if (!GetFileFromSlpk(strFileName, pOriBuffer, nOriBufferSize))
		{
			delete[] pOriBuffer;
			pOriBuffer = nullptr;
            return false;
		}

		bool bResult = false;
		std::wstring strExt = StringUtil::GetExt(strFileName);
		if (StringUtil::CompareNoCase(strExt, FILE_EXT_GZ))
		{
			std::string content;
			TraversalMemory src(pOriBuffer, nOriBufferSize);
			TraversalVector<std::string> des(&content);
			if (SlpkUtils::UncompressGzip(&src, &des) && !content.empty())
			{
				nBufferSize = content.size();
				delete[] pBuffer;
				pBuffer = new unsigned char[nBufferSize];
				memcpy(pBuffer, (unsigned char*)content.data(), nBufferSize);
				bResult = true;
			}

			delete[] pOriBuffer;
			pOriBuffer = nullptr;
		}
		else
		{
			nBufferSize = nOriBufferSize;
			delete[] pBuffer;
			pBuffer = pOriBuffer;
			bResult = true;
		}

		return bResult;
	}

    bool SlpkFile::GetBufferFromESlpk(const std::wstring& strPath, unsigned char*& pBuffer, ulong& nBufferSize)
	{
		return FileUtils::LoadBuffer(strPath, pBuffer, nBufferSize);
	}

    bool SlpkFile::GetFileFromSlpk(const std::wstring& strFileName, unsigned char*& pBuffer, ulong& nBufferSize)
	{
		if (m_hashIndex.IsEmpty() || m_stream.GetLength() == 0)
		{
			return false;
		}

		bool bFound = false;
		long long nOffset = m_hashIndex.FindFile(strFileName);
		if (nOffset >= 0)
		{
			//读取文件头
			SlpkHeader header;
            ulong nActualPackedSize = 0;
			if (header.Load(m_stream, nOffset, strFileName, nActualPackedSize) && (header.nPackedSize == header.nUnpackedSize))
			{
				//读取文件内容
				nBufferSize = nActualPackedSize;
				pBuffer = new unsigned char[nBufferSize];
				m_stream.Load(pBuffer, nBufferSize);
				auto nActualCrc = ~(SlpkUtils::CrcBuffer((char*)pBuffer, nBufferSize));
				bFound = nActualCrc == header.nCrc32 || header.nCrc32 == 0;
			}
		}
		return bFound;
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	SlpkFile::SlpkHeader::SlpkHeader() :
		nSig(0x04034b50),
		nVerToExtract(45),
		nGeneralBits(0),
		nCompressionType(0),
		nLastModFileTime(0),
		nLastModFileDate(0),
		nCrc32(0),
		nPackedSize(0),
		nUnpackedSize(0),
		nFnLength(0),
		nExtraLength(0)
	{

	}

    bool SlpkFile::SlpkHeader::Load(MemoryStream& stream, ulong nOffset, const std::wstring& strActualPath, ulong& nActualPackedSize)
	{
		stream.SetReadPosition(nOffset);
		stream.Load(reinterpret_cast<char*>(this), sizeof(SlpkHeader));
		if (nFnLength > 2048 || nFnLength == 0)
		{
			return false;
		}

		std::string path;
		path.resize(nFnLength);
		stream.Load((char*)path.data(), path.size());

		std::wstring strPath = StringUtil::UTF8_to_UNICODE(path);
		strPath = SlpkUtils::GetNormalizePath(strPath);
		if (!StringUtil::CompareNoCase(strActualPath, strPath))
		{
			return false;
		}

		nActualPackedSize = nPackedSize;
		if (nPackedSize == 0xFFFFFFFF)
		{
			std::vector<char> extraBuffer(nExtraLength);
			stream.Load(extraBuffer.data(), extraBuffer.size());
			ParseExtraRecord(extraBuffer.data(), (int)extraBuffer.size(), 0, nPackedSize, nActualPackedSize);
		}
		else if (nExtraLength)
		{
            ulong nPos = stream.GetReadPosition();
			stream.SetReadPosition(nPos + nExtraLength);
		}

		return true;
	}

    bool SlpkFile::SlpkHeader::ParseExtraRecord(const char* pExtraData, int nExtraSize, unsigned int nOffset, unsigned int nPackedSize, ulong& nActualPackedSize)
	{
		const unsigned int nOverflow = 0xFFFFFFFF;
		const int nBlockHeaderSize = 4;
		const int nZipBlockType = 1;

		nActualPackedSize = nPackedSize;
		if (nOffset != nOverflow && nPackedSize != nOverflow)
		{
			return true;
		}

		int nIter = 0;
		while (nIter < nExtraSize - nBlockHeaderSize)
		{
			unsigned short nBlockType = SlpkUtils::ReadUshort(&pExtraData[nIter]);
			nIter += 2;
			unsigned short nBlockSize = SlpkUtils::ReadUshort(&pExtraData[nIter]);
			nIter += 2;
			if (nBlockType == nZipBlockType)
			{
				if (nPackedSize == nOverflow)
				{
					if (nBlockSize < 16)
					{
						return false;
					}

                    ulong nUncompressedSize = SlpkUtils::ReadUlong(&pExtraData[nIter]);
					nIter += 8;
                    ulong nCompressedSize = SlpkUtils::ReadUlong(&pExtraData[nIter]);
					nIter += 8;

					if (nUncompressedSize != nCompressedSize)
					{
						return false;
					}

					nActualPackedSize = nCompressedSize;
				}

				if (nOffset == nOverflow)
				{
					if (nBlockSize < 8)
					{
						return false;
					}

					SlpkUtils::ReadUlong(&pExtraData[nIter]);
				}
				return true;
			}
			else
			{
				nIter += nBlockSize;
			}
		}
		return false;
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	std::wstring SlpkUtils::GetNormalizePath(const std::wstring& strPath)
	{
		std::wstring strNorPath = strPath;
		std::wstring strOld = U("\\");
		std::wstring strNew = U("/");
		StringUtil::Replace(strNorPath, strOld, strNew);
		StringUtil::TrimLeftString(strNorPath, strNew);
		return strNorPath;
	}

	unsigned short SlpkUtils::ReadUshort(const char* pData)
	{
		return *reinterpret_cast<const unsigned short*>(pData);
	}

    ulong SlpkUtils::ReadUlong(const char* pData)
	{
        return *reinterpret_cast<const ulong*>(pData);
	}

    unsigned int SlpkUtils::CrcBuffer(const char* pData, ulong nSize, unsigned int nCrc)
	{
		/* CRC polynomial 0xedb88320 */
		static const unsigned int crcTable[256] =
		{
		  0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
		  0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
		  0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
		  0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
		  0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
		  0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
		  0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
		  0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
		  0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
		  0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
		  0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
		  0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
		  0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
		  0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
		  0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
		  0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
		  0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
		  0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
		  0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
		  0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
		  0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
		  0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
		  0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
		  0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
		  0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
		  0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
		  0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
		  0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
		  0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
		  0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
		  0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
		  0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d
		};

		for (; nSize != 0; --nSize, ++pData)
		{
			nCrc = crcTable[(nCrc ^ *pData) & 0xff] ^ (nCrc >> 8);
		}

		return nCrc;
	}

	template< class Source, class Des > bool SlpkUtils::UncompressGzip(Source* source, Des* dest)
	{
		int ret;
		z_stream strm;
		strm.zalloc = Z_NULL;
		strm.zfree = Z_NULL;
		strm.opaque = Z_NULL;
		strm.avail_in = 0;
		strm.next_in = Z_NULL;
		ret = inflateInit2(&strm, 16 + MAX_WBITS);
		if (ret != Z_OK)
		{
			return false;
		}

		bool bFinish;
		while (source->ReadChunk(strm.next_in, strm.avail_in, bFinish))
		{
			do
			{
				dest->ReserveChunk(strm.next_out, strm.avail_out);
				ret = inflate(&strm, Z_NO_FLUSH);
				switch (ret) {
				case Z_NEED_DICT:
					ret = Z_DATA_ERROR;
				case Z_DATA_ERROR:
				case Z_MEM_ERROR:
					(void)inflateEnd(&strm);
					return false;
				}
			} while (strm.avail_out == 0);

			dest->Rewind(strm.avail_out);
			if ((ret == Z_STREAM_END) != bFinish)
			{
				return false;
			}
		}

		(void)inflateEnd(&strm);
		return ret == Z_STREAM_END;
	}
}
