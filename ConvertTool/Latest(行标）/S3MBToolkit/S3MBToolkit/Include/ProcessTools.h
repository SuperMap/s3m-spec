#if !defined(PROCESSTOOLS_H__7A246017_0CD3_4B19_8F03_F2842F2D335F__INCLUDED_)
#define PROCESSTOOLS_H__7A246017_0CD3_4B19_8F03_F2842F2D335F__INCLUDED_

#pragma once
#include <string>
#include "stdafx.h"
#include "BoundingSphere.h"
#include "I3SParams.h"
#include "Point3D.h"
#include "ProcessParams.h"
#include "S3MBEnums.h"
#include "S3MBLayerInfos.h"
#include "Shell.h"
#include "MemoryStream.h"
#include "ThreeDTilesParser.h"
#include "OSGBParser.h"
#include "OBJConfig.h"

#include "document.h"
#include "istreamwrapper.h"
#include "prettywriter.h"
#include "stringbuffer.h"

namespace S3MB
{
	class STK_API ThreeDTilesParams : public ProcessParams
	{
	public:
		ThreeDTilesParams();

		virtual ProcessType GetProcessType();

		void SetTilesetPath(const std::wstring& strPath);
		const std::wstring& GetTilesetPath() const;

		void SetOutputDir(const std::wstring& strDir);
		const std::wstring& GetOutputDir() const;

		void SetPosition(const Point3D& pos);
		const Point3D& GetPosition() const;

		void SetVertexCompressType(S3MBVertexTag nType);
		S3MBVertexTag GetVertexCompressType() const;

		void SetTextureCompressType(TextureCompressType nType);
		TextureCompressType GetTextureCompressType() const;

		bool GetIsTextureCompress() const;

		void SetIsModel(bool bIsModel);
		bool GetIsModel() const;

		void SetIsNEU(bool bIsNEU);
		bool GetIsNEU() const;

	private:
		//! \brief tileset文件路径
		std::wstring m_strTilesetPath;
		//! \brief 输出的路径
		std::wstring m_strOutputDir;
		//! \brief 位置
		Point3D m_pntPosition;
		//! \brief 顶点压缩类型
		S3MBVertexTag m_nVertexCompressType;
		//! \brief 纹理压缩类型
		TextureCompressType m_nTextureCompressType;
		//! \brief 是否是模型
		bool m_bIsModel;
		//! \brief 3dTiles数据是否圆球
		bool m_bIsNEU;
	};

	class STK_API S3MBParams : public ProcessParams
	{
	public:
		S3MBParams();

		virtual ProcessType GetProcessType();

		void SetSCPPath(const std::wstring& strPath);
		const std::wstring& GetSCPPath() const;

		void SetOutputDir(const std::wstring& strDir);
		const std::wstring& GetOutputDir() const;

		void SetThreadNum(int nNum);
		int GetThreadNum() const;

	private:
		//! \brief scp文件路径
		std::wstring m_strSCPPath;
		//! \brief 输出的路径
		std::wstring m_strOutputDir;
		//! \brief 线程数
		int m_nThreadNum;
	};

	class STK_API OSGBParams : public ProcessParams
	{
	public:
		OSGBParams();

		virtual ProcessType GetProcessType();

		void SetInputFilePath(const std::wstring& strPath);
		const std::wstring& GetInputFilePath() const;

		void SetOutputDir(const std::wstring& strDir);
		const std::wstring& GetOutputDir() const;

		void SetTextureCompressType(TextureCompressType nType);
		TextureCompressType GetTextureCompressType() const;

	private:
		//! \brief 配置文件路径
		std::wstring m_strInputFilePath;
		//! \brief 输出的路径
		std::wstring m_strOutputDir;
		//! \brief 纹理压缩类型
		TextureCompressType m_nTextureCompressType;
	};

	class STK_API OBJParams : public ProcessParams
	{
	public:
		OBJParams();

		virtual ProcessType GetProcessType();

		void SetInputFilePath(const std::wstring& strPath);
		const std::wstring& GetInputFilePath() const;

		void SetOutputDir(const std::wstring& strDir);
		const std::wstring& GetOutputDir() const;

	private:
		//! \brief 配置文件路径
		std::wstring m_strInputFilePath;
		//! \brief 输出的路径
		std::wstring m_strOutputDir;
	};

	class STK_API OSGBToS3MBTool
	{
	public:
		OSGBToS3MBTool();
		~OSGBToS3MBTool();

		//! \brief 解析配置文件
		bool ReadConfig(const wstring& strPath);
		//! \brief 解析OSGB并且生成S3MB
		bool ProcessBodyData(const wstring& strOutputDir);
		//! \brief 生成配置文件
		bool WriteConfig(const wstring& strOutputDir);

	private:
		void ProcessChildNode(RenderOperationNode* pNode, const std::wstring& strInputDir, const wstring& strOutputDir);
		
		void ReComputeBoundingBox(RenderOperationGroup* pGroup);
		void GetGroupOrientedBoundingBox(RenderOperationNode* pGroup, OrientedBoundingBox& outOBB);
		void ProcessGeodeInfo(RenderOperationGeode* pGeode, Matrix4d& matWorldView, std::vector<Vector3d>& vVec3);
		void GetGroupBoundingBox(RenderOperationGroup*& pGroup, BoundingBox& boundingBox);
		void GetPagedLODBoundingBox(RenderOperationPagedLOD*& pPagedLOD, BoundingBox& boundingBox);

	private:
		//! \brief OSGB数据目录
		std::wstring m_strOSGBDir;
		//! \brief OSGB配置信息
		OSGBConfig m_OSGBConfig;
		//! \brief 根节点包围盒
		std::vector<std::pair<std::wstring, BoundingBox> > m_vecRootNameBox;
		//! \brief 根节点有向包围盒
		std::vector<std::pair<std::wstring, OrientedBoundingBox> > m_vecRootNameOBB;
		//! \brief 范围
		Rect2D m_rcBounds;
	};

	class STK_API OBJToS3MBTool
	{
	public:
		struct OBJLodQuarterTreeNode
		{
			unsigned int m_nLod;
			unsigned int m_nRow;
			unsigned int m_nCol;
			// 节点名字
			std::wstring m_strFileName;
			// 子节点
			std::vector<OBJLodQuarterTreeNode*> m_vecChild;
			// 包围盒
            OrientedBoundingBox m_oriBundingBox;
		};

	public:
		OBJToS3MBTool();
		~OBJToS3MBTool();

		//! \brief 解析配置文件
		bool ReadConfig(const wstring& strPath);
		//! \brief 建立四分树
		void MakeQuarterTree();
		//! \brief 解析OBJ并且生成S3MB
		bool ProcessBodyData(const wstring& strOutputDir);
		//! \brief 生成配置文件
		bool WriteConfig(const wstring& strOutputDir);

	private:
		void GetQuarterChild(unsigned int nLodNum, unsigned int nLod, OBJLodQuarterTreeNode* pNode, unsigned int nMinRow, unsigned int nMaxRow, unsigned int nMinCol, unsigned int nMaxCol);
		OBJLodQuarterTreeNode* CreateQTreeNode(unsigned int nLod, unsigned int nRow, unsigned int nCol);

		void ProcessNode(const wstring& strOutputDir, OBJLodQuarterTreeNode* pNode);
		void AppendRO(RenderOperationGroup* pGroupSrc, RenderOperationGroup* pGroupDes, RenderOperationGeode *pGeodeDes);

	private:
		//! \brief OBJ数据的目录
		std::wstring m_strOBJDir;
		//! \brief OBJ配置信息
		OBJConfig m_OBJConfig;
		//! \brief 四分树
		OBJLodQuarterTreeNode* m_pTreeNode;
	};

	class STK_API ProcessTools
	{
	public:
		ProcessTools();
		~ProcessTools();

		//! \brief 3DTiles转成S3MB
		bool ThreeDTilesToS3MB(const ThreeDTilesParams& params);
		//! \brief S3MB转成3DTiles
		bool S3MBTo3DTiles(const S3MBParams& params);
		//! \brief OSGB转成S3MB
		bool OSGBToS3MB(const OSGBParams& params);
		//! \brief OBJ转成S3MB
		bool OBJToS3MB(const OBJParams& params);
		//! \brief I3S转成S3MB
		bool I3SToS3MB(const I3SParams& params);

	private:
		void SaveGroup(GLTFTreeNode* pNode, std::wstring strOutPath, RenderOperationGroup* pGroup);
		bool SaveSCPFile(const Point3D& pos, const BoundingBox& box, std::vector<std::wstring> vecRootFile, std::vector<BoundingBox> vecRootBox, std::wstring strOutputDir);

		bool GenerateS3MB();
		void GetNodes(GLTFTreeNode* pNode, std::vector<GLTFTreeNode*>& vecNodes);
		void ProcessTreeNode(GLTFTreeNode* pNode, std::wstring strInputDir, std::wstring strOutputDir);
		void ProcessNode(GLTFTreeNode* pNode, std::wstring strInputDir, std::wstring strOutputDir, RenderOperationGroup* pGroup, std::wstring strParentFileName);
		bool ParseCMPT(MemoryStream& stream, GLTFTreeNode* pNode, std::wstring strOutputFile, RenderOperationGroup* pGroup, std::wstring strParentFileName);
		bool ParseB3DM(MemoryStream& stream, GLTFTreeNode* pNode, std::wstring strOutDir, RenderOperationGroup* pGroup, std::wstring strFatherFileName);
		bool ParseI3DM(MemoryStream& stream, GLTFTreeNode* pNode, std::wstring strOutDir, RenderOperationGroup* pGroup, std::wstring strFatherFileName);
		void ParseIDRange(GLTFTreeNode* pNode);
		void ParseIDRangeFromB3DM(MemoryStream& stream);
		void ParseIDRangeFromCMPT(MemoryStream& stream);
		void ParseIDRangeFromI3DM(MemoryStream& stream);

#pragma region glTF 1.0
		void MeshToGroup(GLTFTreeNode * pNode, GLTFTileInfos_1 *& pTileInfos, std::wstring strOutputPath, RenderOperationGroup* pGroup, std::wstring strParentPath, Point3D& pntCenter);
		void GetTextureData(GLTFTileInfos_1 *& pTileInfos, std::wstring& strMaterialName, std::wstring& strOutputDir, std::map<std::wstring, TextureDataInfo*>& texList, TextureDataInfo*& texPtr);
#pragma endregion

#pragma region glTF 2.0
		void MeshToGroup(GLTFTreeNode* pNode, GLTFTileInfos_2*& pTileInfos, std::wstring strOutputPath, RenderOperationGroup* pGroup, std::wstring strParentPath, Point3D& pntCenter);
        bool MeshToGeode(GLTFTreeNode* pNode, GLTFTileInfos_2*& pTileInfos, unsigned int nMeshIndex, std::wstring strOutputPath, RenderOperationGeode* pGeode, std::map<std::wstring, Skeleton*>& mapSkeleton, std::map<std::wstring, Material*>& mapMaterial, std::map<std::wstring, TextureDataInfo*>& mapTexture, std::map<int, TextureDataInfo*>& textureLists, BoundingBox& entileBox, Point3D& pntCenter);
        bool ProcessSkeleton(GLTFTreeNode* pNode, GLTFTileInfos_2*& pTileInfos, Skeleton*& pSkeleton, GLTFPrimitive& gltfPrimitive, Matrix4d mat, std::wstring strMaterialName, Point3D& pntCenter);
		void ProcessMaterial(GLTFTileInfos_2*& pTileInfos, std::wstring strOutputPath, GLTFPrimitive& gltfPrimitive, Material* pMaterial, std::map<int, TextureDataInfo*>& textureLists, std::vector<TextureDataInfo*>& vecTexPtr);
		void GetTextureData(GLTFTileInfos_2*& pTileInfos, int nTexIndex, std::wstring& strOutputDir, std::map<int, TextureDataInfo*>& texList, TextureDataInfo*& texPtr);
#pragma endregion

		void SplitSkeletonByIndex(VertexDataPackage* pDataPackage, IndexPackage* pIndexPackage);
		bool CreateInstanceInfo(VertexDataPackage* pVertexDataPackage, BoundingBox& entireBox, std::vector<Matrix4d>& vecMat, std::vector<unsigned int>& vecId);

	private:
		static rapidjson::Document ParseBuffer(void* pBuffer, unsigned int nLength);
		static TileContentType ParseTileContentType(MemoryStream& stream, bool bReset = true);
		static FieldType GetFieldType(std::string strType);
		static int NumOfComponents(std::string strType);

	private:
		//! \brief 3DTiles解析器
		ThreeDTilesParser m_3DTilesParser;
		//! \brief 3DTiles的参数
		ThreeDTilesParams m_3DTilesParams;

		//! \brief S3MB的参数
		S3MBParams m_S3MBParams;

		//! \brief OSGB的参数
		OSGBParams m_OSGBParams;
	};
}

#endif