#if !defined(ZIPUTIL_H__A923BA60_3F82_405D_B543_AC4FE32D6ED8__INCLUDED_)
#define ZIPUTIL_H__A923BA60_3F82_405D_B543_AC4FE32D6ED8__INCLUDED_

#pragma once
#include "Common.h"
#include "stdafx.h"

#include <string>

namespace S3MB
{
	class STK_API ZipUtil
	{
	public:
		static bool GetFile(const std::wstring& strZipFilePath, const std::wstring& strFileName,
			unsigned char*& pBuffer, unsigned long long& nBufferSize);
	};
}

#endif