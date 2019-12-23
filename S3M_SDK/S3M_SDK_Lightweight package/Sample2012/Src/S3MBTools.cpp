#include <iostream>
#include<fstream>
#include "S3MBTools.h"
#include <assert.h>
#include "ImageOperator.h"
#include "Palette.h"
#include "StringHelper.h"

using namespace std;

static union
{
	int nTest;
	char cTest[sizeof(int)];
}un_bigendina1 = { 1 };
#define  ISBIGENDIAN (un_bigendina1.cTest[0] != 1)

uInt S3MBTools::RGBA(Byte r, Byte g, Byte b, Byte a)
{
	uInt colorValue = 0;
	if (ISBIGENDIAN)
	{
		colorValue = (((uInt)(Byte)(r) << 24) | ((uInt)(Byte)(g) << 16) | ((uInt)(Byte)(b) << 8) | ((uInt)(Byte)(a)));
	}
	else
	{
		colorValue = (((uInt)(Byte)(r)) | ((uInt)(Byte)(g) << 8) | ((uInt)(Byte)(b) << 16) | ((uInt)(Byte)(a) << 24));
	}
	return colorValue;
}

bool S3MBTools::LoadStreamDataFromFile(ifstream &fileStream, MemoryStream& streamUnZipped, MemoryStream& streamShell, MemoryStream& streamSkeleton, \
	MemoryStream& streamSecondColor, MemoryStream& streamTexture, MemoryStream& streamSelInfo, string& strJsonMaterials)
{
	uInt nZippedSize = 0;
	//fileStream >> nZippedSize;
	fileStream.read((char*)&nZippedSize, sizeof(uInt));

	Byte* pZippedData = new(std::nothrow) Byte[nZippedSize];
	fileStream.read((char*)pZippedData, nZippedSize);

	if (!UnZipData(streamUnZipped, pZippedData, nZippedSize))//解压文件流
	{
		return false;
	}

	uInt nOptions = 0;
	//fileStream.read((char*)&nOptions,sizeof(uInt));
	streamUnZipped >> nOptions;

	LoadStream(streamUnZipped, streamShell);
	LoadStream(streamUnZipped, streamSkeleton);
	LoadStream(streamUnZipped, streamSecondColor);
	LoadStream(streamUnZipped, streamTexture);
	streamUnZipped >> strJsonMaterials;
	if (nOptions)//带实例化存储信息	
	{
		LoadStream(streamUnZipped, streamSelInfo);
	}

	return true;
}

bool S3MBTools::UnZipData(MemoryStream& streamUnZipped, Byte*& pZippedData, uInt nZippedSize)
{
	unsigned long nUnZipSize = nZippedSize * 4;
	Byte* pUnZipData = new (std::nothrow) Byte[nUnZipSize];
	bool bResult = false;

	while (!bResult)
	{
		int nResult = uncompress(pUnZipData, &nUnZipSize, pZippedData, nZippedSize);//nUnZipSize的大小会改变
		//防错处理
		switch (nResult)
		{
		case Z_OK:
			bResult = true;
			break;
		case Z_MEM_ERROR:
			cout << "内存分配不足" << endl;
			break;
		case Z_BUF_ERROR:
			//cout << "输出缓存没有足够的空间" << endl;
			break;
		case Z_STREAM_ERROR:
			cout << "压缩文件参数非法" << endl;
			break;
		case Z_DATA_ERROR:
			//数据或者密码错误 //调换顺序，解决桌面打开工作空间未弹出输密码的对话框之前，先报错“数据错误”的问题 DFAB-1100		
			cout << "数据错误" << endl;
			break;
		default:
			//不可识别的错误，
			cout << "不可识别的错误！" << endl;
			break;
		}

		if (!bResult)//如果解压失败.
		{
			if (nResult != Z_BUF_ERROR)
			{
				cout << "解压失败！正在重试..." << endl;
			}
			delete[]pUnZipData;
			nUnZipSize = nUnZipSize * 4;
			pUnZipData = new(std::nothrow) Byte[nUnZipSize];
			continue;
		}
	}
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

void S3MBTools::LoadStream(MemoryStream &streamSrc, MemoryStream &streamDst)
{
	uInt nSize = 0;
	streamSrc >> nSize;
	Byte* pOpenData = 0;
	pOpenData = streamSrc.TakeStream(nSize);
	streamDst.Init(pOpenData, nSize);
}

void S3MBTools::LoadSkeleton(MemoryStream &streamSkeleton, std::map<string, Geometry*> &mapGeometry)
{
	int nCount = 0;
	streamSkeleton >> nCount;

	for (int k = 0; k < nCount; k++)
	{
		string strGeoName;
		streamSkeleton >> strGeoName;
		StreamAlign(streamSkeleton, true);

		VertexDataPackage* pVertexDataPackage = NULL;
		vector<IndexPackage*> arrIndexPackage;

		if (!LoadSkeletonData(streamSkeleton, pVertexDataPackage, arrIndexPackage))
		{
			continue;
		}
		assert(pVertexDataPackage != NULL && arrIndexPackage.size() > 0);
		Geometry* pGeometry = new Geometry();
		pGeometry->m_strGeoName = strGeoName;
		if (arrIndexPackage.size() > 0 && arrIndexPackage[0]->m_strPassName.size() > 0)
		{
			pGeometry->m_strMaterialName = arrIndexPackage[0]->m_strPassName[0];
		}
		pGeometry->m_pVertexDataPackage = pVertexDataPackage;
		pGeometry->m_arrIndexPackage = arrIndexPackage;
		pGeometry->ComputerBoundingBox();
		pGeometry->m_bInstanceBatch = IsInstanceBatch(pVertexDataPackage);
		mapGeometry[strGeoName] = pGeometry;
	}
}

bool S3MBTools::LoadSkeletonData(MemoryStream &streamSkeleton, VertexDataPackage*& pVertexDataPackage, vector<IndexPackage*>& arrIndexPackage)
{
	uInt nTagValue = SV_Unkown;
	streamSkeleton >> nTagValue;
	pVertexDataPackage = new VertexDataPackage();
	if (nTagValue == SV_Standard)//标准数据类型
	{
		LoadVertex(streamSkeleton, pVertexDataPackage);
		LoadNormal(streamSkeleton, pVertexDataPackage);
		LoadVertexColor(streamSkeleton, pVertexDataPackage);
		LoadSecondVertexColor(streamSkeleton, pVertexDataPackage);
		USHORT nTextCount = 0;
		LoadTextureCoords(streamSkeleton, pVertexDataPackage, nTextCount);
		LoadInstanceInfo(streamSkeleton, pVertexDataPackage, nTextCount);
	}
	else if (nTagValue == SV_Compressed)//压缩数据类型
	{
		streamSkeleton >> pVertexDataPackage->m_nCompressOptions;
		if (pVertexDataPackage->HasCompressOptions(SVC_Vertex))
		{
			LoadCompressVertex(streamSkeleton, pVertexDataPackage);
		}
		else
		{
			LoadVertex(streamSkeleton, pVertexDataPackage);
		}

		if (pVertexDataPackage->HasCompressOptions(SVC_Normal))
		{
			LoadCompressNormal(streamSkeleton, pVertexDataPackage);
		}
		else
		{
			LoadNormal(streamSkeleton, pVertexDataPackage);
		}

		LoadVertexColor(streamSkeleton, pVertexDataPackage);
		LoadSecondVertexColor(streamSkeleton, pVertexDataPackage);

		USHORT nTextCount = 0;
		if (pVertexDataPackage->HasCompressOptions(SVC_TexutreCoord))
		{
			LoadCompressTextureCoords(streamSkeleton, pVertexDataPackage, nTextCount);
		}
		else
		{
			LoadTextureCoords(streamSkeleton, pVertexDataPackage, nTextCount);
		}
		LoadInstanceInfo(streamSkeleton, pVertexDataPackage, nTextCount);
	}
	else//无法识别的数据类型
	{
		cout << "顶点数据类型未知！" << endl;
		return false;
	}
	bool bSuccess = true;
	int nIndexPackageCount = 0;
	streamSkeleton >> nIndexPackageCount;
	for (int i = 0; i < nIndexPackageCount; i++)
	{
		IndexPackage* pIndexPackage = new IndexPackage();
		//内存不足返回false
		if (!pIndexPackage->Load(streamSkeleton))
		{
			delete pIndexPackage;
			pIndexPackage = NULL;
			bSuccess = false;
			break;
		}

		arrIndexPackage.push_back(pIndexPackage);
	}

	//失败以后内存清理
	if (!bSuccess)
	{
		delete pVertexDataPackage;
		pVertexDataPackage = NULL;

		for (int i = 0; i < arrIndexPackage.size(); i++)
		{
			delete arrIndexPackage[i];
		}
		arrIndexPackage.clear();
	}

	return bSuccess;
}

void S3MBTools::StreamAlign(MemoryStream &stream, bool isRead)
{
	if (isRead)
	{
		uInt nSize = stream.GetReadPosition();
		if (nSize % 4 == 0)
		{
			return;
		}
		Byte b = 0;
		uInt nReserved = 4 - nSize % 4;
		for (uInt i = 0; i < nReserved; i++)
		{
			stream >> b;
		}
	}
	else
	{
		uInt nSize = stream.GetWritePosition();
		if (nSize % 4 == 0)
		{
			return;
		}
		uInt nReserved = 4 - nSize % 4;
		Byte b = 0;
		for (uInt i = 0; i < nReserved; i++)
		{
			stream << b;
		}
	}
}

bool S3MBTools::LoadVertex(MemoryStream &streamSkeleton, VertexDataPackage*& pVertexDataPackage)
{
	//uint
	streamSkeleton >> pVertexDataPackage->m_nVerticesCount;
	if (pVertexDataPackage->m_nVerticesCount <= 0)
	{
		return false;
	}

	//ushort
	streamSkeleton >> pVertexDataPackage->m_nVertexDimension;
	//ushort
	streamSkeleton >> pVertexDataPackage->m_nVertexStride;

	pVertexDataPackage->m_pVertices = \
		new(std::nothrow) float[pVertexDataPackage->m_nVerticesCount * pVertexDataPackage->m_nVertexDimension];
	streamSkeleton.Load(pVertexDataPackage->m_pVertices, \
		pVertexDataPackage->m_nVerticesCount * pVertexDataPackage->m_nVertexDimension);

	return true;
}

bool S3MBTools::LoadNormal(MemoryStream &streamSkeleton, VertexDataPackage*& pVertexDataPackage)
{
	//uint
	streamSkeleton >> pVertexDataPackage->m_nNormalCount;

	if (pVertexDataPackage->m_nNormalCount == 0)
	{
		return false;
	}

	//ushort 
	streamSkeleton >> pVertexDataPackage->m_nNormalDimension;
	streamSkeleton >> pVertexDataPackage->m_nNormalStride;

	pVertexDataPackage->m_pNormals = \
		new(std::nothrow) float[pVertexDataPackage->m_nNormalCount * pVertexDataPackage->m_nNormalDimension];
	streamSkeleton.Load(pVertexDataPackage->m_pNormals, \
		pVertexDataPackage->m_nNormalCount * pVertexDataPackage->m_nNormalDimension);

	return true;
}

bool S3MBTools::LoadVertexColor(MemoryStream &streamSkeleton, VertexDataPackage*& pVertexDataPackage)
{
	uInt nColorCount = 0;
	streamSkeleton >> pVertexDataPackage->m_nVertexColorCount;
	//顶点颜色
	if (pVertexDataPackage->m_nVertexColorCount <= 0)
	{
		return false;
	}
	Byte nByte = 0;
	streamSkeleton >> pVertexDataPackage->m_nVertexColorStride;
	streamSkeleton >> nByte;
	streamSkeleton >> nByte;

	pVertexDataPackage->m_pVertexColor = new(std::nothrow) uInt[pVertexDataPackage->m_nVertexColorCount];
	for (uInt j = 0; j < pVertexDataPackage->m_nVertexColorCount; j++)
	{
		Byte btR, btG, btB, btA;

		streamSkeleton >> btR;
		streamSkeleton >> btG;
		streamSkeleton >> btB;
		streamSkeleton >> btA;

		pVertexDataPackage->m_pVertexColor[j] = RGBA(btR, btG, btB, btA);
	}
	return true;
}

bool S3MBTools::LoadSecondVertexColor(MemoryStream &streamSkeleton, VertexDataPackage*& pVertexDataPackage)
{
	uInt nColorCount = 0;
	streamSkeleton >> pVertexDataPackage->m_nSecondVertexColorCount;
	//顶点颜色
	if (pVertexDataPackage->m_nSecondVertexColorCount <= 0)
	{
		return false;
	}
	Byte nByte = 0;
	streamSkeleton >> pVertexDataPackage->m_nVertexColorStride;
	streamSkeleton >> nByte;
	streamSkeleton >> nByte;

	pVertexDataPackage->m_pSecondVertexColor = new(std::nothrow) uInt[pVertexDataPackage->m_nSecondVertexColorCount];
	for (uInt j = 0; j < pVertexDataPackage->m_nSecondVertexColorCount; j++)
	{
		Byte btR, btG, btB, btA;

		streamSkeleton >> btR;
		streamSkeleton >> btG;
		streamSkeleton >> btB;
		streamSkeleton >> btA;

		pVertexDataPackage->m_pSecondVertexColor[j] = RGBA(btR, btG, btB, btA);
	}
	return true;
}

bool S3MBTools::LoadTextureCoords(MemoryStream &streamSkeleton, VertexDataPackage*& pVertexDataPackage, USHORT nTextureCoord)
{
	streamSkeleton >> nTextureCoord;

	Byte nByte = 0;
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

bool S3MBTools::LoadInstanceInfo(MemoryStream &streamSkeleton, VertexDataPackage*& pVertexDataPackage, USHORT nTextureCoord)
{
	Byte nByte = 0;
	USHORT nInstanceInfo = 0;
	streamSkeleton >> nInstanceInfo;
	streamSkeleton >> nByte;
	streamSkeleton >> nByte;

	for (int iIndex = 0; iIndex < nInstanceInfo; iIndex++)
	{
		int i = iIndex + nTextureCoord;
		streamSkeleton >> pVertexDataPackage->m_TexCoordCount[i];
		streamSkeleton >> pVertexDataPackage->m_nTexDimensions[i];
		streamSkeleton >> pVertexDataPackage->m_TexCoordStride[i];

		if (pVertexDataPackage->m_nTexDimensions[i] == SIZE_PER_INSTANCE)
		{
			pVertexDataPackage->m_pTexCoords[i] = \
				new(std::nothrow) float[pVertexDataPackage->m_TexCoordCount[i] * pVertexDataPackage->m_nTexDimensions[i]];

			float* pValue = pVertexDataPackage->m_pTexCoords[i];
			uInt* pUint = (uInt*)pValue;
			for (uInt kIndex = 0; kIndex < pVertexDataPackage->m_TexCoordCount[i]; kIndex++)
			{
				streamSkeleton.Load(pValue + kIndex * SIZE_PER_INSTANCE, 16);

				Byte btR, btG, btB, btA;
				streamSkeleton >> btR;
				streamSkeleton >> btG;
				streamSkeleton >> btB;
				streamSkeleton >> btA;
				pUint[SIZE_PER_INSTANCE * kIndex + 16] = RGBA(btR, btG, btB, btA);
			}
		}
		else if (pVertexDataPackage->m_nTexDimensions[i] == SIZE_PER_DM_INSTANCE)
		{
			pVertexDataPackage->m_pTexCoords[i] = \
				new(std::nothrow) float[pVertexDataPackage->m_TexCoordCount[i] * pVertexDataPackage->m_nTexDimensions[i]];

			float* pValue = pVertexDataPackage->m_pTexCoords[i];
			uInt* pUint = (uInt*)pValue;

			for (uInt kIndex = 0; kIndex < pVertexDataPackage->m_TexCoordCount[i]; kIndex++)
			{
				streamSkeleton.Load(pValue + kIndex * SIZE_PER_DM_INSTANCE, 27);
				Byte btR, btG, btB, btA;
				streamSkeleton >> btR;
				streamSkeleton >> btG;
				streamSkeleton >> btB;
				streamSkeleton >> btA;
				pUint[SIZE_PER_DM_INSTANCE * kIndex + 27] = RGBA(btR, btG, btB, btA);

				streamSkeleton >> btR;
				streamSkeleton >> btG;
				streamSkeleton >> btB;
				streamSkeleton >> btA;
				pUint[SIZE_PER_DM_INSTANCE * kIndex + 28] = RGBA(btR, btG, btB, btA);
			}
		}
		else
		{
			int nValueCount = pVertexDataPackage->m_TexCoordCount[i] * pVertexDataPackage->m_nTexDimensions[i];
			float* pValue = new(std::nothrow) float[nValueCount];
			streamSkeleton.Load(pValue, nValueCount);

			pVertexDataPackage->m_pTexCoords[i] = pValue;
		}
	}
	return true;
}

bool S3MBTools::IsInstanceBatch(VertexDataPackage* pVertexDataPackage)
{
	int nTexCount = 0, i = 0;
	for (i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
	{
		if (pVertexDataPackage->m_TexCoordCount[i] == 0)
		{
			break;
		}
		nTexCount++;
	}
	for (i = 0; i < nTexCount; i++)
	{
		if (pVertexDataPackage->m_nTexDimensions[i] == SIZE_PER_INSTANCE ||
			pVertexDataPackage->m_nTexDimensions[i] == SIZE_PER_DM_INSTANCE)
		{
			return true;
			break;
		}
	}
	return false;
}

bool S3MBTools::LoadCompressVertex(MemoryStream &streamSkeleton, VertexDataPackage*& pVertexDataPackage)
{
	//unsigned int 
	streamSkeleton >> pVertexDataPackage->m_nVerticesCount;
	if (pVertexDataPackage->m_nVerticesCount <= 0)
	{
		return false;
	}

	//UGushort 
	streamSkeleton >> pVertexDataPackage->m_nVertexDimension;
	//UGushort
	streamSkeleton >> pVertexDataPackage->m_nVertexStride;

	USHORT* pCompressVertices = new(std::nothrow) USHORT[pVertexDataPackage->m_nVerticesCount * pVertexDataPackage->m_nVertexDimension];

	streamSkeleton >> pVertexDataPackage->m_fVertCompressConstant;
	streamSkeleton.Load(pVertexDataPackage->m_minVerticesValue, 4);
	streamSkeleton.Load(pCompressVertices, pVertexDataPackage->m_nVerticesCount * pVertexDataPackage->m_nVertexDimension);
	pVertexDataPackage->m_pVertices = (float*)pCompressVertices;
	return true;
}

bool S3MBTools::LoadCompressNormal(MemoryStream &streamSkeleton, VertexDataPackage*& pVertexDataPackage)
{
	//unsigned int
	streamSkeleton >> pVertexDataPackage->m_nNormalCount;
	if (pVertexDataPackage->m_nNormalCount == 0)
	{
		return false;
	}

	//UGushort 
	streamSkeleton >> pVertexDataPackage->m_nNormalDimension;
	streamSkeleton >> pVertexDataPackage->m_nNormalStride;
	short* pEncodeNormal = new(std::nothrow) short[pVertexDataPackage->m_nNormalCount * 2];
	streamSkeleton.Load(pEncodeNormal, pVertexDataPackage->m_nNormalCount * 2);

	float* pNormal = NULL;
	DecompressNormal(pVertexDataPackage->m_nNormalCount, pEncodeNormal, pNormal);
	delete[] pEncodeNormal;
	pVertexDataPackage->m_pNormals = pNormal;
	pVertexDataPackage->m_nCompressOptions &= ~SVC_Normal;
	return true;
}

bool S3MBTools::DecompressNormal(uInt nNormalCount, short* pEncodeNormal, float*& pNormal)
{
	pNormal = new(std::nothrow) float[nNormalCount * 3];
	double rangeMax = 65535.0;
	for (uInt i = 0; i < nNormalCount; i++)
	{
		double x = pEncodeNormal[i * 2] + 32768.0;
		double y = pEncodeNormal[i * 2 + 1] + 32768.0;
		if (fabs(x) < 1e-10 && fabs(y) < 1e-10)
		{
			pNormal[i * 3] = 0.0;
			pNormal[i * 3 + 1] = 0.0;
			pNormal[i * 3 + 2] = 0.0;
			continue;
		}

		double resultX = Clamp(x, 0.0, rangeMax) / rangeMax * 2.0 - 1.0;
		double resultY = Clamp(y, 0.0, rangeMax) / rangeMax * 2.0 - 1.0;
		double resultZ = 1.0 - (fabs(resultX) + fabs(resultY));

		if (resultZ < 0.0)
		{
			double oldVX = resultX;
			double signNotZero = oldVX < 0.0 ? -1.0 : 1.0;
			resultX = (1.0 - fabs(resultY)) * signNotZero;
			signNotZero = resultY < 0.0 ? -1.0 : 1.0;
			resultY = (1.0 - fabs(oldVX)) * signNotZero;
		}

		pNormal[i * 3] = resultX;
		pNormal[i * 3 + 1] = resultY;
		pNormal[i * 3 + 2] = resultZ;
	}
	return true;
}

bool S3MBTools::LoadCompressTextureCoords(MemoryStream &streamSkeleton, VertexDataPackage*& pVertexDataPackage, USHORT nTextureCoord)
{
	Byte nByte = 0;
	streamSkeleton >> nTextureCoord;
	streamSkeleton >> nByte;
	streamSkeleton >> nByte;
	for (int i = 0; i < nTextureCoord; i++)
	{
		LoadSingleCompressTextureCoords(streamSkeleton, i, nTextureCoord, pVertexDataPackage);
	}
	return true;
}

bool S3MBTools::LoadSingleCompressTextureCoords(MemoryStream &streamSkeleton, uInt nTexIdx, uInt nTotalTexCount, VertexDataPackage* pVertexDataPackage)
{
	Byte nByte = 0;
	uInt nTexCoordCount = 0;
	bool bNeedTexCoordZ;
	streamSkeleton >> bNeedTexCoordZ;
	streamSkeleton >> nByte;
	streamSkeleton >> nByte;
	streamSkeleton >> nByte;
	streamSkeleton >> nTexCoordCount;

	pVertexDataPackage->m_TexCoordCount[nTexIdx] = nTexCoordCount;

	USHORT compressDimension = 0;
	streamSkeleton >> compressDimension;
	pVertexDataPackage->m_nTexDimensions[nTexIdx] = compressDimension;
	streamSkeleton >> pVertexDataPackage->m_TexCoordStride[nTexIdx];
	short* pEncodeValue = new(std::nothrow) short[nTexCoordCount * compressDimension];
	streamSkeleton >> pVertexDataPackage->m_texCoordCompressConstant[nTexIdx];
	streamSkeleton.Load(pVertexDataPackage->m_minTexCoordValue[nTexIdx], 4);
	streamSkeleton.Load(pEncodeValue, nTexCoordCount * compressDimension);

	pVertexDataPackage->m_pTexCoords[nTexIdx] = (float*)pEncodeValue;
	if (bNeedTexCoordZ)
	{
		float* pTexCoordZ = new(std::nothrow) float[nTexCoordCount];
		streamSkeleton.Load(pTexCoordZ, nTexCoordCount);
		pVertexDataPackage->m_pTexCoords[nTexIdx + nTotalTexCount] = (float*)pTexCoordZ;
		pVertexDataPackage->m_TexCoordCount[nTexIdx + nTotalTexCount] = nTexCoordCount;
		pVertexDataPackage->m_nTexDimensions[nTexIdx + nTotalTexCount] = 1;
		pVertexDataPackage->m_nVertexOptions |= VO_TEXTURE_COORD_Z_IS_MATRIX;
	}
	return true;
}

void S3MBTools::LoadMaterial(string strJsonMaterials, std::map<string, Material*>& mapMaterial)
{
	//strJsonMaterials = StringHelper::UTF8_to_ANSI(strJsonMaterials);
	JsonValue* pJsonValue = new JsonValue(strJsonMaterials);
	JsonValue* pJsonArray = NULL;
	if (!pJsonValue->GetValue(S3MB_MATERIAL, pJsonArray))
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
		LoadMatFromJson(*pOneTile, pMaterial->m_strName, pTechnique);
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

void S3MBTools::LoadMatFromJson(JsonValue& jsonMaterial, string& strMaterialName, Technique* pTechnique)
{
	Pass* pPass = pTechnique->CreatePass();
	JsonValue* pJsonContent = NULL;
	jsonMaterial.GetValue(S3MB_MATERIAL, pJsonContent);

	pJsonContent->GetValue(S3MB_ID, strMaterialName);
	pJsonContent->GetValue(S3MB_MATERIAL_AMBIENT, pPass->m_Ambient);
	pJsonContent->GetValue(S3MB_MATERIAL_DIFFUSE, pPass->m_Diffuse);
	pJsonContent->GetValue(S3MB_MATERIAL_SPECULAR, pPass->m_Specular);
	pJsonContent->GetValue(S3MB_MATERIAL_SHINESS, pPass->m_Shininess);

	string strCullMode;
	pJsonContent->GetValue(S3MB_MATPASS_CULLMODE, strCullMode);
	pPass->m_CullMode = CullModeFromString(strCullMode);
	pJsonContent->GetValue(S3MB_MATERIAL_TRANSPARENTSORT, pPass->m_TransparentSorting);

	//PBR
	PBRParams* pPBR = NULL;
	LoadPBRFromJson(*pJsonContent, pPBR);
	pPass->m_pPRBMaterial = pPBR;

	//TextureUnitStates
	JsonValue *pJsonArrayTextUnit = NULL;
	JsonValue *pJsonTextUnit = new JsonValue(nullValue);
	pJsonContent->GetValue(S3MB_MATERIAL_TEXTUNITS, pJsonArrayTextUnit);
	int nTextureSize = pJsonArrayTextUnit->GetArraySize();
	for (int i = 0; i < nTextureSize; i++)
	{
		TextureUnitState* pTexUnit = pPass->CreateTextureUnitState();
		pJsonArrayTextUnit->GetAt(i, pJsonTextUnit);
		LoadTUSFromJson(*pJsonTextUnit, pTexUnit);
	}
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
	JsonValue* pJsonSub = NULL;
	if (jsonPBR.GetValue(S3MB_MATERIAL_PBRTYPE_MR, pJsonSub)
		&& pJsonSub != NULL)
	{
		PBRMetallicRough* pPBRMR = new PBRMetallicRough();
		pJsonSub->GetValue(S3MB_MATERIAL_PBRM_ROUGHNESSTEXTURE, pPBRMR->m_strMetallicRoughnessTexture);
		pJsonSub->GetValue(S3MB_MATERIAL_PBRM_METALLICFACTOR, pPBRMR->m_fMetallicFactor);
		pJsonSub->GetValue(S3MB_MATERIAL_PBRM_ROUGHNESSFACTOR, pPBRMR->m_fRoughnessFactor);

		pPBR = pPBRMR;
	}
	else 	if (jsonPBR.GetValue(S3MB_MATERIAL_PBRTYPE_SG, pJsonSub)
		&& pJsonSub != NULL)
	{
		PBRSpecularGlossy* pPBRSG = new PBRSpecularGlossy();
		pJsonSub->GetValue(S3MB_MATERIAL_PBRS_DIFFUSEFACTOR, pPBRSG->m_vec4DiffuseFactor);
		pJsonSub->GetValue(S3MB_MATERIAL_PBRS_SPECULARFACTOR, pPBRSG->m_vec3SpecularFactor);
		pJsonSub->GetValue(S3MB_MATERIAL_PBRS_GLOSSINESSFACTOR, pPBRSG->m_fGlossinessFactor);
		pJsonSub->GetValue(S3MB_MATERIAL_PBRS_DIFFUSETEXTURE, pPBRSG->m_strDiffuseTexture);
		pJsonSub->GetValue(S3MB_MATERIAL_PBRS_SPECULARGLOSSINESSTEXTURE, pPBRSG->m_strSpecularGlossinessTexture);

		pPBR = pPBRSG;
	}
	if (pPBR != NULL)
	{
		pJsonSub->GetValue(S3MB_MATERIAL_PBR_EMISSIVEFACTOR, pPBR->m_vec3EmissiveFactor);
		pJsonSub->GetValue(S3MB_MATERIAL_PBR_EMISSIVETEXTURE, pPBR->m_strEmissiveTexture);
		pJsonSub->GetValue(S3MB_MATERIAL_PBR_NORMALTEXTURE, pPBR->m_strNormalTexture);
		pJsonSub->GetValue(S3MB_MATERIAL_PBR_OCCLUSIONTEXTURE, pPBR->m_strOcclusionTexture);
		pJsonSub->GetValue(S3MB_MATERIAL_PBR_BASECOLOR, pPBR->m_vec4BaseColor);
		pJsonSub->GetValue(S3MB_MATERIAL_PBR_BASECOLORTEXTURE, pPBR->m_strBaseColorTexture);
		string strAlphaMode;
		pJsonSub->GetValue(S3MB_MATERIAL_PBR_ALPHAMODE, strAlphaMode);
		pPBR->m_AlphaMode = AlphaModeFromString(strAlphaMode);
		pJsonSub->GetValue(S3MB_MATERIAL_PBR_ALPHACUTOFF, pPBR->m_fAlphaCutoff);
	}
}

void S3MBTools::LoadTUSFromJson(JsonValue& jsonTexture, TextureUnitState* pTextureUnitState)
{
	JsonValue* pJsonContent = NULL;
	jsonTexture.GetValue(S3MB_MATERIAL_TEXTUNIT, pJsonContent);

	pJsonContent->GetValue(S3MB_ID, pTextureUnitState->m_strTextureName);
	pJsonContent->GetValue(S3MB_TEXTURE_URL, pTextureUnitState->m_strTexureURL);

	//AddressMode
	JsonValue *pJsonAddMode = NULL;
	pJsonContent->GetValue(S3MB_MATERIAL_TEXTUNIT_ADDMODE, pJsonAddMode);
	AddressModeFromJson(*pJsonAddMode, pTextureUnitState);
	delete pJsonAddMode;
	pJsonAddMode = NULL;

	//TextureFilteringOption
	int nOptionVal = 0;
	pJsonContent->GetValue(S3MB_MATERIAL_TEXTUNIT_FILTEROPTION, nOptionVal);
	pTextureUnitState->m_TextureFilteringOption = (FilterOptions)nOptionVal;
	pJsonContent->GetValue(S3MB_MATERIAL_TEXTUNIT_FILTERMIN, nOptionVal);
	pTextureUnitState->m_MinFilter = (FilterOptions)nOptionVal;
	pJsonContent->GetValue(S3MB_MATERIAL_TEXTUNIT_FILTERMAX, nOptionVal);
	pTextureUnitState->m_MaxFilter = (FilterOptions)nOptionVal;

	//TexModMatrix
	pJsonContent->GetValue(S3MB_MATERIAL_TEXTUNIT_TEXMODMATRIX, pTextureUnitState->m_TexModMatrix);

	delete pJsonContent;
	pJsonContent = NULL;
}

CullingMode S3MBTools::CullModeFromString(string strType)
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

PBRParams::AlphaMode S3MBTools::AlphaModeFromString(string strType)
{

	PBRParams::AlphaMode emValue = PBRParams::PBRAM_UnKnown;
	if (strType.compare(S3MB_MATERIAL_PBR_ALPHAMODE_OPAQUE) == 0)
	{
		emValue = PBRParams::PBRAM_OPAQUE;
	}
	else 	if (strType.compare(S3MB_MATERIAL_PBR_ALPHAMODE_MASK) == 0)
	{
		emValue = PBRParams::PBRAM_MASK;
	}
	else 	if (strType.compare(S3MB_MATERIAL_PBR_ALPHAMODE_BLEND) == 0)
	{
		emValue = PBRParams::PBRAM_BLEND;
	}
	return emValue;
}

void S3MBTools::AddressModeFromJson(JsonValue& jsonAddMode, TextureUnitState* pTextureUnitState)
{
	int nValue = 0;
	jsonAddMode.GetValue(S3MB_TEXTURE_U, nValue);
	pTextureUnitState->m_AddressMode.u = (TextureUnitState::TextureAddressingMode)nValue;
	jsonAddMode.GetValue(S3MB_TEXTURE_V, nValue);
	pTextureUnitState->m_AddressMode.v = (TextureUnitState::TextureAddressingMode)nValue;
	jsonAddMode.GetValue(S3MB_TEXTURE_W, nValue);
	pTextureUnitState->m_AddressMode.w = (TextureUnitState::TextureAddressingMode)nValue;
}

void S3MBTools::LoadTexture(MemoryStream &streamTexture, std::map<string, TextureDataInfo*>& mapTexture)
{
	int nImageCount = 0;
	streamTexture >> nImageCount;

	for (int k = 0; k < nImageCount; k++)
	{
		TextureDataInfo* pTexture = new TextureDataInfo();
		string strTextureName;
		streamTexture >> strTextureName;
		StreamAlign(streamTexture, true);

		streamTexture >> pTexture->m_nLevel;
		pTexture->m_bMipmap = pTexture->m_nLevel > 0;

		TextureData* pTextureData = new TextureData();
		pTexture->m_pTextureData = pTextureData;

		LoadTextureData(streamTexture, pTextureData);

		mapTexture[strTextureName] = pTexture;
	}
}

void S3MBTools::LoadTextureData(MemoryStream &streamTexture, TextureData* pTextureData)
{
	streamTexture >> pTextureData->m_nWidth;
	streamTexture >> pTextureData->m_nHeight;
	streamTexture >> pTextureData->m_CompressType;
	streamTexture >> pTextureData->m_nSize;
	streamTexture >> (Byte&)pTextureData->m_enFormat;

	pTextureData->m_pBuffer = new(std::nothrow) Byte[pTextureData->m_nSize];
	if (pTextureData->m_pBuffer == NULL)
	{
		delete pTextureData;
		pTextureData = NULL;
		return;
	}
	streamTexture.Load(pTextureData->m_pBuffer, pTextureData->m_nSize);
}

void S3MBTools::ProcessTextureData(TextureData* pTextureData, CodecType nCompressType)
{
	if (pTextureData->m_CompressType == nCompressType)
	{
		return;
	}

	unsigned int comp = 3;
	if (PF_B8G8R8 != pTextureData->m_enFormat && \
		PF_R8G8B8 != pTextureData->m_enFormat)
	{
		comp = 4;
	}

	Byte* pDecodeOut = NULL;
	unsigned int nDecodeSize = 0;
	if (pTextureData->m_CompressType != encNONE)
	{
		//pTextureData->m_CompressType不是None就是enrS3TCDXTN？
		nDecodeSize = ImageOperator::Decode(comp, pTextureData->m_nWidth, \
			pTextureData->m_nHeight, &pDecodeOut, \
			pTextureData->m_pBuffer, pTextureData->m_CompressType);
	}
	else//encNONE
	{
		if (comp == 3)//RGB->RGBA
		{
			ImageData img;
			img.nWidth = pTextureData->m_nWidth;
			img.nHeight = pTextureData->m_nHeight;
			img.btBitsPixel = 24;
			img.nWidthBytes = ImgToolkit::AlignWidth(IPF_RGB, img.nWidth);
			unsigned int nSize = img.nHeight * img.nWidthBytes;
			img.pBits = pTextureData->m_pBuffer;

			//memcpy(img.pBits,pTextureData->m_pBuffer,nSize);

			int nWidthBytes = ImgToolkit::AlignWidth(IPF_RGBA, img.nWidth);
			nDecodeSize = nWidthBytes * img.nHeight;

			Byte* pByte = new Byte[nDecodeSize];
			Byte nAlpha = 255;

			int nImgWidthBytes = img.nWidthBytes;
			if (pTextureData->m_nSize != img.nWidthBytes * img.nHeight)
			{
				nImgWidthBytes = img.nWidth * 3;
			}

			for (unsigned int y = 0; y < (unsigned int)img.nHeight; y++)
			{
				for (unsigned int x = 0; x < (unsigned int)img.nWidth; x++)
				{
					unsigned int nValue = ImgToolkit::GetValue((Byte*)img.pBits, nImgWidthBytes, (PixelFormat)img.btBitsPixel, x, y);
					//if (img.btBitsPixel == PF_B8G8R8)
					//{
					//	nValue |= (nAlpha << 24);
					//}
					//else
					{
						Byte nR = Palette::REDVAL(nValue);
						Byte nG = Palette::GREENVAL(nValue);
						Byte nB = Palette::BLUEVAL(nValue);

						//nValue = RGBA(nB, nG, nR, nAlpha);
						nValue = RGBA(nR, nG, nB, nAlpha);
					}

					ImgToolkit::SetValue(pByte, nWidthBytes, IPF_RGBA, x, y, nValue);
				}
			}

			pDecodeOut = (Byte*)pByte;
			comp = 4;
		}
		else
		{
			if (pTextureData->m_enFormat != PF_BYTE_LA)
			{
				unsigned int nDataSize = pTextureData->m_nWidth * pTextureData->m_nHeight * comp;
				pDecodeOut = new Byte[nDataSize];
				memcpy(pDecodeOut, pTextureData->m_pBuffer, nDataSize);
			}
		}
	}

	//数据展开
	if (pTextureData->m_enFormat == PF_BYTE_LA)
	{
		nDecodeSize = pTextureData->m_nWidth * pTextureData->m_nHeight * 4;
		pDecodeOut = new Byte[nDecodeSize];
		for (unsigned int y = 0; y < (unsigned int)pTextureData->m_nHeight; y++)
		{
			for (unsigned int x = 0; x < (unsigned int)pTextureData->m_nWidth; x++)
			{
				int nTexIndex = y * pTextureData->m_nWidth + x;
				Byte alpha = pTextureData->m_pBuffer[2 * nTexIndex];
				Byte color = pTextureData->m_pBuffer[2 * nTexIndex + 1];
				pDecodeOut[4 * nTexIndex] = alpha;
				pDecodeOut[4 * nTexIndex + 1] = alpha;
				pDecodeOut[4 * nTexIndex + 2] = alpha;
				pDecodeOut[4 * nTexIndex + 3] = color;
			}
		}
	}

	if (nCompressType != encNONE)
	{
		assert(pDecodeOut != NULL);

		Byte* pEncodeOut = NULL;
		unsigned int nCompressedTextureSize = ImageOperator::Encode(comp, \
			pTextureData->m_nWidth, pTextureData->m_nHeight, \
			pDecodeOut, &pEncodeOut, nCompressType);

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

	pTextureData->m_enFormat = PF_BYTE_RGBA;
	pTextureData->m_CompressType = nCompressType;
}

bool S3MBTools::HasSelectionInfo(VertexDataPackage* pVertexDataPack)
{
	bool bHasSelInfo = FALSE;
	if (pVertexDataPack->m_nSecondVertexColorCount != 0 &&
		pVertexDataPack->m_pSecondVertexColor != NULL)
	{
		bHasSelInfo = TRUE;
	}
	else
	{
		int nTexIndex = 0;
		for (; nTexIndex < SMSCN_MAX_TEXTURE_COORD_SETS; nTexIndex++)
		{
			//if (pVertexDataPack->m_TexCoordCount[nTexIndex] != pVertexDataPack->m_nVerticesCount
			//	&& pVertexDataPack->m_TexCoordCount[nTexIndex] > 0)
			//{
			//	bHasSelInfo = TRUE;
			//	break;
			//}
			//else 
			if (pVertexDataPack->m_nTexDimensions[nTexIndex] == SIZE_PER_DM_INSTANCE
				|| pVertexDataPack->m_nTexDimensions[nTexIndex] == SIZE_PER_INSTANCE)
			{
				bHasSelInfo = TRUE;
				break;
			}
		}
	}

	return bHasSelInfo;
}

bool S3MBTools::SaveSelectionInfo(std::map<string, Geometry*>& m_mapGeometry, MemoryStream &m_streamSelectionInfo)
{
	std::map<string, Geometry*>::iterator it = m_mapGeometry.begin();
	if (it == m_mapGeometry.end())
	{
		return false;
	}
	bool m_bHasSelectionInfo = HasSelectionInfo(it->second->m_pVertexDataPackage);
	if (!m_bHasSelectionInfo)
	{
		return false;
	}

	int nCount = m_mapGeometry.size();

	m_streamSelectionInfo.Init();

	m_streamSelectionInfo << nCount;
	for (; it != m_mapGeometry.end(); it++)
	{
		Geometry *pGeometry = it->second;
		VertexDataPackage* pVertexDataPackage = pGeometry->m_pVertexDataPackage;

		// 非实例化的情况下计算dictSelectInfo
		if (pVertexDataPackage->m_pSecondVertexColor)
		{
			std::map<uInt, std::vector<SelectInfo*> > dictSelectInfo;

			uInt nID = pVertexDataPackage->m_pSecondVertexColor[0];

			vector<SelectInfo*> delArray;
			SelectInfo* pSelectInfo = new SelectInfo;
			delArray.push_back(pSelectInfo);
			pSelectInfo->m_nVertexColorOffset = 0;
			pSelectInfo->m_nVertexColorCount = 0;

			std::vector<SelectInfo*> vecSelectInfo;
			vecSelectInfo.push_back(pSelectInfo);
			dictSelectInfo[nID] = vecSelectInfo;

			for (int j = 0; j < pVertexDataPackage->m_nSecondVertexColorCount; j++)
			{
				std::map<uInt, std::vector<SelectInfo*> >::iterator itorCurrentSel;
				uInt nCurrentID = pVertexDataPackage->m_pSecondVertexColor[j];

				if (nCurrentID == nID)
				{
					pSelectInfo->m_nVertexColorCount++;
				}
				else
				{
					pSelectInfo = new SelectInfo;
					delArray.push_back(pSelectInfo);
					pSelectInfo->m_nVertexColorOffset = j;
					pSelectInfo->m_nVertexColorCount = 1;

					itorCurrentSel = dictSelectInfo.find(nCurrentID);
					if (itorCurrentSel == dictSelectInfo.end())
					{
						std::vector<SelectInfo*> vecCurrentSelectInfo;
						vecCurrentSelectInfo.push_back(pSelectInfo);
						dictSelectInfo[nCurrentID] = vecCurrentSelectInfo;
					}
					else
					{
						itorCurrentSel->second.push_back(pSelectInfo);
					}
					nID = nCurrentID;
				}
			}

			std::map<uInt, std::vector<SelectInfo*> >::iterator posSelectInfo;
			uInt nDictID = 0;
			std::vector<SelectInfo*> arrInfo;

			m_streamSelectionInfo << pGeometry->m_strGeoName;

			uInt nCurrentID = 0xffffffff;
			int nRemove = 0;
			std::map<uInt, std::vector<SelectInfo*> >::iterator itorCurrentSel;
			itorCurrentSel = dictSelectInfo.find(nCurrentID);
			if (itorCurrentSel != dictSelectInfo.end())
			{
				nRemove = 1;
			}

			int nSelectInfoCount = dictSelectInfo.size() - nRemove;
			m_streamSelectionInfo << nSelectInfoCount;

			for (posSelectInfo = dictSelectInfo.begin(); posSelectInfo != dictSelectInfo.end(); ++posSelectInfo)
			{
				nDictID = posSelectInfo->first;
				arrInfo = posSelectInfo->second;
				if (nDictID == 0xffffffff)
					continue;


				int nSize = arrInfo.size();
				m_streamSelectionInfo << nDictID;
				m_streamSelectionInfo << nSize;
				for (int j = 0; j < nSize; j++)
				{
					m_streamSelectionInfo << arrInfo[j]->m_nVertexColorOffset;
					m_streamSelectionInfo << arrInfo[j]->m_nVertexColorCount;
				}
			}

			int nDelSize = delArray.size();
			for (int j = 0; j < nDelSize; j++)
			{
				SelectInfo* pInfo = delArray[j];
				delete pInfo;
				pInfo = NULL;
			}
			delArray.clear();
		}
		else
		{
			int nTexIndex = 0;
			for (; nTexIndex < SMSCN_MAX_TEXTURE_COORD_SETS; nTexIndex++)
			{
				if (pVertexDataPackage->m_TexCoordCount[nTexIndex] != pVertexDataPackage->m_nVerticesCount
					&& pVertexDataPackage->m_TexCoordCount[nTexIndex] > 0)
				{
					break;
				}
				else if (pVertexDataPackage->m_nTexDimensions[nTexIndex] == SIZE_PER_DM_INSTANCE
					|| pVertexDataPackage->m_nTexDimensions[nTexIndex] == SIZE_PER_INSTANCE)
				{
					break;
				}
			}

			int nInstanceSize = pVertexDataPackage->m_TexCoordCount[nTexIndex];
			float* pValue = pVertexDataPackage->m_pTexCoords[nTexIndex];
			uInt* pUint = (uInt*)pValue;
			vector<BatchSelAndVertexColor*> delArray;

			std::map<uInt, BatchSelAndVertexColor* > dictSelectInfo;
			BatchSelAndVertexColor* pSelColor = NULL;

			int nTexDimension = pVertexDataPackage->m_nTexDimensions[nTexIndex];
			for (int nInstanceIdx = 0; nInstanceIdx < nInstanceSize; nInstanceIdx++)
			{
				uInt nSelectionID = 0;
				if (nTexDimension == SIZE_PER_DM_INSTANCE)
				{
					nSelectionID = pUint[SIZE_PER_DM_INSTANCE * nInstanceIdx + 28];
				}
				else if (nTexDimension == SIZE_PER_INSTANCE)
				{
					nSelectionID = pUint[SIZE_PER_INSTANCE * nInstanceIdx + 16];
				}

				if (dictSelectInfo.find(nSelectionID) != dictSelectInfo.end())
				{
					pSelColor = dictSelectInfo[nSelectionID];
				}
				else
				{
					pSelColor = new BatchSelAndVertexColor;
					dictSelectInfo[nSelectionID] = pSelColor;
					delArray.push_back(pSelColor);
				}

				pSelColor->mapInstanceIdVec[nSelectionID].push_back(nInstanceIdx);
			}

			m_streamSelectionInfo << pGeometry->m_strGeoName;

			int nSelectInfoCount = delArray.size();
			m_streamSelectionInfo << nSelectInfoCount;

			std::map<uInt, BatchSelAndVertexColor* >::iterator posSelectInfo;
			uInt nDictID = 0;
			BatchSelAndVertexColor* arrInfo;

			for (posSelectInfo = dictSelectInfo.begin(); posSelectInfo != dictSelectInfo.end(); ++posSelectInfo)
			{
				nDictID = posSelectInfo->first;
				arrInfo = posSelectInfo->second;


				int nSize = arrInfo->mapInstanceIdVec[nDictID].size();
				m_streamSelectionInfo << nDictID;
				m_streamSelectionInfo << nSize;
				for (int j = 0; j < nSize; j++)
				{
					uInt nInstanceIndex = arrInfo->mapInstanceIdVec[nDictID][j];
					m_streamSelectionInfo << nInstanceIndex;
				}
			}

			int nDelSize = delArray.size();
			for (int j = 0; j < nDelSize; j++)
			{
				BatchSelAndVertexColor* pInfo = delArray[j];
				delete pInfo;
				pInfo = NULL;
			}
			delArray.clear();
		}
	}
	return true;
}

bool S3MBTools::SaveStreamData2File(const string strDestFilePath, MemoryStream& streamShell, MemoryStream& streamSkeleton, \
	MemoryStream& streamSecondColor, string& strJsonMaterials, MemoryStream& streamTexture, bool bHasSelectionInfo, MemoryStream& streamSelInfo)
{
	//改后缀名，上层处理可能会传入其他类型文件后缀
	string strDestFilePathInner = StringHelper::ChangeExt(strDestFilePath, ".s3mb");
	std::ofstream ofs(strDestFilePathInner, ios::out | ios::binary | ios::trunc);

	//s3mb版本
	float fVersion = 1;
	ofs.write((char*)&fVersion, sizeof(fVersion));

	//流合并
	MemoryStream memStreamTotal;
	memStreamTotal.Init();
	uInt nOptions = bHasSelectionInfo ? SVO_HasInstSelInfo : 0;
	memStreamTotal << nOptions;

	memStreamTotal << (uInt)streamShell.GetLength();
	memStreamTotal.Save(streamShell.GetDataPtr(), streamShell.GetLength());
	streamShell.Clear();

	memStreamTotal << (uInt)streamSkeleton.GetLength();
	memStreamTotal.Save(streamSkeleton.GetDataPtr(), streamSkeleton.GetLength());
	streamSkeleton.Clear();

	memStreamTotal << (uInt)streamSecondColor.GetLength();
	memStreamTotal.Save(streamSecondColor.GetDataPtr(), streamSecondColor.GetLength());
	streamSecondColor.Clear();

	memStreamTotal << (uInt)streamTexture.GetLength();
	memStreamTotal.Save(streamTexture.GetDataPtr(), streamTexture.GetLength());
	streamTexture.Clear();

	memStreamTotal << strJsonMaterials;

	if (bHasSelectionInfo)
	{
		memStreamTotal.Save(streamSelInfo.GetDataPtr(), streamSelInfo.GetLength());
		streamSelInfo.Clear();
	}

	//压缩流
	byte* pZippedData = NULL;
	uInt nZippedSize = 0;
	uInt nTotalLength = (uInt)memStreamTotal.GetLength();
	pZippedData = new byte[nTotalLength];
	nZippedSize = nTotalLength;
	bool result = ZipData(pZippedData, nZippedSize, memStreamTotal.GetDataPtr(), nTotalLength);
	memStreamTotal.Clear();

	//MemoryStream teststream;
	//UnZipData(teststream, pZippedData, nZippedSize);

	//UInt x = 1;
	//teststream >> x;
	//LoadStream(teststream, streamShell);
	//LoadStream(teststream, streamSkeleton);
	//LoadStream(teststream, streamSecondColor);
	//LoadStream(teststream, streamTexture);
	//string l;
	//teststream >> l;


	//写入文件
	ofs.write((char*)&nZippedSize, sizeof(uInt));
	ofs.write((char*)pZippedData, nZippedSize);
	ofs.close();
	delete pZippedData;
	return true;
}

bool S3MBTools::ZipData(byte *pvDestBuffer, uInt &dwDestLen, const byte *pvSrcBuffer, uInt dwSrcLen, int nLevel /*= 8*/)
{
	unsigned long ulDestLen = dwDestLen;	//	WhiteBox_Ignore
	// 这里搞得这么麻烦，是因为compress2的第二个参数是一个指向unsigned long型的指针，需要类型完全匹配，
	// uInt实际上就是unsigned int，而unsigned long在64位系统（如Solaris）上是64位的，就出问题了
	//return compress2(pvDestBuffer, &dwDestLen, pvSrcBuffer, dwSrcLen, nLevel) == Z_OK;
	bool bResult = FALSE;
	int nResult = compress2(pvDestBuffer, &ulDestLen, pvSrcBuffer, dwSrcLen, nLevel);
	dwDestLen = (uInt)ulDestLen;

	// 防错处理
	if (nResult == Z_OK)
	{
		bResult = TRUE;
	}
	else if (nResult == Z_MEM_ERROR)
	{
		assert("内存分配不足");
	}
	else if (nResult == Z_BUF_ERROR)
	{
		assert("输出缓存没有足够的空间");
	}
	else if (nResult == Z_STREAM_ERROR)
	{
		assert("压缩文件参数非法");
	}
	else
	{ // 不可识别的错误，
		assert(FALSE);
	}

	return bResult;
}

void S3MBTools::LoadShell(MemoryStream& streamShell, RenderOperationGroup* pROGroup)
{
	int nCount = 0;
	streamShell >> nCount;

	BoundingSphere sphere;

	for (int i = 0; i < nCount; i++)
	{
		BoundingSphere sphereTemp;
		if (IsGeodeWithoutChild(streamShell))
		{
			RenderOperationGeode* pGeode = new RenderOperationGeode();
			pROGroup->AddChild(pGeode);
			LoadROGeodeWithoutChild(pROGroup, pGeode, streamShell);
			sphereTemp = pGeode->GetBoudingSphere();
		}
		else
		{
			RenderOperationPagedLOD* pPagedLOD = new RenderOperationPagedLOD();
			pPagedLOD->SetParentNode(pROGroup);
			pROGroup->AddChild(pPagedLOD);
			LoadShellPagedLOD(pROGroup, pPagedLOD, streamShell);

			sphereTemp = pPagedLOD->GetBoudingSphere();
		}
		sphere.ExpandBy(sphereTemp);
	}
	pROGroup->SetBoudingSphere(sphere.m_center, sphere.m_radius);

	StreamAlign(streamShell, true);
}

bool S3MBTools::IsGeodeWithoutChild(MemoryStream& stream)
{
	long lPos = stream.GetReadPosition();
	float dbDis = 0.0;
	stream >> dbDis;
	stream.SetReadPosition(lPos);
	return IS0(dbDis);
}

void S3MBTools::LoadROGeodeWithoutChild(RenderOperationGroup* pROGroup, RenderOperationGeode* pGeode, MemoryStream& stream)
{
	//1	LODDistance 
	float dbDis = 0.0;
	stream >> dbDis;

	//2	RangeMode 
	unsigned short uRangeMode = PIXEL_SIZE_ON_SCREEN;
	stream >> uRangeMode;

	//3	包围球 
	Vector3d boundingSphereCenter;
	double dRadius = 0.0;
	stream >> boundingSphereCenter.x;
	stream >> boundingSphereCenter.y;
	stream >> boundingSphereCenter.z;
	stream >> dRadius;

	//4	挂接的下层文件
	string strChildTile;
	stream >> strChildTile;
	assert(strChildTile.empty());

	//4	Geode(s)
	int nCount = 0;
	stream >> nCount;
	assert(nCount == 1);//因为生成的时候每一个Geode被打包成了dbDis=0.0的一个PagedLOD
	for (int i = 0; i < nCount; i++)
	{
		LoadShellROGeode(pROGroup, pGeode, stream);
	}
}

void S3MBTools::LoadShellPagedLOD(RenderOperationGroup* pROGroup, RenderOperationPagedLOD* pROPagedLOD, MemoryStream& stream)
{
	//1	LODDistance 
	float dbDis = 0.0;
	stream >> dbDis;

	//2	RangeMode 
	unsigned short uRangeMode = PIXEL_SIZE_ON_SCREEN;
	stream >> uRangeMode;
	pROPagedLOD->SetRangeMode(RangeMode(uRangeMode));

	if (uRangeMode == PIXEL_SIZE_ON_SCREEN)
	{
		pROPagedLOD->GetRanges().push_back(std::vector<std::pair<float, float> >::value_type(0, dbDis));
	}
	else
	{
		pROPagedLOD->GetRanges().push_back(std::vector<std::pair<float, float> >::value_type(9999.0, dbDis));
	}

	//3	包围球 
	Vector3d boundingSphereCenter;
	double dRadius = 0.0;
	stream >> boundingSphereCenter.x;
	stream >> boundingSphereCenter.y;
	stream >> boundingSphereCenter.z;
	stream >> dRadius;
	pROPagedLOD->SetBoudingSphere(boundingSphereCenter, dRadius);

	//4	挂接的下层文件
	string strChildTile;
	stream >> strChildTile;
	if (!strChildTile.empty())
	{
		pROPagedLOD->GetFileNames().push_back(strChildTile);
	}

	//4	Geode(s)
	int nCount = 0;
	stream >> nCount;

	for (int i = 0; i < nCount; i++)
	{
		RenderOperationGeode* pGeode = new RenderOperationGeode;
		pROPagedLOD->AddChild(pGeode);
		LoadShellROGeode(pROGroup, pGeode, stream);
	}
}

void S3MBTools::LoadShellROGeode(RenderOperationGroup* pROGroup, RenderOperationGeode* pGeode, MemoryStream& stream)
{
	Matrix4d matGeode;
	stream.Load(matGeode.GetPointer(), 16);
	pGeode->SetPosition(matGeode);

	int nCount = 0;
	stream >> nCount;
	string strSkeletonName;
	for (int i = 0; i < nCount; i++)
	{
		stream >> strSkeletonName;
		Geometry* pGeometry = pROGroup->GetGeometry(strSkeletonName);
		assert(pGeometry != NULL);
		pGeode->AddGeometry(pGeometry);
	}
	pGeode->ComputerBoundingBox();
}

void S3MBTools::SaveShell(RenderOperationGroup* pROGroup, MemoryStream& stream)
{
	stream.Init();
	int nCount = pROGroup->GetNumChildren();
	stream << nCount;

	for (int i = 0; i < nCount; i++)
	{
		RenderOperationNode* pChildNode = pROGroup->GetChild(i);
		if (pChildNode == NULL)
		{
			assert(FALSE);
			continue;
		}
		string strName = pChildNode->ClassName();
		if (StringHelper::CompareNoCase(strName, "PagedLOD"))
		{
			S3MBTools::SaveShellPagedLOD((RenderOperationPagedLOD*)pChildNode, stream);
		}
		else if (StringHelper::CompareNoCase(strName, "Geode"))
		{
			//构造一个pagedlod存储
			float dLODDistance = 0.0;
			stream << dLODDistance;

			unsigned short uRangeMode = PIXEL_SIZE_ON_SCREEN;
			stream << uRangeMode;

			RenderOperationGeode* pGeode = (RenderOperationGeode*)pChildNode;
			BoundingSphere boundingSphere = pChildNode->GetBoudingSphere();
			if (!boundingSphere.valid())
			{
				Matrix4d matrix = pGeode->GetPosition();
				pGeode->ComputerBoundingBox();
				BoundingBox bbox = pGeode->GetBoundingBox();
				bbox.Transform(matrix);
				boundingSphere.m_center = bbox.GetCenter();
				boundingSphere.m_radius = (bbox.GetMax() - bbox.GetMin()).Length() / 2;
			}
			Vector3d boundingSphereCenter = boundingSphere.GetCenter();
			stream << boundingSphereCenter.x;
			stream << boundingSphereCenter.y;
			stream << boundingSphereCenter.z;
			stream << boundingSphere.GetRadius();

			string strChildTile;
			stream << strChildTile;

			int nGeodeCount = 1;
			stream << nGeodeCount;

			S3MBTools::SaveShellROGeode(pGeode, stream);
		}
	}
	S3MBTools::StreamAlign(stream, false);
}

void S3MBTools::SaveShellPagedLOD(RenderOperationPagedLOD* pROPagedLOD, MemoryStream& stream)
{
	//1	LODDistance 
	float dLODDistance = pROPagedLOD->GetRanges().size() > 0 ? \
		pROPagedLOD->GetRanges()[0].second : -1.0;

	stream << dLODDistance;

	//2	RangeMode 
	unsigned short uRangeMode = pROPagedLOD->GetRangeMode();
	stream << uRangeMode;

	//3	包围球 
	BoundingSphere boundingSphere = pROPagedLOD->GetBoudingSphere();
	Vector3d boundingSphereCenter = boundingSphere.GetCenter();
	stream << boundingSphereCenter.x;
	stream << boundingSphereCenter.y;
	stream << boundingSphereCenter.z;
	stream << boundingSphere.GetRadius();

	//4	挂接的下层文件
	string strChildTile = pROPagedLOD->GetFileNames().size() > 0 ? \
		pROPagedLOD->GetFileNames()[0] : "";
	if (!strChildTile.empty() && !StringHelper::CompareNoCase(StringHelper::GetExt(strChildTile), ".s3mb"))
	{
		strChildTile = StringHelper::ChangeExt(strChildTile, ".s3mb");
	}
	stream << strChildTile;

	//4	Geode(s)
	int nChildCount = pROPagedLOD->GetNumChildren();
	stream << nChildCount;
	for (int i = 0; i < nChildCount; i++)
	{
		RenderOperationNode* pChildNode = pROPagedLOD->GetChild(i);
		string strName = pChildNode->ClassName();
		if (StringHelper::CompareNoCase(strName, "PagedLOD"))
		{
			assert(FALSE);
		}
		else  if (StringHelper::CompareNoCase(strName, "Geode"))
		{
			S3MBTools::SaveShellROGeode((RenderOperationGeode*)pChildNode, stream);
		}
	}
}

void S3MBTools::SaveShellROGeode(RenderOperationGeode* pROGeode, MemoryStream& stream)
{
	//1、矩阵
	Matrix4d matGeode = pROGeode->GetPosition();
	stream.Save(matGeode.GetPointer(), 16);
	//2、骨架
	int nCount = pROGeode->GetNumGeometry();
	stream << nCount;
	Geometry* pGeometry = NULL;
	for (int i = 0; i < nCount; i++)
	{
		pGeometry = pROGeode->GetGeometry(i);
		stream << pGeometry->m_strGeoName;
	}
}

void S3MBTools::SaveGeometry(std::map<string, Geometry*>& mapGeometry, MemoryStream &streamGeometry, int vertexCompressOptions /*= 19*/)
{
	streamGeometry.Init();
	int nCount = mapGeometry.size();
	streamGeometry << nCount;

	std::map<string, Geometry*>::iterator itGeometry;
	for (itGeometry = mapGeometry.begin(); itGeometry != mapGeometry.end(); itGeometry++)
	{
		streamGeometry << itGeometry->first;
		S3MBTools::StreamAlign(streamGeometry, false);

		Geometry* pGeometry = itGeometry->second;

		// 实例化对象不做顶点压缩
		int nVertexCompressOptions = vertexCompressOptions;
		if (pGeometry->m_bInstanceBatch)
		{
			nVertexCompressOptions = 0;
		}

		VertexDataPackage* pVertexDataPackage = pGeometry->m_pVertexDataPackage;
		S3MBTools::SaveSkeletonData(pVertexDataPackage, pGeometry->m_arrIndexPackage, streamGeometry, nVertexCompressOptions);
	}
}

bool S3MBTools::SaveSkeletonData(VertexDataPackage*& pVertexDataPackage, vector<IndexPackage*>& arrIndexPackage, MemoryStream &streamSkeleton, int vertexCompressOptions /*= 19*/)
{
	int nTagValue = (vertexCompressOptions != 0) ? SV_Compressed : SV_Standard;

	streamSkeleton << nTagValue;

	if (vertexCompressOptions == 0)
	{
		SaveVertex(pVertexDataPackage, streamSkeleton);
		SaveNormal(pVertexDataPackage, streamSkeleton);

		SaveVertexColor(pVertexDataPackage, streamSkeleton);
		SaveSecondVertexColor(pVertexDataPackage, streamSkeleton);
		SaveTextureCoords(pVertexDataPackage, streamSkeleton);

		SaveInstanceInfo(pVertexDataPackage, streamSkeleton);

	}
	else
	{
		pVertexDataPackage->m_nCompressOptions = vertexCompressOptions;
		streamSkeleton << pVertexDataPackage->m_nCompressOptions;
		if (pVertexDataPackage->HasCompressOptions(SVC_Vertex))
		{
			SaveCompressVertex(pVertexDataPackage, streamSkeleton);
		}
		else
		{
			SaveVertex(pVertexDataPackage, streamSkeleton);
		}
		if (pVertexDataPackage->HasCompressOptions(SVC_Normal) && pVertexDataPackage->m_nNormalDimension == 3)
		{
			SaveCompressNormal(pVertexDataPackage, streamSkeleton);
		}
		else
		{
			SaveNormal(pVertexDataPackage, streamSkeleton);
		}

		SaveVertexColor(pVertexDataPackage, streamSkeleton);
		SaveSecondVertexColor(pVertexDataPackage, streamSkeleton);

		if (pVertexDataPackage->HasCompressOptions(SVC_TexutreCoord))
		{
			SaveCompressTextureCoords(pVertexDataPackage, streamSkeleton);
		}
		else
		{
			SaveTextureCoords(pVertexDataPackage, streamSkeleton);
		}

		SaveInstanceInfo(pVertexDataPackage, streamSkeleton);
	}

	int nIndexPackageCount = arrIndexPackage.size();
	streamSkeleton << nIndexPackageCount;

	for (int i = 0; i < nIndexPackageCount; i++)
	{
		IndexPackage* pIndexPackage = arrIndexPackage[i];
		pIndexPackage->Save(streamSkeleton);
	}
	return true;
}

bool S3MBTools::SaveVertex(VertexDataPackage* pVertexDataPack, MemoryStream &stream)
{
	//uint 
	stream << pVertexDataPack->m_nVerticesCount;
	if (pVertexDataPack->m_nVerticesCount <= 0 || pVertexDataPack->m_pVertices == NULL)
	{
		return TRUE;
	}
	//ushort 
	stream << pVertexDataPack->m_nVertexDimension;
	//ushort
	stream << pVertexDataPack->m_nVertexStride;

	stream.Save(pVertexDataPack->m_pVertices, \
		pVertexDataPack->m_nVerticesCount * pVertexDataPack->m_nVertexDimension);

	return TRUE;
}

bool S3MBTools::SaveNormal(VertexDataPackage* pVertexDataPack, MemoryStream &stream)
{
	//uInt
	stream << pVertexDataPack->m_nNormalCount;

	if (pVertexDataPack->m_nNormalCount <= 0 || pVertexDataPack->m_pNormals == NULL)
	{
		return TRUE;
	}
	//UGushort 
	stream << pVertexDataPack->m_nNormalDimension;
	stream << pVertexDataPack->m_nNormalStride;

	stream.Save(pVertexDataPack->m_pNormals, \
		pVertexDataPack->m_nNormalCount * pVertexDataPack->m_nNormalDimension);

	return TRUE;
}

bool S3MBTools::SaveVertexColor(VertexDataPackage* pVertexDataPack, MemoryStream &stream)
{
	stream << pVertexDataPack->m_nVertexColorCount;
	if (pVertexDataPack->m_nVertexColorCount <= 0 || pVertexDataPack->m_pVertexColor == NULL)
	{
		return TRUE;
	}
	byte nByte = 0;
	stream << pVertexDataPack->m_nVertexColorStride;
	stream << nByte;
	stream << nByte;

	for (uInt j = 0; j < pVertexDataPack->m_nVertexColorCount; j++)
	{
		uInt nColor = pVertexDataPack->m_pVertexColor[j];
		byte btR = Palette::REDVAL(nColor);
		byte btG = Palette::GREENVAL(nColor);
		byte btB = Palette::BLUEVAL(nColor);
		byte btA = Palette::ALPHAVAL(nColor);

		stream << btR;
		stream << btG;
		stream << btB;
		stream << btA;
	}
	return TRUE;
}

bool S3MBTools::SaveSecondVertexColor(VertexDataPackage* pVertexDataPack, MemoryStream &stream)
{
	stream << pVertexDataPack->m_nSecondVertexColorCount;
	if (pVertexDataPack->m_nSecondVertexColorCount <= 0 || pVertexDataPack->m_pSecondVertexColor == NULL)
	{
		return TRUE;
	}

	byte nByte = 0;
	stream << pVertexDataPack->m_nSecondVertexColorStride;
	stream << nByte;
	stream << nByte;

	for (uInt j = 0; j < pVertexDataPack->m_nSecondVertexColorCount; j++)
	{
		uInt nColor = pVertexDataPack->m_pSecondVertexColor[j];
		byte btR = Palette::REDVAL(nColor);
		byte btG = Palette::GREENVAL(nColor);
		byte btB = Palette::BLUEVAL(nColor);
		byte btA = Palette::ALPHAVAL(nColor);

		stream << btR;
		stream << btG;
		stream << btB;
		stream << btA;
	}

	return TRUE;
}

bool S3MBTools::SaveTextureCoords(VertexDataPackage* pVertexDataPack, MemoryStream &stream)
{
	unsigned short nTextureCoord = 0;
	for (int i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
	{
		if (pVertexDataPack->m_TexCoordCount[i] == 0)
		{
			break;
		}
		if (IsTexutureCoord(pVertexDataPack->m_nTexDimensions[i]) || IsTexutureCoordStoreW(pVertexDataPack, i))
		{
			nTextureCoord++;
		}
	}

	stream << nTextureCoord;
	byte nByte = 0;
	stream << nByte;
	stream << nByte;
	for (int i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
	{
		if (pVertexDataPack->m_TexCoordCount[i] == 0)
		{
			break;
		}
		if (IsTexutureCoord(pVertexDataPack->m_nTexDimensions[i]) || IsTexutureCoordStoreW(pVertexDataPack, i))
		{
			stream << pVertexDataPack->m_TexCoordCount[i];
			stream << pVertexDataPack->m_nTexDimensions[i];
			stream << pVertexDataPack->m_TexCoordStride[i];
			stream.Save(pVertexDataPack->m_pTexCoords[i], \
				pVertexDataPack->m_TexCoordCount[i] * pVertexDataPack->m_nTexDimensions[i]);
		}
	}
	return TRUE;
}

bool S3MBTools::IsTexutureCoord(unsigned short nDimesion)
{
	//这才是正经的纹理坐标！
	return (nDimesion == 2 || nDimesion == 3 || nDimesion == 4);
}

bool S3MBTools::IsTexutureCoordStoreW(VertexDataPackage* pVertexDataPack, int nTexCoordIndex)
{
	return (pVertexDataPack->HasCompressOptions(SVC_TexutreCoordIsW) && pVertexDataPack->m_nTexDimensions[nTexCoordIndex] == 1);
}

bool S3MBTools::SaveInstanceInfo(VertexDataPackage* pVertexDataPack, MemoryStream &stream)
{
	unsigned short nInstanceInfo = 0;
	byte nByte = 0;
	for (int i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
	{
		if (pVertexDataPack->m_TexCoordCount[i] == 0)
		{
			break;
		}
		if (!IsTexutureCoord(pVertexDataPack->m_nTexDimensions[i]) && !IsTexutureCoordStoreW(pVertexDataPack, i))
		{
			nInstanceInfo++;
		}
	}
	stream << nInstanceInfo;
	stream << nByte;
	stream << nByte;

	if (nInstanceInfo == 0)
	{
		return TRUE;
	}

	for (int i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
	{
		if (pVertexDataPack->m_TexCoordCount[i] == 0)
		{
			break;
		}

		if (pVertexDataPack->m_nTexDimensions[i] == SIZE_PER_INSTANCE)
		{
			stream << pVertexDataPack->m_TexCoordCount[i];
			stream << pVertexDataPack->m_nTexDimensions[i];
			stream << pVertexDataPack->m_TexCoordStride[i];

			float* pValue = pVertexDataPack->m_pTexCoords[i];
			uInt* pUint = (uInt*)pValue;
			for (uInt kIndex = 0; kIndex < pVertexDataPack->m_TexCoordCount[i]; kIndex++)
			{
				// 三个纹理坐标存储矩阵，一个纹理坐标存储选择ID，一个颜色存储选中时顶点颜色
				stream.Save(pValue + kIndex * SIZE_PER_INSTANCE, 16);

				// secondColor拆分为4个byte存储，方便web端使用
				uInt nColor = pUint[SIZE_PER_INSTANCE * kIndex + 16];
				byte btR = Palette::REDVAL(nColor);
				byte btG = Palette::GREENVAL(nColor);
				byte btB = Palette::BLUEVAL(nColor);
				byte btA = Palette::ALPHAVAL(nColor);
				stream << btR;
				stream << btG;
				stream << btB;
				stream << btA;
			}
		}
		else if (pVertexDataPack->m_nTexDimensions[i] == SIZE_PER_DM_INSTANCE)
		{
			stream << pVertexDataPack->m_TexCoordCount[i];
			stream << pVertexDataPack->m_nTexDimensions[i];
			stream << pVertexDataPack->m_TexCoordStride[i];

			float* pValue = pVertexDataPack->m_pTexCoords[i];
			uInt* pUint = (uInt*)pValue;
			for (uInt kIndex = 0; kIndex < pVertexDataPack->m_TexCoordCount[i]; kIndex++)
			{
				stream.Save(pValue + kIndex * SIZE_PER_DM_INSTANCE, 27);

				// vertexColor和secondColor拆分为4个byte存储，方便web端使用
				uInt nColor = pUint[SIZE_PER_DM_INSTANCE * kIndex + 27];
				byte btR = Palette::REDVAL(nColor);
				byte btG = Palette::GREENVAL(nColor);
				byte btB = Palette::BLUEVAL(nColor);
				byte btA = Palette::ALPHAVAL(nColor);
				stream << btR;
				stream << btG;
				stream << btB;
				stream << btA;

				nColor = pUint[SIZE_PER_DM_INSTANCE * kIndex + 28];
				btR = Palette::REDVAL(nColor);
				btG = Palette::GREENVAL(nColor);
				btB = Palette::BLUEVAL(nColor);
				btA = Palette::ALPHAVAL(nColor);
				stream << btR;
				stream << btG;
				stream << btB;
				stream << btA;
			}
		}
		//也不是纹理坐标
		else if (!IsTexutureCoord(pVertexDataPack->m_nTexDimensions[i]))
		{
			stream << pVertexDataPack->m_TexCoordCount[i];
			stream << pVertexDataPack->m_nTexDimensions[i];
			stream << pVertexDataPack->m_TexCoordStride[i];

			stream.Save(pVertexDataPack->m_pTexCoords[i], \
				pVertexDataPack->m_TexCoordCount[i] * pVertexDataPack->m_nTexDimensions[i]);
		}
	}
	return TRUE;
}

bool S3MBTools::SaveCompressVertex(VertexDataPackage* pVertexDataPack, MemoryStream &stream)
{
	//uInt 
	stream << pVertexDataPack->m_nVerticesCount;
	if (pVertexDataPack->m_nVerticesCount <= 0 || pVertexDataPack->m_pVertices == NULL)
	{
		return TRUE;
	}
	// 压缩后统一保存成4short数组，因为3short格式有些显卡不原生支持，性能有问题
	//UGushort
	unsigned short compressDimension = 4;
	stream << compressDimension;
	//UGushort
	stream << pVertexDataPack->m_nVertexStride;

	float* pMinVertexValue = new float[4];
	float fVertCompressConstant;

	short* pEncodeVertices = new(std::nothrow) short[pVertexDataPack->m_nVerticesCount * compressDimension];
	RangeEncode(pVertexDataPack->m_pVertices, pVertexDataPack->m_nVerticesCount, pVertexDataPack->m_nVertexDimension, compressDimension, 14,
		pEncodeVertices, fVertCompressConstant, pMinVertexValue);

	stream << fVertCompressConstant;
	stream.Save(pMinVertexValue, 4);
	stream.Save(pEncodeVertices, pVertexDataPack->m_nVerticesCount * compressDimension);
	delete[] pMinVertexValue;
	delete[] pEncodeVertices;
	return TRUE;
}

void S3MBTools::RangeEncode(float* pValue, uInt nVertexCount, uInt nSrcDimension, uInt nDstDimension, uInt nQuantizationBits, short*& pEncodeValue, float& fNormalConstant, float*& minVal)
{
	float maxVal[4];
	float rangVal[4];
	ComputeMaxMin(pValue, nSrcDimension, nVertexCount, minVal, maxVal);
	for (uInt i = 0; i < nDstDimension; i++)
	{
		rangVal[i] = maxVal[i] - minVal[i];
	}
	float fRange = 0;
	for (uInt i = 0; i < nDstDimension; i++)
	{
		fRange = MAX(fRange, rangVal[i]);
	}
	fNormalConstant = fRange / (1 << nQuantizationBits);
	for (uInt i = 0; i < nVertexCount; i++)
	{
		for (uInt j = 0; j < nDstDimension; j++)
		{
			pEncodeValue[i * nDstDimension + j] = (short)((pValue[i * nSrcDimension + j] - minVal[j]) / fNormalConstant);
		}
	}
}

void S3MBTools::ComputeMaxMin(float* pValue, uInt nDimension, uInt nVertexCount, float* pOutMin, float* pOutMax)
{
	for (int i = 0; i < 4; i++)
	{
		pOutMin[i] = 0.0;
		pOutMax[i] = 0.0;
	}

	int nCount = 0;
	for (uInt i = 0; i < nVertexCount; i++)
	{
		for (uInt j = 0; j < nDimension; j++)
		{
			float fValue = pValue[i * nDimension + j];
			if (fabs(fValue) > FLTMAX)
			{
				fValue = 0.0f;
			}

			if (nCount == 0)
			{
				pOutMax[j] = fValue;
				pOutMin[j] = fValue;
			}
			else
			{
				pOutMax[j] = MAX(pOutMax[j], fValue);
				pOutMin[j] = MIN(pOutMin[j], fValue);
			}
		}
		nCount++;
	}
}

bool S3MBTools::SaveCompressNormal(VertexDataPackage* pVertexDataPack, MemoryStream &stream)
{
	//uInt 
	stream << pVertexDataPack->m_nNormalCount;
	if (pVertexDataPack->m_nNormalCount <= 0 || pVertexDataPack->m_pNormals == NULL)
	{
		return TRUE;
	}
	//UGushort
	stream << pVertexDataPack->m_nNormalDimension;
	stream << pVertexDataPack->m_nNormalStride;

	short* pEncodeNormal = NULL;
	CompressNormal(pVertexDataPack->m_nNormalCount, pVertexDataPack->m_pNormals, pEncodeNormal);

	if (pEncodeNormal != NULL)
	{
		stream.Save(pEncodeNormal, pVertexDataPack->m_nNormalCount * 2);
		delete[] pEncodeNormal;
		pEncodeNormal = NULL;
	}
	return TRUE;
}

bool S3MBTools::CompressNormal(uInt nNormalCount, float* pNormal, short*& pEncodeNormal)
{
	pEncodeNormal = new(std::nothrow) short[nNormalCount * 2];

	double rangeMax = 65535.0;
	Vector3d vector;
	double resultX, resultY;
	for (uInt i = 0; i < nNormalCount; i++)
	{
		vector.x = pNormal[i * 3];
		vector.y = pNormal[i * 3 + 1];
		vector.z = pNormal[i * 3 + 2];

		double magSquared = vector.Length2();
		resultX = vector.x / (fabs(vector.x) + fabs(vector.y) + fabs(vector.z));
		resultY = vector.y / (fabs(vector.x) + fabs(vector.y) + fabs(vector.z));
		if (vector.z < 0)
		{
			double x = resultX;
			double y = resultY;
			double signNotZero = x < 0.0 ? -1.0 : 1.0;
			resultX = (1.0 - fabs(y)) * signNotZero;
			signNotZero = y < 0.0 ? -1.0 : 1.0;
			resultY = (1.0 - fabs(x)) * signNotZero;
		}

		double tempValue = (Clamp(resultX, -1.0, 1.0) * 0.5 + 0.5) * rangeMax;
		resultX = floor(tempValue + 0.5);

		tempValue = (Clamp(resultY, -1.0, 1.0) * 0.5 + 0.5) * rangeMax;
		resultY = floor(tempValue + 0.5);

		pEncodeNormal[i * 2] = (short)(resultX - 32768.0);
		pEncodeNormal[i * 2 + 1] = (short)(resultY - 32768.0);
	}
	return TRUE;
}

bool S3MBTools::SaveCompressTextureCoords(VertexDataPackage* pVertexDataPack, MemoryStream &stream)
{
	unsigned short nTextureCoord = 0;
	byte nByte = 0;
	for (int i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
	{
		if (pVertexDataPack->m_TexCoordCount[i] == 0)
		{
			break;
		}
		if (IsTexutureCoord(pVertexDataPack->m_nTexDimensions[i]) || IsTexutureCoordStoreW(pVertexDataPack, i))
		{
			nTextureCoord++;
		}
	}

	stream << nTextureCoord;
	stream << nByte;
	stream << nByte;
	for (int i = 0; i < nTextureCoord; i++)
	{
		if (pVertexDataPack->m_TexCoordCount[i] == 0)
		{
			break;
		}
		if (IsTexutureCoord(pVertexDataPack->m_nTexDimensions[i]) || IsTexutureCoordStoreW(pVertexDataPack, i))
		{
			SaveOneCompressTextureCoords(pVertexDataPack, stream, i);
		}
	}
	return TRUE;
}

bool S3MBTools::SaveOneCompressTextureCoords(VertexDataPackage* pVertexDataPack, MemoryStream &stream, uInt nTexIdx)
{
	byte nByte = 0;
	bool bNeedTexCoordZ = (pVertexDataPack->m_nVertexOptions & VO_TEXTURE_COORD_Z_IS_MATRIX) > 0;
	unsigned short nSrcDimension = pVertexDataPack->m_nTexDimensions[nTexIdx];
	uInt nTexCoordCount = pVertexDataPack->m_TexCoordCount[nTexIdx];
	bNeedTexCoordZ = bNeedTexCoordZ && (nSrcDimension == 3);
	unsigned short nDstDimension = nSrcDimension;
	if (nDstDimension > 2)
	{
		if (bNeedTexCoordZ)
		{
			nDstDimension = 2;
		}
		else
		{
			nDstDimension = 4;
		}
	}
	stream << bNeedTexCoordZ;
	stream << nByte;
	stream << nByte;
	stream << nByte;

	stream << nTexCoordCount;

	float* pMinValue = new float[4];
	float fCompressConstant;

	// 模型缓存的Z值存储的是矩阵因此Z值不能压缩，需要单独存储
	unsigned short compressDimension = nDstDimension;
	stream << compressDimension;
	stream << pVertexDataPack->m_TexCoordStride[nTexIdx];

	short* pEncodeValue = new(std::nothrow) short[nTexCoordCount * compressDimension];
	RangeEncode(pVertexDataPack->m_pTexCoords[nTexIdx], nTexCoordCount, nSrcDimension, compressDimension, 14,
		pEncodeValue, fCompressConstant, pMinValue);

	stream << fCompressConstant;
	stream.Save(pMinValue, 4);
	stream.Save(pEncodeValue, nTexCoordCount * compressDimension);
	delete[] pMinValue;
	delete[] pEncodeValue;

	if (bNeedTexCoordZ)
	{
		float* pTexCoordZ = new(std::nothrow) float[nTexCoordCount];
		for (uInt i = 0; i < nTexCoordCount; i++)
		{
			pTexCoordZ[i] = pVertexDataPack->m_pTexCoords[nTexIdx][i * nSrcDimension + 2];
		}
		stream.Save(pTexCoordZ, nTexCoordCount);
		delete[] pTexCoordZ;
	}
	return TRUE;
}

void S3MBTools::SaveMaterial(std::map<string, Material*>& mapMaterial, JsonValue& jsonMaterials)
{
	JsonValue jsonArray(arrayValue);
	std::map<string, Material*>::iterator it;
	for (it = mapMaterial.begin(); it != mapMaterial.end(); it++)
	{
		JsonValue jsonMaterial(objectValue);
		S3MBTools::ToJson(it->second, jsonMaterial);

		jsonArray.Add(jsonMaterial);
	}

	jsonMaterials.Add(S3MB_MATERIAL, jsonArray);
}

void S3MBTools::ToJson(Material* pMaterial, JsonValue& jsonMaterial)
{
	if (pMaterial == NULL)
	{
		assert(FALSE);
		return;
	}
	JsonValue jsonContent(objectValue);
	Technique* pTechnique = pMaterial->getTechnique(0);
	S3MBTools::ToJson(pMaterial->m_strName, pTechnique, jsonContent);

	jsonMaterial.Add(S3MB_MATERIAL, jsonContent);
}

void S3MBTools::ToJson(const string strMaterialName, Technique* pTechnique, JsonValue& jsonContent)
{
	jsonContent.Add(S3MB_ID, strMaterialName);

	Pass* pPass = pTechnique->getPass(0);
	jsonContent.Add(S3MB_MATERIAL_AMBIENT, pPass->m_Ambient);
	jsonContent.Add(S3MB_MATERIAL_DIFFUSE, pPass->m_Diffuse);
	jsonContent.Add(S3MB_MATERIAL_SPECULAR, pPass->m_Specular);
	jsonContent.Add(S3MB_MATERIAL_SHINESS, pPass->m_Shininess);

	jsonContent.Add(S3MB_MATPASS_CULLMODE, S3MBTools::CullModeToString(pPass->m_CullMode));
	jsonContent.Add(S3MB_MATERIAL_TRANSPARENTSORT, pPass->m_TransparentSorting);

	//PBR 预留~
	if (pPass->m_pPRBMaterial != NULL)
	{
		//tag
		MaterialParamType emType = pPass->m_pPRBMaterial->GetType();
		string strType = S3MBTools::MaterialParamTypeToString(emType);
		//value
		JsonValue jsonPBR(objectValue);
		ToJson(pPass->m_pPRBMaterial, jsonPBR);

		jsonContent.Add(strType, jsonPBR);
	}

	//TextureUnitStates
	JsonValue jsonArrayTextUnit(arrayValue);
	int nTextureSize = pPass->GetTextureUnitStatesSize();
	for (int i = 0; i < nTextureSize; i++)
	{
		TextureUnitState* pTexUnit = pPass->GetTextureUnitState(i);
		JsonValue jsonTextUint(objectValue);
		ToJson(pTexUnit, jsonTextUint);

		jsonArrayTextUnit.Add(jsonTextUint);
	}
	jsonContent.Add(S3MB_MATERIAL_TEXTUNITS, jsonArrayTextUnit);
}

void S3MBTools::ToJson(PBRParams* pPBR, JsonValue& jsonPBR)
{
	if (pPBR == NULL)
	{
		return;
	}

	jsonPBR.Add(S3MB_MATERIAL_PBR_EMISSIVEFACTOR, pPBR->m_vec3EmissiveFactor);
	jsonPBR.Add(S3MB_MATERIAL_PBR_EMISSIVETEXTURE, pPBR->m_strEmissiveTexture);
	jsonPBR.Add(S3MB_MATERIAL_PBR_NORMALTEXTURE, pPBR->m_strNormalTexture);
	jsonPBR.Add(S3MB_MATERIAL_PBR_OCCLUSIONTEXTURE, pPBR->m_strOcclusionTexture);
	jsonPBR.Add(S3MB_MATERIAL_PBR_BASECOLOR, pPBR->m_vec4BaseColor);
	jsonPBR.Add(S3MB_MATERIAL_PBR_BASECOLORTEXTURE, pPBR->m_strBaseColorTexture);
	jsonPBR.Add(S3MB_MATERIAL_PBR_ALPHAMODE, S3MBTools::AlphaModeToString(pPBR->m_AlphaMode));
	jsonPBR.Add(S3MB_MATERIAL_PBR_ALPHACUTOFF, pPBR->m_fAlphaCutoff);

	MaterialParamType emType = pPBR->GetType();
	if (emType == MAT_PBRMetallicRough)
	{
		PBRMetallicRough* pPBRMR = (PBRMetallicRough*)pPBR;
		jsonPBR.Add(S3MB_MATERIAL_PBRM_ROUGHNESSTEXTURE, pPBRMR->m_strMetallicRoughnessTexture);
		jsonPBR.Add(S3MB_MATERIAL_PBRM_METALLICFACTOR, pPBRMR->m_fMetallicFactor);
		jsonPBR.Add(S3MB_MATERIAL_PBRM_ROUGHNESSFACTOR, pPBRMR->m_fRoughnessFactor);
	}
	else if (emType == MAT_PBRSpecularGlossy)
	{
		PBRSpecularGlossy* pPBRSG = (PBRSpecularGlossy*)pPBR;
		jsonPBR.Add(S3MB_MATERIAL_PBRS_DIFFUSEFACTOR, pPBRSG->m_vec4DiffuseFactor);
		jsonPBR.Add(S3MB_MATERIAL_PBRS_SPECULARFACTOR, pPBRSG->m_vec3SpecularFactor);
		jsonPBR.Add(S3MB_MATERIAL_PBRS_GLOSSINESSFACTOR, pPBRSG->m_fGlossinessFactor);
		jsonPBR.Add(S3MB_MATERIAL_PBRS_DIFFUSETEXTURE, pPBRSG->m_strDiffuseTexture);
		jsonPBR.Add(S3MB_MATERIAL_PBRS_SPECULARGLOSSINESSTEXTURE, pPBRSG->m_strSpecularGlossinessTexture);
	}
}

void S3MBTools::ToJson(TextureUnitState* pTextureUnitState, JsonValue& jsonTexture)
{
	JsonValue jsonContent(objectValue);
	jsonContent.Add(S3MB_ID, pTextureUnitState->m_strTextureName);
	jsonContent.Add(S3MB_TEXTURE_URL, pTextureUnitState->m_strTexureURL);

	//AddressMode
	JsonValue jsonAddMode(objectValue);
	AddressModeToJson(pTextureUnitState, jsonAddMode);
	jsonContent.Add(S3MB_MATERIAL_TEXTUNIT_ADDMODE, jsonAddMode);
	//TextureFilteringOption
	int nOptionVal = pTextureUnitState->m_TextureFilteringOption;
	jsonContent.Add(S3MB_MATERIAL_TEXTUNIT_FILTEROPTION, nOptionVal);
	nOptionVal = pTextureUnitState->m_MinFilter;
	jsonContent.Add(S3MB_MATERIAL_TEXTUNIT_FILTERMIN, nOptionVal);
	nOptionVal = pTextureUnitState->m_MaxFilter;
	jsonContent.Add(S3MB_MATERIAL_TEXTUNIT_FILTERMAX, nOptionVal);

	//TexModMatrix
	jsonContent.Add(S3MB_MATERIAL_TEXTUNIT_TEXMODMATRIX, pTextureUnitState->m_TexModMatrix);

	jsonTexture.Add(S3MB_MATERIAL_TEXTUNIT, jsonContent);
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

void S3MBTools::SaveTextures(std::map<string, TextureDataInfo*>& mapTexture, MemoryStream &streamTexture, unsigned int nCompressType, bool bTextureTransform)
{
	streamTexture.Init();
	int nImageCount = mapTexture.size();
	streamTexture << nImageCount;

	std::map<string, TextureDataInfo*>::iterator it;
	for (it = mapTexture.begin(); it != mapTexture.end(); it++)
	{
		string strName = it->first;
		TextureDataInfo* pTexture = it->second;
		if (pTexture->m_bMipmap && pTexture->m_pTextureData != NULL)
		{
			pTexture->m_nLevel = ImageOperator::GetMipMapLevel(pTexture->m_pTextureData->m_nWidth, pTexture->m_pTextureData->m_nHeight);
		}
		streamTexture << strName;
		S3MBTools::StreamAlign(streamTexture, false);

		// 		streamTexture << pTexture->m_bMipmap;
		streamTexture << pTexture->m_nLevel;

		S3MBTools::SaveTextureData(pTexture->m_pTextureData, streamTexture, nCompressType, bTextureTransform);
	}
}

void S3MBTools::SaveTextureData(TextureData* pTextureData, MemoryStream &streamTexture, uInt nCompressType, bool bIsChangeTexture /*= TRUE*/)
{
	if (bIsChangeTexture)
	{
		ProcessTextureData(pTextureData, nCompressType);
	}

	streamTexture << pTextureData->m_nWidth;
	streamTexture << pTextureData->m_nHeight;
	streamTexture << pTextureData->m_CompressType;
	streamTexture << pTextureData->m_nSize;
	streamTexture << pTextureData->m_enFormat;
	streamTexture.Save(pTextureData->m_pBuffer, pTextureData->m_nSize);
}

std::string S3MBTools::AlphaModeToString(PBRParams::AlphaMode emType)
{
	string strValue;
	if (emType == PBRParams::PBRAM_OPAQUE)
	{
		strValue = S3MB_MATERIAL_PBR_ALPHAMODE_OPAQUE;
	}
	else 	if (emType == PBRParams::PBRAM_MASK)
	{
		strValue = S3MB_MATERIAL_PBR_ALPHAMODE_MASK;
	}
	else 	if (emType == PBRParams::PBRAM_BLEND)
	{
		strValue = S3MB_MATERIAL_PBR_ALPHAMODE_BLEND;
	}
	return strValue;
}

std::string S3MBTools::MaterialParamTypeToString(MaterialParamType emType)
{
	string strType;
	if (emType == MAT_PBRMetallicRough)
	{
		strType = S3MB_MATERIAL_PBRTYPE_MR;
	}
	else if (emType == MAT_PBRSpecularGlossy)
	{
		strType = S3MB_MATERIAL_PBRTYPE_SG;
	}
	return strType;
}

std::string S3MBTools::CullModeToString(CullingMode emType)
{
	string strValue;
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

