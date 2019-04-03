//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcPoint.h
//!  \brief 坐标点
//!  \details  
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCPOINT_H__2388B98A_6C57_4E01_B775_57C22DD6CEC6__INCLUDED_)
#define AFX_OGDCPOINT_H__2388B98A_6C57_4E01_B775_57C22DD6CEC6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base/OgdcSize.h"
#include "Base/OgdcArray.h"

namespace OGDC {

class OgdcSize;
//!  \brief 二维坐标点，OgdcInt型，常用来表达屏幕坐标点或设备坐标点
class BASE_API OgdcPoint  
{
public:
	//! \brief 点的横坐标
	OgdcInt x;
	//! \brief 点的纵坐标
	OgdcInt y;
public:

	//! \brief 		缺省构造函数
	OgdcPoint();
	
	//! \brief 		默认析构函数
	~OgdcPoint();

	//! \brief 		带参构造函数	
	//! \param 		nX			横坐标
	//! \param 		nY			纵坐标		
	OgdcPoint(OgdcInt nX,OgdcInt nY);	

	//! \brief 		拷贝构造函数	
	//! \param 		sz					
	OgdcPoint(const OgdcSize& sz);

	//! \brief 		绕点旋转
	//! \param 		pntAnchor	中心点
	//! \param 		dRadian		顺时针旋转角度(弧度)	
	void Rotate(const OgdcPoint& pntAnchor, double dRadian);

	//! \brief 		绕点旋转
	//! \param 		pntAnchor	中心点
	//! \param 		dCos		余弦值
	//! \param 		dSin		正弦值	
	void Rotate(const OgdcPoint& pntAnchor, double dCos, double dSin);

	//! \brief 		点偏移
	//! \param 		szValue		偏移量	
	void Offset(const OgdcSize &szValue);

	//! \brief 		点偏移
	//! \param 		nOffsetX	横坐标偏移
	//! \param 		nOffsetY	纵坐标偏移	
	void Offset(OgdcInt nOffsetX, OgdcInt nOffsetY);

	//! \brief 		重载 == 
	//! \return 	返回pntStart与pntEnd的比较结果，相等返回true，不等返回false。
	friend BASE_API OgdcBool operator==(const OgdcPoint& pntStart,const OgdcPoint& pntEnd);

	//! \brief 		重载 != 
	//! \return 	返回pntStart与pntEnd的比较结果，不等返回true，相等返回false。	
	friend BASE_API OgdcBool operator!=(const OgdcPoint& pntStart,const OgdcPoint& pntEnd);			

	//! \brief 		重载 = 	
	OgdcPoint& operator=(const OgdcSize& sz);
	
	//! \brief 		重载 = 	
	OgdcPoint& operator=(const OgdcPoint& pnt);

	//! \brief 		重载 += 
	OgdcPoint& operator+=(const OgdcPoint& pnt);

	//! \brief 		重载 += 	
	OgdcPoint& operator+=(const OgdcSize& sz);

	//! \brief 		重载 -= 	
	OgdcPoint& operator-=(const OgdcPoint& pnt);

	//! \brief 		重载 -= 
	OgdcPoint& operator-=(const OgdcSize& sz);	

	//! \brief 		重载 -(取相反数) 
	//! \return 	OgdcPoint
	//! \remarks 	
	OgdcPoint operator-() const;		

	//! \brief 重载 <
	bool operator<(const OgdcPoint& pt) const;

	//! \brief 		重载 +
	friend BASE_API OgdcPoint operator+(const OgdcPoint& pntStart,const OgdcPoint& pntEnd);

	//! \brief 		重载 +	
	friend BASE_API OgdcPoint operator+(const OgdcPoint& pnt,const OgdcSize& sz);
	
	//! \brief 		重载 +	
	friend BASE_API OgdcPoint operator+(const OgdcSize& sz,const OgdcPoint& pnt);	
	
	//! \brief 		重载 -	
	friend BASE_API OgdcPoint operator-(const OgdcPoint& pntStart,const OgdcPoint& pntEnd);

	//! \brief 		重载 -
	friend BASE_API OgdcPoint operator-(const OgdcPoint& pnt,const OgdcSize& sz);

	//! \brief 		重载 -
	friend BASE_API OgdcPoint operator-(const OgdcSize& sz,const OgdcPoint& pnt);
	
	
};

typedef OgdcArray<OgdcPoint> OgdcPoints;
	
}

#endif // !defined(AFX_OGDCPOINT_H__2388B98A_6C57_4E01_B775_57C22DD6CEC6__INCLUDED_)

