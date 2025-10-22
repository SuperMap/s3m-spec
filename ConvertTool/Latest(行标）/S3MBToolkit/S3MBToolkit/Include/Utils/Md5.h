#if !defined(MD5_H__D631901E_B176_4024_A8C5_68D9C29BDA26__INCLUDED_)
#define MD5_H__D631901E_B176_4024_A8C5_68D9C29BDA26__INCLUDED_

#pragma once
#include "Common.h"
#include "stdafx.h"

#include <array>
#include <stdint.h>

namespace S3MB
{
	class STK_API Md5
	{
	public:
		Md5();
		Md5(const std::string& str);

		const std::array<unsigned char, 16>& Digest();

	private:
		void Init();
		void Update(const std::string& str);
		void Update(const unsigned char* pData, size_t nLength);
		void ProcessChunk(const unsigned int* pM);
		void ProcessChunk(const unsigned char* pData);
		void Finalize();

	private:
		unsigned int m_states[4];
		uint64_t m_nCount;
		std::array<unsigned int, 16> m_chunkBuffer;
		std::array<unsigned char, 16> m_digest;
		//是否计算完成
		bool m_bFinished;
	};
}

#endif