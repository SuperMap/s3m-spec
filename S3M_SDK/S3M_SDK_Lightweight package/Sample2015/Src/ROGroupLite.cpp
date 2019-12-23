#include "ROGroupLite.h"
#include "ImageOperator.h"
#include <processenv.h>

RenderOperationGroup* ROGroupLite::CreateROGroupLite()
{
	MeshParamInfo info = CreateMeshData();
	Geometry* pGeometry = CreateGeometry(info);
	string texturePath = "./1.png";
	//创建纹理
	TextureData* pTextureData = CreateTextureData(texturePath);
	TextureDataInfo* pTextureDataInfo = new TextureDataInfo;
	pTextureDataInfo->m_pTextureData = pTextureData;
	//创建材质
	Material* pMaterial = CreateMaterial(texturePath);

	RenderOperationGroup* pGroup = new RenderOperationGroup;
	RenderOperationGeode* pGeode = new RenderOperationGeode;

	//建立纹理，材质，骨架，RenderOperationGeoemtry, UGRenderOperationGeode,UGRenderOperationGroup之间的关系
	pGroup->AddTextureData(texturePath, pTextureDataInfo);
	pGroup->AddMaterial(pMaterial->m_strName, pMaterial);
	pGeometry->m_strMaterialName = pMaterial->m_strName;
	pGeometry->m_arrIndexPackage[0]->m_strPassName.push_back(pMaterial->m_strName);
	pGroup->AddGeometry(pGeometry->m_strGeoName, pGeometry);
	pGeode->AddGeometry(pGeometry);
	pGroup->AddChild(pGeode);
	return pGroup;
}

ROGroupLite::MeshParamInfo ROGroupLite::CreateMeshData()
{
	//顶点信息
	float verts[24] =
	{
		0.1234f, 0.1234f, 0.1234f,   //第0个顶点
		20.1234f, 0.1234f, 0.1234f,  //第1个顶点
		20.1234f, 0.1234f, 20.1234f, //第2个顶点
		0.1234f, 0.1234f, 20.1234f,  //第3个顶点
		0.1234f, 20.1234f, 0.1234f,  //第4个顶点
		20.1234f, 20.1234f, 0.1234f, //第5个顶点
		20.1234f, 20.1234f, 20.1234f,//第6个顶点
		0.1234f, 20.1234f, 20.1234f  //第7个顶点
	};
	std::vector<float> m_Vers(verts, verts + sizeof(verts) / sizeof(verts[0]));
	//顶点索引信息
	unsigned int VertsIndex[36] =
	{
		0, 1, 2,//前方
		0, 2, 3,
		4, 7, 6,//后方
		4, 6, 5,
		4, 0, 3,//左侧
		4, 3, 7,
		1, 5, 6,//右侧
		1, 6, 2,
		3, 2, 6,//顶部
		3, 6, 7,
		0, 4, 1,//底部
		1, 4, 5
	};
	std::vector<unsigned int> m_Indexes(VertsIndex, VertsIndex + sizeof(VertsIndex) / sizeof(VertsIndex[0]));
	//法线信息
	float normals[108] =
	{
		0, -1, 0, 0, -1, 0, 0, -1, 0,
			0, -1, 0, 0, -1, 0, 0, -1, 0,//前方
			0, 1, 0, 0, 1, 0, 0, 1, 0,
			0, 1, 0, 0, 1, 0, 0, 1, 0,//后方
			-1, 0, 0, -1, 0, 0, -1, 0, 0,
			-1, 0, 0, -1, 0, 0, -1, 0, 0,//左侧
			1, 0, 0, 1, 0, 0, 1, 0, 0,
			1, 0, 0, 1, 0, 0, 1, 0, 0,//右侧
			0, 0, 1, 0, 0, 1, 0, 0, 1,
			0, 0, 1, 0, 0, 1, 0, 0, 1,//顶部
			0, 0, -1, 0, 0, -1, 0, 0, -1,
			0, 0, -1, 0, 0, -1, 0, 0, -1//底部
	};
	std::vector<float> m_Normals(normals, normals + sizeof(normals) / sizeof(normals[0]));
	//UV信息
	float uvs[72] =
	{
		0, 0, 2, 0, 2, 2,
		0, 0, 2, 2, 0, 2,
		0, 0, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 1,
		0, 0, 1, 0, 1, 1,
		0, 0, 1, 1, 0, 1
	};
	std::vector<float> m_UVs(uvs, uvs + sizeof(uvs) / sizeof(uvs[0]));
	string strID = "骨架1";
	int mtlID = 1;
	//初始化骨架信息
	struct MeshParamInfo info = { strID,mtlID,m_Vers,m_Indexes,m_Normals,m_UVs };
	return info;
}

Geometry* ROGroupLite::CreateGeometry(MeshParamInfo& meshInfo)
{
	Geometry* pGeometry = new Geometry;
	string strName = to_string((size_t)meshInfo.mtlID);
	//骨架名字，唯一
	pGeometry->m_strGeoName = meshInfo.strID + strName;
	pGeometry->m_pVertexDataPackage = new VertexDataPackage;
	pGeometry->m_pVertexDataPackage->m_nVertexDimension = 3;
	pGeometry->m_pVertexDataPackage->SetVertexNum(meshInfo.verts.size() / 3);
	float* pVertices = pGeometry->m_pVertexDataPackage->m_pVertices;
	int vertPoint = 0;
	for (; vertPoint < meshInfo.verts.size(); vertPoint++, pVertices++)
	{
		*pVertices = meshInfo.verts[vertPoint];
	}

	//uv
	{
		pGeometry->m_pVertexDataPackage->m_TexCoordCount[0] = meshInfo.texCoords.size() / 2;
		pGeometry->m_pVertexDataPackage->m_pTexCoords[0] = new float[meshInfo.texCoords.size()];
		float* pTexCoords = pGeometry->m_pVertexDataPackage->m_pTexCoords[0];
		//赋值UV
		//pTexCoords = NULL;
		for (int i = 0; i < meshInfo.texCoords.size(); i++, pTexCoords++)
		{
			*pTexCoords = meshInfo.texCoords[i];
		}
	}

	IndexPackage* pIndexPackage = new IndexPackage;

	//判断是否需要纹理索引
	bool bHasTexIndex = meshInfo.verts.size() / 3 != meshInfo.texCoords.size() / 2 ;
	if (meshInfo.verts.size() / 3 < 65536)
	{
		if (bHasTexIndex)
		{
			pIndexPackage->m_enIndexType = IT_16BIT_2;
			pIndexPackage->SetIndexNum(meshInfo.indexs.size() * 2);
		}
		else
		{
			pIndexPackage->m_enIndexType = IT_16BIT;
			pIndexPackage->SetIndexNum(meshInfo.indexs.size());
		}
		unsigned short* pIndexes = pIndexPackage->m_pIndexes;
		int indexPoint = 0;
		for (; indexPoint < meshInfo.indexs.size(); indexPoint++, pIndexes++)
		{
			*pIndexes = meshInfo.indexs[indexPoint];
		}
		if (bHasTexIndex)
		{
			for (; indexPoint < meshInfo.indexs.size() * 2; indexPoint++, pIndexes++)
			{
				*pIndexes = indexPoint - meshInfo.indexs.size();
			}
		}
		pGeometry->m_arrIndexPackage.push_back(pIndexPackage);
	}
	else if (meshInfo.verts.size() / 3 >= 65536)
	{
		if (bHasTexIndex)
		{
			pIndexPackage->m_enIndexType = IT_32BIT_2;
			pIndexPackage->SetIndexNum(meshInfo.indexs.size() * 2);
		}
		else
		{
			pIndexPackage->m_enIndexType = IT_32BIT;
			pIndexPackage->SetIndexNum(meshInfo.indexs.size());
		}
		unsigned int* pIndexes = new unsigned int[meshInfo.indexs.size()];
		pIndexPackage->m_pIndexes = (unsigned short*)pIndexes;
		int indexPoint = 0;
		if (bHasTexIndex)
		{
			for (; indexPoint < meshInfo.indexs.size(); indexPoint++, pIndexes++)
			{
				*pIndexes = meshInfo.indexs[indexPoint];
			}
		}
		pGeometry->m_arrIndexPackage.push_back(pIndexPackage);
	}

	if (meshInfo.colors.size() > 0)
	{
		//顶点颜色
		/*pGeometry->m_pVertexDataPackage->SetColorNum(meshInfo.colors.size());
		UGint colorPoint = 0;
		UGuint* pColor = pGeometry->m_pVertexDataPackage->m_pVertexColor;
		for (; colorPoint < meshInfo.colors.size(); colorPoint++, pColor++)
		{
			*pColor = meshInfo.colors[colorPoint];
		}*/
	}

	if (pGeometry->m_pVertexDataPackage->m_nVerticesCount > 0 && pGeometry->m_pVertexDataPackage->m_pVertices != NULL)
	{
		////赋值法线
		//pGeometry->m_pVertexDataPackage->m_pNormals = NULL;
		//pGeometry->m_pVertexDataPackage->m_nNormalCount = pGeometry->m_pVertexDataPackage->m_nVerticesCount;//顶点个数等于法向量个数
	}
	pGeometry->ComputerBoundingBox();
	return pGeometry;
}

TextureData* ROGroupLite::CreateTextureData(string texturePath)
{
	if (texturePath.find(':')==string::npos)
	{
		char buffer[200];
		GetCurrentDirectoryA(200, buffer);
		string strPath(buffer);
		texturePath = StringHelper::GetAbsolutePath(texturePath);
	}
	TextureData* m_pTextureData = ImgToolkit::GetTextureData(texturePath);
	return m_pTextureData;
}

Material* ROGroupLite::CreateMaterial(string textureDataName)
{
	Material* pMaterial3D = new Material;
	string strMtlName = "sample";
	Technique* pTech = pMaterial3D->CreateTechnique();
	Pass* pPass = pTech->CreatePass();

	pPass->SetPFFMode((PolygonFrontFace)PFF_NONE);
	strMtlName += textureDataName;

	TextureUnitState* pTexUnit = pPass->CreateTextureUnitState();
	//材质对象通过贴图对象的名称关联贴图对象
	pTexUnit->m_strTextureName = textureDataName;
	//材质对象名称
	pMaterial3D->m_strName = strMtlName;
	//设置通道名称
	pPass->m_strName = strMtlName;

	// 材质颜色
	//{
	//	UGfloat fParency;
	//	Color diffuseColor;
	//	pPass->m_Diffuse.SetValue(diffuseColor.r, diffuseColor.g, diffuseColor.b, 1.0 - fParency);

	//	Color ambientColor;

	//	pPass->m_Ambient.SetValue(ambientColor.r, ambientColor.g, ambientColor.b, 1.0 - fParency);

	//	Color specularColor;
	//	pPass->m_Specular.SetValue(specularColor.r, specularColor.g, specularColor.b, 1.0 - fParency);

	//	Color emmissiveColor;
	//	pPass->m_SelfIllumination.SetValue(emmissiveColor.r, emmissiveColor.g, emmissiveColor.b, 1.0 - fParency);

	//	pPass->m_Shininess;
	//}
	return pMaterial3D;
}



