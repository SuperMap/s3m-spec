#ifdef WIN32
#include "S3MBAttributeReader.h"
#include "S3MBUtils.h"
#include "Variant.h"
#include <fstream>
#ifdef WIN32
#include <io.h>
#else
#include <sys/io.h>
#endif
#include <iostream>

void S3MBAttributeReader::ReadAttribute(wstring strAttPath)
{
	if (strAttPath.find(L':') == wstring::npos)
	{
		strAttPath = StringUtil::GetAbsolutePath(strAttPath);
	}

	std::vector<LayerCacheInfo> vecLayerInfos;
	// Read attribute.json file
	{
		std::ifstream ifs(strAttPath, ios::in | ios::binary);
		ifs.seekg(0, ios::end);
		int size = ifs.tellg();
		ifs.seekg(0, ios::beg);
		char* buffer = new char[size];
		ifs.read(buffer, size);
		ifs.close();
		S3MBLayerInfos::ReadLayerFieldInfosFromStream(buffer,size, vecLayerInfos);
	}
	std::vector<LayerCacheInfo>::const_iterator itorLayerCaheInfo;
	int nMapFeatureMax = -1;
	int nMapFeatureMin = 9999;
	for (itorLayerCaheInfo = vecLayerInfos.begin(); itorLayerCaheInfo != vecLayerInfos.end(); itorLayerCaheInfo++)
	{
		std::pair<int, int> pairIDRange = itorLayerCaheInfo->m_pairIDRange;
		S3MBFieldInfos s3mbLayerInfo = itorLayerCaheInfo->m_fieldInfos;
		if (pairIDRange.second > nMapFeatureMax)
		{
			nMapFeatureMax = pairIDRange.second;
		}
		if (pairIDRange.first < nMapFeatureMin)
		{
			nMapFeatureMin = pairIDRange.first;
		}
	}
	std::map<unsigned int, Feature*> mapFeature;
	for (int i = nMapFeatureMin; i <= nMapFeatureMax; i++)
	{
		mapFeature[i] = NULL;
	}
	std::wstring strS3MRootPath = StringUtil::GetDir(strAttPath);
	vector<std::wstring> fifileNames;
	vector<std::wstring>::iterator iter;
	S3MBAttributeReader::GetFiles(strS3MRootPath, fifileNames);

	// Traverse and read all attribute files
	for (iter = fifileNames.begin(); iter != fifileNames.end(); iter++)
	{
		std::ifstream ifs(*iter, ios::in | ios::binary);
		ifs.seekg(0,ios::end);
		int len = ifs.tellg();
		ifs.seekg(0,ios::beg);
		char* buffer = new char[len];
		ifs.read(buffer, len);
		ifs.close();
		bool bflag = S3MBLayerInfos::ReadLayerAttributeValueFromStream(buffer,len, mapFeature);
		delete buffer;
		buffer = nullptr;
	}

	// Get specific fieldNames and fieldValues
	for (std::map<unsigned int, Feature*>::iterator iter = mapFeature.begin(); iter != mapFeature.end(); iter++)
	{
		std::wcout << U("=============================") << std::endl;
		Feature* pFeature = iter->second;
		std::wcout << U("FeatureID£º") << pFeature->m_nID << std::endl;
		vector<FieldDefine>* arrayFieldDefines = &(pFeature->m_fieldDefines);

		int nSize = arrayFieldDefines->size();
		for (int i = 0; i < nSize; i++)
		{
			FieldDefine ugFileDefine = arrayFieldDefines->at(i);
			wstring strFieldName = ugFileDefine.m_strName;
			Variant vtValule;
			pFeature->GetValue(strFieldName, vtValule);
			wstring strFieldValue = vtValule.ToString();

			std::wcout << strFieldName << U(":") << strFieldValue << std::endl;
		}
		std::wcout << U("=============================" )<< std::endl;
	}
	// Release all feature
	for (std::map<unsigned int, Feature*>::iterator iter = mapFeature.begin(); iter != mapFeature.end(); iter++)
	{
		if (iter->second != nullptr)
		{
			delete iter->second;
			iter->second = nullptr;
		}
	}
	mapFeature.clear();
}

void S3MBAttributeReader::GetFiles(const std::wstring& path, std::vector<std::wstring>& files)
{

	intptr_t hFile = 0;

	struct _wfinddata_t fileinfo;
	std::wstring p;
	if ((hFile = _wfindfirst(p.assign(path).append(U("\\*")).c_str(), &fileinfo)) != -1)
	{
		do
		{
			if (fileinfo.attrib & _A_SUBDIR)
			{
				if (wcscmp(fileinfo.name, U(".")) != 0 && wcscmp(fileinfo.name, U("..")) != 0)
				{
					GetFiles(p.assign(path).append(U("\\")).append(fileinfo.name), files);
				}
			}
			else
			{
				wstring s3mattribute = U("s3md");
				if (wcsstr(fileinfo.name, s3mattribute.c_str()))
				{
					files.push_back(p.assign(path).append(U("\\")).append(fileinfo.name));
				}
			}
		} while (_wfindnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
#endif
