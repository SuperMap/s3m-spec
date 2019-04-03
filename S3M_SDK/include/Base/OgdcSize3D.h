//////////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcSize3D.h
//!  \brief 3D大小类 
//!  \details   
//!  \attention 
//!   Copyright (c) 2007-2012 SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCSIZE3D_H__75C436AA_5720_4F3A_85BF_67F31C43FB28__INCLUDED_)
#define AFX_OGDCSIZE3D_H__75C436AA_5720_4F3A_85BF_67F31C43FB28__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Base/ogdcdefs.h"

namespace OGDC {
//!  \brief 长方体大小类，存储了长方体的长度cx、宽度cy和高度cz，为OgdcDouble型。
class BASE_API OgdcSize3D  
{
public:
	//! \brief 		长度
	OgdcDouble cx;
	//! \brief 		宽度
	OgdcDouble cy;
	//! \brief 		高度
	OgdcDouble cz;
public:
	
	//! \brief 		缺省构造函数
	OgdcSize3D();
	
	//! \brief 		缺省构造函数
	~OgdcSize3D();
	
	//! \brief 		带参构造函数
	//! \param 		cx		长度
	//! \param 		cy		宽度
	//! \param 		cz		高度
	OgdcSize3D(OgdcDouble dX,OgdcDouble dY,OgdcDouble dZ);
	
	//! \brief 		重载==
	friend BASE_API OgdcBool operator==(const OgdcSize3D& szSrc,const OgdcSize3D& sztag);
	
	//! \brief 		重载!=
	friend BASE_API OgdcBool operator!=(const OgdcSize3D& szSrc,const OgdcSize3D& szTag);	
	
	//! \brief 		重载 +=
	OgdcSize3D& operator+=(const OgdcSize3D& sz);
	
	//! \brief 		重载 -=
	OgdcSize3D& operator-=(const OgdcSize3D& sz);	
	
	//! \brief 		重载 -
	OgdcSize3D operator-();	
	
	//! \brief 		重载 +
	friend BASE_API OgdcSize3D operator+(const OgdcSize3D& szSrc,const OgdcSize3D& szTag);
	
	//! \brief 		重载 -
	friend BASE_API OgdcSize3D operator-(const OgdcSize3D& szSrc,const OgdcSize3D& szTag);	
};

}

#endif // !defined(AFX_OGDCSIZE3D_H__75C436AA_5720_4F3A_85BF_67F31C43FB28__INCLUDED_)

