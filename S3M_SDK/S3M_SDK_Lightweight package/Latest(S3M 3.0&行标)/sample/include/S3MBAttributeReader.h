#ifdef WIN32
#if !defined(S3MBATTRIBUTEREADER_H__C58327B6_1769_4C41_AC77_2AD19EA034FD__INCLUDED_)
#define S3MBATTRIBUTEREADER_H__C58327B6_1769_4C41_AC77_2AD19EA034FD__INCLUDED_

#pragma once
#include "S3MBLayerInfos.h"

using namespace S3MB;
class S3MBAttributeReader
{
public:
	// Read attribute sample
	static void ReadAttribute(std::wstring strAttPath);
	// Get all .s3md files
	static void GetFiles(const std::wstring& path, std::vector<std::wstring>& files);
};

#endif
#endif
