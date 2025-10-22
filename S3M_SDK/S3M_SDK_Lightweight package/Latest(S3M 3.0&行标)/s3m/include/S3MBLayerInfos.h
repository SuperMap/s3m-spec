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

		// ��ֵ
		S3MBFieldInfo& operator=(const S3MBFieldInfo& other);

	public:
		// �ֶ����͡�
		FieldType m_nType;
		// �ֶ����ơ�
		wstring m_strName;
		// �ֶε��������
		wstring m_strForeignName;
		// �ֶγ��ȡ�
		int m_nSize;
		// �Ƿ��Ǳ����ֶΡ�
		bool m_bRequired;
		// �ֶ�Ĭ��ֵ
		wstring m_strDefaultValue;
	};

	class S3MB_API S3MBFieldInfos
	{
	public:
		// Ĭ�Ϲ��캯��
		S3MBFieldInfos();
		// Ĭ����������
		~S3MBFieldInfos();

		// ��ֵ
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
		// ���ݼ���Ϣ
		wstring m_strDatasetName;
		// �ֶ���Ϣ
		S3MBFieldInfos m_fieldInfos;
		// ID��Χ
		std::pair<int, int> m_pairIDRange;
	};

	class S3MB_API Feature
	{
	public:
		void SetFieldInfos(S3MBFieldInfos& fieldInfos);

		// �����ֶ���������ֶ�ֵ��
		// nIndex �ֶ����[in]��
		// varValue �ֶ�ֵ [in]��
		// return �Ƿ����óɹ����ɹ�����TRUE��ʧ�ܷ���FALSE��
		virtual bool SetValue(int nIndex, const Variant& varValue);

		// �����ֶ���Ż�ȡ�ֶ�ֵ��
		// nIndex �ֶ����[in]��
		virtual bool GetValue(int nIndex, Variant& varValue) const;

		// �����ֶ��������ֶ�ֵ��
		// strFieldName �ֶ���[in]��
		// varValue �ֶ�ֵ [in]��
		// return �Ƿ����óɹ����ɹ�����TRUE��ʧ�ܷ���FALSE��
		virtual bool SetValue(const wstring& strFieldName, const Variant& varValue);

		// �����ֶ�����ȡ�ֶ�ֵ��
		// strFieldName �ֶ���[in]��
		virtual bool GetValue(const wstring& strFieldName, Variant& varValue) const;

	public:
		// ����Ҫ������ʸ�����ݼ����е�ID��
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
		// ��s3md�ļ��м�������ֵ
		bool ReadLayerAttributeValueFromFile(const wstring& strAttFilePath);
		const std::map<unsigned int, Feature*>& GetLayerAttributeValue();
		// ��s3md�������м�������ֵ
		static bool ReadLayerAttributeValueFromStream(void* buffer, int bufferSize, std::map<unsigned int, Feature*>& mapFeature);
		// �������ݱ����s3md�ļ�
		static bool WriteLayerAttributeValueToFile(const std::map<unsigned int, std::pair<unsigned int, std::vector<Feature*> > >& mapFeature, std::wstring& strFilePath);

		// ��ȡJson�ļ�,��ȡͼ����Ϣ
		bool ReadLayerFieldInfosFromFile(wstring strJson);
		const vector<LayerCacheInfo>& GetLayerFieldInfos();
		// ��ȡJson����������ȡͼ����Ϣ
		static bool ReadLayerFieldInfosFromStream(void* buffer, int bufferSize, vector<LayerCacheInfo>& vecLayerInfos);
		// ������Ϣ�����json�ļ�
		static bool WriteLayerFieldInfosToFile(const std::vector<LayerCacheInfo>& vecLayerInfo, std::wstring& strFilePath);

	private:
		// ��Json�ļ��м�������ֵ
		static bool AttributeDataFromJson(std::map<unsigned int, Feature*>& mapFeature, JsonValue& jsonValue);
		// ��Json�ַ����м���FeatureValue
		static bool GetFeatureValuesFromJson(Feature* pFeature, JsonValue& jsonValue);
		// ��ȡIDRange
		static void GetIDRangeFromJson(JsonValue& jsonValue, int& nMin, int& nMax);
		// mapFeature ���ID�Ƿ��ڷ�Χ��
		static bool IsIDExisted(std::map<unsigned int, Feature*>& mapFeature, int nBeginID, int nEndID);
		// ��ȡFieldInfos
		static void GetFieldInfosFromJson(S3MBFieldInfos& fieldInfos, JsonValue& jsonValue);
		// ��ȡFieldInfoType
		static FieldType GetFieldInfoType(wstring strType);

		// ������Ϣ�����json����
		static void SaveLayerInfoToJSON(const std::vector<LayerCacheInfo>& vecLayerInfo, JsonValue& jsonValue);
		// ID��Χ�����json����
		static void SaveIDRangeToJSON(int nMin, int nMax, JsonValue& jsonValue);
		// �ֶ���Ϣ�����json����
		static void SaveFieldInfosToJSON(S3MBFieldInfos& fieldInfos, JsonValue& jsonValue);
		// ͨ���ֶ����ͻ�ȡ��Ӧ���ַ���
		static std::wstring GetFieldInfoType(FieldType nType);

		// �������ݱ����json����
		static bool SaveAttributeDataToJSON(const std::map<unsigned int, std::pair<unsigned int, std::vector<Feature*> > >& mapFeature, JsonValue& jsonValue);
		// �ֶ���Ϣ�����json����
		static void SaveFieldDefinesToJSON(const std::vector<FieldDefine>& vecFieldDefine, JsonValue& jsonValue);
		// Ҫ�ص�ֵ�����json����
		static void SaveFeatureValuesToJSON(Feature* pFeature, JsonValue& jsonValue);

	private:
		vector<LayerCacheInfo> m_vecLayerInfos;
		std::map<unsigned int, Feature*> m_mapFeature;
	};
}

#endif