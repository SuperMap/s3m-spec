#pragma once
#include "JsonParser.h"
#include <iostream>
#include "StringHelper.h"
#include <assert.h>
#include "S3mbVariant.h"
#include <map>
using namespace std;
namespace S3MB
{
#define SCPS_EXT_PROFILE							U(".scp")
#define SCPS_EXT_PAGEDLODTREE						U(".json")				//Tile_xxx.json
#define SCPS_ATTINFO_FILENAME						U("attribute.json") //属性信息，固定名字
#define SCPS_EXT_ATTDATA							U(".s3md")			//Tile_xxx.s3md
#define SCPS_EXT_S3MB								U(".s3mb")			//Tile_xxx.s3mb

#define SCPS_XMLHEADER								U("Spatial3DModel")

	//SCP 标签名称定义
#define SCPS_ASSET									U("s3m:Asset")
#define SCPS_VERSION								U("s3m:Version")
#define SCPS_DATATYPE								U("s3m:DataType")				//倾斜、精模、点云、管线、矢量
#define SCPS_PYRAMIDSPLITTYPE						U("s3m:PyramidSplitType")	//Octree		QuadTree
#define SCPS_LODTYPE								U("s3m:LodType")			//LOD类型：添加 or 替换
#define SCPS_POSITION								U("s3m:Position")				//位置点
#define SCPS_GEOBOUNDS								U("s3m:GeoBounds")			//地理范围
#define SCPS_HEIGHTRANGE							U("s3m:HeightRange")		//高度范围
#define SCPS_WDESCRIPT								U("s3m:WDescript")				//w位的描述
#define SCPS_WCATEGORY								U("s3m:Category")				//w位的含义
#define SCPS_WRANGE									U("s3m:Range")					//w位取值范围
#define SCPS_PRJCOORD								U("s3m:CRS")						//坐标系

#define SCPS_TILES									U("s3m:Tiles")
#define SCPS_TILE									U("s3m:Tile")
#define SCPS_TILE_URL								U("s3m:Url")
#define SCPS_TILE_BBOX								U("s3m:Boundingbox")

//SCP的扩展标签
#define SCPS_EXTENSIONS								U("s3m:Extensions")
#define SCPS_ATTACH_FILES							U("s3m:AttachFiles")
#define SCPS_ATTACH_FILE							U("s3m:AttachFile")
//动态拉伸 高度存纹理坐标里了

//SCP标签枚举值
#define SCPS_ASSET_VALUE							U("SuperMap")
#define SCPS_UNKNOWN								U("Unknown")
#define SCPS_DATATYPE_OBLIPHOTO						U("ObliquePhotogrammetry")		//倾斜摄影数据
#define SCPS_DATATYPE_PNTCLOUD						U("PointCloud")	//点云
#define SCPS_DATATYPE_BIM							U("BIM")				//BIM
#define SCPS_DATATYPE_VECTOR						U("Vector")			//矢量
#define SCPS_DATATYPE_PIPELINE						U("PipeLine")		// 管线
#define SCPS_DATATYPE_INSTANCEMODEL					U("InstanceModel") // 点外挂实例化

#define SCPS_PYRAMIDSPLITTYPE_OCTREE				U("Octree")			//八叉树
#define SCPS_PYRAMIDSPLITTYPE_QUADTREE				U("QuadTree")	//四叉树

#define SCPS_LODTYPE_ADD							U("Add")
#define SCPS_LODTYPE_REPLACE						U("Replace")

#define S3MB_LODINFO_TREEEXPORT						U("lodTreeExport")
#define S3MB_LODINFO_ROOTTILE						U("rootTile")
#define S3MB_LODINFO_TILE							U("tile")
#define S3MB_LODINFO_TILEINFO						U("tileInfo")
#define S3MB_LODINFO_NAME							U("name")
#define S3MB_LODINFO_STATUS							U("status")
#define S3MB_LODINFO_LODCOUNT						U("lodCount")
#define S3MB_LODINFO_TILESCOUNT						U("tilesCount")
#define S3MB_LODINFO_LODNUM							U("lodNum")
#define S3MB_LODINFO_MODELPATH						U("modelPath")
#define S3MB_LODINFO_RANGEMODE						U("rangeMode")
#define S3MB_LODINFO_RANGEMODE_PIXELSIZE			U("pixelSizeOnScreen")
#define S3MB_LODINFO_RANGEMODE_DISTANCE				U("distanceFromEvePoint")
#define S3MB_LODINFO_RANGEVALUE						U("rangeValue")
#define S3MB_LODINFO_BBOX							U("boundingBox")
#define S3MB_LODINFO_CHILDREN						U("children")

#define S3MB_LAYERINFOS								U("s3m:LayerInfos")
#define S3MB_LAYERINFO								U("s3m:LayerInfo")
#define S3MB_LAYERINFO_NAME							U("s3m:LayerName")
#define S3MB_LAYERINFO_IDRANGE						U("s3m:IDRange")
#define S3MB_LAYERINFO_FIELDINFOS					U("s3m:Fieldinfos")
#define S3MB_LAYERINFO_FIELDINFO					U("s3m:Fieldinfo")
#define S3MB_FIELD_ATT_NAME							U("Name")
#define S3MB_FIELD_ATT_ALIAS						U("Alias")
#define S3MB_FIELD_ATT_TYPE							U("Type")
#define S3MB_FIELD_ATT_SIZE							U("Size")
#define S3MB_FIELD_ATT_ISREQUIRED					U("IsRequired")

#define S3MB_ATTRIBUTE_DATA							U("s3m:AttributeData")
#define S3MB_ATT_LAYER								U("s3m:Layer")
#define S3MB_ATT_RECORDS							U("s3m:Records")
#define S3MB_ATT_RECORD								U("s3m:Record")
#define S3MB_ATT_Values								U("s3m:Values")
#define S3MB_ATT_Field								U("s3m:Field")
#define S3MB_ATT_ATT_ID								U("ID")
#define S3MB_ATT_ATT_Name							U("Name")

#define S3MB_X										U("s3m:X")
#define S3MB_Y										U("s3m:Y")
#define S3MB_Z										U("s3m:Z")
#define S3MB_TRUE									U("true")
#define S3MB_FALSE									U("false")

#define S3MB_ATT_X									U("X")
#define S3MB_ATT_Y									U("Y")
#define S3MB_ATT_Z									U("Z")

#define S3MB_LEFT									U("s3m:Left")
#define S3MB_TOP									U("s3m:Top")
#define S3MB_RIGHT									U("s3m:Right")
#define S3MB_BOTTOM									U("s3m:Bottom")
#define S3MB_MAX									U("s3m:Max")
#define S3MB_MIN									U("s3m:Min")

#define S3MB_BSHPERE_CENTER			             	U("s3m:Center")
#define S3MB_BSHPERE_RADIUS				            U("s3m:Radius")

// 升级后S3MB属性标签
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
#define S3MB_JSON_FIELD_ATT_TYPE_WCHAR		    	U("wchar")
#define S3MB_JSON_FIELD_ATT_TYPE_STRING			    U("text")
#define S3MB_JSON_FIELD_ATT_TYPE_NTEXT			    U("ntext")
#define S3MB_JSON_FIELD_ATT_TYPE_DATE				U("date")
#define S3MB_JSON_FIELD_ATT_TYPE_TIME				U("time")
#define S3MB_JSON_FIELD_ATT_TYPE_TIMESTAMP		    U("timestamp")
#define UGS3MB_JSON_FIELD_ATT_TYPE_GEOMETRY			U("geometry")  	// 几何数据类型
#define UGS3MB_JSON_FIELD_ATT_TYPE_JSONB			U("jsonb")			// 用作Pg中的Jsonb字段类型
// 点云
#define S3MB_JSON_CLASSIFICATIONINFOS				U("classificationInfos")
#define S3MB_JSON_CLASSIFICATION_ATT_NAME			U("name")
#define S3MB_JSON_CLASSIFICATION_ATT_ID			    U("id")
#define S3MB_JSON_CLASSIFICATION_ATT_POINTNUM		U("pointCount")

#define S3MB_JSON_ATT_RECORDS						U("records")
#define S3MB_JSON_ATT_RECORD						U("record")
#define S3MB_JSON_ATT_VALUES						U("values")
#define S3MB_JSON_ATT_FIELD					    	U("field")
#define S3MB_JSON_ATT_ATT_ID						U("id")
#define S3MB_JSON_ATT_ATT_NAME					    U("name")

#define SCPS_JSON_ASSET								U("asset")
#define SCPS_JSON_VERSION							U("version")
#define SCPS_JSON_DATATYPE							U("dataType")			//倾斜、精模、点云、管线、矢量
#define SCPS_JSON_PYRAMIDSPLITTYPE					U("pyramidSplitType")		//Octree		QuadTree
#define SCPS_JSON_LODTYPE							U("lodType")			//LOD类型：添加 or 替换
#define SCPS_JSON_POSITION							U("position")			//位置点
#define SCPS_JSON_GEOBOUNDS							U("geoBounds")				//地理范围
#define SCPS_JSON_HEIGHTRANGE						U("heightRange")		//高度范围
#define SCPS_JSON_HEIGHTRANGE_MIN					U("min")
#define SCPS_JSON_HEIGHTRANGE_MAX					U("max")
#define SCPS_JSON_WDESCRIPT							U("wDescript")				//w位的描述
#define SCPS_JSON_WCATEGORY							U("category")			//w位的含义
#define SCPS_JSON_WRANGE							U("range")				//w值的范围
#define SCPS_JSON_WRANGE_MIN						U("min")
#define SCPS_JSON_WRANGE_MAX						U("max")
#define SCPS_JSON_PRJCOORD							U("crs")				//坐标系
#define SCPS_JSON_PRJCOORD_EPSG				    	U("epsg:")				//前缀

#define SCPS_JSON_UNITS								U("units")
#define SCPS_JSON_UNITS_METER						U("Meter")
#define SCPS_JSON_UNITS_DEGREE						U("Degree")
#define SCPS_JSON_TILES								U("tiles")
#define SCPS_JSON_TILE								U("tile")
#define SCPS_JSON_TILE_URL							U("url")
#define SCPS_JSON_TILE_BBOX							U("boundingbox")

#define SCPS_JSON_EXTENSIONS						U("extensions")
#define SCPS_JSON_EXTENSIONS_TYPE					U("type")
#define SCPS_JSON_EXTENSIONS_NAME					U("name")
#define SCPS_JSON_EXTENSIONS_NEU				    U("neu")

#define SCPS_JSON_ATTACH_FILES						U("attachFiles")
#define SCPS_JSON_ATTACH_FILE						U("attachFile")
#define SCPS_JSON_POINTCLOUD_LAYERS			        U("pointCloudLayers")
#define SCPS_JSON_POINTCLOUD_LAYER		            U("layer")
#define SCPS_JSON_POINTCLOUD_LAYERNAME		        U("layerName")
#define SCPS_JSON_POINTCLOUD_LAYERBOUNDS	        U("layerBounds")

#define  SCPS_JSON_GLOBAL_LEVELS					U("levels")
#define  SCPS_JSON_GLOBAL_LEVEL						U("level")

#define SCPS_JSON_VOLS								U("vol")
#define SCPS_JSON_VOLS_NAME							U("volName")
#define SCPS_JSON_VOLS_TYPE							U("volType")
#define SCPS_JSON_VOLS_RANGE_MAX					U("volRangeMax")
#define SCPS_JSON_VOLS_RANGE_MIN					U("volRangeMin")

#define S3MB_JSON_X								    U("x")
#define S3MB_JSON_Y							    	U("y")
#define S3MB_JSON_Z								    U("z")

	class S3MB_API S3mbFieldInfo
	{
	public:
		//! \brief 字段类型
		enum FieldType
		{
			//! \brief 无效的字段类型。
			UnKnown = 0,
			//! \brief 布尔值，单字节，true,false。
			Boolean = 1,
			//! \brief 无符号单字节，0-255。 
			Byte = 2,
			//! \brief 短整型，2字节。
			INT16 = 3,
			//! \brief 长整型，4字节。
			INT32 = 4,
			//! \brief 长整型，8字节。
			INT64 = 16,
			//! \brief 单精度浮点型，4字节。
			Float = 6,
			//! \brief 双精度浮点型，8字节。
			Double = 7,
			//! \brief 日期型，年、月、日，不带时间。
			Date = 8,
			//! \brief 固定长度二进制型，需指定长度。
			Binary = 9,
			//! \brief 不定长字符串型。
			Text = 10,
			//! \brief 不定长二进制类型。
			LongBinary = 11,
			//! \brief 定长字符串型，需指定长度。
			Char = 18,
			//! \brief 时间型，小时、分、秒，不带日期。
			Time = 22,
			//! \brief 时间戳型，年、月、日、小时、分、秒。
			TimeStamp = 23,
			//! \brief 宽字节不定长字符串类型。
			NText = 127,
			//! \brief 几何数据类型。
			Geometry = 128,
			//用作Pg中的Jsonb字段类型
			JsonB = 129
		};
		S3mbFieldInfo() {}
		~S3mbFieldInfo() {}
	public:
		//! \brief 字段类型。
		FieldType m_nType;
		//! \brief 字段名称。
		wstring m_strName;
		//! \brief 字段的外键名。
		wstring m_strForeignName;
		//! \brief 字段长度。
		int m_nSize;
		//! \brief 是否是必填字段。
		bool m_bRequired;
		//! \brief 字段默认值
		wstring m_strDefaultValue;
	};

	class S3MB_API S3mbFieldInfos
	{
	public:
		//! \brief 默认构造函数
		S3mbFieldInfos() {}
		//! \brief 默认析构函数
		~S3mbFieldInfos() {
			m_array.clear();
			if (m_array.capacity() > m_array.size()) {
				std::vector<S3mbFieldInfo*> vctTemp(m_array.begin(), m_array.end());
				m_array.swap(vctTemp);
			}
		};
		void add(S3mbFieldInfo* newFieldInfo);
		size_t GetSize() { return m_array.size(); }
		S3mbFieldInfo* GetAt(int nIndex) { assert(nIndex >= 0 && m_array.size() > nIndex); return m_array.at(nIndex); }
	private:
		std::vector<S3mbFieldInfo*> m_array;
	};

	class S3MB_API LayerCacheInfo
	{
	public:
		//! \brief 数据集信息
		wstring m_strDatasetName;
		//! \brief 字段信息
		S3mbFieldInfos m_fieldInfos;
		//! \brief ID范围
		std::pair<int, int> m_pairIDRange;
	};

	class S3MB_API Feature
	{
	public:
		struct FieldDefine
		{
			//! \brief 字段名称。
			wstring m_strName;

			//! \brief 字段类型。
			S3mbFieldInfo::FieldType m_nType;

			//! \brief 字段长度。
			int m_nSize;

			//! \brief 是否是必填字段。
			bool m_bRequired;

			//! \brief 默认构造函数
			FieldDefine()
			{
				m_nType = S3mbFieldInfo::UnKnown;
				m_nSize = 0;
				m_bRequired = false;
			}
			//! \brief 重载=
			const FieldDefine& operator =(const FieldDefine& fieldDefine)
			{
				m_strName = fieldDefine.m_strName;
				m_nType = fieldDefine.m_nType;
				m_nSize = fieldDefine.m_nSize;
				m_bRequired = fieldDefine.m_bRequired;
				return *this;
			}
		};
		void SetFieldInfos(S3mbFieldInfos& fieldInfos);

		//! \brief 根据字段序号获取字段值。
		//! \param nIndex 字段序号[in]。
		//##ModelId=4820305003C8
		virtual bool GetValue(int nIndex, S3mbVariant& varValue) const;
		//! \brief 根据字段名获取字段值。
		//! \param strFieldName 字段名[in]。
		//##ModelId=4820305003CC
		virtual bool GetValue(const wstring& strFieldName, S3mbVariant& varValue) const;

	public:
		//! \brief 特征要素所在矢量数据集表中的ID。
		int m_nID;
		vector<FieldDefine> m_fieldDefines;
		vector<unsigned char*> m_fieldValues;
	};

	class S3MB_API S3MBLayerInfos
	{
	public:
		//! \brief 从s3md文件中加载属性值
		static bool LoadAttributeDataFromFile(const wstring& strAttFilePath, std::map<unsigned int, Feature*>& mapFeature);
		//! \brief 从s3md数据流中加载属性值
		static bool LoadAttributeDataFromStream(void* buffer,int bufferSize, std::map<unsigned int, Feature*>& mapFeature);
		//从Json文件中加载属性值
		static bool AttributeDataFromJson(std::map<unsigned int, Feature*>& mapFeature, JsonValue& jsonValue);
		//从Json字符串中加载FeatureValue
		static bool GetFeatureValuesFromJson(Feature* pFeature, JsonValue& jsonValue);
		//读取Json文件,获取图层信息
		static bool LoadLayerInfoFromJson(wstring strJson, vector<LayerCacheInfo>& vecLayerInfos);
		//读取Json数据流，获取图层信息
		static bool LoadLayerInfoFromStream(void* buffer, int bufferSize, vector<LayerCacheInfo>& vecLayerInfos);
		//读取IDRange
		static void GetIDRangeFromJson(JsonValue& jsonValue, int& nMin, int& nMax);
		//! \brief mapFeature 里的ID是否在范围内
		static bool IsIDExisted(std::map<unsigned int, Feature*>& mapFeature, int nBeginID, int nEndID);
		//读取FieldInfos
		static void GetFieldInfosFromJson(S3mbFieldInfos& fieldInfos, JsonValue& jsonValue);
		//获取FieldInfoType
		static S3mbFieldInfo::FieldType GetFieldInfoType(wstring strType);
	};
}
