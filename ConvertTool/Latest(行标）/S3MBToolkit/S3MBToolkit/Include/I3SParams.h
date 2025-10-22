#if !defined(I3SPARAMS_H__D631901E_B176_4024_A8C5_68D9C29BDA26__INCLUDED_)
#define I3SPARAMS_H__D631901E_B176_4024_A8C5_68D9C29BDA26__INCLUDED_

#pragma once
#include "Common.h"
#include "I3SFormatStructure.h"
#include "ProcessParams.h"
#include "S3MBLayerInfos.h"
#include "stdafx.h"

namespace S3MB
{
	// 用户设置的参数类
	class STK_API I3SParams : public ProcessParams
	{
	public:
		I3SParams();
		~I3SParams();

		virtual ProcessType GetProcessType();

	public:
		// 设置/获取源文件路径
		void SetInputPath(const std::wstring& strPath);
		std::wstring GetInputPath() const;

		// 设置/获取输出路径
		void SetOutputPath(const std::wstring& strPath);
		std::wstring GetOutputPath() const;

		// 设置/获取s3m版本号
		void SetS3MVersion(float fVersion);
		float GetS3MVersion() const;

		// 设置/获取数据名称
		void SetDataName(const std::wstring& strName);
		std::wstring GetDataName() const;

		// 设置/获取是否是经纬度坐标
		// 注意：必须设置的参数
		void SetIsLonLatCoordinates(bool bIs);
		bool GetIsLonLatCoordinates() const;

	private:
		// 源文件路径
		std::wstring m_strInputPath;
		// 输出路径
		std::wstring m_strOutputPath;
		// s3m版本号
		float m_fS3MVersion;
		// 数据名称
		std::wstring m_strDataName;
		// 是否是经纬度坐标
		bool m_bLonLatCoord;
	};

	// 解析的参数类
	class STK_API I3SParseParams
	{
	public:
		I3SParseParams();
		~I3SParseParams();

	public:
		// 设置/获取i3s版本号
		void SetI3SVersion(float fVersion);
		float GetI3SVersion() const;

		// 设置/获取根节点路径的数组
		void SetRootNodes(const std::vector<std::pair<std::wstring, OrientedBoundingBox> >& vecNode);
		std::vector<std::pair<std::wstring, OrientedBoundingBox> > GetRootNodes() const;

		// 设置/获取顶点属性和id的映射关系
		void SetVertexAttributeAndIdRelation(const std::vector<std::pair<std::wstring, int> >& vecRel);
		std::vector<std::pair<std::wstring, int> > GetVertexAttributeAndIdRelation() const;

		// 设置/获取顶点扩展属性和id的映射关系
		void SetVertexAttributeExtensionAndIdRelation(const std::vector<std::pair<std::wstring, int> >& vecRel);
		std::vector<std::pair<std::wstring, int> > GetVertexAttributeExtensionAndIdRelation() const;

		// 设置/获取是否有face属性
		void SetHasFaceAttributes(bool bHas);
		bool GetHasFaceAttributes() const;

		// 设置/获取纹理格式的映射关系
		void SetTextureFormats(const std::map<std::wstring, std::wstring>& mapFormat);
		std::map<std::wstring, std::wstring> GetTextureFormats() const;

		// 设置/获取顶点压缩类型
		void SetVertexCompressionType(S3MBVertexTag nType);
		S3MBVertexTag GetVertexCompressionType() const;

		// 设置/获取坐标系
		void SetEpsgCode(int nCode);
		int GetEpsgCode() const;
		bool GetHasEpsgCode() const;

		// 设置/获取图层在slpk里的路径
		void SetLayerPath(const std::wstring& strPath);
		std::wstring GetLayerPath() const;

		// 设置/获取输出路径
		void SetOutputPath(const std::wstring& strPath);
		std::wstring GetOutputPath() const;

		// 设置/获取绘制信息
		void SetDrawingInfo(const I3SDrawingInfo& info);
		I3SDrawingInfo GetDrawingInfo() const;
		bool GetHasDrawingInfo() const;

		// 设置/获取索引信息
		void SetIndexInfo(const I3SIndexInfo& info);
		I3SIndexInfo GetIndexInfo() const;

		// 设置/获取属性信息
		void SetAttributeInfos(const std::vector<I3SAttributeInfo>& vecInfo);
		std::vector<I3SAttributeInfo> GetAttributeInfos() const;
		void SetFieldInfos(const S3MBFieldInfos& infos);
		S3MBFieldInfos GetFieldInfos() const;

		// 设置/获取点云要素信息
		void SetAttStorageInfos(const std::vector<I3SAttStorageInfo>& vecInfo);
		std::vector<I3SAttStorageInfo> GetAttStorageInfos() const;

	private:
		// i3s版本号
		float m_fI3SVersion;
		// 根节点<路径,包围盒>的数组
		std::vector<std::pair<std::wstring, OrientedBoundingBox> > m_vecRootNode;
		// 顶点属性和id的映射关系(id用于draco压缩)
		std::vector<std::pair<std::wstring, int> > m_vecVertexAttAndId;
		// 顶点扩展属性和id的映射关系(id用于draco压缩)
		std::vector<std::pair<std::wstring, int> > m_vecVertexAttExtAndId;
		// 是否有face属性
		bool m_bHasFaceAttributes;
		// 纹理格式的映射关系
		std::map<std::wstring, std::wstring> m_mapTexFormat;
		// 顶点压缩类型
		S3MBVertexTag m_nVexCompType;
		// 坐标系
		int m_nEpsgCode;
		bool m_bHasEpsgCode;
		// 图层在slpk中的路径
		std::wstring m_strLayerPath;
		// 输出路径
		std::wstring m_strOutputPath;
		// 绘制信息
		I3SDrawingInfo m_drawingInfo;
		bool m_bHasDrawingInfo;
		// 索引信息
		I3SIndexInfo m_indexInfo;
		// 属性信息
		std::vector<I3SAttributeInfo> m_vecAttInfo;
		S3MBFieldInfos m_fieldInfos;
		// 点云要素信息
		std::vector<I3SAttStorageInfo> m_vecAttStorageInfo;
	};
}

#endif