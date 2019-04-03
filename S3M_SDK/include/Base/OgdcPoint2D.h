//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcPoint2D.h
//!  \brief 2D坐标点
//!  \details  
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCPOINT2D_H__8AEE7D11_E2F3_447D_AE4B_CC31AA76F2E0__INCLUDED_)
#define AFX_OGDCPOINT2D_H__8AEE7D11_E2F3_447D_AE4B_CC31AA76F2E0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base/OgdcSize2D.h"
#include "Base/OgdcPoint.h"
#include "Base/OgdcArray.h"

namespace OGDC {
//!  \brief 二维坐标点，OgdcDouble型，一般用来描述地理坐标的点
class BASE_API OgdcPoint2D  
{
public:
	//! \brief 点的横坐标
	OgdcDouble x;
	//! \brief 点的纵坐标
	OgdcDouble y;
public:
		
    //! \brief  	缺省构造函数
	OgdcPoint2D();
		
	//! \brief  	默认析造函数
	~OgdcPoint2D();
		
	//! \brief 		带参构造函数	
	//! \param 		dX			横坐标
	//! \param 		dY			纵坐标
   	OgdcPoint2D(OgdcDouble dX,OgdcDouble dY);
		
	//! \brief 		拷贝构造函数	
	//! \param 		sz			
	OgdcPoint2D(const OgdcSize2D& sz);
		
	//! \brief 		拷贝构造函数	
	//! \param 		sz			
	OgdcPoint2D(const OgdcPoint& pnt);
		
	//! \brief 		绕点旋转
	//! \param 		pntAnchor	中心点
	//! \param 		dRadian		旋转角度(弧度)	
	void Rotate(const OgdcPoint2D& pntAnchor, OgdcDouble dRadian);
       
	//! \brief 		绕点旋转
	//! \param 		pntAnchor	中心点
	//! \param 		dCos		余弦值
	//! \param 		dSin		正弦值
	void Rotate(const OgdcPoint2D& pntAnchor, OgdcDouble dCos, OgdcDouble dSin);
	
	//! \brief 		点偏移
	//! \return 	void
	//! \param 		szValue		偏移量 
	void Offset(const OgdcSize2D &szValue);

	//! \brief 		点偏移
	//! \param 		nOffsetX	横坐标偏移
	//! \param 		nOffsetY	纵坐标偏移
	void Offset(OgdcDouble dOffsetX, OgdcDouble dOffsetY);
	    
	//! \brief 		判断和当前点是否相等
	//! \param 		pntAnchor	要判断的点
	//! \param 		dTolerance	判断容限
	OgdcBool EQ(const OgdcPoint2D& pntAnchor, OgdcDouble dTolerance)const;
		
	//! \brief 		重载 == 
	//! \return 	返回是否相等
	friend BASE_API OgdcBool operator==(const OgdcPoint2D& pntStart,const OgdcPoint2D& pntEnd);
	
	//! \brief 		重载 !=		
	//! \return 	返回是否不相等
	friend BASE_API OgdcBool operator!=(const OgdcPoint2D& pntStart,const OgdcPoint2D& pntEnd);			
		
	//! \brief 		重载 = 	
	OgdcPoint2D& operator=(const OgdcSize2D& sz);
	
	//! \brief 		重载 = 	
	OgdcPoint2D& operator=(const OgdcPoint2D& pnt2);
		
	//! \brief 		重载 = 
	OgdcPoint2D& operator=(const OgdcPoint& pnt);
		
	//! \brief 		重载 += 
	OgdcPoint2D& operator+=(const OgdcPoint2D& pnt);
		
	//! \brief 		重载 += 
	OgdcPoint2D& operator+=(const OgdcSize2D& sz);
		
	//! \brief 		重载 -= 
	OgdcPoint2D& operator-=(const OgdcPoint2D& pnt);
        
	//! \brief 		重载 -= 
	OgdcPoint2D& operator-=(const OgdcSize2D& sz);	
		
	//! \brief 		重载 - 负号 
	OgdcPoint2D operator-() const;		
		
    //! \brief 		重载 + 
	friend BASE_API OgdcPoint2D operator+(const OgdcPoint2D& pntStart,const OgdcPoint2D& pntEnd);
		
	//! \brief 		重载 + 
	friend BASE_API OgdcPoint2D operator+(const OgdcPoint2D& pntStart,const OgdcSize2D& pntEnd);
		
    //! \brief 		重载 + 
	friend BASE_API OgdcPoint2D operator+(const OgdcSize2D& sz,const OgdcPoint2D& pnt);	
		
	//! \brief 		重载 - 
	friend BASE_API OgdcPoint2D operator-(const OgdcPoint2D& pntStart,const OgdcPoint2D& pntEnd);
		
    //! \brief 		重载 - 
	friend BASE_API OgdcPoint2D operator-(const OgdcPoint2D& pntStart,const OgdcSize2D& sz);
		
	//! \brief 		重载 - 
	friend BASE_API OgdcPoint2D operator-(const OgdcSize2D& sz,const OgdcPoint2D& pnt);	
				
	//! \brief 		重载 < 
	friend BASE_API OgdcBool operator<(const OgdcPoint2D& pntStart,const OgdcPoint2D& pntEnd);
		
};
	
typedef OgdcArray<OgdcPoint2D> OgdcPoint2Ds;
	
}


#endif // !defined(AFX_OGDCPOINT2D_H__8AEE7D11_E2F3_447D_AE4B_CC31AA76F2E0__INCLUDED_)

