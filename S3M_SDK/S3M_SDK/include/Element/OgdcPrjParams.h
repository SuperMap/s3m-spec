// OgdcPrjParams.h: interface for the OgdcPrjParams class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcPrjParams.h
//!  \brief 投影参数
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCPRJPARAMS_H__FAB41755_9ABC_4FF5_A375_7637A7E95C7D__INCLUDED_)
#define AFX_OGDCPRJPARAMS_H__FAB41755_9ABC_4FF5_A375_7637A7E95C7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base/ogdcdefs.h"

namespace OGDC {
//!  \brief 投影参数类
class ELEMENT_API OgdcPrjParams  
{
public:
	//!  \brief 投影参数默认构造函数
	OgdcPrjParams();
	//!  \brief 投影参数默认析构函数
	virtual ~OgdcPrjParams();
	//!  \brief 投影参数拷贝构造函数
	OgdcPrjParams(const OgdcPrjParams& prjParams);

public:
	//!  \brief 重载操作符 =
	OgdcPrjParams& operator=(const OgdcPrjParams& prjParams);
	//!  \brief 重载操作符 ==
	OgdcBool operator==(const OgdcPrjParams& prjParams);
	//!  \brief 重载操作符 !=
	OgdcBool operator!=(const OgdcPrjParams& prjParams);

public:
	//! \brief 投影方位角
	OgdcDouble m_dAzimuth;
	//! \brief 中央经线经度			
	OgdcDouble m_dCentralMeridian;
	//! \brief 中央纬线纬度
	OgdcDouble m_dCentralParallel;
	//! \brief 坐标东偏
	OgdcDouble m_dFalseEasting;	
	//! \brief 坐标北偏
	OgdcDouble m_dFalseNorthing;
	//! \brief 第一点经度
	OgdcDouble m_dFirstPointLongitude;
	//! \brief 第二点经度
	OgdcDouble m_dSecondPointLongitude;
	//! \brief 比例因子
	OgdcDouble m_dScaleFactor;
	//! \brief 第一标准纬线
	OgdcDouble m_dStandardParallel1;
	//! \brief 第二标准纬线
	OgdcDouble m_dStandardParallel2;	
	//! \brief 纠正角
	OgdcDouble m_dRectifiedAngle;
};

}

#endif // !defined(AFX_OGDCPRJPARAMS_H__FAB41755_9ABC_4FF5_A375_7637A7E95C7D__INCLUDED_)

