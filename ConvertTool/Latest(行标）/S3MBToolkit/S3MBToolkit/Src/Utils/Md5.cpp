#include "Utils/Md5.h"
#include "Utils/EndianUtil.h"
#include <cstring>

namespace S3MB
{
	template<typename T>
	bool IsAligned(const void* ptr) noexcept
	{
		return (reinterpret_cast<uintptr_t>(ptr) % alignof(T)) == 0;
	}

	unsigned int LeftRotate(unsigned int x, int n)
	{
		return (x << n) | (x >> (32 - n));
	}

	const unsigned int s[] =
	{
	  7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,   // 0..15
	  5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,   // 16..31
	  4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,   // 32..47
	  6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21    // 48..63
	};

	const unsigned int k[] =
	{
	  0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, // 0..3
	  0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, // 4..7
	  0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, // 8..11
	  0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, // 12..15
	  0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, // 16..19
	  0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, // 20..23
	  0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, // 24..27
	  0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, // 28..31
	  0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, // 32..35
	  0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, // 36..39
	  0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, // 40..43
	  0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, // 44..47
	  0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, // 48..51
	  0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, // 52..55
	  0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, // 56..59
	  0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391  // 60..63
	};

	Md5::Md5()
	{
		Init();
	}

	Md5::Md5(const std::string& str)
	{
		Init();
		Update(str);
	}

	const std::array<unsigned char, 16>& Md5::Digest()
	{
		if (!m_bFinished)
		{
			Finalize();
			m_bFinished = true;
		}
		return m_digest;
	}

	void Md5::Init()
	{
		m_states[0] = 0x67452301; //A
		m_states[1] = 0xefcdab89; //B
		m_states[2] = 0x98badcfe; //C
		m_states[3] = 0x10325476; //D
		m_nCount = 0;
		m_bFinished = false;
		std::fill(std::begin(m_chunkBuffer), std::end(m_chunkBuffer), 0);
	}

	void Md5::Update(const std::string& str)
	{
		Update((const unsigned char*)str.c_str(), str.length());
	}

	void Md5::Update(const unsigned char* pData, size_t nLength)
	{
		unsigned int nPos = m_nCount & 0x3f;
		const unsigned int nLeft = 64 - nPos;

		m_nCount += static_cast<uint64_t>(nLength);

		if (nPos > 0 && nLeft <= nLength)
		{
            std::memcpy(reinterpret_cast<unsigned char*>(m_chunkBuffer.data()) + nPos, pData, nLeft);
			ProcessChunk(m_chunkBuffer.data());
			pData += nLeft;
			nLength -= nLeft;
			nPos = 0;
		}

		if (IsAligned<unsigned int>(pData))
		{
			for (; nLength >= 64; pData += 64, nLength -= 64)
				ProcessChunk(reinterpret_cast<const unsigned int*>(pData));
		}
		else
		{
			for (; nLength >= 64; pData += 64, nLength -= 64)
				ProcessChunk(pData);
		}

		if (nLength > 0)
            std::memcpy(reinterpret_cast<unsigned char*>(m_chunkBuffer.data()) + nPos, pData, nLength);
	}

	void Md5::ProcessChunk(const unsigned int* pM)
	{
		unsigned int a = m_states[0];
		unsigned int b = m_states[1];
		unsigned int c = m_states[2];
		unsigned int d = m_states[3];
		int i = 0;

		const auto rotate = [pM, &i, &a, &b, &c, &d](unsigned int f, unsigned int g)
		{
			const auto t = d;
			d = c;
			c = b;
			b += LeftRotate(a + f + k[i] + pM[g], s[i]);
			a = t;
		};

		for (; i < 16; ++i)
			rotate((b & c) | ((~b) & d), i);

		for (; i < 32; ++i)
			rotate((d & b) | ((~d) & c), (i * 5 + 1) & 0x0f);

		for (; i < 48; ++i)
			rotate(b ^ c ^ d, (i * 3 + 5) & 0x0f);

		for (; i < 64; ++i)
			rotate(c ^ (b | (~d)), (i * 7) & 0x0f);

		m_states[0] += a;
		m_states[1] += b;
		m_states[2] += c;
		m_states[3] += d;
	}

	void Md5::ProcessChunk(const unsigned char* pData)
	{
        std::memcpy(m_chunkBuffer.data(), pData, 4);
		ProcessChunk(m_chunkBuffer.data());
	}

	void Md5::Finalize()
	{
		constexpr unsigned char cClosingBit = 0x80;
		unsigned int nPos = m_nCount & 0x3f;
		unsigned int nLeft = 64 - nPos;
		reinterpret_cast<unsigned char*>(m_chunkBuffer.data())[nPos++] = cClosingBit;
		nLeft--;

		if (nLeft < 8)
		{
            std::memset(reinterpret_cast<unsigned char*>(m_chunkBuffer.data()) + nPos, 0, nLeft);
			ProcessChunk(m_chunkBuffer.data());
			nPos = 0;
			nLeft = 64;
		}

		uint64_t nMessageBits = m_nCount << 3;
        std::memset(reinterpret_cast<unsigned char*>(m_chunkBuffer.data()) + nPos, 0, nLeft - 8);
		boost::endian::native_to_little_inplace(nMessageBits);
        std::memcpy(m_chunkBuffer.data() + 14, &nMessageBits, 8);

		ProcessChunk(m_chunkBuffer.data());

		unsigned int *dig = reinterpret_cast<unsigned int*>(m_digest.data());
		dig[0] = m_states[0];
		dig[1] = m_states[1];
		dig[2] = m_states[2];
		dig[3] = m_states[3];
	}
}
