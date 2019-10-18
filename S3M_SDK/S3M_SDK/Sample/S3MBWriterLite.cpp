// S3MBWriterLite.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include "Base3D/UGRenderOperationGroup.h"
#include "FileParser/UGFileParse.h"
#include "FileParser/UGFileParseManager.h"
#include "FileParser/UGFileParseModel.h"
#include "Sample.h"

int main()
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
	std::vector<float> m_Vers(verts,verts+sizeof(verts)/sizeof(verts[0]));
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
	UGString strID = L"骨架1";
	int mtlID = 1;
	UGRenderOperationGroup* pGroup = new UGRenderOperationGroup;
	UGRenderOperationGeode* pGeode = new UGRenderOperationGeode;
	//初始化骨架信息
	struct MeshParamInfo info = {strID,mtlID,m_Vers,m_Indexes,m_Normals,m_UVs};
	//创建Geometry
	UGRenderOperationGeometry* pGeoemetry = Tools::CreateGeometry(info);
	UGString texturePath = L"D:\\5.png";
	//创建纹理
	UGTextureData* pTextureData = Tools::CreateTextureData(texturePath);
	UGRenderOperationNode::TextureDataInfo* pTextureDataInfo = new UGRenderOperationNode::TextureDataInfo;
	pTextureDataInfo->m_pTextureData = pTextureData;
	//创建材质
	UGMaterial3D* pMaterial = Tools::CreateMaterial3D(texturePath);


	//建立纹理，材质，骨架，RenderOperationGeoemtry, UGRenderOperationGeode,UGRenderOperationGroup之间的关系
	pGroup->AddTextureData(texturePath, pTextureDataInfo);
	pGroup->AddMaterial3D(pMaterial->m_strName, pMaterial);
	pGeoemetry->m_strMaterialName = pMaterial->m_strName;
	pGeoemetry->m_arrIndexPackage[0]->m_strPassName.Add(pMaterial->m_strName);
	pGroup->AddGeometry(pGeoemetry->m_strGeoName, pGeoemetry);
	pGeode->AddRenderOperationGeoemtry(pGeoemetry);
	pGroup->AddChild(pGeode);

	//输出S3MB文件
	UGString strS3MFilePath;
	strS3MFilePath = L"D:/1.s3mb";
	UGFileParser* pFilePaserS3M = UGFileParseManager::CreateFileParser(UGFileType::S3MB);
	UGExportParams params;
	params.SetFilePathName(strS3MFilePath);
	((UGFileParseModel*)pFilePaserS3M)->Save(params, pGroup);
}