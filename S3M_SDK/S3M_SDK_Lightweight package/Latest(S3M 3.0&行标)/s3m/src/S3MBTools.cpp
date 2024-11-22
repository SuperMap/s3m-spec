#include "S3MBTools.h"
#include "ImageOperator.h"
#include "Palette.h"
#include "S3MBUtils.h"
#include <algorithm>
#include <assert.h>
#include <math.h>

namespace S3MB
{
	bool S3MBTools::LoadStreamDataFromStream(MemoryStream& dataStream, MemoryStream& streamUnZipped, MemoryStream& streamShell, MemoryStream& streamSkeleton, 
		MemoryStream& streamTexture, MemoryStream& streamIDInfo, wstring& strJsonMaterials, wstring& strExtensions, float fVersion)
	{
		// 0表示不压缩，1表示zip压缩
		unsigned int nCompressedType = 0;
		dataStream >> nCompressedType;
		
		unsigned int nTotalLength = 0;
		dataStream >> nTotalLength;
		unsigned int nZippedSize = 0;
		dataStream >> nZippedSize;

		unsigned char* pZippedData = new(std::nothrow) unsigned char[nZippedSize];
		dataStream.Load(pZippedData, nZippedSize);

		if (nCompressedType == 1)
		{
			if (!UnZipData(streamUnZipped, nTotalLength, pZippedData, nZippedSize))// 解压数据流
			{
				return false;
			}
		}
		else
		{
			streamUnZipped.Init(pZippedData, nZippedSize, true);
		}

		unsigned int nOptions = 0;
		streamUnZipped >> nOptions;

		LoadStream(streamUnZipped, streamShell);
		LoadStream(streamUnZipped, streamSkeleton);
		LoadStream(streamUnZipped, streamTexture);
		streamUnZipped >> strJsonMaterials;
		if (nOptions)// 带ID存储信息	
		{
			unsigned int nIDInfoOption = 0;
			streamUnZipped >> nIDInfoOption;
			
			LoadStream(streamUnZipped, streamIDInfo);
		}

		streamUnZipped >> strExtensions;;

		return true;
	}

	bool S3MBTools::UnZipData(MemoryStream& streamUnZipped, unsigned char*& pZippedData, unsigned int nZippedSize)
	{
		unsigned long nUnZipSize = nZippedSize * 4;
		unsigned char* pUnZipData = new (std::nothrow) unsigned char[nUnZipSize];
		bool bResult = Utils::UnZip(pUnZipData, nUnZipSize, pZippedData, nZippedSize);

		delete pZippedData;
		pZippedData = NULL;

		if (nUnZipSize <= 0)
		{
			delete[]pUnZipData;
			pUnZipData = NULL;
			return false;
		}

		streamUnZipped.Init(pUnZipData, nUnZipSize, true);
		return true;
	}
	bool S3MBTools::UnZipData(MemoryStream& streamUnzipped, unsigned int& nUnZipSize, unsigned char*& pZippedData, unsigned int& nZippedSize)
	{	
		unsigned char *pUnZipData = new(std::nothrow) unsigned char[nUnZipSize];
		if (pUnZipData == NULL)
		{
			nUnZipSize = 0;
			return false;
		}

		Utils::UnZip(pUnZipData, nUnZipSize, pZippedData, nZippedSize);

		delete pZippedData;
		pZippedData = nullptr;

		if (nUnZipSize <= 0)
		{
			delete[]pUnZipData;
			pUnZipData = NULL;
			return false;
		}
		streamUnzipped.Init(pUnZipData, nUnZipSize, true);
		return true;
	}

	void S3MBTools::LoadSkeleton(MemoryStream& streamSkeleton, std::map<wstring, Skeleton*>& mapSkeleton, float fVersion)
	{
		int nCount = 0;
		streamSkeleton >> nCount;

		for (int k = 0; k < nCount; k++)
		{
			wstring strGeoName;
			VertexDataPackage* pVertexDataPackage = NULL;
			vector<IndexPackage*> arrIndexPackage;

			MemoryStream streamPerSkeleton;
			LoadStream(streamSkeleton, streamPerSkeleton);
			streamPerSkeleton >> strGeoName;
			StreamAlign(streamPerSkeleton, true);
			if (!LoadSkeletonData(streamPerSkeleton, pVertexDataPackage, arrIndexPackage, fVersion))
			{
				continue;
			}

			assert(pVertexDataPackage != NULL && arrIndexPackage.size() > 0);
			Skeleton* pSkeleton = new Skeleton();
			pSkeleton->m_strGeoName = strGeoName;
			if (arrIndexPackage.size() > 0 && arrIndexPackage[0]->m_strPassName.size() > 0)
			{
				pSkeleton->m_strMaterialName = arrIndexPackage[0]->m_strPassName[0];
			}
			pSkeleton->m_pVertexDataPackage = pVertexDataPackage;
			pSkeleton->m_arrIndexPackage = arrIndexPackage;
			LoadOBB(streamPerSkeleton, pSkeleton->m_OBB);
			pSkeleton->m_OBB.ToBoundingBox(pSkeleton->m_BoundingBox);

			pSkeleton->m_bInstanceBatch = IsInstanceBatch(pVertexDataPackage);
			mapSkeleton[strGeoName] = pSkeleton;
		}
	}

	bool S3MBTools::LoadSkeletonData(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage, vector<IndexPackage*>& arrIndexPackage, float fVersion)
	{
		pVertexDataPackage = new VertexDataPackage();
		unsigned int nOptions = 0;
		streamSkeleton >> nOptions;
		if (nOptions == 0)
		{
			MemoryStream streamVertexPackage;
			LoadStream(streamSkeleton, streamVertexPackage);

			LoadVertex(streamVertexPackage, pVertexDataPackage);
			LoadNormal(streamVertexPackage, pVertexDataPackage);
			LoadVertexColor(streamVertexPackage, pVertexDataPackage);
			LoadTextureCoords(streamVertexPackage, pVertexDataPackage);
			LoadInstanceInfo(streamVertexPackage, pVertexDataPackage, fVersion);
			LoadVertexAttributeExtension(streamVertexPackage, pVertexDataPackage);
			LoadTangent(streamVertexPackage, pVertexDataPackage);

			if (!LoadIndexPackages(streamSkeleton, arrIndexPackage, fVersion))
			{
				delete pVertexDataPackage;
				pVertexDataPackage = nullptr;
				return false;
			}
		}
		else if (nOptions == 1)
		{
			MemoryStream streamDraco;
			S3MBTools::LoadStream(streamSkeleton, streamDraco);

			bool bResult = LoadDraco(streamDraco, pVertexDataPackage, arrIndexPackage, fVersion);
			if (GREATER_OR_EQUAL(fVersion, S3MB_VERSIONV3_0_1))
			{
				LoadInstanceInfo(streamDraco, pVertexDataPackage, fVersion);
			}
			return bResult;
		}
		return true;
	}

	bool S3MBTools::LoadDraco(MemoryStream &stream, VertexDataPackage*& pVertexDataPack, std::vector<IndexPackage*>& arrIndexPackage, float fVersion /*= 1.0f*/)
	{
		DracoCompressedInfo compressInfo;
		int nPosUniqueID = -1;
		int nNormalUniqueID = -1;
		int nColorUniqueID = -1;
		int nCompressOption = 0;

		stream >> nCompressOption;	

		stream >> nPosUniqueID;
		stream >> nNormalUniqueID;
		stream >> nColorUniqueID;
		if (EQUAL(fVersion,S3MB_VERSIONV3))
		{
			int nSecColor = -1;
			stream >> nSecColor;
		}

		compressInfo.m_posInfo.SetUniqueID(nPosUniqueID);
		compressInfo.m_normalInfo.SetUniqueID(nNormalUniqueID);
		compressInfo.m_colorInfo.SetUniqueID(nColorUniqueID);

		unsigned short nTextureCoord = 0;

		unsigned int nTmpTextureCoord;
		stream >> nTmpTextureCoord;
		nTextureCoord = nTmpTextureCoord;


		for (int i = 0; i < nTextureCoord; i++)
		{
			int nTexCoordUniqueID = -1;
			stream >> nTexCoordUniqueID;
			compressInfo.m_texCoordInfo[i].SetUniqueID(nTexCoordUniqueID);
		}

		unsigned int nVertexAttCount = 0;

		stream >> nVertexAttCount;
		if (nVertexAttCount > 0)
		{
			compressInfo.m_nVertexAttInfoCount = nVertexAttCount;
			compressInfo.m_pVertexAttInfo = new DracoAttributeInfo[nVertexAttCount];
		}
		for (int i = 0; i < nVertexAttCount; i++)
		{
			int nVertexAttInfoID = 0;
			stream >> nVertexAttInfoID;
			if (compressInfo.m_pVertexAttInfo != NULL)
			{
				compressInfo.m_pVertexAttInfo[i].SetUniqueID(nVertexAttInfoID);
			}
		}
		

		// dracoMesh存储顶点和索引不存储索引对应的材质名字。因此需要单独存储索引对应的材质名字
		int nIndexPackageCount = 0;
		stream >> nIndexPackageCount;
		std::vector<wstring> arrPassName;
		for (int i = 0; i < nIndexPackageCount; i++)
		{
			wstring strPassName;
			stream >> strPassName;
			arrPassName.push_back(strPassName);
		}

		// 四字节对齐
		S3MBTools::StreamAlign(stream, true);

		bool isPointCloud = nIndexPackageCount == 0;
		unsigned int nSize = 0;
		stream >> nSize;
		char* pChar = new char[nSize];
		stream.Load(pChar, nSize);

		// 四字节对齐
		S3MBTools::StreamAlign(stream, true);
		

		bool bDecompressResult = DracoTool::DracoDecompress(compressInfo, pChar, nSize, isPointCloud, nVertexAttCount, pVertexDataPack, arrIndexPackage);

		pVertexDataPack->m_nCompressOptions = nCompressOption;

		if (pChar != NULL)
		{
			delete pChar;
			pChar = NULL;
		}

		if (compressInfo.m_pVertexAttInfo != NULL)
		{
			delete[] compressInfo.m_pVertexAttInfo;
			compressInfo.m_pVertexAttInfo = NULL;
		}

		for (unsigned int i = 0; i < arrIndexPackage.size(); i++)
		{
			IndexPackage* pIndexPackage = arrIndexPackage[i];
			pIndexPackage->m_strPassName.push_back(arrPassName[i]);
		}

		wstring strVertexAttributeDescript;
		stream >> strVertexAttributeDescript;
		StreamAlign(stream, true);

		JsonValue jsonVertexAtts;
		if (!jsonVertexAtts.LoadValueFromString(strVertexAttributeDescript))
		{
			return false;
		}
		vector<wstring> arrKeys;
		jsonVertexAtts.GetAllKeys(arrKeys);

		for (int i = 0; i < arrKeys.size(); i++)
		{
			jsonVertexAtts.GetValue(arrKeys[i], pVertexDataPack->m_mapVertexAttributeDescript[arrKeys[i]]);
		}

		return bDecompressResult;
	}

	void S3MBTools::StreamAlign(MemoryStream& stream, bool isRead)
	{
		if (isRead)
		{
			unsigned int nSize = stream.GetReadPosition();
			if (nSize % 4 == 0)
			{
				return;
			}
			unsigned char b = 0;
			unsigned int nReserved = 4 - nSize % 4;
			for (unsigned int i = 0; i < nReserved; i++)
			{
				stream >> b;
			}
		}
		else
		{
			unsigned int nSize = stream.GetWritePosition();
			if (nSize % 4 == 0)
			{
				return;
			}
			unsigned int nReserved = 4 - nSize % 4;
			unsigned char b = 0;
			for (unsigned int i = 0; i < nReserved; i++)
			{
				stream << b;
			}
		}
	}

	bool S3MBTools::LoadVertex(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage)
	{
		// unsigned int
		streamSkeleton >> pVertexDataPackage->m_nVerticesCount;
		if (pVertexDataPackage->m_nVerticesCount <= 0)
		{
			return false;
		}

		// unsigned short
		streamSkeleton >> pVertexDataPackage->m_nVertexDimension;
		// unsigned short
		streamSkeleton >> pVertexDataPackage->m_nVertexStride;

		pVertexDataPackage->m_pVertices = \
			new(std::nothrow) float[pVertexDataPackage->m_nVerticesCount * pVertexDataPackage->m_nVertexDimension];
		streamSkeleton.Load(pVertexDataPackage->m_pVertices, \
			pVertexDataPackage->m_nVerticesCount * pVertexDataPackage->m_nVertexDimension);

		return true;
	}

	bool S3MBTools::LoadNormal(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage)
	{
		// unsigned int
		streamSkeleton >> pVertexDataPackage->m_nNormalCount;

		if (pVertexDataPackage->m_nNormalCount == 0)
		{
			return false;
		}

		// unsigned short 
		streamSkeleton >> pVertexDataPackage->m_nNormalDimension;
		streamSkeleton >> pVertexDataPackage->m_nNormalStride;

		pVertexDataPackage->m_pNormals = \
			new(std::nothrow) float[pVertexDataPackage->m_nNormalCount * pVertexDataPackage->m_nNormalDimension];
		streamSkeleton.Load(pVertexDataPackage->m_pNormals, \
			pVertexDataPackage->m_nNormalCount * pVertexDataPackage->m_nNormalDimension);

		return true;
	}

	bool S3MBTools::LoadTangent(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage)
	{
		// unsigned int
		streamSkeleton >> pVertexDataPackage->m_nTangentCount;

		if (pVertexDataPackage->m_nTangentCount == 0)
		{
			return true;
		}

		// unsigned short 
		streamSkeleton >> pVertexDataPackage->m_nTangentDimension;
		streamSkeleton >> pVertexDataPackage->m_nTangentStride;

		pVertexDataPackage->m_pTangents = \
			new(std::nothrow) float[pVertexDataPackage->m_nTangentCount * pVertexDataPackage->m_nTangentDimension];
		streamSkeleton.Load(pVertexDataPackage->m_pTangents, \
			pVertexDataPackage->m_nTangentCount * pVertexDataPackage->m_nTangentDimension);

		return true;
	}

	bool S3MBTools::LoadVertexColor(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage)
	{
		unsigned int nColorCount = 0;
		streamSkeleton >> pVertexDataPackage->m_nVertexColorCount;
		// 顶点颜色
		if (pVertexDataPackage->m_nVertexColorCount <= 0)
		{
			return false;
		}
		unsigned char nByte = 0;
		streamSkeleton >> pVertexDataPackage->m_nVertexColorStride;
		streamSkeleton >> nByte;
		streamSkeleton >> nByte;

		pVertexDataPackage->m_pVertexColor = new(std::nothrow) unsigned int[pVertexDataPackage->m_nVertexColorCount];
		for (unsigned int j = 0; j < pVertexDataPackage->m_nVertexColorCount; j++)
		{
			unsigned char btR, btG, btB, btA;

			streamSkeleton >> btR;
			streamSkeleton >> btG;
			streamSkeleton >> btB;
			streamSkeleton >> btA;

			pVertexDataPackage->m_pVertexColor[j] = Palette::RGBA(btR, btG, btB, btA);
		}
		return true;
	}

	bool S3MBTools::LoadTextureCoords(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage)
	{
		unsigned short nTextureCoord;
		streamSkeleton >> nTextureCoord;

		unsigned char nByte = 0;
		streamSkeleton >> nByte;
		streamSkeleton >> nByte;

		for (int i = 0; i < nTextureCoord; i++)
		{
			streamSkeleton >> pVertexDataPackage->m_TexCoordCount[i];
			streamSkeleton >> pVertexDataPackage->m_nTexDimensions[i];
			streamSkeleton >> pVertexDataPackage->m_TexCoordStride[i];

			int nValueCount = pVertexDataPackage->m_TexCoordCount[i] * pVertexDataPackage->m_nTexDimensions[i];
			float* pValue = new(std::nothrow) float[nValueCount];
			streamSkeleton.Load(pValue, nValueCount);

			pVertexDataPackage->m_pTexCoords[i] = pValue;
		}
		return true;
	}

	bool S3MBTools::LoadInstanceInfo(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage, float fVersion)
	{
		unsigned char nByte = 0;
		unsigned short nInstanceInfo = 0;
		streamSkeleton >> nInstanceInfo;
		streamSkeleton >> nByte;
		streamSkeleton >> nByte;

		if (nInstanceInfo > 0)
		{
			unsigned int nInstanceInfoNum = 0;
			streamSkeleton >> nInstanceInfoNum;
			for (size_t i = 0; i < nInstanceInfoNum; i++)
			{
				InstanceInfo* pInfo = new InstanceInfo;
				streamSkeleton >> pInfo->nCount;
				if (GREATER_OR_EQUAL(fVersion, S3MB_VERSIONV3_0_1))
				{
					streamSkeleton >> nByte;
					streamSkeleton >> nByte;
				}
				pInfo->pMatrixValues = new float[pInfo->nCount];
				streamSkeleton.Load(pInfo->pMatrixValues, pInfo->nCount);
				pVertexDataPackage->m_vecInstanceInfo.push_back(pInfo);
			}
		}
		return true;
	}

	bool S3MBTools::IsInstanceBatch(VertexDataPackage* pVertexDataPackage)
	{
		if (pVertexDataPackage != nullptr)
		{
			return pVertexDataPackage->m_vecInstanceInfo.size() > 0;
		}
		return false;
	}

	void S3MBTools::LoadMaterial(wstring strJsonMaterials, std::map<wstring, Material*>& mapMaterial,float fVersion)
	{
		JsonValue* pJsonValue = new JsonValue(strJsonMaterials);
		JsonValue* pJsonArray = NULL;
		wstring strMaterials = GREATER_OR_EQUAL(fVersion, S3MB_VERSIONV3_0_1) ? S3MB_MATERIALS : L"material";
		if (!pJsonValue->GetValue(strMaterials, pJsonArray))
		{
			assert(false);
			return;
		}
		int nSize = pJsonArray->GetArraySize();
		JsonValue* pOneTile = new JsonValue(nullValue);
		for (int i = 0; i < nSize; i++)
		{
			Material* pMaterial = new Material();
			Technique* pTechnique = pMaterial->CreateTechnique();
			pJsonArray->GetAt(i, pOneTile);
			if (GREATER_OR_EQUAL(fVersion,S3MB_VERSIONV3_0_1))
			{
				LoadMatFromJson(*pOneTile, pMaterial->m_strName, pTechnique);
			}
			else
			{
				LoadMatFromJsonV3(*pOneTile, pMaterial->m_strName, pTechnique);
			}
			assert(pMaterial != NULL);

			mapMaterial[pMaterial->m_strName] = pMaterial;
		}
		delete pOneTile;
		pOneTile = NULL;
		delete pJsonArray;
		pJsonArray = NULL;
		delete pJsonValue;
		pJsonValue = NULL;
	}

	void S3MBTools::LoadMatFromJson(JsonValue & jsonMaterial, wstring & strMaterialName, Technique * pTechnique)
	{
		Pass* pPass = pTechnique->CreatePass();
		JsonValue* pJsonContent = NULL;
		jsonMaterial.GetValue(S3MB_MATERIAL, pJsonContent);

		pJsonContent->GetValue(S3MB_NAME, strMaterialName);
		wstring strCullMode;
		pJsonContent->GetValue(S3MB_MATPASS_CULLMODE, strCullMode);
		pPass->m_CullMode = S3MBTools::CullModeFromString(strCullMode);
		wstring strAlphaMode;
		pJsonContent->GetValue(S3MB_MATERIAL_PBR_ALPHAMODE, strAlphaMode);
		pPass->m_AlphaMode = S3MBTools::AlphaModeFromString(strAlphaMode);
		pJsonContent->GetValue(S3MB_MATERIAL_PBR_ALPHACUTOFF, pPass->m_fAlphaCutoff);
		pJsonContent->GetValue(S3MB_MATERIAL_SHADERNAME_VERTEX, pPass->strVertexProgram);
		pJsonContent->GetValue(S3MB_MATERIAL_SHADERNAME_FRAGMENT, pPass->strFragmentProgram);

		wstring strMtlType;
		pJsonContent->GetValue(S3MB_MATERIAL_TYPE, strMtlType);
		MaterialType emType = S3MBTools::MaterialTypeFromString(strMtlType);
		if (emType == MAT_PBR)
		{
			// PBR
			PBRParams* pPBR = NULL;
			S3MBTools::LoadPBRFromJson(*pJsonContent, pPBR);
			pPass->m_pPRBMaterial = pPBR;
		}
		else
		{
			pJsonContent->GetValue(S3MB_MATERIAL_AMBIENT, pPass->m_Ambient);
			pJsonContent->GetValue(S3MB_MATERIAL_DIFFUSE, pPass->m_Diffuse);
			pJsonContent->GetValue(S3MB_MATERIAL_SPECULAR, pPass->m_Specular);
			pJsonContent->GetValue(S3MB_MATERIAL_SHININESS, pPass->m_Shininess);
		}

		// TextureUnitStates
		JsonValue* pJsonArrayTextUnit = NULL;
		JsonValue* pJsonTextUnit = new JsonValue(nullValue);
		pJsonContent->GetValue(S3MB_MATERIAL_TEXTUNITS, pJsonArrayTextUnit);
		int nTextureSize = pJsonArrayTextUnit->GetArraySize();
		for (int i = 0; i < nTextureSize; i++)
		{
			TextureUnitState* pTexUnit = pPass->CreateTextureUnitState();
			pJsonArrayTextUnit->GetAt(i, pJsonTextUnit);
			LoadTUSFromJson(*pJsonTextUnit, pTexUnit);
		}
		// Extensions
		pJsonContent->GetValue(S3MB_MATERIAL_EXTENSIONS, pPass->m_strExtensions);

		pPass->m_strName = strMaterialName;

		delete pJsonContent;
		pJsonContent = NULL;
		delete pJsonTextUnit;
		pJsonTextUnit = NULL;
		delete pJsonArrayTextUnit;
		pJsonArrayTextUnit = NULL;
	}

	void S3MBTools::LoadMatFromJsonV3(JsonValue& jsonMaterial, wstring& strMaterialName, Technique* pTechnique)
	{
		Pass* pPass = pTechnique->CreatePass();
		JsonValue* pJsonContent = NULL;
		jsonMaterial.GetValue(S3MB_MATERIAL, pJsonContent);

		pJsonContent->GetValue(L"id", strMaterialName);
		wstring strCullMode;
		pJsonContent->GetValue(S3MB_MATPASS_CULLMODE, strCullMode);
		pPass->m_CullMode = S3MBTools::CullModeFromStringV3(strCullMode);
		wstring strAlphaMode;
		pJsonContent->GetValue(S3MB_MATERIAL_PBR_ALPHAMODE, strAlphaMode);
		pPass->m_AlphaMode = S3MBTools::AlphaModeFromStringV3(strAlphaMode);
		pJsonContent->GetValue(S3MB_MATERIAL_PBR_ALPHACUTOFF, pPass->m_fAlphaCutoff);
		pJsonContent->GetValue(S3MB_MATERIAL_SHADERNAME_VERTEX, pPass->strVertexProgram);
		pJsonContent->GetValue(S3MB_MATERIAL_SHADERNAME_FRAGMENT, pPass->strFragmentProgram);

		if (pJsonContent->Contains(L"pbrMetallicRoughness"))
		{
			// PBR
			PBRParams* pPBR = NULL;
			LoadPBRFromJsonV3(*pJsonContent, pPBR);
			pPass->m_pPRBMaterial = pPBR;
		}
		else
		{
			pJsonContent->GetValue(S3MB_MATERIAL_AMBIENT, pPass->m_Ambient);
			pJsonContent->GetValue(S3MB_MATERIAL_DIFFUSE, pPass->m_Diffuse);
			pJsonContent->GetValue(S3MB_MATERIAL_SPECULAR, pPass->m_Specular);
			pJsonContent->GetValue(S3MB_MATERIAL_SHININESS, pPass->m_Shininess);
		}

		// TextureUnitStates
		JsonValue *pJsonArrayTextUnit = NULL;
		JsonValue *pJsonTextUnit = new JsonValue(nullValue);
		pJsonContent->GetValue(L"textureunitstates", pJsonArrayTextUnit);
		int nTextureSize = pJsonArrayTextUnit->GetArraySize();
		for (int i = 0; i < nTextureSize; i++)
		{
			TextureUnitState* pTexUnit = pPass->CreateTextureUnitState();
			pJsonArrayTextUnit->GetAt(i, pJsonTextUnit);
			LoadTUSFromJsonV3(*pJsonTextUnit, pTexUnit);
		}

		// Extensions
		pJsonContent->GetValue(S3MB_MATERIAL_EXTENSIONS, pPass->m_strExtensions);

		pPass->m_strName = strMaterialName;

		delete pJsonContent;
		pJsonContent = NULL;
		delete pJsonTextUnit;
		pJsonTextUnit = NULL;
		delete pJsonArrayTextUnit;
		pJsonArrayTextUnit = NULL;
	}

	void S3MBTools::LoadPBRFromJson(JsonValue& jsonPBR, PBRParams*& pPBR)
	{		
		PBRMetallicRough* pPBRMR = new PBRMetallicRough();
		jsonPBR.GetValue(S3MB_MATERIAL_PBRM_BASECOLOR, pPBRMR->m_vec4BaseColor);
		jsonPBR.GetValue(S3MB_MATERIAL_PBRM_BASECOLORTEXTUREINDEX, pPBRMR->m_nBaseColorTextureIndex);
		jsonPBR.GetValue(S3MB_MATERIAL_PBRM_BASECOLORTEXTURECOORDINDEX, pPBRMR->m_nBaseColorTextureCoordIndex);
		JsonValue * pJsonBaseColorTextureMotion = NULL;
		jsonPBR.GetValue(S3MB_MATERIAL_PBRM_BASECOLORTEXTUREMOTION, pJsonBaseColorTextureMotion);
		pPBRMR->m_BaseColorTextureMotion.FromJson(*pJsonBaseColorTextureMotion);
		jsonPBR.GetValue(S3MB_MATERIAL_PBRM_METALLICROUGHNESSTEXTUREINDEX, pPBRMR->m_nMetallicRoughnessTextureIndex);
		jsonPBR.GetValue(S3MB_MATERIAL_PBRM_METALLICROUGHNESSTEXTURECOORDINDEX, pPBRMR->m_nMetallicRoughnessTextureCoordIndex);
		jsonPBR.GetValue(S3MB_MATERIAL_PBRM_METALLICFACTOR, pPBRMR->m_fMetallicFactor);
		jsonPBR.GetValue(S3MB_MATERIAL_PBRM_ROUGHNESSFACTOR, pPBRMR->m_fRoughnessFactor);
		pPBR = pPBRMR;
		
		if (pPBR != NULL)
		{	
			jsonPBR.GetValue(S3MB_MATERIAL_PBR_EMISSIVEFACTOR, pPBR->m_vec3EmissiveFactor);
			jsonPBR.GetValue(S3MB_MATERIAL_PBR_EMISSIVETEXTUREINDEX, pPBR->m_nEmissiveTextureIndex);
			jsonPBR.GetValue(S3MB_MATERIAL_PBR_EMISSIVETEXTURECOORDINDEX, pPBR->m_nEmissiveTextureCoordIndex);
			JsonValue * pJsonEmissiveTextureMotion = NULL;
			jsonPBR.GetValue(S3MB_MATERIAL_PBR_EMISSIVETEXTUREMOTION, pJsonEmissiveTextureMotion);
			pPBR->m_EmissiveTextureMotion.FromJson(*pJsonEmissiveTextureMotion);
			jsonPBR.GetValue(S3MB_MATERIAL_PBR_NORMALTEXTUREINDEX, pPBR->m_nNormalTextureIndex);
			jsonPBR.GetValue(S3MB_MATERIAL_PBR_NORMALTEXTURECOORDINDEX, pPBR->m_nNormalTextureCoordIndex);
			jsonPBR.GetValue(S3MB_MATERIAL_PBR_NORMALTEXTURESCALE, pPBR->m_fNormalTextureScale);
			jsonPBR.GetValue(S3MB_MATERIAL_PBR_OCCLUSIONTEXTUREINDEX, pPBR->m_nOcclusionTextureIndex);
			jsonPBR.GetValue(S3MB_MATERIAL_PBR_OCCLUSIONTEXTURECOORDINDEX, pPBR->m_nOcclusionTextureCoordIndex);
			jsonPBR.GetValue(S3MB_MATERIAL_PBR_OCCLUSIONTEXTURESTRENGTH, pPBR->m_fOcclusionTextureStrength);
			jsonPBR.GetValue(S3MB_MATERIAL_PBR_MASKTEXTUREINDEX, pPBR->m_nMaskTextureIndex);
			jsonPBR.GetValue(S3MB_MATERIAL_PBR_MASKTEXTURECOORDINDEX, pPBR->m_nMaskTextureCoordIndex);
			jsonPBR.GetValue(S3MB_MATERIAL_PBR_SEMANTICTEXTUREINDEX, pPBR->m_nSemanticTextureIndex);
			jsonPBR.GetValue(S3MB_MATERIAL_PBR_SEMANTICTEXTURECOORDINDEX, pPBR->m_nSemanticTextureCoordIndex);
		}
	}

	void S3MBTools::LoadPBRFromJsonV3(JsonValue& jsonPBR, PBRParams*& pPBR)
	{
		JsonValue* pJsonSub = NULL;
		if (jsonPBR.GetValue(L"pbrMetallicRoughness", pJsonSub)
			&& pJsonSub != NULL)
		{
			PBRMetallicRough* pPBRMR = new PBRMetallicRough();
			pJsonSub->GetValue(S3MB_MATERIAL_PBRM_BASECOLOR, pPBRMR->m_vec4BaseColor);
			pJsonSub->GetValue(S3MB_MATERIAL_PBRM_BASECOLORTEXTUREINDEX, pPBRMR->m_nBaseColorTextureIndex);
			pJsonSub->GetValue(S3MB_MATERIAL_PBRM_BASECOLORTEXTURECOORDINDEX, pPBRMR->m_nBaseColorTextureCoordIndex);
			JsonValue * pJsonBaseColorTextureMotion = NULL;
			pJsonSub->GetValue(S3MB_MATERIAL_PBRM_BASECOLORTEXTUREMOTION, pJsonBaseColorTextureMotion);
			pPBRMR->m_BaseColorTextureMotion.FromJson(*pJsonBaseColorTextureMotion);
			pJsonSub->GetValue(S3MB_MATERIAL_PBRM_METALLICROUGHNESSTEXTUREINDEX, pPBRMR->m_nMetallicRoughnessTextureIndex);
			pJsonSub->GetValue(S3MB_MATERIAL_PBRM_METALLICROUGHNESSTEXTURECOORDINDEX, pPBRMR->m_nMetallicRoughnessTextureCoordIndex);
			pJsonSub->GetValue(S3MB_MATERIAL_PBRM_METALLICFACTOR, pPBRMR->m_fMetallicFactor);
			pJsonSub->GetValue(S3MB_MATERIAL_PBRM_ROUGHNESSFACTOR, pPBRMR->m_fRoughnessFactor);

			pPBR = pPBRMR;
		}
		if (pPBR != NULL)
		{
			pJsonSub->GetValue(S3MB_MATERIAL_PBR_EMISSIVEFACTOR, pPBR->m_vec3EmissiveFactor);
			pJsonSub->GetValue(S3MB_MATERIAL_PBR_EMISSIVETEXTUREINDEX, pPBR->m_nEmissiveTextureIndex);
			pJsonSub->GetValue(S3MB_MATERIAL_PBR_EMISSIVETEXTURECOORDINDEX, pPBR->m_nEmissiveTextureCoordIndex);
			JsonValue * pJsonEmissiveTextureMotion = NULL;
			pJsonSub->GetValue(S3MB_MATERIAL_PBR_EMISSIVETEXTUREMOTION, pJsonEmissiveTextureMotion);
			pPBR->m_EmissiveTextureMotion.FromJson(*pJsonEmissiveTextureMotion);
			pJsonSub->GetValue(S3MB_MATERIAL_PBR_NORMALTEXTUREINDEX, pPBR->m_nNormalTextureIndex);
			pJsonSub->GetValue(S3MB_MATERIAL_PBR_NORMALTEXTURECOORDINDEX, pPBR->m_nNormalTextureCoordIndex);
			pJsonSub->GetValue(S3MB_MATERIAL_PBR_NORMALTEXTURESCALE, pPBR->m_fNormalTextureScale);
			pJsonSub->GetValue(S3MB_MATERIAL_PBR_OCCLUSIONTEXTUREINDEX, pPBR->m_nOcclusionTextureIndex);
			pJsonSub->GetValue(S3MB_MATERIAL_PBR_OCCLUSIONTEXTURECOORDINDEX, pPBR->m_nOcclusionTextureCoordIndex);
			pJsonSub->GetValue(S3MB_MATERIAL_PBR_OCCLUSIONTEXTURESTRENGTH, pPBR->m_fOcclusionTextureStrength);
			pJsonSub->GetValue(S3MB_MATERIAL_PBR_MASKTEXTUREINDEX, pPBR->m_nMaskTextureIndex);
			pJsonSub->GetValue(S3MB_MATERIAL_PBR_MASKTEXTURECOORDINDEX, pPBR->m_nMaskTextureCoordIndex);
			pJsonSub->GetValue(S3MB_MATERIAL_PBR_SEMANTICTEXTUREINDEX, pPBR->m_nSemanticTextureIndex);
			pJsonSub->GetValue(S3MB_MATERIAL_PBR_SEMANTICTEXTURECOORDINDEX, pPBR->m_nSemanticTextureCoordIndex);
		}
	}

	void S3MBTools::LoadTUSFromJson(JsonValue& jsonTexture, TextureUnitState* pTextureUnitState)
	{
		JsonValue* pJsonContent = NULL;
		jsonTexture.GetValue(S3MB_MATERIAL_TEXTUNIT, pJsonContent);

		pJsonContent->GetValue(S3MB_TEXTURE_NAME, pTextureUnitState->m_strTextureName);
		pJsonContent->GetValue(S3MB_TEXTURE_URL, pTextureUnitState->m_strTexureURL);

		// AddressMode
		AddressModeFromJson(*pJsonContent, pTextureUnitState);

		// TextureFilteringOption
		int nOptionVal = 0;
		pJsonContent->GetValue(S3MB_MATERIAL_TEXTUNIT_FILTEROPTION, nOptionVal);
		pTextureUnitState->m_TextureFilteringOption = (FilterOptions)nOptionVal;
		pJsonContent->GetValue(S3MB_MATERIAL_TEXTUNIT_FILTERMIN, nOptionVal);
		pTextureUnitState->m_MinFilter = (FilterOptions)nOptionVal;
		pJsonContent->GetValue(S3MB_MATERIAL_TEXTUNIT_FILTERMAX, nOptionVal);
		pTextureUnitState->m_MaxFilter = (FilterOptions)nOptionVal;
		// TexMatrix
		pJsonContent->GetValue(S3MB_MATERIAL_TEXTUNIT_MATRIX, pTextureUnitState->m_TexModMatrix);

		delete pJsonContent;
		pJsonContent = NULL;
	}

	void S3MBTools::LoadTUSFromJsonV3(JsonValue& jsonTexture, TextureUnitState* pTextureUnitState)
	{
		JsonValue* pJsonContent = NULL;
		jsonTexture.GetValue(L"textureunitstate", pJsonContent);

		pJsonContent->GetValue(L"id", pTextureUnitState->m_strTextureName);
		pJsonContent->GetValue(S3MB_TEXTURE_URL, pTextureUnitState->m_strTexureURL);

		// AddressMode
		JsonValue *pJsonAddMode = NULL;
		pJsonContent->GetValue(L"addressmode", pJsonAddMode);
		AddressModeFromJson(*pJsonAddMode, pTextureUnitState);
		delete pJsonAddMode;
		pJsonAddMode = NULL;

		// TextureFilteringOption
		int nOptionVal = 0;
		pJsonContent->GetValue(S3MB_MATERIAL_TEXTUNIT_FILTEROPTION, nOptionVal);
		pTextureUnitState->m_TextureFilteringOption = (FilterOptions)nOptionVal;
		pJsonContent->GetValue(L"filtermin", nOptionVal);
		pTextureUnitState->m_MinFilter = (FilterOptions)nOptionVal;
		pJsonContent->GetValue(L"filtermax", nOptionVal);
		pTextureUnitState->m_MaxFilter = (FilterOptions)nOptionVal;

		// TexMatrix
		pJsonContent->GetValue(L"texmodmatrix", pTextureUnitState->m_TexModMatrix);

		delete pJsonContent;
		pJsonContent = NULL;
	}

	MaterialType S3MBTools::MaterialTypeFromString(wstring strType)
	{
		if (strType.compare(S3MB_MATERIAL_TYPE_COMMON) == 0)
		{
			return MAT_COMMON;
		}
		else if (strType.compare(S3MB_MATERIAL_TYPE_PBR) == 0)
		{
			return MAT_PBR;
		}
		return MAT_COMMON;
	}

	CullingMode S3MBTools::CullModeFromString(wstring strType)
	{
		CullingMode emType = CULL_NONE;
		if (strType.compare(S3MB_MATPASS_CULLMODE_NONE) == 0)
		{
			emType = CULL_NONE;
		}
		else if (strType.compare(S3MB_MATPASS_CULLMODE_C) == 0)
		{
			emType = CULL_CLOCKWISE;
		}
		else if (strType.compare(S3MB_MATPASS_CULLMODE_CC) == 0)
		{
			emType = CULL_ANTICLOCKWISE;
		}
		return emType;
	}

	S3MB::CullingMode S3MBTools::CullModeFromStringV3(wstring strType)
	{
		CullingMode emType = CULL_NONE;
		if (strType.compare(L"none") == 0)
		{
			emType = CULL_NONE;
		}
		else if (strType.compare(L"clockwise") == 0)
		{
			emType = CULL_CLOCKWISE;
		}
		else if (strType.compare(L"counterClockwise") == 0)
		{
			emType = CULL_ANTICLOCKWISE;
		}
		return emType;
	}

	AlphaMode S3MBTools::AlphaModeFromString(wstring strType)
	{

		AlphaMode emValue = PBRAM_UnKnown;
		if (strType.compare(S3MB_MATERIAL_PBR_ALPHAMODE_OPAQUE) == 0)
		{
			emValue = PBRAM_OPAQUE;
		}
		else if (strType.compare(S3MB_MATERIAL_PBR_ALPHAMODE_MASK) == 0)
		{
			emValue = PBRAM_MASK;
		}
		else if (strType.compare(S3MB_MATERIAL_PBR_ALPHAMODE_BLEND) == 0)
		{
			emValue = PBRAM_BLEND;
		}
		return emValue;
	}

	S3MB::AlphaMode S3MBTools::AlphaModeFromStringV3(wstring strType)
	{
		AlphaMode emValue = PBRAM_UnKnown;
		if (strType.compare(L"opaque") == 0)
		{
			emValue = PBRAM_OPAQUE;
		}
		else if (strType.compare(L"mask") == 0)
		{
			emValue = PBRAM_MASK;
		}
		else if (strType.compare(L"blend") == 0)
		{
			emValue = PBRAM_BLEND;
		}
		return emValue;
	}

	void S3MBTools::AddressModeFromJson(JsonValue& jsonAddMode, TextureUnitState* pTextureUnitState)
	{
		int nValue = 0;
		jsonAddMode.GetValue(S3MB_TEXTURE_U, nValue);
		pTextureUnitState->m_AddressMode.u = (TextureAddressingMode)nValue;
		jsonAddMode.GetValue(S3MB_TEXTURE_V, nValue);
		pTextureUnitState->m_AddressMode.v = (TextureAddressingMode)nValue;
		jsonAddMode.GetValue(S3MB_TEXTURE_W, nValue);
		pTextureUnitState->m_AddressMode.w = (TextureAddressingMode)nValue;
	}

	void S3MBTools::LoadTexture(MemoryStream& streamTexture, std::map<wstring, TextureDataInfo*>& mapTexture, float fVersion)
	{
		int nImageCount = 0;
		streamTexture >> nImageCount;

		for (int k = 0; k < nImageCount; k++)
		{
			TextureDataInfo* pTexture = new TextureDataInfo();
			wstring strTextureName;
			streamTexture >> strTextureName;
			pTexture->m_strName = strTextureName;
			StreamAlign(streamTexture, true);

			streamTexture >> pTexture->m_nLevel;
			pTexture->m_bMipmap = pTexture->m_nLevel > 0;

			TextureData* pTextureData = new TextureData();
			pTexture->m_pTextureData = pTextureData;

			LoadTextureData(streamTexture, pTextureData, fVersion);

			mapTexture[strTextureName] = pTexture;
		}
	}

	void S3MBTools::LoadTextureData(MemoryStream& streamTexture, TextureData* pTextureData, float fVersion)
	{
		streamTexture >> pTextureData->m_nWidth;
		streamTexture >> pTextureData->m_nHeight;
		if (EQUAL(fVersion, S3MB_VERSIONV3))
		{
			unsigned nCompressType, nFormat;
			streamTexture >> nCompressType;
			streamTexture >> pTextureData->m_nSize;
			streamTexture >> nFormat;
			ToStandardType(pTextureData->m_eCompressType, pTextureData->m_enFormat, nCompressType, nFormat);
		}
		else
		{
			streamTexture >> pTextureData->m_eCompressType;
			streamTexture >> pTextureData->m_nSize;
			streamTexture >> pTextureData->m_enFormat;
		}

		pTextureData->m_pBuffer = new(std::nothrow) unsigned char[pTextureData->m_nSize];
		if (pTextureData->m_pBuffer == NULL)
		{
			delete pTextureData;
			pTextureData = NULL;
			return;
		}
		streamTexture.Load(pTextureData->m_pBuffer, pTextureData->m_nSize);
	}

	void S3MBTools::ProcessTextureData(TextureData* pTextureData, TextureCompressType nCompressType, bool bGenerateMipmap/* = true*/)
	{
		if (pTextureData->m_eCompressType == nCompressType)
		{
			return;
		}

		unsigned int comp = 3;
		if (PixelFormat::PF_RGB8 != pTextureData->m_enFormat)
		{
			comp = 4;
		}

		unsigned char* pDecodeOut = NULL;
		unsigned int nDecodeSize = 0;
		if (pTextureData->m_eCompressType != TC_NONE)
		{
			nDecodeSize = ImageOperator::Decode(comp, pTextureData->m_nWidth, \
				pTextureData->m_nHeight, &pDecodeOut, \
				pTextureData->m_pBuffer, pTextureData->m_eCompressType);
		}
		else// NONE
		{
			if (comp == 3)// RGB->RGBA
			{
				PixelFormat btBitsPixel = PF_RGB8;
				unsigned char nAlpha = 255;

				int nWidth = pTextureData->m_nWidth;
				int nHeight = pTextureData->m_nHeight;
				void* pBits = pTextureData->m_pBuffer;

				int nWidthRGB = ImgToolkit::AlignWidth(PF_RGB8, nWidth);
				int nWidthRGBA = ImgToolkit::AlignWidth(PF_RGBA8, nWidth);
				nDecodeSize = nWidthRGBA * nHeight;
				unsigned char* pByte = new unsigned char[nDecodeSize];

				if (pTextureData->m_nSize != nWidthRGB * nHeight)
				{
					nWidthRGB = nWidth * 3;
				}

				for (unsigned int y = 0; y < (unsigned int)nHeight; y++)
				{
					for (unsigned int x = 0; x < (unsigned int)nWidth; x++)
					{
						unsigned int nValue = ImgToolkit::GetValue((unsigned char*)pBits, nWidthRGB, btBitsPixel, x, y);
						unsigned char nR = Palette::REDVAL(nValue);
						unsigned char nG = Palette::GREENVAL(nValue);
						unsigned char nB = Palette::BLUEVAL(nValue);
						nValue = Palette::RGBA(nR, nG, nB, nAlpha);

						ImgToolkit::SetValue(pByte, nWidthRGBA, PF_RGBA8, x, y, nValue);
					}
				}

				pDecodeOut = (unsigned char*)pByte;
				if (nCompressType != TextureCompressType::TC_DXT1_RGB)
				{
					comp = 4;
				}
			}
			else
			{
				unsigned int nDataSize = pTextureData->m_nWidth * pTextureData->m_nHeight * comp;
				pDecodeOut = new unsigned char[nDataSize];
				memcpy(pDecodeOut, pTextureData->m_pBuffer, nDataSize);				
			}
		}

		if (nCompressType != TC_NONE)
		{
			assert(pDecodeOut != NULL);

			unsigned char* pEncodeOut = NULL;
			unsigned int nCompressedTextureSize = ImageOperator::Encode(comp, \
				pTextureData->m_nWidth, pTextureData->m_nHeight, \
				pDecodeOut, &pEncodeOut, nCompressType,bGenerateMipmap);

			delete[] pDecodeOut;
			pDecodeOut = NULL;

			pDecodeOut = pEncodeOut;
			nDecodeSize = nCompressedTextureSize;
		}

		if (pDecodeOut != NULL)
		{
			delete pTextureData->m_pBuffer;
			pTextureData->m_pBuffer = pDecodeOut;
			pTextureData->m_nSize = nDecodeSize;
		}

		if (nCompressType == TextureCompressType::TC_DXT1_RGB && pTextureData->m_enFormat == PixelFormat::PF_RGBA8)
		{
			pTextureData->m_enFormat = PixelFormat::PF_RGB8;
		}
		else if (nCompressType == TextureCompressType::TC_DXT5 && pTextureData->m_enFormat == PixelFormat::PF_RGB8)
		{
			pTextureData->m_enFormat = PixelFormat::PF_RGBA8;			
		}
		pTextureData->m_eCompressType = nCompressType;
	}

	void S3MBTools::ToStandardType(TextureCompressType& eCompressType, PixelFormat& eFormat, unsigned nCompressType, unsigned nFormat)
	{
		switch (nCompressType)
		{
		case 0:
			eCompressType = TextureCompressType::TC_NONE;
			break;
		case 14:
			if (nFormat == 10 || nFormat == 11 || nFormat == 17)
			{
				eCompressType = TextureCompressType::TC_DXT1_RGB;
			}
			else
			{
				eCompressType = TextureCompressType::TC_DXT5;
			}
			break;
		case 22:
			eCompressType = TextureCompressType::TC_ETC1;
			break;
		case 24:
			eCompressType = TextureCompressType::TC_PVR;
			break;
		case 25:
			eCompressType = TextureCompressType::TC_WEBP;
			break;
		case 27:
			eCompressType = TextureCompressType::TC_CRN;
			break;
		case 30:
			eCompressType = TextureCompressType::TC_ETC2;
			break;
		case 31:
			eCompressType = TextureCompressType::TC_KTX2;
			break;
		default:
			eCompressType = TextureCompressType::TC_NONE;
			break;
		}

		switch (nFormat)
		{
		case 11:
			eFormat = PixelFormat::PF_RGB8;
			break;
		case 13:
			eFormat = PixelFormat::PF_RGBA8;
			break;
		case 24:
			eFormat = PixelFormat::PF_RGB32F;
			break;
		case 25:
			eFormat = PixelFormat::PF_RGBA32F;
			break;
		default:
			break;
		}
	}

	bool S3MBTools::SaveIDInfo(std::map<wstring, Skeleton*>& m_mapSkeleton, MemoryStream& m_streamIDInfo)
	{
		int nCount = m_mapSkeleton.size();
		if (nCount == 0)
		{
			return false;
		}

		bool bHasIDInfo = false;
		std::map<wstring, Skeleton*>::iterator it = m_mapSkeleton.begin();
		for (; it != m_mapSkeleton.end(); it++)
		{
			if (it->second->m_arrIDInfo.size() > 0)
			{
				bHasIDInfo = true;
				break;
			}
		}
		
		if (!bHasIDInfo)
		{
			return false;
		}

		m_streamIDInfo.Init();

		m_streamIDInfo << nCount;
		for (; it != m_mapSkeleton.end(); it++)
		{
			Skeleton* pSkeleton = it->second;
			VertexDataPackage* pVertexDataPackage = pSkeleton->m_pVertexDataPackage;		
			m_streamIDInfo << pSkeleton->m_strGeoName;
			m_streamIDInfo << (int)pSkeleton->m_arrIDInfo.size();
			for (auto& pIDinfo : pSkeleton->m_arrIDInfo)
			{
				m_streamIDInfo << pIDinfo->m_nID;
				if (pIDinfo->m_arrInstanceIndex.size() > 0)
				{
					m_streamIDInfo << (int)pIDinfo->m_arrInstanceIndex.size();
					for (auto iIdx : pIDinfo->m_arrInstanceIndex)
					{
						m_streamIDInfo << iIdx;
						m_streamIDInfo << (int)0;
					}
				}
				else if (pIDinfo->m_arrVertexColorOffsetAndCount.size() > 0)
				{
					m_streamIDInfo << (int)pIDinfo->m_arrVertexColorOffsetAndCount.size();
					for (auto iOffset : pIDinfo->m_arrVertexColorOffsetAndCount)
					{
						m_streamIDInfo << iOffset.first;
						m_streamIDInfo << iOffset.second;
					}
				}
				else
				{
					m_streamIDInfo << 0;
				}
			}
			
		}
		return true;
	}
	
	void S3MBTools::LoadBoundingBox(MemoryStream& stream, BoundingBox& bbox)
	{
		Vector3d vCenter, vExtent, vExtentX, vExtentY, vExtentZ;
		stream >> vCenter.x;
		stream >> vCenter.y;
		stream >> vCenter.z;

		stream >> vExtentX.x;
		stream >> vExtentX.y;
		stream >> vExtentX.z;

		stream >> vExtentY.x;
		stream >> vExtentY.y;
		stream >> vExtentY.z;

		stream >> vExtentZ.x;
		stream >> vExtentZ.y;
		stream >> vExtentZ.z;

		vExtent.x = vExtentX.x;
		vExtent.y = vExtentY.y;
		vExtent.z = vExtentZ.z;

		bbox.SetMin(vCenter - vExtent / 2.0);
		bbox.SetMax(vCenter + vExtent / 2.0);
	}

	void S3MBTools::SaveBoundingBox(const BoundingBox& bbox, MemoryStream& stream)
	{
		double dZero = 0.0;
		Vector3d vCenter = bbox.GetCenter();
		Vector3d vExtent = bbox.GetMax() - bbox.GetMin();

		stream << vCenter.x;
		stream << vCenter.y;
		stream << vCenter.z;

		stream << vExtent.x;
		stream << dZero;
		stream << dZero;

		stream << dZero;
		stream << vExtent.y;
		stream << dZero;

		stream << dZero;
		stream << dZero;
		stream << vExtent.z;
	}

	void S3MBTools::SaveOBB(const OrientedBoundingBox& bBox, MemoryStream& streamBBox)
	{
		Vector3d vec3dCenter = bBox.GetCenter();
		streamBBox << vec3dCenter.x;
		streamBBox << vec3dCenter.y;
		streamBBox << vec3dCenter.z;

		Matrix3d matHalfAxes = bBox.GetHalfAxes();
		Vector3d extAxis0 = matHalfAxes.GetColumn(0);
		Vector3d extAxis1 = matHalfAxes.GetColumn(1);
		Vector3d extAxis2 = matHalfAxes.GetColumn(2);

		streamBBox << extAxis0.x;
		streamBBox << extAxis0.y;
		streamBBox << extAxis0.z;

		streamBBox << extAxis1.x;
		streamBBox << extAxis1.y;
		streamBBox << extAxis1.z;

		streamBBox << extAxis2.x;
		streamBBox << extAxis2.y;
		streamBBox << extAxis2.z;
	}

	void S3MBTools::LoadOBB(MemoryStream& streamBBox, OrientedBoundingBox& bBox)
	{
		Vector3d vec3dCenter;
		streamBBox >> vec3dCenter.x;
		streamBBox >> vec3dCenter.y;
		streamBBox >> vec3dCenter.z;

		Matrix3d matHalfAxes;
		Vector3d vExtent;
		streamBBox >> vExtent.x;
		streamBBox >> vExtent.y;
		streamBBox >> vExtent.z;
		matHalfAxes.SetColumn(0, vExtent);

		streamBBox >> vExtent.x;
		streamBBox >> vExtent.y;
		streamBBox >> vExtent.z;
		matHalfAxes.SetColumn(1, vExtent);

		streamBBox >> vExtent.x;
		streamBBox >> vExtent.y;
		streamBBox >> vExtent.z;
		matHalfAxes.SetColumn(2, vExtent);
		bBox = OrientedBoundingBox(vec3dCenter, matHalfAxes);
	}

	bool S3MBTools::SaveStreamData2File(const wstring strDestFilePath, MemoryStream& streamShell, MemoryStream& streamSkeleton, \
		wstring& strJsonMaterials, MemoryStream& streamTexture, bool bHasIDInfo, MemoryStream& streamIDInfo, wstring& strExtensions, float fVersion)
	{
		// 改后缀名，上层处理可能会传入其他类型文件后缀
		wstring strDestFilePathInner = StringUtil::ChangeExt(strDestFilePath, FILE_EXT_S3MB);
#ifdef WIN32
        std::ofstream ofs(strDestFilePathInner, ios::out | ios::binary | ios::trunc);
#else
        string DestFilePathInner = StringUtil::UnicodeToANSI(strDestFilePathInner);
        std::ofstream ofs(DestFilePathInner, ios::out | ios::binary | ios::trunc);
#endif

		// s3mb version
		ofs.write((char*)&fVersion, sizeof(fVersion));

		// 流合并
		MemoryStream memStreamTotal;
		memStreamTotal.Init();
		unsigned int nOptions = bHasIDInfo ? SVO_HasIDInfo : 0;
		memStreamTotal << nOptions;

		SaveStream(memStreamTotal, streamShell);
		SaveStream(memStreamTotal, streamSkeleton);
		SaveStream(memStreamTotal, streamTexture);
		memStreamTotal << strJsonMaterials;

		if (bHasIDInfo)
		{
			unsigned int nIDInfoOption = 1;
			memStreamTotal << nIDInfoOption;		
			SaveStream(memStreamTotal, streamIDInfo);
		}
		memStreamTotal << strExtensions;

		// 压缩流
		unsigned char* pZippedData = NULL;
		unsigned int nZippedSize = 0;
		unsigned int nTotalLength = (unsigned int)memStreamTotal.GetLength();
		pZippedData = new unsigned char[nTotalLength];
		nZippedSize = nTotalLength;
		bool result = Utils::Zip(pZippedData, nZippedSize, memStreamTotal.GetDataPtr(), nTotalLength);

		// 压缩类型
		// 0表示不压缩，1表示zip压缩
		unsigned int nCompressedType = 1;
		ofs.write((char*)&nCompressedType, sizeof(unsigned int));
		
		// 压缩前数据大小
		ofs.write((char*)&nTotalLength, sizeof(unsigned int));
		// 压缩后数据大小
		ofs.write((char*)&nZippedSize, sizeof(unsigned int));
		ofs.write((char*)pZippedData, nZippedSize);
		
		ofs.close();
		delete pZippedData;
		return true;
	}

	void S3MBTools::LoadShell(MemoryStream& streamShell, RenderOperationGroup* pROGroup, float fVersion)
	{
		int nCount = 0;
		streamShell >> nCount;

		BoundingSphere sphere;

		for (int i = 0; i < nCount; i++)
		{
			BoundingSphere sphereTemp;
			if (IsSingleGeode(streamShell))
			{
				RenderOperationGeode* pGeode = new RenderOperationGeode();
				pROGroup->AddChild(pGeode);
				LoadSingleGeode(pROGroup, pGeode, streamShell, fVersion);
				sphereTemp = pGeode->GetBoundingSphere();
			}
			else
			{
				RenderOperationPagedLOD* pPagedLOD = new RenderOperationPagedLOD();
				pPagedLOD->SetParentNode(pROGroup);
				pROGroup->AddChild(pPagedLOD);
				LoadPagedLOD(pROGroup, pPagedLOD, streamShell, fVersion);

				sphereTemp = pPagedLOD->GetBoundingSphere();
			}
			sphere.ExpandBy(sphereTemp);
		}
		pROGroup->SetBoundingSphere(sphere.m_center, sphere.m_radius);

		StreamAlign(streamShell, true);
	}

	bool S3MBTools::IsSingleGeode(MemoryStream& stream)
	{
		long lPos = stream.GetReadPosition();
		float dbDis = 0.0;
		stream >> dbDis;
		stream.SetReadPosition(lPos);
		return IS0(dbDis);
	}

	void S3MBTools::LoadSingleGeode(RenderOperationGroup* pROGroup, RenderOperationGeode* pGeode, MemoryStream& stream, float fVersion)
	{
		// 1	LODDistance 
		float dbDis = 0.0;
		stream >> dbDis;

		// 2	RangeMode 
		unsigned short uRangeMode = Pixel_Size_OnScreen;
		stream >> uRangeMode;

		// 3	包围球 
		Vector3d boundingSphereCenter;
		double dRadius = 0.0;
		stream >> boundingSphereCenter.x;
		stream >> boundingSphereCenter.y;
		stream >> boundingSphereCenter.z;
		stream >> dRadius;

		// 有向包围盒
		OrientedBoundingBox obb;
		S3MBTools::LoadOBB(stream, obb);
		

		// 4	挂接的下层文件
		string strChildTile;
		stream >> strChildTile;
		assert(strChildTile.empty());

		// 4	Geode(s)
		int nCount = 0;
		stream >> nCount;
		assert(nCount == 1);// 因为生成的时候每一个Geode被打包成了dbDis=0.0的一个PagedLOD
		for (int i = 0; i < nCount; i++)
		{
			LoadGeode(pROGroup, pGeode, stream);
		}

		// 动画
		wstring strAnimations;
		stream >> strAnimations;
		
	}

	void S3MBTools::LoadPagedLOD(RenderOperationGroup* pROGroup, RenderOperationPagedLOD* pROPagedLOD, MemoryStream& stream, float fVersion)
	{
		// 1	LODDistance 
		float dbDis = 0.0;
		stream >> dbDis;

		// 2	RangeMode 
		unsigned short uRangeMode = Pixel_Size_OnScreen;
		stream >> uRangeMode;
		pROPagedLOD->SetRangeMode(RangeMode(uRangeMode));

		if (uRangeMode == Pixel_Size_OnScreen)
		{
			pROPagedLOD->GetRanges().push_back(std::vector<std::pair<float, float> >::value_type(0, dbDis));
		}
		else
		{
			pROPagedLOD->GetRanges().push_back(std::vector<std::pair<float, float> >::value_type(9999.0, dbDis));
		}

		// 3	包围球 
		Vector3d boundingSphereCenter;
		double dRadius = 0.0;
		stream >> boundingSphereCenter.x;
		stream >> boundingSphereCenter.y;
		stream >> boundingSphereCenter.z;
		stream >> dRadius;
		pROPagedLOD->SetBoundingSphere(boundingSphereCenter, dRadius);

		// 有向包围盒
		OrientedBoundingBox obb;
		S3MBTools::LoadOBB(stream, obb);

		// 4	挂接的下层文件
		wstring strChildTile;
		stream >> strChildTile;
		if (!strChildTile.empty())
		{
			pROPagedLOD->GetFileNames().push_back(strChildTile);
		}

		// 4	Geode(s)
		int nCount = 0;
		stream >> nCount;

		for (int i = 0; i < nCount; i++)
		{
			RenderOperationGeode* pGeode = new RenderOperationGeode;
			pROPagedLOD->AddChild(pGeode);
			LoadGeode(pROGroup, pGeode, stream);
		}

		// 动画
		wstring strAnimations;
		stream >> strAnimations;

		JsonValue jsonAnimations;
		if (jsonAnimations.LoadValueFromString(strAnimations))
		{
			S3MBTools::AnimationsFromJson(jsonAnimations, pROPagedLOD);
		}	
	}

	void S3MBTools::LoadGeode(RenderOperationGroup* pROGroup, RenderOperationGeode* pGeode, MemoryStream& stream)
	{
		Matrix4d matGeode;
		stream.Load(matGeode.GetPointer(), 16);
		pGeode->SetPosition(matGeode);

		int nCount = 0;
		stream >> nCount;
		wstring strSkeletonName;
		for (int i = 0; i < nCount; i++)
		{
			stream >> strSkeletonName;
			Skeleton* pSkeleton = pROGroup->GetSkeleton(strSkeletonName);
			assert(pSkeleton != NULL);
			pGeode->AddSkeleton(pSkeleton);
		}
		pGeode->ComputerBoundingBox();
	}

	void S3MBTools::LoadIDInfo(std::map<wstring, Skeleton*>& mapSkeleton, MemoryStream& streamIDInfo, float fVersion)
	{
		int nCount;
		streamIDInfo >> nCount;

		for (int i = 0; i < nCount; i++)
		{
			wstring strGeoName;
			streamIDInfo >> strGeoName;
			int nSelectInfoCount;
			streamIDInfo >> nSelectInfoCount;

			std::map<wstring, Skeleton*>::iterator iterator = mapSkeleton.find(strGeoName);
			Skeleton* pSkeleton = iterator->second;
			for (int j = 0; j < nSelectInfoCount; j++)
			{
				IDInfo* pDInfo = new IDInfo;

				streamIDInfo >> pDInfo->m_nID;

				int nSize;
				streamIDInfo >> nSize;

				for (int k = 0; k < nSize; k++)
				{
					if (iterator->second->m_bInstanceBatch)
					{
						int nInstanceIndex;
						streamIDInfo >> nInstanceIndex;
						// 和非实例化保持结构一致
						int nInstanceCount = 0;
						streamIDInfo >> nInstanceCount;
						
						pDInfo->m_arrInstanceIndex.push_back(nInstanceIndex);
					}
					else
					{
						std::pair<int, int> pair;
						streamIDInfo >> pair.first;
						streamIDInfo >> pair.second;
						pDInfo->m_arrVertexColorOffsetAndCount.push_back(pair);
					}
				}

				pSkeleton->AddIDInfo(pDInfo);
			}
		}
	}

	void S3MBTools::SaveShell(RenderOperationGroup* pROGroup, MemoryStream& stream, float fVersion)
	{
		stream.Init();
		int nCount = pROGroup->GetNumChildren();
		stream << nCount;

		for (int i = 0; i < nCount; i++)
		{
			RenderOperationNode* pChildNode = pROGroup->GetChild(i);
			if (pChildNode == NULL)
			{
				assert(false);
				continue;
			}
			wstring strName = pChildNode->ClassName();
			if (StringUtil::CompareNoCase(strName, S3MB_SHELL_PAGEDLOD))
			{
				S3MBTools::SavePagedLOD((RenderOperationPagedLOD*)pChildNode, stream, fVersion);
			}
			else if (StringUtil::CompareNoCase(strName, S3MB_SHELL_GEODE))
			{
				float dLODDistance = 0.0;
				stream << dLODDistance;

				unsigned short uRangeMode = Pixel_Size_OnScreen;
				stream << uRangeMode;

				RenderOperationGeode* pGeode = (RenderOperationGeode*)pChildNode;
				BoundingSphere bsphere = pChildNode->GetBoundingSphere();
				if (!bsphere.valid())
				{
					pGeode->ComputerBoundingBox();
					bsphere = pGeode->GetBoundingSphere();
				}
				Vector3d vCenter = bsphere.GetCenter();
				stream << vCenter.x;
				stream << vCenter.y;
				stream << vCenter.z;
				stream << bsphere.GetRadius();

				OrientedBoundingBox mOBB = pGeode->GetOrientedBoundingBox();
				S3MBTools::SaveOBB(mOBB, stream);
				

				wstring strChildTile;
				stream << strChildTile;

				int nGeodeCount = 1;
				stream << nGeodeCount;

				S3MBTools::SaveGeode(pGeode, stream);

				// Animations
				wstring strAnimations;
				stream << strAnimations;			
			}
		}
		S3MBTools::StreamAlign(stream, false);
	}

	void S3MBTools::SavePagedLOD(RenderOperationPagedLOD* pROPagedLOD, MemoryStream& stream, float fVersion)
	{
		// 1	LODDistance 
		float dLODDistance = pROPagedLOD->GetRanges().size() > 0 ? \
			pROPagedLOD->GetRanges()[0].second : -1.0;

		unsigned short uRangeMode = pROPagedLOD->GetRangeMode();
		BoundingSphere boundingSphere = pROPagedLOD->GetBoundingSphere();

		stream << dLODDistance;

		// 2	RangeMode 
		stream << uRangeMode;

		// 3	包围球 
		Vector3d boundingSphereCenter = boundingSphere.GetCenter();
		stream << boundingSphereCenter.x;
		stream << boundingSphereCenter.y;
		stream << boundingSphereCenter.z;
		stream << boundingSphere.GetRadius();

		// 有向包围盒
		OrientedBoundingBox mOBB = pROPagedLOD->GetOrientedBoundingBox();
		S3MBTools::SaveOBB(mOBB, stream);
		

		// 4	挂接的下层文件
		wstring strChildTile = pROPagedLOD->GetFileNames().size() > 0 ? \
			pROPagedLOD->GetFileNames()[0] : U("");
		if (!strChildTile.empty() && !StringUtil::CompareNoCase(StringUtil::GetExt(strChildTile), U(".s3mb")))
		{
			strChildTile = StringUtil::ChangeExt(strChildTile, U(".s3mb"));
		}
		stream << strChildTile;

		// 4	Geode(s)
		int nChildCount = pROPagedLOD->GetNumChildren();
		stream << nChildCount;
		for (int i = 0; i < nChildCount; i++)
		{
			RenderOperationNode* pChildNode = pROPagedLOD->GetChild(i);
			wstring strName = pChildNode->ClassName();
			if (StringUtil::CompareNoCase(strName, U("PagedLOD")))
			{
				assert(false);
			}
			else  if (StringUtil::CompareNoCase(strName, U("Geode")))
			{
				S3MBTools::SaveGeode((RenderOperationGeode*)pChildNode, stream);
			}
		}

		JsonValue jsonAnimations(JsonValueType::objectValue);
		S3MBTools::AnimationsToJson(pROPagedLOD, jsonAnimations);
		// 动画
		string strAnimations;
		jsonAnimations.SaveToString(strAnimations);
		stream << strAnimations;	
	}

	void S3MBTools::SaveGeode(RenderOperationGeode* pROGeode, MemoryStream& stream)
	{
		// 1、矩阵
		Matrix4d matGeode = pROGeode->GetPosition();
		stream.Save(matGeode.GetPointer(), 16);
		// 2、骨架
		int nCount = pROGeode->GetNumSkeleton();
		stream << nCount;
		Skeleton* pSkeleton = NULL;
		for (int i = 0; i < nCount; i++)
		{
			pSkeleton = pROGeode->GetSkeleton(i);
			stream << pSkeleton->m_strGeoName;
		}
	}

	void S3MBTools::SaveSkeleton(std::map<wstring, Skeleton*>& mapSkeleton, MemoryStream& streamSkeleton, SkeletonCompressParam& SkeletonCompParam, float fVersion)
	{
		streamSkeleton.Init();
		int nCount = mapSkeleton.size();
		streamSkeleton << nCount;

		std::map<wstring, Skeleton*>::iterator itSkeleton;
		for (itSkeleton = mapSkeleton.begin(); itSkeleton != mapSkeleton.end(); itSkeleton++)
		{
			MemoryStream streamPerSkeleton;
			streamPerSkeleton.Init();
			streamPerSkeleton << itSkeleton->first;
			S3MBTools::StreamAlign(streamPerSkeleton, false);

			Skeleton* pSkeleton = itSkeleton->second;

			VertexDataPackage* pVertexDataPackage = pSkeleton->m_pVertexDataPackage;
			S3MBTools::SaveSkeletonData(pVertexDataPackage, pSkeleton->m_arrIndexPackage, streamPerSkeleton, SkeletonCompParam, fVersion);
			// 存储方向包围盒
			OrientedBoundingBox obb = pSkeleton->m_OBB;
			if (obb.IsNULL())
			{
				pSkeleton->ComputerBoundingBox();
				obb = pSkeleton->m_OBB;
			}
			S3MBTools::SaveOBB(obb, streamPerSkeleton);
			S3MBTools::SaveStream(streamSkeleton, streamPerSkeleton);
		}
	}

	bool S3MBTools::SaveSkeletonData(VertexDataPackage*& pVertexDataPackage, vector<IndexPackage*>& arrIndexPackage, MemoryStream& streamSkeleton, SkeletonCompressParam& SkeletonCompParam, float fVersion)
	{
		unsigned int nOptions = 0;
		if (SkeletonCompParam.m_svTag == SV_Standard ||
			SkeletonCompParam.m_svTag == SV_DracoCompressed && !CanUseDraco(pVertexDataPackage, arrIndexPackage))
		{
			nOptions = 0;
			streamSkeleton << nOptions;

			MemoryStream streamVertexPackage;
			streamVertexPackage.Init();
			SaveVertex(pVertexDataPackage, streamVertexPackage);
			SaveNormal(pVertexDataPackage, streamVertexPackage);
			SaveVertexColor(pVertexDataPackage, streamVertexPackage);
			SaveTextureCoords(pVertexDataPackage, streamVertexPackage);
			SaveInstanceInfo(pVertexDataPackage, streamVertexPackage);
			SaveVertexAttributeExtension(pVertexDataPackage, streamVertexPackage);
			SaveTangent(pVertexDataPackage, streamVertexPackage);
			SaveStream(streamSkeleton, streamVertexPackage);

			SaveIndexPackages(arrIndexPackage, streamSkeleton, fVersion);
		}
		else
		{
			nOptions = 1;
			streamSkeleton << nOptions;
			pVertexDataPackage->m_nCompressOptions = SkeletonCompParam.m_nCompressOption;

			MemoryStream streamDataPackage;
			streamDataPackage.Init();
			SaveDraco(pVertexDataPackage, arrIndexPackage, SkeletonCompParam.m_dracoParam, streamDataPackage, fVersion);
			SaveInstanceInfo(pVertexDataPackage, streamDataPackage);
			SaveStream(streamSkeleton, streamDataPackage);
		}	
		return true;
	}

	bool S3MBTools::SaveDraco(VertexDataPackage* pVertexDataPack, std::vector<IndexPackage*>& arrIndexPackage, const DracoCompressParam& dracoParam, MemoryStream &stream, float fVersion)
	{
		char* pChar = NULL;
		unsigned int nSize = 0;
		DracoCompressedInfo dracoCompressedInfo;
		if (!DracoTool::DracoCompress(dracoParam, dracoCompressedInfo, pVertexDataPack, arrIndexPackage, pChar, nSize))
		{
			if (pChar != NULL)
			{
				delete pChar;
				pChar = NULL;
			}
			return false;
		}

		stream << pVertexDataPack->m_nCompressOptions;

		stream << dracoCompressedInfo.m_posInfo.GetUniqueID();
		stream << dracoCompressedInfo.m_normalInfo.GetUniqueID();
		stream << dracoCompressedInfo.m_colorInfo.GetUniqueID();

		unsigned short nTextureCoord = 0;
		for (int i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
		{
			if (dracoCompressedInfo.m_texCoordInfo[i].GetUniqueID() == -1)
			{
				break;
			}
			nTextureCoord++;
		}

		stream << (unsigned int)nTextureCoord;

		for (int i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
		{
			if (dracoCompressedInfo.m_texCoordInfo[i].GetUniqueID() == -1)
			{
				break;
			}
			stream << dracoCompressedInfo.m_texCoordInfo[i].GetUniqueID();
		}

		stream << dracoCompressedInfo.m_nVertexAttInfoCount;
		for (int i = 0; i < dracoCompressedInfo.m_nVertexAttInfoCount; i++)
		{
			stream << dracoCompressedInfo.m_pVertexAttInfo[i].GetUniqueID();
		}
		

		// dracoMesh存储顶点和索引不存储索引对应的材质名字。因此需要单独存储索引对应的材质名字
		int nIndexPackageCount = arrIndexPackage.size();
		stream << nIndexPackageCount;
		for (unsigned int i = 0; i < arrIndexPackage.size(); i++)
		{
			IndexPackage* pIndexPackage = arrIndexPackage[i];
			stream << pIndexPackage->m_strPassName[0];
		}

		// 四字节对齐
		StreamAlign(stream, false);
		

		stream << nSize;
		stream.Save(pChar, nSize);

		// 四字节对齐
		StreamAlign(stream, false);
		
		if (pChar != NULL)
		{
			delete pChar;
			pChar = NULL;
		}

		std::map<wstring, int> mapVertexAttributeDescript = pVertexDataPack->m_mapVertexAttributeDescript;
		JsonValue jsonVertexAtts(JsonValueType::objectValue);
		std::map<wstring, int>::iterator itor = mapVertexAttributeDescript.begin();
		for (; itor != mapVertexAttributeDescript.end(); itor++)
		{
			jsonVertexAtts.Add(itor->first, itor->second);
		}

		string strJson;
		jsonVertexAtts.SaveToString(strJson);
		stream << StringUtil::UTF8_to_UNICODE(strJson);

		// 字节对齐
		StreamAlign(stream, false);

		return true;
	}

	bool S3MBTools::SaveVertex(VertexDataPackage* pVertexDataPack, MemoryStream& stream)
	{
		// unsigned int 
		stream << pVertexDataPack->m_nVerticesCount;
		if (pVertexDataPack->m_nVerticesCount <= 0 || pVertexDataPack->m_pVertices == NULL)
		{
			return true;
		}
		// unsigned short 
		stream << pVertexDataPack->m_nVertexDimension;
		// unsigned short
		stream << pVertexDataPack->m_nVertexStride;

		stream.Save(pVertexDataPack->m_pVertices, \
			pVertexDataPack->m_nVerticesCount * pVertexDataPack->m_nVertexDimension);

		return true;
	}

	bool S3MBTools::SaveNormal(VertexDataPackage* pVertexDataPack, MemoryStream& stream)
	{
		// unsigned int
		stream << pVertexDataPack->m_nNormalCount;

		if (pVertexDataPack->m_nNormalCount <= 0 || pVertexDataPack->m_pNormals == NULL)
		{
			return true;
		}
		// unsigned short 
		stream << pVertexDataPack->m_nNormalDimension;
		stream << pVertexDataPack->m_nNormalStride;

		stream.Save(pVertexDataPack->m_pNormals, \
			pVertexDataPack->m_nNormalCount * pVertexDataPack->m_nNormalDimension);

		return true;
	}

	bool S3MBTools::SaveTangent(VertexDataPackage* pVertexDataPack, MemoryStream& stream)
	{
		// unsigned int
		stream << pVertexDataPack->m_nTangentCount;

		if (pVertexDataPack->m_nTangentCount <= 0 || pVertexDataPack->m_pTangents == NULL)
		{
			return true;
		}
		// unsigned short 
		stream << pVertexDataPack->m_nTangentDimension;
		stream << pVertexDataPack->m_nTangentStride;

		stream.Save(pVertexDataPack->m_pTangents, \
			pVertexDataPack->m_nTangentCount * pVertexDataPack->m_nTangentDimension);

		return true;
	}

	bool S3MBTools::SaveVertexColor(VertexDataPackage* pVertexDataPack, MemoryStream& stream)
	{
		stream << pVertexDataPack->m_nVertexColorCount;
		if (pVertexDataPack->m_nVertexColorCount <= 0 || pVertexDataPack->m_pVertexColor == NULL)
		{
			return true;
		}
		unsigned char nByte = 0;
		stream << pVertexDataPack->m_nVertexColorStride;
		stream << nByte;
		stream << nByte;

		for (unsigned int j = 0; j < pVertexDataPack->m_nVertexColorCount; j++)
		{
			unsigned int nColor = pVertexDataPack->m_pVertexColor[j];
			unsigned char btR = Palette::REDVAL(nColor);
			unsigned char btG = Palette::GREENVAL(nColor);
			unsigned char btB = Palette::BLUEVAL(nColor);
			unsigned char btA = Palette::ALPHAVAL(nColor);

			stream << btR;
			stream << btG;
			stream << btB;
			stream << btA;
		}
		return true;
	}

	bool S3MBTools::SaveTextureCoords(VertexDataPackage* pVertexDataPack, MemoryStream& stream)
	{
		unsigned short nTextureCoord = 0;
		for (int i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
		{
			if (pVertexDataPack->m_TexCoordCount[i] == 0)
			{
				break;
			}		
			nTextureCoord++;
		}

		stream << nTextureCoord;
		unsigned char nByte = 0;
		stream << nByte;
		stream << nByte;
		for (int i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
		{
			if (pVertexDataPack->m_TexCoordCount[i] == 0)
			{
				break;
			}
			stream << pVertexDataPack->m_TexCoordCount[i];
			stream << pVertexDataPack->m_nTexDimensions[i];
			stream << pVertexDataPack->m_TexCoordStride[i];
			stream.Save(pVertexDataPack->m_pTexCoords[i], \
				pVertexDataPack->m_TexCoordCount[i] * pVertexDataPack->m_nTexDimensions[i]);
			
		}
		return true;
	}

	bool S3MBTools::SaveInstanceInfo(VertexDataPackage* pVertexDataPack, MemoryStream& stream)
	{
		unsigned char nByte = 0;
		if (pVertexDataPack->m_vecInstanceInfo.size() == 0)
		{
			stream << (unsigned short)0;
			stream << nByte;
			stream << nByte;
			return true;

		}
		else
		{
			stream << (unsigned short)1;
			stream << nByte;
			stream << nByte;

		}
		stream << (unsigned int)pVertexDataPack->m_vecInstanceInfo.size();
		for (auto& iInfo : pVertexDataPack->m_vecInstanceInfo)
		{
			stream << iInfo->nCount;
			stream << nByte;
			stream << nByte;
			// 3*4变换矩阵，一个材质颜色
			stream.Save(iInfo->pMatrixValues, iInfo->nCount);
		}
		return true;
	}
	
	void S3MBTools::SaveMaterial(std::map<wstring, Material*>& mapMaterial, JsonValue& jsonMaterials,float fVersion)
	{
		JsonValue jsonArray(arrayValue);
		std::map<wstring, Material*>::iterator it;
		for (it = mapMaterial.begin(); it != mapMaterial.end(); it++)
		{
			JsonValue jsonMaterial(objectValue);
			S3MBTools::MaterialToJson(it->second, jsonMaterial, fVersion);

			jsonArray.Add(jsonMaterial);
		}

		jsonMaterials.Add(S3MB_MATERIALS, jsonArray);
	}
	void S3MBTools::MaterialToJson(Material* pMaterial, JsonValue& jsonMaterial, float fVersion)
	{
		if (pMaterial == NULL)
		{
			assert(false);
			return;
		}
		JsonValue jsonContent(objectValue);
		Technique* pTechnique = pMaterial->getTechnique(0);
		S3MBTools::TechniqueToJson(pMaterial->m_strName, pTechnique, jsonContent, fVersion);

		jsonMaterial.Add(S3MB_MATERIAL, jsonContent);
	}
	void S3MBTools::TechniqueToJson(const wstring strMaterialName, Technique* pTechnique, JsonValue& jsonContent,float fVersion)
	{			
		jsonContent.Add(S3MB_NAME, strMaterialName);
		Pass* pPass = pTechnique->getPass(0);
		if (pPass->m_pPRBMaterial == nullptr)
		{
			jsonContent.Add(S3MB_MATERIAL_TYPE, S3MBTools::MaterialTypeToString(MaterialType::MAT_COMMON));
			jsonContent.Add(S3MB_MATERIAL_AMBIENT, pPass->m_Ambient);
			jsonContent.Add(S3MB_MATERIAL_DIFFUSE, pPass->m_Diffuse);
			jsonContent.Add(S3MB_MATERIAL_SPECULAR, pPass->m_Specular);
			jsonContent.Add(S3MB_MATERIAL_SHININESS, pPass->m_Shininess);
		}
		else
		{
			jsonContent.Add(S3MB_MATERIAL_TYPE, S3MBTools::MaterialTypeToString(MaterialType::MAT_PBR));
			PBRToJson(pPass->m_pPRBMaterial, jsonContent);
		}

		jsonContent.Add(S3MB_MATPASS_CULLMODE, S3MBTools::CullModeToString(pPass->m_CullMode));
		jsonContent.Add(S3MB_MATERIAL_PBR_ALPHAMODE, S3MBTools::AlphaModeToString(pPass->m_AlphaMode));
		jsonContent.Add(S3MB_MATERIAL_PBR_ALPHACUTOFF, pPass->m_fAlphaCutoff);

		// shader name
		jsonContent.Add(S3MB_MATERIAL_SHADERNAME_VERTEX, pPass->strVertexProgram);
		jsonContent.Add(S3MB_MATERIAL_SHADERNAME_FRAGMENT, pPass->strFragmentProgram);

		// TextureUnitStates
		JsonValue jsonArrayTextUnit(JsonValueType::arrayValue);
		int nTextureSize = pPass->GetTextureUnitStatesSize();
		for (int i = 0; i < nTextureSize; i++)
		{
			TextureUnitState* pTexUnit = pPass->GetTextureUnitState(i);
			JsonValue jsonTextUint(JsonValueType::objectValue);
			TUSToJson(pTexUnit, jsonTextUint);

			jsonArrayTextUnit.Add(jsonTextUint);
		}
		jsonContent.Add(S3MB_MATERIAL_TEXTUNITS, jsonArrayTextUnit);

		// Extensions
		jsonContent.Add(S3MB_MATERIAL_EXTENSIONS, pPass->m_strExtensions);
		
	}
	wstring S3MBTools::MaterialTypeToString(MaterialType emType)
	{
		wstring strType;
		if (emType == MaterialType::MAT_COMMON)
		{
			strType = S3MB_MATERIAL_TYPE_COMMON;
		}
		else if (emType == MaterialType::MAT_PBR)
		{
			strType = S3MB_MATERIAL_TYPE_PBR;
		}
		return strType;
	}

	void S3MBTools::PBRToJson(PBRParams* pPBR, JsonValue& jsonPBR)
	{
		if (pPBR == NULL)
		{
			return;
		}

		jsonPBR.Add(S3MB_MATERIAL_PBR_EMISSIVEFACTOR, pPBR->m_vec3EmissiveFactor);
		jsonPBR.Add(S3MB_MATERIAL_PBR_EMISSIVETEXTUREINDEX, pPBR->m_nEmissiveTextureIndex);
		jsonPBR.Add(S3MB_MATERIAL_PBR_EMISSIVETEXTURECOORDINDEX, pPBR->m_nEmissiveTextureCoordIndex);
		JsonValue jsonEmissiveTextureMotion(JsonValueType::objectValue,true);
		pPBR->m_EmissiveTextureMotion.ToJson(jsonEmissiveTextureMotion);
		jsonPBR.Add(S3MB_MATERIAL_PBR_EMISSIVETEXTUREMOTION, jsonEmissiveTextureMotion);
		jsonPBR.Add(S3MB_MATERIAL_PBR_NORMALTEXTUREINDEX, pPBR->m_nNormalTextureIndex);
		jsonPBR.Add(S3MB_MATERIAL_PBR_NORMALTEXTURECOORDINDEX, pPBR->m_nNormalTextureCoordIndex);
		jsonPBR.Add(S3MB_MATERIAL_PBR_NORMALTEXTURESCALE, pPBR->m_fNormalTextureScale);
		jsonPBR.Add(S3MB_MATERIAL_PBR_OCCLUSIONTEXTUREINDEX, pPBR->m_nOcclusionTextureIndex);
		jsonPBR.Add(S3MB_MATERIAL_PBR_OCCLUSIONTEXTURECOORDINDEX, pPBR->m_nOcclusionTextureCoordIndex);
		jsonPBR.Add(S3MB_MATERIAL_PBR_OCCLUSIONTEXTURESTRENGTH, pPBR->m_fOcclusionTextureStrength);
		jsonPBR.Add(S3MB_MATERIAL_PBR_MASKTEXTUREINDEX, pPBR->m_nMaskTextureIndex);
		jsonPBR.Add(S3MB_MATERIAL_PBR_MASKTEXTURECOORDINDEX, pPBR->m_nMaskTextureCoordIndex);
		jsonPBR.Add(S3MB_MATERIAL_PBR_SEMANTICTEXTUREINDEX, pPBR->m_nSemanticTextureIndex);
		jsonPBR.Add(S3MB_MATERIAL_PBR_SEMANTICTEXTURECOORDINDEX, pPBR->m_nSemanticTextureCoordIndex);

		PBRMetallicRough* pPBRMR = (PBRMetallicRough*)pPBR;
		jsonPBR.Add(S3MB_MATERIAL_PBRM_BASECOLOR, pPBRMR->m_vec4BaseColor);
		jsonPBR.Add(S3MB_MATERIAL_PBRM_BASECOLORTEXTUREINDEX, pPBRMR->m_nBaseColorTextureIndex);
		jsonPBR.Add(S3MB_MATERIAL_PBRM_BASECOLORTEXTURECOORDINDEX, pPBRMR->m_nBaseColorTextureCoordIndex);
		JsonValue jsonBaseColorTextureMotion(JsonValueType::objectValue,true);
		pPBRMR->m_BaseColorTextureMotion.ToJson(jsonBaseColorTextureMotion);
		jsonPBR.Add(S3MB_MATERIAL_PBRM_BASECOLORTEXTUREMOTION, jsonBaseColorTextureMotion);
		jsonPBR.Add(S3MB_MATERIAL_PBRM_METALLICROUGHNESSTEXTUREINDEX, pPBRMR->m_nMetallicRoughnessTextureIndex);
		jsonPBR.Add(S3MB_MATERIAL_PBRM_METALLICROUGHNESSTEXTURECOORDINDEX, pPBRMR->m_nMetallicRoughnessTextureCoordIndex);
		jsonPBR.Add(S3MB_MATERIAL_PBRM_METALLICFACTOR, pPBRMR->m_fMetallicFactor);
		jsonPBR.Add(S3MB_MATERIAL_PBRM_ROUGHNESSFACTOR, pPBRMR->m_fRoughnessFactor);
		
	}

	void S3MBTools::TUSToJson(TextureUnitState* pTextureUnitState, JsonValue& jsonTexture)
	{
		JsonValue jsonContent(objectValue);
		jsonContent.Add(S3MB_TEXTURE_NAME, pTextureUnitState->m_strTextureName);
		jsonContent.Add(S3MB_TEXTURE_URL, pTextureUnitState->m_strTexureURL);

		// AddressMode
		AddressModeToJson(pTextureUnitState, jsonContent);
		// TextureFilteringOption
		int nOptionVal = pTextureUnitState->m_TextureFilteringOption;
		jsonContent.Add(S3MB_MATERIAL_TEXTUNIT_FILTEROPTION, nOptionVal);
		nOptionVal = pTextureUnitState->m_MinFilter;
		jsonContent.Add(S3MB_MATERIAL_TEXTUNIT_FILTERMIN, nOptionVal);
		nOptionVal = pTextureUnitState->m_MaxFilter;
		jsonContent.Add(S3MB_MATERIAL_TEXTUNIT_FILTERMAX, nOptionVal);

		// TexMatrix
		jsonContent.Add(S3MB_MATERIAL_TEXTUNIT_MATRIX, pTextureUnitState->m_TexModMatrix);

		jsonTexture.Add(S3MB_MATERIAL_TEXTUNIT, jsonContent);
	}

	void S3MBTools::AnimationsToJson(RenderOperationPagedLOD* pROPagedLOD, JsonValue& jsonAnimations)
	{
		if (pROPagedLOD == NULL)
		{
			assert(false);
			return;
		}

		// nodes
		JsonValue jsonNodes(JsonValueType::arrayValue);
		const std::vector<Node*>& vecNode = pROPagedLOD->GetNodes();
		S3MBTools::NodeToJson(vecNode, jsonNodes);
		jsonAnimations.Add(S3MB_NODES, jsonNodes);

		// skins
		JsonValue jsonSkins(JsonValueType::arrayValue);
		const std::vector<Skin*>& vecSkin = pROPagedLOD->GetSkins();
		S3MBTools::SkinToJson(vecSkin, jsonSkins);
		jsonAnimations.Add(S3MB_SKINS, jsonSkins);

		// animationContents
		JsonValue jsonAnimationContents(JsonValueType::arrayValue);
		const std::vector<Animation*>& vecAnimationState = pROPagedLOD->GetAnimationStates();
		const std::vector<std::map<wstring, AnimationCurve*> >& vecAnimationCurve = pROPagedLOD->GetAnimationCurves();
		assert(vecAnimationState.size() == vecAnimationCurve.size());
		S3MBTools::AnimationToJson(vecAnimationState, vecAnimationCurve, jsonAnimationContents);
		jsonAnimations.Add(S3MB_ANIMATIONCONTENTS, jsonAnimationContents);
	}

	void S3MBTools::NodeToJson(const std::vector<Node*>& vecNode, JsonValue& jsonNodes)
	{
		for (int i = 0; i < vecNode.size(); i++)
		{
			Node* pNode = vecNode[i];
			if (pNode == NULL)
			{
				continue;
			}

			JsonValue jsonNode;
			jsonNode.Add(S3MB_NODE_NAME, pNode->GetName());
			jsonNode.Add(S3MB_NODE_SKININDEX, pNode->GetSkinIndex());
			jsonNode.Add(S3MB_NODE_PATCHINDEX, pNode->GetPatchIndex());

			const std::vector<int>& vecChildNodeIndex = pNode->GetChildNodeIndexes();
			JsonValue jsonChildNodeIndexes(JsonValueType::arrayValue);
			for (int j = 0; j < vecChildNodeIndex.size(); j++)
			{
				jsonChildNodeIndexes.Add(vecChildNodeIndex[j]);
			}
			jsonNode.Add(S3MB_NODE_CHILDNODEINDEXES, jsonChildNodeIndexes);

			jsonNodes.Add(jsonNode);
		}
	}

	void S3MBTools::SkinToJson(const std::vector<Skin*>& vecSkin, JsonValue& jsonSkins)
	{
		for (int i = 0; i < vecSkin.size(); i++)
		{
			Skin* pSkin = vecSkin[i];
			if (pSkin == NULL)
			{
				continue;
			}

			JsonValue jsonSkin(JsonValueType::objectValue);
			jsonSkin.Add(S3MB_SKIN_NAME, pSkin->GetName());

			const std::vector<int>& vecJointNodeIndex = pSkin->GetJointNodeIndexes();
			JsonValue jsonJointNodeIndexes(JsonValueType::arrayValue);
			for (int j = 0; j < vecJointNodeIndex.size(); j++)
			{
				jsonJointNodeIndexes.Add(vecJointNodeIndex[j]);
			}
			jsonSkin.Add(S3MB_SKIN_JOINTNODEINDEXES, jsonJointNodeIndexes);

			const std::vector<Matrix4d>& vecInverseBindMatrix = pSkin->GetInverseBindMatrices();
			JsonValue jsonInverseBindMatrices(JsonValueType::arrayValue);
			for (int j = 0; j < vecInverseBindMatrix.size(); j++)
			{
				jsonInverseBindMatrices.Add(vecInverseBindMatrix[j]);
			}
			jsonSkin.Add(S3MB_SKIN_INVERSEBINDMATRICES, jsonInverseBindMatrices);

			jsonSkins.Add(jsonSkin);
		}
	}

	void S3MBTools::AnimationToJson(const std::vector<Animation*>& vecAnimationState,const std::vector<std::map<wstring, AnimationCurve*> >& vecAnimationCurve, JsonValue& jsonAnimationContents)
	{
		assert(vecAnimationState.size() == vecAnimationCurve.size());
		for (int i = 0; i < vecAnimationState.size(); i++)
		{
			Animation* pAnimationState = vecAnimationState[i];
			if (pAnimationState == NULL)
			{
				continue;
			}

			JsonValue jsonAnimationContent(JsonValueType::objectValue);
			jsonAnimationContent.Add(S3MB_ANIMATIONCONTENT_NAME, pAnimationState->GetAnimationName());
			jsonAnimationContent.Add(S3MB_ANIMATIONCONTENT_TIMEPOSITION, pAnimationState->GetTimePosition());
			jsonAnimationContent.Add(S3MB_ANIMATIONCONTENT_TIMELENGTH, pAnimationState->GetLength());
			jsonAnimationContent.Add(S3MB_ANIMATIONCONTENT_STARTTIME, pAnimationState->GetStartTime());
			jsonAnimationContent.Add(S3MB_ANIMATIONCONTENT_ENDTIME, pAnimationState->GetEndTime());

			JsonValue jsonKeyframes(JsonValueType::arrayValue);
			const std::map<wstring, AnimationCurve*>& mapCurve = vecAnimationCurve[i];
			std::map<wstring, AnimationCurve*>::const_iterator itor;
			for (itor = mapCurve.begin(); itor != mapCurve.end(); itor++)
			{
				AnimationCurve* pCurve = itor->second;
				if (pCurve == NULL)
				{
					continue;
				}

				JsonValue jsonKeyframe(JsonValueType::objectValue); 
#ifdef WIN32
				jsonKeyframe.Add(S3MB_ANIMATIONCONTENT_KEYFRAME_PATCHINDEX, _wtoi(itor->first.c_str()));
#else
                std::string strTemp = StringUtil::UnicodeToANSI(itor->first);
                jsonKeyframe.Add(S3MB_ANIMATIONCONTENT_KEYFRAME_PATCHINDEX, atoi(strTemp.c_str()));
#endif
				jsonKeyframe.Add(S3MB_ANIMATIONCONTENT_KEYFRAME_PATH, pCurve->GetPath());

				JsonValue jsonFramePositions(JsonValueType::arrayValue);
				JsonValue jsonTransformations(JsonValueType::arrayValue);
				std::vector<AnimationKey*>& arrAnimationKey = pCurve->GetKeys();
				for (int j = 0; j < arrAnimationKey.size(); j++)
				{
					AnimationKey* pAnimationKey = arrAnimationKey[j];
					if (pAnimationKey == NULL)
					{
						continue;
					}

					jsonFramePositions.Add(pAnimationKey->GetTime());
					
					if (StringUtil::CompareNoCase(pCurve->GetPath(), S3MB_ANIMATIONCONTENT_KEYFRAME_PATH_SCALE))
					{
						jsonTransformations.Add(pAnimationKey->GetScale());
					}
					else if (StringUtil::CompareNoCase(pCurve->GetPath(), S3MB_ANIMATIONCONTENT_KEYFRAME_PATH_ROTATION))
					{
						Vector4d vec4dRotation;
						vec4dRotation.x = pAnimationKey->GetRotation().x;
						vec4dRotation.y = pAnimationKey->GetRotation().y;
						vec4dRotation.z = pAnimationKey->GetRotation().z;
						vec4dRotation.w = pAnimationKey->GetRotation().w;
						jsonTransformations.Add(vec4dRotation);
					}
					else if (StringUtil::CompareNoCase(pCurve->GetPath(), S3MB_ANIMATIONCONTENT_KEYFRAME_PATH_TRANSLATION))
					{
						jsonTransformations.Add(pAnimationKey->GetTranslate());
					}
					else if (StringUtil::CompareNoCase(pCurve->GetPath(), S3MB_ANIMATIONCONTENT_KEYFRAME_PATH_WEIGHTS))
					{
						jsonTransformations.Add(pAnimationKey->GetWeights());
					}
				}
				jsonKeyframe.Add(S3MB_ANIMATIONCONTENT_KEYFRAME_FRAMEPOSITIONS, jsonFramePositions);
				jsonKeyframe.Add(S3MB_ANIMATIONCONTENT_KEYFRAME_TRANSFORMATIONS, jsonTransformations);
				jsonKeyframes.Add(jsonKeyframe);
			}
			jsonAnimationContent.Add(S3MB_ANIMATIONCONTENT_KEYFRAMES, jsonKeyframes);
			jsonAnimationContents.Add(jsonAnimationContent);
		}
	}

	void S3MBTools::AnimationsFromJson(JsonValue& jsonAnimations, RenderOperationPagedLOD*& pROPagedLOD)
	{
		if (pROPagedLOD == NULL)
		{
			assert(false);
			return;
		}

		// nodes
		JsonValue* pJsonNodes = NULL;
		if (jsonAnimations.GetValue(S3MB_NODES, pJsonNodes))
		{
			std::vector<Node*> vecNode;
			S3MBTools::NodeFromJson(*pJsonNodes, vecNode);
			pROPagedLOD->SetNodes(vecNode);
		}
		delete pJsonNodes;
		pJsonNodes = NULL;

		// skins
		JsonValue* pJsonSkins = NULL;
		if (jsonAnimations.GetValue(S3MB_SKINS, pJsonSkins))
		{
			std::vector<Skin*> vecSkin;
			S3MBTools::SkinFromJson(*pJsonSkins, vecSkin);
			pROPagedLOD->SetSkins(vecSkin);
		}
		delete pJsonSkins;
		pJsonSkins = NULL;

		// animationContents
		JsonValue* pJsonAnimationContents = NULL;
		if (jsonAnimations.GetValue(S3MB_ANIMATIONCONTENTS, pJsonAnimationContents))
		{
			std::vector<Animation*> vecAnimationState;
			std::vector<std::map<wstring, AnimationCurve*> > vecAnimationCurve;
			S3MBTools::AnimationFromJson(*pJsonAnimationContents, vecAnimationState, vecAnimationCurve);
			pROPagedLOD->SetAnimationStates(vecAnimationState);
			pROPagedLOD->SetAnimationCurves(vecAnimationCurve);
		}
		delete pJsonAnimationContents;
		pJsonAnimationContents = NULL;
	}

	void S3MBTools::NodeFromJson(JsonValue& jsonNodes, std::vector<Node*>& vecNode)
	{
		for (int i = 0; i < jsonNodes.GetArraySize(); i++)
		{
			JsonValue jsonNode;
			if (!jsonNodes.GetAt(i, &jsonNode))
			{
				continue;
			}

			Node* pNode = new Node();
			wstring strName;
			jsonNode.GetValue(S3MB_NODE_NAME, strName);
			pNode->SetName(strName);

			int nSkinIndex = 0, nGeodeIndex = 0;
			jsonNode.GetValue(S3MB_NODE_SKININDEX, nSkinIndex);
			pNode->SetSkinIndex(nSkinIndex);
			jsonNode.GetValue(S3MB_NODE_PATCHINDEX, nGeodeIndex);
			pNode->SetPatchIndex(nGeodeIndex);

			JsonValue* pJsonChildNodeIndexes = NULL;
			std::vector<int> vecChildNodeIndex;
			if (jsonNode.GetValue(S3MB_NODE_CHILDNODEINDEXES, pJsonChildNodeIndexes))
			{
				for (int j = 0; j < pJsonChildNodeIndexes->GetArraySize(); j++)
				{
					int nIndex = 0;
					pJsonChildNodeIndexes->GetAt(j, nIndex);
					vecChildNodeIndex.push_back(nIndex);
				}
				delete pJsonChildNodeIndexes;
				pJsonChildNodeIndexes = NULL;
			}
			pNode->SetChildNodeIndexes(vecChildNodeIndex);

			vecNode.push_back(pNode);
		}
	}

	void S3MBTools::SkinFromJson(JsonValue& jsonSkins, std::vector<Skin*>& vecSkin)
	{
		for (int i = 0; i < jsonSkins.GetArraySize(); i++)
		{
			JsonValue jsonSkin;
			if (!jsonSkins.GetAt(i, &jsonSkin))
			{
				continue;
			}

			Skin* pSkin = new Skin();
			wstring strName;
			jsonSkin.GetValue(S3MB_SKIN_NAME, strName);
			pSkin->SetName(strName);

			JsonValue* pJsonJointNodeIndexes = NULL;
			std::vector<int> vecJointNodeIndex;
			if (jsonSkin.GetValue(S3MB_SKIN_JOINTNODEINDEXES, pJsonJointNodeIndexes))
			{
				for (int j = 0; j < pJsonJointNodeIndexes->GetArraySize(); j++)
				{
					int nIndex = 0;
					pJsonJointNodeIndexes->GetAt(j, nIndex);
					vecJointNodeIndex.push_back(nIndex);
				}
				delete pJsonJointNodeIndexes;
				pJsonJointNodeIndexes = NULL;
			}
			pSkin->SetJointNodeIndexes(vecJointNodeIndex);

			JsonValue* pJsonInverseBindMatrices = NULL;
			std::vector<Matrix4d> vecInverseBindMatrix;
			if (jsonSkin.GetValue(S3MB_SKIN_INVERSEBINDMATRICES, pJsonInverseBindMatrices))
			{
				for (int j = 0; j < pJsonInverseBindMatrices->GetArraySize(); j++)
				{
					Matrix4d mat;
					pJsonInverseBindMatrices->GetAt(j, mat);
					vecInverseBindMatrix.push_back(mat);
				}
				delete pJsonInverseBindMatrices;
				pJsonInverseBindMatrices = NULL;
			}
			pSkin->SetInverseBindMatrices(vecInverseBindMatrix);

			vecSkin.push_back(pSkin);
		}
	}

	void S3MBTools::AnimationFromJson(JsonValue& jsonAnimationContents, std::vector<Animation*>& vecAnimationState, std::vector<std::map<wstring, AnimationCurve*> >& vecAnimationCurve)
	{
		for (int i = 0; i < jsonAnimationContents.GetArraySize(); i++)
		{
			JsonValue jsonAnimationContent;
			if (!jsonAnimationContents.GetAt(i, &jsonAnimationContent))
			{
				continue;
			}

			wstring strName;
			jsonAnimationContent.GetValue(S3MB_ANIMATIONCONTENT_NAME, strName);
			double dTimePosition = 0.0, dTimeLength = 0.0, dStartTime = 0.0, dEndTime = 0.0;
			jsonAnimationContent.GetValue(S3MB_ANIMATIONCONTENT_TIMEPOSITION, dTimePosition);
			jsonAnimationContent.GetValue(S3MB_ANIMATIONCONTENT_TIMELENGTH, dTimeLength);
			jsonAnimationContent.GetValue(S3MB_ANIMATIONCONTENT_STARTTIME, dStartTime);
			jsonAnimationContent.GetValue(S3MB_ANIMATIONCONTENT_ENDTIME, dEndTime);

			Animation* pAnimationState = new Animation(strName, dTimePosition, dTimeLength, true);
			pAnimationState->SetStartTime(dStartTime);
			pAnimationState->SetEndTime(dEndTime);
			vecAnimationState.push_back(pAnimationState);

			JsonValue* pJsonKeyframes = NULL;
			if (jsonAnimationContent.GetValue(S3MB_ANIMATIONCONTENT_KEYFRAMES, pJsonKeyframes))
			{
				std::map<wstring, AnimationCurve*> mapCurve;
				for (int j = 0; j < pJsonKeyframes->GetArraySize(); j++)
				{
					JsonValue jsonKeyframe;
					if (!pJsonKeyframes->GetAt(j, &jsonKeyframe))
					{
						continue;
					}

					AnimationCurve* pCurve = new AnimationCurve();

					int nPatchIndex = 0;
					jsonKeyframe.GetValue(S3MB_ANIMATIONCONTENT_KEYFRAME_PATCHINDEX, nPatchIndex);
					wstring strPath;
					jsonKeyframe.GetValue(S3MB_ANIMATIONCONTENT_KEYFRAME_PATH, strPath);
					pCurve->SetPath(strPath);

					JsonValue* pJsonFramePositions = NULL, *pJsonTransformations = NULL;
					if (jsonKeyframe.GetValue(S3MB_ANIMATIONCONTENT_KEYFRAME_FRAMEPOSITIONS, pJsonFramePositions) &&
						jsonKeyframe.GetValue(S3MB_ANIMATIONCONTENT_KEYFRAME_TRANSFORMATIONS, pJsonTransformations))
					{
						for (int k = 0; k < pJsonFramePositions->GetArraySize(); k++)
						{
							AnimationKey* pAnimationKey = pCurve->AddKey();

							double dFramePosition = 0.0;
							pJsonFramePositions->GetAt(k, dFramePosition);
							pAnimationKey->SetTime(dFramePosition);
							
							if (StringUtil::CompareNoCase(strPath, S3MB_ANIMATIONCONTENT_KEYFRAME_PATH_SCALE))
							{
								Vector3d vec3dScale;
								pJsonTransformations->GetAt(k, vec3dScale);
								pAnimationKey->SetScale(vec3dScale);
							}
							else if (StringUtil::CompareNoCase(strPath, S3MB_ANIMATIONCONTENT_KEYFRAME_PATH_ROTATION))
							{
								Vector4d vec4dRotation;
								pJsonTransformations->GetAt(k, vec4dRotation);

								Quaternion qua4dRotation;
								qua4dRotation.x = vec4dRotation.x;
								qua4dRotation.y = vec4dRotation.y;
								qua4dRotation.z = vec4dRotation.z;
								qua4dRotation.w = vec4dRotation.w;
								pAnimationKey->SetRotation(qua4dRotation);
							}
							else if (StringUtil::CompareNoCase(strPath, S3MB_ANIMATIONCONTENT_KEYFRAME_PATH_TRANSLATION))
							{
								Vector3d vec3dTranslate;
								pJsonTransformations->GetAt(k, vec3dTranslate);
								pAnimationKey->SetTranslate(vec3dTranslate);
							}
							else if (StringUtil::CompareNoCase(strPath, S3MB_ANIMATIONCONTENT_KEYFRAME_PATH_WEIGHTS))
							{
								double dWeights = 0.0;
								pJsonTransformations->GetAt(k, dWeights);
								pAnimationKey->SetWeights(dWeights);
							}
						}
						delete pJsonFramePositions;
						pJsonFramePositions = NULL;
					}

					wstring strPatchIndex;
					strPatchIndex = to_wstring(nPatchIndex);
					mapCurve[strPatchIndex] = pCurve;
				}
				delete pJsonKeyframes;
				pJsonKeyframes = NULL;

				vecAnimationCurve.push_back(mapCurve);
			}
		}
	}

	void S3MBTools::AddressModeToJson(TextureUnitState* pTextureUnitState, JsonValue& jsonAddMode)
	{
		int nValue = pTextureUnitState->m_AddressMode.u;
		jsonAddMode.Add(S3MB_TEXTURE_U, nValue);
		nValue = pTextureUnitState->m_AddressMode.v;
		jsonAddMode.Add(S3MB_TEXTURE_V, nValue);
		nValue = pTextureUnitState->m_AddressMode.w;
		jsonAddMode.Add(S3MB_TEXTURE_W, nValue);
	}

	void S3MBTools::SaveTextures(std::map<wstring, TextureDataInfo*>& mapTexture, MemoryStream& streamTexture,
		unsigned int nCompressType, bool bTextureChange, bool bGenerateMipmap/* = true*/)
	{
		int nImageCount = mapTexture.size();
		streamTexture.Init(1024 * 1024 * nImageCount, 1024 * 1024);
		streamTexture << nImageCount;

		std::map<wstring, TextureDataInfo*>::iterator it;
		for (it = mapTexture.begin(); it != mapTexture.end(); it++)
		{
			wstring strName = it->first;
			TextureDataInfo* pTexture = it->second;
			if ((pTexture->m_bMipmap || bGenerateMipmap) && pTexture->m_pTextureData != NULL)
			{
				pTexture->m_nLevel = ImageOperator::GetMipMapLevel(pTexture->m_pTextureData->m_nWidth, pTexture->m_pTextureData->m_nHeight);
			}
			streamTexture << strName;
			S3MBTools::StreamAlign(streamTexture, false);

			streamTexture << pTexture->m_nLevel;

			S3MBTools::SaveTextureData(pTexture->m_pTextureData, streamTexture, nCompressType, bTextureChange, bGenerateMipmap);
		}
	}

	void S3MBTools::SaveTextureData(TextureData* pTextureData, MemoryStream& streamTexture, unsigned int nCompressType, bool bIsChangeTexture /*= true*/, bool bGenerateMipmap/* = true*/)
	{
		if (bIsChangeTexture)
		{
			ProcessTextureData(pTextureData, nCompressType,bGenerateMipmap);
		}

		streamTexture << pTextureData->m_nWidth;
		streamTexture << pTextureData->m_nHeight;
		streamTexture << pTextureData->m_eCompressType;
		streamTexture << pTextureData->m_nSize;
		streamTexture << pTextureData->m_enFormat;
		streamTexture.Save(pTextureData->m_pBuffer, pTextureData->m_nSize);
	}

	void S3MBTools::ProcessTextureData(TextureData* pTextureData, unsigned int nCompressType, bool bGenerateMipmap)
	{
		return ProcessTextureData(pTextureData, (TextureCompressType)nCompressType, bGenerateMipmap);
	}

	std::wstring S3MBTools::AlphaModeToString(AlphaMode emType)
	{
		wstring strValue;
		if (emType == PBRAM_OPAQUE)
		{
			strValue = S3MB_MATERIAL_PBR_ALPHAMODE_OPAQUE;
		}
		else if (emType == PBRAM_MASK)
		{
			strValue = S3MB_MATERIAL_PBR_ALPHAMODE_MASK;
		}
		else if (emType == PBRAM_BLEND)
		{
			strValue = S3MB_MATERIAL_PBR_ALPHAMODE_BLEND;
		}
		return strValue;
	}

	std::wstring S3MBTools::CullModeToString(CullingMode emType)
	{
		wstring strValue;
		if (emType == CULL_NONE)
		{
			strValue = S3MB_MATPASS_CULLMODE_NONE;
		}
		else if (emType == CULL_CLOCKWISE)
		{
			strValue = S3MB_MATPASS_CULLMODE_C;
		}
		else if (emType == CULL_ANTICLOCKWISE)
		{
			strValue = S3MB_MATPASS_CULLMODE_CC;
		}
		return strValue;
	}

	bool S3MBTools::CanUseDraco(VertexDataPackage* pVertexPack, std::vector<IndexPackage*> vecIndexPack)
	{
		// 实例化和顶点索引是一对多的情况下不能用draco压缩
		if (vecIndexPack.size() > 1)
		{
			return false;
		}
		return true;
	}

	void S3MBTools::LoadStream(MemoryStream& streamSrc, MemoryStream& streamDst)
	{
		unsigned int nSize = 0;
		streamSrc >> nSize;
		unsigned char* pOpenData = 0;
		pOpenData = streamSrc.TakeStream(nSize);
		streamDst.Init(pOpenData, nSize);
	}

	void S3MBTools::SaveStream(MemoryStream& streamDst, MemoryStream& streamSrc)
	{
		unsigned int nSize = 0;
		nSize = streamSrc.GetLength();
		streamDst << nSize;
		streamDst.Save(streamSrc.GetDataPtr(), nSize);
	}

	bool S3MBTools::LoadVertexAttributeExtension(MemoryStream& stream, VertexDataPackage* pVertexDataPack)
	{
		if (pVertexDataPack == NULL)
		{
			return false;
		}

		stream >> pVertexDataPack->m_nVertexAttCount;
		for (unsigned i = 0; i < pVertexDataPack->m_nVertexAttCount; i++)
		{
			unsigned int nAttDataCount = 0;
			stream >> nAttDataCount;
			pVertexDataPack->m_vecVertexAttDataCount.push_back(nAttDataCount);
			unsigned short nAttDataDimension = 0;
			stream >> nAttDataDimension;
			pVertexDataPack->m_vecVertexAttDataDimension.push_back(nAttDataDimension);
			unsigned short nAttDataType = 0;
			stream >> nAttDataType;
			pVertexDataPack->m_vecVertexAttDataType.push_back(VertexAttributeType(nAttDataType));

			int nDataSize = nAttDataCount * nAttDataDimension;
			if (nAttDataType == AT_16BIT)
			{
				unsigned short* pAttData = new(std::nothrow) unsigned short[nDataSize];
				memset(pAttData, 0, sizeof(unsigned short) * nDataSize);
				stream.Load(pAttData, nDataSize);
				pVertexDataPack->m_vecVertexAttData.push_back(pAttData);
			}
			else if(nAttDataType == AT_32BIT)
			{
				unsigned int* pAttData = new(std::nothrow) unsigned int[nDataSize];
				memset(pAttData, 0, sizeof(unsigned int) * nDataSize);
				stream.Load(pAttData, nDataSize);
				pVertexDataPack->m_vecVertexAttData.push_back(pAttData);
			}
			else if (nAttDataType == AT_FLOAT)
			{
				float* pAttData = new(std::nothrow) float[nDataSize];
				memset(pAttData, 0, sizeof(float) * nDataSize);
				stream.Load(pAttData, nDataSize);
				pVertexDataPack->m_vecVertexAttData.push_back(pAttData);
			}
			else if (nAttDataType == AT_DOUBLE)
			{
				double* pAttData = new(std::nothrow) double[nDataSize];
				memset(pAttData, 0, sizeof(double) * nDataSize);
				stream.Load(pAttData, nDataSize);
				pVertexDataPack->m_vecVertexAttData.push_back(pAttData);
			}
			else
			{
				assert(false);
			}
		}

		wstring strVertexAttributeDescript;
		stream >> strVertexAttributeDescript;
		StreamAlign(stream, true);

		JsonValue jsonVertexAtts;
		if (!jsonVertexAtts.LoadValueFromString(strVertexAttributeDescript))
		{
			return false;
		}
		vector<wstring> arrKeys;
		jsonVertexAtts.GetAllKeys(arrKeys);

		for (int i = 0; i < arrKeys.size(); i++)
		{
			jsonVertexAtts.GetValue(arrKeys[i], pVertexDataPack->m_mapVertexAttributeDescript[arrKeys[i]]);
		}

		return true;
	}

	bool S3MBTools::SaveVertexAttributeExtension(VertexDataPackage* pVertexDataPack, MemoryStream& stream)
	{
		if (pVertexDataPack == NULL)
		{
			return false;
		}

		stream << pVertexDataPack->m_nVertexAttCount;
		for (unsigned i = 0; i < pVertexDataPack->m_nVertexAttCount; i++)
		{
			stream << pVertexDataPack->m_vecVertexAttDataCount[i];
			stream << pVertexDataPack->m_vecVertexAttDataDimension[i];
			stream << (unsigned short)pVertexDataPack->m_vecVertexAttDataType[i];

			if (pVertexDataPack->m_vecVertexAttDataType[i] == AT_16BIT)
			{
				stream.Save((unsigned short*)pVertexDataPack->m_vecVertexAttData[i], \
					pVertexDataPack->m_vecVertexAttDataCount[i] * pVertexDataPack->m_vecVertexAttDataDimension[i]);
			}
			else if(pVertexDataPack->m_vecVertexAttDataType[i] == AT_32BIT)
			{
				stream.Save((unsigned int*)pVertexDataPack->m_vecVertexAttData[i], \
					pVertexDataPack->m_vecVertexAttDataCount[i] * pVertexDataPack->m_vecVertexAttDataDimension[i]);
			}
			else if (pVertexDataPack->m_vecVertexAttDataType[i] == AT_FLOAT)
			{
				stream.Save((float*)pVertexDataPack->m_vecVertexAttData[i], \
					pVertexDataPack->m_vecVertexAttDataCount[i] * pVertexDataPack->m_vecVertexAttDataDimension[i]);
			}
			else if (pVertexDataPack->m_vecVertexAttDataType[i] == AT_DOUBLE)
			{
				stream.Save((double*)pVertexDataPack->m_vecVertexAttData[i], \
					pVertexDataPack->m_vecVertexAttDataCount[i] * pVertexDataPack->m_vecVertexAttDataDimension[i]);
			}
			else
			{
				assert(false);
			}
		}

		std::map<wstring, int> mapVertexAttributeDescript = pVertexDataPack->m_mapVertexAttributeDescript;
		JsonValue jsonVertexAtts(JsonValueType::objectValue);
		std::map<wstring, int>::iterator itor = mapVertexAttributeDescript.begin();
		for (; itor != mapVertexAttributeDescript.end(); itor++)
		{
			jsonVertexAtts.Add(itor->first, itor->second);
		}

		string strJson;
		jsonVertexAtts.SaveToString(strJson);
		stream << StringUtil::UTF8_to_UNICODE(strJson);

		// 字节对齐
		StreamAlign(stream, false);
		
		return true;
	}

	bool S3MBTools::LoadIndexPackages(MemoryStream& stream, vector<IndexPackage*>& vecIndexPackage, float fVersion)
	{
		bool bResult = true;
		int nIndexPackageCount = 0;
		stream >> nIndexPackageCount;
		for (int i = 0; i < nIndexPackageCount; i++)
		{
			bool bLoad = false;
			IndexPackage* pIndexPackage = new IndexPackage();
			MemoryStream streamIndexPackage;
			LoadStream(stream, streamIndexPackage);
			bLoad = pIndexPackage->Load(streamIndexPackage);

			// 内存不足返回FALSE
			if (!bLoad)
			{
				delete pIndexPackage;
				pIndexPackage = NULL;
				bResult = false;
				break;
			}

			vecIndexPackage.push_back(pIndexPackage);
		}

		// 失败以后内存清理
		if (!bResult)
		{
			for (unsigned int i = 0; i < vecIndexPackage.size(); i++)
			{
				delete vecIndexPackage[i];
			}
			vecIndexPackage.clear();
		}
		return bResult;
	}

	bool S3MBTools::SaveIndexPackages(vector<IndexPackage*>& vecIndexPackage, MemoryStream& stream, float fVersion)
	{
		int nIndexPackageCount = vecIndexPackage.size();
		stream << nIndexPackageCount;

		for (int i = 0; i < nIndexPackageCount; i++)
		{
			MemoryStream streamIndexPackage;
			streamIndexPackage.Init();

			IndexPackage* pIndexPackage = vecIndexPackage[i];
			pIndexPackage->Save(streamIndexPackage);

			SaveStream(stream, streamIndexPackage);
		}
		return true;
	}
}

