#include "I3STranslation.h"
#include "GLTFParser.h"
#include "I3SLepccDecoder.h"
#include "I3SParser.h"
#include "ImageOperator.h"
#include "MathEngine.h"
#include "ProcessTools.h"
#include "S3MBWriter.h"
#include "Shell.h"
#include "Utils/Utils.h"

#define I3S_PRE_ROOT U("root_")

#define I3S_EXT_BINGZ U(".bin.gz")
#define I3S_EXT_BINDDS U(".bin.dds.gz")
#define I3S_EXT_BINPCCXYZ U(".bin.pccxyz")
#define I3S_EXT_BINPCCINT U(".bin.pccint")
#define I3S_EXT_BINPCCRGB U(".bin.pccrgb")
#define I3S_EXT_DDS U(".dds")

#define I3S_FILE_3DNODEINDEXDOCUMENT U("3dNodeIndexDocument.json.gz")

#define I3S_JSON_ID U("id")
#define I3S_JSON_POSITION U("position")
#define I3S_JSON_NORMAL U("normal")
#define I3S_JSON_UV0 U("uv0")
#define I3S_JSON_COLOR U("color")
#define I3S_JSON_COMPRESSED_UVREGION U("uv-region")
#define I3S_JSON_SCENELAYER_FACERANGE U("faceRange")
#define I3S_JSON_SCENELAYER_DDS U("dds")

#define I3S_KEY_ELEVATION U("1")
#define I3S_KEY_INTENSITY U("2")
#define I3S_KEY_RGB U("4")
#define I3S_KEY_CLASSCODE U("8")
#define I3S_KEY_RETURNS U("32")

namespace S3MB
{
	I3SNodesTranslation::I3SNodesTranslation(const I3SParams& i3sParams, const I3SParseParams& i3sParseParams,
		const S3MBConfig& s3mbConfig, const std::vector<std::wstring>& vecPath) :
		m_I3SParams(i3sParams),
		m_I3SParseParams(i3sParseParams),
		m_S3MBConfig(s3mbConfig),
		m_vecNodePath(vecPath)
	{
	}

	I3SNodesTranslation::~I3SNodesTranslation()
	{
	}

	const std::vector<std::pair<std::wstring, AttributeIndexInfo> >& I3SNodesTranslation::GetAttributeIndexInfoPairs() const
	{
		return m_vecAttIndexInfoPair;
	}

	const std::pair<double, double>& I3SNodesTranslation::GetHeightRange() const
	{
		return m_pairHeightRange;
	}

	const std::map<std::wstring, CategoryDescript>& I3SNodesTranslation::GetCategoryDescripts() const
	{
		return m_mapCategoryDescript;
	}

	const std::vector<std::wstring>& I3SNodesTranslation::GetRootNames() const
	{
		return m_vecRootName;
	}

	const std::vector<OrientedBoundingBox>& I3SNodesTranslation::GetRootObbs() const
	{
		return m_vecRootObb;
	}

	bool I3SNodesTranslation::Run()
	{
		m_slpkFile.Open(m_I3SParams.GetInputPath());
		const std::wstring& strOutputPath = m_I3SParseParams.GetOutputPath();

		for (unsigned int i = 0; i < m_vecNodePath.size(); i++)
		{
			std::wstring strName = I3S_PRE_ROOT + StringUtil::GetTitle(m_vecNodePath[i]);
			std::wstring strRootS3M = strOutputPath + strName + U("/") + strName + FILE_EXT_S3MB;
			BoundingBox boxNode;
			OrientedBoundingBox obbNode;
			std::map<int, Feature*> mapFeature;
			std::vector<std::wstring> vecNode;
			vecNode.push_back(m_vecNodePath[i]);

			if (m_S3MBConfig.GetDataType() == SDT_PointCloud)
			{
				std::map<std::wstring, I3SNodeInfo> mapNodeInfo;
				I3SParser::ParsePointCloudNodePages(m_slpkFile, m_I3SParseParams, mapNodeInfo);
				TranslatePointCloudNode(mapNodeInfo, vecNode, strRootS3M, true, boxNode, obbNode);
			}
			else if (m_S3MBConfig.GetDataType() == SDT_InstanceModel)
			{
				TranslatePointNode(vecNode, strRootS3M, true, boxNode, obbNode, mapFeature);
			}
			else
			{
				TranslateNode(vecNode, strRootS3M, true, boxNode, obbNode, mapFeature);
			}

			std::wstring strRootRela = StringUtil::GetRelativePath(strOutputPath, StringUtil::GetDir(strRootS3M));
			strRootRela = strRootRela + StringUtil::GetName(strRootS3M);
			m_vecRootName.push_back(strRootRela);
			m_vecRootObb.push_back(obbNode);

			if (!mapFeature.empty())
			{
				int nMinId = INT32_MAX, nMaxId = INT32_MIN;
				std::vector<Feature*> vecFeature;
				for (auto iter = mapFeature.begin(); iter != mapFeature.end(); iter++)
				{
					nMinId = MIN(nMinId, iter->first);
					nMaxId = MAX(nMaxId, iter->first);

					if (iter->second != nullptr)
					{
						vecFeature.push_back(iter->second);
					}
				}

				//属性索引信息
				std::wstring strAttRela = StringUtil::ChangeExt(strRootRela, FILE_EXT_S3MD);
				AttributeIndexInfo attIndexInfo;
				attIndexInfo.m_strAttributeFileName = strAttRela;
				attIndexInfo.m_pairIdRange = std::make_pair(nMinId, nMaxId);
				m_vecAttIndexInfoPair.push_back(std::make_pair(strRootRela, attIndexInfo));

				//保存属性数据文件
				if (!vecFeature.empty())
				{
					std::wstring strAttFile = StringUtil::ChangeExt(strRootS3M, FILE_EXT_S3MD);
					std::map<unsigned int, std::pair<unsigned int, std::vector<Feature*> > > mapMaxMinIdFeature;
					mapMaxMinIdFeature[nMaxId] = std::pair<int, std::vector<Feature*> >(nMinId, vecFeature);
					S3MBLayerInfos::WriteLayerAttributeValueToFile(mapMaxMinIdFeature, strAttFile);
				}

				for (auto iter = mapFeature.begin(); iter != mapFeature.end(); iter++)
				{
					delete iter->second;
					iter->second = nullptr;
				}
				mapFeature.clear();
			}
		}

		return m_slpkFile.Close();
	}

	bool I3SNodesTranslation::TranslateNode(const std::vector<std::wstring>& vecDir, const std::wstring& strOutputPath, 
		bool bRootNode, BoundingBox& box, OrientedBoundingBox& obb, std::map<int, Feature*>& mapFeature)
	{
		std::vector<Vector3d> vecTotalCorner;
		BoundingBox boxTotal;
		std::map<std::wstring, std::vector<std::wstring> > mapChildDir;
		RenderOperationGroup* pGroup = new RenderOperationGroup();
		for (unsigned int i = 0; i < vecDir.size(); i++)
		{
			std::wstring strtDocPath = vecDir[i] + U("/") + I3S_FILE_3DNODEINDEXDOCUMENT;
			I3SNodeInfo nodeInfo;
			I3SParser::ParseNodeDoc(m_slpkFile, m_I3SParseParams, strtDocPath, nodeInfo);

			//解析材质信息
			I3SMaterialInfo matInfo;
			I3SParser::ParseSharedResource(m_slpkFile, nodeInfo, matInfo);
			if (!matInfo.GetName().empty())
			{
				Material* pMaterial = nullptr;
				TranslateMaterial(matInfo, nodeInfo.GetTextures(), pMaterial);
				pGroup->AddMaterial(pMaterial->m_strName, pMaterial);
			}

			//解析纹理
			std::map<std::wstring, TextureDataInfo*> mapTexInfo;
			TranslateTextures(nodeInfo.GetTextures(), mapTexInfo);
			std::map<std::wstring, TextureDataInfo*>::iterator iterTex;
			for (iterTex = mapTexInfo.begin(); iterTex != mapTexInfo.end(); iterTex++)
			{
				pGroup->AddTextureData(iterTex->first, iterTex->second);
			}

			//计算geode矩阵
			Point3D pntPos = m_S3MBConfig.GetPosition();
			Vector3d vPos(pntPos.x, pntPos.y, pntPos.z);
			Vector3d vNod = nodeInfo.GetObb().GetCenter();
			Matrix4d matPos, matNod;
			GetMatrix(vPos, matPos);
			GetMatrix(vNod, matNod);
			Matrix4d matGeode = matNod * matPos.Invert();

			//解析几何信息
			std::vector<Vector3d> vecCorner;
			BoundingBox boxGeode;
			RenderOperationGeode* pGeode = new RenderOperationGeode();
			std::vector<Skeleton*> vecGeometry;
			TranslateGeometries(nodeInfo.GetGeometries(), nodeInfo, matInfo, vecGeometry);
			if (vecGeometry.empty())
			{
				//没有geometry时通过node的obb计算包围盒
				BoundingBox boxGeometry;
				BoundingBox boxNode;
				nodeInfo.GetObb().ToBoundingBox(boxNode);

				Vector3d vMax = boxNode.GetMax() - vPos;
				Vector3d vMin = boxNode.GetMin() - vPos;
				boxGeometry = BoundingBox(vMin, vMax);

				boxGeode.Merge(boxGeometry);
				const Vector3d* pCorners = boxGeometry.GetCorners();
				for (unsigned int j = 0; j < 8; j++)
				{
					vecCorner.push_back(pCorners[j]);
				}
			}
			else
			{
				for (unsigned int i = 0; i < vecGeometry.size(); i++)
				{
					Skeleton* pGeometry = vecGeometry[i];
					if (pGeometry != nullptr)
					{
						BoundingBox boxGeometry;
						pGeometry->ComputerBoundingBox();
						boxGeometry = pGeometry->m_BoundingBox;
						boxGeometry = BoundingBoxUtils::MultiplyMatrix(boxGeometry, matGeode);

						boxGeode.Merge(boxGeometry);
						const Vector3d* pCorners = boxGeometry.GetCorners();
						for (unsigned int j = 0; j < 8; j++)
						{
							vecCorner.push_back(pCorners[j]);
						}

						pGeode->AddSkeleton(pGeometry);
						pGroup->AddSkeleton(pGeometry->m_strGeoName, pGeometry);
					}
				}
			}

			vecTotalCorner.insert(vecTotalCorner.end(), vecCorner.begin(), vecCorner.end());
			pGeode->SetPosition(matGeode);
			//这里不设置叶子节点的包围盒，由s3mb插件存储的时候计算obb
			BoundingSphere speGeode(boxGeode);
			pGeode->SetBoundingSphere(speGeode.GetCenter(), speGeode.GetRadius());
			boxTotal.Merge(boxGeode);

			const std::vector<std::wstring>& vecChildNode = nodeInfo.GetChildNodes();
			if (!vecChildNode.empty())
			{
				OrientedBoundingBox obbLOD;
				obbLOD.FromPoints(vecCorner);

				RenderOperationPagedLOD* pLOD = new RenderOperationPagedLOD();
				pLOD->SetBoundingBox(boxGeode);
				pLOD->SetBoundingSphere(speGeode.GetCenter(), speGeode.GetRadius());
				pLOD->SetOrientedBoundingBox(obbLOD);
				pLOD->SetRangeMode(Geometric_Error);

				float fRangeValue = nodeInfo.GetRangeValue();
				if (EQUAL(fRangeValue, 0.0))
				{
					fRangeValue = EP;
				}
				fRangeValue = speGeode.GetRadius() * 16.0f / fRangeValue;

				std::vector<std::pair<float, float> >& vecRange = pLOD->GetRanges();
				vecRange.push_back(std::make_pair(0.0, fRangeValue));

				std::wstring strFileName = nodeInfo.GetId() + FILE_EXT_S3MB;
				std::vector<std::wstring>& vecFileName = pLOD->GetFileNames();
				vecFileName.push_back(strFileName);

				pLOD->AddChild(pGeode);
				pLOD->SetParentNode(pGroup);
				pGroup->AddChild(pLOD);

				std::wstring strChildOutputPath = StringUtil::GetDir(strOutputPath) + strFileName;
				mapChildDir[strChildOutputPath] = vecChildNode;
			}
			else
			{
				pGroup->AddChild(pGeode);
			}

			//解析属性
			TranslateAttributes(nodeInfo.GetAttributes(), mapFeature);
		}

		std::wstring strOutputDir = StringUtil::GetDir(strOutputPath);
		if (!StringUtil::IsFileExist(strOutputDir))
		{
			FileUtil::MkDirEx(strOutputDir);
		}

		OrientedBoundingBox obbGroup;
		obbGroup.FromPoints(vecTotalCorner);
		if (bRootNode)
		{
			box = boxTotal;
			obb = obbGroup;
		}

		pGroup->SetOrientedBoundingBox(obbGroup);
		pGroup->SetBoundingBox(boxTotal);

		S3MBWriter writer;
		writer.SetVersion(m_I3SParams.GetS3MVersion());
		writer.SetROGroup(pGroup);
		if (m_I3SParseParams.GetVertexCompressionType() == SV_DracoCompressed &&
			m_S3MBConfig.GetDataType() == SDT_RealityMesh)
		{
			//Draco压缩
			DracoCompressParam dracoParam;
			dracoParam.SetEnable(true);
			SkeletonCompressParam compressParam(dracoParam);
			writer.SetSkeletonCompressParam(compressParam);
		}
		bool bWritten = writer.WriteFile(strOutputPath);
		delete pGroup;
		pGroup = nullptr;

		std::map<std::wstring, std::vector<std::wstring> >::iterator iterDir;
		for (iterDir = mapChildDir.begin(); iterDir != mapChildDir.end(); iterDir++)
		{
			std::vector<std::wstring> vecChildDir = iterDir->second;
			TranslateNode(vecChildDir, iterDir->first, false, box, obb, mapFeature);
		}

		return true;
	}

	bool I3SNodesTranslation::TranslatePointCloudNode(const std::map<std::wstring, I3SNodeInfo>& mapNodeInfo, const std::vector<std::wstring>& vecDir, 
		const std::wstring& strOutputPath, bool bRootNode, BoundingBox& box, OrientedBoundingBox& obb)
	{
		if (vecDir.empty() || mapNodeInfo.empty())
		{
			return false;
		}

		std::vector<Vector3d> vecTotalCorner;
		BoundingBox boxTotal;
		std::map<std::wstring, std::vector<std::wstring> > mapChildDir;
		RenderOperationGroup* pGroup = new RenderOperationGroup();
		for (unsigned int i = 0; i < vecDir.size(); i++)
		{
			std::wstring strNodeId = StringUtil::GetTitle(vecDir[i]);
			if (mapNodeInfo.find(strNodeId) == mapNodeInfo.end())
			{
				continue;
			}

			I3SNodeInfo nodeInfo = mapNodeInfo.at(strNodeId);

			//计算geode矩阵
			Point3D pntPos = m_S3MBConfig.GetPosition();
			Vector3d vPos(pntPos.x, pntPos.y, pntPos.z);
			Vector3d vNod = nodeInfo.GetObb().GetCenter();
			Matrix4d matPos, matNod;
			GetMatrix(vPos, matPos);
			GetMatrix(vNod, matNod);
			Matrix4d matGeode = matNod * matPos.Invert();

			//解析几何信息
			std::vector<Vector3d> vecCorner;
			BoundingBox boxGeode;
			RenderOperationGeode* pGeode = new RenderOperationGeode();
			std::vector<Skeleton*> vecGeometry;
			TranslatePointCloudGeometries(nodeInfo.GetGeometries(), nodeInfo, vecGeometry);
			for (unsigned int i = 0; i < vecGeometry.size(); i++)
			{
				Skeleton* pGeometry = vecGeometry[i];
				if (pGeometry != nullptr)
				{
					BoundingBox boxGeometry;
					pGeometry->ComputerBoundingBox();
					boxGeometry = pGeometry->m_BoundingBox;
					boxGeometry = BoundingBoxUtils::MultiplyMatrix(boxGeometry, matGeode);

					boxGeode.Merge(boxGeometry);
					const Vector3d* pCorners = boxGeometry.GetCorners();
					for (unsigned int j = 0; j < 8; j++)
					{
						vecCorner.push_back(pCorners[j]);
					}

					pGeode->AddSkeleton(pGeometry);
					pGroup->AddSkeleton(pGeometry->m_strGeoName, pGeometry);
				}
			}
			vecTotalCorner.insert(vecTotalCorner.end(), vecCorner.begin(), vecCorner.end());
			pGeode->SetPosition(matGeode);
			//这里不设置叶子节点的包围盒，由s3mb插件存储的时候计算obb
			BoundingSphere speGeode(boxGeode);
			pGeode->SetBoundingSphere(speGeode.GetCenter(), speGeode.GetRadius());
			boxTotal.Merge(boxGeode);

			const std::vector<std::wstring>& vecChildNode = nodeInfo.GetChildNodes();
			if (!vecChildNode.empty())
			{
				OrientedBoundingBox obbLOD;
				obbLOD.FromPoints(vecCorner);

				RenderOperationPagedLOD* pLOD = new RenderOperationPagedLOD();
				pLOD->SetBoundingBox(boxGeode);
				pLOD->SetBoundingSphere(speGeode.GetCenter(), speGeode.GetRadius());
				pLOD->SetOrientedBoundingBox(obbLOD);
				pLOD->SetRangeMode(Geometric_Error);

				float fRangeValue = nodeInfo.GetRangeValue();
				if (EQUAL(fRangeValue, 0.0))
				{
					fRangeValue = EP;
				}
				fRangeValue = speGeode.GetRadius() * 16.0f / fRangeValue;

				std::vector<std::pair<float, float> >& vecRange = pLOD->GetRanges();
				vecRange.push_back(std::make_pair(0.0, fRangeValue));

				std::wstring strFileName = nodeInfo.GetId() + FILE_EXT_S3MB;
				std::vector<std::wstring>& vecFileName = pLOD->GetFileNames();
				vecFileName.push_back(strFileName);

				pLOD->AddChild(pGeode);
				pLOD->SetParentNode(pGroup);
				pGroup->AddChild(pLOD);

				std::wstring strChildOutputPath = StringUtil::GetDir(strOutputPath) + strFileName;
				mapChildDir[strChildOutputPath] = vecChildNode;
			}
			else
			{
				pGroup->AddChild(pGeode);
			}
		}

		std::wstring strOutputDir = StringUtil::GetDir(strOutputPath);
		if (!StringUtil::IsFileExist(strOutputDir))
		{
			FileUtil::MkDirEx(strOutputDir);
		}

		OrientedBoundingBox obbGroup;
		obbGroup.FromPoints(vecTotalCorner);
		if (bRootNode)
		{
			box = boxTotal;
			obb = obbGroup;
		}

		pGroup->SetOrientedBoundingBox(obbGroup);
		pGroup->SetBoundingBox(boxTotal);

		S3MBWriter writer;
		writer.SetVersion(m_I3SParams.GetS3MVersion());
		writer.SetROGroup(pGroup);
		if (m_I3SParseParams.GetVertexCompressionType() == SV_DracoCompressed)
		{
			//Draco压缩
			DracoCompressParam dracoParam;
			dracoParam.SetEnable(true);
			SkeletonCompressParam compressParam(dracoParam);
			writer.SetSkeletonCompressParam(compressParam);
		}
		bool bWritten = writer.WriteFile(strOutputPath);
		delete pGroup;
		pGroup = nullptr;

		std::map<std::wstring, std::vector<std::wstring> >::iterator iterDir;
		for (iterDir = mapChildDir.begin(); iterDir != mapChildDir.end(); iterDir++)
		{
			std::vector<std::wstring> vecChildDir = iterDir->second;
			TranslatePointCloudNode(mapNodeInfo, vecChildDir, iterDir->first, false, box, obb);
		}

		return true;
	}

	bool I3SNodesTranslation::TranslatePointNode(const std::vector<std::wstring>& vecDir, const std::wstring& strOutputPath, 
		bool bRootNode, BoundingBox& box, OrientedBoundingBox& obb, std::map<int, Feature*>& mapFeature)
	{
		std::vector<Vector3d> vecTotalCorner;
		BoundingBox boxTotal;
		std::map<std::wstring, std::vector<std::wstring> > mapChildDir;
		RenderOperationGroup* pGroup = new RenderOperationGroup();

		//每个节点的feature
		std::map<int, Feature*> mapNodeFeature;

		for (unsigned int i = 0; i < vecDir.size(); i++)
		{
			std::wstring strtDocPath = vecDir[i] + U("/") + I3S_FILE_3DNODEINDEXDOCUMENT;
			I3SNodeInfo nodeInfo;
			I3SParser::ParseNodeDoc(m_slpkFile, m_I3SParseParams, strtDocPath, nodeInfo);

			std::wstring strGeoPath = U("./geometries/0");
			strGeoPath = StringUtil::GetAbsolutePath(StringUtil::GetDir(strtDocPath), strGeoPath);
			std::vector<std::wstring> vecGeoPath;
			vecGeoPath.push_back(strGeoPath);
			nodeInfo.SetGeometries(vecGeoPath);

			//解析属性
			TranslateAttributes(nodeInfo.GetAttributes(), mapNodeFeature);

			//计算geode矩阵
			Point3D pntPos = m_S3MBConfig.GetPosition();
			Vector3d vPos(pntPos.x, pntPos.y, pntPos.z);
			Vector3d vNod = nodeInfo.GetObb().GetCenter();
			Matrix4d matPos, matNod;
			GetMatrix(vPos, matPos);
			GetMatrix(vNod, matNod);
			Matrix4d matGeode = matNod * matPos.Invert();

			//解析几何信息
			std::vector<Vector3d> vecCorner;
			BoundingBox boxGeode;
			RenderOperationGeode* pGeode = new RenderOperationGeode();
			std::vector<Skeleton*> vecGeometry;
			TranslatePointGeometries(nodeInfo.GetGeometries(), nodeInfo, mapNodeFeature, vecGeometry);
			for (unsigned int i = 0; i < vecGeometry.size(); i++)
			{
				Skeleton* pGeometry = vecGeometry[i];
				if (pGeometry == nullptr)
				{
					continue;
				}

				BoundingBox boxGeometry = pGeometry->m_BoundingBox;
				boxGeometry = BoundingBoxUtils::MultiplyMatrix(boxGeometry, matGeode);
				boxGeode.Merge(boxGeometry);
				const Vector3d* pCorners = boxGeometry.GetCorners();
				for (unsigned int j = 0; j < 8; j++)
				{
					vecCorner.push_back(pCorners[j]);
				}

				pGeode->AddSkeleton(pGeometry);
				pGroup->AddSkeleton(pGeometry->m_strGeoName, pGeometry);
			}
			vecTotalCorner.insert(vecTotalCorner.end(), vecCorner.begin(), vecCorner.end());
			pGeode->SetPosition(matGeode);
			//这里不设置叶子节点的包围盒，由s3mb插件存储的时候计算obb
			BoundingSphere speGeode(boxGeode);
			pGeode->SetBoundingSphere(speGeode.GetCenter(), speGeode.GetRadius());
			boxTotal.Merge(boxGeode);

			const std::vector<std::wstring>& vecChildNode = nodeInfo.GetChildNodes();
			if (!vecChildNode.empty())
			{
				OrientedBoundingBox obbLOD;
				obbLOD.FromPoints(vecCorner);

				RenderOperationPagedLOD* pLOD = new RenderOperationPagedLOD();
				pLOD->SetBoundingBox(boxGeode);
				pLOD->SetBoundingSphere(speGeode.GetCenter(), speGeode.GetRadius());
				pLOD->SetOrientedBoundingBox(obbLOD);
				pLOD->SetRangeMode(Geometric_Error);

				float fRangeValue = nodeInfo.GetRangeValue();
				if (EQUAL(fRangeValue, 0.0))
				{
					fRangeValue = EP;
				}
				fRangeValue = speGeode.GetRadius() * 16.0f / fRangeValue;

				std::vector<std::pair<float, float> >& vecRange = pLOD->GetRanges();
				vecRange.push_back(std::make_pair(0.0, fRangeValue));

				std::wstring strFileName = nodeInfo.GetId() + FILE_EXT_S3MB;
				std::vector<std::wstring>& vecFileName = pLOD->GetFileNames();
				vecFileName.push_back(strFileName);

				pLOD->AddChild(pGeode);
				pLOD->SetParentNode(pGroup);
				pGroup->AddChild(pLOD);

				std::wstring strChildOutputPath = StringUtil::GetDir(strOutputPath) + strFileName;
				mapChildDir[strChildOutputPath] = vecChildNode;
			}
			else
			{
				pGroup->AddChild(pGeode);
			}
		}

		std::wstring strOutputDir = StringUtil::GetDir(strOutputPath);
		if (!StringUtil::IsFileExist(strOutputDir))
		{
			FileUtil::MkDirEx(strOutputDir);
		}

		OrientedBoundingBox obbGroup;
		obbGroup.FromPoints(vecTotalCorner);
		if (bRootNode)
		{
			box = boxTotal;
			obb = obbGroup;
		}

		pGroup->SetOrientedBoundingBox(obbGroup);
		pGroup->SetBoundingBox(boxTotal);

		S3MBWriter writer;
		writer.SetVersion(m_I3SParams.GetS3MVersion());
		writer.SetROGroup(pGroup);
		bool bWritten = writer.WriteFile(strOutputPath);
		delete pGroup;
		pGroup = nullptr;

		std::copy(mapNodeFeature.begin(), mapNodeFeature.end(), std::inserter(mapFeature, mapFeature.begin()));
		mapNodeFeature.clear();

		std::map<std::wstring, std::vector<std::wstring> >::iterator iterDir;
		for (iterDir = mapChildDir.begin(); iterDir != mapChildDir.end(); iterDir++)
		{
			std::vector<std::wstring> vecChildDir = iterDir->second;
			TranslatePointNode(vecChildDir, iterDir->first, false, box, obb, mapFeature);
		}

		return true;
	}

	void I3SNodesTranslation::TranslateMaterial(const I3SMaterialInfo& matInfo, const std::vector<std::wstring>& vecTexture, Material*& pMaterial)
	{
		if (pMaterial == nullptr)
		{
			pMaterial = new Material();
		}

		pMaterial->m_strName = matInfo.GetName();
		Technique* pTech = pMaterial->CreateTechnique();
		Pass* pPass = pTech->CreatePass();
		pPass->m_strName = pMaterial->m_strName;
		pPass->m_Ambient = matInfo.GetAmbient();
		pPass->m_Specular = matInfo.GetSpecular();
		pPass->m_Specular = matInfo.GetSpecular();
		pPass->m_Shininess = matInfo.GetShininess();
		pPass->m_CullMode = matInfo.GetCullMode();
		if (m_I3SParseParams.GetHasDrawingInfo() &&
			m_I3SParseParams.GetDrawingInfo().GetHasMaterialColor() &&
			m_I3SParseParams.GetDrawingInfo().GetColorMixMode() == I3SDrawingInfo::CMM_Replace)
		{
			pPass->m_Diffuse = m_I3SParseParams.GetDrawingInfo().GetMaterialColor();
		}
		else
		{
			pPass->m_Diffuse = matInfo.GetDiffuse();
		}

		if (!vecTexture.empty())
		{
			const std::map<std::wstring, std::wstring>& mapTexFormat = m_I3SParseParams.GetTextureFormats();
			std::wstring strTexName = vecTexture[0];
			std::wstring strTitle = StringUtil::GetTitle(strTexName);
			std::wstring strFormat = mapTexFormat.at(strTitle);
			if (!strFormat.empty())
			{
				std::wstring strTexPath;
				if (StringUtil::CompareNoCase(strFormat, I3S_JSON_SCENELAYER_DDS))
				{
					strTexPath = strTexName + I3S_EXT_BINDDS;
				}
				else
				{
					strTexPath = strTexName + U(".") + strFormat;
				}

				TextureUnitState* pUnitState = pPass->CreateTextureUnitState();
				pUnitState->m_strTextureName = strTexPath;
			}
		}
	}

	bool I3SNodesTranslation::TranslateTextures(const std::vector<std::wstring>& vecTexture, std::map<std::wstring, TextureDataInfo*>& mapTexInfo)
	{
		const std::map<std::wstring, std::wstring>& mapTexFormat = m_I3SParseParams.GetTextureFormats();
		for (unsigned int i = 0; i < vecTexture.size(); i++)
		{
			std::wstring strTitle = StringUtil::GetTitle(vecTexture[i]);
			std::wstring strFormat = mapTexFormat.at(strTitle);
			if (strFormat.empty())
			{
				continue;
			}

			bool bGz = false;
			std::wstring strTexPath;
			std::wstring strTexExt;
			if (StringUtil::CompareNoCase(strFormat, I3S_JSON_SCENELAYER_DDS))
			{
				bGz = true;
				strTexPath = vecTexture[i] + I3S_EXT_BINDDS;
				strTexExt = I3S_EXT_DDS;
			}
			else
			{
				bGz = false;
				strTexPath = vecTexture[i] + U(".") + strFormat;
				strTexExt = U(".") + strFormat;
			}

			TextureDataInfo* pTexture = nullptr;
			TranslateTexture(strTexPath, strTexExt, pTexture, bGz);
			if (pTexture != nullptr)
			{
				mapTexInfo[strTexPath] = pTexture;
			}
		}

		return true;
	}

	bool I3SNodesTranslation::TranslateTexture(const std::wstring& strName, const std::wstring& strExt, TextureDataInfo*& pTexture, bool bGz)
	{
		unsigned long long nBufferSize = 0;
		unsigned char* pBuffer = nullptr;
		if (!m_slpkFile.GetBuffer(strName, pBuffer, nBufferSize))
		{
			delete[] pBuffer;
			pBuffer = nullptr;
			return false;
		}

		std::wstring strOutputPath = m_I3SParams.GetOutputPath();
		std::wstring strTexPath = BaseUtils::CreateGuid() + strExt;
		strTexPath = StringUtil::GetAbsolutePath(strOutputPath, strTexPath);

		std::string cstrTexPath = StringUtil::UnicodeToANSI(strTexPath);

		if (pBuffer != nullptr)
		{
			if (!StringUtil::IsFileExist(strOutputPath))
			{
				FileUtil::MkDirEx(strOutputPath);
			}

			FILE* pFile = fopen(cstrTexPath.c_str(), "wb");
			fwrite((unsigned char*)pBuffer, sizeof(unsigned char), nBufferSize, pFile);
			fclose(pFile);

			nBufferSize = 0;
			delete[] pBuffer;
			pBuffer = nullptr;
		}

		TextureData* pTextrueData = ImgToolkit::GetTextureData(strTexPath, false);
		std::remove(cstrTexPath.c_str());

		if (pTextrueData == nullptr || pTextrueData->m_pBuffer == nullptr || pTextrueData->m_nSize == 0)
		{
			delete pTextrueData;
			pTextrueData = nullptr;
			return false;
		}

		delete pTexture;
		pTexture = new TextureDataInfo();
		pTexture->m_pTextureData = pTextrueData;

		if (m_S3MBConfig.GetDataType() == SCPSDataType::SDT_ArtificialModel ||
			m_S3MBConfig.GetDataType() == SCPSDataType::SDT_BIM)
		{
			TextureUtils::UpdateTextureTier(pTexture, pTextrueData->m_eCompressType);
		}

		return true;
	}

	bool I3SNodesTranslation::TranslateGeometries(const std::vector<std::wstring>& vecGeoPath, const I3SNodeInfo& nodeInfo,
		const I3SMaterialInfo& matInfo, std::vector<Skeleton*>& vecGeometry)
	{
		for (unsigned int i = 0; i < vecGeoPath.size(); i++)
		{
			std::wstring strGeoPath = vecGeoPath[i] + I3S_EXT_BINGZ;
			Skeleton* pGeometry = nullptr;

			if (m_I3SParseParams.GetVertexCompressionType() == SV_DracoCompressed)
			{
				//暂时这么处理
				std::wstring strGeoDir = StringUtil::GetDir(strGeoPath);
				strGeoPath = strGeoDir + U("1.bin.gz");
			}

			TranslateGeometry(strGeoPath, nodeInfo, matInfo, pGeometry);
			if (pGeometry != nullptr)
			{
				vecGeometry.push_back(pGeometry);
			}
		}

		return true;
	}

	bool I3SNodesTranslation::TranslateGeometry(const std::wstring& strName, const I3SNodeInfo& nodeInfo, const I3SMaterialInfo& matInfo, Skeleton*& pGeometry)
	{
		unsigned long long nBufferSize = 0;
		unsigned char* pBuffer = nullptr;
		if (!m_slpkFile.GetBuffer(strName, pBuffer, nBufferSize))
		{
			delete[] pBuffer;
			pBuffer = nullptr;
			return false;
		}

		bool bResult = false;
		if (pGeometry == nullptr)
		{
			pGeometry = new Skeleton();
		}

		if (m_I3SParseParams.GetVertexCompressionType() == SV_Standard)
		{
			bResult = TranslateStandardGeometry(strName, nodeInfo, matInfo, (unsigned char*)pBuffer, nBufferSize, pGeometry);
		}
		else if (m_I3SParseParams.GetVertexCompressionType() == SV_DracoCompressed)
		{
			bResult = TranslateDracoCompressedGeometry(strName, nodeInfo, matInfo, (char*)pBuffer, nBufferSize, pGeometry);
		}

		delete[] pBuffer;
		pBuffer = nullptr;
		return bResult;
	}

	bool I3SNodesTranslation::TranslateStandardGeometry(const std::wstring& strName, const I3SNodeInfo& nodeInfo, const I3SMaterialInfo& matInfo, 
		unsigned char* pData, size_t nDataSize, Skeleton*& pGeometry)
	{
		MemoryStream stream;
		stream.Init(pData, nDataSize);
		
		const std::vector<std::pair<std::wstring, int> >& vecVexAttAndId = m_I3SParseParams.GetVertexAttributeAndIdRelation();
		const std::vector<std::pair<std::wstring, int> >& vecVexAttExtAndId = nodeInfo.GetVertexAttributeExtensionAndIdRelation();

		VertexDataPackage* pDataPackage = new VertexDataPackage();
		IndexPackage* pIndexPackage = new IndexPackage();
		unsigned int nFeatureCount = 0;
		stream >> pDataPackage->m_nVerticesCount;
		stream >> nFeatureCount;
		std::vector<IDInfo*> vecIDInfo;
		if (pDataPackage->m_nVerticesCount > 0)
		{
			for (unsigned int i = 0; i < vecVexAttAndId.size(); i++)
			{
				TranslateVertexData(stream, nodeInfo, vecVexAttAndId[i].first, pDataPackage);
			}

			for (unsigned int i = 0; i < vecVexAttExtAndId.size(); i++)
			{
				TranslateVertexExtensionData(stream, vecVexAttExtAndId[i].first, pDataPackage);
			}

			TranslateFeatureData(stream, vecVexAttExtAndId, nFeatureCount, vecIDInfo);

			if (!m_I3SParseParams.GetHasFaceAttributes())
			{
				pIndexPackage->m_enIndexType = IT_16BIT;
				//pIndexPackage->m_enIndexType = IT_32BIT;
				pIndexPackage->m_OperationType = OT_TRIANGLE_LIST;
				pIndexPackage->m_nIndexesCount = pDataPackage->m_nVerticesCount;
				unsigned short* pIndexes = new(std::nothrow) unsigned short[pDataPackage->m_nVerticesCount];
				for (unsigned int i = 0; i < pDataPackage->m_nVerticesCount; i++)
				{
					pIndexes[i] = i;
				}
				pIndexPackage->m_pIndexes = (unsigned short *)pIndexes;
			}

			pIndexPackage->m_strPassName.push_back(matInfo.GetName());
		}

		pGeometry->m_strMaterialName = matInfo.GetName();
		pGeometry->m_strGeoName = strName;
		std::vector<IndexPackage*> arrIndexPackage;
		arrIndexPackage.push_back(pIndexPackage);
		SkeletonUtils::RebuildVertexAndIndexPackage(pDataPackage, arrIndexPackage, vecIDInfo);
		pGeometry->m_pVertexDataPackage = pDataPackage;
		pGeometry->m_arrIndexPackage = arrIndexPackage;
		pGeometry->m_arrIDInfo = vecIDInfo;
		return true;
	}

	bool I3SNodesTranslation::TranslateDracoCompressedGeometry(const std::wstring& strName, const I3SNodeInfo& nodeInfo, const I3SMaterialInfo& matInfo, 
		char* pData, size_t nDataSize, Skeleton*& pGeometry)
	{
		const std::vector<std::pair<std::wstring, int> >& vecVexAttExtAndId = nodeInfo.GetVertexAttributeExtensionAndIdRelation();

		bool bResult = false;
		std::vector<std::string> vecVexAttExtDescript;
		for (unsigned int i = 0; i < vecVexAttExtAndId.size(); i++)
		{
			if (StringUtil::CompareNoCase(vecVexAttExtAndId[i].first, ATT_UVREGION))
			{
				vecVexAttExtDescript.push_back(StringUtil::UnicodeToANSI(I3S_JSON_COMPRESSED_UVREGION));
			}
		}
		VertexDataPackage* pDataPackage = nullptr;
		std::vector<IndexPackage*> vecIndexPackage;
		unsigned int* pFeatureIds = nullptr;
		unsigned int nFeatureIdCount = 0;
		if (DracoTool::DracoDecompress(vecVexAttExtDescript, pData, nDataSize, false, pDataPackage, vecIndexPackage, pFeatureIds, nFeatureIdCount))
		{
			if (pDataPackage != nullptr)
			{
				Vector3d vNodePos = nodeInfo.GetObb().GetCenter();
				Matrix4d matNod, matNodInv;
				GetMatrix(vNodePos, matNod);
				matNodInv = matNod.Invert();

				for (unsigned int i = 0; i < pDataPackage->m_nVerticesCount; i++)
				{
					Vector3d vVertex;
					vVertex.x = pDataPackage->m_pVertices[i * 3 + 0] + vNodePos.x;
					vVertex.y = pDataPackage->m_pVertices[i * 3 + 1] + vNodePos.y;
					vVertex.z = pDataPackage->m_pVertices[i * 3 + 2] + vNodePos.z;

					Matrix4d matVex;
					GetMatrix(vVertex, matVex);
					matVex = matVex * matNodInv;
					Vector3d vNewVertex;
					vNewVertex = vNewVertex.MultiplyMatrix(matVex);

					pDataPackage->m_pVertices[i * 3 + 0] = vNewVertex.x;
					pDataPackage->m_pVertices[i * 3 + 1] = vNewVertex.y;
					pDataPackage->m_pVertices[i * 3 + 2] = vNewVertex.z;
				}

				//顶点扩展属性的语义信息
				pDataPackage->m_mapVertexAttributeDescript.clear();
				TranslateDracoCompVertexExtensionData(vecVexAttExtAndId, pDataPackage);
			}

			if (!vecIndexPackage.empty())
			{
				vecIndexPackage[0]->m_strPassName.push_back(matInfo.GetName());
			}

			std::vector<IDInfo*> vecIDInfo;
			if (pFeatureIds != nullptr)
			{
				TranslateFeatureData(pFeatureIds, nFeatureIdCount, vecIDInfo);
				delete[] pFeatureIds;
				pFeatureIds = nullptr;
			}

			pGeometry->m_strMaterialName = matInfo.GetName();
			pGeometry->m_strGeoName = strName;
			SkeletonUtils::RebuildVertexAndIndexPackage(pDataPackage, vecIndexPackage, vecIDInfo);
			pGeometry->m_pVertexDataPackage = pDataPackage;
			pGeometry->m_arrIndexPackage = vecIndexPackage;
			pGeometry->m_arrIDInfo = vecIDInfo;
			bResult = true;
		}
		return bResult;
	}

	bool I3SNodesTranslation::TranslatePointCloudGeometries(const std::vector<std::wstring>& vecGeoPath, const I3SNodeInfo& nodeInfo, std::vector<Skeleton*>& vecGeometry)
	{
		for (unsigned int i = 0; i < vecGeoPath.size(); i++)
		{
			std::wstring strGeoPath = vecGeoPath[i] + I3S_EXT_BINPCCXYZ;
			Skeleton* pGeometry = nullptr;
			TranslatePointCloudGeometry(strGeoPath, nodeInfo, pGeometry);
			if (pGeometry != nullptr)
			{
				vecGeometry.push_back(pGeometry);
			}
		}

		return true;
	}

	bool I3SNodesTranslation::TranslatePointCloudGeometry(const std::wstring& strName, const I3SNodeInfo& nodeInfo, Skeleton*& pGeometry)
	{
		VertexDataPackage* pDataPackage = nullptr;
		if (!TranslatePointCloudVertexData(strName, nodeInfo, pDataPackage))
		{
			delete pDataPackage;
			pDataPackage = nullptr;
			return false;
		}

		std::wstring strDir = StringUtil::GetAbsolutePath(strName, U("../../"));

		const std::vector<I3SAttStorageInfo>& vecInfo = m_I3SParseParams.GetAttStorageInfos();
		for (unsigned int i = 0; i < vecInfo.size(); i++)
		{
			std::wstring strKey = vecInfo[i].GetKey();
			if (StringUtil::CompareNoCase(strKey, I3S_KEY_INTENSITY))
			{
				std::wstring strIntensityPath = strDir + U("attributes/") + I3S_KEY_INTENSITY + I3S_EXT_BINPCCINT;
				TranslatePointCloudIntensityData(strIntensityPath, pDataPackage);
			}
			else if (StringUtil::CompareNoCase(strKey, I3S_KEY_RGB))
			{
				std::wstring strRGBPath = strDir + U("attributes/") + I3S_KEY_RGB + I3S_EXT_BINPCCRGB;
				TranslatePointCloudVertexColorData(strRGBPath, pDataPackage);
			}
			else if (StringUtil::CompareNoCase(strKey, I3S_KEY_CLASSCODE))
			{
				std::wstring strClassCodePath = strDir + U("attributes/") + I3S_KEY_CLASSCODE + I3S_EXT_BINGZ;
				TranslatePointCloudClassCodeData(strClassCodePath, pDataPackage);
			}
		}

		if (pGeometry == nullptr) pGeometry = new Skeleton();
		pGeometry->m_strGeoName = strName;
		pGeometry->m_pVertexDataPackage = pDataPackage;
		return true;
	}

	bool I3SNodesTranslation::TranslatePointGeometries(const std::vector<std::wstring>& vecGeoPath, const I3SNodeInfo& nodeInfo, 
		std::map<int, Feature*>& mapFeature, std::vector<Skeleton*>& vecGeometry)
	{
		for (unsigned int i = 0; i < vecGeoPath.size(); i++)
		{
			std::wstring strGeoPath = vecGeoPath[i] + I3S_EXT_BINGZ;
			TranslatePointGeometry(strGeoPath, nodeInfo, mapFeature, vecGeometry);
		}

		return true;
	}

	bool I3SNodesTranslation::TranslatePointGeometry(const std::wstring& strName, const I3SNodeInfo& nodeInfo, 
		std::map<int, Feature*>& mapFeature, std::vector<Skeleton*>& vecGeometry)
	{
		unsigned long long nBufferSize = 0;
		unsigned char* pBuffer = nullptr;
		if (!m_slpkFile.GetBuffer(strName, pBuffer, nBufferSize))
		{
			delete[] pBuffer;
			pBuffer = nullptr;
			return false;
		}
		const std::vector<std::pair<std::wstring, int> >& vecVexAttExtAndId = nodeInfo.GetVertexAttributeExtensionAndIdRelation();

		bool bResult = false;
		std::vector<std::string> vecVexAttExtDescript;
		for (unsigned int i = 0; i < vecVexAttExtAndId.size(); i++)
		{
			if (StringUtil::CompareNoCase(vecVexAttExtAndId[i].first, ATT_UVREGION))
			{
				vecVexAttExtDescript.push_back(StringUtil::UnicodeToANSI(I3S_JSON_COMPRESSED_UVREGION));
			}
		}

		VertexDataPackage* pDataPackage = nullptr;
		std::vector<IndexPackage*> arrIndexPackage;
		unsigned int* pFeatureIds = nullptr;
		unsigned int nFeatureIdCount = 0;
		//点没有索引数据
		DracoTool::DracoDecompress(vecVexAttExtDescript, (char*)pBuffer, nBufferSize, true, pDataPackage, arrIndexPackage, pFeatureIds, nFeatureIdCount);
		delete[] pBuffer;
		pBuffer = nullptr;

		if (pDataPackage == nullptr || pDataPackage->m_nVerticesCount == 0)
		{
			delete pDataPackage;
			pDataPackage = nullptr;
			delete[] pFeatureIds;
			pFeatureIds = nullptr;
			return false;
		}

		I3SDrawingInfo drawingInfo = m_I3SParseParams.GetDrawingInfo();
		//单一外挂模型
		if (drawingInfo.GetRendererType() == I3SDrawingInfo::RT_Simple &&
			drawingInfo.GetSymbolType() == I3SDrawingInfo::ST_StyleReference)
		{
			std::wstring strSymbolName = drawingInfo.GetDefaultSymbolRef();
			std::map<std::wstring, Matrix4d> matTrans = drawingInfo.GetSymbolTransforms();

			Matrix4d matSymbol;
			if (matTrans.find(strSymbolName) != matTrans.end())
			{
				matSymbol = matTrans.at(strSymbolName);
			}

			// 第一重纹理坐标存矩阵信息
			pDataPackage->m_nTexDimensions[0] = SIZE_PER_INSTANCE;
			pDataPackage->SetTexCoordsNum(0, pDataPackage->m_nVerticesCount);

			float* pTexCoords0 = pDataPackage->m_pTexCoords[0];

			Vector3d vNodePos = nodeInfo.GetObb().GetCenter();
			Matrix4d matNod, matNodInv;
			GetMatrix(vNodePos, matNod);
			matNodInv = matNod.Invert();

			for (unsigned int i = 0; i < pDataPackage->m_nVerticesCount; i++)
			{
				Vector3d vVertex;
				vVertex.x = pDataPackage->m_pVertices[i * 3 + 0] + vNodePos.x;
				vVertex.y = pDataPackage->m_pVertices[i * 3 + 1] + vNodePos.y;
				vVertex.z = pDataPackage->m_pVertices[i * 3 + 2] + vNodePos.z;

				Matrix4d matVex;
				GetMatrix(vVertex, matVex);
				matVex = matSymbol * matVex * matNodInv;

				Vector3d vNewVertex;
				vNewVertex = vNewVertex.MultiplyMatrix(matVex);

				pDataPackage->m_pVertices[i * 3 + 0] = vNewVertex.x;
				pDataPackage->m_pVertices[i * 3 + 1] = vNewVertex.y;
				pDataPackage->m_pVertices[i * 3 + 2] = vNewVertex.z;

				for (int j = 0; j < 3; j++)
				{
					for (int k = 0; k < 4; k++)
					{
						*pTexCoords0++ = static_cast<float>(matVex[k][j]);
					}
				}
				*pTexCoords0++ = ColorUtils::White.r;
				*pTexCoords0++ = ColorUtils::White.g;
				*pTexCoords0++ = ColorUtils::White.b;
				*pTexCoords0++ = ColorUtils::White.a;

				if (i < nFeatureIdCount)
				{
					// 需要转化为uint指针后再写入值才不会有精度损失
					unsigned int* pUint = (unsigned int*)pTexCoords0;
					*pUint++ = pFeatureIds[i];
				}

				pTexCoords0++;
			}

			std::vector<IDInfo*> vecIDInfo;
			if (pFeatureIds != nullptr)
			{
				TranslateFeatureData(pFeatureIds, nFeatureIdCount, vecIDInfo);
			}

			//顶点扩展属性的语义信息
			pDataPackage->m_mapVertexAttributeDescript.clear();
			TranslateDracoCompVertexExtensionData(vecVexAttExtAndId, pDataPackage);

			Skeleton* pGeometry = new Skeleton();
			pGeometry->m_pVertexDataPackage = pDataPackage;
			pGeometry->ComputerBoundingBox();
			delete[] pGeometry->m_pVertexDataPackage->m_pVertices;
			pGeometry->m_pVertexDataPackage->m_pVertices = nullptr;
			pGeometry->m_pVertexDataPackage->m_nVerticesCount = 0;
			pGeometry->m_arrIndexPackage = arrIndexPackage;
			pGeometry->m_arrIDInfo = vecIDInfo;

			if (!strSymbolName.empty())
			{
				std::wstring strGeoName = StringUtil::ChangeExt(strSymbolName, FILE_EXT_S3MB);
				pGeometry->m_strGeoName = strGeoName;
			}

			vecGeometry.push_back(pGeometry);
		}
		else if (drawingInfo.GetRendererType() == I3SDrawingInfo::RT_UniqueValue)
		{
			if (pDataPackage->m_nVerticesCount != nFeatureIdCount)
			{
				delete pDataPackage;
				pDataPackage = nullptr;
				delete[] pFeatureIds;
				pFeatureIds = nullptr;

				std::cout << "Data Error" << std::endl;
				assert(false);
				return false;
			}

			Vector3d vNodePos = nodeInfo.GetObb().GetCenter();
			Matrix4d matNod, matNodInv;
			GetMatrix(vNodePos, matNod);
			matNodInv = matNod.Invert();

			std::wstring strFieldName = drawingInfo.GetFieldName();
			std::map<Variant, std::wstring> mapFieldSymbol = drawingInfo.GetFieldSymbols();
			std::map<std::wstring, Matrix4d> matTrans = drawingInfo.GetSymbolTransforms();

			//建立符号名称和序号的对应关系
			std::map<std::wstring, std::vector<unsigned int> > mapSymbolAndIndex;
			for (unsigned int i = 0; i < nFeatureIdCount; i++)
			{
				unsigned int nId = pFeatureIds[i];
				if (mapFeature.find(nId) == mapFeature.end())
				{
					continue;
				}

				Feature* pFeature = mapFeature.at(nId);
				if (pFeature == nullptr)
				{
					continue;
				}

				//根据属性值取符号名称，若不存在使用默认的符号名称
				std::wstring strSymbolName;
				Variant varFieldValue;
				pFeature->GetValue(strFieldName, varFieldValue);
				if (mapFieldSymbol.find(varFieldValue) != mapFieldSymbol.end())
				{
					strSymbolName = mapFieldSymbol.at(varFieldValue);
				}
				else
				{
					strSymbolName = drawingInfo.GetDefaultSymbolRef();
				}

				if (strSymbolName.empty())
				{
					continue;
				}

				if (mapSymbolAndIndex.find(strSymbolName) == mapSymbolAndIndex.end())
				{
					std::vector<unsigned int> vecEmpty;
					mapSymbolAndIndex[strSymbolName] = vecEmpty;
				}

				mapSymbolAndIndex[strSymbolName].push_back(i);
			}

			std::map<std::wstring, std::vector<unsigned int> >::iterator iter;
			for (iter = mapSymbolAndIndex.begin(); iter != mapSymbolAndIndex.end(); iter++)
			{
				std::wstring strSymbolName = iter->first;
				std::vector<unsigned int> vecIndex = iter->second;
				unsigned int nVertexCount = vecIndex.size();
				if (strSymbolName.empty() ||
					nVertexCount == 0)
				{
					continue;
				}

				Matrix4d matSymbol;
				if (matTrans.find(strSymbolName) != matTrans.end())
				{
					matSymbol = matTrans.at(strSymbolName);
				}

				VertexDataPackage* pSplitDataPackage = new VertexDataPackage();
				pSplitDataPackage->SetVertexNum(nVertexCount);
				// 第一重纹理坐标存矩阵信息
				pSplitDataPackage->m_nTexDimensions[0] = SIZE_PER_INSTANCE;
				pSplitDataPackage->SetTexCoordsNum(0, nVertexCount);

				float* pTexCoords0 = pSplitDataPackage->m_pTexCoords[0];

				unsigned int* pSplitFeatureIds = new unsigned int[nVertexCount];
				unsigned int nSplitFeatureIdCount = nVertexCount;

				for (unsigned int i = 0; i < nVertexCount; i++)
				{
					unsigned int iOri = vecIndex[i];

					Vector3d vVertex;
					vVertex.x = pDataPackage->m_pVertices[iOri * 3 + 0] + vNodePos.x;
					vVertex.y = pDataPackage->m_pVertices[iOri * 3 + 1] + vNodePos.y;
					vVertex.z = pDataPackage->m_pVertices[iOri * 3 + 2] + vNodePos.z;

					Matrix4d matVex;
					GetMatrix(vVertex, matVex);
					matVex = matSymbol * matVex * matNodInv;

					Vector3d vNewVertex;
					vNewVertex = vNewVertex.MultiplyMatrix(matVex);

					pSplitDataPackage->m_pVertices[i * 3 + 0] = vNewVertex.x;
					pSplitDataPackage->m_pVertices[i * 3 + 1] = vNewVertex.y;
					pSplitDataPackage->m_pVertices[i * 3 + 2] = vNewVertex.z;

					for (int j = 0; j < 3; j++)
					{
						for (int k = 0; k < 4; k++)
						{
							*pTexCoords0++ = static_cast<float>(matVex[k][j]);
						}
					}
					*pTexCoords0++ = ColorUtils::White.r;
					*pTexCoords0++ = ColorUtils::White.g;
					*pTexCoords0++ = ColorUtils::White.b;
					*pTexCoords0++ = ColorUtils::White.a;

					// 需要转化为uint指针后再写入值才不会有精度损失
					unsigned int* pUint = (unsigned int*)pTexCoords0;
					*pUint++ = pFeatureIds[iOri];
					pSplitFeatureIds[i] = pFeatureIds[iOri];

					pTexCoords0++;
				}

				std::vector<IDInfo*> vecIDInfo;
				if (pSplitFeatureIds != nullptr)
				{
					TranslateFeatureData(pSplitFeatureIds, nSplitFeatureIdCount, vecIDInfo);
					delete[] pSplitFeatureIds;
					pSplitFeatureIds = nullptr;
				}

				std::wstring strGeoName = StringUtil::ChangeExt(strSymbolName, FILE_EXT_S3MB);
				Skeleton* pGeometry = new Skeleton();
				pGeometry->m_strGeoName = strGeoName;
				pGeometry->m_pVertexDataPackage = pSplitDataPackage;
				pGeometry->ComputerBoundingBox();
				delete[] pGeometry->m_pVertexDataPackage->m_pVertices;
				pGeometry->m_pVertexDataPackage->m_pVertices = nullptr;
				pGeometry->m_pVertexDataPackage->m_nVerticesCount = 0;
				pGeometry->m_arrIndexPackage = arrIndexPackage;
				pGeometry->m_arrIDInfo = vecIDInfo;

				vecGeometry.push_back(pGeometry);
			}

			delete pDataPackage;
			pDataPackage = nullptr;
		}

		delete[] pFeatureIds;
		pFeatureIds = nullptr;
		nFeatureIdCount = 0;

		return true;
	}

	bool I3SNodesTranslation::TranslateVertexData(MemoryStream& stream, const I3SNodeInfo& nodeInfo, const std::wstring& strName, VertexDataPackage* pDataPackage)
	{
		if (StringUtil::CompareNoCase(strName, I3S_JSON_POSITION))
		{
			pDataPackage->m_nVertexDimension = 3;
			pDataPackage->m_pVertices = new(std::nothrow) float[pDataPackage->m_nVerticesCount * pDataPackage->m_nVertexDimension];
			stream.Load(pDataPackage->m_pVertices, pDataPackage->m_nVerticesCount * pDataPackage->m_nVertexDimension);

			Vector3d vNodePos = nodeInfo.GetObb().GetCenter();
			Matrix4d matNod, matNodInv;
			GetMatrix(vNodePos, matNod);
			matNodInv = matNod.Invert();
			for (unsigned int i = 0; i < pDataPackage->m_nVerticesCount; i++)
			{
				Vector3d vVertex;
				vVertex.x = pDataPackage->m_pVertices[i * 3 + 0] + vNodePos.x;
				vVertex.y = pDataPackage->m_pVertices[i * 3 + 1] + vNodePos.y;
				vVertex.z = pDataPackage->m_pVertices[i * 3 + 2] + vNodePos.z;

				Matrix4d matVex;
				GetMatrix(vVertex, matVex);
				matVex = matVex * matNodInv;
				Vector3d vNewVertex;
				vNewVertex = vNewVertex.MultiplyMatrix(matVex);

				pDataPackage->m_pVertices[i * 3 + 0] = vNewVertex.x;
				pDataPackage->m_pVertices[i * 3 + 1] = vNewVertex.y;
				pDataPackage->m_pVertices[i * 3 + 2] = vNewVertex.z;
			}
		}
		else if (StringUtil::CompareNoCase(strName, I3S_JSON_NORMAL))
		{
			pDataPackage->m_nNormalCount = pDataPackage->m_nVerticesCount;
			pDataPackage->m_nNormalDimension = 3;
			pDataPackage->m_pNormals = new(std::nothrow) float[pDataPackage->m_nNormalCount * pDataPackage->m_nNormalDimension];
			stream.Load(pDataPackage->m_pNormals, pDataPackage->m_nNormalCount * pDataPackage->m_nNormalDimension);
		}
		else if (StringUtil::CompareNoCase(strName, I3S_JSON_UV0))
		{
			pDataPackage->m_TexCoordCount[0] = pDataPackage->m_nVerticesCount;
			pDataPackage->m_nTexDimensions[0] = 2;
			float* pTexCoord = new(std::nothrow) float[pDataPackage->m_TexCoordCount[0] * pDataPackage->m_nTexDimensions[0]];
			stream.Load(pTexCoord, pDataPackage->m_TexCoordCount[0] * pDataPackage->m_nTexDimensions[0]);
			pDataPackage->m_pTexCoords[0] = pTexCoord;
		}
		else if (StringUtil::CompareNoCase(strName, I3S_JSON_COLOR))
		{
			pDataPackage->m_nVertexColorCount = pDataPackage->m_nVerticesCount;
			pDataPackage->m_pVertexColor = new(std::nothrow) unsigned int[pDataPackage->m_nVertexColorCount];
			for (unsigned int i = 0; i < pDataPackage->m_nVertexColorCount; i++)
			{
				unsigned char btR, btG, btB, btA;
				stream >> btR;
				stream >> btG;
				stream >> btB;
				stream >> btA;
				pDataPackage->m_pVertexColor[i] = ColorUtils::RGBA(btR, btG, btB, btA);
			}
		}

		return true;
	}

	bool I3SNodesTranslation::TranslateVertexExtensionData(MemoryStream& stream, const std::wstring& strName, VertexDataPackage* pDataPackage)
	{
		if (StringUtil::CompareNoCase(strName, ATT_UVREGION))
		{
			unsigned short nUVRegionDim = 4;
			VertexAttributeType nDataType = AT_16BIT;
			unsigned short* pUVRegion = new(std::nothrow) unsigned short[pDataPackage->m_nVerticesCount * nUVRegionDim];
			stream.Load(pUVRegion, pDataPackage->m_nVerticesCount * nUVRegionDim);

			unsigned int nMapIndex = pDataPackage->m_mapVertexAttributeDescript.size();
			pDataPackage->m_nVertexAttCount++;
			pDataPackage->m_mapVertexAttributeDescript[ATT_UVREGION] = nMapIndex;
			pDataPackage->m_vecVertexAttDataCount.push_back(pDataPackage->m_nVerticesCount);
			pDataPackage->m_vecVertexAttDataDimension.push_back(nUVRegionDim);
			pDataPackage->m_vecVertexAttDataType.push_back(nDataType);
			pDataPackage->m_vecVertexAttData.push_back(pUVRegion);

			//最小值、最大值
			std::vector<double> vecMin, vecMax;
			for (unsigned short i = 0; i < nUVRegionDim; i++)
			{
				vecMin.push_back(USHRT_MAX);
				vecMax.push_back(0);
			}

			for (unsigned int i = 0; i < pDataPackage->m_nVerticesCount; i++)
			{
				for (unsigned int j = 0; j < nUVRegionDim; j++)
				{
					vecMin[j] = MIN(vecMin[j], pUVRegion[nUVRegionDim * i + j]);
					vecMax[j] = MAX(vecMax[j], pUVRegion[nUVRegionDim * i + j]);
				}
			}

			SetDescriptRangeMinMax(ATT_UVREGION, nUVRegionDim, nDataType, vecMin, vecMax);
		}

		return true;
	}

	bool I3SNodesTranslation::TranslateDracoCompVertexExtensionData(const std::vector<std::pair<std::wstring, int> >& vecVexAttExt, VertexDataPackage* pDataPackage)
	{
		if (pDataPackage == nullptr ||
			vecVexAttExt.size() != pDataPackage->m_nVertexAttCount)
		{
			return false;
		}

		for (unsigned int iAtt = 0; iAtt < vecVexAttExt.size(); iAtt++)
		{
			std::wstring strDescript = vecVexAttExt[iAtt].first;
			pDataPackage->m_mapVertexAttributeDescript[strDescript] = iAtt;

			unsigned short nDim = pDataPackage->m_vecVertexAttDataDimension[iAtt];
			VertexAttributeType nDataType = pDataPackage->m_vecVertexAttDataType[iAtt];
			unsigned int nAttDataCount = pDataPackage->m_vecVertexAttDataCount[iAtt];
			void* pAttData = pDataPackage->m_vecVertexAttData[iAtt];

			//最小值、最大值
			std::vector<double> vecMin, vecMax;
			for (unsigned short i = 0; i < nDim; i++)
			{
				vecMin.push_back(DBLMAX);
				vecMax.push_back(-DBLMAX);
			}

			if (nDataType == AT_32BIT)
			{
				for (unsigned int i = 0; i < nAttDataCount; i++)
				{
					for (unsigned int j = 0; j < nDim; j++)
					{
						vecMin[j] = MIN(vecMin[j], ((unsigned int*)pAttData)[nDim * i + j]);
						vecMax[j] = MAX(vecMax[j], ((unsigned int*)pAttData)[nDim * i + j]);
					}
				}
			}
			else if (nDataType == AT_FLOAT)
			{
				for (unsigned int i = 0; i < nAttDataCount; i++)
				{
					for (unsigned int j = 0; j < nDim; j++)
					{
						vecMin[j] = MIN(vecMin[j], ((float*)pAttData)[nDim * i + j]);
						vecMax[j] = MAX(vecMax[j], ((float*)pAttData)[nDim * i + j]);
					}
				}
			}
			else if (nDataType == AT_DOUBLE)
			{
				for (unsigned int i = 0; i < nAttDataCount; i++)
				{
					for (unsigned int j = 0; j < nDim; j++)
					{
						vecMin[j] = MIN(vecMin[j], ((double*)pAttData)[nDim * i + j]);
						vecMax[j] = MAX(vecMax[j], ((double*)pAttData)[nDim * i + j]);
					}
				}
			}
			else if (nDataType == AT_16BIT)
			{
				for (unsigned int i = 0; i < nAttDataCount; i++)
				{
					for (unsigned int j = 0; j < nDim; j++)
					{
						vecMin[j] = MIN(vecMin[j], ((unsigned short*)pAttData)[nDim * i + j]);
						vecMax[j] = MAX(vecMax[j], ((unsigned short*)pAttData)[nDim * i + j]);
					}
				}
			}
			else if (nDataType == AT_8BIT)
			{
				for (unsigned int i = 0; i < nAttDataCount; i++)
				{
					for (unsigned int j = 0; j < nDim; j++)
					{
						vecMin[j] = MIN(vecMin[j], ((unsigned char*)pAttData)[nDim * i + j]);
						vecMax[j] = MAX(vecMax[j], ((unsigned char*)pAttData)[nDim * i + j]);
					}
				}
			}

			SetDescriptRangeMinMax(strDescript, nDim, nDataType, vecMin, vecMax);
		}

		return true;
	}

	bool I3SNodesTranslation::TranslatePointCloudVertexData(const std::wstring& strName, const I3SNodeInfo& nodeInfo, VertexDataPackage*& pDataPackage)
	{
		unsigned long long nBufferSize = 0;
		unsigned char* pBuffer = nullptr;
		if (!m_slpkFile.GetBuffer(strName, pBuffer, nBufferSize))
		{
			delete[] pBuffer;
			pBuffer = nullptr;
			return false;
		}

		unsigned int nPts = 0;
		Point3D* pPts = nullptr;
		LepccDecoder::DecodeXYZ(pBuffer, nBufferSize, nPts, pPts);
		delete[] pBuffer;
		pBuffer = nullptr;

		if (pPts == nullptr || nPts == 0)
		{
			delete[] pPts;
			pPts = nullptr;
			return false;
		}

		Vector3d vNodePos = nodeInfo.GetObb().GetCenter();
		Matrix4d matNod, matNodInv;
		GetMatrix(vNodePos, matNod);
		matNodInv = matNod.Invert();

		if (pDataPackage == nullptr) pDataPackage = new VertexDataPackage();
		pDataPackage->m_nVerticesCount = nPts;
		pDataPackage->m_nVertexDimension = 3;
		pDataPackage->m_pVertices = new(std::nothrow) float[pDataPackage->m_nVerticesCount * pDataPackage->m_nVertexDimension];

		float fMin = FLTMAX, fMax = -FLTMAX;
		float* pElevationData = new float[pDataPackage->m_nVerticesCount];

		for (unsigned int i = 0; i < pDataPackage->m_nVerticesCount; i++)
		{
			const Point3D& pnt = pPts[i];
			Vector3d vVertex(pnt.x, pnt.y, pnt.z);
			pElevationData[i] = pnt.z;
			fMin = MIN(fMin, pnt.z);
			fMax = MAX(fMax, pnt.z);

			Matrix4d matVex;
			GetMatrix(vVertex, matVex);
			matVex = matVex * matNodInv;
			Vector3d vNewVertex;
			vNewVertex = vNewVertex.MultiplyMatrix(matVex);

			pDataPackage->m_pVertices[i * 3 + 0] = vNewVertex.x;
			pDataPackage->m_pVertices[i * 3 + 1] = vNewVertex.y;
			pDataPackage->m_pVertices[i * 3 + 2] = vNewVertex.z;
		}

		//高程
		unsigned short nDim = 1;
		VertexAttributeType nDataType = AT_FLOAT;
		pDataPackage->m_vecVertexAttDataCount.push_back(pDataPackage->m_nVerticesCount);
		pDataPackage->m_vecVertexAttDataDimension.push_back(nDim);
		pDataPackage->m_vecVertexAttDataType.push_back(nDataType);
		pDataPackage->m_vecVertexAttData.push_back(pElevationData);
		pDataPackage->m_mapVertexAttributeDescript[ATT_VERTEXWEIGHT] = (int)pDataPackage->m_nVertexAttCount;
		pDataPackage->m_nVertexAttCount++;

		//高度范围
		m_pairHeightRange.first = MIN(m_pairHeightRange.first, fMin);
		m_pairHeightRange.second = MAX(m_pairHeightRange.second, fMax);

		//最小值、最大值
		std::vector<double> vecMin, vecMax;
		vecMin.push_back(fMin);
		vecMax.push_back(fMax);
		SetDescriptRangeMinMax(ATT_VERTEXWEIGHT, nDim, nDataType, vecMin, vecMax);

		delete[] pPts;
		pPts = nullptr;
		return true;
	}

	bool I3SNodesTranslation::TranslatePointCloudVertexColorData(const std::wstring& strName, VertexDataPackage*& pDataPackage)
	{
		unsigned long long nBufferSize = 0;
		unsigned char* pBuffer = nullptr;
		if (!m_slpkFile.GetBuffer(strName, pBuffer, nBufferSize))
		{
			delete[] pBuffer;
			pBuffer = nullptr;
			return false;
		}

		unsigned int nPts = 0;
		RGB24* pColors = nullptr;
		LepccDecoder::DecodeRGB(pBuffer, nBufferSize, nPts, pColors);
		delete[] pBuffer;
		pBuffer = nullptr;

		if (pColors == nullptr || nPts == 0)
		{
			delete[] pColors;
			pColors = nullptr;
			return false;
		}

		if (pDataPackage == nullptr) return false;
		pDataPackage->m_nVertexColorCount = nPts;
		pDataPackage->m_pVertexColor = new(std::nothrow) unsigned int[nPts];
		for (unsigned int i = 0; i < nPts; i++)
		{
			RGB24 color = pColors[i];
			pDataPackage->m_pVertexColor[i] = ColorUtils::RGBA(color.red, color.green, color.blue, 255);
		}

		delete[] pColors;
		pColors = nullptr;
		return true;
	}

	bool I3SNodesTranslation::TranslatePointCloudIntensityData(const std::wstring& strName, VertexDataPackage*& pDataPackage)
	{
		unsigned long long nBufferSize = 0;
		unsigned char* pBuffer = nullptr;
		if (!m_slpkFile.GetBuffer(strName, pBuffer, nBufferSize))
		{
			delete[] pBuffer;
			pBuffer = nullptr;
			return false;
		}

		unsigned int nElem = 0;
		unsigned short* pIntensities = nullptr;
		LepccDecoder::DecodeIntensity(pBuffer, nBufferSize, nElem, pIntensities);
		delete[] pBuffer;
		pBuffer = nullptr;

		if (pIntensities == nullptr || nElem == 0)
		{
			delete[] pIntensities;
			pIntensities = nullptr;
			return false;
		}

		if (pDataPackage == nullptr) return false;

		unsigned short nDim = 1;
		VertexAttributeType nDataType = AT_16BIT;
		pDataPackage->m_vecVertexAttDataCount.push_back(nElem);
		pDataPackage->m_vecVertexAttDataDimension.push_back(nDim);
		pDataPackage->m_vecVertexAttDataType.push_back(nDataType);
		pDataPackage->m_vecVertexAttData.push_back(pIntensities);
		pDataPackage->m_mapVertexAttributeDescript[ATT_POINTCLOUD_INTENSITY] = (int)pDataPackage->m_nVertexAttCount;
		pDataPackage->m_nVertexAttCount++;

		//最小值、最大值
		unsigned short nMin = USHRT_MAX, nMax = 0;
		for (unsigned int i = 0; i < nElem; i++)
		{
			nMin = MIN(nMin, pIntensities[i]);
			nMax = MAX(nMax, pIntensities[i]);
		}

		std::vector<double> vecMin, vecMax;
		vecMin.push_back(nMin);
		vecMax.push_back(nMax);
		SetDescriptRangeMinMax(ATT_POINTCLOUD_INTENSITY, nDim, nDataType, vecMin, vecMax);

		return true;
	}

	bool I3SNodesTranslation::TranslatePointCloudClassCodeData(const std::wstring& strName, VertexDataPackage*& pDataPackage)
	{
		unsigned long long nBufferSize = 0;
		unsigned char* pBuffer = nullptr;
		if (!m_slpkFile.GetBuffer(strName, pBuffer, nBufferSize))
		{
			delete[] pBuffer;
			pBuffer = nullptr;
			return false;
		}

		if (pDataPackage == nullptr) return false;

		unsigned short nDim = 1;
		VertexAttributeType nDataType = AT_8BIT;
		pDataPackage->m_vecVertexAttDataCount.push_back(nBufferSize);
		pDataPackage->m_vecVertexAttDataDimension.push_back(nDim);
		pDataPackage->m_vecVertexAttDataType.push_back(nDataType);
		pDataPackage->m_vecVertexAttData.push_back(pBuffer);
		pDataPackage->m_mapVertexAttributeDescript[ATT_POINTCLOUD_CLASSCODE] = (int)pDataPackage->m_nVertexAttCount;
		pDataPackage->m_nVertexAttCount++;

		//最小值、最大值
		unsigned char nMin = UCHAR_MAX, nMax = 0;
		for (unsigned int i = 0; i < nBufferSize; i++)
		{
			nMin = MIN(nMin, pBuffer[i]);
			nMax = MAX(nMax, pBuffer[i]);
		}

		std::vector<double> vecMin, vecMax;
		vecMin.push_back(nMin);
		vecMax.push_back(nMax);
		SetDescriptRangeMinMax(ATT_POINTCLOUD_CLASSCODE, nDim, nDataType, vecMin, vecMax);

		return true;
	}

	bool I3SNodesTranslation::TranslateFeatureData(MemoryStream& stream, const std::vector<std::pair<std::wstring, int> >& vecVexAttExtAndId, 
		unsigned int nFeatureCount, std::vector<IDInfo*> vecIDInfo)
	{
		unsigned long long* pFeatureIds = nullptr;
		unsigned int* pFaceRange = nullptr;

		for (unsigned int i = 0; i < vecVexAttExtAndId.size(); i++)
		{
			std::wstring strName = vecVexAttExtAndId[i].first;
			if (StringUtil::CompareNoCase(strName, I3S_JSON_ID))
			{
				pFeatureIds = new(std::nothrow) unsigned long long[nFeatureCount];
				stream.Load(pFeatureIds, nFeatureCount);
			}
			else if (StringUtil::CompareNoCase(strName, I3S_JSON_SCENELAYER_FACERANGE))
			{
				pFaceRange = new(std::nothrow) unsigned int[nFeatureCount * 2];
				stream.Load(pFaceRange, nFeatureCount * 2);
			}
		}

		if (pFeatureIds != nullptr && pFaceRange != nullptr)
		{
			//没有索引的数据
			if (!m_I3SParseParams.GetHasFaceAttributes())
			{
				for (unsigned int i = 0; i < nFeatureCount; i++)
				{
					unsigned int nVertexBegin = pFaceRange[i * 2] * 3;
					unsigned int nVertexEnd = pFaceRange[i * 2 + 1] * 3 + 2;
					std::pair<int, int> pair = std::make_pair(nVertexBegin, nVertexEnd - nVertexBegin + 1);

					IDInfo* pIDInfo = new IDInfo();
					pIDInfo->m_nID = pFeatureIds[i];
					pIDInfo->m_arrVertexColorOffsetAndCount.push_back(pair);
					vecIDInfo.push_back(pIDInfo);
				}
			}
		}

		delete[] pFeatureIds;
		pFeatureIds = nullptr;
		delete[] pFaceRange;
		pFaceRange = nullptr;
		return true;
	}

	bool I3SNodesTranslation::TranslateFeatureData(unsigned int* pFeatureIds, unsigned int nFeatureIdCount, std::vector<IDInfo*>& vecIDInfo)
	{
		if (pFeatureIds == nullptr ||
			nFeatureIdCount == 0)
		{
			return false;
		}

		std::map<unsigned int, std::vector<unsigned int> > mapIdAndVertexIndexes;
		for (unsigned int i = 0; i < nFeatureIdCount; i++)
		{
			unsigned int nId = pFeatureIds[i];
			mapIdAndVertexIndexes[nId].push_back(i);
		}

		for (auto iter = mapIdAndVertexIndexes.begin(); iter != mapIdAndVertexIndexes.end(); iter++)
		{
			std::pair<int, int> pair = std::make_pair(iter->second[0], iter->second.size());

			IDInfo* pIDInfo = new IDInfo();
			pIDInfo->m_nID = iter->first;
			pIDInfo->m_arrVertexColorOffsetAndCount.push_back(pair);
			vecIDInfo.push_back(pIDInfo);
		}

		return true;
	}

	bool I3SNodesTranslation::TranslateAttributes(const std::vector<std::wstring>& vecAttPath, std::map<int, Feature*>& mapFeature)
	{
        S3MBFieldInfos fieldInfos = m_I3SParseParams.GetFieldInfos();
		const std::vector<I3SAttributeInfo>& vecAttInfo = m_I3SParseParams.GetAttributeInfos();
		if (vecAttPath.size() != vecAttInfo.size())
		{
			return false;
		}

		std::vector<Feature*> vecFeature;
		for (unsigned int i = 0; i < vecAttPath.size(); i++)
		{
			std::wstring strAttPath = vecAttPath[i] + I3S_EXT_BINGZ;
			TranslateAttribute(strAttPath, fieldInfos.GetAt(i), vecAttInfo[i], vecFeature);
		}

		for (unsigned int i = 0; i < vecFeature.size(); i++)
		{
			int nID = vecFeature[i]->m_nID;
			if (mapFeature.find(nID) != mapFeature.end())
			{
				delete mapFeature[nID];
			}

			mapFeature[nID] = vecFeature[i];
		}

		return true;
	}

	bool I3SNodesTranslation::TranslateAttribute(const std::wstring& strName, const S3MBFieldInfo& fieldInfo, const I3SAttributeInfo& attInfo, std::vector<Feature*>& vecFeature)
	{
		unsigned long long nBufferSize = 0;
		unsigned char* pBuffer = nullptr;
		if (!m_slpkFile.GetBuffer(strName, pBuffer, nBufferSize))
		{
			delete[] pBuffer;
			pBuffer = nullptr;
			return false;
		}

		bool bResult = false;
		{
			MemoryStream stream;
			stream.Init(pBuffer, nBufferSize);
			{
				std::wstring strName = fieldInfo.m_strName;
				FieldType nOriType = attInfo.GetOriginFieldType();
				bool bIsCoded = attInfo.GetIsCoded();
				const std::map<Variant, std::wstring>& mapCode = attInfo.GetCodeValues();

				unsigned int nCount = 0;
				stream >> nCount;

				switch (nOriType)
				{
				case FieldType::FT_Boolean:
				{
					if (vecFeature.size() == nCount)
					{
						for (unsigned int i = 0; i < nCount; i++)
						{
							bool bValue = false;
							stream >> bValue;

							Variant varValue(bValue);
							if (bIsCoded)
							{
								if (mapCode.find(varValue) != mapCode.end())
								{
									varValue = StringUtil::UnicodeToANSI(mapCode.at(varValue));
								}
								else
								{
									varValue = Variant("");
								}
							}

							vecFeature[i]->SetValue(strName, varValue);
						}
					}
				}
				break;
				case FieldType::FT_INT16:
				{
					if (vecFeature.size() == nCount)
					{
						for (unsigned int i = 0; i < nCount; i++)
						{
							short sValue = 0;
							stream >> sValue;

							Variant varValue(sValue);
							if (bIsCoded)
							{
								if (mapCode.find(varValue) != mapCode.end())
								{
									varValue = StringUtil::UnicodeToANSI(mapCode.at(varValue));
								}
								else
								{
									varValue = Variant("");
								}
							}

							vecFeature[i]->SetValue(strName, varValue);
						}
					}
				}
				break;
				case FieldType::FT_INT32:
				{
					if (StringUtil::CompareNoCase(strName, S3MB_JSON_ATT_SMID))
					{
                        S3MBFieldInfos fieldInfos = m_I3SParseParams.GetFieldInfos();
						for (unsigned int i = 0; i < nCount; i++)
						{
							int nId = 0;
							stream >> nId;

							Feature* pFeature = new Feature;
							//这里设置全部的字段信息，不需要再逐个添加
							pFeature->SetFieldInfos(fieldInfos);
							pFeature->m_nID = nId;
							vecFeature.push_back(pFeature);
						}
					}
					else if (vecFeature.size() == nCount)
					{
						for (unsigned int i = 0; i < nCount; i++)
						{
							int nValue = 0;
							stream >> nValue;

							Variant varValue(nValue);
							if (bIsCoded)
							{
								if (mapCode.find(varValue) != mapCode.end())
								{
									varValue = StringUtil::UnicodeToANSI(mapCode.at(varValue));
								}
								else
								{
									varValue = Variant("");
								}
							}

							vecFeature[i]->SetValue(strName, varValue);
						}
					}
				}
				break;
				case FieldType::FT_INT64:
				{
					if (vecFeature.size() == nCount)
					{
						for (unsigned int i = 0; i < nCount; i++)
						{
							long lValue = 0;
							stream >> lValue;

							Variant varValue(lValue);
							if (bIsCoded)
							{
								if (mapCode.find(varValue) != mapCode.end())
								{
									varValue = StringUtil::UnicodeToANSI(mapCode.at(varValue));
								}
								else
								{
									varValue = Variant("");
								}
							}

							vecFeature[i]->SetValue(strName, varValue);
						}
					}
				}
				break;
				case FieldType::FT_Float:
				{
					if (vecFeature.size() == nCount)
					{
						for (unsigned int i = 0; i < nCount; i++)
						{
							float fValue = 0;
							stream >> fValue;

							Variant varValue(fValue);
							if (bIsCoded)
							{
								if (mapCode.find(varValue) != mapCode.end())
								{
									varValue = StringUtil::UnicodeToANSI(mapCode.at(varValue));
								}
								else
								{
									varValue = Variant("");
								}
							}

							vecFeature[i]->SetValue(strName, varValue);
						}
					}
				}
				break;
				case FieldType::FT_Double:
				{
					unsigned char nAlign = 0;
					stream >> nAlign;
					stream >> nAlign;
					stream >> nAlign;
					stream >> nAlign;

					if (vecFeature.size() == nCount)
					{
						for (unsigned int i = 0; i < nCount; i++)
						{
							double dValue = 0.0;
							stream >> dValue;

							Variant varValue(dValue);
							if (bIsCoded)
							{
								if (mapCode.find(varValue) != mapCode.end())
								{
									varValue = StringUtil::UnicodeToANSI(mapCode.at(varValue));
								}
								else
								{
									varValue = Variant("");
								}
							}

							vecFeature[i]->SetValue(strName, varValue);
						}
					}
				}
				break;
				case FieldType::FT_Text:
				{
					unsigned int nTotalLength = 0;
					stream >> nTotalLength;

					std::vector<unsigned int> vecLength;
					for (unsigned int i = 0; i < nCount; i++)
					{
						unsigned int nLength = 0;
						stream >> nLength;
						vecLength.push_back(nLength);
					}

					if (vecFeature.size() == nCount)
					{
						for (unsigned int i = 0; i < nCount; i++)
						{
							unsigned int nLength = vecLength[i];
							char* pData = new(std::nothrow) char[nLength];
							stream.Load(pData, nLength);
							std::string strValue(pData, nLength);
							delete[] pData;
							pData = nullptr;

							Variant varValue(strValue);
							if (bIsCoded)
							{
								if (mapCode.find(varValue) != mapCode.end())
								{
									varValue = StringUtil::UnicodeToANSI(mapCode.at(varValue));
								}
								else
								{
									varValue = Variant("");
								}
							}

							vecFeature[i]->SetValue(strName, varValue);
						}
					}
				}
				break;
				default:
					break;
				}
			}
		}

		delete[] pBuffer;
		pBuffer = nullptr;

		return bResult;
	}

	void I3SNodesTranslation::SetDescriptRangeMinMax(const std::wstring& strCategory, unsigned short nDim,
		VertexAttributeType nDataType, const std::vector<double>& vecMin, const std::vector<double>& vecMax)
	{
		if (vecMin.size() != nDim ||
			vecMax.size() != nDim)
		{
			assert(false);
			return;
		}

		if (m_mapCategoryDescript.find(strCategory) == m_mapCategoryDescript.end())
		{
			CategoryDescript descript;
			descript.m_strCategory = strCategory;
			descript.m_nDimension = nDim;
			descript.m_vecRangeMin = vecMin;
			descript.m_vecRangeMax = vecMax;
			m_mapCategoryDescript[strCategory] = descript;
		}
		else
		{
			CategoryDescript& descript = m_mapCategoryDescript[strCategory];
			for (unsigned int i = 0; i < nDim; i++)
			{
				descript.m_vecRangeMin[i] = MIN(descript.m_vecRangeMin[i], vecMin[i]);
				descript.m_vecRangeMax[i] = MAX(descript.m_vecRangeMax[i], vecMax[i]);
			}
		}
	}

	void I3SNodesTranslation::GetMatrix(const Vector3d& vPos, Matrix4d& mat)
	{
		if (m_I3SParams.GetIsLonLatCoordinates())
		{
			mat = MathEngine::GetEllipsoidWorldView(vPos);
		}
		else
		{
			Matrix4d matTrans = MatrixUtils::Translation(vPos.x, vPos.y, vPos.z);
			mat = matTrans;
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////

	I3SSymbolsTranslation::I3SSymbolsTranslation(const I3SParams& i3sParams, const I3SParseParams& i3sParseParams, 
		const std::vector<std::wstring>& vecPath) :
		m_I3SParams(i3sParams),
		m_I3SParseParams(i3sParseParams),
		m_vecSymbolPath(vecPath)
	{
	}

	I3SSymbolsTranslation::~I3SSymbolsTranslation()
	{
	}

	bool I3SSymbolsTranslation::Run()
	{
		m_slpkFile.Open(m_I3SParams.GetInputPath());

		std::wstring strOutputDir = m_I3SParseParams.GetOutputPath() + U("ModelFile/");
		for (unsigned int i = 0; i < m_vecSymbolPath.size(); i++)
		{
			const std::wstring& strSymbolPath = m_vecSymbolPath[i];
			std::wstring strOriginPath = strOutputDir + StringUtil::GetTitle(strSymbolPath);
			std::wstring strS3MBPath = StringUtil::ChangeExt(strOriginPath, FILE_EXT_S3MB);
			WriteOriginSymbol(strSymbolPath, strOriginPath);
			OriginSymbolToS3MB(strOriginPath, strS3MBPath);

			std::string cstrOriginPath = StringUtil::UnicodeToANSI(strOriginPath);
			std::remove(cstrOriginPath.c_str());
		}

		return m_slpkFile.Close();
	}

	bool I3SSymbolsTranslation::WriteOriginSymbol(const std::wstring& strSymbolPath, const std::wstring& strOutputPath)
	{
		unsigned long long nBufferSize = 0;
		unsigned char* pBuffer = nullptr;
		if (!m_slpkFile.GetBuffer(strSymbolPath, pBuffer, nBufferSize))
		{
			delete[] pBuffer;
			pBuffer = nullptr;
			return false;
		}

		if (pBuffer == nullptr)
		{
			return false;
		}

		std::wstring strOutputDir = StringUtil::GetDir(strOutputPath);
		if (!StringUtil::IsFileExist(strOutputDir))
		{
			FileUtil::MkDirEx(strOutputDir);
		}

		std::string cstrOutputPath = StringUtil::UnicodeToANSI(strOutputPath);

		FILE* pFile = fopen(cstrOutputPath.c_str(), "wb");
		fwrite(pBuffer, sizeof(unsigned char), nBufferSize, pFile);
		fclose(pFile);

		nBufferSize = 0;
		delete[] pBuffer;
		pBuffer = nullptr;
		return true;
	}

	bool I3SSymbolsTranslation::OriginSymbolToS3MB(const std::wstring& strOriginPath, const std::wstring& strOutputPath)
	{
		GLTFTileInfos_2* pTileInfos = GLTFParser::ParseGLBFile(strOriginPath);
		if (pTileInfos == nullptr)
		{
			return false;
		}

		RenderOperationGroup* pGroup = nullptr;
		GLBToGroup(pTileInfos, m_I3SParseParams.GetOutputPath(), pGroup);
		delete pTileInfos;
		pTileInfos = nullptr;
		if (pGroup == nullptr)
		{
			return false;
		}

		S3MBWriter writer;
		writer.SetVersion(m_I3SParams.GetS3MVersion());
		writer.SetROGroup(pGroup);
		bool bWritten = writer.WriteFile(strOutputPath);
		delete pGroup;
		pGroup = nullptr;
		return bWritten;
	}

	bool I3SSymbolsTranslation::GLBToGroup(GLTFTileInfos_2* pTileInfos, const std::wstring& strOutputPath, RenderOperationGroup*& pGroup)
	{
		if (pTileInfos == nullptr)
		{
			return false;
		}

		pGroup = new RenderOperationGroup();
		BoundingBox boxGroup;

		std::map<int, TextureDataInfo*> textureLists;
		std::map<std::wstring, Skeleton*>& mapSkeleton = pGroup->GetSkeleton();
		std::map<std::wstring, Material*>& mapMaterial = pGroup->GetMaterials();
		std::map<std::wstring, TextureDataInfo*>& mapTexture = pGroup->GetTextureData();

		const std::map<unsigned int, std::vector<unsigned int> >& mapMeshSet = pTileInfos->m_mapMeshSet;
		for (auto iter = mapMeshSet.begin(); iter != mapMeshSet.end(); iter++)
		{
			unsigned int nMeshSize = iter->second.size();
			for (unsigned int i = 0; i < nMeshSize; i++)
			{
				RenderOperationGeode* pGeode = new RenderOperationGeode;
#ifdef WIN32
				pGeode->m_strUniqueID = StringUtil::Format(U("%d"), iter->second[i]);
#else
                pGeode->m_strUniqueID = StringUtil::Format(U("%d").c_str(), iter->second[i]);
#endif

				BoundingBox boxGeode;
				Matrix4d mat = pTileInfos->m_mapMeshToLocalView[iter->second[i]];
				const std::vector<GLTFPrimitive>& vecPrimitive = pTileInfos->m_mapMeshs[iter->first].m_vecGLTFPrimitive;

				for (unsigned int j = 0; j < vecPrimitive.size(); j++)
				{
					const GLTFPrimitive& gltfMesh = vecPrimitive[j];
					Skeleton* pSkeleton = nullptr;
					Material* pMaterial = nullptr;
					std::vector<TextureDataInfo*> vecTexPtr;
					TranslateMaterial(pTileInfos, gltfMesh, strOutputPath, pMaterial, textureLists, vecTexPtr);
					if (!TranslateSkeleton(pTileInfos, gltfMesh, pMaterial->m_strName, pSkeleton))
					{
						delete pSkeleton;
						pSkeleton = nullptr;
						delete pMaterial;
						pMaterial = nullptr;
						continue;
					}

					mapSkeleton[pSkeleton->m_strGeoName] = pSkeleton;
					mapMaterial[pMaterial->m_strName] = pMaterial;
					BoundingBox boxSke = pSkeleton->m_BoundingBox;
					if (boxSke.IsNULL())
					{
						pSkeleton->ComputerBoundingBox();
						boxSke = pSkeleton->m_BoundingBox;
					}
					boxGeode.Merge(boxSke);

					for (unsigned int j = 0; j < vecTexPtr.size(); j++)
					{
						TextureDataInfo* texPtr = vecTexPtr[j];
						mapTexture[texPtr->m_strName] = texPtr;
					}

					pGeode->AddSkeleton(pSkeleton);
				}

				pGeode->SetPosition(mat);
				BoundingSphere speGeode(boxGeode);
				pGeode->SetBoundingSphere(speGeode.GetCenter(), speGeode.GetRadius());
				pGroup->AddChild(pGeode);
				boxGroup.Merge(boxGeode);
			}
		}

		return true;
	}

	bool I3SSymbolsTranslation::TranslateMaterial(GLTFTileInfos_2* pTileInfos, const GLTFPrimitive& gltfPrimitive, const std::wstring& strOutputPath, 
		Material*& pMaterial, std::map<int, TextureDataInfo*>& textureLists, std::vector<TextureDataInfo*>& vecTexPtr)
	{
		pMaterial = new Material();
		if (gltfPrimitive.m_nMaterialIndex == -1)
		{
			MaterialUtils::MakeDefault(pMaterial);
			return true;
		}

		TextureDataInfo* texPtr = nullptr;
		TextureDataInfo* texPtr2 = nullptr;
		const std::vector<GLTFMaterial>& vecMaterial = pTileInfos->m_mapMaterials[gltfPrimitive.m_nMaterialIndex];
		if (!vecMaterial.empty())
		{
			GLTFMaterial gltfMaterial = vecMaterial[0];
			MaterialUtils::MakeDefault(pMaterial);
#ifdef WIN32
			pMaterial->m_strName = StringUtil::Format(U("mtl_%u_"), gltfPrimitive.m_nMaterialIndex);
#else
			pMaterial->m_strName = StringUtil::Format(U("mtl_%u_").c_str(), gltfPrimitive.m_nMaterialIndex);
#endif
			pMaterial->m_strName += gltfMaterial.m_strName;
			Technique* pTech = pMaterial->getTechnique(0);
			Pass* pPass = pTech->getPass(0);
			pPass->m_strName = pMaterial->m_strName;
			pPass->m_AlphaMode = gltfMaterial.m_AlphaMode;
			pPass->m_fAlphaCutoff = gltfMaterial.m_fAlphaCutoff;
			PBRMetallicRough* pPbrParam = new PBRMetallicRough();
			pPbrParam->m_vec4BaseColor = gltfMaterial.m_PBR.m_vec4BaseColor;
			pPbrParam->m_vec3EmissiveFactor = gltfMaterial.m_vec3EmissiveFactor;
			pPbrParam->m_fMetallicFactor = gltfMaterial.m_PBR.m_fMetallicFactor;
			pPbrParam->m_fRoughnessFactor = gltfMaterial.m_PBR.m_fRoughnessFactor;

			if (gltfMaterial.m_PBR.m_nBaseColorTextureIndex > -1)
			{
				TranslateTextures(pTileInfos, gltfMaterial.m_PBR.m_nBaseColorTextureIndex, strOutputPath, textureLists, texPtr);
				if (texPtr != nullptr)
				{
					pPbrParam->m_nBaseColorTextureCoordIndex = gltfMaterial.m_PBR.m_nBaseColorTextureCoordIndex;
					pPbrParam->m_nBaseColorTextureIndex = vecTexPtr.size();
					TextureUnitState* texUS = pPass->CreateTextureUnitState();
					texUS->m_strTextureName = texPtr->m_strName;
					vecTexPtr.push_back(texPtr);
				}
			}

			if (gltfMaterial.m_PBR.m_nMetallicRoughnessTextureIndex > -1)
			{
				TranslateTextures(pTileInfos, gltfMaterial.m_PBR.m_nMetallicRoughnessTextureIndex, strOutputPath, textureLists, texPtr);
				if (texPtr != nullptr)
				{
					pPbrParam->m_nMetallicRoughnessTextureCoordIndex = gltfMaterial.m_PBR.m_nMetallicRoughnessTextureCoordIndex;
					pPbrParam->m_nMetallicRoughnessTextureIndex = vecTexPtr.size();
					TextureUnitState* texUS = pPass->CreateTextureUnitState();
					texUS->m_strTextureName = texPtr->m_strName;
					vecTexPtr.push_back(texPtr);
				}
			}

			if (gltfMaterial.m_nNormalTextureIndex > -1)
			{
				TranslateTextures(pTileInfos, gltfMaterial.m_nNormalTextureIndex, strOutputPath, textureLists, texPtr);
				if (texPtr != nullptr)
				{
					pPbrParam->m_nNormalTextureCoordIndex = gltfMaterial.m_nNormalTextureCoordIndex;
					pPbrParam->m_nNormalTextureIndex = vecTexPtr.size();
					TextureUnitState* texUS = pPass->CreateTextureUnitState();
					texUS->m_strTextureName = texPtr->m_strName;
					vecTexPtr.push_back(texPtr);

					pPbrParam->m_fNormalTextureScale = gltfMaterial.m_fNormalTextureScale;
				}
			}

			if (gltfMaterial.m_nOcclusionTextureIndex > -1)
			{
				TranslateTextures(pTileInfos, gltfMaterial.m_nOcclusionTextureIndex, strOutputPath, textureLists, texPtr);
				if (texPtr != nullptr)
				{
					pPbrParam->m_nOcclusionTextureCoordIndex = gltfMaterial.m_nOcclusionTextureCoordIndex;
					pPbrParam->m_nOcclusionTextureIndex = vecTexPtr.size();
					TextureUnitState* texUS = pPass->CreateTextureUnitState();
					texUS->m_strTextureName = texPtr->m_strName;
					vecTexPtr.push_back(texPtr);
				}
			}

			if (gltfMaterial.m_nEmissiveTextureIndex > -1)
			{
				TranslateTextures(pTileInfos, gltfMaterial.m_PBR.m_nBaseColorTextureIndex, strOutputPath, textureLists, texPtr);
				if (texPtr != nullptr)
				{
					pPbrParam->m_nEmissiveTextureCoordIndex = gltfMaterial.m_nEmissiveTextureCoordIndex;
					pPbrParam->m_nEmissiveTextureIndex = vecTexPtr.size();
					TextureUnitState* texUS = pPass->CreateTextureUnitState();
					texUS->m_strTextureName = texPtr->m_strName;
					vecTexPtr.push_back(texPtr);
				}
			}
			pPass->m_pPRBMaterial = pPbrParam;
			return true;
		}

		//----------------------- 材质纹理----------------------------------------------------------------
		if (gltfPrimitive.m_nMaterialIndex != -1)
		{
			if (vecMaterial.size() > 0)
			{//第一重纹理
				TranslateTextures(pTileInfos, vecMaterial[0].m_PBR.m_nBaseColorTextureIndex, strOutputPath, textureLists, texPtr);
			}
			if (vecMaterial.size() > 1)
			{//第二重纹理
				TranslateTextures(pTileInfos, vecMaterial[1].m_PBR.m_nBaseColorTextureIndex, strOutputPath, textureLists, texPtr2);
			}
		}

		if (texPtr != nullptr)
		{
			pMaterial->m_strName = BaseUtils::CreateGuid();

			std::vector<std::wstring> texName;
			texName.push_back(texPtr->m_strName);
			vecTexPtr.push_back(texPtr);
			if (texPtr2 != nullptr)
			{
				texName.push_back(texPtr2->m_strName);
				vecTexPtr.push_back(texPtr2);
			}
			MaterialUtils::SetTextureName(pMaterial, texName);
		}
		else
		{
			MaterialUtils::MakeDefault(pMaterial);
		}

		if (vecMaterial.size() > 0)
		{
			ColorValue color = vecMaterial[0].m_colorV;
			if (texPtr == nullptr && color != ColorValue())
			{
#ifdef WIN32
				std::wstring strColor = StringUtil::Format(U("_%d"), color.GetValue());
#else
				std::wstring strColor = StringUtil::Format(U("_%d").c_str(), color.GetValue());
#endif
				pMaterial->m_strName += strColor;
			}
			Technique* pTech = nullptr;
			if (pMaterial->getNumTechniques() > 0)
			{
				pTech = pMaterial->getTechnique(0);
			}
			else
			{
				pTech = pMaterial->CreateTechnique();
			}
			Pass *pPass = nullptr;
			if (pTech->getNumPasses() > 0)
			{
				pPass = pTech->getPass(0);
			}
			else
			{
				pPass = pTech->CreatePass();
			}
			pPass->m_strName = pMaterial->m_strName;
			pPass->m_Diffuse = color;
		}

		return true;
	}

	bool I3SSymbolsTranslation::TranslateTextures(GLTFTileInfos_2* pTileInfos, int nTexIndex, const std::wstring& strOutputPath, std::map<int, TextureDataInfo*>& texList, TextureDataInfo*& texPtr)
	{
		if (nTexIndex == -1 || nTexIndex >= pTileInfos->m_mapTextures.size())
		{
			return false;
		}

		const GLTFTexture& gltfTexture = pTileInfos->m_mapTextures[nTexIndex];
		if (gltfTexture.m_nImageIndex == -1)
		{
			return false;
		}

		const GLTFImage& gltfImage = pTileInfos->m_mapImages[gltfTexture.m_nImageIndex];
		if (gltfImage.m_nBufferViewIndex == -1 && gltfImage.m_strFileName.empty())
		{
			return false;
		}

		if (texList.size() > 0)
		{
			auto iter = texList.find(gltfImage.m_nBufferViewIndex);
			if (iter != texList.end())
			{
				texPtr = iter->second;
				return false;
			}
		}

		//生成出的纹理文件路径
		std::wstring strTexFile;
		if (gltfImage.m_strFileName.empty())
		{
			const GLTFBufferView& gltfBufferView = pTileInfos->m_mapBufferViews[gltfImage.m_nBufferViewIndex];
			unsigned int nBufferSize = gltfBufferView.m_nByteLength;
			unsigned char* pBuffer = new unsigned char[nBufferSize];
			const GLTFBuffer& gltfBuffer = pTileInfos->m_mapBuffers[gltfBufferView.m_nBufferIndex];
			memcpy(pBuffer, gltfBuffer.m_pBuffer + gltfBufferView.m_nByteOffset, nBufferSize);

			// 存纹理文件
			std::wstring strTexExt;
			int nLength = gltfImage.m_strImageType.length();
			if (gltfImage.m_strImageType.find(U("jpg")) < nLength ||
				gltfImage.m_strImageType.find(U("jpeg")) < nLength ||
				gltfImage.m_strImageType.find(U("JPG")) < nLength ||
				gltfImage.m_strImageType.find(U("JPEG")) < nLength)
			{
				strTexExt = U(".jpg");
			}
			else if (gltfImage.m_strImageType.find(U("png")) < nLength ||
				gltfImage.m_strImageType.find(U("PNG")) < nLength)
			{
				strTexExt = U(".png");
			}
			else
			{
				std::vector<std::wstring> strDest;
				StringUtil::Split(gltfImage.m_strImageType, strDest, '/');

				if (strDest.size() == 2)
				{
					strTexExt = U(".") + strDest[1];
				}
				strDest.clear();
			}

			std::wstring strTexName = BaseUtils::CreateGuid() + strTexExt;
			strTexFile = StringUtil::GetAbsolutePath(strOutputPath, strTexName);

			std::string strTemp = StringUtil::UNICODE_to_UTF8(strTexFile);
			strTemp = StringUtil::UTF8_to_ANSI(strTemp);

			FILE* file = fopen(strTemp.c_str(), "wb");
			fwrite(pBuffer, sizeof(unsigned char), nBufferSize, file);
			fclose(file);

			delete[] pBuffer;
			pBuffer = nullptr;
		}
		else
		{
			strTexFile = gltfImage.m_strFile;
		}

		TextureData* pTextrueData = nullptr;
		std::wstring strTempTexExt = StringUtil::GetExt(strTexFile);
		if (!StringUtil::CompareNoCase(strTempTexExt, U(".webp")))
		{
			pTextrueData = ImgToolkit::GetTextureData(strTexFile, false);
		}
		else
		{
			//ToDo: webp
		}

		pTextrueData->m_nSize = pTextrueData->m_nWidth * pTextrueData->m_nHeight;

		//数据自带的外挂纹理文件不需要删
		if (gltfImage.m_strFileName.empty())
		{
			std::string strRemove = StringUtil::UnicodeToANSI(strTexFile);
			remove(strRemove.c_str());
		}

		TextureDataInfo* pTexDataInfo = new TextureDataInfo;
		pTexDataInfo->m_strName = BaseUtils::CreateGuid();
		pTexDataInfo->m_pTextureData = pTextrueData;

		if (gltfTexture.m_GLTFSampler.m_nWrapS == 10497 || gltfTexture.m_GLTFSampler.m_nWrapT == 10497)
		{
			TextureUtils::ScaleTextureData(pTexDataInfo->m_pTextureData, TextureUtils::AdjustTextureSize(pTexDataInfo->m_pTextureData->m_nWidth), TextureUtils::AdjustTextureSize(pTexDataInfo->m_pTextureData->m_nHeight), true);
		}

		TextureUtils::UpdateTextureTier(pTexDataInfo, TextureCompressType::TC_DXT5);

		texPtr = pTexDataInfo;
		texList[gltfImage.m_nBufferViewIndex] = texPtr;
		return true;
	}

	bool I3SSymbolsTranslation::TranslateSkeleton(GLTFTileInfos_2* pTileInfos, const GLTFPrimitive& gltfPrimitive, const std::wstring& strMaterialName, Skeleton*& pSkeleton)
	{
		if (pTileInfos == nullptr)
		{
			return false;
		}

		const GLTFAttributes& gltfAttributes = gltfPrimitive.m_gltfAttributes;
		const GLTFDraco& gltfDraco = gltfPrimitive.m_gltfDraco;

		pSkeleton = new Skeleton;
		pSkeleton->m_strMaterialName = strMaterialName;
		pSkeleton->m_strGeoName = BaseUtils::CreateGuid();
		if (gltfDraco.m_nBufferViewIndex == -1)
		{
			VertexDataPackage* pDataPackage = nullptr;
			IndexPackage* pIndexPackage = nullptr;
			TranslateVertexData(pTileInfos, gltfAttributes, pDataPackage);
			TranslateIndexData(pTileInfos, gltfPrimitive, strMaterialName, pIndexPackage);

			std::vector<IndexPackage*> arrIndexPackage;
			arrIndexPackage.push_back(pIndexPackage);
			std::vector<IDInfo*> vecIDInfo;
			SkeletonUtils::RebuildVertexAndIndexPackage(pDataPackage, arrIndexPackage, vecIDInfo);
			pSkeleton->m_pVertexDataPackage = pDataPackage;
			pSkeleton->m_arrIndexPackage = arrIndexPackage;
		}
		// 骨架数据做Draco压缩
		else
		{
			//ToDo:
		}

		return true;
	}

	bool I3SSymbolsTranslation::TranslateVertexData(GLTFTileInfos_2* pTileInfos, const GLTFAttributes& gltfAttributes, VertexDataPackage*& pDataPackage)
	{
		if (gltfAttributes.m_nPosIndex == -1)
		{
			assert(false);
			return false;
		}

		// 顶点
		int nPosDim = 3;
		float* pPosData = (float*)GLTFParser::GetAttributeData(pTileInfos, gltfAttributes.m_nPosIndex, nPosDim);
		// 顶点颜色
		int nColorDim = 3;
		float* pColorData = (float*)GLTFParser::GetAttributeData(pTileInfos, gltfAttributes.m_nColorIndex, nColorDim);
		// 法线
		int nNormalDim = 3;
		float* pNormalData = (float*)GLTFParser::GetAttributeData(pTileInfos, gltfAttributes.m_nNormalIndex, nNormalDim);
		// 纹理坐标
		int nTexDim = 2;
		float* pTexData = (float*)GLTFParser::GetAttributeData(pTileInfos, gltfAttributes.m_nTex1Index, nTexDim);
		// 第二重纹理坐标
		int nTexDim2 = 2;
		float* pTexData2 = (float*)GLTFParser::GetAttributeData(pTileInfos, gltfAttributes.m_nTex2Index, nTexDim2);

		pDataPackage = new VertexDataPackage;
		if (pPosData != nullptr)
		{
			const GLTFAccessor& gltfAccessor = pTileInfos->m_mapAccessors[gltfAttributes.m_nPosIndex];
			pDataPackage->m_nVerticesCount = gltfAccessor.m_nCount;
			pDataPackage->m_pVertices = pPosData;
			pDataPackage->m_nVertexDimension = nPosDim;
		}

		if (pColorData != nullptr)
		{
			const GLTFAccessor& gltfAccessor = pTileInfos->m_mapAccessors[gltfAttributes.m_nColorIndex];
			unsigned int nColorCount = gltfAccessor.m_nCount;
			unsigned int* pColor = new unsigned int[nColorCount];
			for (int i = 0; i < nColorCount; i++)
			{
				float r = pColorData[nColorDim * i];
				float g = pColorData[nColorDim * i + 1];
				float b = pColorData[nColorDim * i + 2];
				float a = nColorDim == 4 ? pColorData[nColorDim * i + 3] : 1;
				pColor[i] = ColorUtils::RGBA((unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255), (unsigned char)(a * 255));

			}

			pDataPackage->m_nVertexColorCount = nColorCount;
			pDataPackage->m_pVertexColor = pColor;
			delete[] pColorData;
			pColorData = nullptr;
		}

		if (pNormalData != nullptr)
		{
			const GLTFAccessor& gltfAccessor = pTileInfos->m_mapAccessors[gltfAttributes.m_nNormalIndex];
			pDataPackage->m_nNormalCount = gltfAccessor.m_nCount;
			pDataPackage->m_pNormals = pNormalData;
			pDataPackage->m_nNormalDimension = nNormalDim;
		}

		if (pTexData != nullptr)
		{
			const GLTFAccessor& gltfAccessor = pTileInfos->m_mapAccessors[gltfAttributes.m_nTex1Index];
			pDataPackage->m_TexCoordCount[0] = gltfAccessor.m_nCount;
			pDataPackage->m_pTexCoords[0] = pTexData;
			pDataPackage->m_nTexDimensions[0] = nTexDim;
		}

		if (pTexData2 != nullptr)
		{
			const GLTFAccessor& gltfAccessor = pTileInfos->m_mapAccessors[gltfAttributes.m_nTex2Index];
			pDataPackage->m_TexCoordCount[1] = gltfAccessor.m_nCount;
			pDataPackage->m_pTexCoords[1] = pTexData2;
			pDataPackage->m_nTexDimensions[1] = nTexDim2;
		}

		return true;
	}

	bool I3SSymbolsTranslation::TranslateIndexData(GLTFTileInfos_2* pTileInfos, const GLTFPrimitive& gltfPrimitive, const std::wstring& strMaterialName, IndexPackage*& pIndexPackage)
	{
		if (gltfPrimitive.m_gltfAttributes.m_nPosIndex == -1)
		{
			return false;
		}

		unsigned short* pUShortData = nullptr;
		unsigned int* pUIntData = nullptr;
		unsigned int nIndicesCount = 0;
		if (gltfPrimitive.m_nIndices != -1)
		{
			const GLTFAccessor& gltfAccessor = pTileInfos->m_mapAccessors[gltfPrimitive.m_nIndices];
			nIndicesCount = gltfAccessor.m_nCount;
			unsigned int nComponentType = gltfAccessor.m_nComponentType;

			int nIndicesDim = 1;
			if (nComponentType == 5122 || nComponentType == 5123)
			{
				pUShortData = (unsigned short*)GLTFParser::GetAttributeData(pTileInfos, gltfPrimitive.m_nIndices, nIndicesDim);
			}
			else if (nComponentType == 5120 || nComponentType == 5121)
			{
				unsigned char* pUCharData = (unsigned char*)GLTFParser::GetAttributeData(pTileInfos, gltfPrimitive.m_nIndices, nIndicesDim);
				if (pUCharData != nullptr)
				{
					pUShortData = new unsigned short[nIndicesCount];
					for (unsigned int i = 0; i < nIndicesCount; i++)
					{
						pUShortData[i] = (unsigned short)pUCharData[i];
					}
					delete[] pUCharData;
					pUCharData = nullptr;
				}
			}
			else
			{
				pUIntData = (unsigned int*)GLTFParser::GetAttributeData(pTileInfos, gltfPrimitive.m_nIndices, nIndicesDim);

				//判断下顶点个数是否小于65536，小于就再转下
				const GLTFAccessor& gltfPosAccessor = pTileInfos->m_mapAccessors[gltfPrimitive.m_gltfAttributes.m_nPosIndex];
				if (pUIntData != nullptr && gltfPosAccessor.m_nCount < 65536)
				{
					pUShortData = new unsigned short[nIndicesCount];
					for (unsigned int i = 0; i < nIndicesCount; i++)
					{
						pUShortData[i] = (unsigned short)pUIntData[i];
					}
					delete[] pUIntData;
					pUIntData = nullptr;
				}
			}
		}

		pIndexPackage = new IndexPackage;
		pIndexPackage->m_OperationType = GLTFParser::ToDrawOperationType(gltfPrimitive.m_nDrawType);
		pIndexPackage->m_strPassName.push_back(strMaterialName);
		pIndexPackage->m_nIndexesCount = nIndicesCount;
		if (pUShortData != nullptr)
		{
			pIndexPackage->m_enIndexType = IT_16BIT;
			pIndexPackage->m_pIndexes = pUShortData;
		}
		else if (pUIntData != nullptr)
		{
			pIndexPackage->m_enIndexType = IT_32BIT;
			pIndexPackage->m_pIndexes = (unsigned short*)pUIntData;
		}
		else
		{
			//当没有索引时添加索引
			const GLTFAccessor& gltfPosAccessor = pTileInfos->m_mapAccessors[gltfPrimitive.m_gltfAttributes.m_nPosIndex];
			unsigned int nVerticesCount = gltfPosAccessor.m_nCount;
			if (nVerticesCount < 65536)
			{
				pIndexPackage->m_enIndexType = IT_16BIT;
				pIndexPackage->SetIndexNum(nVerticesCount);
				for (int i = 0; i < nVerticesCount; i++)
				{
					pIndexPackage->m_pIndexes[i] = (unsigned short)i;
				}
			}
			else
			{
				pIndexPackage->m_enIndexType = IT_32BIT;
				pIndexPackage->SetIndexNum(nVerticesCount);
				unsigned int* pIndex = (unsigned int*)pIndexPackage->m_pIndexes;
				for (unsigned int i = 0; i < nVerticesCount; i++)
				{
					pIndex[i] = i;
				}
			}
		}

		if (pIndexPackage->m_OperationType == OT_TRIANGLE_STRIP || pIndexPackage->m_OperationType == OT_TRIANGLE_FAN)
		{
			SkeletonUtils::TranslateIndices(pIndexPackage);
		}

		return true;
	}
}
