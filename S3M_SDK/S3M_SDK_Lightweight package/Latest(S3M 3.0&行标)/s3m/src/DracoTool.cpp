#include "DracoTool.h"
#include "draco/compression/decode.h"
#include "draco/compression/encode.h"
#include "draco/io/obj_encoder.h"
#include "Palette.h"
#include <algorithm>
#include "S3MBCommon.h"

static const char* c_key_scale_x = "i3s-scale_x";
static const char* c_key_scale_y = "i3s-scale_y";
static const char* c_key_attribute_type = "i3s-attribute-type";
static const char* c_key_feature_ids = "i3s-feature-ids";
static const char* c_value_attribute_uv_region = "uv-region";
static const char* c_value_attribute_feature_index = "feature-index";

// 添加draco的顶点属性
template< class T >
void AddAttributeData(draco::PointCloud* & pPointCloud, std::vector<T> &attributeData, draco::GeometryAttribute::Type attType, draco::DataType dataType, int nDimension, int& att_id)
{
	const int componentCount = nDimension;
	const int vertexCount = attributeData.size() / componentCount;

	uint32_t unique_id = 0;
	draco::PointAttribute att;
	att.Init(attType, componentCount, dataType, false, vertexCount);
	att_id = pPointCloud->AddAttribute(att, true, vertexCount);
	draco::PointAttribute *att_ptr = pPointCloud->attribute(att_id);
	for (draco::PointIndex i(0); i < vertexCount; ++i) {
		std::vector<T> vertex_data(componentCount);
		memcpy(&vertex_data[0], &attributeData[i.value() * componentCount], sizeof(T) * componentCount);
		att_ptr->SetAttributeValue(att_ptr->mapped_index(i), &vertex_data[0]);
	}
}

// 对mesh或者点云进行压缩，存储到buffer中
bool CompressDracoMesh(int nPosQuantizationBits,
	int nTexcoordsQuantizationBits,
	int nNormalsQuantizationBits,
	int nGenericQuantizationBits,
	int nEncodeSpeed,
	int nDecodeSpeed,
	draco::PointCloud* & pPointCloud, draco::Mesh* & pDracoMesh, draco::EncoderBuffer & buffer)
{
	draco::Encoder encoder;
	encoder.SetSpeedOptions(nEncodeSpeed, nDecodeSpeed);

	encoder.SetAttributeQuantization(draco::GeometryAttribute::POSITION, nPosQuantizationBits);
	encoder.SetAttributeQuantization(draco::GeometryAttribute::TEX_COORD, nTexcoordsQuantizationBits);
	encoder.SetAttributeQuantization(draco::GeometryAttribute::NORMAL, nNormalsQuantizationBits);
	encoder.SetAttributeQuantization(draco::GeometryAttribute::GENERIC, nGenericQuantizationBits);

	draco::Status status;
	if (pDracoMesh != 0)
	{
		status = encoder.EncodeMeshToBuffer(*pDracoMesh, &buffer);
	}
	else
	{
		status = encoder.EncodePointCloudToBuffer(*pPointCloud, &buffer);
	}

	if (!status.ok()) {
		std::cerr << "Error: Encode mesh.\n";
		return false;
	}

	return true;

}

S3MB::DracoCompressParam::DracoCompressParam()
{
	m_bEnable = false;
	m_nPosQuantizationBits = 11;
	m_nTexcoordsQuantizationBits = 10;
	m_nNormalsQuantizationBits = 7;
	m_nGenericQuantizationBits = 12;
	m_nEncodeSpeed = 0;
	m_nDecodeSpeed = 0;
}

S3MB::DracoCompressParam::DracoCompressParam(const DracoCompressParam& other) :
	m_bEnable(other.m_bEnable),
	m_nPosQuantizationBits(other.m_nPosQuantizationBits),
	m_nTexcoordsQuantizationBits(other.m_nTexcoordsQuantizationBits),
	m_nNormalsQuantizationBits(other.m_nNormalsQuantizationBits),
	m_nGenericQuantizationBits(other.m_nGenericQuantizationBits),
	m_nEncodeSpeed(other.m_nEncodeSpeed),
	m_nDecodeSpeed(other.m_nDecodeSpeed){}

void S3MB::DracoCompressParam::SetPosQuantizationBits(unsigned int bits)
{
	m_nPosQuantizationBits = Clamp<unsigned int>(bits, 1, 30);
}

unsigned int S3MB::DracoCompressParam::GetPosQuantizationBits() const
{
	return m_nPosQuantizationBits;
}

void S3MB::DracoCompressParam::SetTexcoordsQuantizationBits(unsigned int bits)
{
	m_nTexcoordsQuantizationBits = Clamp<unsigned int>(bits, 1, 30);
}

unsigned int S3MB::DracoCompressParam::GetTexcoordsQuantizationBits() const
{
	return m_nTexcoordsQuantizationBits;
}

void S3MB::DracoCompressParam::SetNormalsQuantizationBits(unsigned int bits)
{
	m_nNormalsQuantizationBits = Clamp<unsigned int>(bits, 1, 30);
}

unsigned int S3MB::DracoCompressParam::GetNormalsQuantizationBits() const
{
	return m_nNormalsQuantizationBits;
}

void S3MB::DracoCompressParam::SetGenericQuantizationBits(unsigned int bits)
{
	m_nGenericQuantizationBits = Clamp<unsigned int>(bits, 1, 30);
}

unsigned int S3MB::DracoCompressParam::GetGenericQuantizationBits() const
{
	return m_nGenericQuantizationBits;
}

void S3MB::DracoCompressParam::SetEncodeSpeed(unsigned int nSpeedLevel)
{
	m_nEncodeSpeed = Clamp<unsigned int>(nSpeedLevel, 0, 10);
}

unsigned int S3MB::DracoCompressParam::GetEncodeSpeed() const
{
	return m_nEncodeSpeed;
}

void S3MB::DracoCompressParam::SetDecodeSpeed(unsigned int nSpeedLevel)
{
	m_nDecodeSpeed = Clamp<unsigned int>(nSpeedLevel, 0, 10);
}

unsigned int S3MB::DracoCompressParam::GetDecodeSpeed() const
{
	return m_nDecodeSpeed;
}

void S3MB::DracoCompressParam::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
}

bool S3MB::DracoCompressParam::IsEnable() const
{
	return m_bEnable;
}

S3MB::DracoAttributeInfo::DracoAttributeInfo()
{
	m_nUniqueID = -1;
	m_enAttributeType = DRACO_GENERIC;
	m_bNeedCompress = false;
}

S3MB::DracoAttributeInfo::DracoAttributeInfo(int nUniqueId, DracoAttributeType enAttributeType, bool bNeedCompress)
{
	m_nUniqueID = nUniqueId;
	m_enAttributeType = enAttributeType;
	m_bNeedCompress = bNeedCompress;
}

S3MB::DracoCompressedInfo::DracoCompressedInfo()
{
	m_posInfo = DracoAttributeInfo(-1, DracoAttributeType::DRACO_POSITION, true);
	m_normalInfo = DracoAttributeInfo(-1, DracoAttributeType::DRACO_NORMAL, false);
	m_colorInfo = DracoAttributeInfo(-1, DracoAttributeType::DRACO_COLOR, false);
	for (int i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
	{
        m_texCoordInfo[i] = DracoAttributeInfo(-1, DracoAttributeType::DRACO_TEX_COORD, false);
	}
    m_pVertexAttInfo = NULL;
    m_nVertexAttInfoCount = 0;
}


bool S3MB::DracoTool::DracoCompress(const DracoCompressParam& param, DracoCompressedInfo& convertParam, VertexDataPackage* pVertexDataPackage, std::vector<IndexPackage*> arrIndexPackage, char*& pOutputData, unsigned int& sizeInBytes)
{
	DracoCompressParam realDracoCompressParam(param);
	if (pVertexDataPackage->m_pVertices != NULL)
	{
		convertParam.m_posInfo.SetEnable(true);
		convertParam.m_posInfo.SetDracoAttributeType(DRACO_POSITION);
	}

	if (pVertexDataPackage->m_pVertexColor != NULL)
	{
		convertParam.m_colorInfo.SetEnable(true);
		convertParam.m_colorInfo.SetDracoAttributeType(DRACO_COLOR);
	}

	if (pVertexDataPackage->m_pNormals != NULL)
	{
		unsigned int nNormalCount = pVertexDataPackage->m_nNormalCount;
		unsigned int nNormalDimension = pVertexDataPackage->m_nNormalDimension;
		if (nNormalCount > 0 && nNormalDimension == 3)
		{
			// draco压缩不能处理法线值非法的情况，或者未归一化的情况
			bool bNoramlValid = true;
			Vector3d vNormal;
			for (unsigned int i = 0; i < nNormalCount; i++)
			{
				vNormal.x = pVertexDataPackage->m_pNormals[i * 3];
				vNormal.y = pVertexDataPackage->m_pNormals[i * 3 + 1];
				vNormal.z = pVertexDataPackage->m_pNormals[i * 3 + 2];
				if (vNormal.x == 0.0 && vNormal.y == 0.0 && vNormal.z == 0.0)
				{
					bNoramlValid = false;
					break;
				}
				vNormal.Normalize();
				pVertexDataPackage->m_pNormals[i * 3] = vNormal.x;
				pVertexDataPackage->m_pNormals[i * 3 + 1] = vNormal.y;
				pVertexDataPackage->m_pNormals[i * 3 + 2] = vNormal.z;
			}
			if (bNoramlValid)
			{
				convertParam.m_normalInfo.SetEnable(true);
				convertParam.m_normalInfo.SetDracoAttributeType(DRACO_NORMAL);
			}
			else
			{
				convertParam.m_normalInfo.SetEnable(false);
			}
		}
		else
		{
			convertParam.m_normalInfo.SetEnable(false);
		}
	}

	for (int i = 0; i < SMSCN_MAX_TEXTURE_COORD_SETS; i++)
	{
		if (pVertexDataPackage->m_pTexCoords[i] == NULL)
		{
			continue;
		}
		unsigned int nTexDim = pVertexDataPackage->m_nTexDimensions[i];
		convertParam.m_texCoordInfo[i].SetEnable(true);
		if (nTexDim == 2)
		{
			convertParam.m_texCoordInfo[i].SetDracoAttributeType(DRACO_TEX_COORD);
		}
		else
		{
			// 精细模型生成缓存时我们会把纹理矩阵存储在纹理坐标的Z值上，占用21位
			realDracoCompressParam.SetGenericQuantizationBits(21);
			// 纹理坐标的范围超出0到1，作为纹理坐标语义的draco压缩会出错，改为通用属性语义
			convertParam.m_texCoordInfo[i].SetDracoAttributeType(DRACO_GENERIC);
		}
	}

	// 自定义属性
	void** pVertexAttData = NULL;
	unsigned short* pVertexAttDataType = NULL;
	unsigned int* pVertexAttCount = NULL;
	unsigned short* pVertexAttDim = NULL;
	unsigned int nVertexAttCount = pVertexDataPackage->m_nVertexAttCount;
	int* pVertexAttUniqueID = NULL;
	if (nVertexAttCount > 0)
	{
		realDracoCompressParam.SetGenericQuantizationBits(21);
		convertParam.m_nVertexAttInfoCount = pVertexDataPackage->m_nVertexAttCount;
		convertParam.m_pVertexAttInfo = new DracoAttributeInfo[pVertexDataPackage->m_nVertexAttCount];
		pVertexAttData = new void*[pVertexDataPackage->m_nVertexAttCount];
		pVertexAttDataType = new unsigned short[pVertexDataPackage->m_nVertexAttCount];
		pVertexAttCount = new unsigned int[pVertexDataPackage->m_nVertexAttCount];
		pVertexAttDim = new unsigned short[pVertexDataPackage->m_nVertexAttCount];
		pVertexAttUniqueID = new int[pVertexDataPackage->m_nVertexAttCount];
		for (int i = 0; i < pVertexDataPackage->m_nVertexAttCount; i++)
		{
			pVertexAttData[i] = pVertexDataPackage->m_vecVertexAttData[i];
			pVertexAttDataType[i] = (unsigned short)pVertexDataPackage->m_vecVertexAttDataType[i];
			pVertexAttCount[i] = pVertexDataPackage->m_vecVertexAttDataCount[i];
			pVertexAttDim[i] = pVertexDataPackage->m_vecVertexAttDataDimension[i];
		}
	}


	int nPosUniqueID = -1;
	int nNormalUniqueID = -1;
	int nVertexColorUniqueID = -1;
	int nSecondColorUniqueID = -1;
	int nTexCoordUniqueID[8];
	for (int i = 0; i < 8; i++)
	{
		nTexCoordUniqueID[i] = -1;
	}
	int* pTexCoordUniqueID = nTexCoordUniqueID;

	if (arrIndexPackage.size() > 0)
	{
		IndexPackage* pIndexPackage = arrIndexPackage[0];
		DracoCompressImpl(realDracoCompressParam.GetPosQuantizationBits(),
			realDracoCompressParam.GetTexcoordsQuantizationBits(),
			realDracoCompressParam.GetNormalsQuantizationBits(),
			realDracoCompressParam.GetGenericQuantizationBits(),
			realDracoCompressParam.GetEncodeSpeed(),
			realDracoCompressParam.GetDecodeSpeed(),
			convertParam.m_texCoordInfo->GetDracoAttributeType(),
			pVertexDataPackage->m_pVertices, pVertexDataPackage->m_nVerticesCount, pVertexDataPackage->m_nVertexDimension,
			pVertexDataPackage->m_pNormals, pVertexDataPackage->m_nNormalCount, pVertexDataPackage->m_nNormalDimension,
			pVertexDataPackage->m_pVertexColor, pVertexDataPackage->m_nVertexColorCount,
			pVertexDataPackage->m_pTexCoords, pVertexDataPackage->m_TexCoordCount, pVertexDataPackage->m_nTexDimensions,
			pVertexAttData, pVertexAttDataType, pVertexAttCount, pVertexAttDim, nVertexAttCount,
			pIndexPackage->m_pIndexes, pIndexPackage->m_nIndexesCount, pIndexPackage->m_enIndexType == IT_32BIT,
			pOutputData, sizeInBytes,
			nPosUniqueID, nNormalUniqueID, nVertexColorUniqueID, pTexCoordUniqueID, pVertexAttUniqueID);

	}
	else
	{
		DracoCompressImpl(realDracoCompressParam.GetPosQuantizationBits(),
			realDracoCompressParam.GetTexcoordsQuantizationBits(),
			realDracoCompressParam.GetNormalsQuantizationBits(),
			realDracoCompressParam.GetGenericQuantizationBits(),
			realDracoCompressParam.GetEncodeSpeed(),
			realDracoCompressParam.GetDecodeSpeed(),
			convertParam.m_texCoordInfo->GetDracoAttributeType(),
			pVertexDataPackage->m_pVertices, pVertexDataPackage->m_nVerticesCount, pVertexDataPackage->m_nVertexDimension,
			pVertexDataPackage->m_pNormals, pVertexDataPackage->m_nNormalCount, pVertexDataPackage->m_nNormalDimension,
			pVertexDataPackage->m_pVertexColor, pVertexDataPackage->m_nVertexColorCount,
			pVertexDataPackage->m_pTexCoords, pVertexDataPackage->m_TexCoordCount, pVertexDataPackage->m_nTexDimensions,
			pVertexAttData, pVertexAttDataType, pVertexAttCount, pVertexAttDim, nVertexAttCount,
			0, 0, false,
			pOutputData, sizeInBytes,
			nPosUniqueID, nNormalUniqueID, nVertexColorUniqueID, pTexCoordUniqueID, pVertexAttUniqueID);
	}
	convertParam.m_posInfo.SetUniqueID(nPosUniqueID);
	convertParam.m_normalInfo.SetUniqueID(nNormalUniqueID);
	convertParam.m_colorInfo.SetUniqueID(nVertexColorUniqueID);
	for (int i = 0; i < 8; i++)
	{
		convertParam.m_texCoordInfo[i].SetUniqueID(pTexCoordUniqueID[i]);
	}


	for (int i = 0; i < pVertexDataPackage->m_nVertexAttCount; i++)
	{
		convertParam.m_pVertexAttInfo[i].SetUniqueID(pVertexAttUniqueID[i]);
	}

	if (pVertexAttData != NULL)
	{
		for (int i = 0; i < pVertexDataPackage->m_nVertexAttCount; i++)
		{
			pVertexAttData[i] = NULL;
		}

		delete pVertexAttDataType;
		pVertexAttDataType = NULL;

		delete[] pVertexAttData;
		pVertexAttData = NULL;

		delete[] pVertexAttCount;
		pVertexAttCount = NULL;

		delete[] pVertexAttDim;
		pVertexAttDim = NULL;

		delete[] pVertexAttUniqueID;
		pVertexAttUniqueID = NULL;
	}

	return true;
}

bool S3MB::DracoTool::DracoDecompress(const DracoCompressedInfo& convertParam, const char *pInputData, unsigned int sizeInBytes, bool isPointCloud, unsigned int nVertexAttCount, VertexDataPackage*& pVertexDataPackage, std::vector<IndexPackage*>& arrIndexPackage)
{
	int pTexCoordUniqueID[8];
	for (int i = 0; i < 8; i++)
	{
		pTexCoordUniqueID[i] = convertParam.m_texCoordInfo[i].GetUniqueID();
	}


	int* pVertexAttUniqueID = NULL;
	void** pVertexAttData = NULL;
	unsigned short* pVertexAttDataType = NULL;
	unsigned int* pVertexAttCount = NULL;
	unsigned short* pVertexAttDim = NULL;
	if (nVertexAttCount > 0)
	{
		pVertexAttUniqueID = new int[nVertexAttCount];
		if (convertParam.m_pVertexAttInfo != NULL)
		{
			for (unsigned int i = 0; i < nVertexAttCount; i++)
			{
				pVertexAttUniqueID[i] = convertParam.m_pVertexAttInfo[i].GetUniqueID();
			}
		}

		pVertexAttData = new void*[nVertexAttCount];
		pVertexAttDataType = new unsigned short[nVertexAttCount];
		pVertexAttCount = new unsigned int[nVertexAttCount];
		pVertexAttDim = new unsigned short[nVertexAttCount];
	}

	if (pVertexDataPackage != NULL)
	{
		delete pVertexDataPackage;
		pVertexDataPackage = NULL;
	}

	pVertexDataPackage = new VertexDataPackage;
	float** pTexCoords = pVertexDataPackage->m_pTexCoords;
	unsigned int* pTexCoordCount = pVertexDataPackage->m_TexCoordCount;
	unsigned short* pTexCoordDim = pVertexDataPackage->m_nTexDimensions;

	IndexPackage* pIndexPackage = new IndexPackage;
	bool bUseUint = false;
	DracoDecompressImpl(pInputData, sizeInBytes, isPointCloud ? true : false,
		convertParam.m_posInfo.GetUniqueID(), convertParam.m_normalInfo.GetUniqueID(),
		convertParam.m_colorInfo.GetUniqueID(), pTexCoordUniqueID,
		pVertexAttUniqueID, nVertexAttCount,
		pVertexDataPackage->m_pVertices, pVertexDataPackage->m_nVerticesCount, pVertexDataPackage->m_nVertexDimension,
		pVertexDataPackage->m_pNormals, pVertexDataPackage->m_nNormalCount, pVertexDataPackage->m_nNormalDimension,
		pVertexDataPackage->m_pVertexColor, pVertexDataPackage->m_nVertexColorCount,
		pTexCoords, pTexCoordCount, pTexCoordDim,
		pVertexAttData, pVertexAttDataType, pVertexAttCount, pVertexAttDim,
		pIndexPackage->m_pIndexes, pIndexPackage->m_nIndexesCount, bUseUint);

	if (nVertexAttCount > 0)
	{
		pVertexDataPackage->m_nVertexAttCount = nVertexAttCount;
		for (unsigned int i = 0; i < nVertexAttCount; i++)
		{
			pVertexDataPackage->m_vecVertexAttDataCount.push_back(pVertexAttCount[i]);
			pVertexDataPackage->m_vecVertexAttDataDimension.push_back(pVertexAttDim[i]);
			pVertexDataPackage->m_vecVertexAttDataType.push_back((VertexAttributeType)pVertexAttDataType[i]);
			pVertexDataPackage->m_vecVertexAttData.push_back(pVertexAttData[i]);
			pVertexAttData[i] = NULL;
		}
		wstring strKey = U("VertexWeight");
		pVertexDataPackage->m_mapVertexAttributeDescript[strKey] = 0;

		delete[] pVertexAttDataType;
		pVertexAttDataType = NULL;

		delete[] pVertexAttData;
		pVertexAttData = NULL;

		delete[] pVertexAttCount;
		pVertexAttCount = NULL;

		delete[] pVertexAttDim;
		pVertexAttDim = NULL;

		delete[] pVertexAttUniqueID;
		pVertexAttUniqueID = NULL;
	}

	if (bUseUint)
	{
		pIndexPackage->m_enIndexType = IT_32BIT;
	}


	if (isPointCloud)
	{
		delete pIndexPackage;
		pIndexPackage = NULL;
	}
	else
	{
		arrIndexPackage.push_back(pIndexPackage);
	}
	return true;
}

bool S3MB::DracoTool::DracoDecompress(const std::vector<std::string>& vecVertexAttDescript, const char *pInputData, unsigned int sizeInBytes, bool isPointCloud, VertexDataPackage*& pVertexDataPackage, std::vector<IndexPackage*>& arrIndexPackage, unsigned int*& pFeatureIds, unsigned int& nFeatureIdCount)
{
	void** pVertexAttData = NULL;
	unsigned short* pVertexAttDataType = NULL;
	unsigned int* pVertexAttCount = NULL;
	unsigned short* pVertexAttDim = NULL;
	unsigned int nVertexAttCount = vecVertexAttDescript.size();
	if (nVertexAttCount > 0)
	{
		pVertexAttData = new void*[nVertexAttCount];
		pVertexAttDataType = new unsigned short[nVertexAttCount];
		pVertexAttCount = new unsigned int[nVertexAttCount];
		pVertexAttDim = new unsigned short[nVertexAttCount];
	}

	if (pVertexDataPackage != NULL)
	{
		delete pVertexDataPackage;
		pVertexDataPackage = NULL;
	}
	pVertexDataPackage = new VertexDataPackage;
	float** pTexCoords = pVertexDataPackage->m_pTexCoords;
	unsigned int* pTexCoordCount = pVertexDataPackage->m_TexCoordCount;
	unsigned short* pTexCoordDim = pVertexDataPackage->m_nTexDimensions;

	IndexPackage* pIndexPackage = new IndexPackage;
	bool bUseUint = false;
	DracoDecompressImpl_descript(pInputData, sizeInBytes, isPointCloud ? true : false,
		vecVertexAttDescript,
		pVertexDataPackage->m_pVertices, pVertexDataPackage->m_nVerticesCount, pVertexDataPackage->m_nVertexDimension,
		pVertexDataPackage->m_pNormals, pVertexDataPackage->m_nNormalCount, pVertexDataPackage->m_nNormalDimension,
		pVertexDataPackage->m_pVertexColor, pVertexDataPackage->m_nVertexColorCount,
		pFeatureIds, nFeatureIdCount,
		pTexCoords, pTexCoordCount, pTexCoordDim,
		pVertexAttData, pVertexAttDataType, pVertexAttCount, pVertexAttDim,
		pIndexPackage->m_pIndexes, pIndexPackage->m_nIndexesCount, bUseUint);

	if (nVertexAttCount > 0)
	{
		pVertexDataPackage->m_nVertexAttCount = nVertexAttCount;
		for (unsigned int i = 0; i < nVertexAttCount; i++)
		{
			pVertexDataPackage->m_vecVertexAttDataCount.push_back(pVertexAttCount[i]);
			pVertexDataPackage->m_vecVertexAttDataDimension.push_back(pVertexAttDim[i]);
			pVertexDataPackage->m_vecVertexAttDataType.push_back((VertexAttributeType)pVertexAttDataType[i]);
			pVertexDataPackage->m_vecVertexAttData.push_back(pVertexAttData[i]);
			pVertexAttData[i] = NULL;
		}

		delete[] pVertexAttData;
		pVertexAttData = NULL;

		delete[] pVertexAttDataType;
		pVertexAttDataType = NULL;

		delete[] pVertexAttCount;
		pVertexAttCount = NULL;

		delete[] pVertexAttDim;
		pVertexAttDim = NULL;
	}

	if (bUseUint)
	{
		pIndexPackage->m_enIndexType = IT_32BIT;
	}

	if (isPointCloud)
	{
		delete pIndexPackage;
		pIndexPackage = NULL;
	}
	else
	{
		arrIndexPackage.push_back(pIndexPackage);
	}
	return true;
}

bool S3MB::DracoTool::DracoDecompressImpl(const char *pInputData, unsigned int sizeInBytes, bool isPointCloud, int nPosUniqueID, int nNormalUniqueID, int nVertexColorUniqueID,
	int pTexCoordUniqueID[8], int* pVertexAttUniqueID, unsigned int nVertexAttCount, float*& pVertices, unsigned int& nVerticesCount, unsigned short& nVertexDim,
	float*& pNormals, unsigned int& nNormalCount, unsigned short& nNormalDim, unsigned int *& pVertexColor, unsigned int& nVertexColorCount, 
	float**& pTexCoords,unsigned int*& pTexCoordCount, unsigned short*& pTexCoordDim, 
	void**& pVertexAttData, unsigned short*& pVertexAttDataType, unsigned int*& pVertexAttCount, unsigned short*& pVertexAttDim,
	unsigned short*& pIndex, unsigned int& nIndexesCount, bool& bUseUint)
{
	draco::Decoder decoder;
	draco::DecoderBuffer out;
	out.Init(pInputData, sizeInBytes);

	draco::Mesh *mesh = 0;
	draco::PointCloud *pointCloud = 0;
	std::unique_ptr<draco::PointCloud> pCloud;
	std::unique_ptr<draco::Mesh> pMesh;
	if (isPointCloud)
	{
		pCloud = decoder.DecodePointCloudFromBuffer(&out).value();
		pointCloud = pCloud.get();
	}
	else
	{
		pMesh = decoder.DecodeMeshFromBuffer(&out).value();
		mesh = pMesh.get();
		pointCloud = mesh;
	}

	if (pointCloud == NULL)
	{
		return false;
	}

	const draco::PointAttribute *posAtt = pointCloud->GetAttributeByUniqueId(nPosUniqueID);
	const draco::PointAttribute *normalAtt = pointCloud->GetAttributeByUniqueId(nNormalUniqueID);
	const draco::PointAttribute *colorAtt = pointCloud->GetAttributeByUniqueId(nVertexColorUniqueID);
	const draco::PointAttribute *texCoord0Att = pointCloud->GetAttributeByUniqueId(pTexCoordUniqueID[0]);
	const draco::PointAttribute *texCoord1Att = pointCloud->GetAttributeByUniqueId(pTexCoordUniqueID[1]);
	const draco::PointAttribute *pVertexAtt = NULL;
	if (pVertexAttUniqueID != NULL)
	{
		pVertexAtt = pointCloud->GetAttributeByUniqueId(pVertexAttUniqueID[0]);
	}

	draco::AttributeValueIndex posAttValueIndex;
	draco::AttributeValueIndex nornamlAttValueIndex;
	draco::AttributeValueIndex texCoord0AttValueIndex;
	draco::AttributeValueIndex texCoord1AttValueIndex;
	draco::AttributeValueIndex colorAttValueIndex;
	draco::AttributeValueIndex vertexAttrAttValueIndex;

	uint32_t nNumPoints = pointCloud->num_points();
	if (posAtt != 0) {
		nVertexDim = posAtt->num_components();
		pVertices = new float[nVertexDim * nNumPoints];
		nVerticesCount = nNumPoints;
	}
	if (normalAtt != 0) {
		nNormalDim = normalAtt->num_components();
		pNormals = new float[nNormalDim * nNumPoints];
		nNormalCount = nNumPoints;
	}

	if (texCoord0Att != 0) {
		pTexCoordDim[0] = texCoord0Att->num_components();
		pTexCoordCount[0] = nNumPoints;
		pTexCoords[0] = new float[pTexCoordDim[0] * nNumPoints];
	}

	if (texCoord1Att != 0) {
		pTexCoordDim[1] = texCoord1Att->num_components();
		pTexCoordCount[1] = nNumPoints;
		pTexCoords[1] = new float[pTexCoordDim[1] * nNumPoints];
	}

	char* pReadColor = 0;
	if (colorAtt != 0) {
		pVertexColor = new uint32_t[nNumPoints];
		nVertexColorCount = nNumPoints;
		pReadColor = new char[colorAtt->num_components()];
	}

	if (pVertexAtt != 0) {
		pVertexAttDim[0] = pVertexAtt->num_components();
		pVertexAttCount[0] = nNumPoints;
		pVertexAttData[0] = new float[pVertexAttDim[0] * nNumPoints];
	}

	for (int i = 0; i < nNumPoints; i++)
	{
		if (posAtt != 0) {
			posAttValueIndex = posAtt->mapped_index(draco::PointIndex(i));
			float* pWriteVertex = pVertices + i * nVertexDim;
			posAtt->GetValue(posAttValueIndex, pWriteVertex);
		}

		if (normalAtt != 0) {
			nornamlAttValueIndex = normalAtt->mapped_index(draco::PointIndex(i));
			float* pWriteVertex = pNormals + i * nNormalDim;
			normalAtt->GetValue(nornamlAttValueIndex, pWriteVertex);
		}
		if (texCoord0Att != 0) {
			texCoord0AttValueIndex = texCoord0Att->mapped_index(draco::PointIndex(i));
			float* pWriteVertex = pTexCoords[0] + i * pTexCoordDim[0];
			texCoord0Att->GetValue(texCoord0AttValueIndex, pWriteVertex);
		}
		if (texCoord1Att != 0) {
			texCoord1AttValueIndex = texCoord1Att->mapped_index(draco::PointIndex(i));
			float* pWriteVertex = pTexCoords[1] + i * pTexCoordDim[1];
			texCoord1Att->GetValue(texCoord1AttValueIndex, pWriteVertex);
		}

		if (colorAtt != 0) {
			colorAttValueIndex = colorAtt->mapped_index(draco::PointIndex(i));
			colorAtt->GetValue(colorAttValueIndex, pReadColor);
			uint32_t vertexColor = Palette::RGBA(pReadColor[0], pReadColor[1], pReadColor[2], pReadColor[3]);
			pVertexColor[i] = vertexColor;
		}
	}

	//自定义属性
	if (pVertexAttUniqueID != nullptr)
	{
		for (int i = 0; i < nVertexAttCount; i++)
		{
			const draco::PointAttribute*pVertexAtt = pointCloud->GetAttributeByUniqueId(pVertexAttUniqueID[i]);
			if (pVertexAtt != 0)
			{
				pVertexAttDim[i] = pVertexAtt->num_components();
				pVertexAttCount[i] = nNumPoints;

				switch (pVertexAtt->data_type())
				{
				case draco::DT_UINT32:
				{
					pVertexAttDataType[i] = 0;//AT_32BIT
					pVertexAttData[i] = new uint32_t[pVertexAttDim[0] * nNumPoints];
					for (int j = 0; j < nNumPoints; j++)
					{
						vertexAttrAttValueIndex = pVertexAtt->mapped_index(draco::PointIndex(j));
						uint32_t* pWriteVertex = (uint32_t*)pVertexAttData[i] + j * pVertexAttDim[0];
						pVertexAtt->GetValue(vertexAttrAttValueIndex, pWriteVertex);
					}
				}
				break;
				case draco::DT_FLOAT32:
				{
					pVertexAttDataType[i] = 1;//AT_FLOAT
					pVertexAttData[i] = new float[pVertexAttDim[0] * nNumPoints];
					for (int j = 0; j < nNumPoints; j++)
					{
						vertexAttrAttValueIndex = pVertexAtt->mapped_index(draco::PointIndex(j));
						float* pWriteVertex = (float*)pVertexAttData[i] + j * pVertexAttDim[0];
						pVertexAtt->GetValue(vertexAttrAttValueIndex, pWriteVertex);
					}
				}
				break;
				case draco::DT_FLOAT64:
				{
					pVertexAttDataType[i] = 2;//AT_DOUBLE
					pVertexAttData[i] = new double[pVertexAttDim[0] * nNumPoints];
					for (int j = 0; j < nNumPoints; j++)
					{
						vertexAttrAttValueIndex = pVertexAtt->mapped_index(draco::PointIndex(j));
						double* pWriteVertex = (double*)pVertexAttData[i] + j * pVertexAttDim[0];
						pVertexAtt->GetValue(vertexAttrAttValueIndex, pWriteVertex);
					}
				}
				break;
				case draco::DT_UINT16:
				{
					pVertexAttDataType[i] = 3;//AT_16BIT
					pVertexAttData[i] = new uint16_t[pVertexAttDim[0] * nNumPoints];
					for (int j = 0; j < nNumPoints; j++)
					{
						vertexAttrAttValueIndex = pVertexAtt->mapped_index(draco::PointIndex(j));
						uint16_t* pWriteVertex = (uint16_t*)pVertexAttData[i] + j * pVertexAttDim[0];
						pVertexAtt->GetValue(vertexAttrAttValueIndex, pWriteVertex);
					}
				}
				break;
				default:
					break;
				}
			}
		}
	}

	if (pReadColor != 0)
	{
		delete[] pReadColor;
		pReadColor = 0;
	}

	if (mesh != 0)
	{
		bUseUint = nNumPoints > 65535;
		uint32_t* pUintIndex = 0;
		uint16_t* pUnshortIndex = 0;

		uint32_t nNumFace = static_cast<uint32_t> (mesh->num_faces()); // 索引数量
		nIndexesCount = nNumFace * 3;
		if (bUseUint)
		{
			pUintIndex = new uint32_t[nIndexesCount];
		}
		else
		{
			pUnshortIndex = new uint16_t[nIndexesCount];
		}
		for (int i = 0; i < nNumFace; i++)
		{
			draco::FaceIndex faceID(i);
			std::array<draco::PointIndex, 3> faces = mesh->face(faceID);
			for (int j = 0; j < 3; j++)
			{
				if (bUseUint)
				{
					pUintIndex[i * 3 + j] = faces[j].value();
				}
				else
				{
					pUnshortIndex[i * 3 + j] = faces[j].value();
				}
			}
		}

		if (bUseUint)
		{
			pIndex = (unsigned short*)pUintIndex;
		}
		else
		{
			pIndex = pUnshortIndex;
		}
	}
	return true;
}

bool S3MB::DracoTool::DracoDecompressImpl_descript(const char *pInputData, unsigned int sizeInBytes, bool isPointCloud,
	const std::vector<std::string>& vecVertexAttDescript,
	float*& pVertices, unsigned int& nVerticesCount, unsigned short& nVertexDim,
	float*& pNormals, unsigned int& nNormalCount, unsigned short& nNormalDim,
	unsigned int *& pVertexColor, unsigned int& nVertexColorCount,
	unsigned int *& pSecondColor, unsigned int& nSecondColorCount,
	float**& pTexCoords, unsigned int*& pTexCoordCount, unsigned short*& pTexCoordDim,
	void**& pVertexAttData, unsigned short*& pVertexAttDataType, unsigned int*& pVertexAttCount, unsigned short*& pVertexAttDim,
	unsigned short*& pIndex, unsigned int& nIndexesCount, bool& bUseUint)
{
	draco::Decoder decoder;
	draco::DecoderBuffer out;
	out.Init(pInputData, sizeInBytes);

	draco::Mesh *mesh = 0;
	draco::PointCloud *pointCloud = 0;
	std::unique_ptr<draco::PointCloud> pCloud;
	std::unique_ptr<draco::Mesh> pMesh;
	if (isPointCloud)
	{
		pCloud = decoder.DecodePointCloudFromBuffer(&out).value();
		pointCloud = pCloud.get();
	}
	else
	{
		pMesh = decoder.DecodeMeshFromBuffer(&out).value();
		mesh = pMesh.get();
		pointCloud = mesh;
	}

	if (pointCloud == nullptr)
	{
		return false;
	}

	const draco::PointAttribute *posAtt = pointCloud->GetNamedAttribute(draco::GeometryAttribute::POSITION);
	const draco::PointAttribute *normalAtt = pointCloud->GetNamedAttribute(draco::GeometryAttribute::NORMAL);
	const draco::PointAttribute *colorAtt = pointCloud->GetNamedAttribute(draco::GeometryAttribute::COLOR);
	const draco::PointAttribute *texCoord0Att = pointCloud->GetNamedAttribute(draco::GeometryAttribute::TEX_COORD);

	draco::AttributeValueIndex posAttValueIndex;
	draco::AttributeValueIndex nornamlAttValueIndex;
	draco::AttributeValueIndex colorAttValueIndex;
	draco::AttributeValueIndex texCoord0AttValueIndex;
	draco::AttributeValueIndex secondColorAttValueIndex;
	draco::AttributeValueIndex vertexAttrAttValueIndex;

	bool isScale = false;
	double dScaleX = 1.0, dScaleY = 1.0;
	uint32_t nNumPoints = pointCloud->num_points();
	if (posAtt != 0) {
		nVertexDim = posAtt->num_components();
		pVertices = new float[nVertexDim * nNumPoints];
		nVerticesCount = nNumPoints;

		const draco::AttributeMetadata *pMetadata = pointCloud->GetAttributeMetadataByAttributeId(posAtt->unique_id());
		if (pMetadata != 0)
		{
			isScale |= pMetadata->GetEntryDouble(c_key_scale_x, &dScaleX);
			isScale |= pMetadata->GetEntryDouble(c_key_scale_y, &dScaleY);
		}
	}
	if (normalAtt != 0) {
		nNormalDim = normalAtt->num_components();
		pNormals = new float[nNormalDim * nNumPoints];
		nNormalCount = nNumPoints;
	}

	if (texCoord0Att != 0) {
		pTexCoordDim[0] = texCoord0Att->num_components();
		pTexCoordCount[0] = nNumPoints;
		pTexCoords[0] = new float[pTexCoordDim[0] * nNumPoints];
	}

	bool hasFeatureId = false;
	std::vector<int32_t> vecFeatureIds;
	const draco::PointAttribute *secondColorAtt = 0;
	const draco::AttributeMetadata* pFeatureIndexMetadata = pointCloud->GetAttributeMetadataByStringEntry(c_key_attribute_type, c_value_attribute_feature_index);
	if (pFeatureIndexMetadata != 0)
	{
		pSecondColor = new uint32_t[nNumPoints];
		nSecondColorCount = nNumPoints;

		secondColorAtt = pointCloud->GetAttributeByUniqueId(pFeatureIndexMetadata->att_unique_id());
		hasFeatureId = pFeatureIndexMetadata->GetEntryIntArray(c_key_feature_ids, &vecFeatureIds);
	}

	unsigned char* pReadColor = 0;
	if (colorAtt != 0) {
		pVertexColor = new uint32_t[nNumPoints];
		nVertexColorCount = nNumPoints;
		pReadColor = new unsigned char[colorAtt->num_components() * sizeof(float)];
	}


	for (int i = 0; i < nNumPoints; i++)
	{
		if (posAtt != 0) {
			posAttValueIndex = posAtt->mapped_index(draco::PointIndex(i));
			float* pWriteVertex = pVertices + i * nVertexDim;
			posAtt->GetValue(posAttValueIndex, pWriteVertex);

			if (nVertexDim >= 2 && isScale)
			{
				pWriteVertex[0] = pWriteVertex[0] * dScaleX;
				pWriteVertex[1] = pWriteVertex[1] * dScaleY;
			}
		}

		if (normalAtt != 0) {
			nornamlAttValueIndex = normalAtt->mapped_index(draco::PointIndex(i));
			float* pWriteVertex = pNormals + i * nNormalDim;
			normalAtt->GetValue(nornamlAttValueIndex, pWriteVertex);
		}
		if (texCoord0Att != 0) {
			texCoord0AttValueIndex = texCoord0Att->mapped_index(draco::PointIndex(i));
			float* pWriteVertex = pTexCoords[0] + i * pTexCoordDim[0];
			texCoord0Att->GetValue(texCoord0AttValueIndex, pWriteVertex);
		}

		if (colorAtt != 0) {
			colorAttValueIndex = colorAtt->mapped_index(draco::PointIndex(i));
			colorAtt->GetValue(colorAttValueIndex, pReadColor);
			uint32_t vertexColor = Palette::RGBA(pReadColor[0], pReadColor[1], pReadColor[2], pReadColor[3]);
			pVertexColor[i] = vertexColor;
		}

		if (secondColorAtt != 0) {
			secondColorAttValueIndex = secondColorAtt->mapped_index(draco::PointIndex(i));
			uint32_t* pWriteVertex = pSecondColor + i;
			if (hasFeatureId)
			{
				uint32_t nIndex = 0;
				secondColorAtt->GetValue(secondColorAttValueIndex, &nIndex);
				if (nIndex < vecFeatureIds.size())
				{
					*pWriteVertex = vecFeatureIds[nIndex];
				}
			}
			else
			{
				secondColorAtt->GetValue(secondColorAttValueIndex, pWriteVertex);
			}
		}

	}

	//自定义属性
	for (int i = 0; i < vecVertexAttDescript.size(); i++)
	{
		const draco::AttributeMetadata* pMetadata = pointCloud->GetAttributeMetadataByStringEntry(c_key_attribute_type, vecVertexAttDescript[i]);
		if (pMetadata == 0)
		{
			assert(false);
			continue;
		}

		const draco::PointAttribute* pVertexAtt = pointCloud->GetAttributeByUniqueId(pMetadata->att_unique_id());
		if (pVertexAtt != 0)
		{
			pVertexAttDim[i] = pVertexAtt->num_components();
			pVertexAttCount[i] = nNumPoints;

			switch (pVertexAtt->data_type())
			{
			case draco::DT_UINT32:
			{
				pVertexAttDataType[i] = 0;//AT_32BIT
				pVertexAttData[i] = new uint32_t[pVertexAttDim[i] * nNumPoints];
				for (int j = 0; j < nNumPoints; j++)
				{
					vertexAttrAttValueIndex = pVertexAtt->mapped_index(draco::PointIndex(j));
					uint32_t* pWriteVertex = (uint32_t*)pVertexAttData[i] + j * pVertexAttDim[i];
					pVertexAtt->GetValue(vertexAttrAttValueIndex, pWriteVertex);
				}
			}
			break;
			case draco::DT_FLOAT32:
			{
				pVertexAttDataType[i] = 1;//AT_FLOAT
				pVertexAttData[i] = new float[pVertexAttDim[i] * nNumPoints];
				for (int j = 0; j < nNumPoints; j++)
				{
					vertexAttrAttValueIndex = pVertexAtt->mapped_index(draco::PointIndex(j));
					float* pWriteVertex = (float*)pVertexAttData[i] + j * pVertexAttDim[i];
					pVertexAtt->GetValue(vertexAttrAttValueIndex, pWriteVertex);
				}
			}
			break;
			case draco::DT_FLOAT64:
			{
				pVertexAttDataType[i] = 2;//AT_DOUBLE
				pVertexAttData[i] = new double[pVertexAttDim[i] * nNumPoints];
				for (int j = 0; j < nNumPoints; j++)
				{
					vertexAttrAttValueIndex = pVertexAtt->mapped_index(draco::PointIndex(j));
					double* pWriteVertex = (double*)pVertexAttData[i] + j * pVertexAttDim[i];
					pVertexAtt->GetValue(vertexAttrAttValueIndex, pWriteVertex);
				}
			}
			break;
			case draco::DT_UINT16:
			{
				pVertexAttDataType[i] = 3;//AT_16BIT
				pVertexAttData[i] = new uint16_t[pVertexAttDim[i] * nNumPoints];
				for (int j = 0; j < nNumPoints; j++)
				{
					vertexAttrAttValueIndex = pVertexAtt->mapped_index(draco::PointIndex(j));
					uint16_t* pWriteVertex = (uint16_t*)pVertexAttData[i] + j * pVertexAttDim[i];
					pVertexAtt->GetValue(vertexAttrAttValueIndex, pWriteVertex);
				}
			}
			break;
			case draco::DT_UINT8:
			{
				pVertexAttDataType[i] = 4;//AT_8BIT
				pVertexAttData[i] = new uint8_t[pVertexAttDim[i] * nNumPoints];
				for (int j = 0; j < nNumPoints; j++)
				{
					vertexAttrAttValueIndex = pVertexAtt->mapped_index(draco::PointIndex(j));
					uint8_t* pWriteVertex = (uint8_t*)pVertexAttData[i] + j * pVertexAttDim[i];
					pVertexAtt->GetValue(vertexAttrAttValueIndex, pWriteVertex);
				}
			}
			break;
			default:
				break;
			}
		}
	}

	if (pReadColor != 0)
	{
		delete[] pReadColor;
		pReadColor = 0;
	}

	if (mesh != 0)
	{
		bUseUint = nNumPoints > 65535;
		uint32_t* pUintIndex = 0;
		uint16_t* pUnshortIndex = 0;

		uint32_t nNumFace = static_cast<uint32_t> (mesh->num_faces()); //索引数量
		nIndexesCount = nNumFace * 3;
		if (bUseUint)
		{
			pUintIndex = new uint32_t[nIndexesCount];
		}
		else
		{
			pUnshortIndex = new uint16_t[nIndexesCount];
		}
		for (int i = 0; i < nNumFace; i++)
		{
			draco::FaceIndex faceID(i);
			std::array<draco::PointIndex, 3> faces = mesh->face(faceID);
			for (int j = 0; j < 3; j++)
			{
				if (bUseUint)
				{
					pUintIndex[i * 3 + j] = faces[j].value();
				}
				else
				{
					pUnshortIndex[i * 3 + j] = faces[j].value();
				}
			}
		}

		if (bUseUint)
		{
			pIndex = (unsigned short*)pUintIndex;
		}
		else
		{
			pIndex = pUnshortIndex;
		}
	}
	return true;
}

void S3MB::DracoTool::DracoCompressImpl(int nPosQuantizationBits, int nTexcoordsQuantizationBits, int nNormalsQuantizationBits, int nGenericQuantizationBits, int nEncodeSpeed, int nDecodeSpeed,
	int texCoordAttributeType, float* pVertices, unsigned int nVerticesCount, unsigned short nVertexDim, float* pNormals, unsigned int nNormalCount, unsigned short nNormalDim,
	unsigned int * pVertexColor, unsigned int nVertexColorCount, float** pTexCoords, unsigned int* nTexCoordCount,unsigned short* pTexCoordDim, 
	void** pVertexAttData, unsigned short*& pVertexAttDataType, unsigned int* pVertexAttCount, unsigned short* pVertexAttDim, unsigned int nVertexAttCount,
	unsigned short* pIndex, unsigned int nIndexesCount, bool bUseUint, char*& pOutputData, unsigned int& sizeInBytes, int& nPosUniqueID, int& nNormalUniqueID,
	int& nVertexColorUniqueID, int*& pTexCoordUniqueID, int*& pVertexAttUniqueID)
{
	draco::Mesh* pDracoMesh = 0;
	draco::PointCloud* pPointCloud = 0;
	if (pIndex == 0)
	{
		pPointCloud = new draco::PointCloud;
	}
	else
	{
		pDracoMesh = new draco::Mesh;
		pPointCloud = pDracoMesh;
	}

	draco::EncoderBuffer buffer;
	pPointCloud->set_num_points(nVerticesCount);
	std::vector<float> atttibuteData;

	// 索引
	if (pIndex != 0)
	{
		const int numTriangles = nIndexesCount / 3;
		pDracoMesh->SetNumFaces(numTriangles);
		if (bUseUint)
		{
			unsigned int* pUintIndex = (unsigned int*)pIndex;
			for (draco::FaceIndex i(0); i < numTriangles; ++i) {
				draco::Mesh::Face face;
				face[0] = pUintIndex[i.value() * 3];
				face[1] = pUintIndex[i.value() * 3 + 1];
				face[2] = pUintIndex[i.value() * 3 + 2];
				pDracoMesh->SetFace(i, face);
			}
		}
		else
		{
			for (draco::FaceIndex i(0); i < numTriangles; ++i) {
				draco::Mesh::Face face;
				face[0] = pIndex[i.value() * 3];
				face[1] = pIndex[i.value() * 3 + 1];
				face[2] = pIndex[i.value() * 3 + 2];
				pDracoMesh->SetFace(i, face);
			}

		}
	}

	// 顶点
	if (nVerticesCount > 0 && pVertices != 0)
	{
		int nCount = nVerticesCount * nVertexDim;
		for (int i = 0; i < nCount; i++)
		{
			atttibuteData.push_back(pVertices[i]);
		}

		AddAttributeData(pPointCloud, atttibuteData, draco::GeometryAttribute::POSITION, draco::DT_FLOAT32, nVertexDim, nPosUniqueID);
	}
	atttibuteData.clear();

	// 法线
	if (nNormalCount > 0 && pNormals != 0)
	{
		int nCount = nNormalCount * nNormalDim;
		for (int i = 0; i < nCount; i++)
		{
			atttibuteData.push_back(pNormals[i]);
		}
		AddAttributeData(pPointCloud, atttibuteData, draco::GeometryAttribute::NORMAL, draco::DT_FLOAT32, nNormalDim, nNormalUniqueID);
	}
	atttibuteData.clear();

	// 颜色
	if (nVertexColorCount > 0 && pVertexColor != 0)
	{
		std::vector<unsigned char> colorAtttibuteData;
		for (int i = 0; i < nVertexColorCount; i++)
		{
			unsigned int nColor = pVertexColor[i];
			unsigned char fR = Palette::REDVAL(nColor);
			unsigned char fG = Palette::GREENVAL(nColor);
			unsigned char fB = Palette::BLUEVAL(nColor);
			unsigned char fA = Palette::ALPHAVAL(nColor);

			colorAtttibuteData.push_back(fR);
			colorAtttibuteData.push_back(fG);
			colorAtttibuteData.push_back(fB);
			colorAtttibuteData.push_back(fA);
		}
		draco::PointAttribute att;
		draco::GeometryAttribute::Type att_type = draco::GeometryAttribute::COLOR;
		att.Init(att_type, 4, draco::DT_UINT8, false, nVertexColorCount);
		nVertexColorUniqueID = pPointCloud->AddAttribute(att, true, nVertexColorCount);
		draco::PointAttribute *att_ptr = pPointCloud->attribute(nVertexColorUniqueID);
		for (draco::PointIndex i(0); i < nVertexColorCount; ++i) {
			att_ptr->SetAttributeValue(att_ptr->mapped_index(i), &colorAtttibuteData[i.value() * 4]);
		}
		colorAtttibuteData.clear();
	}

	// 纹理坐标
	for (int i = 0; i < 8; i++)
	{
		if (nTexCoordCount[i] > 0 && pTexCoords[i] != 0)
		{
			int nCount = nTexCoordCount[i] * pTexCoordDim[i];
			for (int j = 0; j < nCount; j++)
			{
				atttibuteData.push_back(pTexCoords[i][j]);
			}
			AddAttributeData(pPointCloud, atttibuteData, draco::GeometryAttribute::GENERIC, draco::DT_FLOAT32, pTexCoordDim[i], pTexCoordUniqueID[i]);
		}
		atttibuteData.clear();
	}

	// 自定义属性
	for (int i = 0; i < nVertexAttCount; i++)
	{
		if (pVertexAttCount[i] > 0 && pVertexAttData[i] != 0)
		{
			unsigned int nCount = pVertexAttCount[i] * pVertexAttDim[i];
			switch (pVertexAttDataType[i])
			{
			case 0://AT_32BIT
			{
				std::vector<uint32_t> vecAttData;
				for (unsigned int j = 0; j < nCount; j++)
				{
					vecAttData.push_back(((uint32_t*)pVertexAttData[i])[j]);
				}
				AddAttributeData(pPointCloud, vecAttData, draco::GeometryAttribute::GENERIC, draco::DT_UINT32, pVertexAttDim[i], pVertexAttUniqueID[i]);
			}
			break;
			case 1://AT_FLOAT
			{
				std::vector<float> vecAttData;
				for (unsigned int j = 0; j < nCount; j++)
				{
					vecAttData.push_back(((float*)pVertexAttData[i])[j]);
				}
				AddAttributeData(pPointCloud, vecAttData, draco::GeometryAttribute::GENERIC, draco::DT_FLOAT32, pVertexAttDim[i], pVertexAttUniqueID[i]);
			}
			break;
			case 2://AT_DOUBLE
			{
				std::vector<double> vecAttData;
				for (unsigned int j = 0; j < nCount; j++)
				{
					vecAttData.push_back(((double*)pVertexAttData[i])[j]);
				}
				AddAttributeData(pPointCloud, vecAttData, draco::GeometryAttribute::GENERIC, draco::DT_FLOAT64, pVertexAttDim[i], pVertexAttUniqueID[i]);
			}
			break;
			case 3://AT_16BIT
			{
				std::vector<uint16_t> vecAttData;
				for (unsigned int j = 0; j < nCount; j++)
				{
					vecAttData.push_back(((uint16_t*)pVertexAttData[i])[j]);
				}
				AddAttributeData(pPointCloud, vecAttData, draco::GeometryAttribute::GENERIC, draco::DT_UINT16, pVertexAttDim[i], pVertexAttUniqueID[i]);
			}
			break;
			default:
				break;
			}
		}
		atttibuteData.clear();
	}

	pPointCloud->DeduplicateAttributeValues();
	pPointCloud->DeduplicatePointIds();

	// 压缩
	CompressDracoMesh(nPosQuantizationBits, nTexcoordsQuantizationBits, nNormalsQuantizationBits, nGenericQuantizationBits, nEncodeSpeed, nDecodeSpeed,
		pPointCloud, pDracoMesh, buffer);
	sizeInBytes = buffer.size();
	std::vector<char>* pBuffer = buffer.buffer();
	// 此指针需要外部释放
	pOutputData = new char[sizeInBytes];
	memcpy(pOutputData, pBuffer->data(), sizeof(char) * sizeInBytes);

	if (pDracoMesh != 0)
	{
		delete pDracoMesh;
		pDracoMesh = 0;
		pPointCloud = 0;
	}
	else
	{
		delete pPointCloud;
		pPointCloud = 0;
	}
}

