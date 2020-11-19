#include <fstream>
#include <iostream>
#include "MemoryStream.h"
#include <map>
#include "Skeleton.h"
#include "Material.h"
#include "JsonParser.h"
#include "Shell.h"
#include "Texture.h"



using namespace std;

namespace S3MB
{
#define S3MB_ID												        L"id"
#define S3MB_TEXTURE_URL							                L"url"//����ļ�
#define S3MB_TEXTURE_U								                L"u"
#define S3MB_TEXTURE_V								                L"v"
#define S3MB_TEXTURE_W								                L"w"

#define S3MB_MATERIALS												L"materials"
#define S3MB_MATERIAL												L"material"
#define S3MB_MATERIAL_AMBIENT									    L"ambient"
#define S3MB_MATERIAL_DIFFUSE									    L"diffuse"
#define S3MB_MATERIAL_SPECULAR								        L"specular"
#define S3MB_MATERIAL_SHINESS									    L"shininess"

#define S3MB_MATPASS_CULLMODE								        L"cullMode"
#define S3MB_MATPASS_CULLMODE_NONE					                L"none"
#define S3MB_MATPASS_CULLMODE_C							            L"clockwise"
#define S3MB_MATPASS_CULLMODE_CC							        L"counterClockwise"

#define S3MB_MATERIAL_TRANSPARENTSORT					            L"transparentsorting"
#define S3MB_MATERIAL_TEXTUNITS								        L"textureunitstates"
#define S3MB_MATERIAL_TEXTUNIT									    L"textureunitstate"
#define S3MB_MATERIAL_TEXTUNIT_ADDMODE				                L"addressmode"
#define S3MB_MATERIAL_TEXTUNIT_FILTEROPTION			                L"filteringoption"
#define S3MB_MATERIAL_TEXTUNIT_FILTERMIN			                L"filtermin"
#define S3MB_MATERIAL_TEXTUNIT_FILTERMAX				            L"filtermax"
#define S3MB_MATERIAL_TEXTUNIT_TEXMODMATRIX		                    L"texmodmatrix"

#define S3MB_MATERIAL_PBRTYPE_MR							        L"pbrMetallicRohness"
#define S3MB_MATERIAL_PBRTYPE_SG								    L"pbrSpecularGlossiness"
#define S3MB_MATERIAL_PBR_EMISSIVEFACTOR				            L"emissiveFactor" //Vector3d
#define S3MB_MATERIAL_PBR_EMISSIVETEXTURE			                L"emissiveTexture"//wstring
#define S3MB_MATERIAL_PBR_NORMALTEXTURE				                L"normalTexture"//wstring
#define S3MB_MATERIAL_PBR_OCCLUSIONTEXTURE		                    L"occlusionTexture"//wstring
#define S3MB_MATERIAL_PBR_BASECOLOR					                L"baseColor"//Vector4d
#define S3MB_MATERIAL_PBR_BASECOLORTEXTURE		                    L"baseColorTexture"//wstring
#define S3MB_MATERIAL_PBR_ALPHAMODE					                L"alphaMode"//enum AlphaMode
#define S3MB_MATERIAL_PBR_ALPHAMODE_OPAQUE		                    L"opaque"
#define S3MB_MATERIAL_PBR_ALPHAMODE_MASK		                    L"mask" 
#define S3MB_MATERIAL_PBR_ALPHAMODE_BLEND		                    L"blend"
#define S3MB_MATERIAL_PBR_ALPHACUTOFF				                L"alphaCutoff"//float
	//PBRMetallicRoh
#define S3MB_MATERIAL_PBRM_ROUGHNESSTEXTURE			                L"metallicRohnessTexture"//wstring
#define S3MB_MATERIAL_PBRM_METALLICFACTOR					        L"metallicFactor"//float
#define S3MB_MATERIAL_PBRM_ROUGHNESSFACTOR				            L"rohnessFactor"//float
//PBRSpecularGlossy
#define S3MB_MATERIAL_PBRS_DIFFUSEFACTOR						    L"diffuseFactor"//Vector4d
#define S3MB_MATERIAL_PBRS_SPECULARFACTOR					        L"specularFactor"//Vector3d
#define S3MB_MATERIAL_PBRS_GLOSSINESSFACTOR				            L"glossinessFactor"//float
#define S3MB_MATERIAL_PBRS_DIFFUSETEXTURE					        L"diffuseTexture"//wstring
#define S3MB_MATERIAL_PBRS_SPECULARGLOSSINESSTEXTURE	            L"specularGlossinessTexture"//wstring
//! \brief Return the maximum of a or b
#define MAX(a,b) (((a)>(b))?(a):(b))

//! \brief Return the minimum of a or b
#define MIN(a,b) (((a)>(b))?(b):(a))

	enum S3MBVertexOptions
	{
		SVO_HasInstSelInfo = 1,		//��ʵ�����洢��Ϣ
	};
	struct  SelectInfo
	{
		int m_nVertexColorCount;
		int m_nVertexColorOffset;
		SelectInfo()
		{
			m_nVertexColorCount = 0;
			m_nVertexColorOffset = 0;
		}
	};

	struct BatchSelAndVertexColor
	{
		//mesh�е�ʵ����first��ʵ����selectionID, second��ʵ������
		std::map<unsigned int, std::vector<unsigned short>> mapInstanceIdVec;
		//ʵ��ID��ԭʼ�Ķ�����ɫ��map
		std::map<unsigned int, unsigned int> mapInstanceColor;
	};

	class S3MB_API S3MBTools
	{
	public:
		enum S3MBVertexTag
		{
			SV_Unkown = 0,			//δ֪
			SV_Standard = 1,			//��������
			SV_Compressed = 2,		//��ѹ����
		};
		//��4λ�洢RGBA
		static unsigned int RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		//��һ����ֵ���Ƶ�һ����Χ��. 
		template <typename T>
		static T Clamp(T val, T minval, T maxval);
		//����S3MB�ļ�
		static bool LoadStreamDataFromFile(ifstream& fileStream, MemoryStream& streamUnZipped, MemoryStream& streamShell, MemoryStream& streamSkeleton, \
			MemoryStream& streamSecondColor, MemoryStream& streamTexture, MemoryStream& streamSelInfo, wstring& strJsonMaterials);
		//��ѹS3MB�ļ������ͷ�ѹ�����ڴ�
		static bool UnZipData(MemoryStream& streamUnZipped, unsigned char*& pZippedData, unsigned int nZippedSize);
		static void LoadStream(MemoryStream& streamSrc, MemoryStream& streamDst);


		//���عǼ���Ϣ
		static void LoadSkeleton(MemoryStream& streamSkeleton, std::map<wstring, Geometry*>& mapGeometry);
		static bool LoadSkeletonData(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage, vector<IndexPackage*>& arrIndexPackage);
		//�ֽڶ���
		//isRead ָʾ�Ƿ�Ϊ��ȡ��
		static void StreamAlign(MemoryStream& stream, bool isRead);
		//���ض�����Ϣ
		static bool LoadVertex(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage);
		//���ط�����Ϣ
		static bool LoadNormal(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage);
		//���ض�����ɫ
		static bool LoadVertexColor(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage);
		//���ض���ڶ���ɫ
		static bool LoadSecondVertexColor(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage);
		//��������������Ϣ
		static bool LoadTextureCoords(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage, unsigned short nTextureCoord);
		//����ʵ������Ϣ
		static bool LoadInstanceInfo(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage, unsigned short nTextureCoord);
		//�ж��Ƿ���ʵ�������ζ���
		static bool IsInstanceBatch(VertexDataPackage* pVertexDataPackage);
		//����ѹ��������Ϣ
		static bool LoadCompressVertex(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage);
		//����ѹ��������Ϣ
		static bool LoadCompressNormal(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage);
		//��ѹ��ѹ���ķ�����Ϣ
		static bool DecompressNormal(unsigned int nNormalCount, short* pEncodeNormal, float*& pNormal);
		//����ѹ������������Ϣ
		static bool LoadCompressTextureCoords(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage, unsigned short nTextureCoord);
		//��������ѹ������������Ϣ
		static bool LoadSingleCompressTextureCoords(MemoryStream& stream, unsigned int nTexIdx, unsigned int nTotalTexCount, VertexDataPackage* pVertexDataPackage);


		//���ز�����Ϣ
		static void LoadMaterial(wstring strJsonMaterials, std::map<wstring, Material*>& mapMaterial);
		//ͨ��Json���ز�����Ϣ
		static void LoadMatFromJson(JsonValue& jsonMaterial, wstring& strMaterialName, Technique* pTechnique);
		//ͨ��Json����PBR������Ϣ
		static void LoadPBRFromJson(JsonValue& jsonPBR, PBRParams*& pPBR);
		//ͨ��Json����������Ԫ
		static void LoadTUSFromJson(JsonValue& jsonTexture, TextureUnitState* pTextureUnitState);

		static CullingMode CullModeFromString(wstring strType);

		static PBRParams::AlphaMode AlphaModeFromString(wstring strType);

		static void AddressModeFromJson(JsonValue& jsonAddMode, TextureUnitState* pTextureUnitState);

		//����������Ϣ
		static void LoadTexture(MemoryStream& streamTexture, std::map<wstring, TextureDataInfo*>& mapTexture);
		static void LoadTextureData(MemoryStream& streamTexture, TextureData* pTextureData);
		static void ProcessTextureData(TextureData* pTextureData, CodecType nCompressType);

		//����shell��Ϣ
		static void LoadShell(MemoryStream& streamShell, RenderOperationGroup* pROGroup);
		//! \brief �ж��Ƿ��ǵ�����Geode��RO������PagedLODs�¿ɵ�������Geode����ʾ�������²�ڵ㣩
		static bool IsGeodeWithoutChild(MemoryStream& stream);
		static void LoadROGeodeWithoutChild(RenderOperationGroup* pROGroup, RenderOperationGeode* pGeode, MemoryStream& stream);
		static void LoadShellPagedLOD(RenderOperationGroup* pROGroup, RenderOperationPagedLOD* pROPagedLOD, MemoryStream& stream);
		static void LoadShellROGeode(RenderOperationGroup* pROGroup, RenderOperationGeode* pGeode, MemoryStream& stream);

		//����shell��Ϣ
		static void SaveShell(RenderOperationGroup* pROGroup, MemoryStream& stream);
		static void SaveShellPagedLOD(RenderOperationPagedLOD* pROPagedLOD, MemoryStream& stream);
		static void SaveShellROGeode(RenderOperationGeode* pROGeode, MemoryStream& stream);

		//����Ǽ���Ϣ
		static void SaveGeometry(std::map<wstring, Geometry*>& mapSkeleton, MemoryStream& streamGeometry, int vertexCompressOptions = 19);
		//! \brief �Ǽ����ݱ��������
		static bool SaveSkeletonData(VertexDataPackage*& pVertexDataPackage, \
			vector<IndexPackage*>& arrIndexPackage, MemoryStream& streamSkeleton, int vertexCompressOptions = 19);
		//���涥����Ϣ
		static bool SaveVertex(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//���淨����Ϣ
		static bool SaveNormal(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//���涥����ɫ
		static bool SaveVertexColor(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//���涥��������Ϣ
		static bool SaveSecondVertexColor(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//������������
		static bool SaveTextureCoords(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//! \brief �Ƿ�����������
		static bool IsTexutureCoord(unsigned short nDimesion);
		//! \brief �Ƿ�����������洢��Wλ
		static bool IsTexutureCoordStoreW(VertexDataPackage* pVertexDataPack, int nTexCoordIndex);
		//! \brief ����ʵ������Ϣ
		static bool SaveInstanceInfo(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//! \brief ����ѹ���Ķ���
		static bool SaveCompressVertex(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//! \brief ��float��ʽ�Ķ����������ѹ����ѹ����Ϊshort����
		//! \param pValue [in] ��������
		//! \param nVertexCount [in] �������
		//! \param nSrcDimension [in] ԭʼ����Ķ���ά��
		//! \param nDstDimension [in] ѹ��������Ķ���ά��
		//! \param nQuantizationBits [in] ѹ�����ֵռ�ü����ֽڣ����ܳ���16
		//! \param pEncodeValue [out] ���ѹ���������
		//! \param fNormalConstant [out] �������ѹ���Ĺ�һ��ϵ��
		//! \param minVal [out] ������涥���ά����Сֵ��ָ��
		static void RangeEncode(float* pValue, unsigned int nVertexCount, unsigned int nSrcDimension, unsigned int nDstDimension, unsigned int nQuantizationBits, short*& pEncodeValue,
			float& fNormalConstant, float*& minVal);
		//! \brief ���㶥����������Сֵ
		//! \param pValue [in] ��������
		//! \param nDimension [in] ���������ά��
		//! \param nVertexCount [in] �������
		//! \param pOutMin [out] ������涥���ά����Сֵ��ָ��
		//! \param pOutMax [out] ������涥���ά�����ֵ��ָ��
		static void ComputeMaxMin(float* pValue, unsigned int nDimension, unsigned int nVertexCount, float* pOutMin, float* pOutMax);
		//! \brief ����ѹ���ķ���
		static bool SaveCompressNormal(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//! \brief ѹ������
		static bool CompressNormal(unsigned int nNormalCount, float* pNormal, short*& pEncodeNormal);
		//! \brief ����ѹ������������
		static bool SaveCompressTextureCoords(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//! \brief ����ѹ����һ����������
		static bool SaveOneCompressTextureCoords(VertexDataPackage* pVertexDataPack, MemoryStream& stream, unsigned int nTexIdx);

		//���������Ϣ
		static void SaveMaterial(std::map<wstring, Material*>& mapMaterial, JsonValue& jsonMaterials);
		static void ToJson(Material* pMaterial, JsonValue& jsonMaterial);
		//! \brief pMaterialתjson��Ϣ
		static void ToJson(const wstring strMaterialName, Technique* pTechnique, JsonValue& jsonContent);
		static wstring MaterialParamTypeToString(MaterialParamType emType);
		static wstring CullModeToString(CullingMode emType);
		//! \brief PRBתjson��Ϣ
		static void ToJson(PBRParams* pPBR, JsonValue& jsonPBR);
		static wstring AlphaModeToString(PBRParams::AlphaMode emType);
		//! \brief pTextureUnitStateתjson��Ϣ
		static void ToJson(TextureUnitState* pTextureUnitState, JsonValue& jsonMaterial);
		static void AddressModeToJson(TextureUnitState* pTextureUnitState, JsonValue& jsonAddMode);

		static void SaveTextures(std::map<wstring, TextureDataInfo*>& mapTexture, \
			MemoryStream& streamTexture, unsigned int nCompressType, bool bTextureTransform);
		//! \brief �������ݱ���
		static void SaveTextureData(TextureData* pTextureData, \
			MemoryStream& streamTexture, unsigned int nCompressType, bool bIsChangeTexture = true);
		//! \brief ����ѹ������
		static void ProcessTextureData(TextureData* pTextureData, unsigned int nCompressType) { return ProcessTextureData(pTextureData, (CodecType)nCompressType); }

		//! \brief �Ƿ��������Ϣ
		static bool HasSelectionInfo(VertexDataPackage* pVertexDataPack);
		//! \brief ���SelectionInfo
		static bool SaveSelectionInfo(std::map<wstring, Geometry*>& mapSkeleton, MemoryStream& m_streamSelectionInfo);

		static bool SaveStreamData2File(const wstring strDestFilePath, \
			MemoryStream& streamShell, MemoryStream& streamSkeleton, \
			MemoryStream& streamSecondColor, wstring& strJsonMaterials, \
			MemoryStream& streamTexture, \
			bool bHasSelectionInfo, MemoryStream& streamSelInfo);

		static bool ZipData(unsigned char* pvDestBuffer, unsigned int& dwDestLen,
			const unsigned char* pvSrcBuffer, unsigned int dwSrcLen, int nLevel = 8);
	};

	template <typename T>
	T S3MBTools::Clamp(T val, T minval, T maxval)
	{
		if (minval > maxval)
		{
			T temp = minval;
			minval = maxval;
			maxval = temp;
		}
		return max(min(val, maxval), minval);
	}
}