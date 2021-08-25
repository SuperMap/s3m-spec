#include "S3MBAttributeReader.h"
#include <io.h>

using namespace S3MB;
using namespace std;
void S3MBAttributeReader::ReadAttribute(wstring strAttPath)
{
	if (strAttPath.find(L':') == wstring::npos)
	{
		strAttPath = StringHelper::GetAbsolutePath(strAttPath);
	}
	setlocale(LC_ALL, "zh-CN.UTF-8");
	std::vector<LayerCacheInfo> vecLayerInfos;
	// Read attribute.json file
	S3MBLayerInfos::LoadLayerInfoFromJson(strAttPath, vecLayerInfos);
	std::vector<LayerCacheInfo>::const_iterator itorLayerCaheInfo;
	int nMapFeatureMax = -1;
	int nMapFeatureMin = 9999;
	for (itorLayerCaheInfo = vecLayerInfos.begin(); itorLayerCaheInfo != vecLayerInfos.end(); itorLayerCaheInfo++)
	{
		std::pair<int, int> pairIDRange = itorLayerCaheInfo->m_pairIDRange;
		S3mbFieldInfos s3mbLayerInfo = itorLayerCaheInfo->m_fieldInfos;
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
	std::wstring strS3MRootPath = StringHelper::GetDir(strAttPath);
	vector<std::wstring> fifileNames;
	vector<std::wstring>::iterator iter;
	S3MBAttributeReader::GetFiles(strS3MRootPath, fifileNames);

	// Traverse and read all attribute files
	for (iter = fifileNames.begin(); iter != fifileNames.end(); iter++)
	{
		bool bflag = S3MBLayerInfos::LoadAttributeDataFromFile(*iter, mapFeature);
	}

	// Get specific fieldNames and fieldValues
	for (std::map<unsigned int, Feature*>::iterator iter = mapFeature.begin(); iter != mapFeature.end(); iter++)
	{
		std::wcout << L"=============================" << std::endl;
		Feature* pFeature = iter->second;
		std::wcout << L"FeatureID£º" << pFeature->m_nID << std::endl;
		vector<Feature::FieldDefine>* arrayFieldDefines = &(pFeature->m_fieldDefines);

		int nSize = arrayFieldDefines->size();
		for (int i = 0; i < nSize; i++)
		{
			Feature::FieldDefine ugFileDefine = arrayFieldDefines->at(i);
			wstring strFieldName = ugFileDefine.m_strName;
			S3mbVariant vtValule;
			pFeature->GetValue(strFieldName, vtValule);
			wstring strFieldValue = vtValule.ToString();

			std::wcout << strFieldName << L":" << strFieldValue << std::endl;
		}
		std::wcout << L"=============================" << std::endl;
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
	system("pause");
}

void S3MBAttributeReader::GetFiles(const std::wstring& path, std::vector<std::wstring>& files)
{

	intptr_t hFile = 0;

	struct _wfinddata_t fileinfo;
	std::wstring p;
	if ((hFile = _wfindfirst(p.assign(path).append(L"\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			if (fileinfo.attrib & _A_SUBDIR)
			{
				if (wcscmp(fileinfo.name, L".") != 0 && wcscmp(fileinfo.name, L"..") != 0)
				{
					GetFiles(p.assign(path).append(L"\\").append(fileinfo.name), files);
				}
			}
			else
			{
				wstring s3mattribute = L"s3md";
				if (wcsstr(fileinfo.name, s3mattribute.c_str()))
				{
					files.push_back(p.assign(path).append(L"\\").append(fileinfo.name));
				}
			}
		} while (_wfindnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}
