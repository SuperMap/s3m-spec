#include "ROGroupLite.h"
#include "ImageOperator.h"
#include <processenv.h>
#include <S3MBTools.h>
#include "S3MBReader.h"
#include "S3MBWriter.h"
#include "S3MBSCP.h"
#include "S3MBAttributeReader.h"

void ROGroupLite::sampleV1(float fVersion)
{
	///************************************************************************/
	//*          1.创建一个立方体数据并写入S3MB文件
	///************************************************************************/
	RenderOperationGroup* pGroup;
	//输入多个数据来构成PBR材质的多个纹理

	pGroup = ROGroupLite::CreateROGroupLiteV1(1, L"./1.png");

	S3MBWriter m_S3MBWriter;
	pGroup->ReComputeBoundingBox();//当包围盒/球不对或无包围盒/球时可调用
	m_S3MBWriter.SetROGroup(pGroup);
	m_S3MBWriter.SetIsChangeTexture(true, true);//DXT压缩
	wstring strS3mbFile = L"./cube1.s3mb";
	m_S3MBWriter.SetFilePath(strS3mbFile);
	m_S3MBWriter.SetVersion(fVersion);
	m_S3MBWriter.Save(0);
	m_S3MBWriter.Clear();
	delete pGroup;

	pGroup = ROGroupLite::CreateROGroupLiteV1(2, L"./3.jpg");
	pGroup->ReComputeBoundingBox();
	m_S3MBWriter.SetROGroup(pGroup);
	m_S3MBWriter.SetIsChangeTexture(true, true);//DXT压缩
	strS3mbFile = L"./cube2.s3mb";
	m_S3MBWriter.SetFilePath(strS3mbFile);
	m_S3MBWriter.Save(0);
	delete pGroup;
	m_S3MBWriter.Clear();

	pGroup = ROGroupLite::CreateROGroupLiteV1(3, L"./5.jpg");
	pGroup->ReComputeBoundingBox();
	m_S3MBWriter.SetROGroup(pGroup);
	m_S3MBWriter.SetIsChangeTexture(true, true);//DXT压缩
	strS3mbFile = L"./cube3.s3mb";
	m_S3MBWriter.SetFilePath(strS3mbFile);
	//m_S3MBWriter.Save(0);
	m_S3MBWriter.Save(0);
	m_S3MBWriter.Clear();
	///************************************************************************/
	//*        2:通过指定的s3mb文件，读取出来Ro信息，并进行输出                                                                     */
	///************************************************************************/


	S3MBReader m_S3MBReaderRo;
	wstring strS3MBFileSrc = L"./cube1.s3mb";
	m_S3MBReaderRo.SetFilePath(strS3MBFileSrc);
	m_S3MBReaderRo.ReadS3MBAsROGroup();
	pGroup = m_S3MBReaderRo.GetRenderOperationGroup();

	///************************************************************************/
	//*       3:通过指定的s3mb缓存文件夹中json文件，遍历读取s3md属性文件并输出属性字段及其对应值                                                                */
	///************************************************************************/

	wstring strS3MBFolderSrc = L"../../../SampleData/attribute/attribute.json";

	S3MBAttributeReader::ReadAttribute(strS3MBFolderSrc);


	//////////////////////生成SCP文件//////////////////
	//插入点
	Point3D m_Position(118, 39, 0);
	Rect2D geoBounds(0, 0, 0, 0);
	int epsgCode = 4326;
	bool isDegree = true;

	BoundingBox m_Box;
	m_Box.SetNULL();
	for (int i = 0; i < pGroup->GetNumChildren(); i++)
	{
		if (NULL != pGroup->GetChild(i)->AsGeode())
		{
			m_Box.Merge(((RenderOperationGeode*)(pGroup->GetChild(i)))->GetBoundingBox());
		}
	}
	///生成SCP文件//////////////////
	S3MBSCP* pS3MBSCP = new S3MBSCP();
	pS3MBSCP->m_fVersion = fVersion;
	pS3MBSCP->m_strAsset = L"SuperMap";

	pS3MBSCP->m_pntPosition = m_Position;
	pS3MBSCP->m_rcGeoBounds = geoBounds;
	pS3MBSCP->m_prjCoordEPSG = epsgCode;
	pS3MBSCP->m_bIsDegree = isDegree;
	pS3MBSCP->m_dbHeightMax = m_Box.GetMax().z + m_Position.z;
	pS3MBSCP->m_dbHeightMin = m_Box.GetMin().z + m_Position.z;
	//pS3MBSCP->m_enDataType = SDT_ObliquePhoto;

	pS3MBSCP->m_mapExtensions[L"s3m:FileType"] = L"OSGBCacheFile";
	pS3MBSCP->m_mapExtensions[L"s3m:RenderMode"] = L"Normal";
	pS3MBSCP->m_mapExtensions[L"s3m:TileSplitType"] = L"LOCAL";

	std::vector<wstring> vecRootFiles;//根结点
	std::vector<BoundingBox> vecRootBoundboxs;//根结点对应BoundingBox
	vecRootFiles.push_back(L"cube1.s3mb");
	vecRootBoundboxs.push_back(m_Box);

	pS3MBSCP->SetTiles(vecRootFiles, vecRootBoundboxs);
	wstring strOutputFilePath = StringHelper::GetAbsolutePath(strS3MBFileSrc);
	wstring strFilePatch = StringHelper::GetAbsolutePath(strOutputFilePath, L"../sampleV1.scp");
	pS3MBSCP->SaveToJsonFile(strFilePatch);
	delete pS3MBSCP;
	pS3MBSCP = NULL;
}

void ROGroupLite::sampleV2(float fVersion)
{	
	///************************************************************************/
	//*          1.创建一个立方体数据并写入S3MB文件
	///************************************************************************/
	
	RenderOperationGroup* pGroup;
	pGroup = ROGroupLite::CreateROGroupLiteV2(1,true);
	S3MBWriter m_S3MBWriter;
	pGroup->ReComputeBoundingBox();//当包围盒/球不对或无包围盒/球时可调用
	m_S3MBWriter.SetROGroup(pGroup);
	m_S3MBWriter.SetIsChangeTexture(true, true);//DXT压缩
	wstring strS3mbFileWrite = L"./sampleV2.s3mb";
	m_S3MBWriter.SetFilePath(strS3mbFileWrite);
	m_S3MBWriter.SetVersion(fVersion);//设置版本信息
	m_S3MBWriter.Save(0);
	m_S3MBWriter.Clear();


	
	///************************************************************************/
	//*        2:通过指定的s3mb文件，读取出来Ro信息，并进行输出                                                                     */
	///************************************************************************/
	S3MBReader m_S3MBReaderRo;
	wstring strS3MBFileRead = L"./sampleV2.s3mb";
	m_S3MBReaderRo.SetFilePath(strS3MBFileRead);
	m_S3MBReaderRo.ReadS3MBAsROGroup();
	pGroup = m_S3MBReaderRo.GetRenderOperationGroup();
	m_S3MBReaderRo.Clear();


	Point3D m_Position(118, 39, 0);
	Rect2D geoBounds(0, 0, 0, 0);
	int epsgCode = 4326;
	bool isDegree = true;

	BoundingBox m_Box;
	m_Box.SetNULL();
	for (int i = 0; i < pGroup->GetNumChildren(); i++)
	{
		if (NULL != pGroup->GetChild(i)->AsGeode())
		{
			m_Box.Merge(((RenderOperationGeode*)(pGroup->GetChild(i)))->GetBoundingBox());
		}
	}
	///生成SCP文件//////////////////
	S3MBSCP* pS3MBSCP = new S3MBSCP();
	pS3MBSCP->m_fVersion = fVersion;
	pS3MBSCP->m_strAsset = L"SuperMap";

	pS3MBSCP->m_pntPosition = m_Position;
	pS3MBSCP->m_rcGeoBounds = geoBounds;
	pS3MBSCP->m_prjCoordEPSG = epsgCode;
	pS3MBSCP->m_bIsDegree = isDegree;
	pS3MBSCP->m_dbHeightMax = m_Box.GetMax().z + m_Position.z;
	pS3MBSCP->m_dbHeightMin = m_Box.GetMin().z + m_Position.z;
	//pS3MBSCP->m_enDataType = SDT_ObliquePhoto;

	pS3MBSCP->m_mapExtensions[L"s3m:FileType"] = L"OSGBCacheFile";
	pS3MBSCP->m_mapExtensions[L"s3m:RenderMode"] = L"Normal";
	pS3MBSCP->m_mapExtensions[L"s3m:TileSplitType"] = L"LOCAL";

	std::vector<wstring> vecRootFiles;//根结点
	std::vector<BoundingBox> vecRootBoundboxs;//根结点对应BoundingBox
	vecRootFiles.push_back(L"sampleV2.s3mb");
	vecRootBoundboxs.push_back(m_Box);

	pS3MBSCP->SetTiles(vecRootFiles, vecRootBoundboxs);
	wstring strOutputFilePath = StringHelper::GetAbsolutePath(strS3mbFileWrite);
	wstring strFilePatch = StringHelper::GetAbsolutePath(strOutputFilePath, L"../sampleV2.scp");
	pS3MBSCP->SaveToJsonFile(strFilePatch);
	delete pS3MBSCP;
	pS3MBSCP = NULL;
}

RenderOperationGroup* ROGroupLite::CreateROGroupLiteV1(int lodId, wstring texturePath)
{
	MeshParamInfo info = CreateMeshData(lodId);
	Geometry* pGeometry = CreateGeometry(info);

	//创建纹理
	TextureData* pTextureData = CreateTextureData(texturePath);
	TextureDataInfo* pTextureDataInfo = new TextureDataInfo;
	pTextureDataInfo->m_pTextureData = pTextureData;
	//创建材质
	Material* pMaterial = CreateMaterial(texturePath);

	RenderOperationGroup* pGroup = new RenderOperationGroup;
	RenderOperationPagedLOD* pPagedLOD = new RenderOperationPagedLOD;
	RenderOperationGeode* pGeode = new RenderOperationGeode;

	//建立纹理，材质，骨架，RenderOperationGeoemtry, UGRenderOperationGeode,UGRenderOperationGroup之间的关系
	pGroup->AddTextureData(texturePath, pTextureDataInfo);
	pGroup->AddMaterial(pMaterial->m_strName, pMaterial);
	pGeometry->m_strMaterialName = pMaterial->m_strName;
	pGeometry->m_arrIndexPackage[0]->m_strPassName.push_back(pMaterial->m_strName);
	pGroup->AddGeometry(pGeometry->m_strGeoName, pGeometry);
	pGeode->AddGeometry(pGeometry);
	pGroup->AddChild(pPagedLOD);
	pPagedLOD->AddChild(pGeode);

	pGeode->ComputerBoundingBox();
	BoundingBox bboxObj = pGeode->GetBoundingBox();
	BoundingSphere bSphere(bboxObj);
	pGeode->SetBoudingSphere(bSphere.GetCenter(), bSphere.GetRadius());

	//对应的切换文件,目前支持一个pagedLod只挂一个模型，如果下挂多个文件，则创建多个pagedLod,每个挂一个文件并设置切换距离。
	if (lodId < 3)
	{
		wstring strLod = to_wstring(lodId + 1);
		wstring strLodFile = L"cube" + strLod + L".s3mb";
		pPagedLOD->GetFileNames().push_back(strLodFile);
	}
	//Lod切换模式及切换距离可以自己控制。
	pPagedLOD->SetRangeMode(RangeMode::DISTANCE_FROM_EYE_POINT);
	// 计算LOD变换的距离
	double nLodDis = MIN(1024, bSphere.m_radius * pow(2.0, 3 - lodId));
	pPagedLOD->GetRanges().push_back(std::vector<std::pair<float, float> >::value_type(0, nLodDis));
	pPagedLOD->SetBoudingSphere(bSphere.m_center, bSphere.m_radius);

	return pGroup;
}
void ROGroupLite::createTextureDataInfo(const wstring&  mtldataPath, RenderOperationGroup* pGroup)
{
	wstring texturePath = L"";
	// 创建纹理
	TextureData* pTextureData;
	TextureDataInfo* pTextureDataInfo = new TextureDataInfo;
	texturePath = mtldataPath;
	pTextureData = CreateTextureData(texturePath);
	pTextureDataInfo->m_pTextureData = pTextureData;
	pGroup->AddTextureData(texturePath, pTextureDataInfo);
}
RenderOperationGroup* ROGroupLite::CreateROGroupLiteV2(int lodId, bool isV2)
{
	// 创建骨架
	MeshParamInfo info = CreateMeshData(lodId);
	Geometry* pGeometry = CreateGeometry(info,isV2);

	// 创建材质
	MtlData* mtldata = CreateMtlData();
	Material* pMaterial = CreateMaterial(mtldata);

	RenderOperationGroup* pGroup = new RenderOperationGroup;
	RenderOperationPagedLOD* pPagedLOD = new RenderOperationPagedLOD;
	RenderOperationGeode* pGeode = new RenderOperationGeode;

	
	wstring texturePath = L"";
	if (mtldata->baseTexMap.texPath != nullptr)
	{	
		// 创建纹理
		createTextureDataInfo(mtldata->baseTexMap.texPath, pGroup);
	}
	if (mtldata->EmissionMap.texPath != nullptr)
	{	
		// 创建纹理
		createTextureDataInfo(mtldata->EmissionMap.texPath, pGroup);
	}
	if (mtldata->MetallicRoughMap.texPath != nullptr)
	{	
		// 创建纹理
		createTextureDataInfo(mtldata->MetallicRoughMap.texPath, pGroup);
	}
	if (mtldata->NormalMap.texPath != nullptr)
	{	
		// 创建纹理
		createTextureDataInfo(mtldata->NormalMap.texPath, pGroup);
	}
	if (mtldata->OcclusionMap.texPath != nullptr)
	{	
		// 创建纹理
		createTextureDataInfo(mtldata->OcclusionMap.texPath, pGroup);
	}
	// 建立纹理，材质，骨架，RenderOperationGeoemtry, UGRenderOperationGeode,UGRenderOperationGroup之间的关系
	
	//pGroup->AddTextureData(texturePath, pTextureDataInfo);
	pGroup->AddMaterial(pMaterial->m_strName, pMaterial);
	pGeometry->m_strMaterialName = pMaterial->m_strName;
	pGeometry->m_arrIndexPackage[0]->m_strPassName.push_back(pMaterial->m_strName);
	pGroup->AddGeometry(pGeometry->m_strGeoName, pGeometry);
	pGeode->AddGeometry(pGeometry);
	pGroup->AddChild(pPagedLOD);
	pPagedLOD->AddChild(pGeode);

	pGeode->ComputerBoundingBox();
	BoundingBox bboxObj = pGeode->GetBoundingBox();
	BoundingSphere bSphere(bboxObj);
	pGeode->SetBoudingSphere(bSphere.GetCenter(), bSphere.GetRadius());

	//// 对应的切换文件,目前支持一个pagedLod只挂一个模型，如果下挂多个文件，则创建多个pagedLod,每个挂一个文件并设置切换距离。
	//if (lodId < 3)
	//{
	//	wstring strLod = to_wstring(lodId + 1);
	//	wstring strLodFile = L"立方体" + strLod + L".s3mb";
	//	pPagedLOD->GetFileNames().push_back(strLodFile);
	//}
	// Lod切换模式及切换距离可以自己控制。
	pPagedLOD->SetRangeMode(RangeMode::DISTANCE_FROM_EYE_POINT);
	// 计算LOD变换的距离
	//double nLodDis = MIN(1024, bSphere.m_radius * pow(2.0, 3 - lodId));
	//pPagedLOD->GetRanges().push_back(std::vector<std::pair<float, float> >::value_type(0, nLodDis));
	pPagedLOD->SetBoudingSphere(bSphere.m_center, bSphere.m_radius);

	return pGroup;
}

ROGroupLite::MtlData* ROGroupLite::CreateMtlData()
{
	MtlData* mtlData = new MtlData;
	mtlData->baseTexMap.coordIndex = 0;
	mtlData->baseTexMap.texPath = (wchar_t*)L"./0.jpg";
	mtlData->NormalMap.texPath = (wchar_t*)L"./NormalMap.jpg";
	mtlData->NormalMap.coordIndex = 0;
	return mtlData;
}

void ROGroupLite::CreateTextureUnitState(int& iPathFlag, const int& nCoordIdx, int& TextureIndex, int& TextureCoordIndex, Pass* pPass, const wstring& textureDataName, const Matrix4d& texMatrix )
{
	TextureIndex = iPathFlag++;
	TextureCoordIndex = nCoordIdx;
	TextureUnitState* pTexUnit = pPass->CreateTextureUnitState();
	// 材质对象通过贴图对象的名称关联贴图对象
	pTexUnit->m_strTextureName = textureDataName;
	pTexUnit->m_TexModMatrix = texMatrix;
}

Material* ROGroupLite::CreateMaterial(MtlData* mtldata)
{
	Material* pMaterial3D = new Material;
	wstring strMtlName = L"材质";
	Technique* pTech = pMaterial3D->CreateTechnique();
	Pass* pPass = pTech->CreatePass();

	pPass->SetPFFMode((PolygonFrontFace)PFF_NONE);

	pPass->m_pPRBMaterial = new PBRMetallicRough;
	PBRMetallicRough* pPbr = (PBRMetallicRough*)pPass->m_pPRBMaterial;
	wstring textureDataName = L"";

	int nPathFlag = 0;
	if (mtldata->baseTexMap.texPath != nullptr)
	{	
		textureDataName = mtldata->baseTexMap.texPath;
		CreateTextureUnitState(nPathFlag, mtldata->baseTexMap.coordIndex, pPbr->m_nBaseColorTextureIndex, pPbr->m_nBaseColorTextureCoordIndex, 
			pPass, textureDataName, mtldata->baseTexMap.texMatrix);
	}
	if (mtldata->EmissionMap.texPath != nullptr)
	{	
		textureDataName = mtldata->EmissionMap.texPath;
		CreateTextureUnitState(nPathFlag, mtldata->EmissionMap.coordIndex, pPbr->m_nEmissiveTextureIndex, pPbr->m_nEmissiveTextureCoordIndex,
			pPass,textureDataName, mtldata->EmissionMap.texMatrix);
	}
	if (mtldata->MetallicRoughMap.texPath != nullptr)
	{	
		textureDataName = mtldata->MetallicRoughMap.texPath;
		CreateTextureUnitState(nPathFlag, mtldata->MetallicRoughMap.coordIndex, pPbr->m_nMetallicRoughnessTextureIndex, pPbr->m_nMetallicRoughnessTextureCoordIndex,
			pPass,textureDataName, mtldata->MetallicRoughMap.texMatrix);
	}
	if (mtldata->NormalMap.texPath != nullptr)
	{	
		textureDataName = mtldata->NormalMap.texPath;
		CreateTextureUnitState(nPathFlag, mtldata->NormalMap.coordIndex, pPbr->m_nNormalTextureIndex, pPbr->m_nNormalTextureCoordIndex,
			pPass, textureDataName, mtldata->NormalMap.texMatrix);
	}
	if (mtldata->OcclusionMap.texPath != nullptr)
	{	
		textureDataName = mtldata->OcclusionMap.texPath;
		CreateTextureUnitState(nPathFlag, mtldata->OcclusionMap.coordIndex, pPbr->m_nOcclusionTextureIndex, pPbr->m_nOcclusionTextureCoordIndex,
			pPass, textureDataName, mtldata->OcclusionMap.texMatrix);
	}
	strMtlName += textureDataName;
	// 材质对象名称
	pMaterial3D->m_strName = strMtlName;
	// 设置通道名称
	pPass->m_strName = strMtlName;

	pPass->m_fAlphaCutoff = mtldata->alphaCutOff;
	pPass->m_AlphaMode = mtldata->alphaMode;
	pPbr->m_vec4BaseColor =  mtldata->baseColorFactor;
	pPbr->m_vec3EmissiveFactor = mtldata->EmissionFactor;
	pPbr->m_fMetallicFactor =  mtldata->metallicFactor;
	pPbr->m_fRoughnessFactor = mtldata->RoughnessFactor;
	// 材质颜色
	//{
	//	UGfloat fParency;
	//	Color diffuseColor;
	//	pPass->m_Diffuse.SetValue(diffuseColor.r, diffuseColor.g, diffuseColor.b, 1.0 - fParency);

	return pMaterial3D;
}

ROGroupLite::MeshParamInfo ROGroupLite::CreateMeshData(int lodId)
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
	wstring strID = L"骨架1";
	//初始化骨架信息
	struct MeshParamInfo info = { strID,lodId,m_Vers,m_Indexes,m_Normals,m_UVs };
	return info;
}

Geometry* ROGroupLite::CreateGeometry(MeshParamInfo& meshInfo, bool isV2)
{
	Geometry* pGeometry = new Geometry;
	wstring strName = to_wstring((size_t)meshInfo.LodID);
	//骨架名字，唯一
	pGeometry->m_strGeoName = meshInfo.strID + strName;
	pGeometry->m_pVertexDataPackage = new VertexDataPackage;
	pGeometry->m_pVertexDataPackage->m_nVertexDimension = 3;
#pragma region 闭合
	//pGeometry->m_pVertexDataPackage->SetVertexNum(meshInfo.verts.size() / 3);
	//float* pVertices = pGeometry->m_pVertexDataPackage->m_pVertices;
	//int vertPoint = 0;
	//for (; vertPoint < meshInfo.verts.size(); vertPoint++, pVertices++)
	//{
	//	*pVertices = meshInfo.verts[vertPoint];
	//}
#pragma endregion

#pragma region 不闭合
	pGeometry->m_pVertexDataPackage->SetVertexNum(meshInfo.indexs.size());
	float* pVertices = pGeometry->m_pVertexDataPackage->m_pVertices;
	int i = 0;
	for (; i < meshInfo.indexs.size(); i++)
	{
		pVertices[3 * i] = meshInfo.verts[meshInfo.indexs[i] * 3];
		pVertices[3 * i + 1] = meshInfo.verts[meshInfo.indexs[i] * 3 + 1];
		pVertices[3 * i + 2] = meshInfo.verts[meshInfo.indexs[i] * 3 + 2];
	}
#pragma endregion


	//uv
	{
		pGeometry->m_pVertexDataPackage->m_TexCoordCount[0] = meshInfo.texCoords.size() / 2;
		pGeometry->m_pVertexDataPackage->m_pTexCoords[0] = new float[meshInfo.texCoords.size()];
		pGeometry->m_pVertexDataPackage->m_nTexDimensions[0] = 2;
		float* pTexCoords = pGeometry->m_pVertexDataPackage->m_pTexCoords[0];
		//赋值UV
		//pTexCoords = NULL;
		for (int i = 0; i < meshInfo.texCoords.size() / 2; i++)
		{
			pTexCoords[2 * i] = meshInfo.texCoords[2 * i];
			pTexCoords[2 * i + 1] = meshInfo.texCoords[2 * i + 1];
		}
	}

	IndexPackage* pIndexPackage = new IndexPackage;

	if (meshInfo.verts.size()/3 < 65536)
	{

		pIndexPackage->m_enIndexType = IT_16BIT;
		pIndexPackage->SetIndexNum(meshInfo.indexs.size());

		unsigned short* pIndexes = pIndexPackage->m_pIndexes;
		int indexPoint = 0;
		for (; indexPoint < meshInfo.indexs.size(); indexPoint++, pIndexes++)
		{
			//*pIndexes = meshInfo.indexs[indexPoint];
			*pIndexes = indexPoint;
		}
		pGeometry->m_arrIndexPackage.push_back(pIndexPackage);
	}
	else
	{
		
		pIndexPackage->m_enIndexType = IT_32BIT;
		pIndexPackage->SetIndexNum(meshInfo.indexs.size());
		
		unsigned int* pIndexes = (unsigned int*)pIndexPackage->m_pIndexes;
		int indexPoint = 0;
		for (; indexPoint < meshInfo.indexs.size(); indexPoint++, pIndexes++)
		{
			//*pIndexes = meshInfo.indexs[indexPoint];
			*pIndexes = indexPoint;
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

	if (pGeometry->m_pVertexDataPackage->m_nVerticesCount > 0 && pGeometry->m_pVertexDataPackage->m_pVertices != NULL && meshInfo.normals.size() > 0)
	{
		////赋值法线
		//pGeometry->m_pVertexDataPackage->m_pNormals = NULL;
		pGeometry->m_pVertexDataPackage->m_nNormalCount = pGeometry->m_pVertexDataPackage->m_nVerticesCount;//顶点个数等于法向量个数
		float* pNormals = new float[pGeometry->m_pVertexDataPackage->m_nNormalCount * 3];
		pGeometry->m_pVertexDataPackage->m_pNormals = pNormals;
		for (int i = 0; i < pGeometry->m_pVertexDataPackage->m_nNormalCount; i++)
		{
			pNormals[3 * i] = meshInfo.normals[3 * i];
			pNormals[3 * i + 1] = meshInfo.normals[3 * i + 1];
			pNormals[3 * i + 2] = meshInfo.normals[3 * i + 2];
		}
	}

#pragma region 创建instanceInfo示例
	if (!isV2)//s3m2.0 pbr材质暂不支持实例化信息
	{
		std::vector<Matrix4d> vecMats;
		std::vector<unsigned int> vecIds;
		for (int i = 0; i < 5; i++)
		{
			Matrix4d mat = Matrix4d::IDENTITY;
			mat.m[3][0] = 20 * i;
			mat.m[3][1] = 20 * i;
			vecMats.push_back(mat);
			vecIds.push_back(i + 1);
		}
		pGeometry->CreateInstanceInfo(vecMats, vecIds);
	}
#pragma endregion

	pGeometry->ComputerBoundingBox();
	return pGeometry;
}

TextureData* ROGroupLite::CreateTextureData(wstring texturePath)
{	
	if (texturePath.find(L':') == wstring::npos)
	{
		wchar_t buffer[200];
		GetCurrentDirectoryW(200, buffer);
		wstring strPath(buffer);
		texturePath = StringHelper::GetAbsolutePath(texturePath);
	}
	TextureData* m_pTextureData = ImgToolkit::GetTextureData(texturePath);
	return m_pTextureData;
}

Material* ROGroupLite::CreateMaterial(wstring textureDataName)
{
	Material* pMaterial3D = new Material;
	wstring strMtlName = L"材质";
	Technique* pTech = pMaterial3D->CreateTechnique();
	Pass* pPass = pTech->CreatePass();

	pPass->SetPFFMode((PolygonFrontFace)PFF_NONE);
	strMtlName += textureDataName;

	TextureUnitState* pTexUnit = pPass->CreateTextureUnitState();
	//材质对象通过贴图对象的名称关联贴图对象
	pTexUnit->m_strTextureName = textureDataName;

	// 材质对象名称
	pMaterial3D->m_strName = strMtlName;
	// 设置通道名称
	pPass->m_strName = strMtlName;
	pPass->m_pPRBMaterial = new PBRMetallicRough;
	PBRMetallicRough* pPbr = (PBRMetallicRough*)pPass->m_pPRBMaterial;
	pPbr->m_nBaseColorTextureIndex = 0;
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



