#if !defined(S3MBLAYERINFOS_H__BD86AD5B_CB11_4378_9AE3_E5A4C2A9C82B__INCLUDED_)
#define S3MBLAYERINFOS_H__BD86AD5B_CB11_4378_9AE3_E5A4C2A9C82B__INCLUDED_

#pragma once
#include "JsonValue.h"
#include "S3MBCommon.h"
#include "Variant.h"

namespace S3MB
{
	class S3MB_API S3MBFieldInfo
	{
	public:
		S3MBFieldInfo();
		~S3MBFieldInfo();

		// 赋值
		S3MBFieldInfo& operator=(const S3MBFieldInfo& other);

	public:
		// 字段类型。
		FieldType m_nType;
		// 字段名称。
		wstring m_strName;
		// 字段的外键名。
		wstring m_strForeignName;
		// 字段长度。
		int m_nSize;
		// 是否是必填字段。
		bool m_bRequired;
		// 字段默认值
		wstring m_strDefaultValue;
	};

	class S3MB_API S3MBFieldInfos
	{
	public:
		// 默认构造函数
		S3MBFieldInfos();
		// 默认析构函数
		~S3MBFieldInfos();

		// 赋值
		S3MBFieldInfos& operator=(const S3MBFieldInfos& other);

		void add(S3MBFieldInfo newFieldInfo);
		size_t GetSize();
		S3MBFieldInfo GetAt(int nIndex);

		void RemoveAll();

	private:
		std::vector<S3MBFieldInfo> m_array;
	};

	class S3MB_API LayerCacheInfo
	{
	public:
		// 数据集信息
		wstring m_strDatasetName;
		// 字段信息
		S3MBFieldInfos m_fieldInfos;
		// ID范围
		std::pair<int, int> m_pairIDRange;
	};

	class S3MB_API Feature
	{
	public:
		void SetFieldInfos(S3MBFieldInfos& fieldInfos);

		// 根据字段序号设置字段值。
		// nIndex 字段序号[in]。
		// varValue 字段值 [in]。
		// return 是否设置成功，成功返回TRUE，失败返回FALSE。
		virtual bool SetValue(int nIndex, const Variant& varValue);

		// 根据字段序号获取字段值。
		// nIndex 字段序号[in]。
		virtual bool GetValue(int nIndex, Variant& varValue) const;

		// 根据字段名设置字段值。
		// strFieldName 字段名[in]。
		// varValue 字段值 [in]。
		// return 是否设置成功，成功返回TRUE，失败返回FALSE。
		virtual bool SetValue(const wstring& strFieldName, const Variant& varValue);

		// 根据字段名获取字段值。
		// strFieldName 字段名[in]。
		virtual bool GetValue(const wstring& strFieldName, Variant& varValue) const;

	public:
		// 特征要素所在矢量数据集表中的ID。
		int m_nID;
		vector<FieldDefine> m_fieldDefines;
		vector<unsigned char*> m_fieldValues;
	};

	class S3MB_API S3MBLayerInfos
	{
	public:
		S3MBLayerInfos(){}
		~S3MBLayerInfos();
	public:
		// 从s3md文件中加载属性值
		bool ReadLayerAttributeValueFromFile(const wstring& strAttFilePath);
		const std::map<unsigned int, Feature*>& GetLayerAttributeValue();
		// 从s3md数据流中加载属性值
		static bool ReadLayerAttributeValueFromStream(void* buffer, int bufferSize, std::map<unsigned int, Feature*>& mapFeature);
		// 属性数据保存成s3md文件
		static bool WriteLayerAttributeValueToFile(const std::map<unsigned int, std::pair<unsigned int, std::vector<Feature*> > >& mapFeature, std::wstring& strFilePath);

		// 读取Json文件,获取图层信息
		bool ReadLayerFieldInfosFromFile(wstring strJson);
		const vector<LayerCacheInfo>& GetLayerFieldInfos();
		// 读取Json数据流，获取图层信息
		static bool ReadLayerFieldInfosFromStream(void* buffer, int bufferSize, vector<LayerCacheInfo>& vecLayerInfos);
		// 属性信息保存成json文件
		static bool WriteLayerFieldInfosToFile(const std::vector<LayerCacheInfo>& vecLayerInfo, std::wstring& strFilePath);

	private:
		// 从Json文件中加载属性值
		static bool AttributeDataFromJson(std::map<unsigned int, Feature*>& mapFeature, JsonValue& jsonValue);
		// 从Json字符串中加载FeatureValue
		static bool GetFeatureValuesFromJson(Feature* pFeature, JsonValue& jsonValue);
		// 读取IDRange
		static void GetIDRangeFromJson(JsonValue& jsonValue, int& nMin, int& nMax);
		// mapFeature 里的ID是否在范围内
		static bool IsIDExisted(std::map<unsigned int, Feature*>& mapFeature, int nBeginID, int nEndID);
		// 读取FieldInfos
		static void GetFieldInfosFromJson(S3MBFieldInfos& fieldInfos, JsonValue& jsonValue);
		// 获取FieldInfoType
		static FieldType GetFieldInfoType(wstring strType);

		// 属性信息保存成json对象
		static void SaveLayerInfoToJSON(const std::vector<LayerCacheInfo>& vecLayerInfo, JsonValue& jsonValue);
		// ID范围保存成json对象
		static void SaveIDRangeToJSON(int nMin, int nMax, JsonValue& jsonValue);
		// 字段信息保存成json对象
		static void SaveFieldInfosToJSON(S3MBFieldInfos& fieldInfos, JsonValue& jsonValue);
		// 通过字段类型获取对应的字符串
		static std::wstring GetFieldInfoType(FieldType nType);

		// 属性数据保存成json对象
		static bool SaveAttributeDataToJSON(const std::map<unsigned int, std::pair<unsigned int, std::vector<Feature*> > >& mapFeature, JsonValue& jsonValue);
		// 字段信息保存成json对象
		static void SaveFieldDefinesToJSON(const std::vector<FieldDefine>& vecFieldDefine, JsonValue& jsonValue);
		// 要素的值保存成json对象
		static void SaveFeatureValuesToJSON(Feature* pFeature, JsonValue& jsonValue);

	private:
		vector<LayerCacheInfo> m_vecLayerInfos;
		std::map<unsigned int, Feature*> m_mapFeature;
	};
}

#endif