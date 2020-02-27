#include "S3MBWriter.h"
#include "S3MBTools.h"
#include <assert.h>

void S3MBWriter::ProcessSaveInfo(int vertexCompressOptions /*= 19*/)
{
	int nChildCount = m_pROGroup->GetNumChildren();
	for (int i = 0; i < nChildCount; i++)
	{
		RenderOperationNode* pChildNode = m_pROGroup->GetChild(i);
		if (pChildNode == NULL)
		{
			assert(false);
			continue;
		}
		string strName = pChildNode->ClassName();
		if (StringHelper::CompareNoCase(strName,"PagedLOD"))
		{
			FillEntityPack((RenderOperationPagedLOD*)pChildNode);
		}
		else if (StringHelper::CompareNoCase(strName,"Geode"))
		{
			FillEntityPack((RenderOperationGeode*)pChildNode);
		}
	}

	//Shell
	S3MBTools::SaveShell(m_pROGroup, m_streamShell);

	//Skeletons
	S3MBTools::SaveGeometry(m_mapGeometry, m_streamGeometry, vertexCompressOptions);

	//Materials;
	JsonValue* jsonMaterials = new JsonValue(ValueType::objectValue,true);
	S3MBTools::SaveMaterial(m_mapMaterial, *jsonMaterials);
	FastWriter writer;
	m_strJsonMaterials = writer.write((*jsonMaterials).GetValue());

	//Textures
	S3MBTools::SaveTextures(m_mapTexture, m_streamTexture, m_CompressType,m_bChangeTexture);

	m_bHasSelectionInfo = S3MBTools::SaveSelectionInfo(m_mapGeometry,m_streamSelectionInfo);
}

void S3MBWriter::FillEntityPack(RenderOperationPagedLOD* pROPagedLOD)
{
	for (int i = 0; i < pROPagedLOD->GetNumChildren(); i++)
	{
		RenderOperationNode* pChildNode = pROPagedLOD->GetChild(i);
		if (pChildNode == NULL)
		{
			assert(false);
			continue;
		}
		string strName = pChildNode->ClassName();
		if (StringHelper::CompareNoCase(strName,"PagedLOD"))
		{
			FillEntityPack((RenderOperationPagedLOD*)pChildNode);
		}
		else
		{
			FillEntityPack((RenderOperationGeode*)pChildNode);
		}
	}
}

void S3MBWriter::FillEntityPack(RenderOperationGeode* pROGeode)
{
	int nGeometryCount = pROGeode->GetNumGeometry();
	if (nGeometryCount == 0)
	{
		return;
	}

	Geometry* pGeometry = NULL;
	for (int i = 0; i < nGeometryCount; i++)
	{
		pGeometry = pROGeode->GetGeometry(i);
		string strName = pGeometry->m_strGeoName;
		m_mapGeometry[strName] = pGeometry;

		FillEntityPack(pGeometry);
	}
}

void S3MBWriter::FillEntityPack(Geometry* pGeometry)
{
	int nIndexPackageCount = pGeometry->m_arrIndexPackage.size();
	for (int i = 0; i < nIndexPackageCount; i++)
	{
		IndexPackage* pIndexPackage = pGeometry->m_arrIndexPackage[i];
		int nSize = pIndexPackage->m_strPassName.size();
		for (int j = 0; j < nSize; j++)
		{
			string strName = pIndexPackage->m_strPassName[j];
			Material* pMaterial = m_pROGroup->GetMaterial(strName);
			if (pMaterial == NULL)
			{
				assert(false);
				continue;
			}

			m_mapMaterial[strName] = pMaterial;
			pMaterial->m_strName = strName;

			FillEntityPack(pMaterial);
		}
	}
}

void S3MBWriter::FillEntityPack(Material* pMaterial3D)
{
	Technique* pTechnique = pMaterial3D->getTechnique(0);
	Pass* pPass = pTechnique->getPass(0);
	int nTextureSize = pPass->GetTextureUnitStatesSize();
	for (int i = 0; i < nTextureSize; i++)
	{
		TextureUnitState* pTexUnit = pPass->GetTextureUnitState(i);
		string strTextureName = pTexUnit->m_strTextureName;
		TextureDataInfo* pTexture = m_pROGroup->GetTextureData(strTextureName);
		if (pTexture == NULL || pTexture->m_pTextureData == NULL || pTexture->m_pTextureData->m_pBuffer == NULL)
		{
			continue;
		}
		m_mapTexture[strTextureName] = pTexture;
	}
}

bool S3MBWriter::Save(int vertexCompressOptions /*= 19*/)
{
	ProcessSaveInfo(vertexCompressOptions);
	return S3MBTools::SaveStreamData2File(m_strFilePath, \
		m_streamShell, m_streamGeometry, m_streamSelectionInfo, \
		m_strJsonMaterials, m_streamTexture, \
		m_bHasSelectionInfo, m_streamSelectionInfo);
}

