#include "S3MBConfig.h"
#include "S3MBUtils.h"
#include "Variant.h"
#include <sstream>
#include <assert.h>

namespace S3MB
{
	S3MBConfig::S3MBConfig(void)
	{
		m_prjCoordEPSG = GCS_WGS_1984;
		m_strAsset = SCPS_ASSET_VALUE;
		m_fVersion = S3MB_VERSIONV3_0_1;
		m_enDataType = SDT_BIM;
		m_enPyramidSplitType = SSP_Unknown;
		m_enLODType = SLT_Replace;

		m_dbHeightMax = 0.0;
		m_dbHeightMin = 0.0;
		m_dbWMax = 0.0;
		m_dbWMin = 0.0;

		m_bHasPrj = false;
		m_bIsDegree = true;
	}

	S3MBConfig::~S3MBConfig()
	{
	}

	void S3MBConfig::SetTiles(std::vector<std::wstring>& vecRootNames, std::vector<OrientedBoundingBox>& vecOBB)
	{
		m_vecRootNames.clear();
		m_vecRootNames.insert(m_vecRootNames.begin(), vecRootNames.begin(), vecRootNames.end());

		m_vecOBB.clear();
		m_vecOBB.insert(m_vecOBB.begin(), vecOBB.begin(), vecOBB.end());
	}

	void S3MBConfig::GetTiles(std::vector<std::wstring>& vecRootNames, std::vector<OrientedBoundingBox>& vecOBB)
	{
		vecRootNames.clear();
		vecRootNames.insert(vecRootNames.begin(), m_vecRootNames.begin(), m_vecRootNames.end());

		vecOBB.clear();
		vecOBB.insert(vecOBB.begin(), m_vecOBB.begin(), m_vecOBB.end());
	}

	bool S3MBConfig::ReadFile(const wstring& strFilePath)
	{
		JsonValue jsonConfig;
		jsonConfig.LoadFromFile(strFilePath);

		wstring strElemValue;

		jsonConfig.GetValue(SCPS_JSON_ASSET, m_strAsset);

		jsonConfig.GetValue(SCPS_JSON_VERSION, strElemValue);
#ifdef WIN32
		m_fVersion = _wtof(strElemValue.c_str());
#else
		std::string strVersionTemp = StringUtil::UnicodeToANSI(strElemValue.c_str());
		m_fVersion = atof(strVersionTemp.c_str());
#endif

		jsonConfig.GetValue(SCPS_JSON_DATATYPE, strElemValue);
		m_enDataType = ToDataType(strElemValue);

		jsonConfig.GetValue(SCPS_JSON_PYRAMIDSPLITTYPE, strElemValue);
		m_enPyramidSplitType = ToPyramidSplitType(strElemValue);

		jsonConfig.GetValue(SCPS_JSON_LODTYPE, strElemValue);
		m_enLODType = ToLODType(strElemValue);

		// 插入点
		JsonValue* pJsonPosition = NULL;
		if (jsonConfig.GetValue(SCPS_JSON_POSITION, pJsonPosition))
		{
			pJsonPosition->GetValue(S3MB_JSON_X, m_pntPosition.x);
			pJsonPosition->GetValue(S3MB_JSON_Y, m_pntPosition.y);
			pJsonPosition->GetValue(S3MB_JSON_Z, m_pntPosition.z);

			wstring strUnits;
			pJsonPosition->GetValue(SCPS_JSON_UNITS, strUnits);
			m_bIsDegree = StringUtil::CompareNoCase(strUnits, SCPS_JSON_UNITS_DEGREE) ? true : false;
		}
		// GeoBounds
		jsonConfig.GetValue(SCPS_JSON_GEOBOUNDS, m_rcGeoBounds);

		// Height Range
		JsonValue* pJsonHeightRange = NULL;
		if (jsonConfig.GetValue(SCPS_JSON_HEIGHTRANGE, pJsonHeightRange))
		{
			pJsonHeightRange->GetValue(SCPS_JSON_HEIGHTRANGE_MIN, m_dbHeightMin);
			pJsonHeightRange->GetValue(SCPS_JSON_HEIGHTRANGE_MAX, m_dbHeightMax);
		}

		// WDescript
		JsonValue* pJsonWDescript = NULL;
		if (jsonConfig.GetValue(SCPS_JSON_WDESCRIPT, pJsonWDescript))
		{
			pJsonWDescript->GetValue(SCPS_JSON_WCATEGORY, m_strWCategory);
			JsonValue* pJsonRange = NULL;
			if (pJsonWDescript->GetValue(SCPS_JSON_WRANGE, pJsonRange))
			{
				pJsonRange->GetValue(SCPS_JSON_WRANGE_MIN, m_dbWMin);
				pJsonRange->GetValue(SCPS_JSON_WRANGE_MAX, m_dbWMax);
			}

			if (pJsonRange != NULL)
			{
				delete pJsonRange;
				pJsonRange = NULL;
			}
		}

		// VertexAttributeDescript
		JsonValue* pJsonVertexAttDescripts = NULL;
		if (jsonConfig.GetValue(SCPS_JSON_VERTEXATTRIBUTEDESCRIPT, pJsonVertexAttDescripts))
		{
			int nSize = pJsonVertexAttDescripts->GetArraySize();
			for (int i = 0; i < nSize; i++)
			{
				CategoryDescript descript;
				JsonValue jsonDescript;
				if (pJsonVertexAttDescripts->GetAt(i, &jsonDescript))
				{
					jsonDescript.GetValue(SCPS_JSON_CATEGORY, descript.m_strCategory);

					JsonValue* pJsonRanges = NULL;
					if (jsonDescript.GetValue(SCPS_JSON_RANGE, pJsonRanges))
					{
						JsonValue* pJsonRangeMin = NULL;
						JsonValue* pJsonRangeMax = NULL;

						if (pJsonRanges->GetValue(SCPS_JSON_DIMENSION, descript.m_nDimension) &&
							pJsonRanges->GetValue(SCPS_JSON_RANGE_MIN, pJsonRangeMin) && pJsonRanges->GetValue(SCPS_JSON_RANGE_MAX, pJsonRangeMax))
						{
							std::pair<double, double> pairRange;
							for (int j = 0; j < descript.m_nDimension; j++)
							{
								pJsonRangeMin->GetAt(j, pairRange.first);
								pJsonRangeMax->GetAt(j, pairRange.second);
								descript.m_vecRangeMin.push_back(pairRange.first);
								descript.m_vecRangeMax.push_back(pairRange.second);
							}
						}
						delete pJsonRangeMin;
						pJsonRangeMin = NULL;
						delete pJsonRangeMax;
						pJsonRangeMax = NULL;
					}
					delete pJsonRanges;
					pJsonRanges = NULL;

					m_vecVertexAttributeDescript.push_back(descript);
				}
			}
		}

		// Prj
		bool bHasPrj = jsonConfig.GetValue(SCPS_JSON_PRJCOORD, strElemValue);
		if (bHasPrj)
		{
			strElemValue = SCPS_JSON_PRJCOORD_EPSG;
#ifdef WIN32
			m_prjCoordEPSG = _wtoi(strElemValue.c_str());
#else
            std::string strTemp = StringUtil::UnicodeToANSI(strElemValue.c_str());
            m_prjCoordEPSG = atoi(strTemp.c_str());
#endif
			m_bHasPrj = true;
		}

		// TileInfos
		JsonValue* pJsonTileInfos = NULL;
		wstring strTilesKey = (EQUAL(m_fVersion, S3MB_VERSIONV3_0_1) || m_fVersion > S3MB_VERSIONV3_0_1) ? SCPS_JSON_ROOTTILES : SCPS_JSON_TILES;
		if (jsonConfig.GetValue(strTilesKey, pJsonTileInfos))
		{
			JsonValue* pJsonTileInfo = new JsonValue();
			int nTileInfos = pJsonTileInfos->GetArraySize();
			for (int i = 0; i < nTileInfos; i++)
			{
				if (pJsonTileInfos->GetAt(i, pJsonTileInfo))
				{
					if (pJsonTileInfo->GetValue(SCPS_JSON_TILE_URL, strElemValue))
					{
						m_vecRootNames.push_back(strElemValue);
					}

					// bbox
					if (EQUAL(m_fVersion, S3MB_VERSIONV3_0_1) || m_fVersion > S3MB_VERSIONV3_0_1)
					{
						JsonValue* pJsonBBox = NULL;
						if (pJsonTileInfo->GetValue(SCPS_JSON_TILE_BBOX, pJsonBBox))
						{
							Vector3d vec3Center, vec3Extent, vec3XExtent, vec3YExtent, vec3ZExtent;
							pJsonBBox->GetValue(SCPS_JSON_TILE_BBOX_CENTER, vec3Center);
							pJsonBBox->GetValue(SCPS_JSON_TILE_BBOX_XEXTENT, vec3XExtent);
							pJsonBBox->GetValue(SCPS_JSON_TILE_BBOX_YEXTENT, vec3YExtent);
							pJsonBBox->GetValue(SCPS_JSON_TILE_BBOX_ZEXTENT, vec3ZExtent);

							Matrix3d matHalfAxes;
							matHalfAxes.SetColumn(0, vec3XExtent);
							matHalfAxes.SetColumn(1, vec3YExtent);
							matHalfAxes.SetColumn(2, vec3ZExtent);

							OrientedBoundingBox mOBB(vec3Center, matHalfAxes);
							m_vecOBB.push_back(mOBB);
						}

						delete pJsonBBox;
						pJsonBBox = NULL;
					}
					else
					{
						BoundingBox bbox;
						if (pJsonTileInfo->GetValue(SCPS_JSON_TILE_BBOX, bbox))
						{
							m_vecBBox.push_back(bbox);
						}
					}
				}
			}

			delete pJsonTileInfo;
			pJsonTileInfo = NULL;
		}

		// Extensions
		JsonValue* pJsonExtensions = NULL;
		std::vector<wstring> arr;
		if (jsonConfig.GetValue(SCPS_JSON_EXTENSIONS, pJsonExtensions) &&
			pJsonExtensions->GetAllKeys(arr))
		{
			for (int i = 0; i < arr.size(); i++)
			{
				wstring strKey = arr[i];				
				if (StringUtil::CompareNoCase(strKey, SCPS_JSON_POINTCLOUD_LAYERS))
				{
					// PointCloudLayers
					JsonValue* pJsonPointCloudLayers = NULL;
					if (pJsonExtensions->GetValue(SCPS_JSON_POINTCLOUD_LAYERS, pJsonPointCloudLayers))
					{
						JsonValue* pJsonLayerInfo = new JsonValue();
						int nTileInfos = pJsonPointCloudLayers->GetArraySize();
						for (int i = 0; i < nTileInfos; i++)
						{
							if (pJsonPointCloudLayers->GetAt(i, pJsonLayerInfo))
							{
								if (pJsonLayerInfo->GetValue(SCPS_JSON_POINTCLOUD_LAYER, strElemValue))
								{
									Rect2D layerRecBounds;
									m_mapLayerNameRecBounds[strElemValue] = layerRecBounds;
									m_vecPointCloudLayer.push_back(strElemValue);
								}
								else
								{
									wstring strLayerName;
									if (pJsonLayerInfo->GetValue(SCPS_JSON_POINTCLOUD_LAYERNAME, strLayerName))
									{
										Rect2D layerRecBounds;
										if (pJsonLayerInfo->GetValue(SCPS_JSON_POINTCLOUD_LAYERBOUNDS, layerRecBounds))
										{
											m_mapLayerNameRecBounds[strLayerName] = layerRecBounds;
											m_vecPointCloudLayer.push_back(strLayerName);
										}
									}
								}
							}
						}
						if (pJsonLayerInfo != NULL)
						{
							delete pJsonLayerInfo;
							pJsonLayerInfo = NULL;
						}
					}

					if (pJsonPointCloudLayers != NULL)
					{
						delete pJsonPointCloudLayers;
						pJsonPointCloudLayers = NULL;
					}
				}
				else
				{
					wstring strValue;
					if (pJsonExtensions->GetValue(strKey, strValue))
					{
						m_mapExtensions[strKey] = strValue;
					}
				}
			}
		}

		if (pJsonPosition != NULL)
		{
			delete pJsonPosition;
			pJsonPosition = NULL;
		}

		if (pJsonHeightRange != NULL)
		{
			delete pJsonHeightRange;
			pJsonHeightRange = NULL;
		}

		if (pJsonWDescript != NULL)
		{
			delete pJsonWDescript;
			pJsonWDescript = NULL;
		}

		if (pJsonTileInfos != NULL)
		{
			delete pJsonTileInfos;
			pJsonTileInfos = NULL;
		}

		if (pJsonExtensions != NULL)
		{
			delete pJsonExtensions;
			pJsonExtensions = NULL;
		}
		return true;
	}

	bool S3MBConfig::WriteFile(const wstring& strS3MBProfile)
	{
		JsonValue jsonValue(objectValue);
		SaveToJson(jsonValue);

		jsonValue.SaveToFile(strS3MBProfile);
		return true;
	}

	void S3MBConfig::SaveToJson(JsonValue& jsonHeader)
	{
		wstring strElemValue;
		jsonHeader.Add(SCPS_JSON_ASSET, m_strAsset);
		
		Variant varVersion(m_fVersion);
		varVersion.SetPrecision(2);
		strElemValue = varVersion.ToString();
		jsonHeader.Add(SCPS_JSON_VERSION, strElemValue);

		strElemValue = ToDataTypeString(m_enDataType);
		jsonHeader.Add(SCPS_JSON_DATATYPE, strElemValue);

		strElemValue = ToPyramidSplitTypeString(m_enPyramidSplitType);
		jsonHeader.Add(SCPS_JSON_PYRAMIDSPLITTYPE, strElemValue);

		strElemValue = ToLODTypeString(m_enLODType);
		jsonHeader.Add(SCPS_JSON_LODTYPE, strElemValue);

		// 插入点
		JsonValue jsonPosition(objectValue);

		jsonPosition.Add(S3MB_JSON_X, m_pntPosition.x);
		jsonPosition.Add(S3MB_JSON_Y, m_pntPosition.y);
		jsonPosition.Add(S3MB_JSON_Z, m_pntPosition.z);

		// 添加一个标签 ， 区别插入点是否是经纬度 ， IsDegree
		wstring strUnits = m_bIsDegree ? U("Degree") : U("Meter");
		jsonPosition.Add(SCPS_JSON_UNITS, strUnits);

		jsonHeader.Add(SCPS_JSON_POSITION, jsonPosition);

		// GeoBounds
		jsonHeader.Add(SCPS_JSON_GEOBOUNDS, m_rcGeoBounds);

		// Height Range
		JsonValue jsonHeightRange(objectValue);

		jsonHeightRange.Add(SCPS_JSON_HEIGHTRANGE_MIN, m_dbHeightMin);
		jsonHeightRange.Add(SCPS_JSON_HEIGHTRANGE_MAX, m_dbHeightMax);

		jsonHeader.Add(SCPS_JSON_HEIGHTRANGE, jsonHeightRange);
		
		if (GREATER_OR_EQUAL(m_fVersion, S3MB_VERSIONV3_0_1))
		{
			// VertexAttributeDescript
			JsonValue jsonVertexAttDescripts(arrayValue);
			for (int i = 0; i < m_vecVertexAttributeDescript.size(); i++)
			{
				CategoryDescript& vertexAttDescript = m_vecVertexAttributeDescript[i];
				JsonValue jsonDescript(objectValue);
				jsonDescript.Add(SCPS_JSON_CATEGORY, vertexAttDescript.m_strCategory);

				JsonValue jsonRanges(objectValue);
				JsonValue jsonRangeMin(arrayValue);
				JsonValue jsonRangeMax(arrayValue);
				for (int j = 0; j < vertexAttDescript.m_nDimension; j++)
				{
					jsonRangeMin.Add(vertexAttDescript.m_vecRangeMin[j]);
					jsonRangeMax.Add(vertexAttDescript.m_vecRangeMax[j]);
				}
				jsonRanges.Add(SCPS_JSON_DIMENSION, vertexAttDescript.m_nDimension);
				jsonRanges.Add(SCPS_JSON_RANGE_MIN, jsonRangeMin);
				jsonRanges.Add(SCPS_JSON_RANGE_MAX, jsonRangeMax);
				jsonDescript.Add(SCPS_JSON_RANGE, jsonRanges);

				jsonVertexAttDescripts.Add(jsonDescript);
			}
			jsonHeader.Add(SCPS_JSON_VERTEXATTRIBUTEDESCRIPT, jsonVertexAttDescripts);
		}
		else
		{
			// WDescript
			JsonValue jsonWDescript(objectValue);
			jsonWDescript.Add(SCPS_JSON_WCATEGORY, m_strWCategory);

			JsonValue jsonRange(objectValue);

			jsonRange.Add(SCPS_JSON_WRANGE_MIN, m_dbWMin);
			jsonRange.Add(SCPS_JSON_WRANGE_MAX, m_dbWMax);

			jsonWDescript.Add(SCPS_JSON_WRANGE, jsonRange);
			jsonHeader.Add(SCPS_JSON_WDESCRIPT, jsonWDescript);
		}

		// 坐标系
#ifdef WIN32
#if _MSC_VER > 1600
        strElemValue = SCPS_JSON_PRJCOORD_EPSG + to_wstring(m_prjCoordEPSG);
#else
        strElemValue = SCPS_JSON_PRJCOORD_EPSG + to_wstring((unsigned long long)m_prjCoordEPSG);
#endif
#else
#if _MSC_VER > 1600
        stringstream ss;
        ss << m_prjCoordEPSG;
        string s1 = ss.str();
        strElemValue = SCPS_JSON_PRJCOORD_EPSG + StringUtil::UTF8_to_UNICODE(s1);
#else
        std::stringstream ss;
        ss << (unsigned long long)m_prjCoordEPSG;
        string s1 = ss.str();
        strElemValue = SCPS_JSON_PRJCOORD_EPSG + StringUtil::UTF8_to_UNICODE(s1);
#endif
#endif
		jsonHeader.Add(SCPS_JSON_PRJCOORD, strElemValue);

		// TileInfos
		JsonValue jsonTileInfos(arrayValue);

		for (int i = 0; i < m_vecRootNames.size(); i++)
		{
			JsonValue jsonTileInfo(objectValue);

			// url
			strElemValue = m_vecRootNames[i];
			jsonTileInfo.Add(SCPS_JSON_TILE_URL, strElemValue);
			// bbox
			if (GREATER_OR_EQUAL(m_fVersion, S3MB_VERSIONV3_0_1))
			{
				JsonValue jsonBBox(objectValue);
				Vector3d vecCenter, vecXExtent, vecYExtent, vecZExtent;
				if (i < m_vecOBB.size())
				{
					vecCenter = m_vecOBB[i].GetCenter();
					Matrix3d matHalfAxes = m_vecOBB[i].GetHalfAxes();
					vecXExtent = matHalfAxes.GetColumn(0);
					vecYExtent = matHalfAxes.GetColumn(1);
					vecZExtent = matHalfAxes.GetColumn(2);
				}

				jsonBBox.Add(SCPS_JSON_TILE_BBOX_CENTER, vecCenter);
				jsonBBox.Add(SCPS_JSON_TILE_BBOX_XEXTENT, vecXExtent);
				jsonBBox.Add(SCPS_JSON_TILE_BBOX_YEXTENT, vecYExtent);
				jsonBBox.Add(SCPS_JSON_TILE_BBOX_ZEXTENT, vecZExtent);
				jsonTileInfo.Add(SCPS_JSON_TILE_BBOX, jsonBBox);
			}
			else
			{
				jsonTileInfo.Add(SCPS_JSON_TILE_BBOX, m_vecBBox[i]);
			}

			jsonTileInfos.Add(jsonTileInfo);
		}

		wstring strTilesKey = (GREATER_OR_EQUAL(m_fVersion, S3MB_VERSIONV3_0_1)) ? SCPS_JSON_ROOTTILES : SCPS_JSON_TILES;
		jsonHeader.Add(strTilesKey, jsonTileInfos);

		// Extensions
		JsonValue jsonExtensions(objectValue);

		if (m_prjCoordEPSG == 0)
		{
			wstring strPrj = U("<sml:CoordinateReferenceSystem>\r\n<sml:Name>Planar Coordinate System---m</sml:Name>\r\n<sml:Type>0</sml:Type>\r\n<sml:EPSGCode>0</sml:EPSGCode>\r\n<sml:Units>METER</sml:Units>\r\n<sml:DistUnits>METER</sml:DistUnits>\r\n</sml:CoordinateReferenceSystem>\r\n");
			jsonExtensions.Add(SCPS_JSON_PRJCOORD, strPrj);
		}

		std::map<wstring, wstring>::iterator it;
		for (it = m_mapExtensions.begin(); it != m_mapExtensions.end(); it++)
		{
			jsonExtensions.Add(it->first, it->second);
		}

		// PointCloudLayers
		if (m_mapLayerNameRecBounds.size() > 0)
		{
			JsonValue jsonPointCloudLayers(arrayValue);

			std::map<wstring, Rect2D>::iterator ito = m_mapLayerNameRecBounds.begin();
			for (; ito != m_mapLayerNameRecBounds.end(); ito++)
			{
				JsonValue jsonLayerInfo(objectValue);
				wstring strLayerName = ito->first;
				Rect2D layRecBounds = ito->second;
				jsonLayerInfo.Add(SCPS_JSON_POINTCLOUD_LAYERNAME, strLayerName);
				jsonLayerInfo.Add(SCPS_JSON_POINTCLOUD_LAYERBOUNDS, layRecBounds);
				jsonPointCloudLayers.Add(jsonLayerInfo);
			}

			jsonExtensions.Add(SCPS_JSON_POINTCLOUD_LAYERS, jsonPointCloudLayers);
		}

		jsonHeader.Add(SCPS_JSON_EXTENSIONS, jsonExtensions);
	}

	wstring S3MBConfig::ToDataTypeString(const SCPSDataType& enDataType)
	{
		wstring strResult = SCPS_UNKNOWN;
		switch (enDataType)
		{
		case SDT_RealityMesh:
			strResult = SCPS_DATATYPE_RealityMesh;
			break;
		case SDT_BIM:
			strResult = SCPS_DATATYPE_BIM;
			break;
		case SDT_PointCloud:
			strResult = SCPS_DATATYPE_PNTCLOUD;
			break;
		case SDT_Vector:
			strResult = SCPS_DATATYPE_VECTOR;
			break;
		case SDT_Network:
			strResult = SCPS_DATATYPE_NETWORK;
			break;
		case SDT_InstanceModel:
			strResult = SCPS_DATATYPE_INSTANCEMODEL;
			break;
		case SDT_ArtificialModel:
			strResult = SCPS_DATATYPE_ARTIFICIALMODEL;
			break;
		default:
			break;
		}
		return strResult;
	}

	SCPSDataType S3MBConfig::ToDataType(const wstring& strDataType)
	{
		SCPSDataType enType = SDT_Unknown;
		if (StringUtil::CompareNoCase(SCPS_DATATYPE_RealityMesh, strDataType))
		{
			enType = SDT_RealityMesh;
		}
		else if (StringUtil::CompareNoCase(SCPS_DATATYPE_BIM, strDataType))
		{
			enType = SDT_BIM;
		}
		else if (StringUtil::CompareNoCase(SCPS_DATATYPE_PNTCLOUD, strDataType))
		{
			enType = SDT_PointCloud;
		}
		else if (StringUtil::CompareNoCase(SCPS_DATATYPE_VECTOR, strDataType))
		{
			enType = SDT_Vector;
		}
		else if (StringUtil::CompareNoCase(SCPS_DATATYPE_NETWORK, strDataType))
		{
			enType = SDT_Network;
		}
		else if (StringUtil::CompareNoCase(SCPS_DATATYPE_INSTANCEMODEL, strDataType))
		{
			enType = SDT_InstanceModel;
		}
		else if (StringUtil::CompareNoCase(SCPS_DATATYPE_ARTIFICIALMODEL, strDataType))
		{
			enType = SDT_ArtificialModel;
		}
		return enType;
	}

	wstring S3MBConfig::ToPyramidSplitTypeString(const SCPSPyramidSplitType& enType)
	{
		wstring strResult = SCPS_UNKNOWN;
		switch (enType)
		{
		case SSP_Octree:
			strResult = SCPS_PYRAMIDSPLITTYPE_OCTREE;
			break;
		case SSP_QuadTree:
			strResult = SCPS_PYRAMIDSPLITTYPE_QUADTREE;
			break;
		default:
			break;
		}
		return strResult;
	}

	SCPSPyramidSplitType S3MBConfig::ToPyramidSplitType(const wstring& strType)
	{
		SCPSPyramidSplitType enType = SSP_Unknown;
		if (StringUtil::CompareNoCase(SCPS_PYRAMIDSPLITTYPE_OCTREE, strType))
		{
			enType = SSP_Octree;
		}
		else if (StringUtil::CompareNoCase(SCPS_PYRAMIDSPLITTYPE_QUADTREE, strType))
		{
			enType = SSP_QuadTree;
		}
		return enType;
	}

	wstring S3MBConfig::ToLODTypeString(const SCPSLODType& enType)
	{
		wstring strResult = SCPS_UNKNOWN;
		switch (enType)
		{
		case SLT_Add:
			strResult = SCPS_LODTYPE_ADD;
			break;
		case SLT_Replace:
			strResult = SCPS_LODTYPE_REPLACE;
			break;
		default:
			break;
		}
		return strResult;
	}

	SCPSLODType S3MBConfig::ToLODType(const wstring& strType)
	{
		SCPSLODType enType = SLT_Unknown;
		if (StringUtil::CompareNoCase(SCPS_LODTYPE_ADD, strType))
		{
			enType = SLT_Add;
		}
		else if (StringUtil::CompareNoCase(SCPS_LODTYPE_REPLACE, strType))
		{
			enType = SLT_Replace;
		}
		return enType;
	}

	std::wstring S3MBConfig::FromTextureCompressionType(const TextureCompressType& nType)
	{
		std::wstring strType;
		if (nType == TextureCompressType::TC_NONE)
		{
			strType = SCPS_TEXTURECOMPRESSIONTYPE_NONE;
		}
		else if (nType == TextureCompressType::TC_DXT5)
		{
			strType = SCPS_TEXTURECOMPRESSIONTYPE_DXT;
		}
		else if (nType == TextureCompressType::TC_WEBP)
		{
			strType = SCPS_TEXTURECOMPRESSIONTYPE_WEBP;
		}
		else if (nType == TextureCompressType::TC_CRN)
		{
			strType = SCPS_TEXTURECOMPRESSIONTYPE_CRN;
		}
		else if (nType == TextureCompressType::TC_KTX2)
		{
			strType = SCPS_TEXTURECOMPRESSIONTYPE_KTX2;
		}
		else
		{
			assert(false);
		}
		return strType;
	}

	TextureCompressType S3MBConfig::ToTextureCompressionType(const wstring& strType)
	{
		TextureCompressType nType = TextureCompressType::TC_NONE;
		if (StringUtil::CompareNoCase(strType, SCPS_TEXTURECOMPRESSIONTYPE_NONE))
		{
			// 不需要处理
		}
		else if (StringUtil::CompareNoCase(strType, SCPS_TEXTURECOMPRESSIONTYPE_DXT))
		{
			nType = TextureCompressType::TC_DXT5;
		}
		else if (StringUtil::CompareNoCase(strType, SCPS_TEXTURECOMPRESSIONTYPE_WEBP))
		{
			nType = TextureCompressType::TC_WEBP;
		}
		else if (StringUtil::CompareNoCase(strType, SCPS_TEXTURECOMPRESSIONTYPE_CRN))
		{
			nType = TextureCompressType::TC_CRN;
		}
		else if (StringUtil::CompareNoCase(strType, SCPS_TEXTURECOMPRESSIONTYPE_KTX2))
		{
			nType = TextureCompressType::TC_KTX2;
		}
		else
		{
			assert(false);
		}
		return nType;
	}

	std::wstring S3MBConfig::FromVertexCompressionType(const S3MBVertexTag& eType)
	{
		std::wstring strType;
		if (eType == SV_Standard)
		{
			strType = SCPS_VERTEXCOMPRESSIONTYPE_NONE;
		}
		else if (eType == SV_DracoCompressed)
		{
			strType = SCPS_VERTEXCOMPRESSIONTYPE_DRACO;
		}
		else
		{
			assert(false);
		}
		return strType;
	}

	S3MBVertexTag S3MBConfig::ToVertexCompressionType(const wstring& strType)
	{
		S3MBVertexTag eType = SV_Standard;
		if (StringUtil::CompareNoCase(strType, SCPS_VERTEXCOMPRESSIONTYPE_NONE))
		{
			// 不需要处理
		}
		else if (StringUtil::CompareNoCase(strType, SCPS_VERTEXCOMPRESSIONTYPE_DRACO))
		{
			eType = SV_DracoCompressed;
		}
		else
		{
			assert(false);
		}
		return eType;
	}

	void S3MBConfig::SetAsset(const wstring& strAsset)
	{
		m_strAsset = strAsset;
	}

	wstring S3MBConfig::GetAsset()
	{
		return m_strAsset;
	}

	void S3MBConfig::SetVersion(const float& version)
	{
		m_fVersion = version;
	}

	float S3MBConfig::GetVersion()
	{
		return m_fVersion;
	}

	void S3MBConfig::SetSplitType(const SCPSPyramidSplitType& splitType)
	{
		m_enPyramidSplitType = splitType;
	}

	SCPSPyramidSplitType S3MBConfig::GetSplitType()
	{
		return m_enPyramidSplitType;
	}

	void S3MBConfig::SetDataType(const SCPSDataType& dataType)
	{
		m_enDataType = dataType;
	}

	SCPSDataType S3MBConfig::GetDataType()
	{
		return m_enDataType;
	}

	void S3MBConfig::SetLodType(const SCPSLODType& lodType)
	{
		m_enLODType = lodType;
	}

	SCPSLODType S3MBConfig::GetLodType()
	{
		return m_enLODType;
	}

	void S3MBConfig::SetBounds(const Rect2D& bounds)
	{
		m_rcGeoBounds = bounds;
	}

	Rect2D S3MBConfig::GetBounds()
	{
		return m_rcGeoBounds;
	}

	void S3MBConfig::SetHeightMax(const double& dMax)
	{
		m_dbHeightMax = dMax;
	}

	double S3MBConfig::GetHeightMax()
	{
		return m_dbHeightMax;
	}

	void S3MBConfig::SetHeightMin(const double& dMin)
	{
		m_dbHeightMin = dMin;
	}

	double S3MBConfig::GetHeightMin()
	{
		return m_dbHeightMin;
	}

	void S3MBConfig::SetPosition(const Point3D& position)
	{
		m_pntPosition = position;
	}

	Point3D S3MBConfig::GetPosition()
	{
		return m_pntPosition;
	}

	void S3MBConfig::SetEPSGCode(const int& nEpsg)
	{
		m_prjCoordEPSG = nEpsg;
	}

	int S3MBConfig::GetEPSGCode()
	{
		return m_prjCoordEPSG;
	}

	void S3MBConfig::SetIsDegree(const bool& isDegree)
	{
		m_bIsDegree = isDegree;
	}

	bool S3MBConfig::GetIsDegree()
	{
		return m_bIsDegree;
	}

	void S3MBConfig::SetVertexAttributeDescripts(std::vector<CategoryDescript>& vecDescript)
	{
		m_vecVertexAttributeDescript.clear();
		m_vecVertexAttributeDescript = vecDescript;
	}

	void S3MBConfig::GetVertexAttributeDescripts(std::vector<CategoryDescript>& vecDescript)
	{
		vecDescript.clear();
		vecDescript = m_vecVertexAttributeDescript;
	}
	void S3MBConfig::SetExtension(const wstring & strKey, const wstring & strValue)
	{
		m_mapExtensions[strKey] = strValue;
	}
	bool S3MBConfig::HasExtension(const wstring & strKey)
	{
		if (m_mapExtensions.find(strKey) != m_mapExtensions.end())
		{
			return true;
		}
		return false;
	}
	wstring S3MBConfig::GetExtension(const wstring & strKey)
	{
		if (m_mapExtensions.find(strKey) != m_mapExtensions.end())
		{
			return m_mapExtensions[strKey];
		}
		return U("");
	}
}

