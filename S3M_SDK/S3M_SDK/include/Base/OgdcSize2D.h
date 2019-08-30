//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcSize2D.h
//!  \brief 二维矩形大小类。该类存储了矩形的长度cx和宽度cy，类型为OgdcDouble型。
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCSIZE2D_H__C6D5EB89_6542_4FE3_9B39_C2B292F51D6D__INCLUDED_)
#define AFX_OGDCSIZE2D_H__C6D5EB89_6542_4FE3_9B39_C2B292F51D6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base/ogdcdefs.h"

namespace OGDC {
//!  \brief 二维矩形大小类。该类存储了矩形的长度cx和宽度cy，类型为OgdcDouble型。
class BASE_API OgdcSize2D  
{
public:
	//! \brief 		长度
	OgdcDouble cx;
	//! \brief 		宽度
	OgdcDouble cy;
public:
	
	//! \brief 		缺省构造函数
	OgdcSize2D();
	
	//! \brief 		缺省构造函数
	~OgdcSize2D();
	
	//! \brief 		带参构造函数
	//! \param 		cx		长度
	//! \param 		cy		宽度
	OgdcSize2D(OgdcDouble dX,OgdcDouble dY);
	
	//! \brief 		重载==
	friend BASE_API OgdcBool operator==(const OgdcSize2D& szSrc,const OgdcSize2D& sztag);
	
	//! \brief 		重载!=
	friend BASE_API OgdcBool operator!=(const OgdcSize2D& szSrc,const OgdcSize2D& szTag);	
	
	//! \brief 		重载 +=
	OgdcSize2D& operator+=(const OgdcSize2D& sz);
	
	//! \brief 		重载 -=
	OgdcSize2D& operator-=(const OgdcSize2D& sz);	
		
	//! \brief 		重载 -
	OgdcSize2D operator-() const;	
	
	//! \brief 		重载 +
	friend BASE_API OgdcSize2D operator+(const OgdcSize2D& szSrc,const OgdcSize2D& szTag);
	
	//! \brief 		重载 -
	friend BASE_API OgdcSize2D operator-(const OgdcSize2D& szSrc,const OgdcSize2D& szTag);	
};

}

#endif // !defined(AFX_OGDCSIZE2D_H__C6D5EB89_6542_4FE3_9B39_C2B292F51D6D__INCLUDED_)

