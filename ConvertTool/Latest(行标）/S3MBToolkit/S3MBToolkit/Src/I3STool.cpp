#include "I3STool.h"
#include "I3STranslation.h"
#include "S3MBCommon.h"

namespace S3MB
{
	I3STool::I3STool()
	{
	}

	I3STool::~I3STool()
	{
	}

	bool I3STool::ToS3M(const I3SParams& params)
	{
		if (params.GetInputPath().empty() ||
			params.GetOutputPath().empty())
		{
			return false;
		}

		I3SParser parser;
		const I3SParams& modifiedParams = PrepareParams(params);
		if (!parser.Open(modifiedParams) ||
			!parser.ParseConfig())
		{
			return false;
		}

		const std::vector<I3SLayerInfo>& vecLayerInfo = parser.GetLayerInfos();
		const std::map<std::wstring, I3SParseParams>& mapParseParam = parser.GetParseParams();
		const std::map<std::wstring, S3MBConfig>& mapS3MBConfig = parser.GetS3MBConfigs();
		for (unsigned int i = 0; i < vecLayerInfo.size(); i++)
		{
			const I3SLayerInfo& layerInfo = vecLayerInfo[i];
			const I3SParseParams& parseParams = mapParseParam.at(layerInfo.GetId());
			S3MBConfig s3mbConfig = mapS3MBConfig.at(layerInfo.GetId());
			ProcessLayer(modifiedParams, layerInfo, parseParams, s3mbConfig);
		}

		return true;
	}

	I3SParams I3STool::PrepareParams(const I3SParams& params)
	{
		I3SParams modifiedParams = params;
		//最低版本为s3m 3.0
		if (!GREATER_OR_EQUAL(modifiedParams.GetS3MVersion(), S3MB_VERSIONV3))
		{
			modifiedParams.SetS3MVersion(S3MB_VERSIONV3);
		}

		//修改输出路径
		std::wstring strModifiedPath = modifiedParams.GetOutputPath();
		StringUtil::Replace(strModifiedPath, U("\\"), U("/"));
		StringUtil::TrimRightString(strModifiedPath, U("/"));
		strModifiedPath += U("/");
		if (!modifiedParams.GetDataName().empty())
		{
			strModifiedPath += (modifiedParams.GetDataName() + U("/"));
		}
		modifiedParams.SetOutputPath(strModifiedPath);
		return modifiedParams;
	}

	bool I3STool::ProcessLayer(const I3SParams& params, const I3SLayerInfo& layerInfo, const I3SParseParams& parseParams, S3MBConfig& s3mbConfig)
	{
		//点数据只支持带外挂文件的数据类型
		if (s3mbConfig.GetDataType() == SDT_InstanceModel &&
			parseParams.GetDrawingInfo().GetSymbolPaths().empty())
		{
			return false;
		}

		ProcessNodes(params, layerInfo, parseParams, s3mbConfig);
		ProcessSymbols(params, parseParams);
		return true;
	}

	bool I3STool::ProcessNodes(const I3SParams& params, const I3SLayerInfo& layerInfo, const I3SParseParams& parseParams, S3MBConfig& s3mbConfig)
	{
		std::vector<std::wstring> vecRootName;
		std::vector<OrientedBoundingBox> vecRootObb;
		const std::wstring& strOutputPath = parseParams.GetOutputPath();
		const std::vector<std::pair<std::wstring, OrientedBoundingBox> >& vecRootNode = parseParams.GetRootNodes();
		std::vector<std::vector<std::pair<std::wstring, OrientedBoundingBox> > > arrRootNode;

		std::vector<std::wstring> vecNodePath;
		for (unsigned int i = 0; i < vecRootNode.size(); i++)
		{
			vecNodePath.push_back(vecRootNode[i].first);
		}

		I3SNodesTranslation nodesTrans(params, parseParams, s3mbConfig, vecNodePath);
		nodesTrans.Run();

		int nMinId = INT32_MAX, nMaxId = INT32_MIN;
		double dMinHeight = FLTMAX, dMaxHeight = -FLTMAX;
		std::map<std::wstring, std::vector<AttributeIndexInfo> > mapAttIndexInfo;
		std::map<std::wstring, CategoryDescript> mapDescriptTotal;

		const std::vector<std::pair<std::wstring, AttributeIndexInfo> >& vecAttIndexInfoPair = nodesTrans.GetAttributeIndexInfoPairs();
		const std::pair<double, double>& pairHeightRange = nodesTrans.GetHeightRange();
		vecRootName = nodesTrans.GetRootNames();
		vecRootObb = nodesTrans.GetRootObbs();
		mapDescriptTotal = nodesTrans.GetCategoryDescripts();

		for (unsigned int i = 0; i < vecAttIndexInfoPair.size(); i++)
		{
			std::wstring strFile = vecAttIndexInfoPair.at(i).first;
			const AttributeIndexInfo& pairAttIndexInfo = vecAttIndexInfoPair.at(i).second;
			nMinId = MIN(nMinId, pairAttIndexInfo.m_pairIdRange.first);
			nMaxId = MAX(nMaxId, pairAttIndexInfo.m_pairIdRange.second);

			if (mapAttIndexInfo.find(strFile) == mapAttIndexInfo.end())
			{
				mapAttIndexInfo[strFile] = std::vector<AttributeIndexInfo>();
			}
			mapAttIndexInfo[strFile].push_back(pairAttIndexInfo);
		}

		//最小高度、最大高度
		dMinHeight = MIN(dMinHeight, pairHeightRange.first);
		dMaxHeight = MAX(dMaxHeight, pairHeightRange.second);

		s3mbConfig.SetHeightMin(dMinHeight);
		s3mbConfig.SetHeightMax(dMaxHeight);

		std::vector<CategoryDescript> vecDescript;
		for (auto iterDescript = mapDescriptTotal.begin(); iterDescript != mapDescriptTotal.end(); iterDescript++)
		{
			vecDescript.push_back(iterDescript->second);
		}
		s3mbConfig.SetVertexAttributeDescripts(vecDescript);

		//属性描述文件
		if (!mapAttIndexInfo.empty())
		{
			LayerCacheInfo cacheInfo;
			cacheInfo.m_pairIDRange.first = nMinId;
			cacheInfo.m_pairIDRange.second = nMaxId;
			cacheInfo.m_fieldInfos = parseParams.GetFieldInfos();
			cacheInfo.m_strDatasetName = U("TileSet");

			std::vector<LayerCacheInfo> vecLayerInfo;
			vecLayerInfo.push_back(cacheInfo);
			std::wstring strAttDescFile = StringUtil::GetAbsolutePath(strOutputPath, FILE_NAME_ATTINFO);
			S3MBLayerInfos::WriteLayerFieldInfosToFile(vecLayerInfo, strAttDescFile);

			std::wstring strAttExtName = FILE_EXT_S3MD;
			StringUtil::TrimLeftString(strAttExtName, U("."));
			s3mbConfig.SetExtension(CONFIG_ATTRIBUTEEXTENTNAME, strAttExtName);
		}

		s3mbConfig.SetTiles(vecRootName, vecRootObb);

		std::wstring strConfigPath = parseParams.GetOutputPath() + layerInfo.GetName() + FILE_EXT_SCP;
		s3mbConfig.WriteFile(strConfigPath);

		return true;
	}

	bool I3STool::ProcessSymbols(const I3SParams& params, const I3SParseParams& parseParams)
	{
		const std::map<std::wstring, std::wstring>& mapSymbolPaths = parseParams.GetDrawingInfo().GetSymbolPaths();
		if (mapSymbolPaths.empty())
		{
			return false;
		}

		std::vector<std::wstring> vecSymbolPath;
		for (auto iter = mapSymbolPaths.begin(); iter != mapSymbolPaths.end(); iter++)
		{
			vecSymbolPath.push_back(iter->second);
		}

		I3SSymbolsTranslation symbolsTrans(params, parseParams, vecSymbolPath);
		return symbolsTrans.Run();
	}
}
