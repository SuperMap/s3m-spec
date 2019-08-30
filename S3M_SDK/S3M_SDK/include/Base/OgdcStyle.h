//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcStyle.h
//!  \brief 几何对象风格类
//!  \details
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCSTYLE_H__9D5373BD_BA56_420C_8D25_C46E8D809C90__INCLUDED_)
#define AFX_OGDCSTYLE_H__9D5373BD_BA56_420C_8D25_C46E8D809C90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base/ogdcdefs.h"

namespace OGDC{
//! \brief 画刷类型
typedef enum tagBrushGradientMode
{
	//! \brief 一般类型
	scbGradientNone	    = 0,
	//! \brief 线型
	scbGradientLinear	= 1,
	//! \brief 射线状
	scbGradientRadial	= 2,
    //! \brief 圆锥形
	scbGradientConical	= 3,
    //! \brief 直线型
	scbGradientSquare	= 4,    
		
}OGDCBrushGradientMode;

//!  \brief 几何对象风格类
class BASE_API OgdcStyle  
{
public:	
	//! \brief 符号类型，类型定义在用户的符号库中
	OgdcInt    m_nMarkerStyle; 
    //! \brief 符号的旋转角度  单位：度
	OgdcDouble m_dMarkerAngle;
	//! \brief 符号大小   
	OgdcInt    m_nMarkerSize;
	//! \brief 符号宽度
	OgdcInt m_nMarkerWidth;
	//! \brief 符号高度
	OgdcInt m_nMarkerHeight;
	//! \brief 线型类型，类型定义在用户的线性库中
	OgdcInt    m_nLineStyle;  
	//! \brief 线型颜色 
	OgdcColor  m_clrLine;
	//! \brief 线型宽度
	OgdcInt    m_nLineWidth;  
	
	//! \brief 填充类型，类型定义在用户的填充库中
	OgdcInt    m_nFillStyle; 
	//! \brief 填充类型的背景色
	OgdcColor  m_clrFillBack; 
	//! \brief 填充类型的前景色
	OgdcColor  m_clrFillFore;  
    //! \brief 填充类型背景是否透明
	OgdcBool   m_bFillBackOpaque;
	//! \brief 填充类型的透明度
	OgdcByte   m_btFillOpaqueRate;
	//! \brief 填充的渐变类型
	OgdcByte   m_btFillGradientType;
	//! \brief 填充角度
	OgdcDouble m_dFillAngle;
	//! \brief 填充类型和中心位置的X偏移量	  
	OgdcShort  m_nFillCenterOffsetX;
	//! \brief 填充类型和中心位置的Y偏移量
	OgdcShort  m_nFillCenterOffsetY; 
	//! \brief 点符号的大小
	OgdcBool	m_bSymbolScale;	
public:

	//! \brief 默认构造函数
	OgdcStyle();
	//! \brief 默认析构函数
	~OgdcStyle();
	//! \brief 重载 =
	OgdcStyle& operator = (const OgdcStyle &style);

};

}

#endif // !defined(AFX_OGDCSTYLE_H__9D5373BD_BA56_420C_8D25_C46E8D809C90__INCLUDED_)

