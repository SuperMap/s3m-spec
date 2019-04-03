// UGProjectionManager.h: interface for the UGProjectionManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_UGPROJECTIONMANAGER_H__3D504681_5B49_444D_8B4B_60443B570F96__INCLUDED_)
#define AFX_UGPROJECTIONMANAGER_H__3D504681_5B49_444D_8B4B_60443B570F96__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "./Toolkit/UGMarkup.h"

namespace UGC {

class PROJECTION_API UGProjectionManager  
{
public:
	static UGProjectionManager& GetInstance(); 

	UGProjectionManager();
	~UGProjectionManager();

public:	
	//! \brief 投影坐标系xml
	UGMarkup m_PrjMarkup;
	//! \brief 地理坐标系xml
	UGMarkup m_GeoMarkup;
	//! \brief 大地参考系xml
	UGMarkup m_HorizonMarkup;
	//! \brief 中央经线xml
	UGMarkup m_PrimeMarkup;
	//! \brief 地球椭球体xml
	UGMarkup m_SpheroidMarkup;
	//! \brief 投影方式xml
	UGMarkup m_ProjectionMarkup;

	//! \brief supermap 和 epsg 投影对照表
	UGMarkup m_PrjCodeMarkup;

};
//extern UGProjectionManager g_ProjectionManager;

}
#endif // !defined(AFX_UGPROJECTIONMANAGER_H__3D504681_5B49_444D_8B4B_60443B570F96__INCLUDED_)

