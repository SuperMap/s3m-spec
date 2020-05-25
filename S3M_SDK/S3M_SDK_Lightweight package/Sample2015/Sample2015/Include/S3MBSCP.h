#pragma once
#include "Point3D.h"
#include "Skeleton.h"
#include <map>
#include "S3MBLayerInfos.h"
#include <string>
#include <fstream>
using namespace std;
namespace S3MB
{
	const double PI180 = 1.74532925199432957692369076848e-2;
	const double ONEPI = 3.14159265358979323846;
	const double TWOPI = 6.2831853071795864769;
	enum SCPSDataType
	{
		SDT_Unknown,
		SDT_ObliquePhoto,
		SDT_BIM,
		SDT_PointCloud,
		SDT_Vector,
		SDT_PipeLine,
		SDT_InstanceModel
	};
	enum SCPSPyramidSplitType
	{
		SSP_Unknown,
		SSP_Octree,
		SSP_QuadTree
	};
	enum SCPSLODType
	{
		SLT_Unknown,
		SLT_Add,
		SLT_Replace,
	};
	struct ExtensionObjectValue
	{
		// 字符串
		string m_strName;
		string m_strType;
		// 双精度
		double m_dMax;
		double m_dMin;
	};
	enum PrjCoordSys
	{
		GCS_WGS_1984 = 4326,
	};
	class S3MBSCP
	{
	public:
		S3MBSCP(void);
		~S3MBSCP() {}
	public:
		void SetTiles(std::vector<string>& vecRootNames, std::vector<BoundingBox>& vecBBox);

		//! \brief 存储配置文件
		bool SaveToJsonFile(const string strS3MBProfile);
		//! \brief 构造Json对象
		void SaveToJson(JsonValue& jsonValue);

		bool Inverse(Point3D* pPoints);
		bool Forward(Point3D* pPoints);
		//! \brief 经度归算到(-PI--PI)
		double AdjLongitude(double lon);

		static string ToDataTypeString(const SCPSDataType enDataType);
		static SCPSDataType ToDataType(const string& strDataType);

		static string ToPyramidSplitTypeString(const SCPSPyramidSplitType enType);
		static SCPSPyramidSplitType ToPyramidSplitType(const string& strType);

		static string ToLODTypeString(const SCPSLODType enType);
		static SCPSLODType ToLODType(const string& strType);
	public:
		//! \brief 数据生产描述信息
		string m_strAsset;
		//! \brief 版本
		float m_fVersion;
		//! \brief 配置文件类型：倾斜、点云
		SCPSDataType m_enDataType;
		//! \brief 数据剖分方式
		SCPSPyramidSplitType m_enPyramidSplitType;
		//! \brief LOD数据类型：添加 or 替换
		SCPSLODType m_enLODType;

		//! \brief 整个缓存的地理范围
		Rect2D m_rcGeoBounds;
		//! \brief 最大高度
		double m_dbHeightMax;
		//! \brief 最小高度
		double m_dbHeightMin;
		//! \brief 位置
		Point3D m_pntPosition;
		//! \brief 坐标系信息
		int m_prjCoordEPSG;
		bool m_bHasPrj;

		//! \brief w位的含义
		string m_strWCategory;
		double m_dbWMax;
		double m_dbWMin;

		//! \brief 插入点是否为经纬度
		bool m_bIsDegree;

		//! \brief 切片信息
		std::vector<string> m_vecRootNames;
		std::vector<BoundingBox> m_vecBBox;

		//==============  扩展信息 ================
			//! \brief 拓展类
		vector<ExtensionObjectValue> m_arrExtensionObjValues;

		//! \brief 扩展信息 Tag <-> Value
		std::map<string, string> m_mapExtensions;

		//! \brief 外挂文件名字
		std::vector<string> m_vecAttachFile;

		//! \brief 点云图层名-bounds
		std::map<string, Rect2D> m_mapLayerNameRecBounds;

		//! \brief 点云图层名
		std::vector<string> m_vecPointCloudLayer;

		//! \brief 层级信息
		std::vector<int> m_vecGlobalLevel;
		//==============  扩展信息 ================
	};
}