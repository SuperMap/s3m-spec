//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcPoint3D.h
//!  \brief 3D坐标点 
//!  \details  
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCPOINT3D_H__4F7FE3AF_CB0E_4831_98B1_1512914807A3__INCLUDED_)
#define AFX_OGDCPOINT3D_H__4F7FE3AF_CB0E_4831_98B1_1512914807A3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base/OgdcSize3D.h"
#include "Base/OgdcPoint.h"
#include "Base/OgdcArray.h"

namespace OGDC {
//!  \brief 三维坐标点，OgdcDouble型，一般用来描述地理坐标的三维点。
class BASE_API OgdcPoint3D  
{
public:
    //! \brief 点的x轴坐标
	OgdcDouble x;
	//! \brief 点的y轴坐标
	OgdcDouble y;
	//! \brief 点的z轴坐标
	OgdcDouble z;
public:
	//! \brief  	默认构造函数
	OgdcPoint3D();
	//! \brief  	缺省析构函数
	~OgdcPoint3D();

	//! \brief  带参数构造函数
	OgdcPoint3D(OgdcDouble xx,OgdcDouble yy,OgdcDouble zz);	

public:	
	//! \brief 		重载 = 
	OgdcPoint3D& operator=(const OgdcPoint3D& s);
	//! \brief 		重载 += 
	OgdcPoint3D& operator+=(const OgdcPoint3D& p);
	//! \brief 		重载 -= 
	OgdcPoint3D& operator-=(const OgdcPoint3D& p);
	//! \brief 		重载 *= 
	OgdcPoint3D& operator*=(OgdcDouble c);
	//! \brief 		重载 /= 
	OgdcPoint3D& operator/=(OgdcDouble c);
	//! \brief 		重载 - 负号
	OgdcPoint3D operator-() const;
	//! \brief 		重载 == 
	friend BASE_API OgdcBool operator==(const OgdcPoint3D& p,const OgdcPoint3D& q);
	//! \brief 		重载 !=
	friend BASE_API OgdcBool operator!=(const OgdcPoint3D& p,const OgdcPoint3D& q);			
		
};
	
typedef OgdcArray<OgdcPoint3D> OgdcPoint3Ds;
}

#endif // !defined(AFX_OGDCPOINT3D_H__4F7FE3AF_CB0E_4831_98B1_1512914807A3__INCLUDED_)

