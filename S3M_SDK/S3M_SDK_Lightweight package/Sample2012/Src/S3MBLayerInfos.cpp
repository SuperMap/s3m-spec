#include "S3MBLayerInfos.h"
#include <fstream>

S3mbFieldInfo::FieldType S3MBLayerInfos::GetFieldInfoType(string strType)
{
	S3mbFieldInfo::FieldType nType = S3mbFieldInfo::UnKnown;

	if (strType == S3MB_JSON_FIELD_ATT_TYPE_BOOL)
	{
		nType = S3mbFieldInfo::Boolean;
	}
	else if (strType == S3MB_JSON_FIELD_ATT_TYPE_INT16)
	{
		nType = S3mbFieldInfo::INT16;
	}
	else if (strType == S3MB_JSON_FIELD_ATT_TYPE_INT32)
	{
		nType = S3mbFieldInfo::INT32;
	}
	else if (strType == S3MB_JSON_FIELD_ATT_TYPE_INT64)
	{
		nType = S3mbFieldInfo::INT64;
	}
	else if (strType == S3MB_JSON_FIELD_ATT_TYPE_FLOAT)
	{
		nType = S3mbFieldInfo::Float;
	}
	else if (strType == S3MB_JSON_FIELD_ATT_TYPE_DOUBLE)
	{
		nType = S3mbFieldInfo::Double;
	}
	else if (strType == S3MB_JSON_FIELD_ATT_TYPE_STRING)
	{
		nType = S3mbFieldInfo::Text;
	}
	else if (strType == S3MB_JSON_FIELD_ATT_TYPE_NTEXT)
	{
		nType = S3mbFieldInfo::NText;
	}
	else if (strType == S3MB_JSON_FIELD_ATT_TYPE_WCHAR)
	{
		nType = S3mbFieldInfo::Char;
	}
	else if (strType == S3MB_JSON_FIELD_ATT_TYPE_DATE)
	{
		nType = S3mbFieldInfo::Date;
	}
	else if (strType == S3MB_JSON_FIELD_ATT_TYPE_TIME)
	{
		nType = S3mbFieldInfo::Time;
	}
	else if (strType == S3MB_JSON_FIELD_ATT_TYPE_TIMESTAMP)
	{
		nType = S3mbFieldInfo::TimeStamp;
	}

	return nType;
}

void S3mbFieldInfos::add(S3mbFieldInfo* newFieldInfo)
{
	m_array.push_back(newFieldInfo);
}


bool S3MBLayerInfos::LoadAttributeDataFromFile(const string& strAttFilePath, std::map<uInt, Feature*>& mapFeature)
{
	//读取文件并解压
	std::ifstream ifs(strAttFilePath, ios::in | ios::binary);
	if (!ifs)
	{
		return false;
	}
	uInt nUnZipSize = 0;
	uInt nZipSize = 0;
	ifs.read((char*)&nUnZipSize, sizeof(uInt));
	ifs.read((char*)&nZipSize, sizeof(uInt));
	byte* pByteUnZip = new byte[nUnZipSize];
	byte* pByteZip = new byte[nZipSize];
	ifs.read((char*)pByteZip, nZipSize);
	ifs.close();
#pragma region 解压
	unsigned long ulSize = nUnZipSize;
	bool bResult = false;
	int nResult = uncompress(pByteUnZip, &ulSize, pByteZip, nZipSize);//nUnZipSize的大小会改变

	while (!bResult)
	{
		//防错处理
		switch (nResult)
		{
		case Z_OK:
			bResult = true;
			break;
		case Z_MEM_ERROR:
			cout << "内存分配不足" << endl;
			break;
		case Z_BUF_ERROR:
			cout << "输出缓存没有足够的空间" << endl;
			break;
		case Z_STREAM_ERROR:
			cout << "压缩文件参数非法" << endl;
			break;
		case Z_DATA_ERROR:
			//数据或者密码错误 //调换顺序，解决桌面打开工作空间未弹出输密码的对话框之前，先报错“数据错误”的问题 DFAB-1100		
			cout << "数据错误" << endl;
			break;
		default:
			//不可识别的错误，
			cout << "不可识别的错误！" << endl;
			break;
		}

		if (!bResult)//如果解压失败.
		{
			cout << "解压失败！" << endl;

			delete[]pByteZip;
			pByteZip = NULL;
			delete[]pByteUnZip;
			pByteUnZip = NULL;
			continue;
		}
	}
	delete[]pByteZip;
	pByteZip = NULL;
#pragma endregion
	MemoryStream streamUnZipped;
	streamUnZipped.Init(pByteUnZip, nUnZipSize, true);
	pByteUnZip = NULL;

	string strJsonInfo;
	streamUnZipped >> strJsonInfo;
	streamUnZipped.Clear();
	//strJsonInfo = StringHelper::UTF8_to_ANSI(strJsonInfo);

	JsonValue* pJsonValue = new JsonValue();
	if (!pJsonValue->LoadValueFormString(strJsonInfo))
	{
		return false;
	}

	AttributeDataFromJson(mapFeature, *pJsonValue);
	return true;
}

bool S3MBLayerInfos::AttributeDataFromJson(std::map<uInt, Feature*>& mapFeature, JsonValue & jsonValue)
{
	bool bValid = false;
	JsonValue* pJsonLayers = NULL;
	if (jsonValue.GetValue(S3MB_JSON_LAYERINFOS, pJsonLayers))
	{
		JsonValue pJsonLayer(objectValue);
		int nLayerSize = pJsonLayers->GetArraySize();
		for (int i = 0; i < nLayerSize; i++)
		{
			if (pJsonLayers->GetAt(i, &pJsonLayer))
			{
				int nMin = 0, nMax = 0;
				GetIDRangeFromJson(pJsonLayer, nMin, nMax);
				if (!IsIDExisted(mapFeature, nMin, nMax))
				{
					continue;
				}
				S3mbFieldInfos fieldInfos;
				GetFieldInfosFromJson(fieldInfos, pJsonLayer);
				if (fieldInfos.GetSize() == 0)
				{
					continue;
				}
				JsonValue* pRecods = NULL;
				if (pJsonLayer.GetValue(S3MB_JSON_ATT_RECORDS, pRecods))
				{
					JsonValue pRecod(objectValue);
					int nRecords = pRecods->GetArraySize();
					for (int j = 0; j < nRecords; j++)
					{
						if (pRecods->GetAt(j, &pRecod))
						{
							int nID = 0;
							pRecod.GetValue(S3MB_JSON_ATT_ATT_ID, nID);
							if (mapFeature.find(nID) == mapFeature.end() || mapFeature[nID] != NULL)
							{
								continue;
							}

							Feature *pFeature = new Feature();
							pFeature->SetFieldInfos(fieldInfos);
							int nCount = fieldInfos.GetSize();
							pFeature->m_fieldValues.resize(nCount);
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

bool S3MBLayerInfos::GetFeatureValuesFromJson(Feature *pFeature, JsonValue & jsonValue)
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
	//vector<FieldDefine> fieldDefines = pFeature->m_fieldDefines;

	int iField = 0;
	JsonValue pFeatureValue(objectValue);
	int nFeatureSize = pFeatureValues->GetArraySize();
	for (int i = 0; i < nFeatureSize; i++)
	{
		if (!pFeatureValues->GetAt(i, &pFeatureValue))
		{
			continue;
		}

		if (iField >= pFeature->m_fieldDefines.size())
		{
			//前面的字段定义和后面的值个数不一致了
			assert(false);
			break;
		}
		string strFieldName;
		pFeatureValue.GetValue(S3MB_JSON_ATT_ATT_NAME, strFieldName);
		for (uInt i = iField; i < pFeature->m_fieldDefines.size(); i++)
		{
			if (StringHelper::CompareNoCase(pFeature->m_fieldDefines[i].m_strName, strFieldName))
			{
				iField = i;
				break;
			}
		}

		if (iField > pFeature->m_fieldDefines.size())
		{
			//前面的字段定义和后面的字段名不一致了
			assert(false);
			break;
		}
		string strValue;
		pFeatureValue.GetValue(S3MB_JSON_ATT_FIELD, strValue);
		Byte* pByte = NULL;
		switch (pFeature->m_fieldDefines[iField].m_nType)
		{
		case S3mbFieldInfo::Boolean:
		{
			bool bTemp = (strValue == "1") ? true : false;
			pByte = new Byte[sizeof(bTemp)];
			memcpy(pByte, &bTemp, sizeof(bTemp));
		}
		break;
		case S3mbFieldInfo::INT16:
		{
			short sTemp = atoi(strValue.c_str());
			pByte = new Byte[sizeof(sTemp)];
			memcpy(pByte, &sTemp, sizeof(sTemp));
		}
		break;
		case S3mbFieldInfo::INT32:
		{
			int  iTemp = atoi(strValue.c_str());
			pByte = new Byte[sizeof(iTemp)];
			memcpy(pByte, &iTemp, sizeof(iTemp));
		}
		break;
		case S3mbFieldInfo::INT64:
		{
			long lTemp = atol(strValue.c_str());
			pByte = new Byte[sizeof(lTemp)];
			memcpy(pByte, &lTemp, sizeof(lTemp));
		}
		break;
		case S3mbFieldInfo::Float:
		{
			float fTemp = atof(strValue.c_str());
			pByte = new Byte[sizeof(fTemp)];
			memcpy(pByte, &fTemp, sizeof(fTemp));
		}
		break;
		case S3mbFieldInfo::Double:
		{
			double  dTemp = atof(strValue.c_str());
			pByte = new Byte[sizeof(dTemp)];
			memcpy(pByte, &dTemp, sizeof(dTemp));
		}
		break;
		case S3mbFieldInfo::Text:
		case S3mbFieldInfo::NText:
		case S3mbFieldInfo::Char:
		{
			strValue = StringHelper::UTF8_to_ANSI(strValue);
			pByte = new Byte[strValue.size()+1];
			memset(pByte, 0, strValue.size()+1);
			memcpy(pByte, strValue.c_str(), strValue.size()+1);
		}
		break;
		case S3mbFieldInfo::Date:
		{
			//S3mbTime tTemp;
			//if (!tTemp.ParseDateTime(strValue, "%Y-%m-%d"))
			//{
			//	break;
			//}
			//double temp = tTemp.GetTime();
			double temp = atof(strValue.c_str());
			pByte = new Byte[sizeof(temp)];
			memcpy(pByte, &temp, sizeof(temp));
		}
		break;
		case S3mbFieldInfo::Time:
		{
			//S3mbTime tTemp;
			//if (!tTemp.ParseDateTime(strValue, "%H:%M:%S"))
			//{
			//	break;
			//}
			//double temp = tTemp.GetTime();
			double temp = atof(strValue.c_str());
			pByte = new Byte[sizeof(temp)];
			memcpy(pByte, &temp, sizeof(temp));
		}
		break;
		case S3mbFieldInfo::TimeStamp:
		{
			//double dTime = atof(strValue.c_str());
			//S3mbTime tTemp = S3mbTime(dTime);
			//dTime = tTemp.GetTime();
			double dTime = atof(strValue.c_str());
			pByte = new Byte[sizeof(dTime)];
			memcpy(pByte, &dTime, sizeof(dTime));
		}
		break;
		case S3mbFieldInfo::LongBinary:
		{
			vector<string> arrStr;
			StringHelper::Split(strValue, arrStr, '|');

			vector<double> aryPnts;
			int nSize = arrStr.size();
			for (int i = 0; i < nSize; i++)
			{
				double dPos = atof(arrStr[i].c_str());
				aryPnts.push_back(dPos);
			}

			pByte = new Byte[nSize * sizeof(double)];
			memcpy(pByte, &aryPnts.begin(), nSize * sizeof(double));
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

bool S3MBLayerInfos::LoadLayerInfoFromJson(string filePath, vector<LayerCacheInfo>& vecLayerInfos)
{
	std::ifstream ifs(filePath, ios::in | ios::binary);
	if (!ifs)
	{
		return false;
	}
	string strJson;
	getline(ifs, strJson);
	strJson = StringHelper::UTF8_to_ANSI(strJson);
	strJson += '\n';
	JsonValue* pValue = new JsonValue(objectValue);
	Reader reader;
	if (!reader.parse(strJson, pValue->GetValue()))
	{
		return false;
	}
	JsonValue* pJsonLayerInfos = NULL;
	if (pValue->GetValue(S3MB_JSON_LAYERINFOS, pJsonLayerInfos))
	{
		JsonValue pJsonLayerInfo(objectValue);
		int nLayerInfos = pJsonLayerInfos->GetArraySize();
		for (int i = 0; i < nLayerInfos; i++)
		{
			if (pJsonLayerInfos->GetAt(i, &pJsonLayerInfo))
			{
				LayerCacheInfo datasetInfo;

				if (pJsonLayerInfo.GetValue(S3MB_JSON_LAYERINFO_NAME, datasetInfo.m_strDatasetName))
				{
					//IDRange
					int nMin = 0, nMax = 0;
					S3MBLayerInfos::GetIDRangeFromJson(pJsonLayerInfo, nMin, nMax);
					datasetInfo.m_pairIDRange.first = nMin;
					datasetInfo.m_pairIDRange.second = nMax;
					//FieldInfos
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
	if (pValue != NULL)
	{
		delete pValue;
		pValue = NULL;
	}
	return true;
}

void S3MBLayerInfos::GetIDRangeFromJson(JsonValue & jsonValue, int &nMin, int &nMax)
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

bool S3MBLayerInfos::IsIDExisted(std::map<uInt, Feature*>& mapFeature, int nBeginID, int nEndID)
{
	bool bExisted = false;
	std::map<uInt, Feature*>::iterator it;
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

void S3MBLayerInfos::GetFieldInfosFromJson(S3mbFieldInfos& fieldInfos, JsonValue & jsonValue)
{
	string strElemValue;
	JsonValue* pFieldInfos = NULL;
	if (jsonValue.GetValue(S3MB_JSON_LAYERINFO_FIELDINFOS, pFieldInfos))
	{
		JsonValue pFieldInfo(objectValue);
		int nFieldInfos = pFieldInfos->GetArraySize();
		for (int i = 0; i < nFieldInfos; i++)
		{
			S3mbFieldInfo* fieldInfo  = new S3mbFieldInfo;
			pFieldInfos->GetAt(i, &pFieldInfo);

			string strType;
			if (pFieldInfo.GetValue(S3MB_JSON_FIELD_ATT_TYPE, strType))
			{
				S3mbFieldInfo::FieldType nType = GetFieldInfoType(strType);
				if (nType == S3mbFieldInfo::UnKnown)
				{
					continue;
				}
				else
				{
					fieldInfo->m_nType = nType;
				}
			}

			pFieldInfo.GetValue(S3MB_JSON_FIELD_ATT_NAME, fieldInfo->m_strName);
			pFieldInfo.GetValue(S3MB_JSON_FIELD_ATT_ALIAS, fieldInfo->m_strForeignName);
			pFieldInfo.GetValue(S3MB_JSON_FIELD_ATT_SIZE, fieldInfo->m_nSize);
			pFieldInfo.GetValue(S3MB_JSON_FIELD_ATT_ISREQUIRED, fieldInfo->m_bRequired);
			fieldInfos.add(fieldInfo);
		}
	}
	if (pFieldInfos != NULL)
	{
		delete pFieldInfos;
		pFieldInfos = NULL;
	}
}

void Feature::SetFieldInfos(S3mbFieldInfos& fieldInfos)
{
	int nCount = fieldInfos.GetSize();
	m_fieldDefines.resize(nCount);
	m_fieldValues.resize(nCount);
	FieldDefine fieldDefine;
	S3mbFieldInfo* fieldInfo;
	for (int i = 0; i < nCount; i++)
	{
		fieldInfo = fieldInfos.GetAt(i);
		fieldDefine.m_nSize = fieldInfo->m_nSize;
		fieldDefine.m_nType = fieldInfo->m_nType;
		fieldDefine.m_strName = fieldInfo->m_strName;
		fieldDefine.m_bRequired = fieldInfo->m_bRequired;
		m_fieldDefines.at(i) = fieldDefine;

		if (!fieldInfo->m_strDefaultValue.empty() &&
			fieldInfo->m_nType != S3mbFieldInfo::LongBinary)
		{
			Byte* pByte = new Byte[fieldInfo->m_strDefaultValue.size()];
			memcpy(pByte, fieldInfo->m_strDefaultValue.c_str(), fieldInfo->m_strDefaultValue.size());
			m_fieldValues.at(i) = pByte;
		}
	}
}

bool Feature::GetValue(const string& strFieldName, S3mbVariant& varValue) const
{
	int nCount = m_fieldDefines.size();
	for (int i = 0; i < nCount; i++)
	{
		if (StringHelper::CompareNoCase(m_fieldDefines.at(i).m_strName, strFieldName))
		{
			return GetValue(i, varValue);
		}
	}
	return FALSE;
}

bool Feature::GetValue(int nIndex, S3mbVariant& varValue) const
{
	byte* pByte = m_fieldValues.at(nIndex);
	if (pByte == NULL)
	{
		varValue.Clear();
		return TRUE;
	}
	switch (m_fieldDefines.at(nIndex).m_nType)
	{
	case S3mbFieldInfo::Boolean:
	case S3mbFieldInfo::Byte:
	{
		varValue = *pByte;
	}
	break;
	case S3mbFieldInfo::INT16:
	{
		varValue = *(short*)pByte;
	}
	break;
	case S3mbFieldInfo::INT32:
	{
		varValue = *(int*)pByte;
	}
	break;
	case S3mbFieldInfo::INT64:
	{
		varValue = *(long*)pByte;
	}
	break;
	case S3mbFieldInfo::Double:
	{
		varValue = *(double*)pByte;
	}
	break;
	case S3mbFieldInfo::Float:
	{
		varValue = *(float*)pByte;
	}
	break;
	case S3mbFieldInfo::Text:
	case S3mbFieldInfo::Char:
	case S3mbFieldInfo::NText:
	{
		string strTemp = string((char*)pByte);
		strTemp = StringHelper::UTF8_to_ANSI(strTemp);
		varValue = strTemp;
	}
	break;
	case S3mbFieldInfo::Date:
	case S3mbFieldInfo::Time:
	case S3mbFieldInfo::TimeStamp:
	{
		double dValue = 0;
		dValue = *(double*)pByte;
		varValue = dValue;
		S3mbTime dateTime = dValue;
		varValue = dateTime;
	}
	break;
	case S3mbFieldInfo::LongBinary:
	{
		varValue.Set(pByte, m_fieldDefines.at(nIndex).m_nSize);
	}
	break;
	case S3mbFieldInfo::Geometry:
	default:
	{
		varValue.Clear();
		return FALSE;
	}
	break;
	}
}
