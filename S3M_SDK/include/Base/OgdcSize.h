//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)               
//                                                           
//!  \file OgdcSize.h
//!  \brief 二维矩形大小类。该类存储了矩形的长度cx和宽度cy，类型为OgdcInt型。
//!  \details   
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCSIZE_H__ED613AC5_ED5E_4B31_846E_3768AF9C4BA6__INCLUDED_)
#define AFX_OGDCSIZE_H__ED613AC5_ED5E_4B31_846E_3768AF9C4BA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base/ogdcdefs.h"

namespace OGDC {
//!  \brief 矩形大小类，存储了矩形的长度cx和宽度cy，为OgdcInt型。
class BASE_API OgdcSize  
{
public:
	//! \brief 		长度
	OgdcInt cx;
	//! \brief 		宽度
	OgdcInt cy;
public:
	
	//! \brief 		缺省构造函数
	OgdcSize();	

	//! \brief 		缺省析构函数
	~OgdcSize();	
	
	//! \brief 		带参构造函数
	//! \param 		nX		长度
	//! \param 		nY		宽度
	OgdcSize(OgdcInt nX,OgdcInt nY);
	
	//! \brief 		重载==
	friend BASE_API OgdcBool operator==(const OgdcSize& szSrc,const OgdcSize& szTag);
	
	//! \brief 		重载!=
	friend BASE_API OgdcBool operator!=(const OgdcSize& szSrc,const OgdcSize& szTag);	
	
	//! \brief 		重载 +=
	OgdcSize& operator+=(const OgdcSize& sz);
	
	//! \brief 		重载 -=
	OgdcSize& operator-=(const OgdcSize& sz);	
	
	//! \brief 		重载 -
	OgdcSize operator-() const;	
	
	//! \brief 		重载 +
	friend BASE_API OgdcSize operator+(const OgdcSize& szSrc,const OgdcSize& szTab);
	
	//! \brief 		重载 -
	friend BASE_API OgdcSize operator-(const OgdcSize& szSrc,const OgdcSize& szTab);
	
};
	
}

#endif // !defined(AFX_OGDCSIZE_H__ED613AC5_ED5E_4B31_846E_3768AF9C4BA6__INCLUDED_)

