#include "I3SParser.h"
#include "Utils/Utils.h"

#include <assert.h>
#include <set>

#define I3S_PRE_MAT U("mat_")
#define I3S_PRE_MAT2 U("Mat")
#define I3S_PRE_SUB U("sub")

#define I3S_EXT_JSONGZ U(".json.gz")

#define I3S_FILE_METADATA U("metadata.json")
#define I3S_FILE_3DSCENELAYER U("3dSceneLayer.json.gz")
#define I3S_FILE_3DNODEINDEXDOCUMENT U("3dNodeIndexDocument.json.gz")
#define I3S_FILE_SHAREDRESOURCE U("sharedResource.json.gz")

#define I3S_LAYERTYPE_INTEGRATEDMESH U("IntegratedMesh")
#define I3S_LAYERTYPE_BUILDING U("Building")
#define I3S_LAYERTYPE_GROUP U("group")
#define I3S_LAYERTYPE_3DOBJECT U("3DObject")
#define I3S_LAYERTYPE_POINTCLOUD U("PointCloud")
#define I3S_LAYERTYPE_POINT U("Point")

#define I3S_JSON_ID U("id")
#define I3S_JSON_KEY U("key")
#define I3S_JSON_NAME U("name")
#define I3S_JSON_TYPE U("type")
#define I3S_JSON_ALIAS U("alias")
#define I3S_JSON_DOMAIN U("domain")
#define I3S_JSON_CODE U("code")
#define I3S_JSON_METADATA_NODECOUNT U("nodeCount")
#define I3S_JSON_LAYERTYPE U("layerType")
#define I3S_JSON_SUBLAYERS U("sublayers")
#define I3S_JSON_NODES U("nodes")
#define I3S_JSON_NODEPAGES U("nodepages")
#define I3S_JSON_CHILDREN U("children")
#define I3S_JSON_HREF U("href")
#define I3S_JSON_PARAMS U("params")
#define I3S_JSON_POSITION U("position")
#define I3S_JSON_NORMAL U("normal")
#define I3S_JSON_UV0 U("uv0")
#define I3S_JSON_COLOR U("color")
#define I3S_JSON_ITEMS U("items")
#define I3S_JSON_FORMATINFOS U("formatInfos")
#define I3S_JSON_SYMBOLLAYERS U("symbolLayers")
#define I3S_JSON_TILT U("tilt")
#define I3S_JSON_ROLL U("roll")
#define I3S_JSON_HEADING U("heading")
#define I3S_JSON_RESOURCE U("resource")
#define I3S_JSON_POINTS U("points")
#define I3S_JSON_VALUE U("value")
#define I3S_JSON_TRANSPARENCY U("transparency")
#define I3S_JSON_COLORMIXMODE U("colorMixMode")
#define I3S_JSON_COLORMIXMODE_MULTIPLY U("multiply")
#define I3S_JSON_COLORMIXMODE_REPLACE U("replace")
#define I3S_JSON_SYMBOLTYPE_POINT U("PointSymbol3D")
#define I3S_JSON_SYMBOLTYPE_STYLEREF U("styleSymbolReference")
#define I3S_JSON_RENDERERTYPE_SIMPLE U("simple")
#define I3S_JSON_RENDERERTYPE_UNIQUEVALUE U("uniqueValue")
#define I3S_JSON_MATERIAL U("material")
#define I3S_JSON_NONCOMPRESSED_UVREGION U("region")
#define I3S_JSON_COMPRESSED_UVREGION U("uv-region")
#define I3S_JSON_COMPRESSED_FEATUREID U("feature-index")
#define I3S_JSON_XMIN U("xmin")
#define I3S_JSON_XMAX U("xmax")
#define I3S_JSON_YMIN U("ymin")
#define I3S_JSON_YMAX U("ymax")
#define I3S_JSON_ZMIN U("zmin")
#define I3S_JSON_ZMAX U("zmax")
#define I3S_JSON_OBB U("obb")
#define I3S_JSON_OBB_CENTER U("center")
#define I3S_JSON_OBB_HALFSIZE U("halfSize")
#define I3S_JSON_OBB_QUATERNION U("quaternion")
#define I3S_JSON_ENCODING U("encoding")
#define I3S_JSON_ENCODING_DRACO U("draco")
#define I3S_JSON_ENCODING_LEPCCXYZ U("lepcc-xyz")
#define I3S_JSON_ATTRIBUTES U("attributes")
#define I3S_JSON_SCENELAYER_FULLEXTENT U("fullExtent")
#define I3S_JSON_SCENELAYER_SPATIALREFERENCE U("spatialReference")
#define I3S_JSON_SCENELAYER_WKID U("wkid")
#define I3S_JSON_SCENELAYER_LATESTWKID U("latestWkid")
#define I3S_JSON_SCENELAYER_WKT U("wkt")
#define I3S_JSON_SCENELAYER_ATTSTORAGEINFO U("attributeStorageInfo")
#define I3S_JSON_SCENELAYER_DRAWINGINFO U("drawingInfo")
#define I3S_JSON_SCENELAYER_RENDERER U("renderer")
#define I3S_JSON_SCENELAYER_FIELD1 U("field1")
#define I3S_JSON_SCENELAYER_DEFAULTSYMBOL U("defaultSymbol")
#define I3S_JSON_SCENELAYER_UNIQUEVALUEINFOS U("uniqueValueInfos")
#define I3S_JSON_SCENELAYER_SYMBOL U("symbol")
#define I3S_JSON_SCENELAYER_STYLEURL U("styleUrl")
#define I3S_JSON_SCENELAYER_FIELDS U("fields")
#define I3S_JSON_SCENELAYER_CODEDVALUES U("codedValues")
#define I3S_JSON_SCENELAYER_STORE U("store")
#define I3S_JSON_SCENELAYER_PROFILE U("profile")
#define I3S_JSON_SCENELAYER_INDEX U("index")
#define I3S_JSON_SCENELAYER_NODESPERPAGE U("nodesPerPage")
#define I3S_JSON_SCENELAYER_METRICTYPE U("lodSelectionMetricType")
#define I3S_JSON_SCENELAYER_DENSITYTHRESHOLD U("density-threshold")
#define I3S_JSON_SCENELAYER_ROOTNODE U("rootNode")
#define I3S_JSON_SCENELAYER_EXTENT U("extent")
#define I3S_JSON_SCENELAYER_GEOMETRYSCHEMA U("defaultGeometrySchema")
#define I3S_JSON_SCENELAYER_ORDER U("ordering")
#define I3S_JSON_SCENELAYER_FEATUREATTORDER U("featureAttributeOrder")
#define I3S_JSON_SCENELAYER_FACERANGE U("faceRange")
#define I3S_JSON_SCENELAYER_TEXSETDEFS U("textureSetDefinitions")
#define I3S_JSON_SCENELAYER_FORMATS U("formats")
#define I3S_JSON_SCENELAYER_FORMAT U("format")
#define I3S_JSON_SCENELAYER_GEOMETRYDEFS U("geometryDefinitions")
#define I3S_JSON_SCENELAYER_GEOMETRYBUFFERS U("geometryBuffers")
#define I3S_JSON_SCENELAYER_COMPATT U("compressedAttributes")
#define I3S_JSON_NODEINDEXDOC_LODSELECTION U("lodSelection")
#define I3S_JSON_NODEINDEXDOC_METRICTYPE U("metricType")
#define I3S_JSON_NODEINDEXDOC_MAXSCREENTHRESHOLD U("maxScreenThreshold")
#define I3S_JSON_NODEINDEXDOC_MAXERROR U("maxError")
#define I3S_JSON_NODEINDEXDOC_GEOMETRYDATA U("geometryData")
#define I3S_JSON_NODEINDEXDOC_TEXTUREDATA U("textureData")
#define I3S_JSON_NODEINDEXDOC_FEATUREDATA U("featureData")
#define I3S_JSON_NODEINDEXDOC_ATTRIBUTEDATA U("attributeData")
#define I3S_JSON_NODEINDEXDOC_SHAREDRESOURCE U("sharedResource")
#define I3S_JSON_SHAREDRES_MATDEFS U("materialDefinitions")
#define I3S_JSON_SHAREDRES_TEXDEFS U("textureDefinitions")
#define I3S_JSON_SHAREDRES_WRAP U("wrap")
#define I3S_JSON_SHAREDRES_IMAGES U("images")
#define I3S_JSON_SHAREDRES_VERTEXREGIONS U("vertexRegions")
#define I3S_JSON_SHAREDRES_SHININESS U("shininess")
#define I3S_JSON_SHAREDRES_AMBIENT U("ambient")
#define I3S_JSON_SHAREDRES_DIFFUSE U("diffuse")
#define I3S_JSON_SHAREDRES_SPECULAR U("specular")
#define I3S_JSON_SHAREDRES_CULLFACE U("cullFace")
#define I3S_JSON_SHAREDRES_CULLFACE_NONE U("none")
#define I3S_JSON_METADATA_NODECOUNT U("nodeCount")
#define I3S_JSON_NODEPAGE_RESOURCEID U("resourceId")
#define I3S_JSON_NODEPAGE_FIRSTCHILD U("firstChild")
#define I3S_JSON_NODEPAGE_CHILDCOUNT U("childCount")
#define I3S_JSON_NODEPAGE_VERTEXCOUNT U("vertexCount")
#define I3S_JSON_NODEPAGE_LODTHRESHOLD U("lodThreshold")

#define I3S_FIELDTYPE_OID U("esriFieldTypeOID")
#define I3S_FIELDTYPE_SMALLINTEGER U("esriFieldTypeSmallInteger")
#define I3S_FIELDTYPE_INTEGER U("esriFieldTypeInteger")
#define I3S_FIELDTYPE_SINGLE U("esriFieldTypeSingle")
#define I3S_FIELDTYPE_DOUBLE U("esriFieldTypeDouble")
#define I3S_FIELDTYPE_STRING U("esriFieldTypeString")
#define I3S_FIELDTYPE_DATE U("esriFieldTypeDate")

namespace S3MB
{
	I3SParser::I3SParser()
	{
	}

	I3SParser::~I3SParser()
	{
		Close();
	}

	bool I3SParser::Open(const I3SParams& params)
	{
		m_I3SParams = params;

		m_slpkFile.Close();
		return m_slpkFile.Open(params.GetInputPath());
	}

	bool I3SParser::Close()
	{
		m_vecI3SLayerInfo.clear();
		m_mapI3SParseParams.clear();
		m_mapS3MBConfig.clear();
		return m_slpkFile.Close();
	}

	const I3SParams& I3SParser::GetParams() const
	{
		return m_I3SParams;
	}

	const std::vector<I3SLayerInfo>& I3SParser::GetLayerInfos() const
	{
		return m_vecI3SLayerInfo;
	}

	const std::map<std::wstring, I3SParseParams>& I3SParser::GetParseParams() const
	{
		return m_mapI3SParseParams;
	}

	const std::map<std::wstring, S3MBConfig>& I3SParser::GetS3MBConfigs() const
	{
		return m_mapS3MBConfig;
	}

	bool I3SParser::ParseConfig()
	{
		I3SMetadata metaData;
		ParseMetadata(I3S_FILE_METADATA, metaData);
		return Parse3dSceneLayer(I3S_FILE_3DSCENELAYER, U(""), metaData);
	}

	bool I3SParser::ParsePointCloudNodePages(SlpkFile& slpkFile, const I3SParseParams& parseParams, 
		std::map<std::wstring, I3SNodeInfo>& mapNodeInfo)
	{
		std::wstring strNodePagesDir = parseParams.GetLayerPath() + I3S_JSON_NODEPAGES + U("/");
		const I3SIndexInfo& indexInfo = parseParams.GetIndexInfo();
		unsigned int nPageNum = indexInfo.GetPageNum();
		for (unsigned int i = 0; i < nPageNum; i++)
		{
#ifdef WIN32
			std::wstring strNodePageTitle = StringUtil::Format(U("%d"), i);
#else
            std::wstring strNodePageTitle = StringUtil::Format(U("%d").c_str(), i);
#endif
			std::wstring strNodePagePath = strNodePagesDir + strNodePageTitle + I3S_EXT_JSONGZ;
			ParsePointCloudNodePage(slpkFile, parseParams, strNodePagePath, mapNodeInfo);
		}

		return true;
	}

	bool I3SParser::ParsePointCloudNodePage(SlpkFile& slpkFile, const I3SParseParams& parseParams, 
		const std::wstring& strName, std::map<std::wstring, I3SNodeInfo>& mapNodeInfo)
	{
		std::wstring strPageContent;
		if (!slpkFile.GetString(strName, strPageContent))
		{
			return false;
		}

		JsonValue doc;
		if (!doc.LoadValueFromString(strPageContent))
		{
			return false;
		}

		JsonValue jsonNodes;
		doc.GetValue(I3S_JSON_NODES, jsonNodes);
		for (unsigned int i = 0; i < jsonNodes.GetArraySize(); i++)
		{
			I3SNodeInfo nodeInfo;
			JsonValue jsonNode;
			jsonNodes.GetAt(i, &jsonNode);

			std::wstring strId;
			int nResourceId = 0;
			jsonNode.GetValue(I3S_JSON_NODEPAGE_RESOURCEID, nResourceId);
#ifdef WIN32
			strId = StringUtil::Format(U("%d"), nResourceId);
#else
            strId = StringUtil::Format(U("%d").c_str(), nResourceId);
#endif
			nodeInfo.SetId(strId);

			std::wstring strNodePath = (parseParams.GetLayerPath() + I3S_JSON_NODES + U("/") + strId);
			std::wstring strNodesDir = StringUtil::GetDir(strNodePath);

			//geometry
			std::vector<std::wstring> vecGeo;
			std::wstring strGeoPath = strNodePath + U("/geometries/0");
			vecGeo.push_back(strGeoPath);
			nodeInfo.SetGeometries(vecGeo);

			//obb
			OrientedBoundingBox obb;
			JsonValue jsonObb;
			if (jsonNode.GetValue(I3S_JSON_OBB, jsonObb))
			{
				ParseOBB(jsonObb, obb);
			}
			nodeInfo.SetObb(obb);

			//lodThreshold
			double dThreshold = 0.0;
			jsonNode.GetValue(I3S_JSON_NODEPAGE_LODTHRESHOLD, dThreshold);
			double dRangeValue = sqrt(dThreshold / MATH_PI / 0.25);
			nodeInfo.SetRangeValue(dRangeValue);

			//children
			int nFirstChild = 0, nChildCount = 0;
			std::vector<std::wstring> vecChildNode;
			jsonNode.GetValue(I3S_JSON_NODEPAGE_FIRSTCHILD, nFirstChild);
			jsonNode.GetValue(I3S_JSON_NODEPAGE_CHILDCOUNT, nChildCount);
			for (unsigned int iChild = 0; iChild < nChildCount; iChild++)
			{
#ifdef WIN32
				std::wstring strChildNodePath = StringUtil::Format(U("%d"), nFirstChild + iChild);
#else
                std::wstring strChildNodePath = StringUtil::Format(U("%d").c_str(), nFirstChild + iChild);
#endif
				strChildNodePath = strNodesDir + strChildNodePath;
				vecChildNode.push_back(strChildNodePath);
			}
			nodeInfo.SetChildNodes(vecChildNode);
			mapNodeInfo[strId] = nodeInfo;
		}

		return true;
	}

	bool I3SParser::ParseNodeDoc(SlpkFile& slpkFile, const I3SParseParams& parseParams, 
		const std::wstring& strName, I3SNodeInfo& nodeInfo)
	{
		std::wstring strNodeDoc;
		if (!slpkFile.GetString(strName, strNodeDoc))
		{
			return false;
		}

		JsonValue doc;
		if (!doc.LoadValueFromString(strNodeDoc))
		{
			return false;
		}

		std::wstring strNodeDir = StringUtil::GetDir(strName);

		std::wstring strId;
		doc.GetValue(I3S_JSON_ID, strId);
		nodeInfo.SetId(strId);

		OrientedBoundingBox obb;
		JsonValue jsonObb;
		doc.GetValue(I3S_JSON_OBB, jsonObb);
		ParseOBB(jsonObb, obb);
		nodeInfo.SetObb(obb);

		JsonValue jsonLodSel;
		doc.GetValue(I3S_JSON_NODEINDEXDOC_LODSELECTION, jsonLodSel);
		unsigned int nSelSize = jsonLodSel.GetArraySize();
		for (unsigned int i = 0; i < nSelSize; i++)
		{
			JsonValue jsonElem;
			jsonLodSel.GetAt(i, &jsonElem);
			std::wstring strMetricType;
			jsonElem.GetValue(I3S_JSON_NODEINDEXDOC_METRICTYPE, strMetricType);
			if (StringUtil::CompareNoCase(strMetricType, I3S_JSON_NODEINDEXDOC_MAXSCREENTHRESHOLD))
			{
				double dRangeValue = 0.0;
				jsonElem.GetValue(I3S_JSON_NODEINDEXDOC_MAXERROR, dRangeValue);
				nodeInfo.SetRangeValue(dRangeValue);
			}
		}

		JsonValue jsonGeometries;
		doc.GetValue(I3S_JSON_NODEINDEXDOC_GEOMETRYDATA, jsonGeometries);
		std::vector<std::wstring> vecGeo;
		unsigned int nGeoSize = jsonGeometries.GetArraySize();
		for (unsigned int i = 0; i < nGeoSize; i++)
		{
			JsonValue jsonGeo;
			jsonGeometries.GetAt(i, &jsonGeo);
			std::wstring strHref;
			jsonGeo.GetValue(I3S_JSON_HREF, strHref);
			if (!strHref.empty())
			{
				std::wstring strGeoPath = StringUtil::GetAbsolutePath(strNodeDir, strHref);
				vecGeo.push_back(strGeoPath);
			}
		}
		nodeInfo.SetGeometries(vecGeo);

		JsonValue jsonTextures;
		doc.GetValue(I3S_JSON_NODEINDEXDOC_TEXTUREDATA, jsonTextures);
		std::vector<std::wstring> vecTex;
		unsigned int nTexSize = jsonTextures.GetArraySize();
		for (unsigned int i = 0; i < nTexSize; i++)
		{
			JsonValue jsonTex;
			jsonTextures.GetAt(i, &jsonTex);
			std::wstring strHref;
			jsonTex.GetValue(I3S_JSON_HREF, strHref);
			if (!strHref.empty())
			{
				std::wstring strTexPath = StringUtil::GetAbsolutePath(strNodeDir, strHref);
				vecTex.push_back(strTexPath);
			}
		}
		nodeInfo.SetTextures(vecTex);

		JsonValue jsonFeatures;
		doc.GetValue(I3S_JSON_NODEINDEXDOC_FEATUREDATA, jsonFeatures);
		std::vector<std::wstring> vecFea;
		unsigned int nFeaSize = jsonFeatures.GetArraySize();
		for (unsigned int i = 0; i < nFeaSize; i++)
		{
			JsonValue jsonFea;
			jsonFeatures.GetAt(i, &jsonFea);
			std::wstring strHref;
			jsonFea.GetValue(I3S_JSON_HREF, strHref);
			if (!strHref.empty())
			{
				std::wstring strFeaPath = StringUtil::GetAbsolutePath(strNodeDir, strHref);
				vecFea.push_back(strFeaPath);
			}
		}
		nodeInfo.SetFeatures(vecFea);

		JsonValue jsonAttributes;
		doc.GetValue(I3S_JSON_NODEINDEXDOC_ATTRIBUTEDATA, jsonAttributes);
		std::vector<std::wstring> vecAtt;
		unsigned int nAttSize = jsonAttributes.GetArraySize();
		for (unsigned int i = 0; i < nAttSize; i++)
		{
			JsonValue jsonAtt;
			jsonAttributes.GetAt(i, &jsonAtt);
			std::wstring strHref;
			jsonAtt.GetValue(I3S_JSON_HREF, strHref);
			if (!strHref.empty())
			{
				std::wstring strAttPath = StringUtil::GetAbsolutePath(strNodeDir, strHref);
				vecAtt.push_back(strAttPath);
			}
		}
		nodeInfo.SetAttributes(vecAtt);

		JsonValue jsonResource;
		doc.GetValue(I3S_JSON_NODEINDEXDOC_SHAREDRESOURCE, jsonResource);
		std::wstring strHref;
		jsonResource.GetValue(I3S_JSON_HREF, strHref);
		if (!strHref.empty())
		{
			std::wstring strResPath = StringUtil::GetAbsolutePath(strNodeDir, strHref);
			strResPath = strResPath + U("/") + I3S_FILE_SHAREDRESOURCE;
			nodeInfo.SetSharedResource(strResPath);
		}

		JsonValue jsonChildren;
		doc.GetValue(I3S_JSON_CHILDREN, jsonChildren);
		std::vector<std::wstring> vecChildNode;
		unsigned int nChildSize = jsonChildren.GetArraySize();
		for (unsigned int i = 0; i < nChildSize; i++)
		{
			JsonValue jsonChildNode;
			jsonChildren.GetAt(i, &jsonChildNode);
			std::wstring strHref;
			jsonChildNode.GetValue(I3S_JSON_HREF, strHref);
			if (!strHref.empty())
			{
				std::wstring strChildDir = StringUtil::GetAbsolutePath(strNodeDir, strHref);
				vecChildNode.push_back(strChildDir);
			}
		}
		nodeInfo.SetChildNodes(vecChildNode);

		std::vector<std::pair<std::wstring, int> > vecVexAttExtAndId = parseParams.GetVertexAttributeExtensionAndIdRelation();
		nodeInfo.SetVertexAttributeExtensionAndIdRelation(vecVexAttExtAndId);

		return true;
	}

	bool I3SParser::ParseSharedResource(SlpkFile& slpkFile, I3SNodeInfo& nodeInfo, I3SMaterialInfo& matInfo)
	{
		if (nodeInfo.GetId().empty())
		{
			return false;
		}

		std::wstring strRes;
		if (!slpkFile.GetString(nodeInfo.GetSharedResource(), strRes))
		{
			return false;
		}

		JsonValue jsonRes;
		if (!jsonRes.LoadValueFromString(strRes))
		{
			return false;
		}

		JsonValue jsonMatDefs;
		jsonRes.GetValue(I3S_JSON_SHAREDRES_MATDEFS, jsonMatDefs);
		//不同数据，材质的键值不一样
		std::wstring strDefName = I3S_PRE_MAT + nodeInfo.GetId();
		if (!jsonMatDefs.Contains(strDefName))
		{
			strDefName = I3S_PRE_MAT2 + nodeInfo.GetId();
		}

		JsonValue jsonIdInfo;
		if (jsonMatDefs.GetValue(strDefName, jsonIdInfo))
		{
			matInfo.SetName(strDefName);

			JsonValue jsonParams;
			jsonIdInfo.GetValue(I3S_JSON_PARAMS, jsonParams);
			bool bHasUVRegion = false;
			jsonParams.GetValue(I3S_JSON_SHAREDRES_VERTEXREGIONS, bHasUVRegion);
			if (!bHasUVRegion)
			{
				//不存在uv region的情况
				std::vector<std::pair<std::wstring, int> > vecVexAttExtAndId = nodeInfo.GetVertexAttributeExtensionAndIdRelation();
				std::vector<std::pair<std::wstring, int> > vecVexAttExtAndId_node;
				for (unsigned int i = 0; i < vecVexAttExtAndId.size(); i++)
				{
					std::pair<std::wstring, int> pairAttAndId = vecVexAttExtAndId[i];
					if (!StringUtil::CompareNoCase(pairAttAndId.first, ATT_UVREGION))
					{
						vecVexAttExtAndId_node.push_back(pairAttAndId);
					}
				}
				nodeInfo.SetVertexAttributeExtensionAndIdRelation(vecVexAttExtAndId_node);
			}

			float fShininess = 0.0;
			if (jsonParams.GetValue(I3S_JSON_SHAREDRES_SHININESS, fShininess))
			{
				matInfo.SetShininess(fShininess);
			}

			JsonValue jsonAmbient;
			jsonParams.GetValue(I3S_JSON_SHAREDRES_AMBIENT, jsonAmbient);
			std::vector<double> vecAmb;
			unsigned int nAmbSize = jsonAmbient.GetArraySize();
			for (unsigned int i = 0; i < nAmbSize; i++)
			{
				double dElem = 0.0;
				jsonAmbient.GetAt(i, dElem);
				vecAmb.push_back(dElem);
			}

			if (vecAmb.size() >= 3)
			{
				ColorValue color(vecAmb[0], vecAmb[1], vecAmb[2], 1.0);
				matInfo.SetAmbient(color);
			}

			JsonValue jsonDiffuse;
			jsonParams.GetValue(I3S_JSON_SHAREDRES_DIFFUSE, jsonDiffuse);
			std::vector<double> vecDif;
			unsigned int nDifSize = jsonDiffuse.GetArraySize();
			for (unsigned int i = 0; i < nDifSize; i++)
			{
				double dElem = 0.0;
				jsonDiffuse.GetAt(i, dElem);
				vecDif.push_back(dElem);
			}

			if (vecDif.size() >= 3)
			{
				ColorValue color(vecDif[0], vecDif[1], vecDif[2], 1.0);
				matInfo.SetDiffuse(color);
			}

			JsonValue jsonSpecular;
			jsonParams.GetValue(I3S_JSON_SHAREDRES_SPECULAR, jsonSpecular);
			std::vector<double> vecSpe;
			unsigned int nSpeSize = jsonSpecular.GetArraySize();
			for (unsigned int i = 0; i < nSpeSize; i++)
			{
				double dElem = 0.0;
				jsonSpecular.GetAt(i, dElem);
				vecSpe.push_back(dElem);
			}

			if (vecSpe.size() >= 3)
			{
				ColorValue color(vecSpe[0], vecSpe[1], vecSpe[2], 1.0);
				matInfo.SetSpecular(color);
			}

			std::wstring strCullFace;
			jsonParams.GetValue(I3S_JSON_SHAREDRES_CULLFACE, strCullFace);
			if (StringUtil::CompareNoCase(strCullFace, I3S_JSON_SHAREDRES_CULLFACE_NONE))
			{
				matInfo.SetCullMode(CULL_NONE);
			}
		}

		return true;
	}

	bool I3SParser::ParseMetadata(const std::wstring& strFileName, I3SMetadata& metaData)
	{
		std::wstring strMetadata;
		if (!m_slpkFile.GetString(strFileName, strMetadata))
		{
			return false;
		}

		JsonValue jsonMetadata;
		if (jsonMetadata.LoadValueFromString(strMetadata))
		{
			int nNodeCount = 0;
			jsonMetadata.GetValue(I3S_JSON_METADATA_NODECOUNT, nNodeCount);
			metaData.SetNodeCount(nNodeCount);
		}
		return true;
	}

	bool I3SParser::Parse3dSceneLayer(const std::wstring& strFileName, const std::wstring& strParentLayerType, const I3SMetadata& metaData)
	{
		std::wstring strConfig;
		if (!m_slpkFile.GetString(strFileName, strConfig))
		{
			return false;
		}

		JsonValue doc;
		doc.LoadValueFromString(strConfig);

		bool bResult = false;
		std::wstring strLayerType;
		doc.GetValue(I3S_JSON_LAYERTYPE, strLayerType);
		if (StringUtil::CompareNoCase(strLayerType, I3S_LAYERTYPE_BUILDING))
		{
			std::vector<std::wstring> vecLayerId;
			ParseSublayerIds(doc, vecLayerId);
			for (unsigned int i = 0; i < vecLayerId.size(); i++)
			{
				std::wstring strSubLayers = I3S_JSON_SUBLAYERS;
				std::wstring strSubLayerKey = strSubLayers + U("/") + vecLayerId[i] + U("/");
				// 解析元数据文件
				std::wstring strMetadataKey = strSubLayerKey + I3S_FILE_METADATA;
				I3SMetadata metaData;
				ParseMetadata(strMetadataKey, metaData);

				// 解析图层的配置文件
				std::wstring strConfigKey = strSubLayerKey + I3S_FILE_3DSCENELAYER;
				Parse3dSceneLayer(strConfigKey, strLayerType, metaData);
			}
			bResult = true;
		}
		else if (StringUtil::CompareNoCase(strLayerType, I3S_LAYERTYPE_INTEGRATEDMESH) ||
			StringUtil::CompareNoCase(strLayerType, I3S_LAYERTYPE_3DOBJECT) ||
			StringUtil::CompareNoCase(strLayerType, I3S_LAYERTYPE_POINTCLOUD) ||
			StringUtil::CompareNoCase(strLayerType, I3S_LAYERTYPE_POINT))
		{
			bResult = ParseLayerInfo(doc, strParentLayerType, metaData);
		}

		return bResult;
	}

	bool I3SParser::ParseSublayerIds(JsonValue& jsonConfig, std::vector<std::wstring>& vecLayerId)
	{
		JsonValue jsonSublayers;
		jsonConfig.GetValue(I3S_JSON_SUBLAYERS, jsonSublayers);

		unsigned int nSubLayerSize = jsonSublayers.GetArraySize();
		for (unsigned int i = 0; i < nSubLayerSize; i++)
		{
			JsonValue jsonSubLayer;
			jsonSublayers.GetAt(i, &jsonSubLayer);
			if (jsonSubLayer.Contains(I3S_JSON_SUBLAYERS))
			{
				ParseSublayerIds(jsonSubLayer, vecLayerId);
			}
			else if (jsonSubLayer.Contains(I3S_JSON_ID))
			{
				int nLayerId = 0;
				std::wstring strLayerId;
				jsonSubLayer.GetValue(I3S_JSON_ID, nLayerId);
#ifdef WIN32
				strLayerId = StringUtil::Format(U("%d"), nLayerId);
#else
                strLayerId = StringUtil::Format(U("%d").c_str(), nLayerId);
#endif
				vecLayerId.push_back(strLayerId);
			}
		}

		return true;
	}

	bool I3SParser::ParseLayerInfo(JsonValue& jsonConfig, const std::wstring& strParentLayerType, const I3SMetadata& metaData)
	{
		int nLayerId = 0;
		std::wstring strLayerId, strLayerName, strLayerType;
		jsonConfig.GetValue(I3S_JSON_ID, nLayerId);
		jsonConfig.GetValue(I3S_JSON_NAME, strLayerName);
		jsonConfig.GetValue(I3S_JSON_LAYERTYPE, strLayerType);
#ifdef WIN32
		strLayerId = StringUtil::Format(U("%d"), nLayerId);
#else
        strLayerId = StringUtil::Format(U("%d").c_str(), nLayerId);
#endif
		I3SLayerInfo layerInfo;
		layerInfo.SetId(strLayerId);
		layerInfo.SetName(strLayerName);
		layerInfo.SetType(strLayerType);
		m_vecI3SLayerInfo.push_back(layerInfo);

		S3MBConfig s3mbConfig;
		s3mbConfig.SetVersion(m_I3SParams.GetS3MVersion());

		bool bHasParentLayer = !strParentLayerType.empty();
		std::wstring strActualLayerType = bHasParentLayer ? strParentLayerType : strLayerType;
		s3mbConfig.SetDataType(ToDataType(strActualLayerType));

		std::wstring strLayerOutputPath = m_I3SParams.GetOutputPath() + strLayerName + U("/");
		std::wstring strHref;
		jsonConfig.GetValue(I3S_JSON_HREF, strHref);
		I3SParseParams parseParams;
		parseParams.SetOutputPath(strLayerOutputPath);
		if (!strHref.empty() && bHasParentLayer)
		{
			StringUtil::TrimLeftString(strHref, U("./"));
			strHref = I3S_PRE_SUB + strHref + U("/");
			parseParams.SetLayerPath(strHref);
		}

		//解析信息
		ParseFullExtent(jsonConfig, s3mbConfig);
		ParseSpatialReference(jsonConfig, parseParams, s3mbConfig);
		ParseAttributeStorageInfo(jsonConfig, parseParams);
		ParseDrawingInfo(jsonConfig, parseParams);
		ParseFields(jsonConfig, parseParams);
		ParseStore(jsonConfig, metaData, parseParams, s3mbConfig);
		ParseTextureSetDefinitions(jsonConfig, parseParams);
		ParseGeometryDefinitions(jsonConfig, parseParams);

		//唯一值外挂文件的数据，属性值需要转为原来类型
		I3SDrawingInfo drawingInfo = parseParams.GetDrawingInfo();
		if (drawingInfo.GetRendererType() == I3SDrawingInfo::RT_UniqueValue)
		{
			const std::vector<I3SAttributeInfo>& vecAttInfo = parseParams.GetAttributeInfos();
			for (unsigned int i = 0; i < vecAttInfo.size(); i++)
			{
				const I3SAttributeInfo& attInfo = vecAttInfo[i];
				if (attInfo.GetIsCoded() ||
					!StringUtil::CompareNoCase(attInfo.GetFieldName(), drawingInfo.GetFieldName()))
				{
					continue;
				}

				std::map<Variant, std::wstring> mapFieldSymbolNew;
				std::map<Variant, std::wstring>::iterator iter;
				std::map<Variant, std::wstring> mapFieldSymbol = drawingInfo.GetFieldSymbols();
				for (iter = mapFieldSymbol.begin(); iter != mapFieldSymbol.end(); iter++)
				{
					Variant varValue = ToVariant(iter->first, attInfo.GetOriginFieldType());
					mapFieldSymbolNew[varValue] = iter->second;
				}
				drawingInfo.SetFieldSymbols(mapFieldSymbolNew);
				parseParams.SetDrawingInfo(drawingInfo);
			}
		}

		//添加外挂文件路径
		if (parseParams.GetHasDrawingInfo() &&
			parseParams.GetDrawingInfo().GetSymbolType() == I3SDrawingInfo::ST_StyleReference &&
			!parseParams.GetDrawingInfo().GetSymbolPaths().empty())
		{
			s3mbConfig.SetExtension(U("instanceModelUrl"), U("./ModelFile"));
		}

		//经纬度默认是椭球
		if (m_I3SParams.GetIsLonLatCoordinates())
		{
			s3mbConfig.SetExtension(U("globeType"), U("Ellipsoid_WGS84"));
		}

		//设置文件类型
		if (s3mbConfig.GetDataType() == SDT_RealityMesh)
		{
			s3mbConfig.SetExtension(U("s3m:FileType"), U("OSGBFile"));
		}
		else if (s3mbConfig.GetDataType() == SDT_BIM ||
			s3mbConfig.GetDataType() == SDT_ArtificialModel ||
			s3mbConfig.GetDataType() == SDT_InstanceModel)
		{
			s3mbConfig.SetExtension(U("s3m:FileType"), U("OSGBCacheFile"));
		}
		else if (s3mbConfig.GetDataType() == SDT_PointCloud)
		{
			s3mbConfig.SetExtension(U("s3m:FileType"), U("PointCloudFile"));
		}

		if ((s3mbConfig.GetDataType() == SDT_RealityMesh || s3mbConfig.GetDataType() == SDT_PointCloud) &&
			parseParams.GetVertexCompressionType() == SV_DracoCompressed)
		{
			s3mbConfig.SetExtension(U("s3m:VertexCompressionType"), SCPS_VERTEXCOMPRESSIONTYPE_DRACO);
		}

		//外挂模型的点数据不做压缩
		if (s3mbConfig.GetDataType() == SDT_InstanceModel)
		{
			s3mbConfig.SetExtension(U("s3m:VertexCompressionType"), SCPS_VERTEXCOMPRESSIONTYPE_NONE);
		}

		m_mapS3MBConfig[strLayerId] = s3mbConfig;
		m_mapI3SParseParams[strLayerId] = parseParams;
		return true;
	}

	bool I3SParser::ParseFullExtent(JsonValue& jsonConfig, S3MBConfig& s3mbConfig)
	{
		JsonValue jsonFullExtent;
		if (!jsonConfig.GetValue(I3S_JSON_SCENELAYER_FULLEXTENT, jsonFullExtent))
		{
			return false;
		}

		Point3D pntPos;
		Vector3d vecMin, vecMax;
		jsonFullExtent.GetValue(I3S_JSON_XMIN, vecMin.x);
		jsonFullExtent.GetValue(I3S_JSON_XMAX, vecMax.x);
		jsonFullExtent.GetValue(I3S_JSON_YMIN, vecMin.y);
		jsonFullExtent.GetValue(I3S_JSON_YMAX, vecMax.y);
		jsonFullExtent.GetValue(I3S_JSON_ZMIN, vecMin.z);
		jsonFullExtent.GetValue(I3S_JSON_ZMAX, vecMax.z);

		pntPos.x = (vecMin.x + vecMax.x) / 2;
		pntPos.y = (vecMin.y + vecMax.y) / 2;
		pntPos.z = (vecMin.z + vecMax.z) / 2;
		s3mbConfig.SetPosition(pntPos);
		return true;
	}

	bool I3SParser::ParseSpatialReference(JsonValue& jsonConfig, I3SParseParams& parseParams, S3MBConfig& s3mbConfig)
	{
		JsonValue jsonSptlRefer;
		if (!jsonConfig.GetValue(I3S_JSON_SCENELAYER_SPATIALREFERENCE, jsonSptlRefer))
		{
			return false;
		}

		int nWkid = 0;
		std::wstring strWkt;
		if (jsonSptlRefer.GetValue(I3S_JSON_SCENELAYER_LATESTWKID, nWkid) ||
			jsonSptlRefer.GetValue(I3S_JSON_SCENELAYER_WKID, nWkid))
		{
			parseParams.SetEpsgCode(nWkid);
			s3mbConfig.SetEPSGCode(nWkid);
		}
		else if (jsonSptlRefer.GetValue(I3S_JSON_SCENELAYER_WKT, strWkt))
		{
			//ToDo
		}
		return true;
	}

	bool I3SParser::ParseAttributeStorageInfo(JsonValue& jsonConfig, I3SParseParams& parseParams)
	{
		JsonValue jsonAttStorageInfos;
		if (!jsonConfig.GetValue(I3S_JSON_SCENELAYER_ATTSTORAGEINFO, jsonAttStorageInfos))
		{
			return false;
		}

		std::vector<I3SAttStorageInfo> vecInfo;
		unsigned int nInfoSize = jsonAttStorageInfos.GetArraySize();
		for (unsigned int i = 0; i < nInfoSize; i++)
		{
			JsonValue jsonInfo;
			jsonAttStorageInfos.GetAt(i, &jsonInfo);

			std::wstring strKey, strName;
			jsonInfo.GetValue(I3S_JSON_KEY, strKey);
			jsonInfo.GetValue(I3S_JSON_NAME, strName);

			I3SAttStorageInfo info;
			info.SetKey(strKey);
			info.SetName(strName);
			vecInfo.push_back(info);
		}
		parseParams.SetAttStorageInfos(vecInfo);
		return true;
	}

	bool I3SParser::ParseDrawingInfo(JsonValue& jsonConfig, I3SParseParams& parseParams)
	{
		JsonValue jsonDrawingInfo;
		if (!jsonConfig.GetValue(I3S_JSON_SCENELAYER_DRAWINGINFO, jsonDrawingInfo))
		{
			return false;
		}

		I3SDrawingInfo drawingInfo;
		JsonValue jsonRenderer;
		if (jsonDrawingInfo.GetValue(I3S_JSON_SCENELAYER_RENDERER, jsonRenderer))
		{
			std::wstring strType;
			jsonRenderer.GetValue(I3S_JSON_TYPE, strType);
			drawingInfo.SetRendererType(ToRendererType(strType));
			//唯一值的符号类型
			if (drawingInfo.GetRendererType() == I3SDrawingInfo::RT_UniqueValue)
			{
				std::set<std::wstring> setStyleUrl;
				JsonValue jsonDefaultSymbol;
				if (jsonRenderer.GetValue(I3S_JSON_SCENELAYER_DEFAULTSYMBOL, jsonDefaultSymbol))
				{
					std::wstring strDefaultSymType;
					jsonDefaultSymbol.GetValue(I3S_JSON_TYPE, strDefaultSymType);
					I3SDrawingInfo::I3SSymbol3DType eDefaultSymType = ToSymbolType(strDefaultSymType);
					if (eDefaultSymType == I3SDrawingInfo::ST_StyleReference)
					{
						std::wstring strStyleUrl;
						jsonDefaultSymbol.GetValue(I3S_JSON_SCENELAYER_STYLEURL, strStyleUrl);
						if (!strStyleUrl.empty())
						{
							setStyleUrl.insert(strStyleUrl);
						}

						std::wstring strSymbolName;
						jsonDefaultSymbol.GetValue(I3S_JSON_NAME, strSymbolName);
						if (!strSymbolName.empty())
						{
							drawingInfo.SetDefaultSymbolRef(strSymbolName);
						}
					}
				}

				std::wstring strField1;
				jsonRenderer.GetValue(I3S_JSON_SCENELAYER_FIELD1, strField1);
				drawingInfo.SetFieldName(strField1);

				JsonValue jsonUniqueInfos;
				if (jsonRenderer.GetValue(I3S_JSON_SCENELAYER_UNIQUEVALUEINFOS, jsonUniqueInfos))
				{
					std::map<Variant, std::wstring> mapFieldSymbol;
					unsigned int nUniqueInfoSize = jsonUniqueInfos.GetArraySize();
					for (unsigned int i = 0; i < nUniqueInfoSize; i++)
					{
						JsonValue jsonUniqueInfo;
						jsonUniqueInfos.GetAt(i, &jsonUniqueInfo);

						std::wstring strFieldValue;
						jsonUniqueInfo.GetValue(I3S_JSON_VALUE, strFieldValue);
						if (strFieldValue.empty())
						{
							continue;
						}

						std::string cstrFieldValue = StringUtil::UnicodeToANSI(strFieldValue);
						JsonValue jsonSymbol;
						if (jsonUniqueInfo.GetValue(I3S_JSON_SCENELAYER_SYMBOL, jsonSymbol))
						{
							std::wstring strSymbolType;
							jsonSymbol.GetValue(I3S_JSON_TYPE, strSymbolType);
							drawingInfo.SetSymbolType(ToSymbolType(strSymbolType));
							if (drawingInfo.GetSymbolType() == I3SDrawingInfo::ST_StyleReference)
							{
								std::wstring strStyleUrl;
								jsonSymbol.GetValue(I3S_JSON_SCENELAYER_STYLEURL, strStyleUrl);
								if (!strStyleUrl.empty())
								{
									setStyleUrl.insert(strStyleUrl);
								}

								std::wstring strSymbolName;
								jsonSymbol.GetValue(I3S_JSON_NAME, strSymbolName);
								if (!strSymbolName.empty() &&
									mapFieldSymbol.find(cstrFieldValue) == mapFieldSymbol.end())
								{
									mapFieldSymbol[cstrFieldValue] = strSymbolName;
								}
							}
						}
					}

					drawingInfo.SetFieldSymbols(mapFieldSymbol);
				}

				std::set<std::wstring>::iterator iterSet;
				for (iterSet = setStyleUrl.begin(); iterSet != setStyleUrl.end(); iterSet++)
				{
					std::wstring strStyleUrl = *iterSet;
					if (!strStyleUrl.empty())
					{
						StringUtil::TrimLeftString(strStyleUrl, U("./"));
						std::wstring strStylePath = parseParams.GetLayerPath() + strStyleUrl;
						ParseResourcesStyles(strStylePath, drawingInfo);
					}
				}
			}

			JsonValue jsonSymbol;
			if (jsonRenderer.GetValue(I3S_JSON_SCENELAYER_SYMBOL, jsonSymbol))
			{
				JsonValue jsonSymbolLayers;
				jsonSymbol.GetValue(I3S_JSON_SYMBOLLAYERS, jsonSymbolLayers);
				unsigned int nSymLayerSize = jsonSymbolLayers.GetArraySize();
				for (unsigned int i = 0; i < nSymLayerSize; i++)
				{
					JsonValue jsonSymbolLayer;
					jsonSymbolLayers.GetAt(i, &jsonSymbolLayer);

					JsonValue jsonColor;
					JsonValue jsonMaterial;
					if (jsonSymbolLayer.GetValue(I3S_JSON_MATERIAL, jsonMaterial) &&
						jsonMaterial.GetValue(I3S_JSON_COLOR, jsonColor))
					{
						ColorValue color;
						unsigned int nColorSize = jsonColor.GetArraySize();
						if (nColorSize >= 3)
						{
							int nRed = 255, nGreen = 255, nBlue = 255, nAlpha = 255;
							jsonColor.GetAt(0, nRed);
							jsonColor.GetAt(1, nGreen);
							jsonColor.GetAt(2, nBlue);
							if (nColorSize == 4)
							{
								jsonColor.GetAt(3, nAlpha);
							}
							color.SetValue(nRed, nGreen, nBlue, nAlpha);
						}

						//有color才去解析transparency、colorMixMode
						int nTrans = 0;
						jsonMaterial.GetValue(I3S_JSON_TRANSPARENCY, nTrans);
						color.a = (100 - nTrans) / 100.0;
						drawingInfo.SetMaterialColor(color);

						std::wstring strColorMixMode;
						jsonMaterial.GetValue(I3S_JSON_COLORMIXMODE, strColorMixMode);
						drawingInfo.SetColorMixMode(ToColorMixMode(strColorMixMode));
					}
				}

				std::wstring strSymbolType;
				jsonSymbol.GetValue(I3S_JSON_TYPE, strSymbolType);
				drawingInfo.SetSymbolType(ToSymbolType(strSymbolType));
				if (drawingInfo.GetSymbolType() == I3SDrawingInfo::ST_StyleReference)
				{
					std::wstring strStyleUrl;
					jsonSymbol.GetValue(I3S_JSON_SCENELAYER_STYLEURL, strStyleUrl);
					if (!strStyleUrl.empty())
					{
						StringUtil::TrimLeftString(strStyleUrl, U("./"));
						std::wstring strStylePath = parseParams.GetLayerPath() + strStyleUrl;
						ParseResourcesStyles(strStylePath, drawingInfo);
					}

					std::wstring strSymbolName;
					jsonSymbol.GetValue(I3S_JSON_NAME, strSymbolName);
					if (!strSymbolName.empty())
					{
						drawingInfo.SetDefaultSymbolRef(strSymbolName);
					}
				}
			}
		}

		parseParams.SetDrawingInfo(drawingInfo);
		return true;
	}

	bool I3SParser::ParseResourcesStyles(const std::wstring& strName, I3SDrawingInfo& drawingInfo)
	{
		std::wstring strStyle;
		if (!m_slpkFile.GetString(strName, strStyle))
		{
			return false;
		}

		JsonValue doc;
		if (!doc.LoadValueFromString(strStyle))
		{
			return false;
		}

		std::wstring strDir = StringUtil::GetDir(strName);

		JsonValue jsonItems;
		doc.GetValue(I3S_JSON_ITEMS, jsonItems);
		unsigned int nItemSize = jsonItems.GetArraySize();
		for (unsigned int i = 0; i < nItemSize; i++)
		{
			JsonValue jsonItemObj;
			jsonItems.GetAt(i, &jsonItemObj);

			JsonValue jsonFormatInfos;
			jsonItemObj.GetValue(I3S_JSON_FORMATINFOS, jsonFormatInfos);
			unsigned int nInfoSize = jsonFormatInfos.GetArraySize();
			for (unsigned int j = 0; j < nInfoSize; j++)
			{
				JsonValue jsonInfo;
				jsonFormatInfos.GetAt(j, &jsonInfo);

				std::wstring strHref;
				jsonInfo.GetValue(I3S_JSON_HREF, strHref);
				if (!strHref.empty())
				{
					std::wstring strInfoPath = StringUtil::GetAbsolutePath(strDir, strHref);
					ParseResourcesStylesSymbolInfo(strInfoPath, drawingInfo);
				}
			}
		}

		return true;
	}

	bool I3SParser::ParseResourcesStylesSymbolInfo(const std::wstring& strName, I3SDrawingInfo& drawingInfo)
	{
		std::wstring strDoc;
		if (!m_slpkFile.GetString(strName, strDoc))
		{
			return false;
		}

		JsonValue doc;
		if (!doc.LoadValueFromString(strDoc))
		{
			return false;
		}

		std::wstring strDir = StringUtil::GetDir(strName);

		Matrix4d mat;
		std::map<std::wstring, std::wstring> mapSymbolPath = drawingInfo.GetSymbolPaths();
		std::map<std::wstring, Matrix4d> mapTrans = drawingInfo.GetSymbolTransforms();
		JsonValue jsonSymbolLayers;
		doc.GetValue(I3S_JSON_SYMBOLLAYERS, jsonSymbolLayers);
		unsigned int nLayerSize = jsonSymbolLayers.GetArraySize();
		for (unsigned int i = 0; i < nLayerSize; i++)
		{
			JsonValue jsonLayer;
			jsonSymbolLayers.GetAt(i, &jsonLayer);

			if (jsonLayer.Contains(I3S_JSON_TILT))
			{
				double dTilt = 0.0;
				jsonLayer.GetValue(I3S_JSON_TILT, dTilt);
				mat = MatrixUtils::RotationX(dTilt * DTOR);
			}

			if (jsonLayer.Contains(I3S_JSON_ROLL))
			{
				double dRoll = 0.0;
				jsonLayer.GetValue(I3S_JSON_ROLL, dRoll);
				mat = MatrixUtils::RotationY(dRoll * DTOR);
			}

			if (jsonLayer.Contains(I3S_JSON_HEADING))
			{
				double dHeading = 0.0;
				jsonLayer.GetValue(I3S_JSON_HEADING, dHeading);
				//Matrix4d为逆时针旋转，和i3s相反
				mat = MatrixUtils::RotationZ(-dHeading * DTOR);
			}

			JsonValue jsonResource;
			jsonLayer.GetValue(I3S_JSON_RESOURCE, jsonResource);

			std::wstring strHref;
			jsonResource.GetValue(I3S_JSON_HREF, strHref);
			if (!strHref.empty())
			{
				std::wstring strResourcePath = StringUtil::GetAbsolutePath(strDir, strHref);
				std::wstring strTitle = StringUtil::GetTitle(strResourcePath);
				strTitle = StringUtil::GetTitle(strTitle);
				if (mapSymbolPath.find(strTitle) == mapSymbolPath.end())
				{
					mapSymbolPath[strTitle] = strResourcePath;
					mapTrans[strTitle] = mat;
				}
			}
		}

		drawingInfo.SetSymbolPaths(mapSymbolPath);
		drawingInfo.SetSymbolTransforms(mapTrans);
		return true;
	}

	bool I3SParser::ParseFields(JsonValue& jsonConfig, I3SParseParams& parseParams)
	{
		JsonValue jsonFields;
		if (!jsonConfig.GetValue(I3S_JSON_SCENELAYER_FIELDS, jsonFields))
		{
			return false;
		}

		S3MBFieldInfos fieldInfos;
		std::vector<I3SAttributeInfo> vecAttInfo;

		unsigned int nFieldSize = jsonFields.GetArraySize();
		for (unsigned int i = 0; i < nFieldSize; i++)
		{
			JsonValue jsonField;
			jsonFields.GetAt(i, &jsonField);

			std::wstring strFieldName, strFieldType;
			jsonField.GetValue(I3S_JSON_NAME, strFieldName);
			jsonField.GetValue(I3S_JSON_TYPE, strFieldType);

			S3MBFieldInfo fieldInfo = ToFieldInfo(strFieldName, strFieldType);
			I3SAttributeInfo attInfo;
			attInfo.SetFieldName(fieldInfo.m_strName);
			attInfo.SetOriginFieldType(fieldInfo.m_nType);

			JsonValue jsonDomain;
			jsonField.GetValue(I3S_JSON_DOMAIN, jsonDomain);
			JsonValue jsonCodedValues;
			jsonDomain.GetValue(I3S_JSON_SCENELAYER_CODEDVALUES, jsonCodedValues);

			std::map<Variant, std::wstring> mapCode;
			unsigned int nCodedSize = jsonCodedValues.GetArraySize();
			for (unsigned int j = 0; j < nCodedSize; j++)
			{
				JsonValue jsonCodedVal;
				jsonCodedValues.GetAt(j, &jsonCodedVal);

				std::wstring strCodeName;
				jsonCodedVal.GetValue(I3S_JSON_NAME, strCodeName);

				std::wstring strCodeValue = I3S_JSON_CODE;
				Variant varCode = ToVariant(jsonCodedVal, strCodeValue, attInfo.GetOriginFieldType());
				mapCode[varCode] = strCodeName;
			}

			if (!mapCode.empty())
			{
				attInfo.SetIsCoded(true);
				attInfo.SetCodeValues(mapCode);

				fieldInfo.m_nType = FieldType::FT_Text;
				fieldInfo.m_nSize = 255;
			}

			fieldInfos.add(fieldInfo);
			vecAttInfo.push_back(attInfo);
		}

		parseParams.SetFieldInfos(fieldInfos);
		parseParams.SetAttributeInfos(vecAttInfo);
		return true;
	}

	bool I3SParser::ParseStore(JsonValue& jsonConfig, const I3SMetadata& metaData, I3SParseParams& parseParams, S3MBConfig& s3mbConfig)
	{
		JsonValue jsonStore;
		if (!jsonConfig.GetValue(I3S_JSON_SCENELAYER_STORE, jsonStore))
		{
			return false;
		}

		std::wstring strProfile;
		jsonStore.GetValue(I3S_JSON_SCENELAYER_PROFILE, strProfile);

		std::wstring strRootNodeDir;
		jsonStore.GetValue(I3S_JSON_SCENELAYER_ROOTNODE, strRootNodeDir);
		if (!strRootNodeDir.empty())
		{
			if (StringUtil::CompareNoCase(strProfile, I3S_JSON_POINTS))
			{
				StringUtil::TrimLeftString(strRootNodeDir, U("./"));
				strRootNodeDir = parseParams.GetLayerPath() + strRootNodeDir;
				OrientedBoundingBox obb;
				std::vector<std::pair<std::wstring, OrientedBoundingBox> > vecRootNode;
				vecRootNode.push_back(std::make_pair(strRootNodeDir, obb));
				parseParams.SetRootNodes(vecRootNode);
			}
			else
			{
				StringUtil::TrimLeftString(strRootNodeDir, U("./"));
				std::wstring strRootDocPath = parseParams.GetLayerPath() + strRootNodeDir + U("/") + I3S_FILE_3DNODEINDEXDOCUMENT;
				//解析根节点的文档
				ParseRootNodeDoc(strRootDocPath, parseParams);
			}
		}
		else
		{
			if (StringUtil::CompareNoCase(strProfile, I3S_LAYERTYPE_POINTCLOUD))
			{
				JsonValue jsonIndex;
				if (jsonStore.GetValue(I3S_JSON_SCENELAYER_INDEX, jsonIndex))
				{
					unsigned int nPageNum = 0;
					int nNodesPerPage = 0;
					std::wstring strMetricType;
					jsonIndex.GetValue(I3S_JSON_SCENELAYER_NODESPERPAGE, nNodesPerPage);
					jsonIndex.GetValue(I3S_JSON_SCENELAYER_METRICTYPE, strMetricType);
					if (nNodesPerPage > 0)
					{
						//计算页数
						float fNum = metaData.GetNodeCount() * 1.0 / nNodesPerPage;
						nPageNum = CEIL(fNum);
					}

					I3SIndexInfo indexInfo;
					indexInfo.SetPageNum(nPageNum);
					indexInfo.SetNodesPerPage(nNodesPerPage);
					indexInfo.SetLodSelMetricType(strMetricType);
					parseParams.SetIndexInfo(indexInfo);
				}

				OrganizePointCloudRootNodes(metaData, parseParams);
			}
		}

		JsonValue jsonExtent;
		if (jsonStore.GetValue(I3S_JSON_SCENELAYER_EXTENT, jsonExtent) &&
			jsonExtent.GetArraySize() >= 4)
		{
			Rect2D rcBounds;
			jsonExtent.GetAt(0, rcBounds.left);
			jsonExtent.GetAt(1, rcBounds.bottom);
			jsonExtent.GetAt(2, rcBounds.right);
			jsonExtent.GetAt(3, rcBounds.top);
			s3mbConfig.SetBounds(rcBounds);

			if (!jsonConfig.Contains(I3S_JSON_SCENELAYER_FULLEXTENT))
			{
				Point3D pntPos;
				pntPos.x = (rcBounds.left + rcBounds.right) / 2;
				pntPos.y = (rcBounds.top + rcBounds.bottom) / 2;
				s3mbConfig.SetPosition(pntPos);
			}
		}

		//默认几何数据的顶点属性顺序
		JsonValue jsonSchema;
		if (jsonStore.GetValue(I3S_JSON_SCENELAYER_GEOMETRYSCHEMA, jsonSchema))
		{
			std::vector<std::pair<std::wstring, int> > vecVexAttAndId, vecVexAttExtAndId;

			//点云的顶点压缩方式
			std::wstring strEncodeType;
			jsonSchema.GetValue(I3S_JSON_ENCODING, strEncodeType);
			if (StringUtil::CompareNoCase(strEncodeType, I3S_JSON_ENCODING_LEPCCXYZ))
			{
				parseParams.SetVertexCompressionType(SV_DracoCompressed);
			}

			JsonValue jsonOrder;
			jsonSchema.GetValue(I3S_JSON_SCENELAYER_ORDER, jsonOrder);
			unsigned int nOrderSize = jsonOrder.GetArraySize();
			for (unsigned int i = 0; i < nOrderSize; i++)
			{
				std::wstring strValue;
				jsonOrder.GetAt(i, strValue);
				if (StringUtil::CompareNoCase(strValue, I3S_JSON_POSITION) ||
					StringUtil::CompareNoCase(strValue, I3S_JSON_NORMAL) ||
					StringUtil::CompareNoCase(strValue, I3S_JSON_UV0) ||
					StringUtil::CompareNoCase(strValue, I3S_JSON_COLOR))
				{
					vecVexAttAndId.push_back(std::make_pair(strValue, 0));
				}
				else if (StringUtil::CompareNoCase(strValue, I3S_JSON_NONCOMPRESSED_UVREGION))
				{
					vecVexAttExtAndId.push_back(std::make_pair(ATT_UVREGION, 0));
				}
			}

			JsonValue jsonFeatureAttOrder;
			jsonSchema.GetValue(I3S_JSON_SCENELAYER_FEATUREATTORDER, jsonFeatureAttOrder);
			unsigned int nAttOrderSize = jsonFeatureAttOrder.GetArraySize();
			for (unsigned int i = 0; i < nAttOrderSize; i++)
			{
				std::wstring strValue;
				jsonFeatureAttOrder.GetAt(i, strValue);
				if (StringUtil::CompareNoCase(strValue, I3S_JSON_ID) ||
					StringUtil::CompareNoCase(strValue, I3S_JSON_SCENELAYER_FACERANGE))
				{
					vecVexAttExtAndId.push_back(std::make_pair(strValue, 0));
				}
			}

			parseParams.SetVertexAttributeAndIdRelation(vecVexAttAndId);
			parseParams.SetVertexAttributeExtensionAndIdRelation(vecVexAttExtAndId);
		}
		return true;
	}

	bool I3SParser::ParseRootNodeDoc(const std::wstring& strName, I3SParseParams& parseParams)
	{
		std::wstring strNodeDoc;
		if (!m_slpkFile.GetString(strName, strNodeDoc))
		{
			return false;
		}

		JsonValue doc;
		if (!doc.LoadValueFromString(strNodeDoc))
		{
			return false;
		}

		JsonValue jsonChildren;
		if (doc.GetValue(I3S_JSON_CHILDREN, jsonChildren))
		{
			std::vector<std::pair<std::wstring, OrientedBoundingBox> > vecRootNode;
			unsigned int nChildSize = jsonChildren.GetArraySize();
			for (unsigned int i = 0; i < nChildSize; i++)
			{
				JsonValue jsonChild;
				jsonChildren.GetAt(i, &jsonChild);

				std::wstring strHref;
				jsonChild.GetValue(I3S_JSON_HREF, strHref);
				if (!strHref.empty())
				{
					std::wstring strRootNodeDir = StringUtil::GetDir(strName);
					strRootNodeDir = StringUtil::GetAbsolutePath(strRootNodeDir, strHref);

					JsonValue jsonObb;
					jsonChild.GetValue(I3S_JSON_OBB, jsonObb);

					OrientedBoundingBox obb;
					ParseOBB(jsonObb, obb);
					vecRootNode.push_back(std::make_pair(strRootNodeDir, obb));
				}
			}
			parseParams.SetRootNodes(vecRootNode);
		}
		return true;
	}

	bool I3SParser::OrganizePointCloudRootNodes(const I3SMetadata& metaData, I3SParseParams& parseParams)
	{
		std::wstring strNodesDir = parseParams.GetLayerPath() + I3S_JSON_NODES + U("/");

		std::vector<std::pair<std::wstring, OrientedBoundingBox> > vecRootNode;
		//点云数据只有一个根节点
		if (metaData.GetNodeCount() > 0)
		{
			std::wstring strNodeId = U("0");
			std::wstring strRootNodeDir = parseParams.GetLayerPath() + I3S_JSON_NODES + U("/") + strNodeId;
			OrientedBoundingBox obb;
			vecRootNode.push_back(std::make_pair(strRootNodeDir, obb));
		}

		parseParams.SetRootNodes(vecRootNode);
		return true;
	}

	bool I3SParser::ParseTextureSetDefinitions(JsonValue& jsonConfig, I3SParseParams& parseParams)
	{
		JsonValue jsonTexSetDefs;
		if (!jsonConfig.GetValue(I3S_JSON_SCENELAYER_TEXSETDEFS, jsonTexSetDefs))
		{
			return false;
		}

		std::map<std::wstring, std::wstring> mapTexFormat;
		unsigned int nDefSize = jsonTexSetDefs.GetArraySize();
		for (unsigned int i = 0; i < nDefSize; i++)
		{
			JsonValue jsonDef;
			jsonTexSetDefs.GetAt(i, &jsonDef);

			JsonValue jsonFormats;
			jsonDef.GetValue(I3S_JSON_SCENELAYER_FORMATS, jsonFormats);
			unsigned int nFmtSize = jsonFormats.GetArraySize();
			for (unsigned int j = 0; j < nFmtSize; j++)
			{
				JsonValue jsonTexInfo;
				jsonFormats.GetAt(j, &jsonTexInfo);
				std::wstring strName, strFormat;
				jsonTexInfo.GetValue(I3S_JSON_NAME, strName);
				jsonTexInfo.GetValue(I3S_JSON_SCENELAYER_FORMAT, strFormat);
				if (!strName.empty())
				{
					mapTexFormat[strName] = strFormat;
				}
			}
		}
		parseParams.SetTextureFormats(mapTexFormat);
		return true;
	}

	bool I3SParser::ParseGeometryDefinitions(JsonValue& jsonConfig, I3SParseParams& parseParams)
	{
		JsonValue jsonGeoDefs;
		if (!jsonConfig.GetValue(I3S_JSON_SCENELAYER_GEOMETRYDEFS, jsonGeoDefs))
		{
			return false;
		}

		unsigned int nDefSize = jsonGeoDefs.GetArraySize();
		for (unsigned int i = 0; i < nDefSize; i++)
		{
			JsonValue jsonDef;
			jsonGeoDefs.GetAt(i, &jsonDef);

			JsonValue jsonGeoBuffers;
			jsonDef.GetValue(I3S_JSON_SCENELAYER_GEOMETRYBUFFERS, jsonGeoBuffers);
			unsigned int nBufSize = jsonGeoBuffers.GetArraySize();
			for (unsigned int j = 0; j < nBufSize; j++)
			{
				JsonValue jsonBuf;
				jsonGeoBuffers.GetAt(j, &jsonBuf);

				//压缩数据的顶点属性
				JsonValue jsonCompAtt;
				jsonBuf.GetValue(I3S_JSON_SCENELAYER_COMPATT, jsonCompAtt);

				std::wstring strEncodeType;
				jsonCompAtt.GetValue(I3S_JSON_ENCODING, strEncodeType);
				if (StringUtil::CompareNoCase(strEncodeType, I3S_JSON_ENCODING_DRACO))
				{
					parseParams.SetVertexCompressionType(SV_DracoCompressed);
				}

				JsonValue jsonAtt;
				if (jsonCompAtt.GetValue(I3S_JSON_ATTRIBUTES, jsonAtt))
				{
					std::vector<std::pair<std::wstring, int> > vecVexAttAndId, vecVexAttExtAndId;
					unsigned int nAttSize = jsonAtt.GetArraySize();
					for (unsigned int k = 0; k < nAttSize; k++)
					{
						std::wstring strValue;
						jsonAtt.GetAt(k, strValue);
						if (StringUtil::CompareNoCase(strValue, I3S_JSON_POSITION) ||
							StringUtil::CompareNoCase(strValue, I3S_JSON_NORMAL) ||
							StringUtil::CompareNoCase(strValue, I3S_JSON_UV0) ||
							StringUtil::CompareNoCase(strValue, I3S_JSON_COLOR) ||
							StringUtil::CompareNoCase(strValue, I3S_JSON_COMPRESSED_FEATUREID))
						{
							vecVexAttAndId.push_back(std::make_pair(strValue, k));
						}
						else if (StringUtil::CompareNoCase(strValue, I3S_JSON_COMPRESSED_UVREGION))
						{
							vecVexAttExtAndId.push_back(std::make_pair(ATT_UVREGION, k));
						}
					}

					parseParams.SetVertexAttributeAndIdRelation(vecVexAttAndId);
					parseParams.SetVertexAttributeExtensionAndIdRelation(vecVexAttExtAndId);
				}
			}
		}
		return true;
	}

	bool I3SParser::ParseOBB(JsonValue& jsonObb, OrientedBoundingBox& obb)
	{
		Vector3d vCenter, vHalf;
		Vector4d vQuater;

		std::vector<double> vecCenter;
		JsonValue jsonObbCenter;
		jsonObb.GetValue(I3S_JSON_OBB_CENTER, jsonObbCenter);
		unsigned int nCenterSize = jsonObbCenter.GetArraySize();
		for (unsigned int j = 0; j < nCenterSize; j++)
		{
			double dValue = 0.0;
			jsonObbCenter.GetAt(j, dValue);
			vecCenter.push_back(dValue);
		}

		if (vecCenter.size() == 3)
		{
			vCenter.x = vecCenter[0];
			vCenter.y = vecCenter[1];
			vCenter.z = vecCenter[2];
		}

		std::vector<double> vecHalf;
		JsonValue jsonObbHalf;
		jsonObb.GetValue(I3S_JSON_OBB_HALFSIZE, jsonObbHalf);
		unsigned int nHalfSize = jsonObbHalf.GetArraySize();
		for (unsigned int j = 0; j < nHalfSize; j++)
		{
			double dValue = 0.0;
			jsonObbHalf.GetAt(j, dValue);
			vecHalf.push_back(dValue);
		}

		if (vecHalf.size() == 3)
		{
			vHalf.x = vecHalf[0];
			vHalf.y = vecHalf[1];
			vHalf.z = vecHalf[2];
		}

		std::vector<double> vecQuater;
		JsonValue jsonObbQuater;
		jsonObb.GetValue(I3S_JSON_OBB_QUATERNION, jsonObbQuater);
		unsigned int nQuaterSize = jsonObbQuater.GetArraySize();
		for (unsigned int j = 0; j < nQuaterSize; j++)
		{
			double dValue = 0.0;
			jsonObbQuater.GetAt(j, dValue);
			vecQuater.push_back(dValue);
		}

		if (vecQuater.size() == 4)
		{
			vQuater.x = vecQuater[0];
			vQuater.y = vecQuater[1];
			vQuater.z = vecQuater[2];
			vQuater.w = vecQuater[3];
		}

		ToOBB(vCenter, vHalf, vQuater, obb);
		return true;
	}

	void I3SParser::ToOBB(const Vector3d& vCenter, const Vector3d& vHalf, const Vector4d& vQuaternion, OrientedBoundingBox& obb)
	{
		const auto x2 = vQuaternion.x + vQuaternion.x;
		const auto y2 = vQuaternion.y + vQuaternion.y;
		const auto z2 = vQuaternion.z + vQuaternion.z;

		const auto xx = x2 * vQuaternion.x;
		const auto xy = x2 * vQuaternion.y;
		const auto xz = x2 * vQuaternion.z;
		const auto xw = x2 * vQuaternion.w;
		const auto yy = y2 * vQuaternion.y;
		const auto yz = y2 * vQuaternion.z;
		const auto yw = y2 * vQuaternion.w;
		const auto zz = z2 * vQuaternion.z;
		const auto zw = z2 * vQuaternion.w;

		const auto c_1 = static_cast<double>(1);
		Vector3d vAxisX, vAxisY, vAxisZ;
		vAxisX.x = c_1 - (yy + zz);
		vAxisX.y = xy + zw;
		vAxisX.z = xz - yw;
		vAxisY.x = xy - zw;
		vAxisY.y = c_1 - (xx + zz);
		vAxisY.z = yz + xw;
		vAxisZ.x = xz + yw;
		vAxisZ.y = yz - xw;
		vAxisZ.z = c_1 - (xx + yy);

		Vector3d vHalfAxisX, vHalfAxisY, vHalfAxisZ;
		vHalfAxisX = vAxisX * vHalf.x;
		vHalfAxisY = vAxisY * vHalf.y;
		vHalfAxisZ = vAxisZ * vHalf.z;

		Matrix3d matHalfAxes;
		matHalfAxes.SetColumn(0, vHalfAxisX);
		matHalfAxes.SetColumn(1, vHalfAxisY);
		matHalfAxes.SetColumn(2, vHalfAxisZ);

		obb = OrientedBoundingBox(vCenter, matHalfAxes);
	}

	SCPSDataType I3SParser::ToDataType(const std::wstring &strLayerType)
	{
		SCPSDataType eDataType = SDT_BIM;
		if (StringUtil::CompareNoCase(strLayerType, I3S_LAYERTYPE_INTEGRATEDMESH))
		{
			eDataType = SDT_RealityMesh;
		}
		else if (StringUtil::CompareNoCase(strLayerType, I3S_LAYERTYPE_BUILDING))
		{
			eDataType = SDT_BIM;
		}
		else if (StringUtil::CompareNoCase(strLayerType, I3S_LAYERTYPE_3DOBJECT))
		{
			eDataType = SDT_ArtificialModel;
		}
		else if (StringUtil::CompareNoCase(strLayerType, I3S_LAYERTYPE_POINTCLOUD))
		{
			eDataType = SDT_PointCloud;
		}
		else if (StringUtil::CompareNoCase(strLayerType, I3S_LAYERTYPE_POINT))
		{
			eDataType = SDT_InstanceModel;
		}
		else
		{
			assert(false);
		}
		return eDataType;
	}

	I3SDrawingInfo::I3SRendererType I3SParser::ToRendererType(const std::wstring& strType)
	{
		I3SDrawingInfo::I3SRendererType eType = I3SDrawingInfo::RT_None;
		if (StringUtil::CompareNoCase(strType, I3S_JSON_RENDERERTYPE_SIMPLE))
		{
			eType = I3SDrawingInfo::RT_Simple;
		}
		else if (StringUtil::CompareNoCase(strType, I3S_JSON_RENDERERTYPE_UNIQUEVALUE))
		{
			eType = I3SDrawingInfo::RT_UniqueValue;
		}
		return eType;
	}

	I3SDrawingInfo::I3SSymbol3DType I3SParser::ToSymbolType(const std::wstring& strType)
	{
		I3SDrawingInfo::I3SSymbol3DType eType = I3SDrawingInfo::ST_None;
		if (StringUtil::CompareNoCase(strType, I3S_JSON_SYMBOLTYPE_POINT))
		{
			eType = I3SDrawingInfo::ST_Point;
		}
		else if (StringUtil::CompareNoCase(strType, I3S_JSON_SYMBOLTYPE_STYLEREF))
		{
			eType = I3SDrawingInfo::ST_StyleReference;
		}
		return eType;
	}

	I3SDrawingInfo::I3SColorMixMode I3SParser::ToColorMixMode(const std::wstring& strMode)
	{
		I3SDrawingInfo::I3SColorMixMode eMode = I3SDrawingInfo::CMM_None;
		if (StringUtil::CompareNoCase(strMode, I3S_JSON_COLORMIXMODE_MULTIPLY))
		{
			eMode = I3SDrawingInfo::CMM_Multiply;
		}
		else if (StringUtil::CompareNoCase(strMode, I3S_JSON_COLORMIXMODE_REPLACE))
		{
			eMode = I3SDrawingInfo::CMM_Replace;
		}
		return eMode;
	}

	S3MBFieldInfo I3SParser::ToFieldInfo(const std::wstring& strFieldName, const std::wstring& strFieldType)
	{
		S3MBFieldInfo fieldInfo;
		if (StringUtil::CompareNoCase(strFieldType, I3S_FIELDTYPE_OID))
		{
			fieldInfo.m_nType = FieldType::FT_INT32;
			fieldInfo.m_nSize = 4;
			fieldInfo.m_strName = S3MB_JSON_ATT_SMID;
			fieldInfo.m_strForeignName = S3MB_JSON_ATT_SMID;
			fieldInfo.m_bRequired = true;
		}
		else if (StringUtil::CompareNoCase(strFieldType, I3S_FIELDTYPE_SMALLINTEGER))
		{
			fieldInfo.m_nType = FieldType::FT_INT16;
			fieldInfo.m_nSize = 2;
			fieldInfo.m_strName = strFieldName;
			fieldInfo.m_strForeignName = strFieldName;
			fieldInfo.m_bRequired = false;
		}
		else if (StringUtil::CompareNoCase(strFieldType, I3S_FIELDTYPE_INTEGER))
		{
			fieldInfo.m_nType = FieldType::FT_INT32;
			fieldInfo.m_nSize = 4;
			fieldInfo.m_strName = strFieldName;
			fieldInfo.m_strForeignName = strFieldName;
			fieldInfo.m_bRequired = false;
		}
		else if (StringUtil::CompareNoCase(strFieldType, I3S_FIELDTYPE_SINGLE))
		{
			fieldInfo.m_nType = FieldType::FT_Float;
			fieldInfo.m_nSize = 4;
			fieldInfo.m_strName = strFieldName;
			fieldInfo.m_strForeignName = strFieldName;
			fieldInfo.m_bRequired = false;
		}
		else if (StringUtil::CompareNoCase(strFieldType, I3S_FIELDTYPE_DOUBLE))
		{
			fieldInfo.m_nType = FieldType::FT_Double;
			fieldInfo.m_nSize = 8;
			fieldInfo.m_strName = strFieldName;
			fieldInfo.m_strForeignName = strFieldName;
			fieldInfo.m_bRequired = false;
		}
		else if (StringUtil::CompareNoCase(strFieldType, I3S_FIELDTYPE_STRING) ||
			StringUtil::CompareNoCase(strFieldType, I3S_FIELDTYPE_DATE))
		{
			fieldInfo.m_nType = FieldType::FT_Text;
			fieldInfo.m_nSize = 255;
			fieldInfo.m_strName = strFieldName;
			fieldInfo.m_strForeignName = strFieldName;
			fieldInfo.m_bRequired = false;
		}
		else
		{
			assert(false);
		}

		return fieldInfo;
	}

	Variant I3SParser::ToVariant(JsonValue& jsonValue, const std::wstring& strKey, FieldType nFieldType)
	{
		Variant varResult;
		switch (nFieldType)
		{
		case FieldType::FT_Boolean:
		{
			bool bValue = false;
			jsonValue.GetValue(strKey, bValue);
			varResult.Set(bValue);
		}
		break;
		case FieldType::FT_INT32:
		{
			int nValue = 0;
			jsonValue.GetValue(strKey, nValue);
			varResult.Set(nValue);
		}
		break;
		case FieldType::FT_Float:
		{
			float fValue = 0.0f;
			jsonValue.GetValue(strKey, fValue);
			varResult.Set(fValue);
		}
		break;
		case FieldType::FT_Double:
		{
			double dValue = 0.0;
			jsonValue.GetValue(strKey, dValue);
			varResult.Set(dValue);
		}
		break;
		case FieldType::FT_Text:
		{
			std::wstring strValue;
			jsonValue.GetValue(strKey, strValue);
			std::string cstrValue = StringUtil::UnicodeToANSI(strValue);
			varResult.Set(cstrValue);
		}
		break;
		default:
			break;
		}
		return varResult;
	}

	Variant I3SParser::ToVariant(const Variant& var, FieldType nFieldType)
	{
		Variant varResult;
		switch (nFieldType)
		{
		case FieldType::FT_Boolean:
		{
			varResult.Set(var.ToBoolean());
		}
		break;
		case FieldType::FT_INT16:
		{
			short sValue = var.ToInt();
			varResult.Set(sValue);
		}
		break;
		case FieldType::FT_INT32:
		{
			varResult.Set(var.ToInt());
		}
		break;
		case FieldType::FT_Float:
		{
			float fValue = var.ToDouble();
			varResult.Set(fValue);
		}
		break;
		case FieldType::FT_Double:
		{
			varResult.Set(var.ToDouble());
		}
		break;
		case FieldType::FT_Text:
		{
			std::wstring strVar = var.ToString();
			std::string cstrVar = StringUtil::UnicodeToANSI(strVar);
			varResult.Set(cstrVar);
		}
		break;
		default:
			break;
		}
		return varResult;
	}
}
