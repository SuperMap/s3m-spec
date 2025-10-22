#include "ProcessTools.h"
#include "Common.h"
#include "GLTFParser.h"
#include "I3STool.h"
#include "MathEngine.h"
#include "S3MBUtils.h"
#include "Utils/Utils.h"
#include "S3MBCommon.h"
#include "Shell.h"
#include "ImageOperator.h"
#include "BoundingBox.h"
#include "S3MBParser.h"
#include "S3MBWriter.h"
#include "S3MBConfig.h"
#include "Point3D.h"

#include <assert.h>
#include <fstream>
#include <strstream>
#include <unordered_set>

namespace S3MB
{
	ThreeDTilesParams::ThreeDTilesParams()
	{
		m_nVertexCompressType = S3MBVertexTag::SV_Standard;
		m_nTextureCompressType = TextureCompressType::TC_NONE;
		m_bIsModel = false;
		m_bIsNEU = false;
	}

	ProcessType ThreeDTilesParams::GetProcessType()
	{
		return ProcessType::P_3DTilesToS3MB;
	}

	void ThreeDTilesParams::SetTilesetPath(const std::wstring& strPath)
	{
		m_strTilesetPath = strPath;
	}

	const std::wstring& ThreeDTilesParams::GetTilesetPath() const
	{
		return m_strTilesetPath;
	}

	void ThreeDTilesParams::SetOutputDir(const std::wstring& strDir)
	{
		m_strOutputDir = strDir;
	}

	const std::wstring& ThreeDTilesParams::GetOutputDir() const
	{
		return m_strOutputDir;
	}

	void ThreeDTilesParams::SetPosition(const Point3D& pos)
	{
		m_pntPosition = pos;
	}

	const Point3D& ThreeDTilesParams::GetPosition() const
	{
		return m_pntPosition;
	}

	void ThreeDTilesParams::SetVertexCompressType(S3MBVertexTag nType)
	{
		m_nVertexCompressType = nType;
	}

	S3MBVertexTag ThreeDTilesParams::GetVertexCompressType() const
	{
		return m_nVertexCompressType;
	}

	void ThreeDTilesParams::SetTextureCompressType(TextureCompressType nType)
	{
		m_nTextureCompressType = nType;
	}

	TextureCompressType ThreeDTilesParams::GetTextureCompressType() const
	{
		return m_nTextureCompressType;
	}

	bool ThreeDTilesParams::GetIsTextureCompress() const
	{
		return m_nTextureCompressType != TextureCompressType::TC_NONE;
	}

	void ThreeDTilesParams::SetIsModel(bool bIsModel)
	{
		m_bIsModel = bIsModel;
	}

	bool ThreeDTilesParams::GetIsModel() const
	{
		return m_bIsModel;
	}

	void ThreeDTilesParams::SetIsNEU(bool bIsNEU)
	{
		m_bIsNEU = bIsNEU;
	}

	bool ThreeDTilesParams::GetIsNEU() const
	{
		return m_bIsNEU;
	}

	//=================================================================================================================

	S3MBParams::S3MBParams()
	{
		m_nThreadNum = 8;
	}

	ProcessType S3MBParams::GetProcessType()
	{
		return ProcessType::P_S3MBTo3DTiles;
	}

	void S3MBParams::SetSCPPath(const std::wstring& strPath)
	{
		m_strSCPPath = strPath;
	}

	const std::wstring& S3MBParams::GetSCPPath() const
	{
		return m_strSCPPath;
	}

	void S3MBParams::SetOutputDir(const std::wstring& strDir)
	{
		m_strOutputDir = strDir;
	}

	const std::wstring& S3MBParams::GetOutputDir() const
	{
		return m_strOutputDir;
	}

	void S3MBParams::SetThreadNum(int nNum)
	{
		m_nThreadNum = nNum;
	}

	int S3MBParams::GetThreadNum() const
	{
		return m_nThreadNum;
	}

	//=================================================================================================================

	OSGBParams::OSGBParams()
	{
		m_nTextureCompressType = TextureCompressType::TC_NONE;
	}

	ProcessType OSGBParams::GetProcessType()
	{
		return ProcessType::P_OSGBToS3MB;
	}

	void OSGBParams::SetInputFilePath(const std::wstring& strPath)
	{
		m_strInputFilePath = strPath;
	}

	const std::wstring& OSGBParams::GetInputFilePath() const
	{
		return m_strInputFilePath;
	}

	void OSGBParams::SetOutputDir(const std::wstring& strDir)
	{
		m_strOutputDir = strDir;
	}

	const std::wstring& OSGBParams::GetOutputDir() const
	{
		return m_strOutputDir;
	}

	void OSGBParams::SetTextureCompressType(TextureCompressType nType)
	{
		m_nTextureCompressType = nType;
	}

	TextureCompressType OSGBParams::GetTextureCompressType() const
	{
		return m_nTextureCompressType;
	}

	//=================================================================================================================

	OBJParams::OBJParams()
	{
	}

	ProcessType OBJParams::GetProcessType()
	{
		return ProcessType::P_OBJToS3MB;
	}

	void OBJParams::SetInputFilePath(const std::wstring& strPath)
	{
		m_strInputFilePath = strPath;
	}

	const std::wstring& OBJParams::GetInputFilePath() const
	{
		return m_strInputFilePath;
	}

	void OBJParams::SetOutputDir(const std::wstring& strDir)
	{
		m_strOutputDir = strDir;
	}

	const std::wstring& OBJParams::GetOutputDir() const
	{
		return m_strOutputDir;
	}

	//=================================================================================================================

	OSGBToS3MBTool::OSGBToS3MBTool()
	{

	}

	OSGBToS3MBTool::~OSGBToS3MBTool()
	{

	}

	bool OSGBToS3MBTool::ReadConfig(const wstring& strPath)
	{
		if (!StringUtil::IsFileExist(strPath))
		{
			return false;
		}

		m_strOSGBDir = StringUtil::GetDir(strPath);
		return m_OSGBConfig.LoadFromFile(strPath);
	}

	bool OSGBToS3MBTool::ProcessBodyData(const wstring& strOutputDir)
	{
		const std::vector<std::wstring>& vecFileName = m_OSGBConfig.GetFileNames();
		for (unsigned int i = 0; i < vecFileName.size(); i++)
		{
			std::wstring strRootPath = StringUtil::GetAbsolutePath(m_strOSGBDir, vecFileName[i]);
			if (!StringUtil::IsFileExist(strRootPath))
			{
				continue;
			}

			OSGBParser parser;
			parser.ParseNodeFile(strRootPath, U("noTriStripPolygons"));
			RenderOperationGroup* pGroup = nullptr;
			parser.GetGroup(pGroup);
			if (pGroup != nullptr)
			{
				std::wstring strRelativePath = StringUtil::GetRelativePath(m_strOSGBDir, strRootPath);
				strRelativePath = StringUtil::ChangeExt(strRelativePath, FILE_EXT_S3MB);
				std::wstring strOutputPath = StringUtil::GetAbsolutePath(strOutputDir, strRelativePath);
				FileUtil::MkDirEx(StringUtil::GetDir(strOutputPath));

				ProcessChildNode(pGroup, StringUtil::GetDir(strRootPath), strOutputDir);

				ReComputeBoundingBox(pGroup);
				OrientedBoundingBox obb;
				GetGroupOrientedBoundingBox(pGroup, obb);
				pGroup->SetOrientedBoundingBox(obb);

				BoundingBox box;
				GetGroupBoundingBox(pGroup, box);
				pGroup->SetBoundingBox(box);

				m_vecRootNameBox.push_back(std::make_pair(strRelativePath, box));
				m_vecRootNameOBB.push_back(std::make_pair(strRelativePath, obb));

				S3MBWriter writer;
				writer.SetROGroup(pGroup);
                writer.SetIsChangeTexture(true, true);
				writer.WriteFile(strOutputPath);
				writer.Clear();
				delete pGroup;
				pGroup = nullptr;
			}
		}

		if (!m_vecRootNameBox.empty())
		{
			BoundingBox totalBox = m_vecRootNameBox[0].second;
			for (int i = 0; i < m_vecRootNameBox.size(); i++)
			{
				totalBox.Merge(m_vecRootNameBox[i].second);
			}

			const Point3D& pntPos = m_OSGBConfig.GetPosition();
			Matrix4d matCenter;
			matCenter.SetTrans(Vector3d(pntPos.x, pntPos.y, pntPos.z));
			totalBox.Transform(matCenter);
			Point3D pntInnerPoint3D;
			pntInnerPoint3D.x = totalBox.GetCenter().x;
			pntInnerPoint3D.y = totalBox.GetCenter().y;
			pntInnerPoint3D.z = 0.0;

			Matrix4d matCenterInvert = matCenter.Invert();
			totalBox.Transform(matCenterInvert);
			double dWidth = ABS(totalBox.GetMax().x - totalBox.GetMin().x);
			double dHeight = ABS(totalBox.GetMax().y - totalBox.GetMin().y);

			double dLeft = pntInnerPoint3D.x - dWidth / 2;
			double dTop = pntInnerPoint3D.y + dHeight / 2;
			double dRight = pntInnerPoint3D.x + dWidth / 2;
			double dBottom = pntInnerPoint3D.y - dHeight / 2;
			m_rcBounds = Rect2D(dLeft, dTop, dRight, dBottom);
		}

		return true;
	}

	bool OSGBToS3MBTool::WriteConfig(const wstring& strOutputDir)
	{
		std::wstring strPath = strOutputDir + U("/config.scp");
		S3MBConfig config;
		config.SetPosition(m_OSGBConfig.GetPosition());
		config.SetEPSGCode(m_OSGBConfig.GetEpsgCode());
		config.SetBounds(m_rcBounds);
		config.SetIsDegree(false);
		config.SetDataType(SDT_RealityMesh);

		std::vector<std::wstring> vecRootName;
		std::vector<OrientedBoundingBox> vecOBB;
		for (unsigned int i = 0; i < m_vecRootNameOBB.size(); i++)
		{
			vecRootName.push_back(m_vecRootNameOBB[i].first);
			vecOBB.push_back(m_vecRootNameOBB[i].second);
		}
		config.SetTiles(vecRootName, vecOBB);

		config.SetExtension(U("s3m:FileType"), U("OSGBFile"));
		config.SetExtension(U("globeType"), U("Ellipsoid_WGS84"));
		config.SetExtension(U("s3m:TextureCompressionType"), U("DXT"));
		config.SetExtension(U("s3m:VertexCompressionType"), U("NONE"));

		std::vector<CategoryDescript> vecDescript;
		CategoryDescript mVertexWeightCategory;
		mVertexWeightCategory.m_strCategory = ATT_VERTEXWEIGHT;
		mVertexWeightCategory.m_vecRangeMax.push_back(0.0);
		mVertexWeightCategory.m_vecRangeMin.push_back(0.0);
		mVertexWeightCategory.m_nDimension = 1;
		vecDescript.push_back(mVertexWeightCategory);

		config.SetVertexAttributeDescripts(vecDescript);
		return config.WriteFile(strPath);
	}

	void OSGBToS3MBTool::ProcessChildNode(RenderOperationNode* pNode, const std::wstring& strInputDir, const wstring& strOutputDir)
	{
		if (pNode == nullptr)
		{
			return;
		}

		if (pNode->ClassName() == S3MB_SHELL_GROUP)
		{
            if (pNode->GetSkeleton().size() == 0)
			{
				return;
			}

			std::vector<std::wstring> vecChildFileName;
			for (int i = 0; i < ((RenderOperationGroup*)pNode)->GetNumChildren(); i++)
			{
				RenderOperationNode* pChildNode = ((RenderOperationGroup*)pNode)->GetChild(i);
				ProcessChildNode(pChildNode, strInputDir, strOutputDir);
			}
		}
		else if (pNode->ClassName() == S3MB_SHELL_PAGEDLOD)
		{
			RenderOperationPagedLOD* pPagedLOD = (RenderOperationPagedLOD*)pNode;
			int nCount = pPagedLOD->GetNumChildren();
			BoundingSphere newBoundingSphere;
			for (int i = 0; i < nCount; i++)
			{
				RenderOperationNode* pChildNode = pPagedLOD->GetChild(i);
				ProcessChildNode(pChildNode, strInputDir, strOutputDir);
				newBoundingSphere.ExpandBy(pChildNode->GetBoundingSphere());
			}

			pPagedLOD->SetBoundingSphere(newBoundingSphere.GetCenter(), newBoundingSphere.GetRadius());
			if (pPagedLOD->GetFileNames().size() > 0)
			{
				std::wstring strChildPath = StringUtil::GetAbsolutePath(strInputDir, pPagedLOD->GetFileNames()[0]);
				if (StringUtil::IsFileExist(strChildPath))
				{
					OSGBParser parser;
					parser.ParseNodeFile(strChildPath, U("noTriStripPolygons"));
					RenderOperationGroup* pChildGroup = nullptr;
					parser.GetGroup(pChildGroup);
					if (pChildGroup != nullptr)
					{
						pChildGroup->SetName(strChildPath);
						ProcessChildNode(pChildGroup, StringUtil::GetDir(strChildPath), strOutputDir);
						
						ReComputeBoundingBox(pChildGroup);
						OrientedBoundingBox obb;
						GetGroupOrientedBoundingBox(pChildGroup, obb);
						pChildGroup->SetOrientedBoundingBox(obb);

						std::wstring strRelativePath = StringUtil::GetRelativePath(m_strOSGBDir, strChildPath);
						strRelativePath = StringUtil::ChangeExt(strRelativePath, FILE_EXT_S3MB);
						std::wstring strOutputPath = StringUtil::GetAbsolutePath(strOutputDir, strRelativePath);
						FileUtil::MkDirEx(StringUtil::GetDir(strOutputPath));
                        pPagedLOD->SetRangeMode(RangeMode::Geometric_Error);

						S3MBWriter writer;
						writer.SetROGroup(pChildGroup);
                        writer.SetIsChangeTexture(true, true);
						writer.WriteFile(strOutputPath);
						writer.Clear();
						delete pChildGroup;
						pChildGroup = nullptr;
					}
				}
			}
		}
		else if (pNode->ClassName() == S3MB_SHELL_GEODE)
		{
			RenderOperationGeode* pGeode = (RenderOperationGeode*)pNode;
            if (pGeode->GetNumSkeleton() == 0)
			{
				return;
			}

			pGeode->ComputerBoundingBox();
            OrientedBoundingBox obb = pGeode->GetOrientedBoundingBox();
            BoundingSphere sphere(BoundingBoxUtils::OBBToAABB(obb));
			pGeode->SetBoundingSphere(sphere.GetCenter(), sphere.GetRadius());
		}
	}

	void OSGBToS3MBTool::ReComputeBoundingBox(RenderOperationGroup* pGroup)
	{
        std::map<std::wstring, Skeleton*>& mapSkeleton = pGroup->GetSkeleton();
        std::map<std::wstring, Skeleton*>::iterator itor;
        for (itor = mapSkeleton.begin(); itor != mapSkeleton.end(); itor++)
		{
            Skeleton* pGeometry = itor->second;
			if (pGeometry == nullptr)
			{
				continue;
			}
			pGeometry->ComputerBoundingBox();
		}
	}

	void OSGBToS3MBTool::GetGroupOrientedBoundingBox(RenderOperationNode* pGroup, OrientedBoundingBox& outOBB)
	{
		std::vector<RenderOperationNode*> vecNode;
		unsigned int rear = 1;
		unsigned int head = 0;
		vecNode.push_back(pGroup);

		std::map<unsigned int, unsigned int> mapNodeRelationship;

		std::map<unsigned int, std::vector<Vector3d> > mapPagedLodCornerPoints;

		const Point3D& pntPos = m_OSGBConfig.GetPosition();

		Matrix4d matCenter;
		matCenter.SetTrans(Vector3d(pntPos.x, pntPos.y, pntPos.z));
		while (head < rear)
		{
			RenderOperationNode* pNode = (RenderOperationNode*)vecNode[head];
			if (pNode->ClassName() == S3MB_SHELL_GROUP)
			{
				for (int nIndex = 0; nIndex < ((RenderOperationGroup*)pNode)->GetNumChildren(); nIndex++)
				{
					RenderOperationNode* pChildNode = ((RenderOperationGroup*)pNode)->GetChild(nIndex);
					++rear;
					vecNode.push_back(pChildNode);
				}
			}
			if (pNode->ClassName() == S3MB_SHELL_PAGEDLOD)
			{
				for (int nIndex = 0; nIndex < ((RenderOperationPagedLOD*)pNode)->GetNumChildren(); nIndex++)
				{
					RenderOperationNode* pChildNode = ((RenderOperationPagedLOD*)pNode)->GetChild(nIndex);
					std::vector<Vector3d> temp;
					mapPagedLodCornerPoints[head] = temp;
					mapNodeRelationship[rear] = head;
					++rear;
					vecNode.push_back(pChildNode);
				}
			}
			if (pNode->ClassName() == S3MB_SHELL_GEODE)
			{
				if (mapPagedLodCornerPoints.find(mapNodeRelationship[head]) != mapPagedLodCornerPoints.end())
				{
					ProcessGeodeInfo((RenderOperationGeode*)pNode, matCenter, mapPagedLodCornerPoints[mapNodeRelationship[head]]);
				}
			}
			++head;
		}
		std::vector<Vector3d> vecPoint;
		std::map<unsigned int, std::vector<Vector3d> >::iterator itor = mapPagedLodCornerPoints.begin();
		for (; itor != mapPagedLodCornerPoints.end(); itor++)
		{
			unsigned int nPagedLod = itor->first;
			if (itor->second.size() > 0)
			{
				OrientedBoundingBox pagedLodOBB;
				pagedLodOBB.FromPoints(itor->second);
				vecPoint.insert(vecPoint.end(), itor->second.begin(), itor->second.end());
				RenderOperationPagedLOD* pPagedLod = (RenderOperationPagedLOD*)vecNode[nPagedLod];
				pPagedLod->SetOrientedBoundingBox(pagedLodOBB);
				itor->second.clear();
			}
		}

		outOBB.FromPoints(vecPoint);
		vecPoint.clear();
		vecNode.clear();
	}

	void OSGBToS3MBTool::ProcessGeodeInfo(RenderOperationGeode* pGeode, Matrix4d& matWorldView, std::vector<Vector3d>& vVec3)
	{
		Matrix4d matGeode = pGeode->GetPosition() * matWorldView;
        for (int nIndex = 0; nIndex < pGeode->GetNumSkeleton(); nIndex++)
		{
            Skeleton* pROGeometry = pGeode->GetSkeleton(nIndex);
			VertexDataPackage* pVertexDataPackage = pROGeometry->m_pVertexDataPackage;
			int nNumDim = pVertexDataPackage->m_nVertexDimension;
			float * pVertex = pROGeometry->m_pVertexDataPackage->m_pVertices;

			int nVertexCount = pROGeometry->m_pVertexDataPackage->m_nVerticesCount;
			float* pAttributetemp = new float[nVertexCount];

			for (unsigned int nVertexindex = 0; nVertexindex < nVertexCount; nVertexindex++)
			{
				Vector3d vecPnt(pVertex[nNumDim*nVertexindex + 0], pVertex[nNumDim*nVertexindex + 1], pVertex[nNumDim*nVertexindex + 2]);
				vecPnt = vecPnt.MultiplyMatrix(matGeode);
				pAttributetemp[nVertexindex] = vecPnt.z;
			}

			pVertexDataPackage->m_mapVertexAttributeDescript[ATT_VERTEXWEIGHT] = 0;
			pVertexDataPackage->m_vecVertexAttDataCount.push_back(nVertexCount);
			pVertexDataPackage->m_nVertexAttCount++;
			pVertexDataPackage->m_vecVertexAttDataDimension.push_back(1);
			pVertexDataPackage->m_vecVertexAttDataType.push_back(AT_FLOAT);
			pVertexDataPackage->m_vecVertexAttData.push_back(pAttributetemp);

			BoundingBox bBox = pROGeometry->m_BoundingBox;
			Matrix4d matGeodePos = pGeode->GetPosition();
			bBox = BoundingBoxUtils::MultiplyMatrix(bBox, matGeodePos);

			const Vector3d* pCorners = bBox.GetCorners();
			for (int nCornerIndex = 0; nCornerIndex < 8; nCornerIndex++)
			{
				vVec3.push_back(pCorners[nCornerIndex]);
			}
		}
	}

	void OSGBToS3MBTool::GetGroupBoundingBox(RenderOperationGroup*& pGroup, BoundingBox& boundingBox)
	{
		int nChildCount = pGroup->GetNumChildren();
		for (int nIndex = 0; nIndex < nChildCount; nIndex++)
		{
			RenderOperationNode* pChildNode = pGroup->GetChild(nIndex);
			std::wstring strName = pChildNode->ClassName();

			if (pChildNode == nullptr ||
				(!StringUtil::CompareNoCase(strName, S3MB_SHELL_PAGEDLOD) && !StringUtil::CompareNoCase(strName, S3MB_SHELL_GEODE)))
			{
				continue;
			}

			if (StringUtil::CompareNoCase(strName, S3MB_SHELL_PAGEDLOD))
			{
				RenderOperationPagedLOD* pPagedLod = (RenderOperationPagedLOD*)pChildNode;
				GetPagedLODBoundingBox(pPagedLod, boundingBox);
			}
			else
			{
				RenderOperationGeode* pGeode = (RenderOperationGeode*)pChildNode;
				BoundingBox tempBoundingBox = pGeode->GetBoundingBox();
				boundingBox.Merge(tempBoundingBox);
			}
		}
	}

	void OSGBToS3MBTool::GetPagedLODBoundingBox(RenderOperationPagedLOD*& pPagedLOD, BoundingBox& boundingBox)
	{
		int i = 0;
		int nChildCount = pPagedLOD->GetNumChildren();
		if (nChildCount == 0)
		{
			return;
		}

		for (i = 0; i < nChildCount; i++)
		{
			RenderOperationNode* pChildNode = pPagedLOD->GetChild(i);
			std::wstring strName = pChildNode->ClassName();

			if (pChildNode == nullptr ||
				(!StringUtil::CompareNoCase(strName, S3MB_SHELL_PAGEDLOD) && !StringUtil::CompareNoCase(strName, S3MB_SHELL_GEODE)))
			{
				continue;
			}

			if (StringUtil::CompareNoCase(strName, S3MB_SHELL_PAGEDLOD))
			{
				RenderOperationPagedLOD* pChildPagedLod = (RenderOperationPagedLOD*)pChildNode;
				GetPagedLODBoundingBox(pChildPagedLod, boundingBox);
			}
			else
			{
				RenderOperationGeode* pGeode = (RenderOperationGeode*)pChildNode;
				BoundingBox tempBoundingBox = pGeode->GetBoundingBox();
				boundingBox.Merge(tempBoundingBox);
			}
		}
	}

	//=================================================================================================================

	OBJToS3MBTool::OBJToS3MBTool()
	{
		m_pTreeNode = nullptr;
	}

	OBJToS3MBTool::~OBJToS3MBTool()
	{
		if (m_pTreeNode != nullptr)
		{
			delete m_pTreeNode;
			m_pTreeNode = nullptr;
		}
	}

	bool OBJToS3MBTool::ReadConfig(const wstring& strPath)
	{
		if (!StringUtil::IsFileExist(strPath))
		{
			return false;
		}

		m_strOBJDir = StringUtil::GetDir(strPath);
		return m_OBJConfig.LoadFromFile(strPath);
	}

	void OBJToS3MBTool::MakeQuarterTree()
	{
		int nLodNum = m_OBJConfig.GetLodNum();
		int nTileLength = m_OBJConfig.GetTileLength();
		GetQuarterChild(nLodNum, 0, m_pTreeNode, 0, nTileLength, 0, nTileLength);
	}

	bool OBJToS3MBTool::ProcessBodyData(const wstring& strOutputDir)
	{
		ProcessNode(strOutputDir, m_pTreeNode);
		return true;
	}

	bool OBJToS3MBTool::WriteConfig(const wstring& strOutputDir)
	{
		S3MBConfig config;
        OrientedBoundingBox obbox = m_pTreeNode->m_oriBundingBox;
		Point3D pos = m_OBJConfig.GetPosition();
		bool bHasSrs = m_OBJConfig.GetHasSrs();
		if (bHasSrs)
		{
            const Vector3d points = obbox.GetCenter();
			Rect2D rec;
			Vector3d vecPosCenter(pos.x, pos.y, pos.z);
			Vector3d vecPoint;
            vecPoint = MathEngine::GlobalENUToSpherical(vecPosCenter, points.x, points.y, points.z);

			rec.left = rec.right = vecPoint.x;
			rec.top = rec.bottom = vecPoint.y;
			for (int j = 1; j < 8; j++)
			{
                vecPoint = MathEngine::GlobalENUToSpherical(vecPosCenter, points.x, points.y, points.z);

				if (rec.left > vecPoint.x)
				{
					rec.left = vecPoint.x;
				}

				if (rec.right < vecPoint.x)
				{
					rec.right = vecPoint.x;
				}

				if (rec.top < vecPoint.y)
				{
					rec.top = vecPoint.y;
				}

				if (rec.bottom > vecPoint.y)
				{
					rec.bottom = vecPoint.y;
				}
			}
			config.SetBounds(rec);
		}

		config.SetAsset(U("SuperMap"));
		config.SetPosition(pos);

		std::vector<std::wstring> vecRootFiles;
        std::vector<OrientedBoundingBox> vecRootOBB;
		std::wstring strFileName = StringUtil::ChangeExt(m_pTreeNode->m_strFileName, U(".s3mb"));
		vecRootFiles.push_back(strFileName);
        vecRootOBB.push_back(m_pTreeNode->m_oriBundingBox);
        config.SetTiles(vecRootFiles, vecRootOBB);

		std::wstring strScpName = U("config.scp");
		std::wstring strScpFilePath = StringUtil::GetAbsolutePath(strOutputDir, strScpName);
		return config.WriteFile(strScpFilePath);
	}

	void OBJToS3MBTool::GetQuarterChild(unsigned int nLodNum, unsigned int nLod, OBJLodQuarterTreeNode* pNode, unsigned int nMinRow, unsigned int nMaxRow, unsigned int nMinCol, unsigned int nMaxCol)
	{
		unsigned int nCol = nMinCol, nRow = nMinRow;
		unsigned int nD = (nMaxRow - nMinRow) / 2;

		if (pNode == nullptr && m_pTreeNode == nullptr)
		{
			m_pTreeNode = CreateQTreeNode(nLod, nMinRow, nMinCol);
			pNode = m_pTreeNode;
		}

		if (pNode == nullptr)
		{
			return;
		}

		for (unsigned int i = 0; i < 2; i++)
		{
			nRow += i * nD;
			nCol = nMinCol;
			for (unsigned int j = 0; j < 2; j++)
			{
				nCol += j * nD;
				OBJLodQuarterTreeNode* pChild = CreateQTreeNode(nLod + 1, nRow, nCol);
				pNode->m_vecChild.push_back(pChild);

				unsigned int n = nLod + 1;
				if (n < nLodNum)
				{
					GetQuarterChild(nLodNum, n, pChild, nRow, nRow + nD, nCol, nCol + nD);
				}
			}
		}
	}

	OBJToS3MBTool::OBJLodQuarterTreeNode* OBJToS3MBTool::CreateQTreeNode(unsigned int nLod, unsigned int nRow, unsigned int nCol)
	{
#ifdef WIN32
		std::wstring strTileName = StringUtil::Format(U("tile_%d_%d_%d_tex"), nLod, nCol, nRow);
#else
		std::wstring strTileName = StringUtil::Format(U("tile_%d_%d_%d_tex").c_str(), nLod, nCol, nRow);
#endif
		OBJLodQuarterTreeNode* pChildNodeOne = new OBJLodQuarterTreeNode;
		std::wstring strObjFileName = StringUtil::ChangeExt(strTileName, U(".obj"));
		pChildNodeOne->m_strFileName = strObjFileName;
		pChildNodeOne->m_nLod = nLod;
		pChildNodeOne->m_nCol = nCol;
		pChildNodeOne->m_nRow = nRow;
		return pChildNodeOne;
	}

	void OBJToS3MBTool::ProcessNode(const wstring& strOutputDir, OBJLodQuarterTreeNode* pNode)
	{
		if (pNode == nullptr)
		{
			return;
		}

		unsigned int nSize = pNode->m_vecChild.size();
		if (nSize == 0)
		{
			return;
		}

		Matrix4d matModelTran = m_OBJConfig.GetModelTransform();
		int nLodNum = m_OBJConfig.GetLodNum();
		BoundingBox bboxMerged;
        OrientedBoundingBox obbMerged;
		RenderOperationGroup* pGroup = new RenderOperationGroup();
		// 把四分的子节点，合并成一层
		for (unsigned int i = 0; i < nSize; i++)
		{
			OBJLodQuarterTreeNode* pChildNode = pNode->m_vecChild[i];

			//填充 pGeode
			std::wstring strObjFileName = pChildNode->m_strFileName;
			std::wstring strFullPath = StringUtil::GetAbsolutePath(m_strOBJDir, strObjFileName);
			OSGBParser parser;
			parser.ParseNodeFile(strFullPath, U(""));
			RenderOperationGroup* pGroupObj = nullptr;
			parser.GetGroup(pGroupObj);
			if (pGroupObj == nullptr)
			{
				continue;
			}

			RenderOperationPagedLOD* pPagedLOD = new RenderOperationPagedLOD();
			RenderOperationGeode* pGeode = new RenderOperationGeode();
			pGroup->AddChild(pPagedLOD);
			pPagedLOD->AddChild(pGeode);

			AppendRO(pGroupObj, pGroup, pGeode);
            pGroupObj->ClearSkeleton();
			pGroupObj->ClearMaterial3D();
			pGroupObj->ClearTexData();
			delete pGroupObj;
			pGroupObj = nullptr;

			// 包围盒
            pGeode->ComputerBoundingBox();
            BoundingBox bboxGeode = pGeode->GetBoundingBox();
            OrientedBoundingBox obbGeode = pGeode->GetOrientedBoundingBox();
			if (matModelTran != Matrix4d::IDENTITY)
			{
				pGeode->SetPosition(matModelTran);
                bboxGeode.Transform(matModelTran);
			}
            obbMerged.Merge(obbGeode);
            bboxMerged.Merge(bboxGeode);
            BoundingBox bundingbox = BoundingBoxUtils::OBBToAABB(obbGeode);
            BoundingSphere bSphere(bundingbox);
			pGeode->SetBoundingSphere(bSphere.GetCenter(), bSphere.GetRadius());

			//对应的切换文件
			std::wstring strLodFile = StringUtil::ChangeExt(pNode->m_vecChild[i]->m_strFileName, U(".s3mb"));
			pPagedLOD->GetFileNames().push_back(strLodFile);
			// 计算LOD变换的距离
			double nLodDis = MIN(1024, bSphere.m_radius * pow(2.0, (nLodNum - pNode->m_nLod - 1)));
            nLodDis = bSphere.GetRadius() * 16.0f / nLodDis;
            pPagedLOD->SetRangeMode(RangeMode::Geometric_Error);
			pPagedLOD->GetRanges().push_back(std::vector<std::pair<float, float> >::value_type(0, nLodDis));
			pPagedLOD->SetBoundingSphere(bSphere.m_center, bSphere.m_radius);
		}

		if (pGroup->GetNumChildren() == 0)
		{
			return;
		}

		// 清空精细层的patch
        pNode->m_oriBundingBox = obbMerged;

		std::wstring strS3MFilePath = StringUtil::GetAbsolutePath(strOutputDir, pNode->m_strFileName);
		strS3MFilePath = StringUtil::ChangeExt(strS3MFilePath, U(".s3mb"));
		S3MBWriter writer;
		writer.SetROGroup(pGroup);
		writer.SetIsChangeTexture(true, false);//DXT压缩
		writer.WriteFile(strS3MFilePath);
		writer.Clear();
		delete pGroup;
		pGroup = nullptr;

		//处理子节点
		for (unsigned int i = 0; i < nSize; i++)
		{
			ProcessNode(strOutputDir, pNode->m_vecChild[i]);
		}
	}

	void OBJToS3MBTool::AppendRO(RenderOperationGroup* pGroupSrc, RenderOperationGroup* pGroupDes, RenderOperationGeode *pGeodeDes)
	{
        std::map<std::wstring, Skeleton*> mapSkeleton = pGroupSrc->GetSkeleton();
        std::map<std::wstring, Skeleton*>::iterator itGeo;
        for (itGeo = mapSkeleton.begin(); itGeo != mapSkeleton.end(); itGeo++)
		{
            pGroupDes->AddSkeleton(itGeo->first, itGeo->second);
            pGeodeDes->AddSkeleton(itGeo->second);
		}

		std::map<std::wstring, Material*> mapMaterial = pGroupSrc->GetMaterials();
		std::map<std::wstring, Material*>::iterator itMat = mapMaterial.begin();
		for (itMat = mapMaterial.begin(); itMat != mapMaterial.end(); itMat++)
		{
			pGroupDes->AddMaterial(itMat->first, itMat->second);
		}

		std::map<std::wstring, TextureDataInfo*> mapTex = pGroupSrc->GetTextureData();
		std::map<std::wstring, TextureDataInfo*>::iterator itTex = mapTex.begin();
		for (itTex = mapTex.begin(); itTex != mapTex.end(); itTex++)
		{
			pGroupDes->AddTextureData(itTex->first, itTex->second);
		}
	}

	//=================================================================================================================

	ProcessTools::ProcessTools()
	{
	}

	ProcessTools::~ProcessTools()
	{

	}

	bool ProcessTools::ThreeDTilesToS3MB(const ThreeDTilesParams& params)
	{
		m_3DTilesParams = params;
		m_3DTilesParser.SetPosCenter(params.GetPosition());
		m_3DTilesParser.SetIsNEU(params.GetIsNEU());
		m_3DTilesParser.SetTextureCompressType(params.GetTextureCompressType());

		m_3DTilesParser.ParseTileset(params.GetTilesetPath(), nullptr, 0, Matrix4d::IDENTITY);
		m_3DTilesParser.ComputeTransMat();

		return GenerateS3MB();
	}

	bool ProcessTools::S3MBTo3DTiles(const S3MBParams& params)
	{
		m_S3MBParams = params;
		const std::wstring& strSCPPath = params.GetSCPPath();
		const std::wstring& strOutputDir = params.GetOutputDir();

		if (!StringUtil::IsFileExist(strSCPPath))
		{
			return false;
		}

		if (!StringUtil::IsFileExist(strOutputDir))
		{
			FileUtil::MkDirEx(strOutputDir);
		}

		S3MBParser parser;
		parser.SetThreadNum(m_S3MBParams.GetThreadNum());
		if (!parser.ParseSCP(strSCPPath))
		{
			return false;
		}

		if (!parser.GenerateTilesetAndB3DM(strSCPPath, strOutputDir))
		{
			return false;
		}

		parser.GenerateTileset(strSCPPath, strOutputDir);
		return true;
	}

	bool ProcessTools::OSGBToS3MB(const OSGBParams& params)
	{
		m_OSGBParams = params;

		OSGBToS3MBTool tool;
		tool.ReadConfig(params.GetInputFilePath());
		tool.ProcessBodyData(params.GetOutputDir());
		tool.WriteConfig(params.GetOutputDir());
		return true;
	}

	bool ProcessTools::OBJToS3MB(const OBJParams& params)
	{
		OBJToS3MBTool tool;
		tool.ReadConfig(params.GetInputFilePath());
		tool.MakeQuarterTree();
		tool.ProcessBodyData(params.GetOutputDir());
		tool.WriteConfig(params.GetOutputDir());
		return true;
	}

	bool ProcessTools::I3SToS3MB(const I3SParams& params)
	{
		I3STool tool;
		return tool.ToS3M(params);
	}

	void ProcessTools::SaveGroup(GLTFTreeNode* pNode, std::wstring strOutPath, RenderOperationGroup* pGroup)
	{
		BoundingSphere bSphere;
		// 计算节点的包围盒
        std::map<wstring, Skeleton*>& mapSkeleton = pGroup->GetSkeleton();
		if (mapSkeleton.size() == 0)
		{
			bSphere.m_center = Vector3d(0, 0, 0);
			bSphere.m_radius = (pNode->m_boundSphere.valid() ? pNode->m_boundSphere.GetRadius() : GLOBAL_RADIUS);
			if (pGroup->GetNumChildren() > 0)
			{
				RenderOperationNode* pRONode = pGroup->GetChild(0);
				if (pRONode != nullptr)
				{
					pRONode->SetBoundingSphere(bSphere.m_center, bSphere.m_radius);
				}
			}
		}
		else
		{
			int nPagedLodNum = pGroup->GetNumChildren();
			for (unsigned int i = 0; i < nPagedLodNum; i++)
			{
				RenderOperationNode* pRONode = pGroup->GetChild(i);
				if (pRONode != nullptr)
				{
					BoundingSphere bs = pRONode->GetBoundingSphere();
					if (bs.valid())
					{
						bSphere.ExpandBy(bs);
					}
				}
			}
		}

		pNode->m_boundSphere = bSphere;

		std::wstring strDir = StringUtil::GetDir(strOutPath);
		if (!StringUtil::IsFileExist(strDir))
		{
			FileUtil::MkDirEx(strDir);
		}

		S3MBWriter writer;
		writer.SetROGroup(pGroup);
        writer.SetIsChangeTexture(true, true);
		writer.WriteFile(strOutPath);
		writer.Clear();

		if (m_3DTilesParams.GetIsModel())
		{//写入属性相关
			bool bValidIDRange = false;
			int IDMin = 0, IDMax = 0;
			for (unsigned int i = 0; i < pNode->m_vecChildNode.size(); i++)
			{
				
				std::pair<unsigned int, unsigned int> IdRange = pNode->m_vecChildNode[i]->m_pairIDRange;
				if (IdRange.first == 0 || IdRange.second == 0)
				{
					continue;
				}

				bValidIDRange = true;
				if (i == 0)
				{
					IDMin = IdRange.first;
					IDMax = IdRange.second;
				}
				else
				{
					IDMin = IDMin < IdRange.first ? IDMin : IdRange.first;
					IDMax = IDMax > IdRange.second ? IDMax : IdRange.second;
				}
			}
			if (!bValidIDRange)
			{
				const std::vector<Feature*>& vecFeature = m_3DTilesParser.GetFeatures();
				for (int i = 0; i < vecFeature.size(); i++)
				{
					if (vecFeature[i] != NULL)
					{
						delete vecFeature[i];
					}
				}
				m_3DTilesParser.ClearFeatures();
				return;
			}
			const std::vector<Feature*>& vecFeature = m_3DTilesParser.GetFeatures();
			std::wstring strAttrbuteFile = StringUtil::ChangeExt(strOutPath, U(".attribute"));
			std::map<unsigned int, std::pair<unsigned int, std::vector<Feature*> > > mapDatasetFeature;
			mapDatasetFeature[IDMax] = std::pair<unsigned int, std::vector<Feature*>>(IDMin, vecFeature);
			S3MBLayerInfos::WriteLayerAttributeValueToFile(mapDatasetFeature, strAttrbuteFile);
			for (int i = 0; i < vecFeature.size(); i++)
			{
				if (vecFeature[i] != NULL)
				{
					delete vecFeature[i];
				}
			}
			m_3DTilesParser.ClearFeatures();

			GLTFTreeNode* pRootNode = pNode;
			while (pRootNode->m_pParentNode != NULL)
			{
				pRootNode = pRootNode->m_pParentNode;
			}
			std::wstring strInputDir = StringUtil::GetDir(m_3DTilesParams.GetTilesetPath());
			std::wstring strOutputDir = m_3DTilesParams.GetOutputDir();

			std::wstring strFileRelativePath = StringUtil::GetRelativePath(strInputDir, StringUtil::GetDir(StringUtil::GetAbsolutePath(pRootNode->m_strParentDir, pRootNode->m_strFile)));
			std::wstring strFile = strFileRelativePath + StringUtil::GetName(pRootNode->m_strFile);
			strFile = StringUtil::ChangeExt(strFile, FILE_EXT_S3MB);
		
			GLTFAttributeIndexInfo attInfo;
			attInfo.m_strAttributeFileName = StringUtil::GetRelativePath(strOutputDir, StringUtil::GetDir(strAttrbuteFile)) + StringUtil::GetName(strAttrbuteFile);
			attInfo.m_pairIDRange.first = IDMin;
			attInfo.m_pairIDRange.second = IDMax;
			m_3DTilesParser.AddAttributeIndexInfo(strFile, attInfo);
		}
	}

	bool ProcessTools::SaveSCPFile(const Point3D& pos, const BoundingBox& box, std::vector<std::wstring> vecRootFile, std::vector<BoundingBox> vecRootBox, std::wstring strOutputDir)
	{
		std::wstring strRenderMode = m_3DTilesParams.GetIsModel() ? U("Batch") : U("Normal");
		std::wstring strFileType = m_3DTilesParams.GetIsModel() ? U("OSGBCacheFile") : U("OSGBFile");
		SCPSDataType nDataType = m_3DTilesParams.GetIsModel() ? SDT_ArtificialModel : SDT_RealityMesh;
		std::wstring strGlobeType = m_3DTilesParser.GetIsNEU() ? U("Ellipsoid_WGS84") : U("Sphere");
		SCPSLODType nLODType = m_3DTilesParser.GetIsAddMode() ? SLT_Add : SLT_Replace;
		std::wstring strProcessType = m_3DTilesParser.GetIsAddMode() ? U("Add") : U("Normal");

		std::wstring strTexCompType;
		bool bTexComp = m_3DTilesParams.GetIsTextureCompress();
		TextureCompressType nOriTexCompType = m_3DTilesParams.GetTextureCompressType();
		TextureCompressType nTexCompType = m_3DTilesParser.GetTextureCompressType();
		if (nTexCompType != TextureCompressType(-1))
		{
			TextureCompressType nType = !bTexComp ? nTexCompType : nOriTexCompType;
			strTexCompType = S3MBConfig::FromTextureCompressionType(nType);
		}

		std::wstring strVexCompType;
		if (m_3DTilesParams.GetVertexCompressType() != SV_Unkown)
		{
			strVexCompType = S3MBConfig::FromVertexCompressionType(m_3DTilesParams.GetVertexCompressType());
		}

		Vector3d min = box.GetMin();
		Vector3d max = box.GetMax();
		if (pos.x <= 180 && pos.x >= -180 && pos.y <= 90 && pos.y >= -90)
		{
			// 经纬度
			Vector3d temp = MathEngine::SphericalToCartesian(pos.x * DTOR, pos.y*DTOR, pos.z + GLOBAL_RADIUS);
			Matrix4d matRot = MatrixUtils::RotationXYZ(-pos.y * DTOR, pos.x*DTOR, 0);

			Matrix4d mat = matRot * MatrixUtils::Translation(temp.x, temp.y, temp.z);
			min = min.MultiplyMatrix(mat);
			max = max.MultiplyMatrix(mat);

			Vector3d tmin = MathEngine::CartesianToSphericalD(min.x, min.y, min.z);
			Vector3d tmax = MathEngine::CartesianToSphericalD(max.x, max.y, max.z);

			tmin.x = tmin.x * RTOD;
			tmin.y = tmin.y * RTOD;
			tmin.z = tmin.z - GLOBAL_RADIUS;

			tmax.x = tmax.x * RTOD;
			tmax.y = tmax.y * RTOD;
			tmax.z = tmax.z - GLOBAL_RADIUS;

			min = tmin;
			max = tmax;
		}
		else
		{
			// 平面坐标
			min.x = min.x + pos.x;
			min.y = min.y + pos.y;
			min.z = min.z + pos.z;

			max.x = max.x + pos.x;
			max.y = max.y + pos.y;
			max.z = max.z + pos.z;
		}

		// 设置地理范围信息
		if (min.x > max.x)
		{
			double tempValue = min.x;
			min.x = max.x;
			max.x = tempValue;
		}
		if (min.y > max.y)
		{
			double tempValue = min.y;
			min.y = max.y;
			max.y = tempValue;
		}
		Rect2D rect2D = Rect2D(min.x, max.y, max.x, min.y);

		std::vector<OrientedBoundingBox> vecOBB;
		for (int i = 0; i < vecRootBox.size(); i++)
		{
			OrientedBoundingBox obb;
			vecRootBox[i].ToOrientedBoundingBox(obb);
			vecOBB.push_back(obb);
		}

		S3MBConfig config;
		config.SetAsset(U("SuperMap"));
		config.SetDataType(nDataType);
		config.SetPosition(pos);
		config.SetBounds(rect2D);
		config.SetLodType(nLODType);
		config.SetTiles(vecRootFile, vecOBB);
		config.SetExtension(U("s3m:FileType"), strFileType);
		config.SetExtension(U("RenderMode"), strRenderMode);
		config.SetExtension(U("s3m:TileSplitType"), U("GLOBAL"));
		config.SetExtension(U("globeType"), strGlobeType);
		config.SetExtension(U("s3m:AttributeExtentName"), U("s3md"));
		config.SetExtension(U("s3m:TextureCompressionType"), strTexCompType);
		config.SetExtension(U("s3m:VertexCompressionType"), strVexCompType);
		config.SetExtension(U("ProcessType"), strProcessType);

		std::wstring strFilePath = StringUtil::GetAbsolutePath(strOutputDir, U("tilesetS3MB.scp"));
        return config.WriteFile(strFilePath);
	}

	bool ProcessTools::GenerateS3MB()
	{
		std::wstring strInputDir = StringUtil::GetDir(m_3DTilesParams.GetTilesetPath());
		std::wstring strOutputDir = m_3DTilesParams.GetOutputDir();
		Point3D posCenter = m_3DTilesParser.GetPosCenter();

		std::vector<std::wstring> vecRootFile;
		std::vector<BoundingBox> vecRootBBox;

		std::vector<std::pair<std::wstring, BoundingSphere> > vecBoxSphere;
		BoundingSphere boundingSphereTotal;
		BoundingBox boundingBox;

		m_3DTilesParser.ResetFieldInfos();

		std::vector<GLTFTreeNode*> vecAllNode;
		const std::vector<GLTFTreeNode*>& vecGLTFNode = m_3DTilesParser.GetGLTFNodes();
		for (unsigned int i = 0; i < vecGLTFNode.size(); i++)
		{
			GLTFTreeNode* pNode = vecGLTFNode[i];

			{
				GLTFTreeNode* pVirtualNode = new GLTFTreeNode();
				pVirtualNode->m_strFile = StringUtil::GetDir(pNode->m_strFile) + StringUtil::GetTitle(pNode->m_strFile) + U("_ROOT") + StringUtil::GetExt(pNode->m_strFile);
				pVirtualNode->m_strParentDir = pNode->m_strParentDir;
				pVirtualNode->m_boundSphere = pNode->m_boundSphere;
				pVirtualNode->m_vecChildNode.push_back(pNode);
				pVirtualNode->m_dGeometryError = pNode->m_dGeometryError;
				pNode->m_pParentNode = pVirtualNode;
				pNode = pVirtualNode;
				m_3DTilesParser.SetGLTFNode(i, pNode);
			}

			if (m_3DTilesParams.GetIsModel())
			{
				ParseIDRange(pNode);
			}

			GetNodes(pNode, vecAllNode);
		}

		// 所有节点单独处理
		for (unsigned int i = 0; i < vecAllNode.size(); i++)
		{
			ProcessTreeNode(vecAllNode[i], strInputDir, strOutputDir);
		}
		
		for (unsigned int i = 0; i < vecGLTFNode.size(); i++)
		{
			GLTFTreeNode* pNode = vecGLTFNode[i];
			/*求根节点的路径*/
			std::wstring strFileRelativePath = StringUtil::GetRelativePath(strInputDir, StringUtil::GetDir(StringUtil::GetAbsolutePath(pNode->m_strParentDir, pNode->m_strFile)));
			std::wstring strFile = strFileRelativePath + StringUtil::GetName(pNode->m_strFile);
			strFile = StringUtil::ChangeExt(strFile, FILE_EXT_S3MB);

			vecRootFile.push_back(strFile);

			BoundingSphere sphere = pNode->m_boundSphere;
			boundingSphereTotal.ExpandBy(sphere);
			Vector3d minBound, maxBound;

			minBound.x = sphere.m_center.x - sphere.m_radius;
			minBound.y = sphere.m_center.y - sphere.m_radius;
			minBound.z = sphere.m_center.z - sphere.m_radius;

			maxBound.x = sphere.m_center.x + sphere.m_radius;
			maxBound.y = sphere.m_center.y + sphere.m_radius;
			maxBound.z = sphere.m_center.z + sphere.m_radius;

			BoundingBox box(minBound, maxBound);
			boundingBox.Merge(box);
			vecRootBBox.push_back(box);
			vecBoxSphere.push_back(std::make_pair(strFile, sphere));
		}

		if (posCenter == Point3D())
		{
			posCenter = m_3DTilesParser.GetPosExtract();
		}

		bool bSaved = SaveSCPFile(posCenter, boundingBox, vecRootFile, vecRootBBox, strOutputDir);

		if (m_3DTilesParams.GetIsModel() && m_3DTilesParser.GetBatchIDNum() > 0)
		{
			LayerCacheInfo layerCacheInfo;
			layerCacheInfo.m_pairIDRange.first = 1;
			layerCacheInfo.m_pairIDRange.second = m_3DTilesParser.GetBatchIDNum();
			layerCacheInfo.m_fieldInfos = m_3DTilesParser.GetFieldInfos();
			layerCacheInfo.m_strDatasetName = U("TileSet");
			std::vector<LayerCacheInfo> vecLayerInfos;
			vecLayerInfos.push_back(layerCacheInfo);
			std::wstring strAttrbuteFile = StringUtil::GetAbsolutePath(strOutputDir, U("attrbute.json"));
			S3MBLayerInfos::WriteLayerFieldInfosToFile(vecLayerInfos, strAttrbuteFile);
		}
		return bSaved;
	}

	void ProcessTools::GetNodes(GLTFTreeNode* pNode, std::vector<GLTFTreeNode*>& vecNodes)
	{
		if (pNode->m_vecChildNode.size() == 0)
		{
			return;
		}

		vecNodes.push_back(pNode);
		for (unsigned int i = 0; i < pNode->m_vecChildNode.size(); i++)
		{
			GetNodes(pNode->m_vecChildNode[i], vecNodes);
		}
	}

	void ProcessTools::ProcessTreeNode(GLTFTreeNode* pNode, std::wstring strInputDir, std::wstring strOutputDir)
	{
		const std::vector<GLTFTreeNode*>& vecChildNode = pNode->m_vecChildNode;
		if (vecChildNode.size() == 0)
		{
			return;
		}

		RenderOperationGroup* pGroup = new RenderOperationGroup();
		//当前节点的父节点
		std::wstring strParentPath = StringUtil::GetRelativePath(strInputDir, pNode->m_strParentDir);
		strParentPath = StringUtil::GetAbsolutePath(strOutputDir, strParentPath);
		strParentPath = StringUtil::GetAbsolutePath(strParentPath, pNode->m_strFile);

		//合并子节点
		bool bChildFileExist = false;
		for (unsigned int i = 0; i < vecChildNode.size(); i++)
		{
			GLTFTreeNode* pChildNode = vecChildNode[i];
			if (pChildNode != nullptr && pChildNode->m_bFileFind)
			{
				ProcessNode(pChildNode, strInputDir, strOutputDir, pGroup, strParentPath);
				bChildFileExist = true;
			}
		}

		if (pGroup->GetNumChildren() > 0)
		{
			SaveGroup(pNode, StringUtil::ChangeExt(strParentPath, FILE_EXT_S3MB), pGroup);
		}

		delete pGroup;
		pGroup = nullptr;
	}

	void ProcessTools::ProcessNode(GLTFTreeNode* pNode, std::wstring strInputDir, std::wstring strOutputDir, RenderOperationGroup* pGroup, std::wstring strParentPath)
	{
		strOutputDir = StringUtil::GetAbsolutePath(strOutputDir, StringUtil::GetRelativePath(strInputDir, pNode->m_strParentDir));
		std::wstring strOutFile = StringUtil::GetAbsolutePath(strOutputDir, pNode->m_strFile);
		std::wstring strFile = StringUtil::GetAbsolutePath(pNode->m_strParentDir, pNode->m_strFile);
		
#ifdef WIN32
		std::ifstream ifs(strFile, ios::in | ios::binary);
#else
		string filePath = StringUtil::UnicodeToANSI(strFile);
		std::ifstream ifs(filePath, ios::in | ios::binary);
#endif
		if (!ifs)
		{
			return;
		}
		ifs.seekg(0, ios::end);
		int size = ifs.tellg();
		ifs.seekg(0, ios::beg);
		char* buffer = new char[size];
		ifs.read(buffer, size);
		ifs.close();

		MemoryStream stream;
		stream.Init(buffer, size);

		m_3DTilesParser.SetBufferSize(size);

		TileContentType nContentType = ParseTileContentType(stream);
		if (nContentType == TileContentType::TC_CMPT)
		{
			ParseCMPT(stream, pNode, strOutFile, pGroup, strParentPath);
		}
		else if (nContentType == TileContentType::TC_B3DM)
		{
			ParseB3DM(stream, pNode, strOutFile, pGroup, strParentPath);
		}
		else if (nContentType == TileContentType::TC_I3DM)
		{
			ParseI3DM(stream, pNode, strOutFile, pGroup, strParentPath);
		}

		delete[] buffer;
		buffer = nullptr;
	}

	bool ProcessTools::ParseCMPT(MemoryStream& stream, GLTFTreeNode* pNode, std::wstring strOutputPath, RenderOperationGroup* pGroup, std::wstring strParentPath)
	{
		TileContentType nContentType = ParseTileContentType(stream, false);
		if (nContentType != TileContentType::TC_CMPT)
		{
			return false;
		}

		unsigned int nVersion, nByteLength, nTilesLength;
		stream >> nVersion >> nByteLength >> nTilesLength;
		for (int i = 0; i < nTilesLength; i++)
		{
			unsigned int tileBeginOffset = stream.GetReadPosition();
			nContentType = ParseTileContentType(stream, false);
			unsigned int nVersion, nTotalLenth;
			stream >> nVersion >> nTotalLenth;
			stream.SetReadPosition(tileBeginOffset);
			m_3DTilesParser.SetBufferSize(nTotalLenth);
			if (nContentType == TileContentType::TC_CMPT)
			{
				ParseCMPT(stream, pNode, strOutputPath, pGroup, strParentPath);
			}
			else if (nContentType == TileContentType::TC_B3DM)
			{
				ParseB3DM(stream, pNode, strOutputPath, pGroup, strParentPath);
			}
			else if (nContentType == TileContentType::TC_I3DM)
			{
				ParseI3DM(stream, pNode, strOutputPath, pGroup, strParentPath);
			}
			stream.SetReadPosition(tileBeginOffset + nTotalLenth);
		}
		return true;
	}

	bool ProcessTools::ParseB3DM(MemoryStream& stream, GLTFTreeNode* pNode, std::wstring strOutputPath, RenderOperationGroup* pGroup, std::wstring strParentPath)
	{
		unsigned int nOffset = stream.GetReadPosition();
		TileContentType nContentType = ParseTileContentType(stream, false);
		if (nContentType != TileContentType::TC_B3DM)
		{
			return false;
		}

		unsigned int nVersion, nTotalLenth;
		stream >> nVersion >> nTotalLenth;
		unsigned int nFeatureTableLength, nFeatureBinLength, nBatchTableLength, nBatchBinLength;
		stream >> nFeatureTableLength >> nFeatureBinLength >> nBatchTableLength >> nBatchBinLength;
		nOffset += 28;
		Point3D pntCenter;
		unsigned int nBatchSize = 0;
#pragma region 兼容以前版本B3DM
		if (nBatchTableLength >= 570425344)
		{
			nOffset -= sizeof(unsigned int) * 2;
			nBatchTableLength = nFeatureBinLength;
			nBatchBinLength = 0;
			nFeatureTableLength = 0;
			nFeatureBinLength = 0;
		}
		else if (nBatchBinLength >= 570425344)
		{
			nOffset -= sizeof(unsigned int);
			nBatchTableLength = nFeatureTableLength;
			nBatchBinLength = nFeatureBinLength;
			nFeatureTableLength = 0;
			nFeatureBinLength = 0;
		}
#pragma endregion
		else if (nFeatureTableLength > 0)
		{
			unsigned char* pFeatureJson = new unsigned char[nFeatureTableLength];
			stream.Load(pFeatureJson, nFeatureTableLength);

			rapidjson::Document docFeature = ParseBuffer(pFeatureJson, nFeatureTableLength);
			if (docFeature.HasMember("RTC_CENTER"))
			{
				rapidjson::Value & vCenter = docFeature["RTC_CENTER"];
				pntCenter.x = vCenter.GetArray()[0].GetDouble();
				pntCenter.y = vCenter.GetArray()[1].GetDouble();
				pntCenter.z = vCenter.GetArray()[2].GetDouble();
			}

			if (m_3DTilesParams.GetIsModel())
			{
				unsigned char* pFeatrueBin = nullptr;
				if (nFeatureBinLength > 0)
				{
					pFeatrueBin = new unsigned char[nFeatureBinLength];
					stream.Load(pFeatrueBin, nFeatureBinLength);
				}

				unsigned char* pBatchJson = nullptr;
				if (nBatchTableLength > 0)
				{
					pBatchJson = new unsigned char[nBatchTableLength];
					stream.Load(pBatchJson, nBatchTableLength);
				}
				unsigned char* pBatchBin = nullptr;
				if (nBatchBinLength > 0)
				{
					pBatchBin = new unsigned char[nBatchBinLength];
					stream.Load(pBatchBin, nBatchBinLength);
				}

				std::vector<Feature*> vecFeature;
				nBatchSize = docFeature["BATCH_LENGTH"].GetUint();
				for (unsigned int i = 0; i < nBatchSize; i++)
				{
					std::wstring strID = U("SmID");
					int nID = pNode->m_pairIDRange.first + i;

					S3MBFieldInfo fieldInfo;
					fieldInfo.m_nType = FieldType::FT_INT32;
					fieldInfo.m_nSize = 4;
					fieldInfo.m_strName = strID;
					fieldInfo.m_strForeignName = strID;
					fieldInfo.m_bRequired = true;

					S3MBFieldInfos fieldInfos;
					fieldInfos.add(fieldInfo);

					Feature* pFeature = new Feature;
					pFeature->SetFieldInfos(fieldInfos);
					pFeature->m_nID = nID;
					pFeature->SetValue(strID, Variant(nID));
					
					vecFeature.push_back(pFeature);
				}

				if (nBatchTableLength > 0 && nBatchSize > 0)
				{
					rapidjson::Document docBatch = ParseBuffer(pBatchJson, nBatchTableLength);
					for (int i = 0; i < vecFeature.size(); i++)
					{
						S3MBFieldInfos fieldInfos = m_3DTilesParser.GetFieldInfos();
						vecFeature[i]->SetFieldInfos(fieldInfos);
					}
					for (auto itor = docBatch.MemberBegin(); itor != docBatch.MemberEnd(); itor++)
					{
						auto key = (itor->name).GetString();
						std::wstring strKey = StringUtil::UTF8_to_UNICODE(key);
						rapidjson::Value& member = docBatch[key];
						if (member.IsArray())
						{
							for (int i = 0; i < member.Size(); i++)
							{
								std::wstring strValue;
								if (member[i].IsNull())
								{
									strValue = U("null");
								}
								else if (member[i].IsString())
								{
									strValue = StringUtil::UTF8_to_UNICODE(member[i].GetString());
								}
								else if (member[i].IsFloat())
								{
#ifdef WIN32
                                    strValue = StringUtil::Format(U("%f"), member[i].GetFloat());
#else
                                    strValue = StringUtil::Format(U("%f").c_str(), member[i].GetFloat());
#endif
								}
								else if (member[i].IsInt())
								{
#ifdef WIN32
                                    strValue = StringUtil::Format(U("%d"), member[i].GetInt());
#else
                                    strValue = StringUtil::Format(U("%d").c_str(), member[i].GetInt());
#endif
								}
								else if (member[i].IsBool())
								{
									strValue = (member[i].GetBool() ? U("True") : U("False"));
								}

								vecFeature[i]->SetValue(strKey, StringUtil::UNICODE_to_UTF8(strValue));
							}
						}
						else
						{
							unsigned int nByteOffset = member["byteOffset"].GetUint();

							std::string	strCompType = member["componentType"].GetString();
							std::string strType = member["type"].GetString();
							FieldType nType = GetFieldType(strCompType);
							int nDim = NumOfComponents(strType);

							switch (nType)
							{
							case FT_Byte:
							{
								for (unsigned int i = 0; i < nBatchSize; i++)
								{
									std::wstring strValue;
									for (int iDim = 0; iDim < nDim; iDim++)
									{
										std::wstring strPart;
										unsigned char nByte = (unsigned char)pBatchBin[nByteOffset + i * nDim + iDim];
#ifdef WIN32
                                        strPart = StringUtil::Format(U("%d"), nByte);
#else
                                        strPart = StringUtil::Format(U("%d").c_str(), nByte);
#endif
										if (iDim != 0)
										{
											strPart = U(",") + strPart;
										}
										strValue += strPart;
									}
									vecFeature[i]->SetValue(strKey, StringUtil::UNICODE_to_UTF8(strValue));
								}
							}
							break;
							case FT_INT16:
							{
								short* pBinOffset = (short*)(pBatchBin + nByteOffset);
								for (unsigned int i = 0; i < nBatchSize; i++)
								{
									std::wstring strValue;
									for (int iDim = 0; iDim < nDim; iDim++)
									{
										std::wstring strPart;
										short nValue = pBinOffset[i * nDim + iDim];
										strPart = StringUtil::Format(L"%d", nValue);
										if (iDim != 0)
										{
											strPart = L"," + strPart;
										}
										strValue += strPart;
									}
									vecFeature[i]->SetValue(strKey, StringUtil::UNICODE_to_UTF8(strValue));
								}
							}
							break;
							case FT_INT32:
							{
								int* pBinOffset = (int*)(pBatchBin + nByteOffset);
								for (unsigned int i = 0; i < nBatchSize; i++)
								{
									std::wstring strValue;
									for (int iDim = 0; iDim < nDim; iDim++)
									{
										std::wstring strPart;
										int nValue = pBinOffset[i * nDim + iDim];
										strPart = StringUtil::Format(L"%d", nValue);
										if (iDim != 0)
										{
											strPart = L"," + strPart;
										}
										strValue += strPart;
									}
									vecFeature[i]->SetValue(strKey, StringUtil::UNICODE_to_UTF8(strValue));
								}
							}
							break;
							case FT_Float:
							{
								float* pBinOffset = (float*)(pBatchBin + nByteOffset);
								for (unsigned int i = 0; i < nBatchSize; i++)
								{
									std::wstring strValue;
									for (int iDim = 0; iDim < nDim; iDim++)
									{
										std::wstring strPart;
										float fValue = pBinOffset[i * nDim + iDim];
										strPart = StringUtil::Format(L"%f", fValue);
										if (iDim != 0)
										{
											strPart = L"," + strPart;
										}
										strValue += strPart;
									}
									vecFeature[i]->SetValue(strKey, StringUtil::UNICODE_to_UTF8(strValue));
								}
							}
							break;
							case FT_Double:
							{
								double* pBinOffset = (double*)(pBatchBin + nByteOffset);
								for (unsigned int i = 0; i < nBatchSize; i++)
								{
									std::wstring strValue;
									for (int iDim = 0; iDim < nDim; iDim++)
									{
										std::wstring strPart;
										double dValue = pBinOffset[i * nDim + iDim];
										strPart = StringUtil::Format(L"%f", dValue);
										if (iDim != 0)
										{
											strPart = L"," + strPart;
										}
										strValue += strPart;
									}
									vecFeature[i]->SetValue(strKey, StringUtil::UNICODE_to_UTF8(strValue));
								}
							}
							break;
							default:
								break;
							}
						}
					}
				}

				for (int i = 0; i < vecFeature.size(); i++)
				{
					m_3DTilesParser.AddFeature(vecFeature[i]);
				}

				delete pFeatrueBin;
				pFeatrueBin = nullptr;
				delete pBatchJson;
				pBatchJson = nullptr;
				delete pBatchBin;
				pBatchBin = nullptr;
			}

			delete[] pFeatureJson;
			pFeatureJson = nullptr;
		}

		unsigned int nStartGltfOffset = nOffset + nFeatureBinLength + nFeatureTableLength + nBatchTableLength + nBatchBinLength;
		stream.SetReadPosition(nStartGltfOffset);

		unsigned char gltfHead[4];
		stream >> gltfHead[0] >> gltfHead[1] >> gltfHead[2] >> gltfHead[3];

		unsigned int nGltfVersion, nGLTFTotal, nJsonLenth, nJson;
		stream >> nGltfVersion >> nGLTFTotal >> nJsonLenth >> nJson;

		nStartGltfOffset = nStartGltfOffset + 20;

		unsigned char* pBuffer = stream.GetDataPtr() + nStartGltfOffset;
		unsigned int nBufferSize = m_3DTilesParser.GetBufferSize();
		m_3DTilesParser.SetBufferSize(nBufferSize - nStartGltfOffset);
		if (nJsonLenth > 0)
		{
			nBufferSize = m_3DTilesParser.GetBufferSize();
			m_3DTilesParser.SetBufferSize(nBufferSize - nJsonLenth);
			if (EQUAL(nGltfVersion, 2.0))
			{
				GLTFTileInfos_2* pTileInfos = m_3DTilesParser.ParseGLTF(pNode, pBuffer, nJsonLenth, strOutputPath);
				if (pTileInfos != nullptr)
				{
					Point3D pntRtcCenter(pntCenter.x + pTileInfos->m_vCesuimRTC.x, pntCenter.y + pTileInfos->m_vCesuimRTC.y, pntCenter.z + pTileInfos->m_vCesuimRTC.z);
					MeshToGroup(pNode, pTileInfos, strOutputPath, pGroup, strParentPath, pntRtcCenter);
					delete pTileInfos;
					pTileInfos = nullptr;
				}
			}
			else if (EQUAL(nGltfVersion, 1.0))
			{
				GLTFTileInfos_1* pTileInfos = m_3DTilesParser.ParseGLTF_V1(pBuffer, nJsonLenth, strOutputPath);
				if (pTileInfos != nullptr)
				{
					Point3D pntRtcCenter(pntCenter.x + pTileInfos->m_vCesuimRTC.x, pntCenter.y + pTileInfos->m_vCesuimRTC.y, pntCenter.z + pTileInfos->m_vCesuimRTC.z);
					MeshToGroup(pNode, pTileInfos, strOutputPath, pGroup, strParentPath, pntRtcCenter);
					delete pTileInfos;
					pTileInfos = nullptr;
				}
			}
			m_3DTilesParser.Clear();
		}

		return true;
	}

	bool ProcessTools::ParseI3DM(MemoryStream& stream, GLTFTreeNode* pNode, std::wstring strOutDir, RenderOperationGroup* pGroup, std::wstring strFatherFileName)
	{
		unsigned int nOffset = stream.GetReadPosition();
		TileContentType nContentType = ParseTileContentType(stream, false);
		if (nContentType != TileContentType::TC_I3DM)
		{
			return false;
		}

		unsigned int nVersion, nTotalLenth;
		stream >> nVersion >> nTotalLenth;
		unsigned int nFeatureTableLength, nFeatureBinLength, nBatchTableLength, nBatchBinLength, nGltfFormat;
		stream >> nFeatureTableLength >> nFeatureBinLength >> nBatchTableLength >> nBatchBinLength >> nGltfFormat;
		nOffset += 32;
		Vector3d ptTileCenter;
		unsigned int nInstanceSize = 0;

		unsigned char* pFeatureJson = new unsigned char[nFeatureTableLength];
		stream.Load(pFeatureJson, nFeatureTableLength);

		rapidjson::Document docFeature = ParseBuffer(pFeatureJson, nFeatureTableLength);

		if (docFeature.HasMember("RTC_CENTER"))
		{
			rapidjson::Value & vCenter = docFeature["RTC_CENTER"];
			ptTileCenter.x = vCenter.GetArray()[0].GetDouble();
			ptTileCenter.y = vCenter.GetArray()[1].GetDouble();
			ptTileCenter.z = vCenter.GetArray()[2].GetDouble();
		}

		unsigned char* pFeatrueBin = NULL;
		if (nFeatureBinLength > 0)
		{
			pFeatrueBin = new unsigned char[nFeatureBinLength];
			stream.Load(pFeatrueBin, nFeatureBinLength);
		}
		
		m_3DTilesParser.ParseInstanceInfo(docFeature, pFeatrueBin);

		if (m_3DTilesParams.GetIsModel())
        {
			unsigned char* pBatchJson = NULL;
			if (nBatchTableLength > 0)
			{
				pBatchJson = new unsigned char[nBatchTableLength];
				stream.Load(pBatchJson, nBatchTableLength);
			}
			unsigned char* pBatchBin = NULL;
			if (nBatchBinLength > 0)
			{
				pBatchBin = new unsigned char[nBatchBinLength];
				stream.Load(pBatchBin, nBatchBinLength);
			}

			std::vector<Feature*> arrFeature;
			rapidjson::Value & vIS = docFeature["INSTANCES_LENGTH"];
			nInstanceSize = vIS.GetUint();
			std::wstring strSmID = U("SmID");
			for (unsigned int i = 0; i < nInstanceSize; i++)
			{
				S3MBFieldInfo pFieldInfo;
				pFieldInfo.m_nType = FT_INT32;
				pFieldInfo.m_nSize = 4;
				pFieldInfo.m_strName = strSmID;
				pFieldInfo.m_strForeignName = strSmID;
				pFieldInfo.m_bRequired = true;

				S3MBFieldInfos fieldInfos;
				fieldInfos.add(pFieldInfo);

				Feature* pFeature = new Feature;
				pFeature->SetFieldInfos(fieldInfos);
				pFeature->m_nID = pNode->m_pairIDRange.first + i;
				pFeature->SetValue(strSmID, Variant(pFeature->m_nID));

				arrFeature.push_back(pFeature);
			}

			if (nInstanceSize > 0 && nBatchTableLength > 0)
			{
				rapidjson::Document docBatch = ParseBuffer(pBatchJson, nBatchTableLength);
				
				for (int i = 0; i < arrFeature.size(); i++)
				{
					S3MBFieldInfos fieldInfos = m_3DTilesParser.GetFieldInfos();
					arrFeature[i]->SetFieldInfos(fieldInfos);
				}

				for (auto itor = docBatch.MemberBegin(); itor != docBatch.MemberEnd(); itor++)
				{
					auto key = (itor->name).GetString();
					std::wstring strKey = StringUtil::UTF8_to_UNICODE(key);
					rapidjson::Value& member = docBatch[key];

					if (member.IsArray())
					{
						for (int i = 0; i < member.Size(); i++)
						{
							std::wstring strValue;
							if (member[i].IsNull())
							{
								strValue = U("null");
							}
							else if (member[i].IsString())
							{
								strValue = StringUtil::UTF8_to_UNICODE(member[i].GetString());
							}
							else if (member[i].IsFloat())
							{
#ifdef WIN32
								strValue = StringUtil::Format(U("%f"), member[i].GetFloat());
#else
								strValue = StringUtil::Format(U("%f").c_str(), member[i].GetFloat());
#endif
							}
							else if (member[i].IsInt())
							{
#ifdef WIN32
								strValue = StringUtil::Format(U("%d"), member[i].GetInt());
#else
                                strValue = StringUtil::Format(U("%d").c_str(), member[i].GetInt());
#endif
							}
							else if (member[i].IsBool())
							{
								strValue = (member[i].GetBool() ? U("True") : U("False"));
							}

							arrFeature[i]->SetValue(strKey, StringUtil::UNICODE_to_UTF8(strValue));
						}
					}
					else
					{
						unsigned int nByteOffset = member["byteOffset"].GetUint();

						std::string	strCompType = member["componentType"].GetString();
						std::string strType = member["type"].GetString();
						FieldType nType = GetFieldType(strCompType);
						int nDim = NumOfComponents(strType);

						switch (nType)
						{
						case FT_Byte:
						{
							for (unsigned int i = 0; i < nInstanceSize; i++)
							{
								std::wstring strValue;
								for (int iDim = 0; iDim < nDim; iDim++)
								{
									std::wstring strPart;
									unsigned char nByte = (unsigned char)pBatchBin[nByteOffset + i * nDim + iDim];
#ifdef WIN32
									strPart = StringUtil::Format(U("%d"), nByte);
#else
									strPart = StringUtil::Format(U("%d").c_str(), nByte);
#endif
									if (iDim != 0)
									{
										strPart = U(",") + strPart;
									}
									strValue += strPart;
								}
								arrFeature[i]->SetValue(strKey, StringUtil::UNICODE_to_UTF8(strValue));
							}
						}
						break;
						case FT_INT16:
						{
							short* pBinOffset = (short*)(pBatchBin + nByteOffset);
							for (unsigned int i = 0; i < nInstanceSize; i++)
							{
								std::wstring strValue;
								for (int iDim = 0; iDim < nDim; iDim++)
								{
									std::wstring strPart;
									short nValue = pBinOffset[i * nDim + iDim];
									strPart = StringUtil::Format(L"%d", nValue);
									if (iDim != 0)
									{
										strPart = L"," + strPart;
									}
									strValue += strPart;
								}
								arrFeature[i]->SetValue(strKey, StringUtil::UNICODE_to_UTF8(strValue));
							}
						}
						break;
						case FT_INT32:
						{
							int* pBinOffset = (int*)(pBatchBin + nByteOffset);
							for (unsigned int i = 0; i < nInstanceSize; i++)
							{
								std::wstring strValue;
								for (int iDim = 0; iDim < nDim; iDim++)
								{
									std::wstring strPart;
									int nValue = pBinOffset[i * nDim + iDim];
									strPart = StringUtil::Format(L"%d", nValue);
									if (iDim != 0)
									{
										strPart = L"," + strPart;
									}
									strValue += strPart;
								}
								arrFeature[i]->SetValue(strKey, StringUtil::UNICODE_to_UTF8(strValue));
							}
						}
						break;
						case FT_Float:
						{
							float* pBinOffset = (float*)(pBatchBin + nByteOffset);
							for (unsigned int i = 0; i < nInstanceSize; i++)
							{
								std::wstring strValue;
								for (int iDim = 0; iDim < nDim; iDim++)
								{
									std::wstring strPart;
									float fValue = pBinOffset[i * nDim + iDim];
									strPart = StringUtil::Format(L"%f", fValue);
									if (iDim != 0)
									{
										strPart = L"," + strPart;
									}
									strValue += strPart;
								}
								arrFeature[i]->SetValue(strKey, StringUtil::UNICODE_to_UTF8(strValue));
							}
						}
						break;
						case FT_Double:
						{
							double* pBinOffset = (double*)(pBatchBin + nByteOffset);
							for (unsigned int i = 0; i < nInstanceSize; i++)
							{
								std::wstring strValue;
								for (int iDim = 0; iDim < nDim; iDim++)
								{
									std::wstring strPart;
									double dValue = pBinOffset[i * nDim + iDim];
									strPart = StringUtil::Format(L"%f", dValue);
									if (iDim != 0)
									{
										strPart = L"," + strPart;
									}
									strValue += strPart;
								}
								arrFeature[i]->SetValue(strKey, StringUtil::UNICODE_to_UTF8(strValue));
							}
						}
						break;
						default:
							break;
						}
					}
				}
			}

			for (int i = 0; i < arrFeature.size(); i++)
			{
				m_3DTilesParser.AddFeature(arrFeature[i]);
			}

			if (pFeatrueBin)
			{
				delete pFeatrueBin;
				pFeatrueBin = NULL;
			}
			if (pBatchJson)
			{
				delete pBatchJson;
				pBatchJson = NULL;
			}
			if (pBatchBin)
			{
				delete pBatchBin;
				pBatchBin = NULL;
			}
		}

		unsigned int nStartGltfOffset = nOffset + nFeatureBinLength + nFeatureTableLength + nBatchTableLength + nBatchBinLength;

		stream.SetReadPosition(nStartGltfOffset);

		unsigned char gltfHead[4];
		stream >> gltfHead[0] >> gltfHead[1] >> gltfHead[2] >> gltfHead[3];

		unsigned int nGltfVersion, nGLTFTotal, nJsonLenth, nJson;
		stream >> nGltfVersion >> nGLTFTotal >> nJsonLenth >> nJson;

		nStartGltfOffset = nStartGltfOffset + 20;

		unsigned char* pBuffer = stream.GetDataPtr() + nStartGltfOffset;
		unsigned int nBufferSize = m_3DTilesParser.GetBufferSize();
		m_3DTilesParser.SetBufferSize(nBufferSize - nStartGltfOffset);
		if (nJsonLenth > 0)
		{
			nBufferSize = m_3DTilesParser.GetBufferSize();
			m_3DTilesParser.SetBufferSize(nBufferSize - nJsonLenth);
			if (EQUAL(nGltfVersion, 2.0))
			{
				GLTFTileInfos_2* pTileInfos = m_3DTilesParser.ParseGLTF(pNode, pBuffer, nJsonLenth, strOutDir);
				if (pTileInfos != nullptr)
				{
					Point3D pntRtcCenter(ptTileCenter.x + pTileInfos->m_vCesuimRTC.x, ptTileCenter.y + pTileInfos->m_vCesuimRTC.y, ptTileCenter.z + pTileInfos->m_vCesuimRTC.z);
					MeshToGroup(pNode, pTileInfos, strOutDir, pGroup, strFatherFileName, pntRtcCenter);
					delete pTileInfos;
					pTileInfos = nullptr;
				}
			}
			else if (EQUAL(nGltfVersion, 1.0))
			{
				GLTFTileInfos_1* pTileInfos = m_3DTilesParser.ParseGLTF_V1(pBuffer, nJsonLenth, strOutDir);
				if (pTileInfos != nullptr)
				{
					Point3D pntRtcCenter(ptTileCenter.x + pTileInfos->m_vCesuimRTC.x, ptTileCenter.y + pTileInfos->m_vCesuimRTC.y, ptTileCenter.z + pTileInfos->m_vCesuimRTC.z);
					MeshToGroup(pNode, pTileInfos, strOutDir, pGroup, strFatherFileName, pntRtcCenter);
					delete pTileInfos;
					pTileInfos = nullptr;
				}
			}
			m_3DTilesParser.Clear();
		}

		return true;
	}

	void ProcessTools::ParseIDRange(GLTFTreeNode* pNode)
	{
		for (unsigned int i = 0; i < pNode->m_vecChildNode.size(); i++)
		{
			unsigned int nIdOffset = m_3DTilesParser.GetBatchIDNum();

			std::wstring strFile = StringUtil::GetAbsolutePath(pNode->m_vecChildNode[i]->m_strParentDir, pNode->m_vecChildNode[i]->m_strFile);

			std::string strTemp = StringUtil::UNICODE_to_UTF8(strFile);
			strTemp = StringUtil::UTF8_to_ANSI(strTemp);
			FILE* file = fopen(strTemp.c_str(), "rb");
			if (file == NULL)
			{
				continue;
			}
			fseek(file, 0, SEEK_END);
			int size = ftell(file);
			fclose(file);

			file = fopen(strTemp.c_str(), "rb");
			if (file == NULL)
			{
				continue;
			}
			unsigned char* buffer = (unsigned char*)malloc(size);
			unsigned int bytesRead = fread(buffer, sizeof(unsigned char), size, file);
			fclose(file);

			MemoryStream stream;
			stream.Init(buffer, size);

			unsigned int tileBeginOffset = stream.GetReadPosition();
			unsigned char head[4];
			stream >> head[0] >> head[1] >> head[2] >> head[3];
			stream.SetReadPosition(0);
			if (head[0] == 'c'  && head[1] == 'm' && head[2] == 'p' && head[3] == 't')
			{
				ParseIDRangeFromCMPT(stream);
			}
			else if (head[0] == 'b'  && head[1] == '3' && head[2] == 'd' && head[3] == 'm')
			{
				ParseIDRangeFromB3DM(stream);
			}
			else if (head[0] == 'i'  && head[1] == '3' && head[2] == 'd' && head[3] == 'm')
			{
				ParseIDRangeFromI3DM(stream);
			}
			if (nIdOffset < m_3DTilesParser.GetBatchIDNum())
			{
				pNode->m_vecChildNode[i]->m_pairIDRange = std::make_pair(nIdOffset + 1, m_3DTilesParser.GetBatchIDNum());
			}
		}
		for (unsigned int i = 0; i < pNode->m_vecChildNode.size(); i++)
		{
			ParseIDRange(pNode->m_vecChildNode[i]);
		}
	}

	void ProcessTools::ParseIDRangeFromB3DM(MemoryStream& stream)
	{
		unsigned int nOffset = stream.GetReadPosition();
		unsigned char head[4];
		stream >> head[0] >> head[1] >> head[2] >> head[3];
		if (!(head[0] == 'b'  && head[1] == '3' && head[2] == 'd' && head[3] == 'm'))
		{
			return;
		}
		unsigned int nVersion, nTotalLenth;
		stream >> nVersion >> nTotalLenth;
		unsigned int nFeatureTableLength, nFeatureBinLength, nBatchTableLength, nBatchBinLength;
		stream >> nFeatureTableLength >> nFeatureBinLength >> nBatchTableLength
			>> nBatchBinLength;
		nOffset += 28;
		Vector3d ptTileCenter;
		unsigned int nBatchSize = 0;
		if (!(nBatchTableLength >= 570425344 || nBatchBinLength >= 570425344) && nFeatureTableLength > 0)
		{
			unsigned char* pFeatureJson = new unsigned char[nFeatureTableLength];
			stream.Load(pFeatureJson, nFeatureTableLength);

			rapidjson::Document docFeature = ParseBuffer(pFeatureJson, nFeatureTableLength);

			rapidjson::Value & vBS = docFeature["BATCH_LENGTH"];
			nBatchSize = vBS.GetUint();
			if (nBatchSize > 0)
			{
				unsigned int nBatchIDNum = m_3DTilesParser.GetBatchIDNum();
				m_3DTilesParser.SetBatchIDNum(nBatchIDNum + nBatchSize);
			}
		}
	}

	void ProcessTools::ParseIDRangeFromCMPT(MemoryStream& stream)
	{
		unsigned char head[4];
		stream >> head[0] >> head[1] >> head[2] >> head[3];
		if (!(head[0] == 'c'  && head[1] == 'm' && head[2] == 'p' && head[3] == 't'))
		{
			return;
		}
		unsigned int nVersion, nByteLength, nTilesLength;
		stream >> nVersion >> nByteLength >> nTilesLength;
		for (int i = 0; i < nTilesLength; i++)
		{
			unsigned int tileBeginOffset = stream.GetReadPosition();
			stream >> head[0] >> head[1] >> head[2] >> head[3];
			unsigned int nVersion, nTotalLenth;
			stream >> nVersion >> nTotalLenth;
			stream.SetReadPosition(tileBeginOffset);
			if ((head[0] == 'c'  && head[1] == 'm' && head[2] == 'p' && head[3] == 't'))
			{
				ParseIDRangeFromCMPT(stream);
			}
			else if (head[0] == 'b'  && head[1] == '3' && head[2] == 'd' && head[3] == 'm')
			{
				ParseIDRangeFromB3DM(stream);
			}
			else if (head[0] == 'i'  && head[1] == '3' && head[2] == 'd' && head[3] == 'm')
			{
				ParseIDRangeFromI3DM(stream);
			}
			stream.SetReadPosition(tileBeginOffset + nTotalLenth);
		}
	}

	void ProcessTools::ParseIDRangeFromI3DM(MemoryStream& stream)
	{
		unsigned int nOffset = stream.GetReadPosition();
		unsigned char head[4];
		stream >> head[0] >> head[1] >> head[2] >> head[3];
		if (!(head[0] == 'i'  && head[1] == '3' && head[2] == 'd' && head[3] == 'm'))
		{
			return;
		}
		unsigned int nVersion, nTotalLenth;
		stream >> nVersion >> nTotalLenth;
		unsigned int nFeatureTableLength, nFeatureBinLength, nBatchTableLength, nBatchBinLength, nGltfFormat;
		stream >> nFeatureTableLength >> nFeatureBinLength >> nBatchTableLength >> nBatchBinLength >> nGltfFormat;
		nOffset += 32;
		Vector3d ptTileCenter;
		unsigned int nInstanceSize = 0;

		unsigned char* pFeatureJson = new unsigned char[nFeatureTableLength];
		stream.Load(pFeatureJson, nFeatureTableLength);

		std::strstreambuf buf((char*)pFeatureJson, nFeatureTableLength);
		std::istream ifs(&buf);
		rapidjson::IStreamWrapper isw(ifs);
		rapidjson::Document docFeature;
		docFeature.ParseStream(isw);

		rapidjson::Value & vIS = docFeature["INSTANCES_LENGTH"];
		nInstanceSize = vIS.GetUint();
		if (nInstanceSize > 0)
		{
			unsigned int nBatchIDNum = m_3DTilesParser.GetBatchIDNum();
			m_3DTilesParser.SetBatchIDNum(nBatchIDNum + nInstanceSize);
		}
	}

	void ProcessTools::MeshToGroup(GLTFTreeNode * pNode, GLTFTileInfos_1 *& pTileInfos, std::wstring strOutputPath, RenderOperationGroup* pGroup, std::wstring strParentPath, Point3D& pntCenter)
	{
		RenderOperationPagedLOD* pLOD = new RenderOperationPagedLOD();
		pLOD->SetName(StringUtil::GetName(strParentPath));
		pGroup->AddChild(pLOD);

		std::map<std::wstring, TextureDataInfo*> textureLists;
        std::map<std::wstring, Skeleton*>& mapSkeleton = pGroup->GetSkeleton();
		std::map<std::wstring, Material*>& mapMaterial = pGroup->GetMaterials();
		std::map<std::wstring, TextureDataInfo*>& mapTexture = pGroup->GetTextureData();

		const std::map<std::wstring, std::vector<std::wstring> >& mapMeshSet = pTileInfos->m_mapMeshSet;
		std::map<std::wstring, std::vector<std::wstring> >::const_iterator it = mapMeshSet.begin();
		while (it != mapMeshSet.end())
		{
			unsigned int nMeshSize = it->second.size();
			Matrix4d mat = Matrix4d::IDENTITY;
			for (unsigned int i = 0; i < nMeshSize; i++)
			{
				TextureDataInfo* texPtr = nullptr;
				RenderOperationGeode* pGeode = new RenderOperationGeode;

				const GLTFMesh& gltfMesh = pTileInfos->m_mapMeshs[it->second[i]];
				mat = pTileInfos->m_mapMeshTOLocalView[it->second[i]];

				unsigned int nPosSize = gltfMesh.m_vecStrPos.size();

				for (unsigned int j = 0; j < nPosSize; j++)
				{
					std::wstring strPosName = gltfMesh.m_vecStrPos[j];
					//---------------- 顶点----------------------------------------------------------------------------
					const GLTFAccessor& gltfPosAccessor = pTileInfos->m_mapAccessors[strPosName];
					int nPosDim = 3;
					float* pPosData = (float*)GLTFParser::GetAttributeData(pTileInfos, strPosName, nPosDim);
					//---------------- 顶点颜色---------------------------------------------------------------------------
					float* pColorData = nullptr;
					GLTFAccessor gltfVecColor;
					int nColorDim = 3;
					if (gltfMesh.m_vecStrColor.size() > j)
					{
						std::wstring strColorName = gltfMesh.m_vecStrColor[j];
						gltfVecColor = pTileInfos->m_mapAccessors[strColorName];
						pColorData = (float*)GLTFParser::GetAttributeData(pTileInfos, strColorName, nColorDim);
					}
					//--------------- 法线----------------------------------------------------------------------------
					GLTFAccessor gltfNormalAccessor;
					int nNormalDim = 3;
					float* pNormalData = nullptr;
					if (gltfMesh.m_vecStrNormal.size() > j)
					{
						std::wstring strNormalName = gltfMesh.m_vecStrNormal[j];
						gltfNormalAccessor = pTileInfos->m_mapAccessors[strNormalName];
						pNormalData = (float*)GLTFParser::GetAttributeData(pTileInfos, strNormalName, nNormalDim);
					}

					//--------------- 纹理----------------------------------------------------------------------------
					float* pTexData = nullptr;
					GLTFAccessor gltfTexAccessor;
					int nTexDim = 2;
					if (gltfMesh.m_vecStrTex.size() > j)
					{
						std::wstring strTexName = gltfMesh.m_vecStrTex[j];
						gltfTexAccessor = pTileInfos->m_mapAccessors[strTexName];
						pTexData = (float*)GLTFParser::GetAttributeData(pTileInfos, strTexName, nTexDim);
					}
					//--------------- 材质纹理-------------------------------------------------------------------------
					if (gltfMesh.m_vecStrMaterial.size() > j)
					{
						std::wstring strMaterialName = gltfMesh.m_vecStrMaterial[j];
						std::wstring strDir = StringUtil::GetDir(strOutputPath);
						GetTextureData(pTileInfos, strMaterialName, strDir, textureLists, texPtr);
					}
					//--------------- 索引-----------------------------------------------------------------------------------
					GLTFAccessor gltfIndicesAccessor;
					unsigned int nIndicesCount = 0;
					unsigned short* pUShortIndicesData = nullptr;
					unsigned int* pUIntIndicesData = nullptr;
					int nIndicesDim = 1;
					if (gltfMesh.m_vecStrIndices.size() > j)
					{
						std::wstring strIndiceName = gltfMesh.m_vecStrIndices[j];
						gltfIndicesAccessor = pTileInfos->m_mapAccessors[strIndiceName];
						nIndicesCount = gltfIndicesAccessor.m_nCount;

						if (gltfIndicesAccessor.m_nComponentType == 5122 || gltfIndicesAccessor.m_nComponentType == 5123)
						{
							pUShortIndicesData = (unsigned short*)GLTFParser::GetAttributeData(pTileInfos, strIndiceName, nIndicesDim);
						}
						else
						{
							pUIntIndicesData = (unsigned int*)GLTFParser::GetAttributeData(pTileInfos, strIndiceName, nIndicesDim);
						}
					}

					//--------------------------材质----------------------------------------------------------------------------
					std::wstring strName = BaseUtils::CreateGuid();
					std::wstring strMaterialName = BaseUtils::CreateGuid();
                    Skeleton* pSkeleton = new Skeleton;
					pSkeleton->m_strGeoName = strName;

					Material* pMaterial = new Material;;
					if (texPtr != nullptr)
					{
						pSkeleton->m_strMaterialName = strMaterialName;
						pMaterial->m_strName = strMaterialName;

						std::vector<std::wstring> texName;
						texName.push_back(texPtr->m_strName);
						MaterialUtils::SetTextureName(pMaterial, texName);
					}
					else
					{
						MaterialUtils::MakeDefault(pMaterial);
						pSkeleton->m_strMaterialName = pMaterial->m_strName;
						strMaterialName = pSkeleton->m_strMaterialName;
					}
					VertexDataPackage* pDataPackage = new VertexDataPackage;
					if (pPosData != nullptr)
					{
						pDataPackage->m_nVerticesCount = gltfPosAccessor.m_nCount;
						pDataPackage->m_pVertices = pPosData;
						pDataPackage->m_nVertexDimension = nPosDim;

						if (!m_3DTilesParser.GetIsNEU())
						{
							m_3DTilesParser.ConvertPoint(pNode, pPosData, gltfPosAccessor.m_nCount, nPosDim, mat, pntCenter);
						}
						else
						{
							pSkeleton->m_matLocalView = mat;
						}
					}

					if (pColorData != nullptr)
					{
						unsigned int* pColor = new unsigned int[gltfVecColor.m_nCount];
						for (int i = 0; i < gltfVecColor.m_nCount; i++)
						{
							float r, g, b, a;
							r = pColorData[3 * i];
							g = pColorData[3 * i + 1];
							b = pColorData[3 * i + 2];
							a = 1;
                            pColor[i] = ColorUtils::RGBA((unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255), (unsigned char)(a * 255));

						}
						pDataPackage->m_nVertexColorCount = gltfVecColor.m_nCount;
						pDataPackage->m_pVertexColor = pColor;
					}
					if (pNormalData != nullptr)
					{
						pDataPackage->m_nNormalCount = gltfNormalAccessor.m_nCount;
						pDataPackage->m_pNormals = pNormalData;
						pDataPackage->m_nNormalDimension = nNormalDim;
					}

					if (pTexData != nullptr)
					{
						pDataPackage->m_TexCoordCount[0] = gltfTexAccessor.m_nCount;
						pDataPackage->m_pTexCoords[0] = pTexData;
						pDataPackage->m_nTexDimensions[0] = nTexDim;
					}

					IndexPackage* pIndexPackage = new IndexPackage;
					pIndexPackage->m_OperationType = GLTFParser::ToDrawOperationType(gltfMesh.m_vecDrawType[j]);
					pIndexPackage->m_strPassName.push_back(strMaterialName);
					pIndexPackage->m_nIndexesCount = nIndicesCount;
					if (pUShortIndicesData != nullptr)
					{
						pIndexPackage->m_enIndexType = IT_16BIT;
						pIndexPackage->m_pIndexes = pUShortIndicesData;
					}
					else if (pUIntIndicesData != nullptr)
					{
						pIndexPackage->m_enIndexType = IT_32BIT;
						pIndexPackage->m_pIndexes = (unsigned short *)pUIntIndicesData;
					}

					// 根据骨架做拆分
					SplitSkeletonByIndex(pDataPackage, pIndexPackage);

					pSkeleton->m_pVertexDataPackage = pDataPackage;
					pSkeleton->m_arrIndexPackage.push_back(pIndexPackage);

					mapSkeleton[pSkeleton->m_strGeoName] = pSkeleton;
					mapMaterial[pMaterial->m_strName] = pMaterial;
					mapTexture[texPtr->m_strName] = texPtr;
				}

				pGeode->SetPosition(Matrix4d());
				pLOD->AddChild(pGeode);
			}
			it++;
		}

		pLOD->ReComputeBoundingBox();
		BoundingSphere bSphere = pLOD->GetBoundingSphere();
        if (false)
		{
			pLOD->SetRangeMode(Distance_From_EyePoint);
			pLOD->GetRanges().push_back(std::vector<std::pair<float, float> >::value_type(float(9999.0), pNode->m_dGeometryError));
		}
		else
		{
			pLOD->SetRangeMode(Pixel_Size_OnScreen);
			if (pNode->m_dGeometryError <= 0)
			{
				pNode->m_dGeometryError = 0.1;
			}
			double dLodRangeValue = bSphere.m_radius / (pNode->m_dGeometryError / 16.0);
			pLOD->GetRanges().push_back(std::vector<std::pair<float, float> >::value_type(float(0.0), dLodRangeValue));
		}

		{
			// 设置子节点
			std::wstring strS3MBFilePath = StringUtil::ChangeExt(strOutputPath, FILE_EXT_S3MB);
			std::wstring strS3MBFileName = StringUtil::GetName(strS3MBFilePath);
			std::wstring strS3MBFileDir = StringUtil::GetDir(strS3MBFilePath);

			std::wstring strRelativeDir = StringUtil::GetDir(strParentPath);
			strRelativeDir = StringUtil::GetRelativePath(strRelativeDir, strS3MBFileDir);
			std::vector<wstring>& vecFileName = pLOD->GetFileNames();
			vecFileName.push_back(strRelativeDir + strS3MBFileName);
		}
	}

	void ProcessTools::GetTextureData(GLTFTileInfos_1 *& pTileInfos, std::wstring& strMaterialName, std::wstring& strOutputDir, std::map<std::wstring, TextureDataInfo*>& texList, TextureDataInfo*& texPtr)
	{
		if (strMaterialName.length() == 0)
		{
			return;
		}

		const GLTFMaterial& gltfMaterial = pTileInfos->m_mapMaterials[strMaterialName];
		if (gltfMaterial.m_strTextureName.length() == 0)
		{
			return;
		}

		const GLTFTexture& gltfTexture = pTileInfos->m_mapTextures[gltfMaterial.m_strTextureName];
		if (gltfTexture.m_strImageName.length() == 0)
		{
			return;
		}

		const GLTFImage& gltfImage = pTileInfos->m_mapImages[gltfTexture.m_strImageName];
		if (gltfImage.m_strBufferViewName.length() == 0)
		{
			return;
		}

		if (texList.size() > 0)
		{
			std::map<std::wstring, TextureDataInfo*>::iterator it = texList.find(gltfImage.m_strBufferViewName);
			if (it != texList.end())
			{
				texPtr = it->second;
				return;
			}
		}

		const GLTFBufferView & gltfBufferView = pTileInfos->m_mapBufferViews[gltfImage.m_strBufferViewName];

		unsigned int nBufferSize = gltfBufferView.m_nByteLength;
		unsigned char* pData = new unsigned char[nBufferSize];
		memcpy(pData, pTileInfos->m_mapBuffers[gltfBufferView.m_strBufferName].m_pBuffer + gltfBufferView.m_nByteOffset, nBufferSize);

		std::wstring strImageType = gltfImage.m_strImageType;
		int nTypeLength = strImageType.length();
		std::wstring strTexExt;
		if (strImageType.find(U("jpg")) < nTypeLength ||
			strImageType.find(U("jpeg")) < nTypeLength ||
			strImageType.find(U("JPG")) < nTypeLength ||
			strImageType.find(U("JPEG")) < nTypeLength)
		{
			strTexExt = U(".jpg");
		}
		else if (strImageType.find(U("png")) < nTypeLength ||
			strImageType.find(U("PNG")) < nTypeLength)
		{
			strTexExt = U(".png");
		}
		else
		{
			std::vector<std::wstring> strDest;
			StringUtil::Split(strImageType, strDest, '/');
			if (strDest.size() == 2)
			{
				strTexExt = U(".") + strDest[1];
			}
			strDest.clear();
		}

		std::wstring strTexName = U("Test") + strTexExt;
		std::wstring strTexFile = StringUtil::GetAbsolutePath(strOutputDir, strTexName);

		std::string strTempTexFile = StringUtil::UNICODE_to_UTF8(strTexFile);
		strTempTexFile = StringUtil::UTF8_to_ANSI(strTempTexFile);

		FILE* file = fopen(strTempTexFile.c_str(), "wb");
		fwrite(pData, sizeof(unsigned char), nBufferSize, file);
		fclose(file);

		TextureData* pTextrueData = ImgToolkit::GetTextureData(strTexFile, false);
		pTextrueData->m_nSize = pTextrueData->m_nWidth * pTextrueData->m_nHeight;

		remove(strTempTexFile.c_str());

		TextureDataInfo* pTexDataInfo = new TextureDataInfo;
		pTexDataInfo->m_strName = BaseUtils::CreateGuid();
		pTexDataInfo->m_pTextureData = pTextrueData;
		TextureUtils::UpdateTextureTier(pTexDataInfo, TC_DXT5);
		texList[gltfImage.m_strBufferViewName] = pTexDataInfo;

		delete pData;
		pData = nullptr;
		return;
	}

	void ProcessTools::MeshToGroup(GLTFTreeNode* pNode, GLTFTileInfos_2*& pTileInfos, std::wstring strOutputPath, RenderOperationGroup* pGroup, std::wstring strParentPath, Point3D& pntCenter)
	{
		std::map<int, TextureDataInfo*> textureLists;
        std::map<std::wstring, Skeleton*>& mapSkeleton = pGroup->GetSkeleton();
		std::map<std::wstring, Material*>& mapMaterial = pGroup->GetMaterials();
		std::map<std::wstring, TextureDataInfo*>& mapTexture = pGroup->GetTextureData();

		RenderOperationPagedLOD* pLOD = nullptr;
		if (StringUtil::GetExt(strOutputPath) == U(".cmpt") && pGroup->GetNumChildren() > 0)
		{
			pLOD = (RenderOperationPagedLOD*)(pGroup->GetChild(0));
		}
		else
		{
			pLOD = new RenderOperationPagedLOD();
			pGroup->AddChild(pLOD);
		}

		pLOD->SetName(StringUtil::GetName(strParentPath));

		BoundingBox entileBox;
		const std::map<unsigned int, std::vector<unsigned int> >& mapMeshSet = pTileInfos->m_mapMeshSet;
		std::map<unsigned int, std::vector<unsigned int> >::const_iterator it = mapMeshSet.begin();
		while (it != mapMeshSet.end())
		{
			unsigned int nMeshSize = it->second.size();
			for (unsigned int i = 0; i < nMeshSize; i++)
			{
				RenderOperationGeode* pGeode = new RenderOperationGeode;
				if (!MeshToGeode(pNode, pTileInfos, it->second[i], strOutputPath, pGeode, mapSkeleton, mapMaterial, mapTexture, textureLists, entileBox, pntCenter))
				{
					delete pGeode;
					pGeode = nullptr;
					continue;
				}

				pLOD->AddChild(pGeode);
			}
			it++;
		}

		BoundingSphere bSphere;
		OrientedBoundingBox obb;
        if (m_3DTilesParams.GetVertexCompressType() != SV_DracoCompressed)
		{
			BoundingBox boxLOD = pLOD->GetBoundingBox();
			if (boxLOD.IsNULL())
			{
				pLOD->ReComputeBoundingBox(false);
				boxLOD = pLOD->GetBoundingBox();
			}
			entileBox.Merge(boxLOD);
			bSphere = BoundingSphere(entileBox);
			pLOD->SetBoundingSphere(bSphere.GetCenter(), bSphere.GetRadius());
			bSphere = pLOD->GetBoundingSphere();
			pLOD->SetBoundingBox(entileBox);
			pLOD->GetBoundingBox().ToOrientedBoundingBox(obb);
			pLOD->SetOrientedBoundingBox(obb);
		}
		else
		{
			bSphere = pNode->m_boundSphere;
			pLOD->SetBoundingSphere(bSphere.GetCenter(), bSphere.GetRadius());
			bSphere.GetBoundingBox().ToOrientedBoundingBox(obb);
			pLOD->SetOrientedBoundingBox(obb);
		}

		pLOD->SetRangeMode(Geometric_Error);
		pLOD->GetRanges().push_back(std::vector<std::pair<float, float> >::value_type(float(9999.0), pNode->m_dGeometryError));

		std::vector<wstring>& vecFileName = pLOD->GetFileNames();
		if (vecFileName.empty())
		{
			// 设置子节点
			std::wstring strS3MBFilePath = StringUtil::ChangeExt(strOutputPath, FILE_EXT_S3MB);
			std::wstring strS3MBFileName = StringUtil::GetName(strS3MBFilePath);
			std::wstring strS3MBFileDir = StringUtil::GetDir(strS3MBFilePath);

			std::wstring strRelativeDir = StringUtil::GetDir(strParentPath);
			strRelativeDir = StringUtil::GetRelativePath(strRelativeDir, strS3MBFileDir);
			vecFileName.push_back(strRelativeDir + strS3MBFileName);
		}
	}

    bool ProcessTools::MeshToGeode(GLTFTreeNode* pNode, GLTFTileInfos_2*& pTileInfos, unsigned int nMeshIndex, std::wstring strOutputPath, RenderOperationGeode* pGeode, std::map<std::wstring, Skeleton*>& mapSkeleton, std::map<std::wstring, Material*>& mapMaterial, std::map<std::wstring, TextureDataInfo*>& mapTexture, std::map<int, TextureDataInfo*>& textureLists, BoundingBox& entileBox, Point3D& pntCenter)
	{
		const GLTFMesh& gltfMesh = pTileInfos->m_mapMeshs[nMeshIndex];
		Matrix4d mat = pTileInfos->m_mapMeshToLocalView[nMeshIndex];

		for (unsigned int i = 0; i < gltfMesh.m_vecGLTFPrimitive.size(); i++)
		{
			GLTFPrimitive gltfPrimitive = gltfMesh.m_vecGLTFPrimitive[i];
			if (gltfPrimitive.m_nDrawType < 4)
			{
				continue;
			}

            Skeleton* pSkeleton = new Skeleton;
			Material* pMaterial = new Material;
			std::vector<TextureDataInfo*> vecTexPtr;
			ProcessMaterial(pTileInfos, StringUtil::GetDir(strOutputPath), gltfPrimitive, pMaterial, textureLists, vecTexPtr);

			std::wstring strMaterialName = pMaterial->m_strName;
			if (!ProcessSkeleton(pNode, pTileInfos, pSkeleton, gltfPrimitive, mat, strMaterialName, pntCenter))
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
			entileBox.Merge(boxSke);
			for (unsigned int j = 0; j < vecTexPtr.size(); j++)
			{
				TextureDataInfo* texPtr = vecTexPtr[j];
				mapTexture[texPtr->m_strName] = texPtr;
			}

            pGeode->AddSkeleton(pSkeleton);
		}

		if (mapSkeleton.size() == 0 && mapMaterial.size() == 0 && mapTexture.size() == 0)
		{
			return false;
		}

		pGeode->SetPosition(Matrix4d::IDENTITY);

		return true;
	}

    bool ProcessTools::ProcessSkeleton(GLTFTreeNode* pNode, GLTFTileInfos_2*& pTileInfos, Skeleton*& pSkeleton, GLTFPrimitive& gltfPrimitive, Matrix4d mat, std::wstring strMaterialName, Point3D& pntCenter)
	{
		GLTFAttributes& gltfAttributes = gltfPrimitive.m_gltfAttributes;
		GLTFDraco& gltfDraco = gltfPrimitive.m_gltfDraco;

		pSkeleton->m_strMaterialName = strMaterialName;
		pSkeleton->m_strGeoName = BaseUtils::CreateGuid();

		// 骨架数据没有做Draco压缩
		if (gltfDraco.m_nBufferViewIndex == -1)
		{
			const GLTFAccessor& gltfPosAccessor = pTileInfos->m_mapAccessors[gltfAttributes.m_nPosIndex];
			int nPosDim = 3;
			float* pPosData = (float*)GLTFParser::GetAttributeData(pTileInfos, gltfAttributes.m_nPosIndex, nPosDim);
			//----------------------- 顶点颜色-------------------------------------------------------------
			float* pColorData = nullptr;
			GLTFAccessor gltfVecColor;
			int nColorDim = 3;
			if (gltfAttributes.m_nColorIndex != -1)
			{
				gltfVecColor = pTileInfos->m_mapAccessors[gltfAttributes.m_nColorIndex];
				pColorData = (float*)GLTFParser::GetAttributeData(pTileInfos, gltfAttributes.m_nColorIndex, nColorDim);
			}
			//----------------------- 法线----------------------------------------------------------------
			float* pNormalData = nullptr;
			GLTFAccessor gltfNormalAccessor;
			int nNormalDim = 3;
			if (gltfAttributes.m_nNormalIndex != -1)
			{
				gltfNormalAccessor = pTileInfos->m_mapAccessors[gltfAttributes.m_nNormalIndex];
				pNormalData = (float*)GLTFParser::GetAttributeData(pTileInfos, gltfAttributes.m_nNormalIndex, nNormalDim);
			}
			//------------------------ 纹理坐标 ---------------------------------------------------------------------
			float* pTexData = nullptr;
			GLTFAccessor gltfTexAccessor;
			int nTexDim = 2;
			if (gltfAttributes.m_nTex1Index != -1)
			{
				gltfTexAccessor = pTileInfos->m_mapAccessors[gltfAttributes.m_nTex1Index];
				pTexData = (float*)GLTFParser::GetAttributeData(pTileInfos, gltfAttributes.m_nTex1Index, nTexDim);
			}
			//----------------------- 第二重纹理坐标----------------------------------------------------------------
			float* pTexData2 = nullptr;
			GLTFAccessor gltfTexAccessor2;
			int nTexDim2 = 2;
			if (gltfAttributes.m_nTex2Index != -1)
			{
				gltfTexAccessor2 = pTileInfos->m_mapAccessors[gltfAttributes.m_nTex2Index];
				pTexData2 = (float*)GLTFParser::GetAttributeData(pTileInfos, gltfAttributes.m_nTex2Index, nTexDim2);
			}
			//-----------------------BatchId--------------------------------------------------
			GLTFAccessor gltfBatchIdsAccessor;
			unsigned int nBatchIdsCount = 0;
			unsigned short* pUShortBatchIdsData = nullptr;
			unsigned int* pUIntBatchIdsData = nullptr;
			float* pfloatBatchIdData = nullptr;
			unsigned char* pByteBatchIdData = nullptr;
			int nBatchIdsDim = 1;
			if (m_3DTilesParams.GetIsModel() && gltfAttributes.m_nBatchIds != -1)
			{
				gltfBatchIdsAccessor = pTileInfos->m_mapAccessors[gltfAttributes.m_nBatchIds];
				nBatchIdsCount = gltfBatchIdsAccessor.m_nCount;

				if (gltfBatchIdsAccessor.m_nComponentType == 5122 || gltfBatchIdsAccessor.m_nComponentType == 5123)
				{
					pUShortBatchIdsData = (unsigned short*)GLTFParser::GetAttributeData(pTileInfos, gltfAttributes.m_nBatchIds, nBatchIdsDim);
				}
				else if (gltfBatchIdsAccessor.m_nComponentType == 5125)
				{
					pUIntBatchIdsData = (unsigned int*)GLTFParser::GetAttributeData(pTileInfos, gltfAttributes.m_nBatchIds, nBatchIdsDim);
				}
				else if (gltfBatchIdsAccessor.m_nComponentType == 5120 || gltfBatchIdsAccessor.m_nComponentType == 5121)
				{
					pByteBatchIdData = (unsigned char*)GLTFParser::GetAttributeData(pTileInfos, gltfAttributes.m_nBatchIds, nBatchIdsDim);
				}
				else if (gltfBatchIdsAccessor.m_nComponentType == 5126)
				{
					pfloatBatchIdData = (float*)GLTFParser::GetAttributeData(pTileInfos, gltfAttributes.m_nBatchIds, nBatchIdsDim);
				}
			}
			//----------------------- 索引---------------------------------------------------------
			GLTFAccessor gltfIndicesAccessor;
			unsigned int nIndicesCount = 0;
			unsigned short* pUShortIndicesData = nullptr;
			unsigned int* pUIntIndicesData = nullptr;
			int nIndicesDim = 1;
			if (gltfPrimitive.m_nIndices != -1)
			{
				gltfIndicesAccessor = pTileInfos->m_mapAccessors[gltfPrimitive.m_nIndices];
				nIndicesCount = gltfIndicesAccessor.m_nCount;

				if (gltfIndicesAccessor.m_nComponentType == 5122 || gltfIndicesAccessor.m_nComponentType == 5123)
				{
					pUShortIndicesData = (unsigned short*)GLTFParser::GetAttributeData(pTileInfos, gltfPrimitive.m_nIndices, nIndicesDim);
				}
				else if (gltfIndicesAccessor.m_nComponentType == 5120 || gltfIndicesAccessor.m_nComponentType == 5121)
				{
					unsigned char* pUByteIndicesData = (unsigned char*)GLTFParser::GetAttributeData(pTileInfos, gltfPrimitive.m_nIndices, nIndicesDim);
					pUShortIndicesData = new unsigned short[nIndicesCount];
					for (int iIndexIndx = 0; iIndexIndx < nIndicesCount; iIndexIndx++)
					{
						pUShortIndicesData[iIndexIndx] = (unsigned short)pUByteIndicesData[iIndexIndx];
					}
					delete pUByteIndicesData; pUByteIndicesData = nullptr;
				}
				else
				{
					pUIntIndicesData = (unsigned int*)GLTFParser::GetAttributeData(pTileInfos, gltfPrimitive.m_nIndices, nIndicesDim);
				}
				//判断下顶点个数是否小于65536，小于就再转下
				if (pUIntIndicesData != nullptr && gltfPosAccessor.m_nCount < 65536)
				{
					pUShortIndicesData = new unsigned short[nIndicesCount];
					for (int iIndexIndx = 0; iIndexIndx < nIndicesCount; iIndexIndx++)
					{
						pUShortIndicesData[iIndexIndx] = (unsigned short)pUIntIndicesData[iIndexIndx];
					}
					delete pUIntIndicesData; pUIntIndicesData = nullptr;
				}
			}

			VertexDataPackage* pDataPackage = new VertexDataPackage;
			if (pPosData != nullptr)
			{
				pDataPackage->m_nVerticesCount = gltfPosAccessor.m_nCount;
				pDataPackage->m_pVertices = pPosData;
				pDataPackage->m_nVertexDimension = nPosDim;
				m_3DTilesParser.ConvertPoint(pNode, pPosData, gltfPosAccessor.m_nCount, nPosDim, mat, pntCenter);
			}

			if (pColorData != nullptr)
			{
				unsigned int* pColor = new unsigned int[gltfVecColor.m_nCount];
				bool bIsTrans = pTileInfos->m_mapAccessors[gltfAttributes.m_nColorIndex].m_strType == U("VEC4");
				for (int i = 0; i < gltfVecColor.m_nCount; i++)
				{
					float r, g, b, a;
					r = pColorData[nColorDim * i];
					g = pColorData[nColorDim * i + 1];
					b = pColorData[nColorDim * i + 2];
					a = bIsTrans ? pColorData[nColorDim * i + 3] : 1;
                    pColor[i] = ColorUtils::RGBA((unsigned char)(r * 255), (unsigned char)(g * 255), (unsigned char)(b * 255), (unsigned char)(a * 255));

				}
				if (pColorData != nullptr)
				{
					delete[] pColorData;
					pColorData = nullptr;
				}
				pDataPackage->m_nVertexColorCount = gltfVecColor.m_nCount;
				pDataPackage->m_pVertexColor = pColor;
			}
			if (pNormalData != nullptr)
			{
				pDataPackage->m_nNormalCount = gltfNormalAccessor.m_nCount;
				pDataPackage->m_pNormals = pNormalData;
				pDataPackage->m_nNormalDimension = nNormalDim;
			}

			if (pTexData != nullptr)
			{
				pDataPackage->m_TexCoordCount[0] = gltfTexAccessor.m_nCount;
				pDataPackage->m_pTexCoords[0] = pTexData;
				pDataPackage->m_nTexDimensions[0] = nTexDim;
			}

			if (pTexData2 != nullptr)
			{
				pDataPackage->m_TexCoordCount[1] = gltfTexAccessor2.m_nCount;
				pDataPackage->m_pTexCoords[1] = pTexData2;
				pDataPackage->m_nTexDimensions[1] = nTexDim2;
			}

			std::map<unsigned int, std::vector<BatchIDInfo*> > mapBatchInfo;
			if (pUIntBatchIdsData != nullptr)
			{
				unsigned int nID = pUIntBatchIdsData[0] + pNode->m_pairIDRange.first;
				BatchIDInfo* pIDInfo = new BatchIDInfo;
				pIDInfo->m_nCount = 0;
				pIDInfo->m_nOffset = 0;

				std::vector<BatchIDInfo*> vecIDInfo;
				vecIDInfo.push_back(pIDInfo);
				mapBatchInfo[nID] = vecIDInfo;

				for (int i = 0; i < gltfBatchIdsAccessor.m_nCount; i++)
				{
					unsigned int nCurrentID = pUIntBatchIdsData[i] + pNode->m_pairIDRange.first;
					if (nCurrentID == nID)
					{
						pIDInfo->m_nCount++;
					}
					else
					{
						pIDInfo = new BatchIDInfo;
						pIDInfo->m_nOffset = i;
						pIDInfo->m_nCount = 1;

						if (mapBatchInfo.find(nCurrentID) == mapBatchInfo.end())
						{
							std::vector<BatchIDInfo*> vecCurrentIDInfo;
							vecCurrentIDInfo.push_back(pIDInfo);
							mapBatchInfo[nCurrentID] = vecCurrentIDInfo;
						}
						else
						{
							mapBatchInfo[nCurrentID].push_back(pIDInfo);
						}
						nID = nCurrentID;
					}
				}
				delete pUIntBatchIdsData;
				pUIntBatchIdsData = nullptr;
			}
			else if (pUShortBatchIdsData != nullptr)
			{
				unsigned int nID = pUShortBatchIdsData[0] + pNode->m_pairIDRange.first;
				BatchIDInfo* pIDInfo = new BatchIDInfo;
				pIDInfo->m_nCount = 0;
				pIDInfo->m_nOffset = 0;

				std::vector<BatchIDInfo*> vecIDInfo;
				vecIDInfo.push_back(pIDInfo);
				mapBatchInfo[nID] = vecIDInfo;

				for (int i = 0; i < gltfBatchIdsAccessor.m_nCount; i++)
				{
					unsigned int nCurrentID = pUShortBatchIdsData[i] + pNode->m_pairIDRange.first;
					if (nCurrentID == nID)
					{
						pIDInfo->m_nCount++;
					}
					else
					{
						pIDInfo = new BatchIDInfo;
						pIDInfo->m_nOffset = i;
						pIDInfo->m_nCount = 1;

						if (mapBatchInfo.find(nCurrentID) == mapBatchInfo.end())
						{
							std::vector<BatchIDInfo*> vecCurrentIDInfo;
							vecCurrentIDInfo.push_back(pIDInfo);
							mapBatchInfo[nCurrentID] = vecCurrentIDInfo;
						}
						else
						{
							mapBatchInfo[nCurrentID].push_back(pIDInfo);
						}
						nID = nCurrentID;
					}
				}

				delete pUShortBatchIdsData;
				pUShortBatchIdsData = nullptr;
			}
			else if (pByteBatchIdData != nullptr)
			{
				unsigned int nID = pByteBatchIdData[0] + pNode->m_pairIDRange.first;
				BatchIDInfo* pIDInfo = new BatchIDInfo;
				pIDInfo->m_nCount = 0;
				pIDInfo->m_nOffset = 0;

				std::vector<BatchIDInfo*> vecIDInfo;
				vecIDInfo.push_back(pIDInfo);
				mapBatchInfo[nID] = vecIDInfo;

				for (int i = 0; i < gltfBatchIdsAccessor.m_nCount; i++)
				{
					unsigned int nCurrentID = pByteBatchIdData[i] + pNode->m_pairIDRange.first;
					if (nCurrentID == nID)
					{
						pIDInfo->m_nCount++;
					}
					else
					{
						pIDInfo = new BatchIDInfo;
						pIDInfo->m_nOffset = i;
						pIDInfo->m_nCount = 1;

						if (mapBatchInfo.find(nCurrentID) == mapBatchInfo.end())
						{
							std::vector<BatchIDInfo*> vecCurrentIDInfo;
							vecCurrentIDInfo.push_back(pIDInfo);
							mapBatchInfo[nCurrentID] = vecCurrentIDInfo;
						}
						else
						{
							mapBatchInfo[nCurrentID].push_back(pIDInfo);
						}
						nID = nCurrentID;
					}
				}

				delete pByteBatchIdData;
				pByteBatchIdData = nullptr;
			}
			else if (pfloatBatchIdData != nullptr)
			{
				unsigned int nID = pfloatBatchIdData[0] + pNode->m_pairIDRange.first;
				BatchIDInfo* pIDInfo = new BatchIDInfo;
				pIDInfo->m_nCount = 0;
				pIDInfo->m_nOffset = 0;

				std::vector<BatchIDInfo*> vecIDInfo;
				vecIDInfo.push_back(pIDInfo);
				mapBatchInfo[nID] = vecIDInfo;

				for (int i = 0; i < gltfBatchIdsAccessor.m_nCount; i++)
				{
					unsigned int nCurrentID = pfloatBatchIdData[i] + pNode->m_pairIDRange.first;
					if (nCurrentID == nID)
					{
						pIDInfo->m_nCount++;
					}
					else
					{
						pIDInfo = new BatchIDInfo;
						pIDInfo->m_nOffset = i;
						pIDInfo->m_nCount = 1;

						if (mapBatchInfo.find(nCurrentID) == mapBatchInfo.end())
						{
							std::vector<BatchIDInfo*> vecCurrentIDInfo;
							vecCurrentIDInfo.push_back(pIDInfo);
							mapBatchInfo[nCurrentID] = vecCurrentIDInfo;
						}
						else
						{
							mapBatchInfo[nCurrentID].push_back(pIDInfo);
						}
						nID = nCurrentID;
					}
				}

				delete pfloatBatchIdData;
				pfloatBatchIdData = nullptr;
			}
			IndexPackage* pIndexPackage = new IndexPackage;
			pIndexPackage->m_OperationType = GLTFParser::ToDrawOperationType(gltfPrimitive.m_nDrawType);
			pIndexPackage->m_strPassName.push_back(strMaterialName);
			pIndexPackage->m_nIndexesCount = nIndicesCount;
			if (pUShortIndicesData != nullptr)
			{
				pIndexPackage->m_enIndexType = IT_16BIT;
				pIndexPackage->m_pIndexes = pUShortIndicesData;
			}
			else if (pUIntIndicesData != nullptr)
			{
				pIndexPackage->m_enIndexType = IT_32BIT;
				pIndexPackage->m_pIndexes = (unsigned short*)pUIntIndicesData;
			}
			else
			{//当没有索引时添加索引
				if (pDataPackage->m_nVerticesCount < 65536)
				{
					pIndexPackage->m_enIndexType = IT_16BIT;
					pIndexPackage->SetIndexNum(pDataPackage->m_nVerticesCount);
					for (int i = 0; i < pDataPackage->m_nVerticesCount; i++)
					{
						pIndexPackage->m_pIndexes[i] = (unsigned short)i;
					}
				}
				else
				{
					pIndexPackage->m_enIndexType = IT_32BIT;
					pIndexPackage->SetIndexNum(pDataPackage->m_nVerticesCount);
					unsigned int* pIndex = (unsigned int*)pIndexPackage->m_pIndexes;
					for (unsigned int i = 0; i < pDataPackage->m_nVerticesCount; i++)
					{
						pIndex[i] = i;
					}
				}
			}

			if (pIndexPackage->m_OperationType == OT_TRIANGLE_STRIP || pIndexPackage->m_OperationType == OT_TRIANGLE_FAN)
			{
				SkeletonUtils::TranslateIndices(pIndexPackage);
			}

			unsigned int nErrorID = 0xffffffff;
			if (mapBatchInfo.find(nErrorID) != mapBatchInfo.end())
			{
				std::vector<BatchIDInfo*>& vecInfo = mapBatchInfo[nErrorID];
				for (unsigned int i = 0; i < vecInfo.size(); i++)
				{
					delete vecInfo[i];
					vecInfo[i] = nullptr;
				}
				mapBatchInfo.erase(nErrorID);
			}

			std::vector<IDInfo*> vecIDInfo;
			std::map<unsigned int, std::vector<BatchIDInfo*> >::iterator itor;
			for (itor = mapBatchInfo.begin(); itor != mapBatchInfo.end(); itor++)
			{
				IDInfo* pIDInfo = new IDInfo;
				pIDInfo->m_nID = itor->first;
				std::vector<std::pair<int, int> >& vecOffsetAndCount = pIDInfo->m_arrVertexColorOffsetAndCount;
				std::vector<BatchIDInfo*>& vecInfo = itor->second;
				for (unsigned int i = 0; i < vecInfo.size(); i++)
				{
					vecOffsetAndCount.push_back(std::make_pair(vecInfo[i]->m_nOffset, vecInfo[i]->m_nCount));
					delete vecInfo[i];
					vecInfo[i] = nullptr;
				}
				vecInfo.clear();
				vecIDInfo.push_back(pIDInfo);
			}
			mapBatchInfo.clear();

			std::vector<IndexPackage*> arrIndexPackage;
			arrIndexPackage.push_back(pIndexPackage);
			SkeletonUtils::RebuildVertexAndIndexPackage(pDataPackage, arrIndexPackage, vecIDInfo);
			pSkeleton->m_pVertexDataPackage = pDataPackage;
			pSkeleton->m_arrIndexPackage = arrIndexPackage;
			pSkeleton->m_arrIDInfo = vecIDInfo;
			I3DMIDInfo i3dmInfo = m_3DTilesParser.GetI3DMInfo();
			if (!i3dmInfo.m_vecMat.empty() && !i3dmInfo.m_vecId.empty())
			{
				pSkeleton->SetInstanceInfoAndIDInfo(i3dmInfo.m_vecMat, i3dmInfo.m_vecId);
				pSkeleton->ComputerBoundingBox();
			}
		}
		// 骨架数据做Draco压缩
		else
		{
			//ToDo:
		}
		return true;
	}

	void ProcessTools::ProcessMaterial(GLTFTileInfos_2*& pTileInfos, std::wstring strOutputPath, GLTFPrimitive& gltfPrimitive, Material* pMaterial, std::map<int, TextureDataInfo*>& textureLists, std::vector<TextureDataInfo*>& vecTexPtr)
	{
		TextureDataInfo* texPtr = nullptr;
		TextureDataInfo* texPtr2 = nullptr;
		if (m_3DTilesParams.GetIsModel())
		{//模型才使用pbr
			int nMatIndex = gltfPrimitive.m_nMaterialIndex;
			const std::vector<GLTFMaterial>& vecMaterial = pTileInfos->m_mapMaterials[nMatIndex];
			if (!vecMaterial.empty())
			{
				GLTFMaterial gltfMat = vecMaterial[0];
				MaterialUtils::MakeDefault(pMaterial);
#ifdef WIN32
				pMaterial->m_strName = StringUtil::Format(U("mtl_%u_"), nMatIndex);
#else
                pMaterial->m_strName = StringUtil::Format(U("mtl_%u_").c_str(), nMatIndex);
#endif
				pMaterial->m_strName += gltfMat.m_strName;
				Technique* pTech = pMaterial->getTechnique(0);
				Pass* pPass = pTech->getPass(0);
				pPass->m_strName = pMaterial->m_strName;
				PBRMetallicRough* pPbrParam = new PBRMetallicRough();
				pPbrParam->m_vec4BaseColor = gltfMat.m_PBR.m_vec4BaseColor;
				pPbrParam->m_vec3EmissiveFactor = gltfMat.m_vec3EmissiveFactor;
				pPbrParam->m_fMetallicFactor = gltfMat.m_PBR.m_fMetallicFactor;
				pPbrParam->m_fRoughnessFactor = gltfMat.m_PBR.m_fRoughnessFactor;
				pPass->m_AlphaMode = gltfMat.m_AlphaMode;
				pPass->m_fAlphaCutoff = gltfMat.m_fAlphaCutoff;

				if (gltfMat.m_PBR.m_nBaseColorTextureIndex > -1)
				{
					GetTextureData(pTileInfos, gltfMat.m_PBR.m_nBaseColorTextureIndex, strOutputPath, textureLists, texPtr);
					if (texPtr != nullptr)
					{
						pPbrParam->m_nBaseColorTextureCoordIndex = gltfMat.m_PBR.m_nBaseColorTextureCoordIndex;
						pPbrParam->m_nBaseColorTextureIndex = vecTexPtr.size();
						TextureUnitState* texUS = pPass->CreateTextureUnitState();
						texUS->m_strTextureName = texPtr->m_strName;
						vecTexPtr.push_back(texPtr);
					}
				}
				if (gltfMat.m_PBR.m_nMetallicRoughnessTextureIndex > -1)
				{
					GetTextureData(pTileInfos, gltfMat.m_PBR.m_nMetallicRoughnessTextureIndex, strOutputPath, textureLists, texPtr);
					if (texPtr != nullptr)
					{
						pPbrParam->m_nMetallicRoughnessTextureCoordIndex = gltfMat.m_PBR.m_nMetallicRoughnessTextureCoordIndex;
						pPbrParam->m_nMetallicRoughnessTextureIndex = vecTexPtr.size();
						TextureUnitState* texUS = pPass->CreateTextureUnitState();
						texUS->m_strTextureName = texPtr->m_strName;
						vecTexPtr.push_back(texPtr);
					}
				}
				if (gltfMat.m_nNormalTextureIndex > -1)
				{
					GetTextureData(pTileInfos, gltfMat.m_nNormalTextureIndex, strOutputPath, textureLists, texPtr);
					if (texPtr != nullptr)
					{
						pPbrParam->m_nNormalTextureCoordIndex = gltfMat.m_nNormalTextureCoordIndex;
						pPbrParam->m_nNormalTextureIndex = vecTexPtr.size();
						TextureUnitState* texUS = pPass->CreateTextureUnitState();
						texUS->m_strTextureName = texPtr->m_strName;
						vecTexPtr.push_back(texPtr);

						pPbrParam->m_fNormalTextureScale = gltfMat.m_fNormalTextureScale;
					}
				}
				if (gltfMat.m_nOcclusionTextureIndex > -1)
				{
					GetTextureData(pTileInfos, gltfMat.m_nOcclusionTextureIndex, strOutputPath, textureLists, texPtr);
					if (texPtr != nullptr)
					{
						pPbrParam->m_nOcclusionTextureCoordIndex = gltfMat.m_nOcclusionTextureCoordIndex;
						pPbrParam->m_nOcclusionTextureIndex = vecTexPtr.size();
						TextureUnitState* texUS = pPass->CreateTextureUnitState();
						texUS->m_strTextureName = texPtr->m_strName;
						vecTexPtr.push_back(texPtr);
					}
				}
				if (gltfMat.m_nEmissiveTextureIndex > -1)
				{
					GetTextureData(pTileInfos, gltfMat.m_PBR.m_nBaseColorTextureIndex, strOutputPath, textureLists, texPtr);
					if (texPtr != nullptr)
					{
						pPbrParam->m_nEmissiveTextureCoordIndex = gltfMat.m_nEmissiveTextureCoordIndex;
						pPbrParam->m_nEmissiveTextureIndex = vecTexPtr.size();
						TextureUnitState* texUS = pPass->CreateTextureUnitState();
						texUS->m_strTextureName = texPtr->m_strName;
						vecTexPtr.push_back(texPtr);
					}
				}
				pPass->m_pPRBMaterial = pPbrParam;
				return;
			}
		}

		//----------------------- 材质纹理----------------------------------------------------------------
		if (gltfPrimitive.m_nMaterialIndex != -1)
		{
			const std::vector<GLTFMaterial>& vecMaterial = pTileInfos->m_mapMaterials[gltfPrimitive.m_nMaterialIndex];
			if (vecMaterial.size() > 0)
			{//第一重纹理
				GetTextureData(pTileInfos, vecMaterial[0].m_PBR.m_nBaseColorTextureIndex, strOutputPath, textureLists, texPtr);
			}
			if (vecMaterial.size() > 1)
			{//第二重纹理
				GetTextureData(pTileInfos, vecMaterial[1].m_PBR.m_nBaseColorTextureIndex, strOutputPath, textureLists, texPtr2);
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

		const std::vector<GLTFMaterial>& vecMaterial = pTileInfos->m_mapMaterials[gltfPrimitive.m_nMaterialIndex];
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
	}

	void ProcessTools::GetTextureData(GLTFTileInfos_2*& pTileInfos, int nTexIndex, std::wstring& strOutputDir, std::map<int, TextureDataInfo*>& texList, TextureDataInfo*& texPtr)
	{
		if (nTexIndex == -1 || nTexIndex >= pTileInfos->m_mapTextures.size())
		{
			return;
		}

		const GLTFTexture& gltfTexture = pTileInfos->m_mapTextures[nTexIndex];
		if (gltfTexture.m_nImageIndex == -1)
		{
			return;
		}

		bool bTo2N = gltfTexture.m_GLTFSampler.m_nWrapS == 10497 || gltfTexture.m_GLTFSampler.m_nWrapT == 10497;

		const GLTFImage& gltfImage = pTileInfos->m_mapImages[gltfTexture.m_nImageIndex];
		if (gltfImage.m_nBufferViewIndex == -1 && gltfImage.m_strFileName.length() == 0)
		{
			return;
		}

		if (texList.size() > 0)
		{
			std::map<int, TextureDataInfo*>::iterator it = texList.find(gltfImage.m_nBufferViewIndex);
			if (it != texList.end())
			{
				texPtr = it->second;
				return;
			}
		}
		//生成出的纹理文件路径
		std::wstring strTexFile;
		std::wstring strTexExt;
		if (gltfImage.m_strFileName.length() == 0)
		{
			const GLTFBufferView& gltfBufferView = pTileInfos->m_mapBufferViews[gltfImage.m_nBufferViewIndex];

			unsigned int nBufferSize = gltfBufferView.m_nByteLength;
			unsigned char* pBuffer = new unsigned char[nBufferSize];
			const GLTFBuffer& gltfBuffer = pTileInfos->m_mapBuffers[gltfBufferView.m_nBufferIndex];
			memcpy(pBuffer, gltfBuffer.m_pBuffer + gltfBufferView.m_nByteOffset, nBufferSize);

			// 存纹理文件
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
			strTexFile = StringUtil::GetAbsolutePath(strOutputDir, strTexName);

			std::string strTemp = StringUtil::UNICODE_to_UTF8(strTexFile);
			strTemp = StringUtil::UTF8_to_ANSI(strTemp);

			FILE* file = fopen(strTemp.c_str(), "wb");
			fwrite(pBuffer, sizeof(unsigned char), nBufferSize, file);
			fclose(file);

			delete pBuffer;
			pBuffer = nullptr;
		}
		else
		{
			if (!gltfImage.m_strFileName.empty())
			{
				strTexExt = StringUtil::GetExt(gltfImage.m_strFileName);
			}
			else
			{
				strTexExt = StringUtil::GetExt(gltfImage.m_strFile);
			}
			
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
		if (gltfImage.m_strFileName.length() == 0)
		{
			std::string strRemove = StringUtil::UNICODE_to_UTF8(strTexFile);
			strRemove = StringUtil::UTF8_to_ANSI(strRemove);
			remove(strRemove.c_str());
		}

		TextureDataInfo* pTexDataInfo = new TextureDataInfo;
		pTexDataInfo->m_strName = BaseUtils::CreateGuid();
		pTexDataInfo->m_pTextureData = pTextrueData;

		if (bTo2N)
		{
			TextureUtils::ScaleTextureData(pTexDataInfo->m_pTextureData, TextureUtils::AdjustTextureSize(pTexDataInfo->m_pTextureData->m_nWidth), TextureUtils::AdjustTextureSize(pTexDataInfo->m_pTextureData->m_nHeight), true);
		}

		bool bTexCmp = m_3DTilesParams.GetIsTextureCompress();
		TextureCompressType nTexCmpType = m_3DTilesParser.GetTextureCompressType();
		TextureUtils::UpdateTextureTier(pTexDataInfo, nTexCmpType);

		texPtr = pTexDataInfo;
		texList[gltfImage.m_nBufferViewIndex] = texPtr;
	}

	void ProcessTools::SplitSkeletonByIndex(VertexDataPackage* pDataPackage, IndexPackage* pIndexPackage)
	{
		int nPosDim = pDataPackage->m_nVertexDimension;
		int nNormalDim = pDataPackage->m_nNormalDimension;
		int nColorDim = 1;
		int nTexDim = 2;

		unsigned int nTriangleCount = pIndexPackage->m_nIndexesCount / 3;
		// 根据索引来拆分三角形
		if (nTriangleCount > 0)
		{
			float* pNewPosData = new float[nTriangleCount * nPosDim * 3];
			float* pNewNormalData = nullptr;
			unsigned int* pNewColorData = nullptr;
			float* pNewTexData = nullptr;

			unsigned short* pNewUShortIndexs = nullptr;
			unsigned int* pNewUintIndexs = nullptr;
			// 法线
			if (pDataPackage->m_nNormalCount > 0 && pDataPackage->m_pNormals != nullptr)
			{
				pNewNormalData = new float[nTriangleCount * nNormalDim * 3];
			}

			// 顶点颜色
			if (pDataPackage->m_nVertexColorCount > 0 && pDataPackage->m_pVertexColor != nullptr)
			{
				pNewColorData = new unsigned int[nTriangleCount * nColorDim * 3];
			}

			// 纹理
			if (pDataPackage->m_TexCoordCount[0] > 0 && pDataPackage->m_pTexCoords[0] != nullptr)
			{
				pNewTexData = new float[nTriangleCount * nTexDim * 3];;
			}

			if (nTriangleCount * 3 > 65535)
			{
				pNewUintIndexs = new unsigned int[nTriangleCount * 3];
			}
			else
			{
				pNewUShortIndexs = new unsigned short[nTriangleCount * 3];
			}

			for (unsigned int nTriangle = 0; nTriangle < nTriangleCount; nTriangle++)
			{
				// 获取顶点索引
				unsigned int nIndex1 = pIndexPackage->m_pIndexes[nTriangle * 3 + 0];
				unsigned int nIndex2 = pIndexPackage->m_pIndexes[nTriangle * 3 + 1];
				unsigned int nIndex3 = pIndexPackage->m_pIndexes[nTriangle * 3 + 2];

				// 获取顶点
				for (unsigned int nD = 0; nD < nPosDim; nD++)
				{
					// 第一个点 
					pNewPosData[(nTriangle * 3 + 0) * nPosDim + nD] = pDataPackage->m_pVertices[nIndex1 * nPosDim + nD];
					// 第二个点
					pNewPosData[(nTriangle * 3 + 1) * nPosDim + nD] = pDataPackage->m_pVertices[nIndex2 * nPosDim + nD];
					// 第三个点
					pNewPosData[(nTriangle * 3 + 2) * nPosDim + nD] = pDataPackage->m_pVertices[nIndex3 * nPosDim + nD];
				}

				// 法线
				if (pNewNormalData != nullptr)
				{
					// 获取法线
					for (unsigned int nD = 0; nD < nNormalDim; nD++)
					{
						// 第一个点 
						pNewNormalData[(nTriangle * 3 + 0) * nNormalDim + nD] = pDataPackage->m_pNormals[nIndex1 * nNormalDim + nD];
						// 第二个点
						pNewNormalData[(nTriangle * 3 + 1) * nNormalDim + nD] = pDataPackage->m_pNormals[nIndex2 * nNormalDim + nD];
						// 第三个点
						pNewNormalData[(nTriangle * 3 + 2) * nNormalDim + nD] = pDataPackage->m_pNormals[nIndex3 * nNormalDim + nD];
					}
				}

				// 顶点颜色
				if (pNewColorData != nullptr)
				{
					// 获取
					for (unsigned int nD = 0; nD < nColorDim; nD++)
					{
						// 第一个点 
						pNewColorData[(nTriangle * 3 + 0) * nColorDim + nD] = pDataPackage->m_pVertexColor[nIndex1 * nColorDim + nD];
						// 第二个点
						pNewColorData[(nTriangle * 3 + 1) * nColorDim + nD] = pDataPackage->m_pVertexColor[nIndex2 * nColorDim + nD];
						// 第三个点
						pNewColorData[(nTriangle * 3 + 2) * nColorDim + nD] = pDataPackage->m_pVertexColor[nIndex3 * nColorDim + nD];
					}
				}

				// 顶点纹理
				if (pNewTexData != nullptr)
				{
					// 获取
					for (unsigned int nD = 0; nD < nTexDim; nD++)
					{
						// 第一个点 
						pNewTexData[(nTriangle * 3 + 0) * nTexDim + nD] = pDataPackage->m_pTexCoords[0][nIndex1 * nTexDim + nD];
						// 第二个点
						pNewTexData[(nTriangle * 3 + 1) * nTexDim + nD] = pDataPackage->m_pTexCoords[0][nIndex2 * nTexDim + nD];
						// 第三个点
						pNewTexData[(nTriangle * 3 + 2) * nTexDim + nD] = pDataPackage->m_pTexCoords[0][nIndex3 * nTexDim + nD];
					}
				}

				// 索引
				if (pNewUintIndexs != nullptr)
				{
					pNewUintIndexs[nTriangle * 3 + 0] = nTriangle * 3 + 0;
					pNewUintIndexs[nTriangle * 3 + 1] = nTriangle * 3 + 1;
					pNewUintIndexs[nTriangle * 3 + 2] = nTriangle * 3 + 2;
				}
				else if (pNewUShortIndexs != nullptr)
				{
					pNewUShortIndexs[nTriangle * 3 + 0] = nTriangle * 3 + 0;
					pNewUShortIndexs[nTriangle * 3 + 1] = nTriangle * 3 + 1;
					pNewUShortIndexs[nTriangle * 3 + 2] = nTriangle * 3 + 2;
				}
			}

			pDataPackage->clear();

			// 顶点
			pDataPackage->m_nVertexDimension = nPosDim;
			pDataPackage->m_nVerticesCount = nTriangleCount * 3;
			pDataPackage->m_pVertices = pNewPosData;

			// 顶点颜色
			if (pNewColorData != nullptr)
			{
				pDataPackage->m_nVertexColorCount = nTriangleCount * 3;
				pDataPackage->m_pVertexColor = pNewColorData;
			}

			// 顶点法线
			if (pNewNormalData != nullptr)
			{
				pDataPackage->m_nNormalCount = nTriangleCount * 3;
				pDataPackage->m_nNormalDimension = nNormalDim;
				pDataPackage->m_pNormals = pNewNormalData;
			}

			// 纹理
			if (pNewTexData != nullptr)
			{
				pDataPackage->m_nTexDimensions[0] = nTexDim;
				pDataPackage->m_TexCoordCount[0] = nTriangleCount * 3;
				pDataPackage->m_pTexCoords[0] = pNewTexData;
			}

			// 索引
			if (pNewUintIndexs != nullptr)
			{
				if (pIndexPackage->m_pIndexes != nullptr)
				{
					delete[] pIndexPackage->m_pIndexes;
					pIndexPackage->m_pIndexes = nullptr;
				}

				pIndexPackage->m_nIndexesCount = nTriangleCount * 3;
				pIndexPackage->m_enIndexType = IT_32BIT;
				pIndexPackage->m_pIndexes = (unsigned short*)pNewUintIndexs;
			}
			else if (pNewUShortIndexs != nullptr)
			{
				if (pIndexPackage->m_pIndexes != nullptr)
				{
					delete[] pIndexPackage->m_pIndexes;
					pIndexPackage->m_pIndexes = nullptr;
				}

				pIndexPackage->m_nIndexesCount = nTriangleCount * 3;
				pIndexPackage->m_enIndexType = IT_16BIT;
				pIndexPackage->m_pIndexes = pNewUShortIndexs;
			}
		}
	}

	bool ProcessTools::CreateInstanceInfo(VertexDataPackage * pVertexDataPackage, BoundingBox & entireBox, std::vector<Matrix4d>& vecMat, std::vector<unsigned int>& vecId)
	{
		if (vecMat.size() != vecId.size())
		{
			return false;
		}
		if (pVertexDataPackage == NULL || pVertexDataPackage->m_nVerticesCount < 1)
		{
			return false;
		}
#pragma region 原始包围盒向量
		Vector3d vecMax, vecMin;
		vecMax.x = -DBLMAX;
		vecMax.y = -DBLMAX;
		vecMax.z = -DBLMAX;
		vecMin.x = DBLMAX;
		vecMin.y = DBLMAX;
		vecMin.z = DBLMAX;

		int nVertexDim = pVertexDataPackage->m_nVertexDimension;
		int nValidVertexDim = nVertexDim > 3 ? 3 : nVertexDim;
		short* pCompressVertex = (short*)pVertexDataPackage->m_pVertices;
		for (int i = 0; i < pVertexDataPackage->m_nVerticesCount; i++)
		{
			Vector3d vertChange;
			for (int j = 0; j < nValidVertexDim; j++)
			{
				if (pVertexDataPackage->m_nCompressOptions & SVC_Vertex)
				{
					short encodeVal = pCompressVertex[i * nVertexDim + j];
					vertChange[j] = encodeVal * pVertexDataPackage->m_fVertCompressConstant + pVertexDataPackage->m_minVerticesValue[j];
				}
				else
				{
					vertChange[j] = pVertexDataPackage->m_pVertices[i * nVertexDim + j];
				}
			}

			if (vertChange.x > vecMax.x) vecMax.x = vertChange.x;
			if (vertChange.y > vecMax.y) vecMax.y = vertChange.y;
			if (vertChange.z > vecMax.z) vecMax.z = vertChange.z;
			if (vertChange.x < vecMin.x) vecMin.x = vertChange.x;
			if (vertChange.y < vecMin.y) vecMin.y = vertChange.y;
			if (vertChange.z < vecMin.z) vecMin.z = vertChange.z;
		}
#pragma endregion

#pragma region 填充2重纹理坐标
		for (int i = 0; i < 2; i++)
		{
			if (pVertexDataPackage->m_pTexCoords[i] == NULL)
			{
				pVertexDataPackage->SetTexCoordsNum(i, pVertexDataPackage->m_nVerticesCount);
			}
		}
#pragma endregion

#pragma region 填充InstanceInfo
		pVertexDataPackage->m_nTexDimensions[2] = 17;
		pVertexDataPackage->SetTexCoordsNum(2, vecId.size());
		float* pBuffer = pVertexDataPackage->m_pTexCoords[2];
		for (int iID = 0; iID < vecId.size(); iID++)
		{
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 4; j++)
					*pBuffer++ = static_cast<float>(vecMat[iID][j][i]);
			}
			*pBuffer++ = 1.0f;//R
			*pBuffer++ = 1.0f;//G
			*pBuffer++ = 1.0f;//B
			*pBuffer++ = 1.0f;//A
			*(unsigned int*)pBuffer++ = vecId[iID];
		}
#pragma endregion

#pragma region 计算并存储包围盒
		for (int iMat = 0; iMat < vecMat.size(); iMat++)
		{
			BoundingBox tmpBox;
			tmpBox.SetExtents(vecMin.MultiplyMatrix(vecMat[iMat]), vecMax.MultiplyMatrix(vecMat[iMat]));
			entireBox.Merge(tmpBox);
		}
#pragma endregion

		return true;
	}

	rapidjson::Document ProcessTools::ParseBuffer(void* pBuffer, unsigned int nLength)
	{
		std::strstreambuf buf((char*)pBuffer, nLength);
		std::istream ifs(&buf);
		rapidjson::IStreamWrapper isw(ifs);
		rapidjson::Document doc;
		doc.ParseStream(isw);
		return doc;
	}

	TileContentType ProcessTools::ParseTileContentType(MemoryStream& stream, bool bReset)
	{
		unsigned char head[4];
		size_t nPos = stream.GetReadPosition();
		stream >> head[0] >> head[1] >> head[2] >> head[3];
		if (bReset)
		{
			stream.SetReadPosition(nPos);
		}

		TileContentType nType;
		if (head[0] == 'b' && head[1] == '3' && head[2] == 'd' && head[3] == 'm')
		{
			nType = TC_B3DM;
		}
		else if (head[0] == 'c' && head[1] == 'm' && head[2] == 'p' && head[3] == 't')
		{
			nType = TC_CMPT;
		}
		else if (head[0] == 'i'  && head[1] == '3' && head[2] == 'd' && head[3] == 'm')
		{
			nType = TC_I3DM;
		}

		return nType;
	}

	FieldType ProcessTools::GetFieldType(std::string strType)
	{
		FieldType nType;
		if (strType == "DOUBLE")
		{
			nType = FT_Double;
		}
		else if (strType == "FLOAT")
		{
			nType = FT_Float;
		}
		else if (strType == "BYTE")
		{
			nType = FT_Byte;
		}
		else if (strType == "UNSIGNED_SHORT")
		{
			nType = FT_INT16;
		}
		else
		{
			assert(false);
		}
		return nType;
	}

	int ProcessTools::NumOfComponents(std::string strType)
	{
		int nNum = 1;
		if (strType == "SCALAR")
		{
			nNum = 1;
		}
		else if (strType == "VEC2")
		{
			nNum = 2;
		}
		else if (strType == "VEC3")
		{
			nNum = 3;
		}
		else if (strType == "VEC4")
		{
			nNum = 4;
		}
		else
		{
			assert(false);
		}
		return nNum;
	}
}
