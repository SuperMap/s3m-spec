//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcTextStyle.h
//!  \brief 文本风格类 
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCTEXTSTYLE_H__AD2CEE70_B618_484B_B4AD_ABF76D28DF21__INCLUDED_)
#define AFX_OGDCTEXTSTYLE_H__AD2CEE70_B618_484B_B4AD_ABF76D28DF21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base/OgdcString.h"

namespace OGDC{
//!  \brief 文本风格类 
class BASE_API OgdcTextStyle  
{

public:
	//! \brief 字体颜色
	OgdcColor	m_clrFore;
	//! \brief 背景色，阴影色，外框色
	OgdcColor	m_clrBack;
	//! \brief 旋转角度，以度为单位,x坐标正方向为起始，逆时针方向
	OgdcDouble	m_dAngle;	
	//! \brief 旋转角度，以度为单位,x坐标正方向为起始，逆时针方向
	OgdcDouble	m_dItalicAngle;	
	//! \brief 字体宽度，地理单位
	OgdcDouble	m_dWidth;
	//! \brief 字体高度，地理单位			
	OgdcDouble	m_dHeight;	
	//! \brief 笔划宽度， 100、200、300、400、500、600、700
	OgdcInt		m_nWeight;	
	//! 风格:粗体，斜体，透明，黑体,排列等
	OgdcInt		m_nStyle;
	//! \brief 对齐方式	0-11，值与对齐方式的关系如下图
	//BYTE1 文本的对齐方式
	//	   左  中  右
	// 上	0	1	2
	// 中	3	4	5
	// 下	6	7	8                 
	OgdcByte m_nAlign;
	//! \brief 是否粗体
	OgdcBool m_bBold;
	//! \brief 是否下划线
	OgdcBool m_bUnderline;	
	//! \brief 是否斜体
	OgdcBool m_bItalic;	
	//! \brief 是否删除线
	OgdcBool m_bStrikeOut;	
	//! \brief 是否固定大小
	OgdcBool m_bFixedSize;	
	//! \brief 是否背景不透明
	OgdcBool m_bBackOpaque;	
	//! \brief 是否显示外框
	OgdcBool m_bHalo;
	//! \brief 是否显示阴影
	OgdcBool m_bShadow;	
	//! \brief 字体名字			
	OgdcString	m_strFaceName;			
	
public:
	//! \brief 构造一个文本风格对象
	OgdcTextStyle();
    
	//! \brief 默认析构函数
	~OgdcTextStyle();
	//! \brief 拷贝构造函数
    OgdcTextStyle& operator = (const OgdcTextStyle& textStyle);

};


}

#endif // !defined(AFX_OGDCTEXTSTYLE_H__AD2CEE70_B618_484B_B4AD_ABF76D28DF21__INCLUDED_)

