#pragma once
#include "Base3D/UGRenderOperationGroup.h"
#include "Stream/ugdefs.h"
#include <string>
#include "FileParser/UGFileParseToolkit.h"
using namespace std;
using namespace UGC;
struct MeshParamInfo
{
	UGString strID;
	UGint mtlID;
	std::vector<UGfloat> verts;
	std::vector<UGuint> indexs;
	std::vector<UGfloat> normals;
	std::vector<UGfloat> texCoords;
	std::vector<UGfloat> colors;
};
class Tools
{
public:
	//! \brief 根据读取的顶点信息创建geometry数据
	static UGRenderOperationGeometry* CreateGeometry(MeshParamInfo& meshInfo);
	//! \brief 创建材质
	static UGMaterial3D* CreateMaterial3D(UGString textureDataName);
	//! \brief 创建纹理
	static UGTextureData* CreateTextureData(UGString texturePath);
};

