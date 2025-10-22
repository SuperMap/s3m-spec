#if !defined(I3SPARSER_H__198A393A_6505_4720_A0C8_E36274A707A1__INCLUDED_)
#define I3SPARSER_H__198A393A_6505_4720_A0C8_E36274A707A1__INCLUDED_

#pragma once
#include "Common.h"
#include "I3SParams.h"
#include "S3MBConfig.h"
#include "SlpkFile.h"
#include "stdafx.h"

namespace S3MB
{
	class STK_API I3SParser
	{
	public:
		I3SParser();
		~I3SParser();

	public:
		// ��
		bool Open(const I3SParams& params);
		// �ر�
		bool Close();

		// �û����õĲ���
		const I3SParams& GetParams() const;
		// ��ȡͼ����Ϣ�ļ���
		const std::vector<I3SLayerInfo>& GetLayerInfos() const;
		// ��ȡ�����Ĳ����ļ���
		const std::map<std::wstring, I3SParseParams>& GetParseParams() const;
		// ��ȡs3m������Ϣ�ļ���
		const std::map<std::wstring, S3MBConfig>& GetS3MBConfigs() const;

		// ����������Ϣ
		bool ParseConfig();

		// �������ƽڵ�������Ϣ
		static bool ParsePointCloudNodePages(SlpkFile& slpkFile, const I3SParseParams& parseParams, 
			std::map<std::wstring, I3SNodeInfo>& mapNodeInfo);
		static bool ParsePointCloudNodePage(SlpkFile& slpkFile, const I3SParseParams& parseParams, 
			const std::wstring& strName, std::map<std::wstring, I3SNodeInfo>& mapNodeInfo);
		// �����ڵ��ĵ�
		static bool ParseNodeDoc(SlpkFile& slpkFile, const I3SParseParams& parseParams, 
			const std::wstring& strName, I3SNodeInfo& nodeInfo);
		// ����������Ϣ
		static bool ParseSharedResource(SlpkFile& slpkFile, I3SNodeInfo& nodeInfo, I3SMaterialInfo& matInfo);

	private:
		// ����Ԫ�����ļ�
		bool ParseMetadata(const std::wstring& strFileName, I3SMetadata& metaData);
		// ����i3s 3dSceneLayer�ļ�
		bool Parse3dSceneLayer(const std::wstring& strFileName, const std::wstring& strParentLayerType, const I3SMetadata& metaData);
		// ������ͼ��(��ʵ�ʼ�����Ϣ)�ı�ʶ
		bool ParseSublayerIds(JsonValue& jsonConfig, std::vector<std::wstring>& vecLayerId);
		// ����ͼ����Ϣ
		bool ParseLayerInfo(JsonValue& jsonConfig, const std::wstring& strParentLayerType, const I3SMetadata& metaData);
		// ������Χ
		bool ParseFullExtent(JsonValue& jsonConfig, S3MBConfig& s3mbConfig);
		// �����ռ�ο���Ϣ
		bool ParseSpatialReference(JsonValue& jsonConfig, I3SParseParams& parseParams, S3MBConfig& s3mbConfig);
		// �������Դ洢��Ϣ
		bool ParseAttributeStorageInfo(JsonValue& jsonConfig, I3SParseParams& parseParams);
		// ����������Ϣ
		bool ParseDrawingInfo(JsonValue& jsonConfig, I3SParseParams& parseParams);
		// ������Դ�����Ϣ
		bool ParseResourcesStyles(const std::wstring& strName, I3SDrawingInfo& drawingInfo);
		// ������Դ���ķ�����Ϣ
		bool ParseResourcesStylesSymbolInfo(const std::wstring& strName, I3SDrawingInfo& drawingInfo);
		// ���������ֶ���Ϣ
		bool ParseFields(JsonValue& jsonConfig, I3SParseParams& parseParams);
		// �����洢��Ϣ
		bool ParseStore(JsonValue& jsonConfig, const I3SMetadata& metaData, I3SParseParams& parseParams, S3MBConfig& s3mbConfig);
		// �������ڵ��ĵ�
		bool ParseRootNodeDoc(const std::wstring& strName, I3SParseParams& parseParams);
		// ��֯���Ƹ��ڵ�
		bool OrganizePointCloudRootNodes(const I3SMetadata& metaData, I3SParseParams& parseParams);
		// ������������Ϣ
		bool ParseTextureSetDefinitions(JsonValue& jsonConfig, I3SParseParams& parseParams);
		// �������ζ�����Ϣ
		bool ParseGeometryDefinitions(JsonValue& jsonConfig, I3SParseParams& parseParams);

		// ����obb
		static bool ParseOBB(JsonValue& jsonObb, OrientedBoundingBox& obb);
		// obbת��s3m obb
		static void ToOBB(const Vector3d& vCenter, const Vector3d& vHalf, const Vector4d& vQuaternion, OrientedBoundingBox& obb);
		// i3s layer typeת��s3m data type
		static SCPSDataType ToDataType(const std::wstring& strLayerType);
		// i3s ��Ⱦ������ �ַ���תö��
		static I3SDrawingInfo::I3SRendererType ToRendererType(const std::wstring& strType);
		// i3s �������� �ַ���תö��
		static I3SDrawingInfo::I3SSymbol3DType ToSymbolType(const std::wstring& strType);
		// ��ɫ���ģʽ �ַ���תö��
		static I3SDrawingInfo::I3SColorMixMode ToColorMixMode(const std::wstring& strMode);
		// �������ƺ�����ֵ����ת��s3m�����ֶ���Ϣ
		static S3MBFieldInfo ToFieldInfo(const std::wstring& strFieldName, const std::wstring& strFieldType);
		// jsonֵת�ɱ�������
		static Variant ToVariant(JsonValue& jsonValue, const std::wstring& strKey, FieldType nFieldType);
		// ת�ɸ������͵ı���
		static Variant ToVariant(const Variant& var, FieldType nFieldType);

	private:
		// �û����õĲ���
		I3SParams m_I3SParams;
		// ͼ����Ϣ�ļ���
		std::vector<I3SLayerInfo> m_vecI3SLayerInfo;
		// �����Ĳ���<layerId, params>
		std::map<std::wstring, I3SParseParams> m_mapI3SParseParams;
		// s3m������Ϣ<layerId, scp>
		std::map<std::wstring, S3MBConfig> m_mapS3MBConfig;
		// slpk�ļ�
		SlpkFile m_slpkFile;
	};
}

#endif