#if !defined(ROGROUPLITE_H__9359659B_B1D2_43BD_B34A_1B85C669451A__INCLUDED_)
#define ROGROUPLITE_H__9359659B_B1D2_43BD_B34A_1B85C669451A__INCLUDED_

#pragma once
#include "Shell.h"
#include <math.h>
using namespace S3MB;
class ROGroupLite
{
public:
	struct MeshParamInfo
	{
		wstring strID;
		int LodID;
		// 顶点信息
		std::vector<float> verts;
		// 顶点索引信息
		std::vector<unsigned int> indexs;
		// 法线信息
		std::vector<float> normals;
		// uv
		std::vector<float> texCoords;
		// 顶点颜色
		std::vector<float> colors;
	};

	struct TexInfo
	{
		wchar_t* texPath;
		int coordIndex;
		void* otherData;
		Matrix4d texMatrix;
		TexInfo():texPath(nullptr),coordIndex(0),otherData(nullptr), texMatrix(Matrix4d::IDENTITY) {}
	};

	typedef struct MtlPbrData
	{
		wchar_t * name;
		AlphaMode alphaMode;
		float alphaCutOff;
		TexInfo baseTexMap;
		Vector4d baseColorFactor;
		TexInfo MetallicRoughMap;
		float metallicFactor;
		float RoughnessFactor;
		TexInfo OcclusionMap;
		TexInfo EmissionMap;
		Vector3d EmissionFactor;
		TexInfo NormalMap;
        MtlPbrData()
		{
#ifdef WIN32
			name = U("default");
#else
			name = (wchar_t*)U("default").c_str();
#endif
			alphaMode = AlphaMode::PBRAM_OPAQUE;
			alphaCutOff = 0.0f; 
			baseColorFactor.x = 1;
			baseColorFactor.y = 1;
			baseColorFactor.z = 1;
			baseColorFactor.w = 1;
			metallicFactor = 0.0; 
			RoughnessFactor = 0;
			EmissionFactor.x = 0;
			EmissionFactor.y = 0;
			EmissionFactor.z = 0;
		}
	} MtlData;

	static void sampleV1();

	static void sampleV2();

	static RenderOperationGroup* CreateROGroupLiteV1(int lodId, wstring texturePath);

	static RenderOperationGroup* CreateROGroupLiteV2(int lodId);

	static void createTextureDataInfo(const wstring&  mtldataPath, RenderOperationGroup* pGroup);
	// 通过MtlData创建材质
	static Material* CreateMaterial(MtlData* mtldatae);

	static void CreateTextureUnitState(int& iPathFlag, const int& nCoordIdx, int& TextureIndex, int& TextureCoordIndex, Pass* pPass, const wstring& textureDataName, const Matrix4d& texMatrix);
	// 创建一个模型骨架数据
	static MeshParamInfo CreateMeshData(int lodId);

	static MtlData* CreateMtlData();

	// 根据读取的顶点信息创建Skeleton数据
	static Skeleton* CreateSkeleton(MeshParamInfo& meshInfo, bool hasPBR = false);

	// 创建纹理
	static TextureData* CreateTextureData(wstring texturePath);

	// 创建材质
	static Material* CreateMaterial(wstring textureDataName);
};

#endif
