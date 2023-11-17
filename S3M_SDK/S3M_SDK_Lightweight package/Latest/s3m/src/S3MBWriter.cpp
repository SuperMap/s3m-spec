#include "S3MBWriter.h"
#include <assert.h>

namespace S3MB
{
	S3MBWriter::S3MBWriter()
	{
		m_bChangeTexture = true;
		m_CompressType = (int)TextureCompressType::TC_DXT5;// dxt5Ñ¹Ëõ
		m_bGenerateMipMap = true;
		m_fVersion = S3MB_VERSIONV3_0_1;
	}

	S3MBWriter::~S3MBWriter()
	{
	}

	void S3MBWriter::ProcessEntities()
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
			wstring strName = pChildNode->ClassName();
			if (StringUtil::CompareNoCase(strName, S3MB_SHELL_PAGEDLOD))
			{
				FillEntityPack((RenderOperationPagedLOD*)pChildNode);
			}
			else if (StringUtil::CompareNoCase(strName, S3MB_SHELL_GEODE))
			{
				FillEntityPack((RenderOperationGeode*)pChildNode);
			}
		}
		// Shell
		S3MBTools::SaveShell(m_pROGroup, m_streamShell, m_fVersion);

		// Skeletons
		S3MBTools::SaveSkeleton(m_mapSkeleton, m_streamSkeleton, m_SkeletonCompressParam, m_fVersion);

		// Materials;
		string strMaterials;
		JsonValue jsonMaterials(JsonValueType::objectValue, true);
		S3MBTools::SaveMaterial(m_mapMaterial, jsonMaterials, m_fVersion);
		jsonMaterials.SaveToString(strMaterials);
		m_strJsonMaterials = StringUtil::UTF8_to_UNICODE(strMaterials);

		// Textures
		S3MBTools::SaveTextures(m_mapTexture, m_streamTexture, m_CompressType, m_bChangeTexture, m_bGenerateMipMap);

		m_bHasIDInfo = S3MBTools::SaveIDInfo(m_mapSkeleton, m_streamIDInfo);
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
			wstring strName = pChildNode->ClassName();
			if (StringUtil::CompareNoCase(strName, U("PagedLOD")))
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
		int nSkeletonCount = pROGeode->GetNumSkeleton();
		if (nSkeletonCount == 0)
		{
			return;
		}

		Skeleton* pSkeleton = NULL;
		for (int i = 0; i < nSkeletonCount; i++)
		{
			pSkeleton = pROGeode->GetSkeleton(i);
			wstring strName = pSkeleton->m_strGeoName;
			m_mapSkeleton[strName] = pSkeleton;

			FillEntityPack(pSkeleton);
		}
	}

	void S3MBWriter::FillEntityPack(Skeleton* pSkeleton)
	{
		int nIndexPackageCount = pSkeleton->m_arrIndexPackage.size();
		for (int i = 0; i < nIndexPackageCount; i++)
		{
			IndexPackage* pIndexPackage = pSkeleton->m_arrIndexPackage[i];
			int nSize = pIndexPackage->m_strPassName.size();
			for (int j = 0; j < nSize; j++)
			{
				wstring strName = pIndexPackage->m_strPassName[j];
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
			wstring strTextureName = pTexUnit->m_strTextureName;
			TextureDataInfo* pTexture = m_pROGroup->GetTextureData(strTextureName);
			if (pTexture == NULL || pTexture->m_pTextureData == NULL || pTexture->m_pTextureData->m_pBuffer == NULL)
			{
				continue;
			}
			m_mapTexture[strTextureName] = pTexture;
		}
	}

	bool S3MBWriter::WriteFile(const wstring& filePath)
	{	
		if (filePath.find(L':') == wstring::npos)
			m_strFilePath = StringUtil::GetAbsolutePath(filePath);
		else
			m_strFilePath = filePath;

		ProcessEntities();

		return S3MBTools::SaveStreamData2File(m_strFilePath, \
			m_streamShell, m_streamSkeleton,m_strJsonMaterials, m_streamTexture, \
			m_bHasIDInfo, m_streamIDInfo, m_strExtensions, m_fVersion);
		
	}

	void S3MBWriter::SetROGroup(RenderOperationGroup* pGroup)
	{
		m_pROGroup = pGroup;
	}

	void S3MBWriter::SetIsChangeTexture(bool bChange, bool bGenerateMipMap) {
		m_bChangeTexture = bChange;
		m_bGenerateMipMap = bGenerateMipMap;
	}

	void S3MBWriter::SetExtensions(const std::wstring& strExtensions)
	{
		m_strExtensions = strExtensions;
	}

	void S3MBWriter::SetSkeletonCompressParam(SkeletonCompressParam param)
	{
		m_SkeletonCompressParam = param;
	}

	void S3MBWriter::Clear()
	{
		m_mapSkeleton.clear();
		m_mapMaterial.clear();
		m_mapTexture.clear();
	}
}

