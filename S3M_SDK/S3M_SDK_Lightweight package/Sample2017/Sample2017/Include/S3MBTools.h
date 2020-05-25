#include <fstream>
#include <iostream>
#include "zlib/include/zlib.h"
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
#define S3MB_ID												        "id"
#define S3MB_TEXTURE_URL							                "url"//外挂文件
#define S3MB_TEXTURE_U								                "u"
#define S3MB_TEXTURE_V								                "v"
#define S3MB_TEXTURE_W								                "w"

#define S3MB_MATERIALS												"materials"
#define S3MB_MATERIAL												"material"
#define S3MB_MATERIAL_AMBIENT									    "ambient"
#define S3MB_MATERIAL_DIFFUSE									    "diffuse"
#define S3MB_MATERIAL_SPECULAR								        "specular"
#define S3MB_MATERIAL_SHINESS									    "shininess"

#define S3MB_MATPASS_CULLMODE								        "cullMode"
#define S3MB_MATPASS_CULLMODE_NONE					                "none"
#define S3MB_MATPASS_CULLMODE_C							            "clockwise"
#define S3MB_MATPASS_CULLMODE_CC							        "counterClockwise"

#define S3MB_MATERIAL_TRANSPARENTSORT					            "transparentsorting"
#define S3MB_MATERIAL_TEXTUNITS								        "textureunitstates"
#define S3MB_MATERIAL_TEXTUNIT									    "textureunitstate"
#define S3MB_MATERIAL_TEXTUNIT_ADDMODE				                "addressmode"
#define S3MB_MATERIAL_TEXTUNIT_FILTEROPTION			                "filteringoption"
#define S3MB_MATERIAL_TEXTUNIT_FILTERMIN			                "filtermin"
#define S3MB_MATERIAL_TEXTUNIT_FILTERMAX				            "filtermax"
#define S3MB_MATERIAL_TEXTUNIT_TEXMODMATRIX		                    "texmodmatrix"

#define S3MB_MATERIAL_PBRTYPE_MR							        "pbrMetallicRohness"
#define S3MB_MATERIAL_PBRTYPE_SG								    "pbrSpecularGlossiness"
#define S3MB_MATERIAL_PBR_EMISSIVEFACTOR				            "emissiveFactor" //Vector3d
#define S3MB_MATERIAL_PBR_EMISSIVETEXTURE			                "emissiveTexture"//string
#define S3MB_MATERIAL_PBR_NORMALTEXTURE				                "normalTexture"//string
#define S3MB_MATERIAL_PBR_OCCLUSIONTEXTURE		                    "occlusionTexture"//String
#define S3MB_MATERIAL_PBR_BASECOLOR					                "baseColor"//Vector4d
#define S3MB_MATERIAL_PBR_BASECOLORTEXTURE		                    "baseColorTexture"//string
#define S3MB_MATERIAL_PBR_ALPHAMODE					                "alphaMode"//enum AlphaMode
#define S3MB_MATERIAL_PBR_ALPHAMODE_OPAQUE		                    "opaque"
#define S3MB_MATERIAL_PBR_ALPHAMODE_MASK		                    "mask" 
#define S3MB_MATERIAL_PBR_ALPHAMODE_BLEND		                    "blend"
#define S3MB_MATERIAL_PBR_ALPHACUTOFF				                "alphaCutoff"//float
	//PBRMetallicRoh
#define S3MB_MATERIAL_PBRM_ROUGHNESSTEXTURE			                "metallicRohnessTexture"//string
#define S3MB_MATERIAL_PBRM_METALLICFACTOR					        "metallicFactor"//float
#define S3MB_MATERIAL_PBRM_ROUGHNESSFACTOR				            "rohnessFactor"//float
//PBRSpecularGlossy
#define S3MB_MATERIAL_PBRS_DIFFUSEFACTOR						    "diffuseFactor"//Vector4d
#define S3MB_MATERIAL_PBRS_SPECULARFACTOR					        "specularFactor"//Vector3d
#define S3MB_MATERIAL_PBRS_GLOSSINESSFACTOR				            "glossinessFactor"//float
#define S3MB_MATERIAL_PBRS_DIFFUSETEXTURE					        "diffuseTexture"//string
#define S3MB_MATERIAL_PBRS_SPECULARGLOSSINESSTEXTURE	            "specularGlossinessTexture"//string
//! \brief Return the maximum of a or b
#define MAX(a,b) (((a)>(b))?(a):(b))

//! \brief Return the minimum of a or b
#define MIN(a,b) (((a)>(b))?(b):(a))

	enum S3MBVertexOptions
	{
		SVO_HasInstSelInfo = 1,		//带实例化存储信息
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
		//mesh中的实例，first是实例的selectionID, second是实例数组
		std::map<unsigned int, std::vector<unsigned short>> mapInstanceIdVec;
		//实例ID与原始的顶点颜色的map
		std::map<unsigned int, unsigned int> mapInstanceColor;
	};

	class S3MBTools
	{
	public:
		enum S3MBVertexTag
		{
			SV_Unkown = 0,			//未知
			SV_Standard = 1,			//正常数据
			SV_Compressed = 2,		//带压缩的
		};
		//用4位存储RGBA
		static unsigned int RGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
		//将一个数值限制到一定范围内. 
		template <typename T>
		static T Clamp(T val, T minval, T maxval);
		//加载S3MB文件
		static bool LoadStreamDataFromFile(ifstream& fileStream, MemoryStream& streamUnZipped, MemoryStream& streamShell, MemoryStream& streamSkeleton, \
			MemoryStream& streamSecondColor, MemoryStream& streamTexture, MemoryStream& streamSelInfo, string& strJsonMaterials);
		//解压S3MB文件流并释放压缩流内存
		static bool UnZipData(MemoryStream& streamUnZipped, unsigned char*& pZippedData, unsigned int nZippedSize);
		static void LoadStream(MemoryStream& streamSrc, MemoryStream& streamDst);


		//加载骨架信息
		static void LoadSkeleton(MemoryStream& streamSkeleton, std::map<string, Geometry*>& mapGeometry);
		static bool LoadSkeletonData(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage, vector<IndexPackage*>& arrIndexPackage);
		//字节对齐
		//isRead 指示是否为读取流
		static void StreamAlign(MemoryStream& stream, bool isRead);
		//加载顶点信息
		static bool LoadVertex(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage);
		//加载法线信息
		static bool LoadNormal(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage);
		//加载顶点颜色
		static bool LoadVertexColor(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage);
		//加载顶点第二颜色
		static bool LoadSecondVertexColor(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage);
		//加载纹理坐标信息
		static bool LoadTextureCoords(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage, unsigned short nTextureCoord);
		//加载实例化信息
		static bool LoadInstanceInfo(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage, unsigned short nTextureCoord);
		//判断是否是实例化几何对象
		static bool IsInstanceBatch(VertexDataPackage* pVertexDataPackage);
		//加载压缩顶点信息
		static bool LoadCompressVertex(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage);
		//加载压缩法线信息
		static bool LoadCompressNormal(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage);
		//解压被压缩的法线信息
		static bool DecompressNormal(unsigned int nNormalCount, short* pEncodeNormal, float*& pNormal);
		//加载压缩纹理坐标信息
		static bool LoadCompressTextureCoords(MemoryStream& streamSkeleton, VertexDataPackage*& pVertexDataPackage, unsigned short nTextureCoord);
		//处理单个压缩纹理坐标信息
		static bool LoadSingleCompressTextureCoords(MemoryStream& stream, unsigned int nTexIdx, unsigned int nTotalTexCount, VertexDataPackage* pVertexDataPackage);


		//加载材质信息
		static void LoadMaterial(string strJsonMaterials, std::map<string, Material*>& mapMaterial);
		//通过Json加载材质信息
		static void LoadMatFromJson(JsonValue& jsonMaterial, string& strMaterialName, Technique* pTechnique);
		//通过Json加载PBR材质信息
		static void LoadPBRFromJson(JsonValue& jsonPBR, PBRParams*& pPBR);
		//通过Json加载纹理单元
		static void LoadTUSFromJson(JsonValue& jsonTexture, TextureUnitState* pTextureUnitState);

		static CullingMode CullModeFromString(string strType);

		static PBRParams::AlphaMode AlphaModeFromString(string strType);

		static void AddressModeFromJson(JsonValue& jsonAddMode, TextureUnitState* pTextureUnitState);

		//加载纹理信息
		static void LoadTexture(MemoryStream& streamTexture, std::map<string, TextureDataInfo*>& mapTexture);
		static void LoadTextureData(MemoryStream& streamTexture, TextureData* pTextureData);
		static void ProcessTextureData(TextureData* pTextureData, CodecType nCompressType);

		//加载shell信息
		static void LoadShell(MemoryStream& streamShell, RenderOperationGroup* pROGroup);
		//! \brief 判断是否是单独的Geode（RO对象中PagedLODs下可单独挂载Geode，表示不再有下层节点）
		static bool IsGeodeWithoutChild(MemoryStream& stream);
		static void LoadROGeodeWithoutChild(RenderOperationGroup* pROGroup, RenderOperationGeode* pGeode, MemoryStream& stream);
		static void LoadShellPagedLOD(RenderOperationGroup* pROGroup, RenderOperationPagedLOD* pROPagedLOD, MemoryStream& stream);
		static void LoadShellROGeode(RenderOperationGroup* pROGroup, RenderOperationGeode* pGeode, MemoryStream& stream);

		//保存shell信息
		static void SaveShell(RenderOperationGroup* pROGroup, MemoryStream& stream);
		static void SaveShellPagedLOD(RenderOperationPagedLOD* pROPagedLOD, MemoryStream& stream);
		static void SaveShellROGeode(RenderOperationGeode* pROGeode, MemoryStream& stream);

		//保存骨架信息
		static void SaveGeometry(std::map<string, Geometry*>& mapSkeleton, MemoryStream& streamGeometry, int vertexCompressOptions = 19);
		//! \brief 骨架数据保存与加载
		static bool SaveSkeletonData(VertexDataPackage*& pVertexDataPackage, \
			vector<IndexPackage*>& arrIndexPackage, MemoryStream& streamSkeleton, int vertexCompressOptions = 19);
		//保存顶点信息
		static bool SaveVertex(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//保存法线信息
		static bool SaveNormal(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//保存顶点颜色
		static bool SaveVertexColor(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//保存顶点属性信息
		static bool SaveSecondVertexColor(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//保存纹理坐标
		static bool SaveTextureCoords(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//! \brief 是否是纹理坐标
		static bool IsTexutureCoord(unsigned short nDimesion);
		//! \brief 是否是纹理坐标存储了W位
		static bool IsTexutureCoordStoreW(VertexDataPackage* pVertexDataPack, int nTexCoordIndex);
		//! \brief 保存实例化信息
		static bool SaveInstanceInfo(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//! \brief 保存压缩的顶点
		static bool SaveCompressVertex(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//! \brief 对float格式的顶点数组进行压缩，压缩后为short类型
		//! \param pValue [in] 顶点数组
		//! \param nVertexCount [in] 顶点个数
		//! \param nSrcDimension [in] 原始数组的顶点维度
		//! \param nDstDimension [in] 压缩后数组的顶点维度
		//! \param nQuantizationBits [in] 压缩后的值占用几个字节，不能超过16
		//! \param pEncodeValue [out] 输出压缩后的数组
		//! \param fNormalConstant [out] 输出保存压缩的归一化系数
		//! \param minVal [out] 输出保存顶点各维度最小值的指针
		static void RangeEncode(float* pValue, unsigned int nVertexCount, unsigned int nSrcDimension, unsigned int nDstDimension, unsigned int nQuantizationBits, short*& pEncodeValue,
			float& fNormalConstant, float*& minVal);
		//! \brief 计算顶点数组的最大小值
		//! \param pValue [in] 顶点数组
		//! \param nDimension [in] 顶点数组的维度
		//! \param nVertexCount [in] 顶点个数
		//! \param pOutMin [out] 输出保存顶点各维度最小值的指针
		//! \param pOutMax [out] 输出保存顶点各维度最大值的指针
		static void ComputeMaxMin(float* pValue, unsigned int nDimension, unsigned int nVertexCount, float* pOutMin, float* pOutMax);
		//! \brief 保存压缩的法线
		static bool SaveCompressNormal(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//! \brief 压缩法线
		static bool CompressNormal(unsigned int nNormalCount, float* pNormal, short*& pEncodeNormal);
		//! \brief 保存压缩的纹理坐标
		static bool SaveCompressTextureCoords(VertexDataPackage* pVertexDataPack, MemoryStream& stream);
		//! \brief 保存压缩的一重纹理坐标
		static bool SaveOneCompressTextureCoords(VertexDataPackage* pVertexDataPack, MemoryStream& stream, unsigned int nTexIdx);

		//保存材质信息
		static void SaveMaterial(std::map<string, Material*>& mapMaterial, JsonValue& jsonMaterials);
		static void ToJson(Material* pMaterial, JsonValue& jsonMaterial);
		//! \brief pMaterial转json信息
		static void ToJson(const string strMaterialName, Technique* pTechnique, JsonValue& jsonContent);
		static string MaterialParamTypeToString(MaterialParamType emType);
		static string CullModeToString(CullingMode emType);
		//! \brief PRB转json信息
		static void ToJson(PBRParams* pPBR, JsonValue& jsonPBR);
		static string AlphaModeToString(PBRParams::AlphaMode emType);
		//! \brief pTextureUnitState转json信息
		static void ToJson(TextureUnitState* pTextureUnitState, JsonValue& jsonMaterial);
		static void AddressModeToJson(TextureUnitState* pTextureUnitState, JsonValue& jsonAddMode);

		static void SaveTextures(std::map<string, TextureDataInfo*>& mapTexture, \
			MemoryStream& streamTexture, unsigned int nCompressType, bool bTextureTransform);
		//! \brief 纹理数据保存
		static void SaveTextureData(TextureData* pTextureData, \
			MemoryStream& streamTexture, unsigned int nCompressType, bool bIsChangeTexture = true);
		//! \brief 纹理压缩处理
		static void ProcessTextureData(TextureData* pTextureData, unsigned int nCompressType) { return ProcessTextureData(pTextureData, (CodecType)nCompressType); }

		//! \brief 是否有外挂信息
		static bool HasSelectionInfo(VertexDataPackage* pVertexDataPack);
		//! \brief 填充SelectionInfo
		static bool SaveSelectionInfo(std::map<string, Geometry*>& mapSkeleton, MemoryStream& m_streamSelectionInfo);

		static bool SaveStreamData2File(const string strDestFilePath, \
			MemoryStream& streamShell, MemoryStream& streamSkeleton, \
			MemoryStream& streamSecondColor, string& strJsonMaterials, \
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
