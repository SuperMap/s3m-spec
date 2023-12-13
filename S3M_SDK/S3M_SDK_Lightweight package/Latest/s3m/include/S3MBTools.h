#if !defined(S3MBTOOLS_H__028B6316_FE68_40F4_8803_836609995FC3__INCLUDED_)
#define S3MBTOOLS_H__028B6316_FE68_40F4_8803_836609995FC3__INCLUDED_

#pragma once
#include "JsonValue.h"
#include "Material.h"
#include "MemoryStream.h"
#include "Shell.h"
#include "Skeleton.h"
#include "Texture.h"
#include <fstream>
#include <iostream>
#include "BoundingBox.h"
#include "DracoTool.h"

namespace S3MB
{
	// 压缩参数
	class S3MB_API SkeletonCompressParam
	{
	public:
		SkeletonCompressParam()
		{
			m_svTag = SV_Standard;
			m_nCompressOption = 0;
		};
		SkeletonCompressParam(DracoCompressParam& dracoParam)
		{
			m_svTag = SV_DracoCompressed;
			m_dracoParam = dracoParam;
			m_nCompressOption = 0;
		};
		SkeletonCompressParam& operator=(const SkeletonCompressParam& cm)
		{
			m_svTag = cm.m_svTag;
			m_nCompressOption = cm.m_nCompressOption;
			m_dracoParam = cm.m_dracoParam;
			return *this;
		};
	public:
		// S3MBVertexTag
		S3MBVertexTag m_svTag;
		// 压缩类型
		int m_nCompressOption;
		// SV_DracoCompressed 时使用
		DracoCompressParam m_dracoParam;
	};


	class S3MB_API S3MBTools
	{
	public:
		// 加载S3MB数据流
		static bool LoadStreamDataFromStream(MemoryStream& dataStream, MemoryStream& streamUnZipped, MemoryStream& streamShell, MemoryStream& streamSkeleton, 
			MemoryStream& streamTexture, MemoryStream& streamIDInfo, wstring& strJsonMaterials, wstring& strExtensions, float fVersion = S3MB_VERSIONV3_0_1);
		// 解压S3MB文件流并释放压缩流内存
		static bool UnZipData(MemoryStream& streamUnZipped, unsigned char*& pZippedData, unsigned int nZippedSize);
		static bool UnZipData(MemoryStream& streamUnzipped, unsigned int& nUnZipSize, unsigned char*& pZippedData, unsigned int& nZippedSize);

		// 加载骨架信息
		static void LoadSkeleton(MemoryStream& streamSkeleton, std::map<wstring, Skeleton*>& mapSkeleton, float fVersion = S3MB_VERSIONV3_0_1);
		static bool LoadSkeletonData(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage, vector<IndexPackage*>& arrIndexPackage, float fVersion = S3MB_VERSIONV3_0_1);

		// 加载并解压Draco
		static bool  LoadDraco(MemoryStream &stream, \
			VertexDataPackage*& pVertexDataPack, std::vector<IndexPackage*>& arrIndexPackage, float fVersion = S3MB_VERSIONV3_0_1);

		// 字节对齐
		// isRead 指示是否为读取流
		static void StreamAlign(MemoryStream& stream, bool isRead);
		// 加载顶点信息
		static bool LoadVertex(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage);
		// 加载法线信息
		static bool LoadNormal(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage);
		// 加载切线信息
		static bool LoadTangent(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage);
		// 加载顶点颜色
		static bool LoadVertexColor(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage);
		// 加载纹理坐标信息
		static bool LoadTextureCoords(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage);
		// 加载实例化信息
		static bool LoadInstanceInfo(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage, float fVersion = S3MB_VERSIONV3_0_1);
		// 判断是否是实例化几何对象
		static bool IsInstanceBatch(VertexDataPackage* pVertexDataPackage);


		// 加载材质信息
		static void LoadMaterial(wstring strJsonMaterials, std::map<wstring, Material*>& mapMaterial,float fVersion);
		// 通过Json加载材质信息
		static void LoadMatFromJson(JsonValue& jsonMaterial, wstring& strMaterialName, Technique* pTechnique);
		static void LoadMatFromJsonV3(JsonValue& jsonMaterial, wstring& strMaterialName, Technique* pTechnique);
		// 通过Json加载PBR材质信息
		static void LoadPBRFromJson(JsonValue& jsonPBR, PBRParams*& pPBR);
		static void LoadPBRFromJsonV3(JsonValue& jsonPBR, PBRParams*& pPBR);
		// 通过Json加载纹理单元
		static void LoadTUSFromJson(JsonValue& jsonTexture, TextureUnitState* pTextureUnitState);
		static void LoadTUSFromJsonV3(JsonValue& jsonTexture, TextureUnitState* pTextureUnitState);

		static MaterialType MaterialTypeFromString(wstring strType);

		static CullingMode CullModeFromString(wstring strType);
		static CullingMode CullModeFromStringV3(wstring strType);

		static AlphaMode AlphaModeFromString(wstring strType);
		static AlphaMode AlphaModeFromStringV3(wstring strType);

		static void AddressModeFromJson(JsonValue& jsonAddMode, TextureUnitState* pTextureUnitState);

		// 加载纹理信息
		static void LoadTexture(MemoryStream& streamTexture, std::map<wstring, TextureDataInfo*>& mapTexture, float fVersion);
		static void LoadTextureData(MemoryStream& streamTexture, TextureData* pTextureData, float fVersion);
		static void ProcessTextureData(TextureData* pTextureData, TextureCompressType nCompressType, bool bGenerateMipmap = true);
		static void ToStandardType(TextureCompressType& eCompressType, PixelFormat& eFormat, unsigned nCompressType, unsigned nFormat);

		// 加载shell信息
		static void LoadShell(MemoryStream& streamShell, RenderOperationGroup* pROGroup, float fVersion = S3MB_VERSIONV3_0_1);
		// 判断是否是单独的Geode（RO对象中PagedLODs下可单独挂载Geode，表示不再有下层节点）
		static bool IsSingleGeode(MemoryStream& stream);
		static void LoadSingleGeode(RenderOperationGroup* pROGroup, RenderOperationGeode* pGeode, MemoryStream& stream, float fVersion = S3MB_VERSIONV3_0_1);
		
		static void LoadPagedLOD(RenderOperationGroup* pROGroup, RenderOperationPagedLOD* pROPagedLOD, MemoryStream& stream, float fVersion = S3MB_VERSIONV3_0_1);
		
		static void LoadGeode(RenderOperationGroup* pROGroup, RenderOperationGeode* pGeode, MemoryStream& stream);

		// 加载IDInfo
		static void LoadIDInfo(std::map<wstring, Skeleton*>& mapSkeleton, MemoryStream& streamIDInfo,float fVersion);

		// 保存shell信息
		static void SaveShell(RenderOperationGroup* pROGroup, MemoryStream& stream, float fVersion = S3MB_VERSIONV3_0_1);
		static void SavePagedLOD(RenderOperationPagedLOD* pROPagedLOD, MemoryStream& stream, float fVersion = S3MB_VERSIONV3_0_1);
		static void SaveGeode(RenderOperationGeode* pROGeode, MemoryStream& stream);

		// 保存骨架信息
		static void SaveSkeleton(std::map<wstring, Skeleton*>& mapSkeleton, MemoryStream& streamSkeleton, SkeletonCompressParam& SkeletonCompParam, float fVersion = S3MB_VERSIONV3_0_1);
		// 骨架数据保存与加载
		static bool SaveSkeletonData(VertexDataPackage*& pVertexDataPackage, \
			vector<IndexPackage*>& arrIndexPackage, MemoryStream& streamSkeleton, SkeletonCompressParam& SkeletonCompParam, float fVersion = S3MB_VERSIONV3_0_1);
		
		static bool SaveDraco(VertexDataPackage* pVertexDataPack, std::vector<IndexPackage*>& arrIndexPackage, \
			const DracoCompressParam& dracoParam, MemoryStream &stream, float fVersion);
		
		// 保存顶点信息
		static bool SaveVertex(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		// 保存法线信息
		static bool SaveNormal(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		// 保存切线信息
		static bool SaveTangent(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		// 保存顶点颜色
		static bool SaveVertexColor(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		// 保存纹理坐标
		static bool SaveTextureCoords(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		// 保存实例化信息
		static bool SaveInstanceInfo(VertexDataPackage* pVertexDataPack, MemoryStream& stream);

		// 保存材质信息
		static void SaveMaterial(std::map<wstring, Material*>& mapMaterial, JsonValue& jsonMaterials, float fVersion);
		static void MaterialToJson(Material* pMaterial, JsonValue& jsonMaterial, float fVersion);
		// pMaterial转json信息
		static void TechniqueToJson(const wstring strMaterialName, Technique* pTechnique, JsonValue& jsonContent,float fVersion);
		static wstring MaterialTypeToString(MaterialType emType);
		static wstring CullModeToString(CullingMode emType);
		// PRB转json信息
		static void PBRToJson(PBRParams* pPBR, JsonValue& jsonPBR);
		static wstring AlphaModeToString(AlphaMode emType);
		// pTextureUnitState转json信息
		static void TUSToJson(TextureUnitState* pTextureUnitState, JsonValue& jsonMaterial);
		static void AddressModeToJson(TextureUnitState* pTextureUnitState, JsonValue& jsonAddMode);

		static void SaveTextures(std::map<wstring, TextureDataInfo*>& mapTexture,MemoryStream& streamTexture,
			unsigned int nCompressType, bool bTextureChange = true, bool bGenerateMipmap = true);
		// 纹理数据保存
		static void SaveTextureData(TextureData* pTextureData, \
			MemoryStream& streamTexture, unsigned int nCompressType, bool bIsChangeTexture = true, bool bGenerateMipmap = true);
		// 纹理压缩处理
		static void ProcessTextureData(TextureData* pTextureData, unsigned int nCompressType, bool bGenerateMipmap = true);

		// 保存IDInfo
		static bool SaveIDInfo(std::map<wstring, Skeleton*>& mapSkeleton, MemoryStream& m_streamIDInfo);

		// 包围盒的保存与加载
		static void LoadBoundingBox(MemoryStream& stream, BoundingBox& bbox);
		static void SaveBoundingBox(const BoundingBox& bbox, MemoryStream& stream);

		// 方向包围盒的保存与加载
		static void SaveOBB(const OrientedBoundingBox& bBox, MemoryStream& streamBBox);
		static void LoadOBB(MemoryStream& streamBBox, OrientedBoundingBox& bBox);
		// 保存流到文件
		static bool SaveStreamData2File(const wstring strDestFilePath, \
			MemoryStream& streamShell, MemoryStream& streamSkeleton, \
			wstring& strJsonMaterials, MemoryStream& streamTexture, \
			bool bHasIDInfo, MemoryStream& streamIDInfo, wstring& strExtensions, float fVersion);

		private:
			static bool CanUseDraco(VertexDataPackage* pVertexPack, std::vector<IndexPackage*> vecIndexPack);

			static void LoadStream(MemoryStream& streamSrc, MemoryStream& streamDst);
			static void SaveStream(MemoryStream& streamDst, MemoryStream& streamSrc);

			// 骨骼动画的保存与加载
			static void AnimationsToJson(RenderOperationPagedLOD* pROPagedLOD, JsonValue& jsonAnimations);
			static void AnimationsFromJson(JsonValue& jsonAnimations, RenderOperationPagedLOD*& pROPagedLOD);

			// node转json信息
			static void NodeToJson(const std::vector<Node*>& vecNode, JsonValue& jsonNodes);
			static void NodeFromJson(JsonValue& jsonNodes, std::vector<Node*>& vecNode);

			// skin转json信息
			static void SkinToJson(const std::vector<Skin*>& vecSkin, JsonValue& jsonSkins);
			static void SkinFromJson(JsonValue& jsonSkins, std::vector<Skin*>& vecSkin);

			// 动画转json信息
			static void AnimationToJson(const std::vector<Animation*>& vecAnimationState, \
				const std::vector<std::map<wstring, AnimationCurve*> >& vecAnimationCurve, JsonValue& jsonAnimationContents);
			static void AnimationFromJson(JsonValue& jsonAnimationContents, \
				std::vector<Animation*>& vecAnimationState, std::vector<std::map<wstring, AnimationCurve*> >& vecAnimationCurve);

			// 顶点属性的扩展
			static bool LoadVertexAttributeExtension(MemoryStream& stream, VertexDataPackage* pVertexDataPack);
			static bool SaveVertexAttributeExtension(VertexDataPackage* pVertexDataPack, MemoryStream& stream);

			// 顶点索引
			static bool LoadIndexPackages(MemoryStream& stream, vector<IndexPackage*>& vecIndexPackage, float fVersion = S3MB_VERSIONV3_0_1);
			static bool SaveIndexPackages(vector<IndexPackage*>& vecIndexPackage, MemoryStream& stream, float fVersion = S3MB_VERSIONV3_0_1);
	};
}

#endif
