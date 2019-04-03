// OgdcElemFactory.h: interface for the OgdcElemFactory class.
//
//////////////////////////////////////////////////////////////////////
//                     ____    ____   ____     ____                    
//   Project          / _  \  / ___) |  _ \   / ___)                   
//                   | | | | | | __  | | \ | | |                       
//                   | |_| | | |_\ | | |_/ | | \___                    
//                    \____/  \____| |____/   \ ___)                
//                                                           
//!  \file OgdcElemFactory.h
//!  \brief 几何对象工厂
//!  \attention 
//!   Copyright(c) 2007-2012  SuperMap Software Co., Ltd.  <br> 
//!   All Rights Reserved.
//!  \version 1.0(2012)
//////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OGDCELEMFACTORY_H__244F3B3F_FFBD_4586_B5E1_ABCF70E26CA6__INCLUDED_)
#define AFX_OGDCELEMFACTORY_H__244F3B3F_FFBD_4586_B5E1_ABCF70E26CA6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "OgdcElement.h"

namespace OGDC {

//! \brief 回调函数，用于扩展对象绘制
//! \param 		pElement		外部要创建的扩展对象
//! \param 		nType		扩展对象的类型
//! \param 		pVoid			实现该回调函数对象的指针
typedef void (OGDCSTDCALL *CREATEELEMENTEXTENDCALLBACKPROC)
(OgdcElement** pElement, OgdcInt nType, void* pVoid);
//!  \brief 几何对象工厂类
class ELEMENT_API OgdcElemFactory  
{
public:
	//!  \brief 几何对象工厂类默认构造函数
	OgdcElemFactory();
	//!  \brief 几何对象工厂类默认析构函数
	virtual ~OgdcElemFactory();

public:
	//! \brief	创建指定类型的几何对象。
	//! \param	nType	几何对象类型。
	static OgdcElement* CreateElement(OgdcInt nType);

	//! \brief  克隆一个几何对象。
	//! \param  pElement 要被克隆的几何对象指针, 不能为NULL。
	static OgdcElement* CloneElement(const OgdcElement* pElement);

	//! \brief  拷贝一个几何对象。
	//! \param  pElement 原几何对象指针, 不能为NULL。[in]
	//! \param  pResElement 目标几何对象指针, 不能为NULL。[out]	
	static OgdcBool CopyElement(const OgdcElement* pElement, OgdcElement* pResElement);

};

}

#endif // !defined(AFX_OGDCELEMFACTORY_H__244F3B3F_FFBD_4586_B5E1_ABCF70E26CA6__INCLUDED_)

