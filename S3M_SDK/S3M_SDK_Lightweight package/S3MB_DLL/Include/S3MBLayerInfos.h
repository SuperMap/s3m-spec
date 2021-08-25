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
#define SCPS_EXT_PROFILE							L".scp"
#define SCPS_EXT_PAGEDLODTREE						L".json"				//Tile_xxx.json
#define SCPS_ATTINFO_FILENAME						L"attribute.json" //属性信息，固定名字
#define SCPS_EXT_ATTDATA							L".s3md"			//Tile_xxx.s3md
#define SCPS_EXT_S3MB								L".s3mb"			//Tile_xxx.s3mb

#define SCPS_XMLHEADER								L"Spatial3DModel"

	//SCP 标签名称定义
#define SCPS_ASSET									L"s3m:Asset"
#define SCPS_VERSION								L"s3m:Version"
#define SCPS_DATATYPE								L"s3m:DataType"				//倾斜、精模、点云、管线、矢量
#define SCPS_PYRAMIDSPLITTYPE						L"s3m:PyramidSplitType"	//Octree		QuadTree
#define SCPS_LODTYPE								L"s3m:LodType"				//LOD类型：添加 or 替换
#define SCPS_POSITION								L"s3m:Position"				//位置点
#define SCPS_GEOBOUNDS								L"s3m:GeoBounds"			//地理范围
#define SCPS_HEIGHTRANGE							L"s3m:HeightRange"			//高度范围
#define SCPS_WDESCRIPT								L"s3m:WDescript"				//w位的描述
#define SCPS_WCATEGORY								L"s3m:Category"				//w位的含义
#define SCPS_WRANGE									L"s3m:Range"					//w位取值范围
#define SCPS_PRJCOORD								L"s3m:CRS"						//坐标系

#define SCPS_TILES									L"s3m:Tiles"
#define SCPS_TILE									L"s3m:Tile"
#define SCPS_TILE_URL								L"s3m:Url"
#define SCPS_TILE_BBOX								L"s3m:Boundingbox"

//SCP的扩展标签
#define SCPS_EXTENSIONS								L"s3m:Extensions"
#define SCPS_ATTACH_FILES							L"s3m:AttachFiles"
#define SCPS_ATTACH_FILE							L"s3m:AttachFile"
//动态拉伸 高度存纹理坐标里了

//SCP标签枚举值
#define SCPS_ASSET_VALUE							L"SuperMap"
#define SCPS_UNKNOWN								L"Unknown"
#define SCPS_DATATYPE_OBLIPHOTO						L"ObliquePhotogrammetry"		//倾斜摄影数据
#define SCPS_DATATYPE_PNTCLOUD						L"PointCloud"	//点云
#define SCPS_DATATYPE_BIM							L"BIM"				//BIM
#define SCPS_DATATYPE_VECTOR						L"Vector"			//矢量
#define SCPS_DATATYPE_PIPELINE						L"PipeLine"		// 管线
#define SCPS_DATATYPE_INSTANCEMODEL					L"InstanceModel" // 点外挂实例化

#define SCPS_PYRAMIDSPLITTYPE_OCTREE				L"Octree"			//八叉树
#define SCPS_PYRAMIDSPLITTYPE_QUADTREE				L"QuadTree"	//四叉树

#define SCPS_LODTYPE_ADD							L"Add"
#define SCPS_LODTYPE_REPLACE						L"Replace"

#define S3MB_LODINFO_TREEEXPORT						L"lodTreeExport"
#define S3MB_LODINFO_ROOTTILE						L"rootTile"
#define S3MB_LODINFO_TILE							L"tile"
#define S3MB_LODINFO_TILEINFO						L"tileInfo"
#define S3MB_LODINFO_NAME							L"name"
#define S3MB_LODINFO_STATUS							L"status"
#define S3MB_LODINFO_LODCOUNT						L"lodCount"
#define S3MB_LODINFO_TILESCOUNT						L"tilesCount"
#define S3MB_LODINFO_LODNUM							L"lodNum"
#define S3MB_LODINFO_MODELPATH						L"modelPath"
#define S3MB_LODINFO_RANGEMODE						L"rangeMode"
#define S3MB_LODINFO_RANGEMODE_PIXELSIZE			L"pixelSizeOnScreen"
#define S3MB_LODINFO_RANGEMODE_DISTANCE				L"distanceFromEvePoint"
#define S3MB_LODINFO_RANGEVALUE						L"rangeValue"
#define S3MB_LODINFO_BBOX							L"boundingBox"
#define S3MB_LODINFO_CHILDREN						L"children"

#define S3MB_LAYERINFOS								L"s3m:LayerInfos"
#define S3MB_LAYERINFO								L"s3m:LayerInfo"
#define S3MB_LAYERINFO_NAME							L"s3m:LayerName"
#define S3MB_LAYERINFO_IDRANGE						L"s3m:IDRange"
#define S3MB_LAYERINFO_FIELDINFOS					L"s3m:Fieldinfos"
#define S3MB_LAYERINFO_FIELDINFO					L"s3m:Fieldinfo"
#define S3MB_FIELD_ATT_NAME							L"Name"
#define S3MB_FIELD_ATT_ALIAS						L"Alias"
#define S3MB_FIELD_ATT_TYPE							L"Type"
#define S3MB_FIELD_ATT_SIZE							L"Size"
#define S3MB_FIELD_ATT_ISREQUIRED					L"IsRequired"

#define S3MB_ATTRIBUTE_DATA							L"s3m:AttributeData"
#define S3MB_ATT_LAYER								L"s3m:Layer"
#define S3MB_ATT_RECORDS							L"s3m:Records"
#define S3MB_ATT_RECORD								L"s3m:Record"
#define S3MB_ATT_Values								L"s3m:Values"
#define S3MB_ATT_Field								L"s3m:Field"
#define S3MB_ATT_ATT_ID								L"ID"
#define S3MB_ATT_ATT_Name							L"Name"

#define S3MB_X										L"s3m:X"
#define S3MB_Y										L"s3m:Y"
#define S3MB_Z										L"s3m:Z"
#define S3MB_TRUE									L"true"
#define S3MB_FALSE									L"false"

#define S3MB_ATT_X									L"X"
#define S3MB_ATT_Y									L"Y"
#define S3MB_ATT_Z									L"Z"

#define S3MB_LEFT									L"s3m:Left"
#define S3MB_TOP									L"s3m:Top"
#define S3MB_RIGHT									L"s3m:Right"
#define S3MB_BOTTOM									L"s3m:Bottom"
#define S3MB_MAX									L"s3m:Max"
#define S3MB_MIN									L"s3m:Min"

#define S3MB_BSHPERE_CENTER			             	L"s3m:Center"
#define S3MB_BSHPERE_RADIUS				            L"s3m:Radius"

// 升级后S3MB属性标签
#define S3MB_JSON_LAYERINFOS						L"layerInfos"
#define S3MB_JSON_LAYERINFO					    	L"LayerInfo"
#define S3MB_JSON_LAYERINFO_NAME					L"layerName"
#define S3MB_JSON_LAYERINFO_IDRANGE				    L"idRange"
#define S3MB_JSON_LAYERINFO_IDRANGE_MIN				L"minID"
#define S3MB_JSON_LAYERINFO_IDRANGE_MAX				L"maxID"
#define S3MB_JSON_LAYERINFO_FIELDINFOS			    L"fieldInfos"
#define S3MB_JSON_LAYERINFO_FIELDINFO				L"fieldInfo"
#define S3MB_JSON_FIELD_ATT_NAME					L"name"
#define S3MB_JSON_FIELD_ATT_ALIAS					L"alias"
#define S3MB_JSON_FIELD_ATT_TYPE					L"type"
#define S3MB_JSON_FIELD_ATT_SIZE					L"size"
#define S3MB_JSON_FIELD_ATT_ISREQUIRED			    L"isRequired"

#define S3MB_JSON_FIELD_ATT_TYPE_BOOL			    L"bool"
#define S3MB_JSON_FIELD_ATT_TYPE_INT16			    L"int16"
#define S3MB_JSON_FIELD_ATT_TYPE_UINT16			    L"uint16"
#define S3MB_JSON_FIELD_ATT_TYPE_INT32			    L"int32"
#define S3MB_JSON_FIELD_ATT_TYPE_UINT32			    L"uint32"
#define S3MB_JSON_FIELD_ATT_TYPE_INT64			    L"int64"
#define S3MB_JSON_FIELD_ATT_TYPE_FLOAT			    L"float"
#define S3MB_JSON_FIELD_ATT_TYPE_DOUBLE		        L"double"
#define S3MB_JSON_FIELD_ATT_TYPE_WCHAR		    	L"wchar"
#define S3MB_JSON_FIELD_ATT_TYPE_STRING			    L"text"
#define S3MB_JSON_FIELD_ATT_TYPE_NTEXT			    L"ntext"
#define S3MB_JSON_FIELD_ATT_TYPE_DATE				L"date"
#define S3MB_JSON_FIELD_ATT_TYPE_TIME				L"time"
#define S3MB_JSON_FIELD_ATT_TYPE_TIMESTAMP		    L"timestamp"
#define UGS3MB_JSON_FIELD_ATT_TYPE_GEOMETRY			L"geometry"  	// 几何数据类型
#define UGS3MB_JSON_FIELD_ATT_TYPE_JSONB			L"jsonb"			// 用作Pg中的Jsonb字段类型
// 点云
#define S3MB_JSON_CLASSIFICATIONINFOS				L"classificationInfos"
#define S3MB_JSON_CLASSIFICATION_ATT_NAME			L"name"
#define S3MB_JSON_CLASSIFICATION_ATT_ID			    L"id"
#define S3MB_JSON_CLASSIFICATION_ATT_POINTNUM		L"pointCount"

#define S3MB_JSON_ATT_RECORDS						L"records"
#define S3MB_JSON_ATT_RECORD						L"record"
#define S3MB_JSON_ATT_VALUES						L"values"
#define S3MB_JSON_ATT_FIELD					    	L"field"
#define S3MB_JSON_ATT_ATT_ID						L"id"
#define S3MB_JSON_ATT_ATT_NAME					    L"name"

#define SCPS_JSON_ASSET								L"asset"
#define SCPS_JSON_VERSION							L"version"
#define SCPS_JSON_DATATYPE							L"dataType"				//倾斜、精模、点云、管线、矢量
#define SCPS_JSON_PYRAMIDSPLITTYPE					L"pyramidSplitType"		//Octree		QuadTree
#define SCPS_JSON_LODTYPE							L"lodType"			//LOD类型：添加 or 替换
#define SCPS_JSON_POSITION							L"position"			//位置点
#define SCPS_JSON_GEOBOUNDS							L"geoBounds"				//地理范围
#define SCPS_JSON_HEIGHTRANGE						L"heightRange"		//高度范围
#define SCPS_JSON_HEIGHTRANGE_MIN					L"min"
#define SCPS_JSON_HEIGHTRANGE_MAX					L"max"
#define SCPS_JSON_WDESCRIPT							L"wDescript"				//w位的描述
#define SCPS_JSON_WCATEGORY							L"category"			//w位的含义
#define SCPS_JSON_WRANGE							L"range"				//w值的范围
#define SCPS_JSON_WRANGE_MIN						L"min"
#define SCPS_JSON_WRANGE_MAX						L"max"
#define SCPS_JSON_PRJCOORD							L"crs"				//坐标系
#define SCPS_JSON_PRJCOORD_EPSG				    	L"epsg:"				//前缀

#define SCPS_JSON_UNITS								L"units"
#define SCPS_JSON_UNITS_METER						L"Meter"
#define SCPS_JSON_UNITS_DEGREE						L"Degree"
#define SCPS_JSON_TILES								L"tiles"
#define SCPS_JSON_TILE								L"tile"
#define SCPS_JSON_TILE_URL							L"url"
#define SCPS_JSON_TILE_BBOX							L"boundingbox"

#define SCPS_JSON_EXTENSIONS						L"extensions"
#define SCPS_JSON_EXTENSIONS_TYPE					L"type"
#define SCPS_JSON_EXTENSIONS_NAME					L"name"
#define SCPS_JSON_EXTENSIONS_NEU				    L"neu"

#define SCPS_JSON_ATTACH_FILES						L"attachFiles"
#define SCPS_JSON_ATTACH_FILE						L"attachFile"
#define SCPS_JSON_POINTCLOUD_LAYERS			        L"pointCloudLayers"
#define SCPS_JSON_POINTCLOUD_LAYER		            L"layer"
#define SCPS_JSON_POINTCLOUD_LAYERNAME		        L"layerName"
#define SCPS_JSON_POINTCLOUD_LAYERBOUNDS	        L"layerBounds"

#define  SCPS_JSON_GLOBAL_LEVELS					L"levels"
#define  SCPS_JSON_GLOBAL_LEVEL						L"level"

#define SCPS_JSON_VOLS								L"vol"
#define SCPS_JSON_VOLS_NAME							L"volName"
#define SCPS_JSON_VOLS_TYPE							L"volType"
#define SCPS_JSON_VOLS_RANGE_MAX					L"volRangeMax"
#define SCPS_JSON_VOLS_RANGE_MIN					L"volRangeMin"

#define S3MB_JSON_X								    L"x"
#define S3MB_JSON_Y							    	L"y"
#define S3MB_JSON_Z								    L"z"

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
		//从Json文件中加载属性值
		static bool AttributeDataFromJson(std::map<unsigned int, Feature*>& mapFeature, JsonValue& jsonValue);
		//从Json字符串中加载FeatureValue
		static bool GetFeatureValuesFromJson(Feature* pFeature, JsonValue& jsonValue);
		//读取Json文件
		static bool LoadLayerInfoFromJson(wstring strJson, vector<LayerCacheInfo>& vecLayerInfos);
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
