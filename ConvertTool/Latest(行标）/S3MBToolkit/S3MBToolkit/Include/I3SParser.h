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
		// 打开
		bool Open(const I3SParams& params);
		// 关闭
		bool Close();

		// 用户设置的参数
		const I3SParams& GetParams() const;
		// 获取图层信息的集合
		const std::vector<I3SLayerInfo>& GetLayerInfos() const;
		// 获取解析的参数的集合
		const std::map<std::wstring, I3SParseParams>& GetParseParams() const;
		// 获取s3m配置信息的集合
		const std::map<std::wstring, S3MBConfig>& GetS3MBConfigs() const;

		// 解析配置信息
		bool ParseConfig();

		// 解析点云节点索引信息
		static bool ParsePointCloudNodePages(SlpkFile& slpkFile, const I3SParseParams& parseParams, 
			std::map<std::wstring, I3SNodeInfo>& mapNodeInfo);
		static bool ParsePointCloudNodePage(SlpkFile& slpkFile, const I3SParseParams& parseParams, 
			const std::wstring& strName, std::map<std::wstring, I3SNodeInfo>& mapNodeInfo);
		// 解析节点文档
		static bool ParseNodeDoc(SlpkFile& slpkFile, const I3SParseParams& parseParams, 
			const std::wstring& strName, I3SNodeInfo& nodeInfo);
		// 解析共享信息
		static bool ParseSharedResource(SlpkFile& slpkFile, I3SNodeInfo& nodeInfo, I3SMaterialInfo& matInfo);

	private:
		// 解析元数据文件
		bool ParseMetadata(const std::wstring& strFileName, I3SMetadata& metaData);
		// 解析i3s 3dSceneLayer文件
		bool Parse3dSceneLayer(const std::wstring& strFileName, const std::wstring& strParentLayerType, const I3SMetadata& metaData);
		// 解析子图层(有实际几何信息)的标识
		bool ParseSublayerIds(JsonValue& jsonConfig, std::vector<std::wstring>& vecLayerId);
		// 解析图层信息
		bool ParseLayerInfo(JsonValue& jsonConfig, const std::wstring& strParentLayerType, const I3SMetadata& metaData);
		// 解析范围
		bool ParseFullExtent(JsonValue& jsonConfig, S3MBConfig& s3mbConfig);
		// 解析空间参考信息
		bool ParseSpatialReference(JsonValue& jsonConfig, I3SParseParams& parseParams, S3MBConfig& s3mbConfig);
		// 解析属性存储信息
		bool ParseAttributeStorageInfo(JsonValue& jsonConfig, I3SParseParams& parseParams);
		// 解析绘制信息
		bool ParseDrawingInfo(JsonValue& jsonConfig, I3SParseParams& parseParams);
		// 解析资源风格信息
		bool ParseResourcesStyles(const std::wstring& strName, I3SDrawingInfo& drawingInfo);
		// 解析资源风格的符号信息
		bool ParseResourcesStylesSymbolInfo(const std::wstring& strName, I3SDrawingInfo& drawingInfo);
		// 解析属性字段信息
		bool ParseFields(JsonValue& jsonConfig, I3SParseParams& parseParams);
		// 解析存储信息
		bool ParseStore(JsonValue& jsonConfig, const I3SMetadata& metaData, I3SParseParams& parseParams, S3MBConfig& s3mbConfig);
		// 解析根节点文档
		bool ParseRootNodeDoc(const std::wstring& strName, I3SParseParams& parseParams);
		// 组织点云根节点
		bool OrganizePointCloudRootNodes(const I3SMetadata& metaData, I3SParseParams& parseParams);
		// 解析纹理定义信息
		bool ParseTextureSetDefinitions(JsonValue& jsonConfig, I3SParseParams& parseParams);
		// 解析几何定义信息
		bool ParseGeometryDefinitions(JsonValue& jsonConfig, I3SParseParams& parseParams);

		// 解析obb
		static bool ParseOBB(JsonValue& jsonObb, OrientedBoundingBox& obb);
		// obb转成s3m obb
		static void ToOBB(const Vector3d& vCenter, const Vector3d& vHalf, const Vector4d& vQuaternion, OrientedBoundingBox& obb);
		// i3s layer type转成s3m data type
		static SCPSDataType ToDataType(const std::wstring& strLayerType);
		// i3s 渲染器类型 字符串转枚举
		static I3SDrawingInfo::I3SRendererType ToRendererType(const std::wstring& strType);
		// i3s 符号类型 字符串转枚举
		static I3SDrawingInfo::I3SSymbol3DType ToSymbolType(const std::wstring& strType);
		// 颜色混合模式 字符串转枚举
		static I3SDrawingInfo::I3SColorMixMode ToColorMixMode(const std::wstring& strMode);
		// 属性名称和属性值类型转成s3m属性字段信息
		static S3MBFieldInfo ToFieldInfo(const std::wstring& strFieldName, const std::wstring& strFieldType);
		// json值转成变体类型
		static Variant ToVariant(JsonValue& jsonValue, const std::wstring& strKey, FieldType nFieldType);
		// 转成给定类型的变体
		static Variant ToVariant(const Variant& var, FieldType nFieldType);

	private:
		// 用户设置的参数
		I3SParams m_I3SParams;
		// 图层信息的集合
		std::vector<I3SLayerInfo> m_vecI3SLayerInfo;
		// 解析的参数<layerId, params>
		std::map<std::wstring, I3SParseParams> m_mapI3SParseParams;
		// s3m配置信息<layerId, scp>
		std::map<std::wstring, S3MBConfig> m_mapS3MBConfig;
		// slpk文件
		SlpkFile m_slpkFile;
	};
}

#endif