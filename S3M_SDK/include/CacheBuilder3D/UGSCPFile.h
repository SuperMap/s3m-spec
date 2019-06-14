#pragma once
#include "Toolkit/UGMarkup.h"
#include "Base3D/UGBoundingBox.h"
#include "Base3D/UGBoundingSphere.h"
#include "Projection/UGPrjCoordSys.h"

namespace UGC{

class CACHEBUILDER3D_API UGSCPFile
{
public:
	UGSCPFile(void);
	~UGSCPFile(void);
public:
	//! \brief strSCPFile是文件路径
	UGbool LoadFromFile(const UGString strSCPFile);
	//! \brief strSCPXML xml内容，不是路径
	UGbool Load(const UGString strSCPXML);
	UGbool Save(UGString& strSCPXML);

	UGbool SaveToFile(const UGString& strSCPPath);

	//! \brief 版本号
	UGdouble GetVersion();

	//! \brief 数据生产描述信息
	UGString GetAsset();
	void SetAsset(const UGString& strAsset);

	//! \brief 文件类型
	UGString GetFileType();
	void SetFileType(const UGString &strFileType);

	//! \brief RenderMode
	UGString GetRenderMode();
	
	//! \brief 空间剖分方式
	UGString GetTileSplitType();
	void SetTileSplitType(const UGString &strTileSplitType);

	//! \brief 数据切分方式：四叉树、八叉树
	UGString GetPyramidSplitType();
	void SetPyramidSplitType(const UGString &strPyramidType);

	std::map<UGdouble, UGString> GetScales();

	//! \brief 插入点
	UGPoint3D GetPosition();
	void SetPosition(const UGPoint3D & pnt);

	//! \brief 缓存的模型BBOX
	UGBoundingBox GetBBox();
	void SetBBox(UGBoundingBox &bbox);

	//! \brief 缓存的地理范围
	UGRect2D GetGeoBounds();
	void SetGeoBounds(UGRect2D &rcBounds);

	//! \brief 高度最小值
	UGdouble GetHeightMin();
	void SetHeightMin(UGdouble dValue);
	
	//! \brief 高度最大值
	UGdouble GetHeightMax();
	void SetHeightMax(UGdouble dValue);

	//! \brief W位特征值最小值
	UGdouble GetWCategoryMin();
	void SetWCategoryMin(UGdouble dValue);

	//! \brief W位特征值最大值
	UGdouble GetWCategoryMax();
	void SetWCategoryMax(UGdouble dValue);

	//! \brief 坐标系信息
	UGbool GetPrjCoords(UGPrjCoordSys& prjCoordj);
	void SetPrjCoords(UGPrjCoordSys& prjCoordj);

	//获取根节点名称
	std::vector<UGString> GetRootNames();
	void SetRootNames(const std::vector<UGString> &vecRootNames);
	//获取根节点包围球
	std::vector<UGBoundingSphere> GetBoundingSpheres();
	void SetBoundingSpheres(const std::vector<UGBoundingSphere>& vecBSpheres);

private:
	UGdouble m_dbVersion;
	//! \brief 数据生产描述信息
	UGString m_strAsset;

	UGString m_strFileType;
	UGString m_strPyramidSplitType;
	UGString m_strRenderMode;
	UGString m_strTileSplitType;

	std::map<UGdouble, UGString> m_mapScaleCaption;

	//! \brief 插入点
	UGPoint3D m_pntPosition;
	
	//! \brief 整个缓存的模型包围盒
	UGBoundingBox m_bbox;

	//! \brief 整个缓存的地理范围
	UGRect2D m_rcGeoBounds;

	//! \brief 最大高度
	UGdouble m_dbHeightMax;
	//! \brief 最小高度
	UGdouble m_dbHeightMin;

	//! \brief w位的含义
	UGdouble m_dbWMax;
	UGdouble m_dbWMin;

	//! \brief TileInfo(s)
	std::vector<UGString> m_vecRootNames;
	std::vector<UGBoundingSphere> m_vecBSpheres;	//x y z radius

	UGPrjCoordSys m_prjCoord;

	UGbool m_bHasPrj;
};

}