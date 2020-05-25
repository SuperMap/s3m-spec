#pragma once
#include "JsonParser.h"
#include <iostream>
#include "StringHelper.h"
#include "zlib/include/zconf.h"
#include "zlib/include/zlib.h"
#include <assert.h>
#include "S3mbVariant.h"
using namespace std;
namespace S3MB
{
#define SCPS_EXT_PROFILE							".scp"
#define SCPS_EXT_PAGEDLODTREE						".json"				//Tile_xxx.json
#define SCPS_ATTINFO_FILENAME						"attribute.json" //属性信息，固定名字
#define SCPS_EXT_ATTDATA							".s3md"			//Tile_xxx.s3md
#define SCPS_EXT_S3MB								".s3mb"			//Tile_xxx.s3mb

#define SCPS_XMLHEADER								"Spatial3DModel"

	//SCP 标签名称定义
#define SCPS_ASSET									"s3m:Asset"
#define SCPS_VERSION								"s3m:Version"
#define SCPS_DATATYPE								"s3m:DataType"				//倾斜、精模、点云、管线、矢量
#define SCPS_PYRAMIDSPLITTYPE						"s3m:PyramidSplitType"	//Octree		QuadTree
#define SCPS_LODTYPE								"s3m:LodType"				//LOD类型：添加 or 替换
#define SCPS_POSITION								"s3m:Position"				//位置点
#define SCPS_GEOBOUNDS								"s3m:GeoBounds"			//地理范围
#define SCPS_HEIGHTRANGE							"s3m:HeightRange"			//高度范围
#define SCPS_WDESCRIPT								"s3m:WDescript"				//w位的描述
#define SCPS_WCATEGORY								"s3m:Category"				//w位的含义
#define SCPS_WRANGE									"s3m:Range"					//w位取值范围
#define SCPS_PRJCOORD								"s3m:CRS"						//坐标系

#define SCPS_TILES									"s3m:Tiles"
#define SCPS_TILE									"s3m:Tile"
#define SCPS_TILE_URL								"s3m:Url"
#define SCPS_TILE_BBOX								"s3m:Boundingbox"

//SCP的扩展标签
#define SCPS_EXTENSIONS								"s3m:Extensions"
#define SCPS_ATTACH_FILES							"s3m:AttachFiles"
#define SCPS_ATTACH_FILE							"s3m:AttachFile"
//动态拉伸 高度存纹理坐标里了

//SCP标签枚举值
#define SCPS_ASSET_VALUE							"SuperMap"
#define SCPS_UNKNOWN								"Unknown"
#define SCPS_DATATYPE_OBLIPHOTO						"ObliquePhotogrammetry"		//倾斜摄影数据
#define SCPS_DATATYPE_PNTCLOUD						"PointCloud"	//点云
#define SCPS_DATATYPE_BIM							"BIM"				//BIM
#define SCPS_DATATYPE_VECTOR						"Vector"			//矢量
#define SCPS_DATATYPE_PIPELINE						"PipeLine"		// 管线
#define SCPS_DATATYPE_INSTANCEMODEL					"InstanceModel" // 点外挂实例化

#define SCPS_PYRAMIDSPLITTYPE_OCTREE				"Octree"			//八叉树
#define SCPS_PYRAMIDSPLITTYPE_QUADTREE				"QuadTree"	//四叉树

#define SCPS_LODTYPE_ADD							"Add"
#define SCPS_LODTYPE_REPLACE						"Replace"

#define S3MB_LODINFO_TREEEXPORT						"lodTreeExport"
#define S3MB_LODINFO_ROOTTILE						"rootTile"
#define S3MB_LODINFO_TILE							"tile"
#define S3MB_LODINFO_TILEINFO						"tileInfo"
#define S3MB_LODINFO_NAME							"name"
#define S3MB_LODINFO_STATUS							"status"
#define S3MB_LODINFO_LODCOUNT						"lodCount"
#define S3MB_LODINFO_TILESCOUNT						"tilesCount"
#define S3MB_LODINFO_LODNUM							"lodNum"
#define S3MB_LODINFO_MODELPATH						"modelPath"
#define S3MB_LODINFO_RANGEMODE						"rangeMode"
#define S3MB_LODINFO_RANGEMODE_PIXELSIZE			"pixelSizeOnScreen"
#define S3MB_LODINFO_RANGEMODE_DISTANCE				"distanceFromEvePoint"
#define S3MB_LODINFO_RANGEVALUE						"rangeValue"
#define S3MB_LODINFO_BBOX							"boundingBox"
#define S3MB_LODINFO_CHILDREN						"children"

#define S3MB_LAYERINFOS								"s3m:LayerInfos"
#define S3MB_LAYERINFO								"s3m:LayerInfo"
#define S3MB_LAYERINFO_NAME							"s3m:LayerName"
#define S3MB_LAYERINFO_IDRANGE						"s3m:IDRange"
#define S3MB_LAYERINFO_FIELDINFOS					"s3m:Fieldinfos"
#define S3MB_LAYERINFO_FIELDINFO					"s3m:Fieldinfo"
#define S3MB_FIELD_ATT_NAME							"Name"
#define S3MB_FIELD_ATT_ALIAS						"Alias"
#define S3MB_FIELD_ATT_TYPE							"Type"
#define S3MB_FIELD_ATT_SIZE							"Size"
#define S3MB_FIELD_ATT_ISREQUIRED					"IsRequired"

#define S3MB_ATTRIBUTE_DATA							"s3m:AttributeData"
#define S3MB_ATT_LAYER								"s3m:Layer"
#define S3MB_ATT_RECORDS							"s3m:Records"
#define S3MB_ATT_RECORD								"s3m:Record"
#define S3MB_ATT_Values								"s3m:Values"
#define S3MB_ATT_Field								"s3m:Field"
#define S3MB_ATT_ATT_ID								"ID"
#define S3MB_ATT_ATT_Name							"Name"

#define S3MB_X										"s3m:X"
#define S3MB_Y										"s3m:Y"
#define S3MB_Z										"s3m:Z"
#define S3MB_TRUE									"true"
#define S3MB_FALSE									"false"

#define S3MB_ATT_X									"X"
#define S3MB_ATT_Y									"Y"
#define S3MB_ATT_Z									"Z"

#define S3MB_LEFT									"s3m:Left"
#define S3MB_TOP									"s3m:Top"
#define S3MB_RIGHT									"s3m:Right"
#define S3MB_BOTTOM									"s3m:Bottom"
#define S3MB_MAX									"s3m:Max"
#define S3MB_MIN									"s3m:Min"

#define S3MB_BSHPERE_CENTER			             	"s3m:Center"
#define S3MB_BSHPERE_RADIUS				            "s3m:Radius"

// 升级后S3MB属性标签
#define S3MB_JSON_LAYERINFOS						"layerInfos"
#define S3MB_JSON_LAYERINFO					    	"LayerInfo"
#define S3MB_JSON_LAYERINFO_NAME					"layerName"
#define S3MB_JSON_LAYERINFO_IDRANGE				    "idRange"
#define S3MB_JSON_LAYERINFO_IDRANGE_MIN				"minID"
#define S3MB_JSON_LAYERINFO_IDRANGE_MAX				"maxID"
#define S3MB_JSON_LAYERINFO_FIELDINFOS			    "fieldInfos"
#define S3MB_JSON_LAYERINFO_FIELDINFO				"fieldInfo"
#define S3MB_JSON_FIELD_ATT_NAME					"name"
#define S3MB_JSON_FIELD_ATT_ALIAS					"alias"
#define S3MB_JSON_FIELD_ATT_TYPE					"type"
#define S3MB_JSON_FIELD_ATT_SIZE					"size"
#define S3MB_JSON_FIELD_ATT_ISREQUIRED			    "isRequired"

#define S3MB_JSON_FIELD_ATT_TYPE_BOOL			    "bool"
#define S3MB_JSON_FIELD_ATT_TYPE_INT16			    "int16"
#define S3MB_JSON_FIELD_ATT_TYPE_UINT16			    "uint16"
#define S3MB_JSON_FIELD_ATT_TYPE_INT32			    "int32"
#define S3MB_JSON_FIELD_ATT_TYPE_UINT32			    "uint32"
#define S3MB_JSON_FIELD_ATT_TYPE_INT64			    "int64"
#define S3MB_JSON_FIELD_ATT_TYPE_FLOAT			    "float"
#define S3MB_JSON_FIELD_ATT_TYPE_DOUBLE		        "double"
#define S3MB_JSON_FIELD_ATT_TYPE_WCHAR		    	"wchar"
#define S3MB_JSON_FIELD_ATT_TYPE_STRING			    "String"
#define S3MB_JSON_FIELD_ATT_TYPE_NTEXT			    "ntext"
#define S3MB_JSON_FIELD_ATT_TYPE_DATE				"date"
#define S3MB_JSON_FIELD_ATT_TYPE_TIME				"time"
#define S3MB_JSON_FIELD_ATT_TYPE_TIMESTAMP		    "timestamp"
// 点云
#define S3MB_JSON_CLASSIFICATIONINFOS				"classificationInfos"
#define S3MB_JSON_CLASSIFICATION_ATT_NAME			"name"
#define S3MB_JSON_CLASSIFICATION_ATT_ID			    "id"
#define S3MB_JSON_CLASSIFICATION_ATT_POINTNUM		"pointCount"

#define S3MB_JSON_ATT_RECORDS						"records"
#define S3MB_JSON_ATT_RECORD						"record"
#define S3MB_JSON_ATT_VALUES						"values"
#define S3MB_JSON_ATT_FIELD					    	"field"
#define S3MB_JSON_ATT_ATT_ID						"id"
#define S3MB_JSON_ATT_ATT_NAME					    "name"

#define SCPS_JSON_ASSET								"asset"
#define SCPS_JSON_VERSION							"version"
#define SCPS_JSON_DATATYPE							"dataType"				//倾斜、精模、点云、管线、矢量
#define SCPS_JSON_PYRAMIDSPLITTYPE					"pyramidSplitType"		//Octree		QuadTree
#define SCPS_JSON_LODTYPE							"lodType"			//LOD类型：添加 or 替换
#define SCPS_JSON_POSITION							"position"			//位置点
#define SCPS_JSON_GEOBOUNDS							"geoBounds"				//地理范围
#define SCPS_JSON_HEIGHTRANGE						"heightRange"		//高度范围
#define SCPS_JSON_HEIGHTRANGE_MIN					"min"
#define SCPS_JSON_HEIGHTRANGE_MAX					"max"
#define SCPS_JSON_WDESCRIPT							"wDescript"				//w位的描述
#define SCPS_JSON_WCATEGORY							"category"			//w位的含义
#define SCPS_JSON_WRANGE							"range"				//w值的范围
#define SCPS_JSON_WRANGE_MIN						"min"
#define SCPS_JSON_WRANGE_MAX						"max"
#define SCPS_JSON_PRJCOORD							"crs"				//坐标系
#define SCPS_JSON_PRJCOORD_EPSG				    	"epsg:"				//前缀

#define SCPS_JSON_UNITS								"units"
#define SCPS_JSON_UNITS_METER						"Meter"
#define SCPS_JSON_UNITS_DEGREE						"Degree"
#define SCPS_JSON_TILES								"tiles"
#define SCPS_JSON_TILE								"tile"
#define SCPS_JSON_TILE_URL							"url"
#define SCPS_JSON_TILE_BBOX							"boundingbox"

#define SCPS_JSON_EXTENSIONS						"extensions"
#define SCPS_JSON_EXTENSIONS_TYPE					"type"
#define SCPS_JSON_EXTENSIONS_NAME					"name"
#define SCPS_JSON_EXTENSIONS_NEU				    "neu"

#define SCPS_JSON_ATTACH_FILES						"attachFiles"
#define SCPS_JSON_ATTACH_FILE						"attachFile"
#define SCPS_JSON_POINTCLOUD_LAYERS			        "pointCloudLayers"
#define SCPS_JSON_POINTCLOUD_LAYER		            "layer"
#define SCPS_JSON_POINTCLOUD_LAYERNAME		        "layerName"
#define SCPS_JSON_POINTCLOUD_LAYERBOUNDS	        "layerBounds"

#define  SCPS_JSON_GLOBAL_LEVELS					"levels"
#define  SCPS_JSON_GLOBAL_LEVEL						"level"

#define SCPS_JSON_VOLS								"vol"
#define SCPS_JSON_VOLS_NAME							"volName"
#define SCPS_JSON_VOLS_TYPE							"volType"
#define SCPS_JSON_VOLS_RANGE_MAX					"volRangeMax"
#define SCPS_JSON_VOLS_RANGE_MIN					"volRangeMin"

#define S3MB_JSON_X								    "x"
#define S3MB_JSON_Y							    	"y"
#define S3MB_JSON_Z								    "z"

	class S3mbFieldInfo
	{
	public:
		//! \brief 字段类型
		enum FieldType
		{
			//! \brief 无效的字段类型。
			UnKnown = 0,
			//! \brief 布尔值，单字节，true,false。
			Boolean = 1,
			//! \brief 无符号单字李，0-255。 
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
		string m_strName;
		//! \brief 字段的外键名。
		string m_strForeignName;
		//! \brief 字段长度。
		int m_nSize;
		//! \brief 是否是必填字段。
		bool m_bRequired;
		//! \brief 字段默认值
		string m_strDefaultValue;
	};

	class S3mbFieldInfos
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

	class LayerCacheInfo
	{
	public:
		//! \brief 数据集信息
		string m_strDatasetName;
		//! \brief 字段信息
		S3mbFieldInfos m_fieldInfos;
		//! \brief ID范围
		std::pair<int, int> m_pairIDRange;
	};

	class Feature
	{
	public:
		struct FieldDefine
		{
			//! \brief 字段名称。
			string m_strName;

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
		virtual bool GetValue(const string& strFieldName, S3mbVariant& varValue) const;

	public:
		//! \brief 特征要素所在矢量数据集表中的ID。
		int m_nID;
		vector<FieldDefine> m_fieldDefines;
		vector<unsigned char*> m_fieldValues;
	};

	class S3MBLayerInfos
	{
	public:
		//! \brief 从s3md文件中加载属性值
		static bool LoadAttributeDataFromFile(const string& strAttFilePath, std::map<unsigned int, Feature*>& mapFeature);
		//从Json文件中加载属性值
		static bool AttributeDataFromJson(std::map<unsigned int, Feature*>& mapFeature, JsonValue& jsonValue);
		//从Json字符串中加载FeatureValue
		static bool GetFeatureValuesFromJson(Feature* pFeature, JsonValue& jsonValue);
		//读取Json文件
		static bool LoadLayerInfoFromJson(string strJson, vector<LayerCacheInfo>& vecLayerInfos);
		//读取IDRange
		static void GetIDRangeFromJson(JsonValue& jsonValue, int& nMin, int& nMax);
		//! \brief mapFeature 里的ID是否在范围内
		static bool IsIDExisted(std::map<unsigned int, Feature*>& mapFeature, int nBeginID, int nEndID);
		//读取FieldInfos
		static void GetFieldInfosFromJson(S3mbFieldInfos& fieldInfos, JsonValue& jsonValue);
		//获取FieldInfoType
		static S3mbFieldInfo::FieldType GetFieldInfoType(string strType);
	};
}
