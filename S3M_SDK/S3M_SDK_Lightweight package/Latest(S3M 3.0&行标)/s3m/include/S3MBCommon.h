#if !defined(S3MBCOMMON_H__CA9A7EEE_6991_4F50_BE87_F150E50ED55B__INCLUDED_)
#define S3MBCOMMON_H__CA9A7EEE_6991_4F50_BE87_F150E50ED55B__INCLUDED_

#pragma once
#include "stdafx.h"
#include "S3MBEnums.h"
#include <map>
#include <string.h>
#include <vector>
#ifndef WIN32
#include <fstream>
#include <regex>
#endif

namespace S3MB
{
	#define DBLMAX 1.7976931348623158e+308
	#define DBLMIN 2.2250738585072014e-308
	#define FLTMAX 3.402823466e+38F
	#define FLTMIN 1.175494351e-38F

	#ifndef IS0(x)
	//  极小值定义
	#define EP 1e-10
	#define NEP -1e-10
	#define IS0(x) (((x) < EP) && ((x) > NEP))
	#endif // !IS0(x)

	#ifndef EQUAL(x,y)
	#define EQUAL(x,y) Equal(x, y)
	#endif // !EQUAL(X,Y)

	#define GREATER_OR_EQUAL(x, y)	(((x) > (y)) || Equal(x, y))

	#define ROUND(x) (((x)>0)?int((x)+0.5):(int((x)-0.5)))

	// 绝对值
	#define ABS(x) (((x)>=0)?(x):-(x))

	// 选择最大值
	#define MAX(a,b) (((a)>(b))?(a):(b))
	// 选择最小值
	#define MIN(a,b) (((a)>(b))?(b):(a))

	// 多重纹理最大个数。
	#define SMSCN_MAX_TEXTURE_COORD_SETS 8

	#define SIZE_PER_INSTANCEINFO 16

	#define TEXTUREMOTION_OFFSETSPEEDU			U("offsetSpeedU")
	#define TEXTUREMOTION_OFFSETSPEEDV			U("offsetSpeedV")
	#define TEXTUREMOTION_OFFSETPERIOD			U("offsetPeriod")
	#define TEXTUREMOTION_TILINGSPEEDU			U("tilingSpeedU")
	#define TEXTUREMOTION_TILINGSPEEDV			U("tilingSpeedV")
	#define TEXTUREMOTION_TILINGPERIOD			U("tilingPeriod")
	#define TEXTUREMOTION_OFFSETSTARTU			U("offsetStartU")
	#define TEXTUREMOTION_OFFSETSTARTV			U("offsetStartV")
	#define TEXTUREMOTION_TILINGSTARTU			U("tilingStartU")
	#define TEXTUREMOTION_TILINGSTARTV			U("tilingStartV")


	#define S3MB_NAME												    U("name")
	#define S3MB_ID														U("id")
	#define S3MB_TEXTURE_NAME							                U("textureName")
	#define S3MB_TEXTURE_URL							                U("url")// 外挂文件
	#define S3MB_TEXTURE_U								                U("uAddressMode")
	#define S3MB_TEXTURE_V								                U("vAddressMode")
	#define S3MB_TEXTURE_W								                U("wAddressMode")

	#define S3MB_TEXTURE_ADDMODE										U("addressmode")
	#define S3MB_TEXTURE_COORDINATE_U									U("u")
	#define S3MB_TEXTURE_COORDINATE_V									U("v")
	#define S3MB_TEXTURE_COORDINATE_W									U("w")

	#define S3MB_MATERIALS												U("materials")
	#define S3MB_MATERIAL												U("material")
	#define S3MB_MATERIAL_TYPE											U("type")
	#define S3MB_MATERIAL_TYPE_COMMON									U("common")
	#define S3MB_MATERIAL_TYPE_PBR									    U("PBR")
	#define S3MB_MATERIAL_AMBIENT									    U("ambient")
	#define S3MB_MATERIAL_DIFFUSE									    U("diffuse")
	#define S3MB_MATERIAL_SPECULAR								        U("specular")
	#define S3MB_MATERIAL_SHININESS									    U("shininess")

	#define S3MB_MATERIAL_PBRTYPE_MR									U("pbrMetallicRoughness")
	#define S3MB_MATERIAL_PBRTYPE_SG									U("pbrSpecularGlossiness")

	#define S3MB_MATPASS_CULLMODE								        U("cullMode")
	#define S3MB_MATPASS_CULLMODE_NONE					                U("CULL_NONE")
	#define S3MB_MATPASS_CULLMODE_C							            U("CULL_CLOCKWISE")
	#define S3MB_MATPASS_CULLMODE_CC							        U("CULL_ANTICLOCKWISE")

	#define S3MB_MATPASS_CULLMODE_NONE_V3								U("none")
	#define S3MB_MATPASS_CULLMODE_C_V3									U("clockwise")
	#define S3MB_MATPASS_CULLMODE_CC_V3									U("counterClockwise")

	#define S3MB_MATERIAL_TEXTUNITS								        U("textureStates")
	#define S3MB_MATERIAL_TEXTUNIT									    U("textureUnitState")
	#define S3MB_MATERIAL_TEXTUNIT_FILTEROPTION			                U("filteringOption")
	#define S3MB_MATERIAL_TEXTUNIT_FILTERMIN			                U("minFilter")
	#define S3MB_MATERIAL_TEXTUNIT_FILTERMAX				            U("maxFilter")
	#define S3MB_MATERIAL_TEXTUNIT_MATRIX								U("matrix")

	#define S3MB_MATERIAL_TEXTUNITS_V3									U("textureunitstates")
	#define S3MB_MATERIAL_TEXTUNIT_V3									U("textureunitstate")
	#define S3MB_MATERIAL_TEXTUNIT_FILTEROPTION_V3						U("filteringoption")
	#define S3MB_MATERIAL_TEXTUNIT_FILTERMIN_V3							U("filtermin")
	#define S3MB_MATERIAL_TEXTUNIT_FILTERMAX_V3							U("filtermax")
	#define S3MB_MATERIAL_TEXTUNIT_MATRIX_V3							U("texmodmatrix")

	#define S3MB_MATERIAL_PBR_EMISSIVEFACTOR				            U("emissiveFactor") 
	#define S3MB_MATERIAL_PBR_EMISSIVETEXTURE			                U("emissiveTexture")
	#define S3MB_MATERIAL_PBR_NORMALTEXTURE				                U("normalTexture")
	#define S3MB_MATERIAL_PBR_OCCLUSIONTEXTURE		                    U("occlusionTexture")
	#define S3MB_MATERIAL_PBR_BASECOLOR					                U("baseColor")
	#define S3MB_MATERIAL_PBR_BASECOLORTEXTURE		                    U("baseColorTexture")
	#define S3MB_MATERIAL_PBR_ALPHAMODE					                U("alphaMode")
	#define S3MB_MATERIAL_PBR_ALPHAMODE_OPAQUE		                    U("AM_OPAQUE")
	#define S3MB_MATERIAL_PBR_ALPHAMODE_MASK		                    U("AM_MASK" )
	#define S3MB_MATERIAL_PBR_ALPHAMODE_BLEND		                    U("AM_BLEND")
	#define S3MB_MATERIAL_PBR_ALPHACUTOFF				                U("alphaCutoff")

	#define S3MB_MATERIAL_PBR_ALPHAMODE_OPAQUE_V3						U("opaque")
	#define S3MB_MATERIAL_PBR_ALPHAMODE_MASK_V3							U("mask")
	#define S3MB_MATERIAL_PBR_ALPHAMODE_BLEND_V3						U("blend")

	// PBRParameter
	#define S3MB_MATERIAL_PBR_EMISSIVETEXTUREINDEX						U("emissiveTextureIndex")
	#define S3MB_MATERIAL_PBR_EMISSIVETEXTURECOORDINDEX					U("emissiveTextureCoordIndex")
	#define S3MB_MATERIAL_PBR_EMISSIVETEXTUREMOTION						U("emissiveTextureMotion")
	#define S3MB_MATERIAL_PBR_NORMALTEXTUREINDEX						U("normalTextureIndex")
	#define S3MB_MATERIAL_PBR_NORMALTEXTURECOORDINDEX					U("normalTextureCoordIndex")
	#define S3MB_MATERIAL_PBR_NORMALTEXTURESCALE						U("normalTextureScale")
	#define S3MB_MATERIAL_PBR_OCCLUSIONTEXTUREINDEX						U("occlusionTextureIndex")
	#define S3MB_MATERIAL_PBR_OCCLUSIONTEXTURECOORDINDEX				U("occlusionTextureCoordIndex")
	#define S3MB_MATERIAL_PBR_OCCLUSIONTEXTURESTRENGTH					U("occlusionTextureStrength")
	#define S3MB_MATERIAL_PBR_MASKTEXTUREINDEX							U("maskTextureIndex")
	#define S3MB_MATERIAL_PBR_MASKTEXTURECOORDINDEX					    U("maskTextureCoordIndex")
	#define S3MB_MATERIAL_PBR_SEMANTICTEXTUREINDEX						U("semanticTextureIndex")
	#define S3MB_MATERIAL_PBR_SEMANTICTEXTURECOORDINDEX					U("semanticTextureCoordIndex")
	// PBRMetallicRough
	#define S3MB_MATERIAL_PBRM_BASECOLOR								U("baseColor")
	#define S3MB_MATERIAL_PBRM_BASECOLORTEXTUREINDEX					U("baseColorTextureIndex")
	#define S3MB_MATERIAL_PBRM_BASECOLORTEXTURECOORDINDEX				U("baseColorTextureCoordIndex")
	#define S3MB_MATERIAL_PBRM_BASECOLORTEXTUREMOTION					U("baseColorTextureMotion")
	#define S3MB_MATERIAL_PBRM_METALLICROUGHNESSTEXTUREINDEX			U("metallicRoughnessTextureIndex")
	#define S3MB_MATERIAL_PBRM_METALLICROUGHNESSTEXTURECOORDINDEX		U("metallicRoughnessTextureCoordIndex")

	#define S3MB_MATERIAL_PBRM_ROUGHNESSTEXTURE			                U("metallicRohnessTexture")
	#define S3MB_MATERIAL_PBRM_METALLICFACTOR					        U("metallicFactor")
	#define S3MB_MATERIAL_PBRM_ROUGHNESSFACTOR				            U("roughnessFactor")
	// PBRSpecularGlossy
	#define S3MB_MATERIAL_PBRS_DIFFUSEFACTOR						    U("diffuseFactor")
	#define S3MB_MATERIAL_PBRS_SPECULARFACTOR					        U("specularFactor")
	#define S3MB_MATERIAL_PBRS_GLOSSINESSFACTOR				            U("glossinessFactor")
	#define S3MB_MATERIAL_PBRS_DIFFUSETEXTURE					        U("diffuseTexture")
	#define S3MB_MATERIAL_PBRS_SPECULARGLOSSINESSTEXTURE	            U("specularGlossinessTexture")
	
	// nodes
	#define S3MB_NODES													U("nodes")
	#define S3MB_NODE_NAME												U("nodeName")
	#define S3MB_NODE_SKININDEX											U("skinIndex")
	#define S3MB_NODE_PATCHINDEX										U("patchIndex")
	#define S3MB_NODE_CHILDNODEINDEXES									U("childNodeIndexes")
	
	// skins
	#define S3MB_SKINS													U("skins")
	#define S3MB_SKIN_NAME												U("skinName")
	#define S3MB_SKIN_JOINTNODEINDEXES									U("jointNodeIndexes")
	#define S3MB_SKIN_INVERSEBINDMATRICES								U("inverseBindMatrices")
	
	// animationContents
	#define S3MB_ANIMATIONCONTENTS										U("animationContents")
	#define S3MB_ANIMATIONCONTENT_NAME									U("animationName")
	#define S3MB_ANIMATIONCONTENT_TIMEPOSITION							U("timePosition")
	#define S3MB_ANIMATIONCONTENT_TIMELENGTH							U("timeLength")
	#define S3MB_ANIMATIONCONTENT_STARTTIME								U("startTime")
	#define S3MB_ANIMATIONCONTENT_ENDTIME								U("endTime")
	// keyframeAnimations
	#define S3MB_ANIMATIONCONTENT_KEYFRAMES								U("keyframeAnimations")
	#define S3MB_ANIMATIONCONTENT_KEYFRAME_PATCHINDEX					U("patchIndex")
	#define S3MB_ANIMATIONCONTENT_KEYFRAME_PATH							U("path")
	#define S3MB_ANIMATIONCONTENT_KEYFRAME_FRAMEPOSITIONS				U("framePositions")
	#define S3MB_ANIMATIONCONTENT_KEYFRAME_TRANSFORMATIONS				U("transformations")
	// path
	#define S3MB_ANIMATIONCONTENT_KEYFRAME_PATH_SCALE					U("scale")
	#define S3MB_ANIMATIONCONTENT_KEYFRAME_PATH_ROTATION				U("rotation")
	#define S3MB_ANIMATIONCONTENT_KEYFRAME_PATH_TRANSLATION				U("translation")
	#define S3MB_ANIMATIONCONTENT_KEYFRAME_PATH_WEIGHTS					U("weights")
	
	//
	#define S3MB_MATERIAL_SHADERNAME_VERTEX								U("vertexShaderName")
	#define S3MB_MATERIAL_SHADERNAME_FRAGMENT							U("fragmentShaderName")

	#define S3MB_MATERIAL_EXTENSIONS									U("extensions")

	// 字符串
    #ifdef WIN32
        #define U(x) L ## x
    #else
        #define U(x) (string_to_wstring(x))
    #endif

	// 是否是路径分隔符
    #define ISPATHSEP(c) ((c)==L'/' || (c)==L'\\')
	// 路径分隔符
	#ifdef WIN32
		#define PATHSEPSTRING U("\\")
	#else
		#define PATHSEPSTRING U("/")
	#endif

	#define MAX_PRO_STRING_LENGTH 10240

	#define DXT1_ENCODE_SIZE(w,h) (((w-1)/4 + 1)*((h-1)/4 + 1)*8)
	#define DXT3_ENCODE_SIZE(w,h) (((w-1)/4 + 1)*((h-1)/4 + 1)*16)
	#define DXT5_ENCODE_SIZE DXT3_ENCODE_SIZE

	// 得到以unsigned char为单位的DWORD对齐的宽度.
	#define WIDTHBYTES(bits)    (((bits) + 31) / 32 * 4)

	// ==================================版本==================================
	#define S3MB_VERSIONV3			3.0f
	#define S3MB_VERSIONV3_0_1		3.01f// 行标
	// ============================================================================

	// ==================================描述文件==================================
	#define S3MB_JSON_X								    U("x")
	#define S3MB_JSON_Y							    	U("y")
	#define S3MB_JSON_Z								    U("z")

	#define SCPS_JSON_ASSET								U("asset")
	#define SCPS_JSON_VERSION							U("version")
	#define SCPS_JSON_DATATYPE							U("dataType")					// 倾斜、精模、点云、管线、矢量
	#define SCPS_JSON_PYRAMIDSPLITTYPE					U("pyramidSplitType")			// Octree		QuadTree
	#define SCPS_JSON_LODTYPE							U("lodType")					// LOD类型：添加 or 替换
	#define SCPS_JSON_POSITION							U("position")					// 位置点
	#define SCPS_JSON_POINT3D							U("point3D")					// 点
	#define SCPS_JSON_GEOBOUNDS							U("geoBounds")					// 地理范围
	#define SCPS_JSON_HEIGHTRANGE						U("heightRange")				// 高度范围
	#define SCPS_JSON_HEIGHTRANGE_MIN					U("min")
	#define SCPS_JSON_HEIGHTRANGE_MAX					U("max")
	#define SCPS_JSON_WDESCRIPT							U("wDescript")					// w位的描述
	#define SCPS_JSON_WCATEGORY							U("category")					// w位的含义
	#define SCPS_JSON_WRANGE							U("range")						// w值的范围
	#define SCPS_JSON_WRANGE_MIN						U("min")
	#define SCPS_JSON_WRANGE_MAX						U("max")
	#define SCPS_JSON_VERTEXATTRIBUTEDESCRIPT			U("vertexAttributeDescript")	// 顶点属性含义描述信息和数值范围
	#define SCPS_JSON_CATEGORY							U("category")					// 顶点属性或分类信息含义描述信息
	#define SCPS_JSON_RANGE								U("range")						// 顶点属性或分类信息数值范围
	#define SCPS_JSON_DIMENSION							U("dimension")					// 顶点属性或分类信息的维度
	#define SCPS_JSON_RANGE_MIN							U("min")						// 最小值
	#define SCPS_JSON_RANGE_MAX							U("max")						// 最大值
	#define SCPS_JSON_PRJCOORD							U("crs")						// 坐标系
	#define SCPS_JSON_PRJCOORD_EPSG				    	U("epsg:")						// 前缀

	#define SCPS_JSON_UNIT								U("unit")
	#define SCPS_JSON_UNITS								U("units")
	#define SCPS_JSON_UNITS_METER						U("Meter")
	#define SCPS_JSON_UNITS_DEGREE						U("Degree")
	#define SCPS_JSON_ROOTTILES							U("rootTiles")
	#define SCPS_JSON_TILES								U("tiles")
	#define SCPS_JSON_TILE								U("tile")
	#define SCPS_JSON_TILE_URL							U("url")
	#define SCPS_JSON_TILE_BBOX							U("boundingbox")
	#define SCPS_JSON_TILE_BBOX2						U("boundingBox")
	#define SCPS_JSON_TILE_BBOX_CENTER					U("center")
	#define SCPS_JSON_TILE_BBOX_XEXTENT					U("xExtent")
	#define SCPS_JSON_TILE_BBOX_YEXTENT					U("yExtent")
	#define SCPS_JSON_TILE_BBOX_ZEXTENT					U("zExtent")

	#define SCPS_JSON_EXTENSIONS						U("extensions")
	#define SCPS_JSON_EXTENSIONS_TYPE					U("type")
	#define SCPS_JSON_EXTENSIONS_NAME					U("name")
	#define SCPS_JSON_EXTENSIONS_NEU				    U("neu")

	#define SCPS_JSON_POINTCLOUD_LAYERS			        U("pointCloudLayers")
	#define SCPS_JSON_POINTCLOUD_LAYER		            U("layer")
	#define SCPS_JSON_POINTCLOUD_LAYERNAME		        U("layerName")
	#define SCPS_JSON_POINTCLOUD_LAYERBOUNDS	        U("layerBounds")

    // SCP标签枚举值
	#define SCPS_ASSET_VALUE							U("SuperMap")
	#define SCPS_UNKNOWN								U("Unknown")
	#define SCPS_DATATYPE_RealityMesh					U("RealityMesh")			// 倾斜摄影数据
	#define SCPS_DATATYPE_PNTCLOUD						U("PointCloud")				// 点云
	#define SCPS_DATATYPE_BIM							U("BIM")					// BIM
	#define SCPS_DATATYPE_VECTOR						U("Vector")					// 矢量
	#define SCPS_DATATYPE_NETWORK						U("Network")				// 网络数据
	#define SCPS_DATATYPE_INSTANCEMODEL					U("InstanceModel")			// 外挂模型
	#define SCPS_DATATYPE_ARTIFICIALMODEL				U("ArtificialModel")		// 人工模型

	#define SCPS_PYRAMIDSPLITTYPE_OCTREE				U("Octree")					// 八叉树
	#define SCPS_PYRAMIDSPLITTYPE_QUADTREE				U("QuadTree")				// 四叉树

	#define SCPS_LODTYPE_ADD							U("Add")
	#define SCPS_LODTYPE_REPLACE						U("Replace")

	// 纹理压缩类型值
	#define SCPS_TEXTURECOMPRESSIONTYPE_NONE			U("NONE")
	#define SCPS_TEXTURECOMPRESSIONTYPE_WEBP			U("WEBP")
	#define SCPS_TEXTURECOMPRESSIONTYPE_CRN				U("CRN_DXT5")
	#define SCPS_TEXTURECOMPRESSIONTYPE_DXT				U("DXT")
	#define SCPS_TEXTURECOMPRESSIONTYPE_KTX2		    U("KTX2.0")

	// 顶点压缩类型值
	#define SCPS_VERTEXCOMPRESSIONTYPE_NONE				U("NONE")
	// #define SCPS_VERTEXCOMPRESSIONTYPE_NORMAL			U("NORMAL")
	#define SCPS_VERTEXCOMPRESSIONTYPE_DRACO			U("DRACO")
	// #define SCPS_VERTEXCOMPRESSIONTYPE_MESHOPT		U("MESHOPT")
	// ============================================================================

	// ==================================索引树文件================================
	#define S3MB_LODTREEEXPORT							U("lodTreeExport")
	#define S3MB_PAGEDLODTREE_NAME						U("name")
	#define S3MB_PAGEDLODTREE_TILEINFO					U("tileInfo")
	#define S3MB_PAGEDLODTREE_TILEINFO_LODNUM			U("lodNum")
	#define S3MB_PAGEDLODTREE_TILEINFO_MODELPATH		U("modelPath")
	#define S3MB_PAGEDLODTREE_TILEINFO_INDEXPATH		U("indexFilePath")
	#define S3MB_PAGEDLODTREE_TILEINFO_RANGEMODE		U("rangeMode")
	#define S3MB_PAGEDLODTREE_TILEINFO_RANGEMODE_DISTANCE			U("distanceFromEvePoint")
	#define S3MB_PAGEDLODTREE_TILEINFO_RANGEMODE_PIXELSIZE			U("pixelSizeOnScreen")
	#define S3MB_PAGEDLODTREE_TILEINFO_RANGEMODE_GEOMETRY_ERROR		U("geometricError")
	#define S3MB_PAGEDLODTREE_TILEINFO_RANGEVALUE		U("rangeValue")
	#define S3MB_PAGEDLODTREE_TILEINFO_STATE			U("state")
	#define S3MB_PAGEDLODTREE_TILEINFO_STATE_ALLLEAF	U("AllLeaf")
	#define S3MB_PAGEDLODTREE_TILEINFO_STATE_PARTLEAF	U("PartLeaf")
	#define S3MB_PAGEDLODTREE_TILEINFO_STATE_NOLEAF		U("NoLeaf")
	#define S3MB_PAGEDLODTREE_TILEINFO_BBOX				U("boundingBox")
	#define S3MB_PAGEDLODTREE_TILEINFO_CHILDREN			U("children")
	#define S3MB_PAGEDLODTREE_STATUS					U("status")
	#define S3MB_PAGEDLODTREE_STATUS_LODCOUNT			U("lodCount")
	#define S3MB_PAGEDLODTREE_STATUS_TILESCOUNT			U("tilesCount")
	// ============================================================================

	// ==================================属性文件==================================
	#define S3MB_JSON_LAYERINFOS						U("layerInfos")
	#define S3MB_JSON_LAYERINFO					    	U("LayerInfo")
	#define S3MB_JSON_LAYERINFO_NAME					U("layerName")
	#define S3MB_JSON_LAYERINFO_IDRANGE				    U("idRange")
	#define S3MB_JSON_LAYERINFO_IDRANGE_MIN				U("minID")
	#define S3MB_JSON_LAYERINFO_IDRANGE_MAX				U("maxID")
	#define S3MB_JSON_LAYERINFO_FIELDINFOS			    U("fieldInfos")
	#define S3MB_JSON_LAYERINFO_FIELDINFO				U("fieldInfo")
	#define S3MB_JSON_FIELD_ATT_NAME					U("name")
	#define S3MB_JSON_FIELD_ATT_ALIAS					U("alias")
	#define S3MB_JSON_FIELD_ATT_TYPE					U("type")
	#define S3MB_JSON_FIELD_ATT_SIZE					U("size")
	#define S3MB_JSON_FIELD_ATT_ISREQUIRED			    U("isRequired")

	#define S3MB_JSON_FIELD_ATT_TYPE_BOOL			    U("bool")
	#define S3MB_JSON_FIELD_ATT_TYPE_INT16			    U("int16")
	#define S3MB_JSON_FIELD_ATT_TYPE_UINT16			    U("uint16")
	#define S3MB_JSON_FIELD_ATT_TYPE_INT32			    U("int32")
	#define S3MB_JSON_FIELD_ATT_TYPE_UINT32			    U("uint32")
	#define S3MB_JSON_FIELD_ATT_TYPE_INT64			    U("int64")
	#define S3MB_JSON_FIELD_ATT_TYPE_FLOAT			    U("float")
	#define S3MB_JSON_FIELD_ATT_TYPE_DOUBLE		        U("double")
	#define S3MB_JSON_FIELD_ATT_TYPE_STRING			    U("text")
	#define S3MB_JSON_FIELD_ATT_TYPE_DATE				U("date")
	#define S3MB_JSON_FIELD_ATT_TYPE_TIME				U("time")
	#define S3MB_JSON_FIELD_ATT_TYPE_TIMESTAMP		    U("timestamp")

	#define S3MB_JSON_ATT_RECORDS						U("records")
	#define S3MB_JSON_ATT_RECORD						U("record")
	#define S3MB_JSON_ATT_VALUES						U("values")
	#define S3MB_JSON_ATT_FIELD					    	U("field")
	#define S3MB_JSON_ATT_ATT_ID						U("id")
	#define S3MB_JSON_ATT_ATT_NAME					    U("name")
	#define S3MB_JSON_ATT_SMID							U("SmID")
	// ============================================================================

	// ==================================文件名称==================================
	#define FILE_NAME_ATTINFO							U("attribute.json")
	// ============================================================================

	// ==================================后缀名称==================================
	#define FILE_EXT_S3MB								U(".s3mb")
	#define FILE_EXT_S3MD								U(".s3md")
	#define FILE_EXT_JSON								U(".json")
	#define FILE_EXT_SCP								U(".scp")
	// ============================================================================

	// ==================================壳==================================
	#define S3MB_SHELL_PAGEDLOD							U("PagedLOD")
	#define S3MB_SHELL_GEODE							U("Geode")
	// ============================================================================

	static union
	{
		int nTest;
		char cTest[sizeof(int)];
	}un_bigendina = { 1 };
	#define  ISBIGENDIAN (un_bigendina.cTest[0] != 1)

	// 将一个数值限制到一定范围内. 
	template <typename T>
	T Clamp(T val, T minval, T maxval)
	{
		if (minval > maxval)
		{
			T temp = minval;
			minval = maxval;
			maxval = temp;
		}
		return max(min(val, maxval), minval);
	}

	// 属性表
	struct S3MB_API GeodeInfo
	{
		wstring m_strTableName;
		unsigned int m_nID;
	};

	class S3MB_API Rect2D
	{
	public:
		// 构造函数一个二维矩形对象
		Rect2D() { left = right = top = bottom = 0.0; }
		Rect2D(double dLeft, double dTop, double dRight, double dBottom)
			:left(dLeft), top(dTop), right(dRight), bottom(dBottom) {}
	public:
		// 矩形左边界
		double left;
		// 矩形上边界
		double top;
		// 矩形右边界。left<right
		double right;
		// 矩形下边界。bottom < top
		double bottom;
	};

	// UV两个方向运动的参数
	struct S3MB_API UVMotionParameter
	{
		// U方向的初始值
		float m_fStartU;
		// V方向的初始值
		float m_fStartV;
		// U方向的速度
		float m_fSpeedU;
		// V方向的速度
		float m_fSpeedV;
		// 周期
		float m_fPeriod;

		UVMotionParameter()
		{
			m_fStartU = 0.0;
			m_fStartV = 0.0;
			m_fSpeedU = 0.0;
			m_fSpeedV = 0.0;
			m_fPeriod = 0.0;
		}

		UVMotionParameter(float fStartU, float fStartV, float fSpeedU, float fSpeedV, float fPeriod)
		{
			m_fStartU = fStartU;
			m_fStartV = fStartV;
			m_fSpeedU = fSpeedU;
			m_fSpeedV = fSpeedV;
			m_fPeriod = fPeriod;
		}

		UVMotionParameter(const UVMotionParameter& other)
			:m_fStartU(other.m_fStartU),
			m_fStartV(other.m_fStartV), 
			m_fSpeedU(other.m_fSpeedU),
			m_fSpeedV(other.m_fSpeedV),
			m_fPeriod(other.m_fPeriod)
		{
		}

		UVMotionParameter & operator=(const UVMotionParameter& other)
		{
			m_fStartU = other.m_fStartU;
			m_fStartV = other.m_fStartV;
			m_fSpeedU = other.m_fSpeedU;
			m_fSpeedV = other.m_fSpeedV;
			m_fPeriod = other.m_fPeriod;
			return *this;
		}

		~UVMotionParameter()
		{
		}
	};

	struct UVWAddressingMode
	{
		TextureAddressingMode u, v, w;
	};

	// 一个调色板信息
	struct PaletteEntry
	{
		unsigned char peRed;      // 红色的比重
		unsigned char peGreen;    // 绿色的比重
		unsigned char peBlue;     // 蓝色的比重
		unsigned char peFlags;    // 标识符

		PaletteEntry()
		{
			peRed = 0;
			peGreen = 0;
			peBlue = 0;
			peFlags = 0;
		}
	};

	struct BinaryVal {
		// 存储 Binary 指针或者 String指针
		void* pVal;
		// 存储Binary 数据流的大小 以字节为单位。
		unsigned int nSize;
	};

	// 变体联合
	union VarValue
	{
		unsigned char bVal;
		short sVal;
		int	iVal;
		long  lVal;
		float fVal;
		double dVal;
		double   tmVal; // S3MBTime的内部实现
		struct BinaryVal binVal;
	};

	enum S3MBVertexOptions
	{
		SVO_HasIDInfo = 1,		// 带ID信息
	};

	struct SelectInfo
	{
		int m_nVertexColorCount;
		int m_nVertexColorOffset;
		SelectInfo()
		{
			m_nVertexColorCount = 0;
			m_nVertexColorOffset = 0;
		}
	};
	struct ExtensionObjectValue
	{
		// 字符串
		wstring m_strName;
		wstring m_strType;
		// 双精度
		double m_dMax;
		double m_dMin;
	};

	struct FieldDefine
	{
		// 字段名称。
		wstring m_strName;

		// 字段类型。
		FieldType m_nType;

		// 字段长度。
		int m_nSize;

		// 是否是必填字段。
		bool m_bRequired;

		// 默认构造函数
		FieldDefine()
		{
			m_nType = FT_UnKnown;
			m_nSize = 0;
			m_bRequired = false;
		}
		// 重载=
		const FieldDefine& operator =(const FieldDefine& fieldDefine)
		{
			m_strName = fieldDefine.m_strName;
			m_nType = fieldDefine.m_nType;
			m_nSize = fieldDefine.m_nSize;
			m_bRequired = fieldDefine.m_bRequired;
			return *this;
		}
	};

	struct CategoryDescript
	{
		// 顶点属性或分类信息含义描述信息
		wstring m_strCategory;
		// 顶点属性或分类信息数值范围
		std::vector<double> m_vecRangeMin;
		std::vector<double> m_vecRangeMax;

		// 顶点属性或分类信息的维度
		int m_nDimension;
	};

	inline bool Equal(double dValue1, double dValue2)
	{
		if (IS0(dValue1)) 
		{
			return IS0(dValue2);
		}
		else if (IS0(dValue2)) 
		{
			return IS0(dValue1);
		}

		double dMaxValue = MAX(ABS(dValue1), ABS(dValue2));
		double dAbsolute = dValue1 - dValue2;
		return ((dAbsolute >= (dMaxValue * NEP)) && (dAbsolute <= (dMaxValue * EP)));
	}

#ifndef WIN32
    inline std::wstring string_to_wstring(const std::string& str)
	{
        std::wstring ret;
        std::mbstate_t state = {};
        const char* temp_str = str.c_str();
        size_t len = std::mbsrtowcs(nullptr, &temp_str, 0, &state);
        if(static_cast<size_t>(-1) != len){
            std::unique_ptr<wchar_t[]> buff(new wchar_t[len+1]);
            len = std::mbsrtowcs(buff.get(), &temp_str, len, &state);
            if(static_cast<size_t>(-1) != len){
                ret.assign(buff.get(), len);
            }
        }
        return ret;
	}
#endif
}

#endif
