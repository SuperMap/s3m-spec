// OgdcElement.h: interface for the OgdcElement class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcElement.h
//!  \brief 几何对象基类
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMENT_H__D804932C_6A86_4E0B_8EE6_B114437AA9D4__INCLUDED_)
#define AFX_OGDCELEMENT_H__D804932C_6A86_4E0B_8EE6_B114437AA9D4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base/OgdcStyle.h"
#include "Base/OgdcRect2D.h"
#include "Base/OgdcArray.h"

namespace OGDC {

//!  \brief 几何对象基类
class ELEMENT_API OgdcElement  
{
public:
    //! \brief element类型
	//! \remarks 0-10000为内部预留，10000以上为用户扩展。
	enum Type
	{
		//! \brief 空
		ElemNull			=0,
		//! \brief 点
		ElemPoint			=1,
		//! \brief 多点
		ElemMultiPoint		=2,
		//! \brief 线
		ElemLine			=3,
		//! \brief 面
		ElemRegion			=5,
		//! \brief 文本
		ElemText			=7,
		//! \brief 矩形，支持倾斜
		ElemRect			=12,
		//! \brief 圆角矩形，支持倾斜
		ElemRectRound		=13,
		//! \brief 圆
		ElemCircle			=15,
		//! \brief 椭圆，支持倾斜
		ElemEllipse			=20,
		//! \brief 椭圆扇形面
		ElemPie				=21,
		//! \brief 椭圆弓形面
		ElemChord			=23,
		//! \brief 圆弧
		ElemArc				=24,
		//! \brief 椭圆弧
		ElemEllipticArc		=25,
		//! \brief Cardinal曲线,保证曲线一定通过控制点
		ElemCardinal		=27,
		//! \brief DGN curve，曲线
		ElemCurve			=28,
		//! \brief 三次均匀B样条曲线
		ElemBSpline			=29,
		//! \brief 刻度线（带线性坐标系的线）
		ElemLineM			=35,
		//! \brief 三维点
		ElemPoint3D			=101,
		//! \brief 三维多点
		ElemMultiPoint3D	=102,
		//! \brief 三维线
		ElemLine3D			=103,
		//! \brief 三维面
		ElemRegion3D		=105,
		//! \brief 复合对象
		ElemCompound		=1000,
		//! \brief 用户定义对象
		ElemUser			=1001,	
	};

public:
	//!  \brief 几何对象基类默认构造函数
	OgdcElement();
	//!  \brief 几何对象基类默认析构函数
	virtual ~OgdcElement();

public:
	//! \brief  得到Element类型。
	virtual OgdcInt GetType() const=0;

	//! \brief  得到对象风格维数，比如点0维、线1维、面2维、文本-1维。
	virtual OgdcInt GetStyleDimension() const=0;		

	//! \brief  得到对象的四至。
	virtual OgdcRect2D GetBounds();	

public:
	//! \brief 	几何对象ID，存取到数据时要保证唯一。
	OgdcInt m_nID;
	//! \brief  0维、1维、2维、-1维几何对象的风格维数。
	OgdcStyle* m_pStyle;
	//! \brief	对象的四至。
	OgdcRect2D m_rcBounds;			

};

}

#endif // !defined(AFX_OGDCELEMENT_H__D804932C_6A86_4E0B_8EE6_B114437AA9D4__INCLUDED_)

