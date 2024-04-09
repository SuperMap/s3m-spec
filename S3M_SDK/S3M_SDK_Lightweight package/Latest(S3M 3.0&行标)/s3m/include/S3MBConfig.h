#if !defined(S3MBConfig_H__D894A2D3_CDC8_4B18_BC13_96B71C8ABA83__INCLUDED_)
#define S3MBConfig_H__D894A2D3_CDC8_4B18_BC13_96B71C8ABA83__INCLUDED_

#pragma once
#include "BoundingBox.h"
#include "JsonValue.h"
#include "Point3D.h"
#include "S3MBCommon.h"

namespace S3MB
{
	class S3MB_API S3MBConfig
	{
	public:
		S3MBConfig(void);
		~S3MBConfig();

	public:
		bool ReadFile(const wstring& strFilePath);

		// 存储配置文件
		bool WriteFile(const wstring& strS3MBProfile);
	public:

		// 设置/获取描述信息
		void SetAsset(const wstring& strAsset);
		wstring GetAsset();

		// 设置/获取版本号
		void SetVersion(const float& version);
		float GetVersion();

		// 设置/获取数据剖分类型
		void SetSplitType(const SCPSPyramidSplitType& splitType);
		SCPSPyramidSplitType GetSplitType();

		// 设置/获取数据类型
		void SetDataType(const SCPSDataType& dataType);
		SCPSDataType GetDataType();

		// 设置/获取Lod切换类型
		void SetLodType(const SCPSLODType& lodType);
		SCPSLODType GetLodType();

		// 设置/获取Bounds
		void SetBounds(const Rect2D& bounds);
		Rect2D GetBounds();

		// 设置/获取最大高度
		void SetHeightMax(const double& dMax);
		double GetHeightMax();

		// 设置/获取最小高度
		void SetHeightMin(const double& dMin);
		double GetHeightMin();

		// 设置/获取位置
		void SetPosition(const Point3D& position);
		Point3D GetPosition();

		// 设置/获取坐标系epsg
		void SetEPSGCode(const int& nEpsg);
		int GetEPSGCode();

		// 设置/获取坐标系epsg
		void SetIsDegree(const bool& isDegree);
		bool GetIsDegree();

		// 设置/获取根节点信息
		void SetTiles(std::vector<wstring>& vecRootNames, std::vector<OrientedBoundingBox>& vecBBox);
		void GetTiles(std::vector<wstring>& vecRootNames, std::vector<OrientedBoundingBox>& vecBBox);

		// 设置/获取顶点属性含义描述信息和数值范围
		void SetVertexAttributeDescripts(std::vector<CategoryDescript>& vecDescript);
		void GetVertexAttributeDescripts(std::vector<CategoryDescript>& vecDescript);

		// 设置/获取扩展信息
		void SetExtension(const wstring& strKey, const wstring& strValue);
		bool HasExtension(const wstring& strKey);
		wstring GetExtension(const wstring& strKey);

	public:
		// 数据类型< - >字符串
		static wstring ToDataTypeString(const SCPSDataType& enDataType);
		static SCPSDataType ToDataType(const wstring& strDataType);
		// 剖分类型< - >字符串
		static wstring ToPyramidSplitTypeString(const SCPSPyramidSplitType& enType);
		static SCPSPyramidSplitType ToPyramidSplitType(const wstring& strType);
		// Lod切换类型< - >字符串
		static wstring ToLODTypeString(const SCPSLODType& enType);
		static SCPSLODType ToLODType(const wstring& strType);
		// 纹理类型< - >字符串
		static std::wstring FromTextureCompressionType(const TextureCompressType& nType);
		static TextureCompressType ToTextureCompressionType(const wstring& strType);
		// 顶点压缩类型< - >字符串
		static std::wstring FromVertexCompressionType(const S3MBVertexTag& eType);
		static S3MBVertexTag ToVertexCompressionType(const wstring& strType);

	private:
		// 构造Json对象
		void SaveToJson(JsonValue& jsonValue);
	private:
		// 数据生产描述信息
		wstring m_strAsset;
		// 版本
		float m_fVersion;
		// 配置文件类型：倾斜、点云
		SCPSDataType m_enDataType;
		// 数据剖分方式
		SCPSPyramidSplitType m_enPyramidSplitType;
		// LOD数据类型：添加 or 替换
		SCPSLODType m_enLODType;

		// 整个缓存的地理范围
		Rect2D m_rcGeoBounds;
		// 最大高度
		double m_dbHeightMax;
		// 最小高度
		double m_dbHeightMin;
		// 位置
		Point3D m_pntPosition;
		// 坐标系信息
		int m_prjCoordEPSG;
		bool m_bHasPrj;

		// w位的含义
		wstring m_strWCategory;
		double m_dbWMax;
		double m_dbWMin;

		// 插入点是否为经纬度
		bool m_bIsDegree;

		// 切片信息
		std::vector<wstring> m_vecRootNames;
		std::vector<BoundingBox> m_vecBBox;
		std::vector<OrientedBoundingBox> m_vecOBB;

		// ==============  扩展信息 ================
			// 拓展类
		vector<ExtensionObjectValue> m_arrExtensionObjValues;

		// 扩展信息 Tag <-> Value
		std::map<wstring, wstring> m_mapExtensions;

		// 点云图层名-bounds
		std::map<wstring, Rect2D> m_mapLayerNameRecBounds;

		// 点云图层名
		std::vector<wstring> m_vecPointCloudLayer;

		// ==============  扩展信息 ================

		// 顶点属性含义描述信息和数值范围
		std::vector<CategoryDescript> m_vecVertexAttributeDescript;
	};
}

#endif