#include "Sample.h"

UGRenderOperationGeometry* Tools::CreateGeometry(MeshParamInfo& meshInfo)
{
	UGRenderOperationGeometry* pGeometry = new UGRenderOperationGeometry;
	UGString strName;
	strName.Format(_U("_%d"), meshInfo.mtlID);
	//骨架名字，唯一
	pGeometry->m_strGeoName = meshInfo.strID + strName;
	pGeometry->m_pVertexDataPackage = new UGVertexDataPackage;
	pGeometry->m_pVertexDataPackage->m_nVertexDimension = 3;
	pGeometry->m_pVertexDataPackage->SetVertexNum(meshInfo.verts.size() / 3);
	UGfloat* pVertices = pGeometry->m_pVertexDataPackage->m_pVertices;
	UGint vertPoint = 0;
	for (; vertPoint < meshInfo.verts.size(); vertPoint++, pVertices++)
	{
		*pVertices = meshInfo.verts[vertPoint];
	}

	//uv
	{
		pGeometry->m_pVertexDataPackage->m_TexCoordCount[0] = meshInfo.texCoords.size()/2;
		pGeometry->m_pVertexDataPackage->m_pTexCoords[0] = new UGfloat[meshInfo.texCoords.size()];
		UGfloat* pTexCoords = pGeometry->m_pVertexDataPackage->m_pTexCoords[0];
		//赋值UV
		//pTexCoords = NULL;
		for (int i = 0; i < meshInfo.texCoords.size(); i++, pTexCoords++)
		{
			*pTexCoords = meshInfo.texCoords[i];
		}
	}

	UGIndexPackage* pIndexPackage = new UGIndexPackage;
	//如果 meshInfo.verts.size() / 3  == meshInfo.texCoords.size() / 2  则将下面的 IT_16BIT_2 与 IT_32BIT_2 分别改为IT_16BIT 和 IT_32BIT
	//然后将第二句改为 pIndexPackage->SetIndexNum(meshInfo.indexs.size());
	//并去掉if和else中的第二个for循环
	if (meshInfo.verts.size() / 3 < 65536)
	{
		pIndexPackage->m_enIndexType = IT_16BIT_2;
		pIndexPackage->SetIndexNum(meshInfo.indexs.size()*2);
		UGushort* pIndexes = pIndexPackage->m_pIndexes;
		UGint indexPoint = 0;
		for (; indexPoint < meshInfo.indexs.size(); indexPoint++, pIndexes++)
		{
			*pIndexes = meshInfo.indexs[indexPoint];
		}
		for (;indexPoint<meshInfo.indexs.size()*2;indexPoint++,pIndexes++)
		{
			*pIndexes = indexPoint - meshInfo.indexs.size();
		}
		pGeometry->m_arrIndexPackage.Add(pIndexPackage);
	}
	else if (meshInfo.verts.size() / 3 >= 65536)
	{
		pIndexPackage->m_enIndexType = IT_32BIT_2;
		pIndexPackage->m_nIndexesCount = meshInfo.indexs.size();
		UGuint* pIndexes = new UGuint[meshInfo.indexs.size()];
		pIndexPackage->m_pIndexes = (UGushort*)pIndexes;
		UGint indexPoint = 0;
		for (; indexPoint < meshInfo.indexs.size(); indexPoint++, pIndexes++)
		{
			*pIndexes = meshInfo.indexs[indexPoint];
		}
		pGeometry->m_arrIndexPackage.Add(pIndexPackage);
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

UGMaterial3D* Tools::CreateMaterial3D(UGString textureDataName)
{
	UGMaterial3D* pMaterial3D = new UGMaterial3D;
	UGString strMtlName = _U("");
	UGTechnique* pTech = pMaterial3D->createTechnique();
	UGPass* pPass = pTech->createPass();

	pPass->SetPFFMode((PolygonFrontFace)PFF_NONE);
	strMtlName = textureDataName;

	UGTextureUnitState* pTexUnit = pPass->CreateTextureUnitState();
	//材质对象关联贴图对象，用贴图对象的名称关联
	pTexUnit->m_strTextureName = strMtlName;
	//材质对象名称
	pMaterial3D->m_strName = strMtlName;
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

UGTextureData* Tools::CreateTextureData(UGString texturePath)
{
	UGTextureData* m_pTextureData = new UGTextureData();
	UGFileParseToolkit::GetTextureData(texturePath, m_pTextureData);
	return m_pTextureData;
}
