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

	//! \brief TileInfo(s)
	std::vector<UGString> m_vecRootNames;
	std::vector<UGBoundingSphere> m_vecBSpheres;	//x y z radius

	UGPrjCoordSys m_prjCoord;

	UGbool m_bHasPrj;
};

}