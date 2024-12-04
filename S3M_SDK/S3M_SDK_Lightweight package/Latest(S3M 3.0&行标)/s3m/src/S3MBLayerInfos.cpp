#include "S3MBLayerInfos.h"
#include "MemoryStream.h"
#include "S3MBUtils.h"
#include <fstream>
#include <assert.h>
#include <string.h>

namespace S3MB
{
	S3MBFieldInfo::S3MBFieldInfo()
	{
	}

	S3MBFieldInfo::~S3MBFieldInfo()
	{
	}

	S3MBFieldInfos::S3MBFieldInfos()
	{
	}

	S3MBFieldInfos::~S3MBFieldInfos()
	{
		m_array.clear();
		if (m_array.capacity() > m_array.size()) {
			std::vector<S3MBFieldInfo> vctTemp(m_array.begin(), m_array.end());
			m_array.swap(vctTemp);
		}
	}

	void S3MBFieldInfos::add(S3MBFieldInfo newFieldInfo)
	{
		m_array.push_back(newFieldInfo);
	}

	size_t S3MBFieldInfos::GetSize()
	{
		return m_array.size();
	}

	S3MBFieldInfo S3MBFieldInfos::GetAt(int nIndex)
	{
		assert(nIndex >= 0 && m_array.size() > nIndex);
		return m_array.at(nIndex);
	}

	void S3MBFieldInfos::RemoveAll()
	{
		m_array.clear();
	}

	FieldType S3MBLayerInfos::GetFieldInfoType(wstring strType)
	{
		FieldType nType = FT_UnKnown;

		if (strType == S3MB_JSON_FIELD_ATT_TYPE_BOOL)
		{
			nType = FT_Boolean;
		}
		else if (strType == S3MB_JSON_FIELD_ATT_TYPE_INT16)
		{
			nType = FT_INT16;
		}
		else if (strType == S3MB_JSON_FIELD_ATT_TYPE_INT32)
		{
			nType = FT_INT32;
		}
		else if (strType == S3MB_JSON_FIELD_ATT_TYPE_INT64)
		{
			nType = FT_INT64;
		}
		else if (strType == S3MB_JSON_FIELD_ATT_TYPE_FLOAT)
		{
			nType = FT_Float;
		}
		else if (strType == S3MB_JSON_FIELD_ATT_TYPE_DOUBLE)
		{
			nType = FT_Double;
		}
		else if (strType == S3MB_JSON_FIELD_ATT_TYPE_STRING)
		{
			nType = FT_Text;
		}
		else if (strType == S3MB_JSON_FIELD_ATT_TYPE_DATE)
		{
			nType = FT_Date;
		}
		else if (strType == S3MB_JSON_FIELD_ATT_TYPE_TIME)
		{
			nType = FT_Time;
		}
		else if (strType == S3MB_JSON_FIELD_ATT_TYPE_TIMESTAMP)
		{
			nType = FT_TimeStamp;
		}

		return nType;
	}

	S3MBLayerInfos::~S3MBLayerInfos()
	{
		for(std::map<unsigned int, Feature*>::iterator itor = m_mapFeature.begin();itor != m_mapFeature.end();itor++)
		{
			if (itor->second != nullptr)
			{
				delete itor->second;
				itor->second = nullptr;
			}
		}
		m_mapFeature.clear();
		m_vecLayerInfos.clear();
	}

	bool S3MBLayerInfos::ReadLayerAttributeValueFromFile(const wstring& strAttFilePath)
	{
		// 读取文件并解压
#ifdef WIN32
        std::ifstream ifs(strAttFilePath, ios::in | ios::binary);
#else
        string AttFilePath = StringUtil::UnicodeToANSI(strAttFilePath);
        std::ifstream ifs(AttFilePath, ios::in | ios::binary);
#endif
		if (!ifs)
		{
			return false;
		}
		unsigned int nUnZipSize = 0;
		unsigned int nZipSize = 0;
		ifs.read((char*)&nUnZipSize, sizeof(unsigned int));
		ifs.read((char*)&nZipSize, sizeof(unsigned int));
		unsigned char* pByteUnZip = new unsigned char[nUnZipSize];
		unsigned char* pByteZip = new unsigned char[nZipSize];
		ifs.read((char*)pByteZip, nZipSize);
		ifs.close();
#pragma region 解压
		bool bResult = Utils::UnZip(pByteUnZip, nUnZipSize, pByteZip, nZipSize);
		delete[]pByteZip;
		pByteZip = NULL;
#pragma endregion
		MemoryStream streamUnZipped;
		streamUnZipped.Init(pByteUnZip, nUnZipSize, true);
		pByteUnZip = NULL;

		wstring strJsonInfo;
		streamUnZipped >> strJsonInfo;

		JsonValue* pJsonValue = new JsonValue();
		if (!pJsonValue->LoadValueFromString(strJsonInfo))
		{
			return false;
		}

		AttributeDataFromJson(m_mapFeature, *pJsonValue);
		return true;
	}

	const std::map<unsigned int, Feature*>& S3MBLayerInfos::GetLayerAttributeValue()
	{
		return m_mapFeature;
	}

	bool S3MBLayerInfos::ReadLayerAttributeValueFromStream(void* buffer, int bufferSize, std::map<unsigned int, Feature*>& mapFeature)
	{
		MemoryStream streamS3md;
		streamS3md.Init(buffer, bufferSize);
		unsigned int nUnZipSize = 0;
		unsigned int nZipSize = 0;
		streamS3md >> nUnZipSize;
		streamS3md >> nZipSize;
		unsigned char* pByteUnZip = new unsigned char[nUnZipSize];
		unsigned char* pByteZip = new unsigned char[nZipSize];
		streamS3md.Load(pByteZip, nZipSize);

#pragma region 解压
		bool bResult = Utils::UnZip(pByteUnZip, nUnZipSize, pByteZip, nZipSize);
		delete[]pByteZip;
		pByteZip = NULL;
#pragma endregion
		MemoryStream streamUnZipped;
		streamUnZipped.Init(pByteUnZip, nUnZipSize, true);
		pByteUnZip = NULL;

		wstring strJsonInfo;
		streamUnZipped >> strJsonInfo;

		JsonValue* pJsonValue = new JsonValue();
		if (!pJsonValue->LoadValueFromString(strJsonInfo))
		{
			return false;
		}

		AttributeDataFromJson(mapFeature, *pJsonValue);
		return true;
	}

	bool S3MBLayerInfos::AttributeDataFromJson(std::map<unsigned int, Feature*>& mapFeature, JsonValue& jsonValue)
	{
		bool bValid = false;
		JsonValue* pJsonLayers = NULL;
		if (jsonValue.GetValue(S3MB_JSON_LAYERINFOS, pJsonLayers))
		{
			JsonValue pJsonLayer(JsonValueType::objectValue);
			int nLayerSize = pJsonLayers->GetArraySize();
			for (int i = 0; i < nLayerSize; i++)
			{
				if (pJsonLayers->GetAt(i, &pJsonLayer))
				{
					int nMin = 0, nMax = 0;
					GetIDRangeFromJson(pJsonLayer, nMin, nMax);
					S3MBFieldInfos fieldInfos;
					GetFieldInfosFromJson(fieldInfos, pJsonLayer);
					if (fieldInfos.GetSize() == 0)
					{
						continue;
					}
					JsonValue* pRecods = NULL;
					if (pJsonLayer.GetValue(S3MB_JSON_ATT_RECORDS, pRecods))
					{
						JsonValue pRecod(JsonValueType::objectValue);
						int nRecords = pRecods->GetArraySize();
						for (int j = 0; j < nRecords; j++)
						{
							if (pRecods->GetAt(j, &pRecod))
							{
								int nID = 0;
								pRecod.GetValue(S3MB_JSON_ATT_ATT_ID, nID);
								if (mapFeature.find(nID) == mapFeature.end())
								{
									mapFeature[nID] = NULL;
								}
								if (mapFeature[nID] != NULL)
								{
									continue;
								}

								Feature* pFeature = new Feature();
								pFeature->SetFieldInfos(fieldInfos);
								pFeature->m_fieldValues.resize(fieldInfos.GetSize());
								pFeature->m_nID = nID;

								GetFeatureValuesFromJson(pFeature, pRecod);

								mapFeature[nID] = pFeature;
								bValid = true;
							}
						}
					}
					if (pRecods != NULL)
					{
						delete pRecods;
						pRecods = NULL;
					}
				}
			}
		}
		if (pJsonLayers != NULL)
		{
			delete pJsonLayers;
			pJsonLayers = NULL;
		}

		return bValid;
	}

	bool S3MBLayerInfos::GetFeatureValuesFromJson(Feature* pFeature, JsonValue& jsonValue)
	{
		JsonValue* pFeatureValues = NULL;
		if (!jsonValue.GetValue(S3MB_JSON_ATT_VALUES, pFeatureValues))
		{
			if (pFeatureValues != NULL)
			{
				delete pFeatureValues;
				pFeatureValues = NULL;
			}
			return false;
		}

		int iField = 0;
		JsonValue pFeatureValue(JsonValueType::objectValue);
		int nFeatureSize = pFeatureValues->GetArraySize();
		for (int i = 0; i < nFeatureSize; i++)
		{
			if (!pFeatureValues->GetAt(i, &pFeatureValue))
			{
				continue;
			}

			if (iField >= pFeature->m_fieldDefines.size())
			{
				// 前面的字段定义和后面的值个数不一致了
				assert(false);
				break;
			}
			wstring strFieldName;
			pFeatureValue.GetValue(S3MB_JSON_ATT_ATT_NAME, strFieldName);
			for (unsigned int i = iField; i < pFeature->m_fieldDefines.size(); i++)
			{
				if (StringUtil::CompareNoCase(pFeature->m_fieldDefines[i].m_strName, strFieldName))
				{
					iField = i;
					break;
				}
			}

			if (iField > pFeature->m_fieldDefines.size())
			{
				// 前面的字段定义和后面的字段名不一致了
				assert(false);
				break;
			}
			wstring strValueW;
			pFeatureValue.GetValue(S3MB_JSON_ATT_FIELD, strValueW);
#ifdef WIN32
            string strValue = StringUtil::UNICODE_to_UTF8(strValueW);
#else
            string strValue = StringUtil::UnicodeToANSI(strValueW);
#endif

			unsigned char* pByte = NULL;
			switch (pFeature->m_fieldDefines[iField].m_nType)
			{
			case FT_Boolean:
			{
				bool bTemp = (strValue == "1") ? true : false;
				pByte = new unsigned char[sizeof(bTemp)];
				memcpy(pByte, &bTemp, sizeof(bTemp));
			}
			break;
			case FT_INT16:
			{
				short sTemp = atoi(strValue.c_str());
				pByte = new unsigned char[sizeof(sTemp)];
				memcpy(pByte, &sTemp, sizeof(sTemp));
			}
			break;
			case FT_INT32:
			{
				int  iTemp = atoi(strValue.c_str());
				pByte = new unsigned char[sizeof(iTemp)];
				memcpy(pByte, &iTemp, sizeof(iTemp));
			}
			break;
			case FT_INT64:
			{
				long lTemp = atol(strValue.c_str());
				pByte = new unsigned char[sizeof(lTemp)];
				memcpy(pByte, &lTemp, sizeof(lTemp));
			}
			break;
			case FT_Float:
			{
				float fTemp = atof(strValue.c_str());
				pByte = new unsigned char[sizeof(fTemp)];
				memcpy(pByte, &fTemp, sizeof(fTemp));
			}
			break;
			case FT_Double:
			{
				double  dTemp = atof(strValue.c_str());
				pByte = new unsigned char[sizeof(dTemp)];
				memcpy(pByte, &dTemp, sizeof(dTemp));
			}
			break;
			case FT_Text:
			{
				pByte = new unsigned char[strValue.capacity() + 1];
				memset(pByte, 0, strValue.capacity() + 1);
				memcpy(pByte, strValue.c_str(), strValue.capacity() + 1);
			}
			break;
			case FT_Date:
			{
				double temp = atof(strValue.c_str());
				pByte = new unsigned char[sizeof(temp)];
				memcpy(pByte, &temp, sizeof(temp));
			}
			break;
			case FT_Time:
			{
				double temp = atof(strValue.c_str());
				pByte = new unsigned char[sizeof(temp)];
				memcpy(pByte, &temp, sizeof(temp));
			}
			break;
			case FT_TimeStamp:
			{
				double dTime = atof(strValue.c_str());
				pByte = new unsigned char[sizeof(dTime)];
				memcpy(pByte, &dTime, sizeof(dTime));
			}
			break;
			default:
				break;
			}
			pFeature->m_fieldValues[iField] = pByte;
			iField++;
		}
		if (pFeatureValues != NULL)
		{
			delete pFeatureValues;
			pFeatureValues = NULL;
		}

		return true;
	}

	bool S3MBLayerInfos::ReadLayerFieldInfosFromFile(wstring filePath)
	{
#ifdef WIN32
        std::ifstream ifs(filePath, ios::in | ios::binary);
#else
        string path = StringUtil::UnicodeToANSI(filePath);
        std::ifstream ifs(path, ios::in | ios::binary);
#endif
		if (!ifs)
		{
			return false;
		}
		string strJson;
		getline(ifs, strJson);
		strJson += '\n';
		JsonValue jValue;
		if (!jValue.LoadValueFromString(strJson))
		{
			return false;
		}

		JsonValue* pJsonLayerInfos = NULL;
		if (jValue.GetValue(S3MB_JSON_LAYERINFOS, pJsonLayerInfos))
		{
			JsonValue pJsonLayerInfo(JsonValueType::objectValue);
			int nLayerInfos = pJsonLayerInfos->GetArraySize();
			for (int i = 0; i < nLayerInfos; i++)
			{
				if (pJsonLayerInfos->GetAt(i, &pJsonLayerInfo))
				{
					LayerCacheInfo datasetInfo;

					if (pJsonLayerInfo.GetValue(S3MB_JSON_LAYERINFO_NAME, datasetInfo.m_strDatasetName))
					{
						// IDRange
						int nMin = 0, nMax = 0;
						S3MBLayerInfos::GetIDRangeFromJson(pJsonLayerInfo, nMin, nMax);
						datasetInfo.m_pairIDRange.first = nMin;
						datasetInfo.m_pairIDRange.second = nMax;
						// FieldInfos
						S3MBLayerInfos::GetFieldInfosFromJson(datasetInfo.m_fieldInfos, pJsonLayerInfo);
						m_vecLayerInfos.push_back(datasetInfo);
					}
				}
			}
		}
		if (pJsonLayerInfos != NULL)
		{
			delete pJsonLayerInfos;
			pJsonLayerInfos = NULL;
		}
		return true;
	}

	const vector<LayerCacheInfo>& S3MBLayerInfos::GetLayerFieldInfos()
	{
		return m_vecLayerInfos;
	}

	bool S3MBLayerInfos::ReadLayerFieldInfosFromStream(void* buffer, int bufferSize, vector<LayerCacheInfo>& vecLayerInfos)
	{
		string strJson;
		strJson = (char*)buffer;
		strJson += '\n';
		JsonValue jValue;
		if (!jValue.LoadValueFromString(strJson))
		{
			return false;
		}

		JsonValue* pJsonLayerInfos = NULL;
		if (jValue.GetValue(S3MB_JSON_LAYERINFOS, pJsonLayerInfos))
		{
			JsonValue pJsonLayerInfo(JsonValueType::objectValue);
			int nLayerInfos = pJsonLayerInfos->GetArraySize();
			for (int i = 0; i < nLayerInfos; i++)
			{
				if (pJsonLayerInfos->GetAt(i, &pJsonLayerInfo))
				{
					LayerCacheInfo datasetInfo;

					if (pJsonLayerInfo.GetValue(S3MB_JSON_LAYERINFO_NAME, datasetInfo.m_strDatasetName))
					{
						// IDRange
						int nMin = 0, nMax = 0;
						S3MBLayerInfos::GetIDRangeFromJson(pJsonLayerInfo, nMin, nMax);
						datasetInfo.m_pairIDRange.first = nMin;
						datasetInfo.m_pairIDRange.second = nMax;
						// FieldInfos
						S3MBLayerInfos::GetFieldInfosFromJson(datasetInfo.m_fieldInfos, pJsonLayerInfo);
						vecLayerInfos.push_back(datasetInfo);
					}
				}
			}
		}
		if (pJsonLayerInfos != NULL)
		{
			delete pJsonLayerInfos;
			pJsonLayerInfos = NULL;
		}
		return true;
	}

	void S3MBLayerInfos::GetIDRangeFromJson(JsonValue& jsonValue, int& nMin, int& nMax)
	{
		JsonValue* pIDRange = NULL;
		if (jsonValue.GetValue(S3MB_JSON_LAYERINFO_IDRANGE, pIDRange))
		{
			pIDRange->GetValue(S3MB_JSON_LAYERINFO_IDRANGE_MIN, nMin);
			pIDRange->GetValue(S3MB_JSON_LAYERINFO_IDRANGE_MAX, nMax);
		}
		if (pIDRange != NULL)
		{
			delete pIDRange;
			pIDRange = NULL;
		}
	}

	bool S3MBLayerInfos::IsIDExisted(std::map<unsigned int, Feature*>& mapFeature, int nBeginID, int nEndID)
	{
		bool bExisted = false;
		std::map<unsigned int, Feature*>::iterator it;
		for (it = mapFeature.begin(); it != mapFeature.end(); it++)
		{
			if (it->first <= nEndID && it->first >= nBeginID)
			{
				bExisted = true;
				break;
			}
		}
		return bExisted;
	}

	void S3MBLayerInfos::GetFieldInfosFromJson(S3MBFieldInfos& fieldInfos, JsonValue& jsonValue)
	{
		wstring strElemValue;
		JsonValue* pFieldInfos = NULL;
		if (jsonValue.GetValue(S3MB_JSON_LAYERINFO_FIELDINFOS, pFieldInfos))
		{
			JsonValue pFieldInfo(JsonValueType::objectValue);
			int nFieldInfos = pFieldInfos->GetArraySize();
			for (int i = 0; i < nFieldInfos; i++)
			{
				S3MBFieldInfo fieldInfo;
				pFieldInfos->GetAt(i, &pFieldInfo);

				wstring strType;
				if (pFieldInfo.GetValue(S3MB_JSON_FIELD_ATT_TYPE, strType))
				{
					FieldType nType = GetFieldInfoType(strType);
					if (nType == FT_UnKnown)
					{
						continue;
					}
					else
					{
						fieldInfo.m_nType = nType;
					}
				}

				pFieldInfo.GetValue(S3MB_JSON_FIELD_ATT_NAME, fieldInfo.m_strName);
				pFieldInfo.GetValue(S3MB_JSON_FIELD_ATT_ALIAS, fieldInfo.m_strForeignName);
				pFieldInfo.GetValue(S3MB_JSON_FIELD_ATT_SIZE, fieldInfo.m_nSize);
				pFieldInfo.GetValue(S3MB_JSON_FIELD_ATT_ISREQUIRED, fieldInfo.m_bRequired);
				fieldInfos.add(fieldInfo);
			}
		}
		if (pFieldInfos != NULL)
		{
			delete pFieldInfos;
			pFieldInfos = NULL;
		}
	}

	bool S3MBLayerInfos::WriteLayerFieldInfosToFile(const std::vector<LayerCacheInfo>& vecLayerInfo, std::wstring& strFilePath)
	{
		strFilePath = StringUtil::GetDir(strFilePath) + FILE_NAME_ATTINFO;

		JsonValue jsonValue(objectValue);
		SaveLayerInfoToJSON(vecLayerInfo, jsonValue);
		jsonValue.SaveToFile(strFilePath);
		return true;
	}

	bool S3MBLayerInfos::WriteLayerAttributeValueToFile(const std::map<unsigned int, std::pair<unsigned int, std::vector<Feature*> > >& mapFeature, \
		std::wstring& strFilePath)
	{
		strFilePath = StringUtil::ChangeExt(strFilePath, FILE_EXT_S3MD);

		JsonValue jsonValue(objectValue);
		SaveAttributeDataToJSON(mapFeature, jsonValue);

		std::wstring strJson;
		jsonValue.SaveToString(strJson);

		// Zip the file
		MemoryStream streamUnZip;
		streamUnZip.Init();
		streamUnZip << strJson;

		// 压缩流
		unsigned char* pZippedData = NULL;
		unsigned int nZippedSize = 0;
		unsigned int nUnZipSize = (unsigned int)streamUnZip.GetLength();
		pZippedData = new unsigned char[nUnZipSize];
		nZippedSize = nUnZipSize;
		bool result = Utils::Zip(pZippedData, nZippedSize, streamUnZip.GetDataPtr(), nUnZipSize);

#ifdef WIN32
		std::ofstream ofs(strFilePath, ios::out | ios::binary | ios::trunc);
#else
		string strDestFilePath = StringUtil::UnicodeToANSI(strFilePath);
		std::ofstream ofs(strDestFilePath, ios::out | ios::binary | ios::trunc);
#endif

		ofs.write((char*)&nUnZipSize, sizeof(unsigned int));
		ofs.write((char*)&nZippedSize, sizeof(unsigned int));
		ofs.write((char*)pZippedData, nZippedSize);
		ofs.close();
		delete[] pZippedData;
		pZippedData = nullptr;

		return true;
	}

	void S3MBLayerInfos::SaveLayerInfoToJSON(const std::vector<LayerCacheInfo>& vecLayerInfo, JsonValue& jsonValue)
	{
		JsonValue jsonLayerInfos(JsonValueType::arrayValue);
		for (int i = 0; i < vecLayerInfo.size(); i++)
		{
			LayerCacheInfo datasetInfo = vecLayerInfo[i];

			JsonValue jsonLayerInfo(JsonValueType::objectValue);
			jsonLayerInfo.Add(S3MB_JSON_LAYERINFO_NAME, datasetInfo.m_strDatasetName);

			SaveIDRangeToJSON(datasetInfo.m_pairIDRange.first, datasetInfo.m_pairIDRange.second, jsonLayerInfo);
			SaveFieldInfosToJSON(datasetInfo.m_fieldInfos, jsonLayerInfo);

			jsonLayerInfos.Add(jsonLayerInfo);
		}

		jsonValue.Add(S3MB_JSON_LAYERINFOS, jsonLayerInfos);
	}

	void S3MBLayerInfos::SaveIDRangeToJSON(int nMin, int nMax, JsonValue& jsonValue)
	{
		JsonValue jsonIDRange(JsonValueType::objectValue);
		jsonIDRange.Add(S3MB_JSON_LAYERINFO_IDRANGE_MIN, nMin);
		jsonIDRange.Add(S3MB_JSON_LAYERINFO_IDRANGE_MAX, nMax);
		jsonValue.Add(S3MB_JSON_LAYERINFO_IDRANGE, jsonIDRange);
	}

	void S3MBLayerInfos::SaveFieldInfosToJSON(S3MBFieldInfos& fieldInfos, JsonValue& jsonValue)
	{
		JsonValue jsonFieldInfos(JsonValueType::arrayValue);
		for (int i = 0; i < fieldInfos.GetSize(); i++)
		{
			S3MBFieldInfo fieldInfo = fieldInfos.GetAt(i);
			std::wstring strType = GetFieldInfoType(fieldInfo.m_nType);
			if (strType.length() == 0)
			{
				continue;
			}

			std::wstring strName = fieldInfo.m_strName;
			int nSize = fieldInfo.m_nSize < 0 ? 0 : fieldInfo.m_nSize;

			JsonValue jsonFieldInfo(JsonValueType::objectValue);
			jsonFieldInfo.Add(S3MB_JSON_FIELD_ATT_NAME, fieldInfo.m_strName);
			jsonFieldInfo.Add(S3MB_JSON_FIELD_ATT_ALIAS, fieldInfo.m_strForeignName);
			jsonFieldInfo.Add(S3MB_JSON_FIELD_ATT_TYPE, strType);
			jsonFieldInfo.Add(S3MB_JSON_FIELD_ATT_SIZE, nSize);
			jsonFieldInfo.Add(S3MB_JSON_FIELD_ATT_ISREQUIRED, fieldInfo.m_bRequired);
			jsonFieldInfos.Add(jsonFieldInfo);
		}

		jsonValue.Add(S3MB_JSON_LAYERINFO_FIELDINFOS, jsonFieldInfos);
	}

	std::wstring S3MBLayerInfos::GetFieldInfoType(FieldType nType)
	{
		std::wstring strType;

		switch (nType)
		{
		case FT_Boolean:
			strType = S3MB_JSON_FIELD_ATT_TYPE_BOOL;
			break;
		case FT_INT16:
			strType = S3MB_JSON_FIELD_ATT_TYPE_INT16;
			break;
		case FT_INT32:
			strType = S3MB_JSON_FIELD_ATT_TYPE_INT32;
			break;
		case FT_INT64:
			strType = S3MB_JSON_FIELD_ATT_TYPE_INT64;
			break;
		case FT_Float:
			strType = S3MB_JSON_FIELD_ATT_TYPE_FLOAT;
			break;
		case FT_Double:
			strType = S3MB_JSON_FIELD_ATT_TYPE_DOUBLE;
			break;
		case FT_Text:
			strType = S3MB_JSON_FIELD_ATT_TYPE_STRING;
			break;
		case FT_Date:
			strType = S3MB_JSON_FIELD_ATT_TYPE_DATE;
			break;
		case FT_Time:
			strType = S3MB_JSON_FIELD_ATT_TYPE_TIME;
			break;
		case FT_TimeStamp:
			strType = S3MB_JSON_FIELD_ATT_TYPE_TIMESTAMP;
			break;
		default:
			break;
		}

		return strType;
	}

	bool S3MBLayerInfos::SaveAttributeDataToJSON(const std::map<unsigned int, std::pair<unsigned int, std::vector<Feature*> > >& mapFeature, JsonValue& jsonValue)
	{
		JsonValue jsonLayerInfos(JsonValueType::arrayValue);
		std::map<unsigned int, std::pair<unsigned int, std::vector<Feature*> > >::const_iterator it;
		for (it = mapFeature.begin(); it != mapFeature.end(); it++)
		{
			std::vector<Feature*> vecFeature = it->second.second;
			if (vecFeature.size() == 0)
			{
				continue;
			}

			// 图层信息
			JsonValue jsonLayerInfo(JsonValueType::objectValue);
			SaveIDRangeToJSON(it->second.first, it->first, jsonLayerInfo);

			std::vector<FieldDefine> vecFieldDefine = vecFeature[0]->m_fieldDefines;
			SaveFieldDefinesToJSON(vecFieldDefine, jsonLayerInfo);

			// 属性记录信息
			JsonValue jsonRecords(JsonValueType::arrayValue);
			for (int i = 0; i < vecFeature.size(); i++)
			{
				Feature* pFeature = vecFeature[i];
				if (pFeature == nullptr)
				{
					continue;
				}

				JsonValue jsonRecord(JsonValueType::objectValue);
				jsonRecord.Add(S3MB_JSON_ATT_ATT_ID, pFeature->m_nID);
				SaveFeatureValuesToJSON(pFeature, jsonRecord);
				jsonRecords.Add(jsonRecord);
			}

			jsonLayerInfo.Add(S3MB_JSON_ATT_RECORDS, jsonRecords);
			jsonLayerInfos.Add(jsonLayerInfo);
		}

		jsonValue.Add(S3MB_JSON_LAYERINFOS, jsonLayerInfos);
		return true;
	}

	void S3MBLayerInfos::SaveFieldDefinesToJSON(const std::vector<FieldDefine>& vecFieldDefine, JsonValue& jsonValue)
	{
		JsonValue jsonFieldDefines(JsonValueType::arrayValue);
		for (unsigned int i = 0; i < vecFieldDefine.size(); i++)
		{
			FieldDefine fieldDefine = vecFieldDefine[i];
			std::wstring strType = GetFieldInfoType(fieldDefine.m_nType);
			if (strType.length() == 0)
			{
				continue;
			}

			JsonValue jsonFieldDefine(JsonValueType::objectValue);
			jsonFieldDefine.Add(S3MB_JSON_FIELD_ATT_NAME, fieldDefine.m_strName);
			jsonFieldDefine.Add(S3MB_JSON_FIELD_ATT_TYPE, strType);
			jsonFieldDefine.Add(S3MB_JSON_FIELD_ATT_SIZE, fieldDefine.m_nSize);
			jsonFieldDefine.Add(S3MB_JSON_FIELD_ATT_ISREQUIRED, fieldDefine.m_bRequired);
			jsonFieldDefines.Add(jsonFieldDefine);
		}

		jsonValue.Add(S3MB_JSON_LAYERINFO_FIELDINFOS, jsonFieldDefines);
	}

	void S3MBLayerInfos::SaveFeatureValuesToJSON(Feature* pFeature, JsonValue& jsonValue)
	{
		JsonValue jsonFeatureValues(JsonValueType::arrayValue);

		std::wstring strRecordValue;
		std::vector<FieldDefine>& vecFieldDefine = pFeature->m_fieldDefines;
		for (int i = 0; i < vecFieldDefine.size(); i++)
		{
			FieldDefine fieldDefine = vecFieldDefine[i];
			switch (fieldDefine.m_nType)
			{
			case FT_Boolean:
			case FT_INT16:
			case FT_INT64:
			case FT_Float:
			case FT_Double:
			case FT_Date:
			case FT_Text:
			case FT_Time:
			case FT_TimeStamp:
				{
					Variant varValue;
					if (!pFeature->GetValue(fieldDefine.m_strName, varValue))
					{
						continue;
					}
					strRecordValue = varValue.ToString();
				}
				break;
			case FT_INT32:
				{
					Variant varValue;
					if (!pFeature->GetValue(fieldDefine.m_strName, varValue))
					{
						continue;
					}
					if (StringUtil::CompareNoCase(fieldDefine.m_strName, U("SmID")))
					{
						varValue = Variant(pFeature->m_nID);
					}
					strRecordValue = varValue.ToString();
				}
				break;
			default:
				continue;
			}

			JsonValue jsonFeatureValue(JsonValueType::objectValue);
			jsonFeatureValue.Add(S3MB_JSON_ATT_ATT_NAME, fieldDefine.m_strName);
			jsonFeatureValue.Add(S3MB_JSON_ATT_FIELD, strRecordValue);
			jsonFeatureValues.Add(jsonFeatureValue);
		}

		jsonValue.Add(S3MB_JSON_ATT_VALUES, jsonFeatureValues);
	}

	void Feature::SetFieldInfos(S3MBFieldInfos& fieldInfos)
	{
		int nCount = fieldInfos.GetSize();
		m_fieldDefines.resize(nCount);
		m_fieldValues.resize(nCount);
		FieldDefine fieldDefine;
		for (int i = 0; i < nCount; i++)
		{
			S3MBFieldInfo fieldInfo = fieldInfos.GetAt(i);
			fieldDefine.m_nSize = fieldInfo.m_nSize;
			fieldDefine.m_nType = fieldInfo.m_nType;
			fieldDefine.m_strName = fieldInfo.m_strName;
			fieldDefine.m_bRequired = fieldInfo.m_bRequired;
			m_fieldDefines.at(i) = fieldDefine;

			if (!fieldInfo.m_strDefaultValue.empty())
			{
				unsigned char* pByte = new unsigned char[fieldInfo.m_strDefaultValue.size()];
				memcpy(pByte, fieldInfo.m_strDefaultValue.c_str(), fieldInfo.m_strDefaultValue.size());
				m_fieldValues.at(i) = pByte;
			}
		}
	}

	bool Feature::SetValue(int nIndex, const Variant& varValue)
	{
		if (nIndex >= m_fieldValues.size())
		{
			return false;
		}

		if (varValue.m_nType == VarType::VT_Null)
		{
			delete[] m_fieldValues[nIndex];
			m_fieldValues[nIndex] = NULL;
			return true;
		}

		unsigned char* pByte = pByte = new unsigned char[m_fieldDefines[nIndex].m_nSize];
		switch (m_fieldDefines[nIndex].m_nType)
		{
		case FT_Boolean:
		case FT_Byte:
		{
			*pByte = (unsigned char)varValue.ToInt();
			break;
		}
		case FT_INT16:
		{
			*(short*)pByte = (short)varValue.ToInt();
			break;
		}
		case FT_INT32:
		{
			*(int*)pByte = varValue.ToInt();
			break;
		}
		case FT_INT64:
		{
			*(long*)pByte = varValue.ToLong();
			break;
		}
		case FT_Float:
		{
			*(float*)pByte = (float)varValue.ToDouble();
			break;
		}
		case FT_Double:
		{
			*(double*)pByte = varValue.ToDouble();
			break;
		}
		case FT_Text:
		{
			std::string strTemp = StringUtil::UNICODE_to_UTF8(varValue.ToString());
			strcpy((char*)pByte, strTemp.c_str());
			break;
		}
		case FT_Date:
		case FT_Time:
		case FT_TimeStamp:
		{
			*(double*)pByte = varValue.GetValue().tmVal;
			break;
		}
		break;
		}

		delete[] m_fieldValues[nIndex];
		m_fieldValues[nIndex] = pByte;

		return true;
	}

	bool Feature::SetValue(const wstring& strFieldName, const Variant& varValue)
	{
		int nCount = m_fieldDefines.size();
		for (int i = 0; i < nCount; i++)
		{
			if (StringUtil::CompareNoCase(m_fieldDefines.at(i).m_strName, strFieldName))
			{
				return SetValue(i, varValue);
			}
	}
		return false;
	}

	bool Feature::GetValue(const wstring& strFieldName, Variant& varValue) const
	{
		int nCount = m_fieldDefines.size();
		for (int i = 0; i < nCount; i++)
		{
			if (StringUtil::CompareNoCase(m_fieldDefines.at(i).m_strName, strFieldName))
			{
				return GetValue(i, varValue);
			}
		}
		return false;
	}

	bool Feature::GetValue(int nIndex, Variant& varValue) const
	{
		unsigned char* pByte = m_fieldValues.at(nIndex);
		if (pByte == NULL)
		{
			varValue.Clear();
			return true;
		}
		switch (m_fieldDefines.at(nIndex).m_nType)
		{
		case FT_Boolean:
		case FT_Byte:
		{
			varValue = *pByte;
		}
		break;
		case FT_INT16:
		{
			varValue = *(short*)pByte;
		}
		break;
		case FT_INT32:
		{
			varValue = *(int*)pByte;
		}
		break;
		case FT_INT64:
		{
			varValue = *(long*)pByte;
		}
		break;
		case FT_Double:
		{
			varValue = *(double*)pByte;
		}
		break;
		case FT_Float:
		{
			varValue = *(float*)pByte;
		}
		break;
		case FT_Text:
		{
			string strTemp = string((char*)pByte);
			varValue = strTemp;
		}
		break;
		case FT_Date:
		case FT_Time:
		case FT_TimeStamp:
		{
			double dValue = 0;
			dValue = *(double*)pByte;
			varValue = dValue;
			Time dateTime = dValue;
			varValue = dateTime;
		}
		break;
		default:
		{
			varValue.Clear();
			return false;
		}
		break;
		}
		return false;
	}
}
