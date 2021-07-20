#pragma once
#include "Shell.h"
using namespace std;
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
		// uv?
		std::vector<float> texCoords;
		// 
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
		PBRParams::AlphaMode alphaMode;
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
		MtlPbrData():name((wchar_t*)L"default"),alphaMode(PBRParams::AlphaMode::PBRAM_OPAQUE),alphaCutOff(0.0f),baseColorFactor(1,1,1,1),
			metallicFactor(0),RoughnessFactor(0),EmissionFactor(0,0,0){}
	} MtlData;
	static void sampleV1(float fVersion);

	static void sampleV2(float fVersion);

	static RenderOperationGroup* CreateROGroupLiteV1(int lodId, wstring texturePath);

	static RenderOperationGroup* CreateROGroupLiteV2(int lodId, bool isV2 = false);

	static void createTextureDataInfo(const wstring&  mtldataPath, RenderOperationGroup* pGroup);
	//! \brief 通过MtlData创建材质
	static Material* CreateMaterial(MtlData* mtldatae);

	static void CreateTextureUnitState(int& iPathFlag, const int& nCoordIdx, int& TextureIndex, int& TextureCoordIndex, Pass* pPass, const wstring& textureDataName, const Matrix4d& texMatrix);
	//! \brief 创建一个模型骨架数据
	static MeshParamInfo CreateMeshData(int lodId);

	static MtlData* CreateMtlData();

	//! \brief 根据读取的顶点信息创建geometry数据
	static Geometry* CreateGeometry(MeshParamInfo& meshInfo, bool isV2 = false);

	//! \brief 创建纹理
	static TextureData* CreateTextureData(wstring texturePath);

	//! \brief 创建材质
	static Material* CreateMaterial(wstring textureDataName);
};
