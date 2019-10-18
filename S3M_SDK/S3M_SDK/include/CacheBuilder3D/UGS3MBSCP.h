#pragma once
#include "Toolkit/UGMarkup.h"
#include "CacheBuilder3D/UGS3MBTags.h"
#include "Projection/UGPrjCoordSys.h"
#include "Base3D/UGBoundingSphere.h"
#include "FileParser/UGFileParseModel.h"
#include "Toolkit3D/UGJsonUtils.h"

//.scp 标准文件

namespace UGC{

class CACHEBUILDER3D_API UGS3MBSCP
{

public:
	UGS3MBSCP(void);
	~UGS3MBSCP(void);
public:
	//--------------------------升级---------------------------

	//! \brief 判断是否是JSON文件格式
	static UGbool IsJsonFile(UGString strScpFile);

	//! \brief 读取配置文件
	UGbool LoadFromJsonFile(const UGString strS3MBProfile);

	//! \brief 存储配置文件
	UGbool SaveToJsonFile(const UGString strS3MBProfile);
	//---------------------------------------------------------
	//! \brief strS3MBProfile 文件路径
	UGbool LoadFromFile(const UGString strS3MBProfile);
	
	//! \brief strS3MBProfile 文件路径，后缀名强制为scp
	UGbool SaveToFile(const UGString strS3MBProfile);

	//! \brief 转化为9D版本的scp文件
	UGbool ToSCPFile(const UGString strSCPPath);

	UGdouble GetVersion();
	void SetVersion(UGdouble dVersion);

	UGString GetAsset();
	void SetAsset(const UGString& strAsset);

	SCPSDataType GetDataType();
	void SetDataType(const SCPSDataType& enType);

	SCPSPyramidSplitType GetPyramidSplitType();
	void SetPyramidSplitType(const SCPSPyramidSplitType& enType);

	UGPoint3D GetPosition();
	void SetPosition(UGPoint3D& pnt);

	UGRect2D GetGeoBounds();
	void SetGeoBounds(UGRect2D & rcBounds);

	UGbool GetCoordSys(UGPrjCoordSys& prjCoord);
	void SetCoordSys(UGPrjCoordSys &coordsys);

	UGString GetWCategory();
	void SetWCategory(const UGString &strW);

	void GetTiles(std::vector<UGString>& vecRootNames, std::vector<UGBoundingBox>& vecBBox);
	void SetTiles(std::vector<UGString>& vecRootNames, std::vector<UGBoundingBox>& vecBBox);

	void AddExtension(const UGString &strTag, const UGString &strValue);
	UGString GetExtension(const UGString &strTag);

	void AddExtension(UGExtensionObjectValue & pObjValue);
	void GetExtension(UGArray<UGExtensionObjectValue> & arrExtensionObjValues);

	void GetAttachFile(std::vector<UGString>& vecAttachFile);
	void SetAttachFile(std::vector<UGString>& vecAttachFile);

	void GetPointCloudLayers(std::vector<UGString>& vecPointCloudLayers);
	void SetPointCloudLayers(std::vector<UGString>& vecPointCloudLayers);

	//! \brief 设置/获取高度最小值
	void SetHeightMin(UGdouble dValue);
	UGdouble GetHeightMin();

	//! \brief 设置/获取高度最大值
	void SetHeightMax(UGdouble dValue);
	UGdouble GetHeightMax();

	//! \brief 设置/获取W位特征值最小值
	void SetWCategoryMin(UGdouble dValue);
	UGdouble GetWCategoryMin();

	//! \brief 设置/获取W位特征值最大值
	void SetWCategoryMax(UGdouble dValue);
	UGdouble GetWCategoryMax();

	//! \brief 设置插入点是否为经纬度
	void SetIsDegree(UGbool bIsDegree);
	UGbool GetIsDegree();

	//! \brief 构造Markup对象
	void SaveToMarkup(UGMarkup& markup);

	//! \brief 设置/获取层级
	void SetGlobalLevels(std::vector<UGint>& vecGlobalLevel);
	void GetGlobalLevels(std::vector<UGint>& vecGlobalLevel);

private:
	//! \brief 枚举与字符串的转换函数
	static UGString ToDataTypeString(const SCPSDataType enDataType);
	static SCPSDataType ToDataType(const UGString &strDataType);

	static UGString ToPyramidSplitTypeString(const SCPSPyramidSplitType enType);
	static SCPSPyramidSplitType ToPyramidSplitType(const UGString &strType);

	static UGString ToLODTypeString(const SCPSLODType enType);
	static SCPSLODType ToLODType(const UGString &strType);

public:
	//! \brief 从Markup对象加载
	UGbool LoadFromMarkup(UGMarkup& markup);
	//------------------------------------------------
	//! \brief 构造Json对象
	void SaveToJson(UGJsonValue & jsonValue);

	//! \brief 从Json对象加载
	void LoadFormJson(UGJsonValue & jsonValue);
	//------------------------------------------------
private:
	//! \brief 数据生产描述信息
	UGString m_strAsset;
	//! \brief 版本
	UGfloat m_fVersion;
	//! \brief 配置文件类型：倾斜、点云
	SCPSDataType m_enDataType;
	//! \brief 数据剖分方式
	SCPSPyramidSplitType m_enPyramidSplitType;
	//! \brief LOD数据类型：添加 or 替换
	SCPSLODType m_enLODType;
	
	//! \brief 整个缓存的地理范围
	UGRect2D m_rcGeoBounds;
	//! \brief 最大高度
	UGdouble m_dbHeightMax;
	//! \brief 最小高度
	UGdouble m_dbHeightMin;
	//! \brief 位置
	UGPoint3D m_pntPosition;
	//! \brief 坐标系信息
	UGPrjCoordSys m_prjCoord;
	UGbool m_bHasPrj;

	//! \brief w位的含义
	UGString m_strWCategory;
	UGdouble m_dbWMax;
	UGdouble m_dbWMin;

	//! \brief 插入点是否为经纬度
	UGbool m_bIsDegree;

	//! \brief 切片信息
	std::vector<UGString> m_vecRootNames;
	std::vector<UGBoundingBox> m_vecBBox;

	//==============  扩展信息 ================
	//! \brief 拓展类
	UGArray<UGExtensionObjectValue> m_arrExtensionObjValues;

	//! \brief 扩展信息 Tag <-> Value
	std::map<UGString, UGString> m_mapExtensions;

	//! \brief 外挂文件名字
	std::vector<UGString> m_vecAttachFile;

	//! \brief 点云图层名
	std::vector<UGString> m_vecPointCloudLayer;

	//! \brief 层级信息
	std::vector<UGint> m_vecGlobalLevel;
	//==============  扩展信息 ================
};

}