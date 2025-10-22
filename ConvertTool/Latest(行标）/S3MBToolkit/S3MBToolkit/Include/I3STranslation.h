#if !defined(I3SNODETRANSLATION_H__D631901E_B176_4024_A8C5_68D9C29BDA26__INCLUDED_)
#define I3SNODETRANSLATION_H__D631901E_B176_4024_A8C5_68D9C29BDA26__INCLUDED_

#pragma once
#include "Common.h"
#include "GLTFContent.h"
#include "I3SParams.h"
#include "s3m/include/MemoryStream.h"
#include "S3MBConfig.h"
#include "Shell.h"
#include "Skeleton.h"
#include "SlpkFile.h"
#include "stdafx.h"
#include "Texture.h"
#include <limits.h>

namespace S3MB
{
	class Material;
	class TextureDataInfo;
	class STK_API I3SNodesTranslation
	{
	public:
		I3SNodesTranslation(const I3SParams& i3sParams, const I3SParseParams& i3sParseParams, 
			const S3MBConfig& s3mbConfig, const std::vector<std::wstring>& vecPath);
		~I3SNodesTranslation();

		// 获取属性文件路径和id范围
		const std::vector<std::pair<std::wstring, AttributeIndexInfo> >& GetAttributeIndexInfoPairs() const;
		// 获取高度范围
		const std::pair<double, double>& GetHeightRange() const;
		// 获取顶点扩展属性
		const std::map<std::wstring, CategoryDescript>& GetCategoryDescripts() const;
		// 获取根节点名称和包围盒
		const std::vector<std::wstring>& GetRootNames() const;
		const std::vector<OrientedBoundingBox>& GetRootObbs() const;

		// 运行
		bool Run();

	private:
		// 转化节点
		bool TranslateNode(const std::vector<std::wstring>& vecDir, const std::wstring& strOutputPath, 
			bool bRootNode, BoundingBox& box, OrientedBoundingBox& obb, std::map<int, Feature*>& mapFeature);
		bool TranslatePointCloudNode(const std::map<std::wstring, I3SNodeInfo>& mapNodeInfo, const std::vector<std::wstring>& vecDir, 
			const std::wstring& strOutputPath, bool bRootNode, BoundingBox& box, OrientedBoundingBox& obb);
		bool TranslatePointNode(const std::vector<std::wstring>& vecDir, const std::wstring& strOutputPath, 
			bool bRootNode, BoundingBox& box, OrientedBoundingBox& obb, std::map<int, Feature*>& mapFeature);
		// i3s material info转成s3m material
		void TranslateMaterial(const I3SMaterialInfo& matInfo, const std::vector<std::wstring>& vecTexture, Material*& pMaterial);
		// 转化纹理
		bool TranslateTextures(const std::vector<std::wstring>& vecTexture, std::map<std::wstring, TextureDataInfo*>& mapTexInfo);
		bool TranslateTexture(const std::wstring& strName, const std::wstring& strExt, TextureDataInfo*& pTexture, bool bGz);
		// 转化几何结构
		bool TranslateGeometries(const std::vector<std::wstring>& vecGeoPath, const I3SNodeInfo& nodeInfo,
			const I3SMaterialInfo& matInfo, std::vector<Skeleton*>& vecGeometry);
		bool TranslateGeometry(const std::wstring& strName, const I3SNodeInfo& nodeInfo, const I3SMaterialInfo& matInfo, Skeleton*& pGeometry);
		bool TranslateStandardGeometry(const std::wstring& strName, const I3SNodeInfo& nodeInfo, const I3SMaterialInfo& matInfo, 
			unsigned char* pData, size_t nDataSize, Skeleton*& pGeometry);
		bool TranslateDracoCompressedGeometry(const std::wstring& strName, const I3SNodeInfo& nodeInfo, const I3SMaterialInfo& matInfo, 
			char* pData, size_t nDataSize, Skeleton*& pGeometry);
		bool TranslatePointCloudGeometries(const std::vector<std::wstring>& vecGeoPath, const I3SNodeInfo& nodeInfo, std::vector<Skeleton*>& vecGeometry);
		bool TranslatePointCloudGeometry(const std::wstring& strName, const I3SNodeInfo& nodeInfo, Skeleton*& pGeometry);
		bool TranslatePointGeometries(const std::vector<std::wstring>& vecGeoPath, const I3SNodeInfo& nodeInfo, 
			std::map<int, Feature*>& mapFeature, std::vector<Skeleton*>& vecGeometry);
		bool TranslatePointGeometry(const std::wstring& strName, const I3SNodeInfo& nodeInfo, 
			std::map<int, Feature*>& mapFeature, std::vector<Skeleton*>& vecGeometry);
		// 转化顶点数据
		bool TranslateVertexData(MemoryStream& stream, const I3SNodeInfo& nodeInfo, const std::wstring& strName, VertexDataPackage* pDataPackage);
		bool TranslateVertexExtensionData(MemoryStream& stream, const std::wstring& strName, VertexDataPackage* pDataPackage);
		bool TranslateDracoCompVertexExtensionData(const std::vector<std::pair<std::wstring, int> >& vecVexAttExt, VertexDataPackage* pDataPackage);
		bool TranslatePointCloudVertexData(const std::wstring& strName, const I3SNodeInfo& nodeInfo, VertexDataPackage*& pDataPackage);
		bool TranslatePointCloudVertexColorData(const std::wstring& strName, VertexDataPackage*& pDataPackage);
		bool TranslatePointCloudIntensityData(const std::wstring& strName, VertexDataPackage*& pDataPackage);
		bool TranslatePointCloudClassCodeData(const std::wstring& strName, VertexDataPackage*& pDataPackage);
		// 转化特征数据
		bool TranslateFeatureData(MemoryStream& stream, const std::vector<std::pair<std::wstring, int> >& vecVexAttExtAndId, 
			unsigned int nFeatureCount, std::vector<IDInfo*> vecIDInfo);
		bool TranslateFeatureData(unsigned int* pFeatureIds, unsigned int nFeatureIdCount, std::vector<IDInfo*>& vecIDInfo);
		// 转化属性
		bool TranslateAttributes(const std::vector<std::wstring>& vecAttPath, std::map<int, Feature*>& mapFeature);
		bool TranslateAttribute(const std::wstring& strName, const S3MBFieldInfo& fieldInfo, const I3SAttributeInfo& attInfo, std::vector<Feature*>& vecFeature);
		// 设置顶点扩展属性极值
		void SetDescriptRangeMinMax(const std::wstring& strCategory, unsigned short nDim,
			VertexAttributeType nDataType, const std::vector<double>& vecMin, const std::vector<double>& vecMax);
		// 获取矩阵
		void GetMatrix(const Vector3d& vPos, Matrix4d& mat);

	private:
		// 用户设置的参数
		I3SParams m_I3SParams;
		// 解析的参数
		I3SParseParams m_I3SParseParams;
		// s3m配置文件
		S3MBConfig m_S3MBConfig;
		// slpk文件
		SlpkFile m_slpkFile;
		// 节点路径名的数组
		std::vector<std::wstring> m_vecNodePath;
		// 顶点扩展属性
		std::map<std::wstring, CategoryDescript> m_mapCategoryDescript;
		// 高度范围
		std::pair<double, double> m_pairHeightRange;
		// 根节点名称和包围盒
		std::vector<std::wstring> m_vecRootName;
		std::vector<OrientedBoundingBox> m_vecRootObb;
		// 属性
		std::vector<std::pair<std::wstring, AttributeIndexInfo> > m_vecAttIndexInfoPair;
	};

	class STK_API I3SSymbolsTranslation
	{
	public:
		I3SSymbolsTranslation(const I3SParams& i3sParams, const I3SParseParams& i3sParseParams, const std::vector<std::wstring>& vecPath);
		~I3SSymbolsTranslation();

		// 运行
		bool Run();

	private:
		// 写原始符号
		bool WriteOriginSymbol(const std::wstring& strSymbolPath, const std::wstring& strOutputPath);
		// 原始符号转为s3m
		bool OriginSymbolToS3MB(const std::wstring& strOriginPath, const std::wstring& strOutputPath);

		// glb转Group
		static bool GLBToGroup(GLTFTileInfos_2* pTileInfos, const std::wstring& strOutputPath, RenderOperationGroup*& pGroup);
		// 转化材质
		static bool TranslateMaterial(GLTFTileInfos_2* pTileInfos, const GLTFPrimitive& gltfPrimitive, const std::wstring& strOutputPath, 
			Material*& pMaterial, std::map<int, TextureDataInfo*>& textureLists, std::vector<TextureDataInfo*>& vecTexPtr);
		// 转化纹理
		static bool TranslateTextures(GLTFTileInfos_2* pTileInfos, int nTexIndex, const std::wstring& strOutputPath, std::map<int, TextureDataInfo*>& texList, TextureDataInfo*& texPtr);
		// 转化几何结构
		static bool TranslateSkeleton(GLTFTileInfos_2* pTileInfos, const GLTFPrimitive& gltfPrimitive, const std::wstring& strMaterialName, Skeleton*& pSkeleton);
		// 转化顶点数据
		static bool TranslateVertexData(GLTFTileInfos_2* pTileInfos, const GLTFAttributes& gltfAttributes, VertexDataPackage*& pDataPackage);
		// 转化索引数据
		static bool TranslateIndexData(GLTFTileInfos_2* pTileInfos, const GLTFPrimitive& gltfPrimitive, const std::wstring& strMaterialName, IndexPackage*& pIndexPackage);

	private:
		// 用户设置的参数
		I3SParams m_I3SParams;
		// 解析的参数
		I3SParseParams m_I3SParseParams;
		// slpk文件
		SlpkFile m_slpkFile;
		// 符号路径名的数组
		std::vector<std::wstring> m_vecSymbolPath;
	};
}

#endif